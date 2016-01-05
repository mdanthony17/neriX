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
#include "COutputThread.h"
#include "CRunState.h"
#include "DataBuffer.h"
#include <string>
#include <Exception.h>
#include <ErrnoException.h>
#include <Globals.h>
#include <TclServer.h>

#include <assert.h>
#include <buffer.h>
#include <buftypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include <CRingBuffer.h>

#include <CRingStateChangeItem.h>
#include <CRingPhysicsEventCountItem.h>
#include <CRingScalerItem.h>
#include <CRingTextItem.h>

#include <sys/time.h>



using namespace std;

static const int MONITOR_STACK(7);

static DataBuffer* lastBuffer(0);
static const unsigned ReadoutStack(0);
static const unsigned ScalerStack(1);
static const unsigned MonitorStack(7);


///////////////////////////////////////////////////////////////////////
///////////////////// Local data types ////////////////////////////////
//////////////////////////////////////////////////////////////////////

typedef struct _BeginRunBuffer {
  BHEADER         s_header;
  struct ctlbody  s_body;
} BeginRunBuffer, *pBeginRunBuffer;

typedef struct _ScalerBuffer {
  BHEADER         s_header;
  struct sclbody  s_body;
} ScalerBuffer, *pScalerBuffer;

typedef struct _EventBuffer {
  BHEADER      s_header;
  uint16_t     s_body[1];
} EventBuffer, *pEventBuffer;

////////////////////////////////////////////////////////////////////////
//   mytimersub - since BSD timeval is not the same as POSIX timespec:
////////////////////////////////////////////////////////////////////////
static inline void 
mytimersub(timespec* minuend, timespec* subtrahend, timespec* difference)
{
  // We'll cheat and map these to timevals, use timersub and convert back:
  // this means we're only good to a microsecond not a nanosecond _sigh_
  // if this is not good enough we'll do the subtraction manually later.

  timeval m,s,d;
  m.tv_sec   = minuend->tv_sec;
  m.tv_usec  = minuend->tv_nsec/1000;

  s.tv_sec   = subtrahend->tv_sec;
  s.tv_usec  = subtrahend->tv_nsec/1000;

  timersub(&m, &s, &d);

  difference->tv_sec  = d.tv_sec;
  difference->tv_nsec = d.tv_usec * 1000;
}

////////////////////////////////////////////////////////////////////////
///////////////////// Construction and destruction /////////////////////
////////////////////////////////////////////////////////////////////////

/*!
   Create an outupt thread. 

   @param ring - Name of the ring buffer into which data will be put.


*/
COutputThread::COutputThread(std::string ring) : 
  m_nOutputBufferSize(0),		// Don't know yet.
  m_pBuffer(0),
  m_pCursor(0),
  m_nWordsInBuffer(0),
  m_ringName(ring),
  m_pRing(0)
{
  
}
/*!
  Destructor...actually this is probably not called as the thread lifetime
  is the lifetime of the program.  Regardless...do things cleanly.
*/
COutputThread::~COutputThread()
{
  delete m_pRing;		// close connection to ring.
  free(m_pBuffer);		// Free the event assembly buffer.
}

////////////////////////////////////////////////////////////////////////
////////////////////// Thread entry point... ///////////////////////////
////////////////////////////////////////////////////////////////////////

/* Adapt between nextgen and spectrodaq style threading models:
 */
void
COutputThread::run()
{
  
  operator()();
}


