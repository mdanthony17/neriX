/*
    This software is Copyright by the Board of Trustees of Michigan
    State University (c) Copyright 2005.

    You may use this software under the terms of the GNU public license
    (GPL).  The terms of this license are described at:

     http://www.gnu.org/licenses/gpl.txt

     Author:
             Ron Fox
	     NSCL
	     Michigan State University
	     East Lansing, MI 48824-1321
*/

#include <config.h>
#include "CAcquisitionThread.h"
#include <CReadoutModule.h>
#include <CStack.h>
#include <CCCUSB.h>
#include <CCCUSBReadoutList.h>
#include <DataBuffer.h>
#include <CControlQueues.h>
#include "../router/CRunState.h" // else pick up the daq one by mistake.
#include <assert.h>
#include <time.h>
#include <string>
#include <Exception.h>
#include <Globals.h>
#include <CConfiguration.h>
#include <iostream>

#include <stdlib.h>

#include <string.h>
#include <errno.h>

using namespace std;


static const unsigned DRAINTIMEOUTS(5);	// # consecutive drain read timeouts before giving up.
static const unsigned USBTIMEOUT(10);

static const unsigned ReadoutStackNum(0);
static const unsigned ScalerStackNum(1);


// buffer types:
//


bool                CAcquisitionThread::m_Running(false);
CCCUSB*             CAcquisitionThread::m_pCamac(0);
CAcquisitionThread* CAcquisitionThread::m_pTheInstance(0);





/*!
   Construct the the acquisition thread object.
   This is just data initialization its the start member that is
   important.
*/

CAcquisitionThread::CAcquisitionThread() {

}
 
/*!
   Return the singleton instance of the acquisition thread object:

*/
CAcquisitionThread*
CAcquisitionThread::getInstance() 
{
  if (!m_pTheInstance) {
    m_pTheInstance =  new CAcquisitionThread;
  }
  return m_pTheInstance;
}

/*!
   Start the thread.
   - get an instance which, if necessary forces creation.
   - Set the adc/scaler lists.
   - Initiate the thread which gets the ball rolling.
   \param usb    :CCCUSB*
      Pointer to the vme interface object we use to deal with all this stuff.


*/
void
CAcquisitionThread::start(CCCUSB* usb)

{
  CRunState* pState = CRunState::getInstance();
  pState->setState(CRunState::Active);


  CAcquisitionThread* pThread = getInstance();
  m_pCamac = usb;



  // starting the thread will eventually get operator() called and that
  // will do all the rest of the work in thread context.

  getInstance()->Thread::start();
  
}

/*!
    Returns true if the thread is running.
*/
bool
CAcquisitionThread::isRunning()
{
  return m_Running;
}

/*!
   Wait for the thread to exit.
*/
void
CAcquisitionThread::waitExit()
{
  getInstance()->join();
}

/**
 * Bridge between new and old threading model.
 */
void
CAcquisitionThread::init()
{
  try {
    beginRun();			// Emit begin run buffer.
    startDaq();  		// Setup and start data taking.
    m_Running = true;		// Thread is off and running now.    
  }
  catch (string msg) {
    cerr << "CAcquisition thread caught a string exception: " << msg << endl;
  }
  catch (char* msg) {
    cerr << "CAcquisition thread caught a char* exception: " << msg << endl;
  }
  catch (CException& err) {
    cerr << "CAcquisition thread caught a daq exception: "
	 << err.ReasonText() << " while " << err.WasDoing() << endl;
  }
  catch (...) {
    cerr << "CAcquisition thread caught some other exception type.\n";
  }

}

/*!
   Entry point for the thread.
*/
void
CAcquisitionThread::operator()()
{
  try {
    
    mainLoop();			// Enter the main processing loop.
  }
  catch (...) {			// exceptions are used to exit the main loop.?
  }
  
  endRun();			// Emit end run buffer.
  m_Running = false;		// Exiting.

}