/*
   Thread entry point.  After attaching to the outputring,
   this is just an infinite buffer processing loop.
*/
int
COutputThread::operator()()
{
  // Main loop is pretty simple.
  try {
    attachRing();
    while(1) {
      
      DataBuffer& buffer(getBuffer());
      processBuffer(buffer);
      freeBuffer(buffer);
      
    }
  }
  catch (string msg) {
    cerr << "COutput thread caught a string exception: " << msg << endl;
    throw;
  }
  catch (char* msg) {
    cerr << "COutput thread caught a char* exception: " << msg << endl;
    throw;
  }
  catch (CException& err) {
    cerr << "COutputThread thread caught a daq exception: "
	 << err.ReasonText() << " while " << err.WasDoing() << endl;
    throw;
  }
  catch (...) {
    cerr << "COutput thread caught some other exception type.\n";
    throw;
  }
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////// Utility functions ///////////////////////////
/////////////////////////////////////////////////////////////////////////


/*
   Get a buffer from the event queue.  A reference to the buffer will
   be returned.

   Note that this will block if needed to wait for a buffer.
   note as well that between runs, we'll wind up blocking in here.

   @return DataBuffer&
   @retval reference to the buffer at the head of the buffer queue.

*/
DataBuffer&
COutputThread::getBuffer()
{
  DataBuffer* pBuffer = gFilledBuffers.get(); // Will block if needed.
  return *pBuffer;

}
/*
   Free a buffer that has been completely processed.
   This will return the buffer to the gFreeBuffers queue.

   @param buffer - Reference to the buffer to return to the free pool.

*/
void
COutputThread::freeBuffer(DataBuffer& buffer)
{
  gFreeBuffers.queue(&buffer);
}
/*
   Process a buffer from the reader.  At this time we are just going
   to figure out what type of buffer we have and dispatch accordingly.
   Buffers are as follows:

  -  Begin run is indicated by the type in the DataBuffer, all others look like
      data buffers.
  -  End run is indicated by the data buffer type.
  - All others are depend on the stack from which they came, and are handled
    by processEvent.

  @note begin and end buffers are generated by software not the VM-USB.

  @param buffer  - The buffer from the readout thread.
*/
void
COutputThread::processBuffer(DataBuffer& buffer)
{
  if (buffer.s_bufferType == TYPE_START) {
    startRun(buffer);
  }
  else if (buffer.s_bufferType == TYPE_STOP) {
    endRun(buffer);
  }
  
  else if (buffer.s_bufferType == TYPE_EVENTS) {
    processEvents(buffer);
  }
  else if (buffer.s_bufferType == TYPE_STRINGS) {
    pStringsBuffer pBody = reinterpret_cast<pStringsBuffer>(buffer.s_rawData);
    processStrings(buffer, *pBody);
  }
  else {
    // Unexpected buffer type is really bad throw:

    char exceptionText[1000];
    sprintf(exceptionText, "Unexpected buffer type %d encounterd", buffer.s_bufferType);
    throw std::string(exceptionText);
  }
}

/*
   Process a begin run pseudo buffer. I call this a psuedo buffer because
   there will not be any data from the VM_usb for this

   We must:
   - Update our concept of the run state.
   - Set the m_nOutputBufferSize accordingly.
   - Create and submit a state BEGIN_RUN State change item to the destination ring
     buffer
   
   @param buffer   - the buffer from the buffer queue.

   @throws CErrnoException if system calls fail.
   @throws std::string If CRingBufer operations fail.
*/
void
COutputThread::startRun(DataBuffer& buffer)
{

  time_t timestamp;

  m_nOutputBufferSize = Globals::usbBufferSize;

  if (time(&timestamp) == -1) {
    throw CErrnoException("Failed to get the time in COutputThread::startRun");
  }

  // Update our concept of run state.

  CRunState* pState = CRunState::getInstance();
  m_runNumber       = pState->getRunNumber();
  m_title           = pState->getTitle();

  clock_gettime(CLOCK_REALTIME, &m_startTimestamp);
  m_lastStampedBuffer = m_startTimestamp; // Last timestamped event...that is.
  m_elapsedSeconds = 0;
  
  m_nEventsSeen    = 0;

  CRingStateChangeItem begin(BEGIN_RUN,
			     m_runNumber,
			     0,
			     static_cast<uint32_t>(timestamp),
			     m_title);

  begin.commitToRing(*m_pRing);


}
/*
  Called when an end of run has occured. The end of run is pseudo buffer
  generated by software.   All we need to do is crate an END_RUN state change
  ring item and submit it to our target ring.

  @param buffer - Data buffer from the VM-USB.
  
  @throws CErrnoException if unable to get the timestamp for the end of the run.
  @throws std::string from CRingBuffer if the ring can't be manipulated.

*/
void
COutputThread::endRun(DataBuffer& buffer)
{

  free(m_pBuffer);
  m_pBuffer = 0;

  // Determine the absolute timestamp.

  time_t stamp;
  if (time(&stamp) == -1) {
    throw CErrnoException("Failed  to get the timestamp in COutputThread::endRun");
  }
  // Determine the run relative timestamp:
  // See Issue #423 - need to factor this.
 
  timespec microtime;
  clock_gettime(CLOCK_REALTIME, &microtime);
  timespec microdiff;
  mytimersub(&microtime, &m_startTimestamp, &microdiff);
  
  CRingStateChangeItem end(END_RUN,
			   m_runNumber,
			   microdiff.tv_sec,
			   stamp,
			   m_title);

  end.commitToRing(*m_pRing);
			   


}

/**
 * Process events in a buffer creating output buffers as required.
 *  - Figure out the used words in the buffer
 *  - For each event in the buffer invoke either event or scaler depending on
 *    the stack number.  Stack 1 is always a scaler event while any other stack
 *    is considered a physics event.
 *  - Stack 7 is the monitor stack and is sent to the Tcl server via a Tcl event
 *    sent to its event queue.
 *
 * @param inBuffer  -  Reference to a raw input buffer.
 */
static uint32_t  bufferNumber = 0; 
void 
COutputThread::processEvents(DataBuffer& inBuffer)
{
  uint16_t* pContents = inBuffer.s_rawData;
  int16_t  nEvents   = (*pContents) & VMUSBNEventMask;
  bool     continuous = ((*pContents) & VMUSBContinuous) != 0;
  bool     multibuffer = ((*pContents) & VMUSBMultiBuffer) != 0;

  bufferNumber++;



  pContents++;			// Point to first event.
  ssize_t    nWords    = (inBuffer.s_bufferSize)/sizeof(uint16_t) - 1; // Remaining words read.


  while (nWords > 0) {
    if (nEvents <= 0) {
      // Next long should be 0xffffffff buffer terminator:
      // I've seen this happen but it's not fatal...just go on to the next buffer.

      uint32_t* pNextLong = reinterpret_cast<uint32_t*>(pContents);
      if (*pNextLong != 0xffffffff) {
	cerr << "Ran out of events but did not see buffer terminator\n";
	cerr << nWords << " remaining unprocessed\n";
      }

      break;			// trusting event count vs word count(?).
    }

    // Pull the event length and stack number from the header:
    // event length is not self inclusive and is in uint16_t units.

    uint16_t header     = *pContents;
    size_t   eventLength = header & VMUSBEventLengthMask;
    uint8_t  stackNum   = (header & VMUSBStackIdMask) >> VMUSBStackIdShift;

    // Dispatch depending on the actual stack number.  The dispatch provided
    // allows for multiple event stacks (e.g. interrupt triggered stacks
    // that do different things).

    if (stackNum == ScalerStack) {
      scaler(pContents);
    }
    else if (stackNum == MonitorStack) {
      sendToTclServer(pContents);
    }
    else {
      event(pContents);
    }
    // Point at the next event and compute the remaining word and event counts.
    
    pContents += eventLength + 1; // Event count is not self inclusive.
    nWords    -= (eventLength + 1);
    nEvents--;
  }

  // I've seen the VM-USB hand me a bogus event count...but never a bogus
  // buffer word count.  This is non fatal but reported.

  if (nWords < 0) {
    cerr << "Warning used up more than the buffer  by " << (-nWords) << endl;
  }
}
/**
 * stringBuffer:
 *    Process a strings buffer.  Strings buffers contain a set of null terminated
 *    strings.  They are used to carry e.g. Control data buffers.
 *
 * @param buffer - Reference to the full data buffer.
 * @param strings The body of the data buffer already cst to a reference 
 *                to a StringsBuffer.
 */
void
COutputThread::processStrings(DataBuffer& buffer, StringsBuffer& strings)
{

  // Marshall the strings into a vector as expected by the CRingTextItem c-tor.

  std::vector<std::string> stringVector; 
  const char* pSrc = strings.s_strings;
  
  for (int i=0; i < strings.s_stringCount; i++) {
    stringVector.push_back(pSrc);
    pSrc += strlen(pSrc) + 1;	// +1 for the null terminator.
  }
  // Once we have a timestamp we're ready to go.

  time_t now = time(NULL);

  // Create and commit the item to the ring.

  CRingTextItem texts(strings.s_ringType,
		      stringVector,
		      m_elapsedSeconds, // best we can do for now.
		      now);
  texts.commitToRing(*m_pRing);

}


/**
 * Output a physics trigger count event item.  These are used to monitor
 * overall rates as well as to provide sampling statistics for sampling
 * consumers.
 *
 * @param runOffset - seconds intothe run at which this is being emitted.
 */
void
COutputThread::outputTriggerCount(uint32_t runOffset)
{
  CRingPhysicsEventCountItem item(m_nEventsSeen, runOffset);
  item.commitToRing(*m_pRing);
}

/**
 * Process a scaler event:
 * - Figure out the time interval start/stop times, and the absolute time.
 * - extract the vector of scalers from the VM-USB event.
 * - Create and submit the CRingScalerItem to the ring.
 *  
 * @note We assume scaler events won't consist of more than 2048
 *       scalers (e.g. the continuation bit is not set.

 * @param pData - Pointer to scaler data.
 *
 * @throw std::string - If a scaler buffer has a continuation segment.
 * @throw CErrnoException - If we can't get the absolute timestamp.
 * @throw std::string - From CRingBuffer if unable to commit the item to the ring.
 */
void
COutputThread::scaler(void* pData)
{


  time_t timestamp;
  if (time(&timestamp) == -1) {
    throw CErrnoException("COutputThread::scaler unable to get the absolute timestamp");
  }

  // Figure out where the scalers are and fetch the event header.

  uint16_t* pHeader = reinterpret_cast<uint16_t*>(pData);
  uint16_t  header  = *pHeader;
  uint32_t* pBody   = reinterpret_cast<uint32_t*>(pHeader+1); // Pointer to the scalers.

  // See Issue #424 - for now throw an error  if there's a continuation segment:

  if (header & VMUSBContinuation) {
    throw std::string("Scaler continuation segments are not supported yet.");
  }

  // Figure out how many wods/scalers there are:

  size_t        nWords   =  header & VMUSBEventLengthMask;
  size_t        nScalers =  nWords/(sizeof(uint32_t)/sizeof(uint16_t));

  // Marshall the scalers into an std::vector:

  std::vector<uint32_t> counterData;
  for (int i = 0; i < nScalers; i++) {
    counterData.push_back(*pBody++);
  }

  // The VM-USB does not timestamp scaler data for us at this time so we
  // are going to rely on the scaler period to be correct:

  uint32_t endTime = m_elapsedSeconds + Globals::scalerPeriod;

  // Output a ring count item using this time:

  outputTriggerCount(endTime);

  // Create the final scaler item and submit it to the ring.

  CRingScalerItem scalers(m_elapsedSeconds, endTime, timestamp, counterData);
  scalers.commitToRing(*m_pRing);
  m_elapsedSeconds = endTime;
}



/**
 * Create a new output buffer.
 * for now this is trivial
 *
 * @return uint8_t*
 * @retval Pointer to the output buffer.
 */
uint8_t* 
COutputThread::newOutputBuffer()
{
  return reinterpret_cast<uint8_t*>(malloc(m_nOutputBufferSize));
  
}
/**
 * Process a single event:
 * - If necessary create the event assembly buffer and initialize its
 *   cursor.
 * - Put the segment in the event assembly buffer.
 * - If there is a continuation segment we're done for now..as we'll get called again with the next
 *   segment
 * - If there is no continuation segment then we create and submit the output
 *   event to the ring and reset the cursor.
 *
 * @param pData - pointer to a VM-USB event segment.
 *
 * @throws std::string - event overflows the output buffer.
 * @throws std::string - Errors from the ring buffer classes.
 *
 * @note The data go in in native VM-USB format.  That's what the SpecTcl disassembler expects.
 */
void 
COutputThread::event(void* pData)
{
  // If necessary make an new output buffer

  if (!m_pBuffer) {
    m_pBuffer        = newOutputBuffer();
    m_pCursor        = m_pBuffer;
    m_nWordsInBuffer = 0;	  
  }

  // Initialize the pointers to event bits and pieces.

  uint16_t* pSegment = reinterpret_cast<uint16_t*>(pData);
  uint16_t  header   = *pSegment;

  // Figure out the header:
 
  size_t segmentSize = header & VMUSBEventLengthMask;
  bool   haveMore    = (header & VMUSBContinuation) != 0;
  
  // Events must currently fit in the buffer...otherwise we throw an error.

  segmentSize += 1;		// Size is not self inclusive
  if ((segmentSize + m_nWordsInBuffer) >= m_nOutputBufferSize/sizeof(uint16_t)) {
    int newSize          = 2*segmentSize*sizeof(uint16_t);
    uint8_t* pNewBuffer = reinterpret_cast<uint8_t*>(realloc(m_pBuffer, newSize));
    if (pNewBuffer) {
      m_pBuffer            = pNewBuffer;
      m_pCursor            = m_pBuffer + m_nWordsInBuffer * sizeof(uint16_t);
      m_nOutputBufferSize += newSize;
    } else {
      throw std::string("Failed to resize event buffer to fit an oversized segment");
    }


  }
  // Next we can copy our data to the output buffer and update the cursor
  // remembering that the size is not self inclusive:
  //
  memcpy(m_pCursor, pData, segmentSize*sizeof(uint16_t));
  m_nWordsInBuffer += segmentSize;
  m_pCursor += segmentSize*sizeof(uint16_t); // advance the cursor

  

  // If that was the last segment submit it and reset cursors and counters.

  if (!haveMore) {			    // Ending segment:
    CRingItem event(PHYSICS_EVENT, m_nWordsInBuffer*sizeof(uint16_t) + 100); // +100 really needed?

    // Put the data in the event and figure out where the end pointer is.

    void* pDest = event.getBodyPointer();
    memcpy(pDest, m_pBuffer, m_nWordsInBuffer*sizeof(uint16_t));
    uint8_t* pEnd = reinterpret_cast<uint8_t*>(pDest);
    pEnd += m_nWordsInBuffer*sizeof(uint16_t); // Where the new body cursor goes.

    event.setBodyCursor(pEnd);
    event.updateSize();
    event.commitToRing(*m_pRing);

    // Reset the cursor and word count in the assembly buffer:

    m_nWordsInBuffer = 0;
    m_pCursor        = m_pBuffer;
    
    m_nEventsSeen++;
  }

}


/**
 * Sends a control monitoring event (one from stack 7) to the 
 * control server. These events contain periodic monitoring data.
 * It's up to the Tcl server to assemble them into a single 
 * event (if they are split across a buffer boundary).
 * 
 * @param pEvent - Pointer to the event fragment.
 */
void
COutputThread::sendToTclServer(uint16_t* pEvent)
{


  // Locate the Tcl Server and queue the event:

  TclServer* pServer = Globals::pTclServer;
  pServer->QueueBuffer(pEvent);

  
}
/**
 * Attach to the ring buffer specified in m_ringName.  If that ring does not
 * yet exist, it is created.  A pointer to the created CRingBuffer object is
 * put in m_pRing
 *
 * @throws std::string if we can't attach to the ring or creation fails.
 *                     The actual exceptions are tossed from the bowels of
 *                     the CRingBuffer class.
 */
void
COutputThread::attachRing()
{
  m_pRing = CRingBuffer::createAndProduce(m_ringName);

}