/*!
  The main loop is simply one that loops:
  - Reading a buffer from the vm-usb and processing it if one comes in.
  - Checking for control commands and processing them if they come in.
*/
void
CAcquisitionThread::mainLoop()
{
  DataBuffer*     pBuffer   = gFreeBuffers.get();
  CControlQueues* pCommands = CControlQueues::getInstance(); 
  try {
    while (true) {
      
      // Event data from the VM-usb.
      if (m_Running) {
	size_t bytesRead;
	int status = m_pCamac->usbRead(pBuffer->s_rawData, pBuffer->s_storageSize,
				       &bytesRead,
				       USBTIMEOUT*1000 );
	if (status == 0) {
	  pBuffer->s_bufferSize = bytesRead;
	  pBuffer->s_bufferType   = TYPE_EVENTS;
	  processBuffer(pBuffer);	// Submitted to output thread so...
	  pBuffer = gFreeBuffers.get(); // need a new one.
	} 
	else {
	  if (errno != ETIMEDOUT) {
	    cerr << "Bad status from usbread: " << strerror(errno) << endl;
	    cerr << "Ending the run .. check CAMAC crate.  If it tripped off ";
	    cerr << " you'll need to restart this program\n";
	    throw 1;
	  }
	}
      // Commands from our command queue.
      }
      CControlQueues::opCode request;
      bool   gotOne = pCommands->testRequest(request);
      if (gotOne) {
	processCommand(request);
      }
    }
  }
  catch (...) {
    gFreeBuffers.queue(pBuffer);	// Don't lose buffers!!
    throw;
  }
}
/*!
  Process a command from our command queue.  The command can be one of the
  following:
  ACQUIRE   - The commander wants to acquire the VM-USB we must temporarily
              shutdown data taking and drain the VMusb...then ack the message
              and wait paitently for a RELEASE before restarting.
  PAUSE     - Commander wants the run to pause.
  END       - The commander wants the run to end.
*/
void
CAcquisitionThread::processCommand(CControlQueues::opCode command)
{
  CControlQueues* queues = CControlQueues::getInstance();

  if (command == CControlQueues::ACQUIRE) {
    stopDaq();
    queues->Acknowledge();
    CControlQueues::opCode release  = queues->getRequest();
    assert(release == CControlQueues::RELEASE);
    queues->Acknowledge();
    CCusbToAutonomous();
  }
  else if (command == CControlQueues::END) {
    if (m_Running) {
      stopDaq();
    }
    queues->Acknowledge();
    throw "Run ending";
  }
  else if (command == CControlQueues::PAUSE) {
    pauseDaq();

  }
  else {
    // bad error:

    assert(0);
  }
}
/*!
   Process a buffer of data from the VM-USB (not artificially generated
   buffers.  There are only two types of buffers that can come from the
   VM-USB:
   - Event data
   - Scalers
   These are distinguishable from the Scaler bit in the header word of the
   data from the VM-USB.
   \param buffer : DataBuffer*
      the buffer of data from the VM-USB.  The following fiels have been set:
      - s_storageSize  : number of bytes of physical storage in a buffer. 
      - s_bufferType.
      - s_buffersSize  : number of bytes read from the USB.
      - s_rawData      : the data read from the USB.

   The disposition of full buffers is done by another thread that is connected
   to us by a buffer queue.  Once we fill in the buffer type we just have to
   submit the buffer to the queue.. which can wake up the output thread and
   cause a scheduler pass... or not.

*/
void
CAcquisitionThread::processBuffer(DataBuffer* pBuffer)
{
  time_t acquiredTime;		// Need to generate  our own timestamps.
  time(&acquiredTime);
  pBuffer->s_timeStamp  = acquiredTime;

  // In this version, all stack ids are good.  The output thread will ensure that
  // stack 1 completions are scalers and all others are events.

  gFilledBuffers.queue(pBuffer);	// Send it on to the output thread.
}
/*!
    startDaq start data acquisition from a standing stop. To do this we need to:
    - Emit a begin run buffer.
    - Create VMUSBReadoutLists from the m_scalers and m_adcs
    - Download those lists into the VM-USB, list 2 will be used for event readout.
      list 1 for scalers.
    - Set the trigger for list 2 to be interrupt number 1 with vmeVector and
      vmeIPL as described in the static consts in this module
      \bug May want to make this parameterizable, but probably not necessary.
    - Set the buffersize to 13k (max). to get optimum throughput.
    - Setup scaler readout to happen every 10 seconds.
    - Initialize the hardware
    - Start USB data acuisition.

*/
void
CAcquisitionThread::startDaq()
{

  m_pCamac->writeActionRegister(CCCUSB::ActionRegister::clear);


  uint32_t fware;
  int status;
  while((status = m_pCamac->readFirmware(fware)) != 0) {
    cerr << "Attempting firmware register read\n";
  }
  if (status < 0) {
    cerr << "Could not even read the module firmware status: " << status 
	 << " errno: " << errno <<endl;
    exit(status);
  }
  // Process the configuration. This must be done in a way that preserves the
  // Interpreter since loadStack and Initialize for each stack will need the
  // interpreter for our support of tcl drivers.

  Globals::pConfig = new CConfiguration;
  Globals::pConfig->processConfiguration(Globals::configurationFilename);
  std::vector<CReadoutModule*> Stacks = Globals::pConfig->getStacks();

  cerr << "CCUSB located firmware revision: " << hex << fware << dec << endl;

  // Set up the default  ouptuts, 
  //  NIM 01  - Busy.
  //  NIM 02  - Acquire
  //  NIM 03  - end of busy.


    m_pCamac->writeOutputSelector(CCCUSB::OutputSourceRegister::nimO1Busy |
  				CCCUSB::OutputSourceRegister::nimO2Acquire |
  				CCCUSB::OutputSourceRegister::nimO3BusyEnd);

  // Set up the buffer size and mode:
  // don't want multibuffering...1sec timeout is fine.

  m_pCamac->writeUSBBulkTransferSetup(0 << CCCUSB::TransferSetupRegister::timeoutShift);


  // The global mode:
  //   4kword buffer
  //   Single event seperator.
  //   Single header word.
  //
  m_pCamac->writeGlobalMode((CCCUSB::GlobalModeRegister::bufferLen4K << CCCUSB::GlobalModeRegister::bufferLenShift));


  // The CCUSB has two stacks to load; an event stack and a scaler stack.
  // though the loop below makes you believe it might have an arbitrary number...
  // it still should work.

  cerr << "Loading " << Stacks.size() << " Stacks to cc-usb\n";
  for(int i =0; i < Stacks.size(); i++) {
    CStack* pStack = dynamic_cast<CStack*>(Stacks[i]->getHardwarePointer());
    assert(pStack);
    pStack->Initialize(*m_pCamac);    // INitialize daq hardware associated with the stack.
    pStack->loadStack(*m_pCamac);     // Load into CC-USB .. The stack knows if it is event or scaler
    pStack->enableStack(*m_pCamac);   // Enable the trigger logic for the stack.
  }

  CCusbToAutonomous();

}
/*!
   Stop data taking this involves:
   - Forcing a scaler trigger (action register write)
   - Setting clearing the DAQ start bit (action register write)
   - draining data from the VMUSB:
*/
void
CAcquisitionThread::stopDaq()
{
  m_pCamac->writeActionRegister(CCCUSB::ActionRegister::scalerDump);


  drainUsb();
}
/*!
  Pause the daq. This means doing a stopDaq() and fielding 
  requests until resume or stop was sent.

*/
void
CAcquisitionThread::pauseDaq()
{
  CControlQueues* queues = CControlQueues::getInstance();
  stopDaq();
  CRunState* pState = CRunState::getInstance();
  pState->setState(CRunState::Paused);
  queues->Acknowledge();

  while (1) {
    CControlQueues::opCode req = queues->getRequest();
    
    // Acceptable actions are to acquire the USB, 
    // End the run or resume the run.
    //

    if (req == CControlQueues::ACQUIRE) {
      queues->Acknowledge();
      CControlQueues::opCode release = queues->getRequest();
      assert (release == CControlQueues::RELEASE);
      queues->Acknowledge();
    }
    else if (req == CControlQueues::END) {
      queues->Acknowledge();
      pState->setState(CRunState::Idle);
      throw "Run Ending";
    }
    else if (req == CControlQueues::RESUME) {
      startDaq();
      queues->Acknowledge();
      pState->setState(CRunState::Active);
      return;
    }
    else {
      assert(0);
    }
  }
  
}
/*!
   Turn on Data taking this is just a write of CCCUSB::ActionRegister::startDAQ
   to the action register
*/
void
CAcquisitionThread::CCusbToAutonomous()
{
  m_pCamac->writeActionRegister(CCCUSB::ActionRegister::startDAQ);
}
/*!
  Drain usb - We read buffers from the DAQ (with an extended timeout)
  until the buffer we get indicates it was the last one (data taking turned off).
  Each buffer is processed normally.
*/
void
CAcquisitionThread::drainUsb()
{
  bool done = false;
  DataBuffer* pBuffer = gFreeBuffers.get();
  int timeouts(0);
  size_t bytesRead;
  cerr << "CAcquisitionThread::drainUsb...\n";
  do {
    int    status = m_pCamac->usbRead(pBuffer->s_rawData, pBuffer->s_storageSize,
				    &bytesRead, 
				    DRAINTIMEOUTS*1000); // 5 second timeout!!
    if (status == 0) {
      pBuffer->s_bufferSize = bytesRead;
      pBuffer->s_bufferType   = TYPE_EVENTS;
      cerr << "Got a buffer, with type header: " << hex << pBuffer->s_rawData[0] << endl;
      if (pBuffer->s_rawData[0] & CCUSBLastBuffer) {
	cerr << "Done\n";
	done = true;
      }
      processBuffer(pBuffer);
      pBuffer = gFreeBuffers.get();
    }
    else {
      timeouts++;		// By the time debugged this is only failure.
      cerr << "Read timed out\n";
      if(timeouts >= DRAINTIMEOUTS) {
	cerr << "Warning: drainUsb() persistent timeout assuming already drained\n";
	uint32_t junk;
	cerr << "Desparate measures being employed to attempt final drain\n";
	m_pCamac->writeActionRegister(CCCUSB::ActionRegister::clear);
	m_pCamac->writeActionRegister(0);
	usleep(100);
	status = m_pCamac->usbRead(pBuffer->s_rawData, pBuffer->s_storageSize,
				 &bytesRead, DRAINTIMEOUTS*1000);
	cerr << "Final desparate attempt to flush usb fifo got status: " 
	     << status << endl;
	done = true;
      }
    }
  } while (!done);


  gFreeBuffers.queue(pBuffer);
  cerr << "Done finished\n";
  
}
/*!
   Emit a begin run buffer to the output thread.
   the key info in this buffer is just its type and timestamp.
   No info at all is in the body, however since the buffer will be returned
   to the free list we have to get it from there to begin with:
*/
void
CAcquisitionThread::beginRun()
{
  DataBuffer* pBuffer   = gFreeBuffers.get();
  pBuffer->s_bufferSize = pBuffer->s_storageSize;
  pBuffer->s_bufferType = TYPE_START;
  processBuffer(pBuffer);	// Rest gets taken care of there.
}
/*!
   Emit an end of run buffer.
   This is just like a begin run buffer except for the buffer type:
*/
void
CAcquisitionThread::endRun()
{
  DataBuffer* pBuffer   = gFreeBuffers.get();
  pBuffer->s_bufferSize = pBuffer->s_storageSize;
  pBuffer->s_bufferType = TYPE_STOP;
  processBuffer(pBuffer);
} 

