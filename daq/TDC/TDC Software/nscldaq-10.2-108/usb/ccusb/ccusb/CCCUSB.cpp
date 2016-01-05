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

// Implementation of the CCCUSB class.

#include "CCCUSB.h"
#include "CCCUSBReadoutList.h"
#include <usb.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <stdio.h>


using namespace std;

// Constants:

// Identifying marks for the VM-usb:

static const short USB_WIENER_VENDOR_ID(0x16dc);
static const short USB_VMUSB_PRODUCT_ID(0xb);
static const short USB_CCUSB_PRODUCT_ID(1);

// Bulk transfer endpoints

static const int ENDPOINT_OUT(2);
static const int ENDPOINT_IN(0x86);

// Bits in the list target address words (TAV)

static const uint16_t TAVcsWrite(4);  // Operation writes.
static const uint16_t TAVcsDATA(2);   // DAQ event Data stack.
static const uint16_t TAVcsSCALER(3); // DAQ scaler data stack.
static const uint16_t TAVcsCNAF(0xc);   // Immediate execution of a CNAF list.
static const uint16_t TAVcsIMMED(TAVcsCNAF);


// Timeouts:

static const int DEFAULT_TIMEOUT(2000);	// ms.


//   The following flag determines if enumerate needs to init the libusb:

static bool usbInitialized(false);

//
// Top level statics (here for swig):
//
const uint16_t CCCUSB::Q(1);
const uint16_t CCCUSB::X(2);

/////////////////////////////////////////////////////////////////////
/*!
  Enumerate the Wiener/JTec VM-USB devices.
  This function returns a vector of usb_device descriptor pointers
  for each Wiener/JTec device on the bus.  The assumption is that
  some other luck function has initialized the libusb.
  It is perfectly ok for thesre to be no VM-USB device on the USB 
  subsystem.  In that case an empty vector is returned.
*/
vector<struct usb_device*>
CCCUSB::enumerate()
{
  if(!usbInitialized) {
    usb_init();
    usbInitialized = true;
  }
  usb_find_busses();		// re-enumerate the busses
  usb_find_devices();		// re-enumerate the devices.
  
  // Now we are ready to start the search:
  
  vector<struct usb_device*> devices;	// Result vector.
  struct usb_bus* pBus = usb_get_busses();

  while(pBus) {
    struct usb_device* pDevice = pBus->devices;
    while(pDevice) {
      usb_device_descriptor* pDesc = &(pDevice->descriptor);
      if ((pDesc->idVendor  == USB_WIENER_VENDOR_ID)    &&
	  (pDesc->idProduct == USB_CCUSB_PRODUCT_ID)) {
	devices.push_back(pDevice);
      }
      
      pDevice = pDevice->next;
    }
    
    pBus = pBus->next;
  }
  
  return devices;
}
/**
 * Return the serial number of a usb device.  This involves:
 * - Opening the device.
 * - Doing a simple string fetch on the SerialString
 * - closing the device.
 * - Converting that to an std::string which is then returned to the caller.
 *
 * @param dev - The usb_device* from which we want the serial number string.
 *
 * @return std::string
 * @retval The serial number string of the device.  For VM-USB's this is a
 *         string of the form VMnnnn where nnnn is an integer.
 *
 * @throw std::string exception if any of the USB functions fails indicating
 *        why.
 */
string
CCCUSB::serialNo(struct usb_device* dev)
{
  usb_dev_handle* pDevice = usb_open(dev);

  if (pDevice) {
    char szSerialNo[256];	// actual string is only 6chars + null.
    int nBytes = usb_get_string_simple(pDevice, dev->descriptor.iSerialNumber,
				       szSerialNo, sizeof(szSerialNo));
    usb_close(pDevice);

    if (nBytes > 0) {
      return std::string(szSerialNo);
    } else {
      throw std::string("usb_get_string_simple failed in CCCUSB::serialNo");
    }
				       
  } else {
    throw std::string("usb_open failed in CCCUSB::serialNo");
  }

}

////////////////////////////////////////////////////////////////////
/*!
  Construct the CCCUSB object.  This involves storing the
  device descriptor we are given, opening the device and
  claiming it.  Any errors are signalled via const char* exceptions.
  \param vmUsbDevice   : usb_device*
      Pointer to a USB device descriptor that we want to open.

  \bug
      At this point we take the caller's word that this is a VM-USB.
      Future implementations should verify the vendor and product
      id in the device structure, throwing an appropriate exception
      if there is aproblem.

*/
CCCUSB::CCCUSB(struct usb_device* device) :
    m_handle(0),
    m_device(device),
    m_timeout(DEFAULT_TIMEOUT)
{
  m_serial = serialNo(m_device);
  openUsb();

}
////////////////////////////////////////////////////////////////
/*!
    Destruction of the interface involves releasing the claimed
    interface, followed by closing the device.
*/
CCCUSB::~CCCUSB()
{
    usb_release_interface(m_handle, 0);
    usb_close(m_handle);
    usleep(5000);
}


/**
 * reconnect
 *   
 * Drop connection with the CC-USB and re-open.
 * this can be called when you suspect the CC-USB might
 * have been power cycled.
I*
*/
void
CCCUSB::reconnect()
{
  usb_release_interface(m_handle, 0);
  usb_close(m_handle);
  usleep(1000);

  openUsb();
}


/** Create a readout list that this understands.
 *
 * @return an empty CCCUSBReadoutList
 */
CCCUSBReadoutList*
CCCUSB::createReadoutList() const 
{
  return new CCCUSBReadoutList;
}

////////////////////////////////////////////////////////////////////
//////////////////////// Register operations ///////////////////////
////////////////////////////////////////////////////////////////////
/*!
    Writing a value to the action register.  This is really the only
    special case for this code.  The action register is the only
    item that cannot be handled by creating a local list and
    then executing it immediately.
    Action register I/O requires a special list, see section 4.2, 4.3
    of the Wiener VM-USB manual for more information
    \param value : uint16_t
       The register value to write.
*/
void
CCCUSB::writeActionRegister(uint16_t value)
{
    char outPacket[100];


    // Build up the output packet:

    char* pOut = outPacket;
    
    pOut = static_cast<char*>(addToPacket16(pOut, 5)); // Select Register block for transfer.
    pOut = static_cast<char*>(addToPacket16(pOut, 1)); // Select action register wthin block.
    pOut = static_cast<char*>(addToPacket16(pOut, value));

    // This operation is write only.

    int outSize = pOut - outPacket;
    int status = usb_bulk_write(m_handle, ENDPOINT_OUT, 
				outPacket, outSize, m_timeout);
    if (status < 0) {
	string message = "Error in usb_bulk_write, writing action register ";
	message == strerror(-status);
	throw message;
    }
    if (status != outSize) {
	throw "usb_bulk_write wrote different size than expected";
    }
}
/********************************************************************************/
/*!
  Do a simple 16 bit write to CAMAC.   This is really done
by creating a list with a single write, and executing it immediately.
Note that writes return a 16  bit word that contains the Q/X status of
the operation.

\param n     Slot to which the operation is directed. 
\param a     Subaddress to which the operation is directe.
\param f     Function code, must be in the range 16-23 else an exception
             is thrown. That's just the law of CAMAC.        
\param data  16 bit datum to write.
\param qx    Returns the q/x mask.  See 4.2 of the CC-USB manual for information
             about the bit encoding of this 16 bit word.

\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

\throw string - If a parameter is invalid (e.g. n,a,f).
 
*/
int
CCCUSB::simpleWrite16(int n, int a, int f, uint16_t data, uint16_t& qx)
{
  CCCUSBReadoutList l;
  size_t            nRead;


  return write16(n,a,f, data, qx); // validatees naf.
}
/*******************************************************************************/
/*!
   Do a simple 24 bit CAMAC write.  This is done by creating a list with
   a single 24 bit write operation installed, and executing it immediately.
   Note tht writes return a 16 bit word that has the X and Q response.
\param n     Slot to which the operation is directed. 
\param a     Subaddress to which the operation is directe.
\param f     Function code, must be in the range 16-23 else an exception
             is thrown. That's just the law of CAMAC.        
\param data  16 bit datum to write.
\param qx    Returns the q/x mask.  See 4.2 of the CC-USB manual for information
             about the bit encoding of this 16 bit word.

\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

\throw string - If a parameter is invalid (e.g. n,a,f).
 
*/
int
CCCUSB::simpleWrite24(int n, int a, int f, uint32_t data, uint16_t& qx)
{
  CCCUSBReadoutList l;
  size_t            nRead;


  return write32(n,a,f, data, qx);
}
/************************************************************************/
/*!
  Do a simple 16 bit CAMAC read from a module.  This is done by creating
a single element list with a 24 bit read in it an immediately executing it.
It is necessary to do the 24 bit read in order to get the Q/X response.
We'll return the low 16 bits of the read and the Q/X response formatted
in the bottom 2 bits of the qx word as in the figure at the bottom of
page 29 of the CC-USB manual.

\param n     - Slot to which the operation is directed.
\param a     - Subaddress to which the operation is directed.
\param f     - Function code of the operation (must be 0-7 else
               a string exception is thrown).
\param data  - Reference to uint16_t that will hold the read data.
\param qx    - Reference to a uint16_t that will hold the q/x response
               of the data.

\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

\throw string - If a parameter is invalid (e.g. n,a,f).
*/
int
CCCUSB::simpleRead16(int n, int a, int f, uint16_t& data, uint16_t& qx)
{
  CCCUSBReadoutList l;
  uint32_t          buffer;	// For the return data.

  // Use read32 to do the actual read.  Then put the bottom 16 bits in data
  // and the q/x bits in qx.

  int status = read32(n,a,f, buffer); // validates n/a/f.
  data = buffer & 0xffff;
  qx  = (buffer >> 24) & 0x3;

  return status;

}
/***********************************************************************/
/*!  
  Do a 24 bit read; This is the same as the above, but 
  the user gets all 24 bits of data in their buffer.

\param n     - Slot to which the operation is directed.
\param a     - Subaddress to which the operation is directed.
\param f     - Function code of the operation (must be 0-7 else
               a string exception is thrown).
\param data  - Reference to uint32_t that will hold the read data.
\param qx    - Reference to a uint16_t that will hold the q/x response
               of the data.

\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

\throw string - If a parameter is invalid (e.g. n,a,f).

*/
int
CCCUSB::simpleRead24(int n, int a, int f, uint32_t& data, uint16_t& qx)
{
  CCCUSBReadoutList l;

  // Use read 32 to do the read:

  int status = read32(n,a,f, data);
 

  // Figure out the q/x and remove the top 8 bits of the data.

  qx    = (data >> 24) & 0x3;
  data &= 0xffffff;


  return status;

}
/*!
   Do a simple CAMAC control operation.
   Looks a lot like a simple write but there's no data to transfer.

\param n   - Slot
\param a   - Subaddress
\param f   - Function code.
\param qx  - uint16_t& that will get the q/x response stored in it.

\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

\throw string - If a parameter is invalid (e.g. n,a,f).

*/
int
CCCUSB::simpleControl(int n, int a, int f, uint16_t& qx)
{
  CCCUSBReadoutList l;
  size_t            nRead;
  
 

  l.addControl(n,a,f);		// validates n/a/f.

  int status = executeList(l, &qx, sizeof(qx), &nRead);

  return status;
}


////////////////////////////////////////////////////////////////////////
///////////////////////// Register I/O ////////////////////////////////
//////////////////////////////////////////////////////////////////////


// The CCUSB registers are mapped to CAMAC operations. We can
// therefore use the previous set of functions to do register I/O.
//

/*************************************************************************/
/*!
   Read the firmware id.  This is a 24 bit read from 
  N = 25 a = 0 f = 0:

  \param value - reference to a 32 bit value that will hold the firmware
                 version.
\return int
\retval 0      - Success
\retval other  -Failure code from executeList.



*/
int
CCCUSB::readFirmware(uint32_t& value)
{

  return  read32(25, 0, 0, value);
}

/**************************************************************************/
/*!
  Read the global mode register.  This is 
  n=25 a = 1 f = 0.

\param value  - uint16_t& to receive the contents of the global mode register.
\return int
\retval 0      - Success
\retval other  -Failure code from executeList.


*/
int
CCCUSB::readGlobalMode(uint16_t& value)
{
  uint32_t d;
  // return read16(25, 1, 0, value);
 int status = read32(25, 1, 0, d);
 value = d;
 return status;

}
/****************************************************************************/
/*!
  Write the global mode register. This is just a 16
  bit write.

  n=25, a = 1, f = 16

\param value - the uint16_t to write to the global mode register.
\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

*/
int
CCCUSB::writeGlobalMode(uint16_t data)
{
  uint16_t qx;
  return write16(25, 1, 16, data, qx);
}

/*********************************************************************************/
/*!
   Read the delays register.  This is just a 16 bit read;
   N=25, a=2, f= 0

\param value - uint16_t& into which the delays register value is read.
\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

*/
int
CCCUSB::readDelays(uint16_t& value)
{
  return read16(25,2,0, value);
}
/********************************************************************************/
/*!
   Write the delays register.  This is just a 16 bit write
   N=25, a=0, f=16

\param value - uint16_t to write to the register.
\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

*/
int
CCCUSB::writeDelays(uint16_t value)
{
  uint16_t qx;
  return write16(25,2,16, value, qx);
}
/******************************************************************************/
/*!
   Read the scaler control register. This is a 32 bit read:
   N=25, a = 3 f =0

\param value - uint32_t& into which the scaler control register is read.

\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

*/
int
CCCUSB::readScalerControl(uint32_t& value)
{
  return read32(25,3,0, value);
}
/*****************************************************************************/
/*!
  Write the scaler control register. This is a 32 bit write.
   N=25 a=3, f=16

\param  value - The uint32_t to write.
\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

*/
int
CCCUSB::writeScalerControl(uint32_t value)
{
  uint16_t qx;
  return write32(25, 3, 16, value, qx);
}
/***************************************************************************/
/*!
  Read the led selector register.  This is a 32 bit read:
  n=25, a=4, f=0

\param value - uint32_t& into which the current value of the register will
               be read.

\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

*/
int
CCCUSB::readLedSelector(uint32_t& value)
{
  return read32(25,4,0, value);
}
/*************************************************************************/
/*!
  Write the led selector register.  This is a 32 bit write:
   n=25, a=4, f=16.
\param  value - The uint32_t to write.
\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

*/
int
CCCUSB::writeLedSelector(uint32_t value)
{
  uint16_t qx;
  return write32(25,4,16, value, qx);
}

/************************************************************************/
/*!
  Read the output selector register.  This register determines what the
  NIM outputson the front panel represent.  This is a 32 bit read from:
  n=25, a=5, f=16
\param value - uint32_t& into which the current value of the register will
               be read.

\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

*/
int
CCCUSB::readOutputSelector(uint32_t& value)
{
  return read32(25,5,0, value);
}



/*************************************************************************/
/*!
  Write the output selector regiseter.  This register determines what the
  NIM outputs on the front panel reflect.  This is a 32 bit write to
  n=25, a=5, f=16

\param  value - The uint32_t to write.
\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

*/
int
CCCUSB::writeOutputSelector(uint32_t value)
{
  uint16_t qx;

  return write32(25, 5, 16, value, qx);
}

/************************************************************************/
/*!
   Read the device source selector register.  This determines what
   provides inputs to the onboard devices.  This is a 32 bit read from
   n=25, a=6, f=0

\param value - uint32_t& into which the current value of the register will
               be read.

\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

*/
int
CCCUSB::readDeviceSourceSelectors(uint32_t& value)
{
  return read32(25,6, 0, value);
}

/**************************************************************************/
/*!
   Write the device source selector register.  This determines what
   provides inputs to the onboard devices.  This is a 32 bit write to:
   n=25, a=6, f=16

\param  value - The uint32_t to write.
\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

*/
int
CCCUSB::writeDeviceSourceSelectors(uint32_t value)
{
  uint16_t qx;
  return write32(25,6,16, value, qx);
}

/************************************************************************/
/*!
  Read the timing register for gate and delay register A.  This register
  sets the delay and width of the pulses from that register.  This is a
  32 bit read from  n=25, a=7, f=0
\param value - uint32_t& into which the current value of the register will
               be read.

\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

*/
int
CCCUSB::readDGGA(uint32_t& value)
{
  return read32(25,7,0, value);
}

/************************************************************************/
/*!
  Write the timing register for gate and delay register A.  This register
  sets the delay and width of the pulses from that register.  This is a
  32 bit write from  n=25, a=7, f=16
\param value - uint32_t  containging the value to write.


\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

*/
int
CCCUSB::writeDGGA(uint32_t value)
{
  uint16_t qx;
  return write32(25,7,16, value, qx);
}
/**************************************************************************/
/*!
  Read the timing register for gate and delay register B.  This register 
  sets the delay and width of the pulses from that register.  This is a
  32 bit read from  n=25, a=8, f=0
\param value - uint32_t& into which the current value of the register will
               be read.

\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

*/
int
CCCUSB::readDGGB(uint32_t& value)
{
  return read32(25,8,0, value);
}
/******************************************************************************/
/*!
     Write the timing register for gate and delay register B.  This register
  sets the delay and width of the pulses from that register.  This is a
  32 bit write from  n=25, a=8, f=16
\param value - uint32_t  containging the value to write.


\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

*/
int
CCCUSB::writeDGGB(uint32_t value)
{
  uint16_t qx;
  return write32(25,8,16, value, qx);
}

/****************************************************************************/
/*!
   Read the DGGExt register.  The gate and delay register timing information
   has additional more significant in this register for both DGGs.
   This is a 32 bit read from N=25 A=13 F=0
\param value - uint32_t& into which the current value of the register will
               be read.

\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

*/
int
CCCUSB::readDGGExt(uint32_t& value)
{
  return read32(25,13,0, value);
}
/*************************************************************************/
/*!
   Write the DGGEXT register. The gate and delay register timing information
   has additional more significant in this register for both DGGs.
   This is a 32 bit read from N=25 A=13 F=16
\param value - uint32_t  containging the value to write.


\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

*/
int
CCCUSB::writeDGGExt(uint32_t value)
{
  uint16_t qx;
  return write32(25,13, 16, value, qx);
}

/***********************************************************************/
/*!
   Read the A scler.  This is a 24 bit counter.  We will ensure the
   top bits are zero.
   This is a read from N=25 A=11 F=0

\param value - uint32_t& into which the current value of the register will
               be read.

\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

*/
int
CCCUSB::readScalerA(uint32_t& value)
{
  int status = read32(25,11,0, value);
  value     &= 0xffffff;
  return status;

}

/**********************************************************************/
/*!
   Read the B scaler register. This is a 24 bit counter.  We will ensure the
   top bits are zero.
   This is a read from N=25 A=12 F=0

\param value - uint32_t& into which the current value of the register will
               be read.

\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

*/
int
CCCUSB::readScalerB(uint32_t& value)
{
  int status = read32(25,12,0, value);
  value     &= 0xffffff;
  return status;

}

/*************************************************************************/
/*!
    Read the LAM Trigger register.  This register determines, when a lam 
triggers the readout list, which set of lams can do so.  If this register
is zero, the readout list is triggered by the IN1.
This is a 24 value:
N=25, a=9, f=0.

\param value - Reference to the uint32_t that will hold the
               read lam-mask.  The top 8 bits are removed
               forcibly by us prior to return to ensure there's no
               trash in them.

\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

*/
int
CCCUSB::readLamTriggers(uint32_t& value)
{
  int status = read32(25,9,0, value);
  value     &= 0xffffff;
  return status;
}
/************************************************************************/
/*!
   Write the LAM trigger register. This register determines, when a lam 
triggers the readout list, which set of lams can do so.  If this register
is zero, the readout list is triggered by the IN1.
This is a 24 value:
N=25, a=9, f=16.

\param value - The uint32_t that holds the trigger mask to write.
               It's not 100% clear, but I think that if a bit is a one,
               a LAM from the corresponding slot can trigger readout?
               Or is it that all LAMs are needed to trigger readout?
\return int
\retval 0      - Success
\retval other  -Failure code from executeList.

*/
int
CCCUSB::writeLamTriggers(uint32_t value)
{
  uint16_t qx;
  return write32(25,9,16, value, qx);
}
/************************************************************************/
/*!
  Read the USB Bulk transfer setup.  This register defines how the
  module buffers data. Specifically, it is possible to have the
  module transfer more than one buffer in a USB transfer as  well
  as to set the timout after which the module will close off a transfer
  (send a PKTEND indication).  One important guideline:
  
  - The software will have a USB read timeout in data taking mode.
  That timeout will ensure that the software remains live to other
  stimulii  (e.g. end of run request) even if no data is arriving.
  This software timeout should be longer than the timeout set in this
  register.. .probably significantly longer.
  
  This is a 32 bit read.  N=25, A=14, F=0.
  
  \param value - Reference to the uint32_t value that will  hold the value
  read from this register.
  
  \return int
  \retval 0      - Success
  \retval other  -Failure code from executeList.
  
*/
int
CCCUSB::readUSBBulkTransferSetup(uint32_t& value)
{
  return read32(25,14,0, value);
}
/************************************************************************/
/*!
   Write the USB Bulk transfer setup register.  See readUSBBulkTransferSetup
above for more information about this register.  This is a 32 bit write
N=25, A=14, F=16.

\param value - The uin32_t value to write to the register.

  \return int
  \retval 0      - Success
  \retval other  -Failure code from executeList.
  
*/
int
CCCUSB::writeUSBBulkTransferSetup(uint32_t value)
{
  uint16_t qx;
  return write32(25,14,16, value, qx);
}
				
/*************************************************************************/
/*!
  Perform a crate C cycle.  In the 'laws of CAMAC', it's completely up to
the module what to make of this cycle, however many modules use it to
return to their power up configuration..  Almost all clear any buffered data
an busy, but your module may vary, and only reading the manual for
the module will ensure that you know what this does.
This is  N=28, A=8, F=29

  \return int
  \retval 0      - Success
  \retval other  -Failure code from executeList.
  
*/
int
CCCUSB::c()
{
  uint16_t qx;
  return simpleControl(28,8,29, qx);
}
/**********************************************************************/
/*!
   Perform a Crate Z cycle.  Once more what this does is up to
module designers.  Read the documentation of the modules you are using
to understand the effect this will have on your crate.
N=28, a=9, f=29
  \return int
  \retval 0      - Success
  \retval other  -Failure code from executeList.
  
*/
int
CCCUSB::z()
{
  uint16_t qx;
  return simpleControl(28, 9, 29, qx);
}

/************************************************************************/
/*!
  Set the Crate I (Inhibit) line.  What this does is once more up to the
individual module.  This is a control operation:
N=29, A=9, F=24
  \return int
  \retval 0      - Success
  \retval other  -Failure code from executeList.
  
*/
int
CCCUSB::inhibit()
{
  uint16_t qx;
  return simpleControl(29, 9, 24, qx);
}
/***********************************************************************/
/*!
   Remove the crate I (inhibit) line.  What this does is once more
up to the individual module.  This is a control operation.
N=29, A=9, F=26

  \return int
  \retval 0      - Success
  \retval other  -Failure code from executeList.
  
*/
int
CCCUSB::uninhibit()
{
  uint16_t qx;
  return simpleControl(29, 9, 26, qx);
}

//////////////////////////////////////////////////////////////////////////
/////////////////////////// List operations  ////////////////////////////
/////////////////////////////////////////////////////////////////////////
  
/*!
    Execute a list immediately.  It is the caller's responsibility
    to ensure that no data taking is in progress and that data taking
    has run down (the last buffer was received).  
    The list is transformed into an out packet to the CCUSB and
    transaction is called to execute it and to get the response back.
    \param list  : CCCUSBReadoutList&
       A reference to the list of operations to execute.
    \param pReadBuffer : void*
       A pointer to the buffer that will receive the reply data.
    \param readBufferSize : size_t
       number of bytes of data available to the pReadBuffer.
    \param bytesRead : size_t*
       Return value to hold the number of bytes actually read.
 
    \return int
    \retval  0    - All went well.
    \retval -1    - The usb_bulk_write failed.
    \retval -2    - The usb_bulk_read failed.

    In case of failure, the reason for failure is stored in the
    errno global variable.
*/
int
CCCUSB::executeList(CCCUSBReadoutList&     list,
		   void*                  pReadoutBuffer,
		   size_t                 readBufferSize,
		   size_t*                bytesRead)
{
  size_t outSize;
  uint16_t* outPacket = listToOutPacket(TAVcsIMMED,
					list, &outSize);
    
    // Now we can execute the transaction:
    
  int status = transaction(outPacket, outSize,
			   pReadoutBuffer, readBufferSize);
  
  
  
  delete []outPacket;
  if(status >= 0) {
    *bytesRead = status;
  } 
  else {
    *bytesRead = 0;
  }
  return (status >= 0) ? 0 : status;
  
}
/**
 * This is a swig friendly version of execute list:
 * 
 * @param list - reference to the CCCUSBReadoutList to run.
 * @param maxReadWords - maximum number of 16 bit words that could be read by this list.
 *
 * @return std::vector<uint16_t>
 * @retval Vector of data read by the list.
 */
std::vector<uint16_t>
CCCUSB::executeList(CCCUSBReadoutList& list, int maxReadWords)
{
  // Allocate the read buffer:

  uint16_t* pReadBuffer = new uint16_t[maxReadWords];
  size_t   actualBytes(0);

  executeList(list, pReadBuffer, maxReadWords*sizeof(uint16_t),
	      &actualBytes);

  std::vector<uint16_t> result;
  for (int i =0; i < actualBytes/sizeof(uint16_t); i++) {
    result.push_back(pReadBuffer[i]);
  }
  delete []pReadBuffer;

  return result;

  
}



/*!
   Load a list into the CC-USB for later execution.
   It is the callers responsibility to:
   -  keep track of the lists and their  storage requirements, so that 
      they are not loaded on top of or overlapping
      each other, or so that the available list memory is not exceeded.
   - Ensure the list number is valid and map it to a TAV.
   - The listOffset is valid and that there is room in the list memory
     following it for the entire list being loaded.
   This code just load the list, it does not attach it to any specific trigger.
   that is done via register operations performed after all the required lists
   are in place.
    
   \param listNumber : uint8_t  
      Number of the list to load. 
      - 0 - Data list
      - 1 - Scaler list.
   \param list       : CCCUSBReadoutList
      The constructed list.


\return int
\retval 0  - AOK.
\retval -4 - List number is illegal
\retval other - error from transaction.

*/
int
CCCUSB::loadList(uint8_t  listNumber, CCCUSBReadoutList& list)
{
  // Need to construct the TA field, straightforward except for the list number
  // which is splattered all over creation.
  
  uint16_t ta =  TAVcsWrite;
  if (listNumber == 0) {
    ta |= TAVcsDATA;
  }
  else if (listNumber == 1) {
    ta |= TAVcsSCALER;
  }
  else {
    return -4; 
  }

  size_t   packetSize;
  uint16_t* outPacket = listToOutPacket(ta, list, &packetSize);


  int status = usb_bulk_write(m_handle, ENDPOINT_OUT,
			      reinterpret_cast<char*>(outPacket),
			      packetSize, m_timeout);
  if (status < 0) {
    errno = -status;
    status= -1;
  }

  delete []outPacket;
  return (status >= 0) ? 0 : status;


  
}
/*!
  Execute a bulk read for the user.  The user will need to do this
  when the VMUSB is in autonomous data taking mode to read buffers of data
  it has available.
  \param data : void*
     Pointer to user data buffer for the read.
  \param buffersSize : size_t
     size of 'data' in bytes.
  \param transferCount : size_t*
     Number of bytes actually transferred on success.
  \param timeout : int [2000]
     Timeout for the read in ms.
 
  \return int
  \retval 0   Success, transferCount has number of bytes transferred.
  \retval -1  Read failed, errno has the reason. transferCount will be 0.

*/
int 
CCCUSB::usbRead(void* data, size_t bufferSize, size_t* transferCount, int timeout)
{
  int status = usb_bulk_read(m_handle, ENDPOINT_IN,
			     static_cast<char*>(data), bufferSize,
			     timeout);
  if (status >= 0) {
    *transferCount = status;
    status = 0;
  } 
  else {
    errno = -status;
    status= -1;
    *transferCount = 0;
  }
  return status;
}

/*! 
   Set a new transaction timeout.  The transaction timeout is used for
   all usb transactions but usbRead where the user has full control.
   \param ms : int
      New timeout in milliseconds.
*/
void
CCCUSB::setDefaultTimeout(int ms)
{
  m_timeout = ms;
}


////////////////////////////////////////////////////////////////////////
/////////////////////////////// Utility methods ////////////////////////
////////////////////////////////////////////////////////////////////////

// Debug methods:

// #define TRACE			// Comment out if not tracing


void dumpWords(void* pWords, size_t readSize)
{
  readSize = readSize / sizeof(uint16_t);
  uint16_t* s = reinterpret_cast<uint16_t*>(pWords);

 
  for (int i =0; i < readSize; i++) {
    fprintf(stderr, "%04x ", *s++);
    if (((i % 8) == 0) && (i != 0)) {
      fprintf(stderr, "\n");
    }
  }
  fprintf(stderr, "\n");
}

static void dumpRequest(void* pWrite, size_t writeSize, size_t readSize)
{
#ifdef TRACE
  fprintf(stderr, "%d write, %d read\n", writeSize, readSize);
  dumpWords(pWrite, writeSize);
#endif
}

static void dumpResponse(void* pData, size_t readSize)
{
#ifdef TRACE
  fprintf(stderr, "%d bytes in response\n", readSize);
  dumpWords(pData, readSize);
#endif
}


/*
   Utility function to perform a 'symmetric' transaction.
   Most operations on the VM-USB are 'symmetric' USB operations.
   This means that a usb_bulk_write will be done followed by a
   usb_bulk_read to return the results/status of the operation requested
   by the write.
   Parametrers:
   void*   writePacket   - Pointer to the packet to write.
   size_t  writeSize     - Number of bytes to write from writePacket.
   
   void*   readPacket    - Pointer to storage for the read.
   size_t  readSize      - Number of bytes to attempt to read.


   Returns:
     > 0 the actual number of bytes read into the readPacket...
         and all should be considered to have gone well.
     -1  The write failed with the reason in errno.
     -2  The read failed with the reason in errno.

   NOTE:  The m_timeout is used for both write and read timeouts.

*/
int
CCCUSB::transaction(void* writePacket, size_t writeSize,
		    void* readPacket,  size_t readSize)
{
    int status = usb_bulk_write(m_handle, ENDPOINT_OUT,
				static_cast<char*>(writePacket), writeSize, 
				m_timeout);
    dumpRequest(writePacket, writeSize, readSize);
    if (status < 0) {
	errno = -status;
	return -1;		// Write failed!!
    }
    status    = usb_bulk_read(m_handle, ENDPOINT_IN,
				static_cast<char*>(readPacket), readSize, m_timeout);
    if (status < 0) {
	errno = -status;
	return -2;
    }
#ifdef TRACE
    if (status == 0) {
      fprintf(stderr, "usb_bulk_read returned 0\n");
    } else {
      dumpResponse(readPacket, status);
    }
#endif
    return status;
}


////////////////////////////////////////////////////////////////////////
/*
   Build up a packet by adding a 16 bit word to it;
   the datum is packed low endianly into the packet.

*/
void*
CCCUSB::addToPacket16(void* packet, uint16_t datum)
{
    char* pPacket = static_cast<char*>(packet);

    *pPacket++ = (datum  & 0xff); // Low byte first...
    *pPacket++ = (datum >> 8) & 0xff; // then high byte.

    return static_cast<void*>(pPacket);
}
/////////////////////////////////////////////////////////////////////////
/*
  Build up a packet by adding a 32 bit datum to it.
  The datum is added low-endianly to the packet.
*/
void*
CCCUSB::addToPacket32(void* packet, uint32_t datum)
{
    char* pPacket = static_cast<char*>(packet);

    *pPacket++    = (datum & 0xff);
    *pPacket++    = (datum >> 8) & 0xff;
    *pPacket++    = (datum >> 16) & 0xff;
    *pPacket++    = (datum >> 24) & 0xff;

    return static_cast<void*>(pPacket);
}
/////////////////////////////////////////////////////////////////////
/* 
    Retrieve a 16 bit value from a packet; packet is little endian
    by usb definition. datum will be retrieved in host byte order.
*/
void*
CCCUSB::getFromPacket16(void* packet, uint16_t* datum)
{
    char* pPacket = static_cast<char*>(packet);

    uint16_t low = *pPacket++;
    uint16_t high= *pPacket++;

    *datum =  (low | (high << 8));

    return static_cast<void*>(pPacket);
	
}
/*!
   Same as above but a 32 bit item is returned.
*/
void*
CCCUSB::getFromPacket32(void* packet, uint32_t* datum)
{
    char* pPacket = static_cast<char*>(packet);

    uint32_t b0  = *pPacket++;
    uint32_t b1  = *pPacket++;
    uint32_t b2  = *pPacket++;
    uint32_t b3  = *pPacket++;

    *datum = b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);


    return static_cast<void*>(pPacket);
}

//  Utility to create a stack from a transfer address word and
//  a CCCUSBReadoutList and an optional list offset (for non VCG lists).
//  Parameters:
//     uint16_t ta               The transfer address word.
//     CCCUSBReadoutList& list:  The list of operations to create a stack from.
//     size_t* outSize:          Pointer to be filled in with the final out packet size
//  Returns:
//     A uint16_t* for the list. The result is dynamically allocated
//     and must be released via delete []p e.g.
//
uint16_t*
CCCUSB::listToOutPacket(uint16_t ta, CCCUSBReadoutList& list,
			size_t* outSize)
{
    int listShorts      = list.size();
    int packetShorts    = (listShorts+2);
    uint16_t* outPacket = new uint16_t[packetShorts];
    uint16_t* p         = outPacket;
    
    // Fill the outpacket... a bit different from the VM-USB.

    p = static_cast<uint16_t*>(addToPacket16(p, ta)); 
    p = static_cast<uint16_t*>(addToPacket16(p, listShorts)); 


    vector<uint16_t> stack = list.get();
    for (int i = 0; i < listShorts; i++) {
	p = static_cast<uint16_t*>(addToPacket16(p, stack[i]));
    }
    *outSize = packetShorts*sizeof(short);
    return outPacket;
}






// Utility to do a 32 bit read.  For CAMAC targets,
// The high order bits include the Q/X encoding.
// this will not be the case for register reads
//
int 
CCCUSB::read32(int n, int a, int f, uint32_t& data)
{
  CCCUSBReadoutList l;
  size_t            nRead;
 
  l.addRead24(n,a,f);
  int status = executeList(l, 
			   &data,
			   sizeof(data),
			   &nRead);

  return status;
}

// Utility for a 32 bit read.. This is only used in register reads
// where the q/x are not meaningful.

int 
CCCUSB::read16(int n, int a, int f, uint16_t& data)
{
  CCCUSBReadoutList l;
  size_t            nRead;

  l.addRead16(n,a,f);
  return  executeList(l,
		      &data,
		      sizeof(data),
		      &nRead);
}
//
// Utility to do a 32 bit write.  For CAMAC targets,
// the qx is meaningful, for register targets it is not.
//
int
CCCUSB::write32(int n, int a, int f, uint32_t data, uint16_t& qx)
{
  CCCUSBReadoutList l;
  size_t            nRead;
  

  l.addWrite24(n,a,f, data);
  int status  = executeList(l,
			    &qx,
			    sizeof(qx),
			    &nRead);

  return status;
}
 
// Utility to do a 16 bit write.
 
int 
CCCUSB::write16(int n, int a, int f, uint16_t data, uint16_t& qx)
{
  CCCUSBReadoutList l;
  size_t nRead;

  return write32(n, a, f, (uint32_t)data, qx);
  

  l.addWrite16(n,a,f, data);
  return executeList(l,
		     &qx,
		     sizeof(qx),
		     &nRead);

}


/**
 * openUsb
 *
 *  Does the common stuff required to open a connection
 *  to a CCUSB given that the device has been filled in.
 *
 *  Since the point of this is that it can happen after a power cycle
 *  on the CAMAC crate, we are only going to rely on m_serial being
 *  right and re-enumerate.
 *
 *  @throw std::string - on errors.
 */
void
CCCUSB::openUsb()
{
  // Re-enumerate and get the right value in m_device or throw
  // if our serial number is no longer there:

  std::vector<struct usb_device*> devices = enumerate();
  m_device = 0;
  for (int i = 0; i < devices.size(); i++) {
    if (serialNo(devices[i]) == m_serial) {
      m_device = devices[i];
      break;
    }
  }
  if (!m_device) {
    std::string msg = "CC-USB with serial number ";
    msg += m_serial;
    msg += " cannot be located";
    throw msg;
  }

    m_handle  = usb_open(m_device);
    if (!m_handle) {
	throw "CCCUSB::CCCUSB  - unable to open the device";
    }
    // Now claim the interface.. again this could in theory fail.. but.

    usb_set_configuration(m_handle, 1);
    int status = usb_claim_interface(m_handle, 0);
    if (status == -EBUSY) {
	throw "CCCUSB::CCCUSB - some other process has already claimed";
    }

    if (status == -ENOMEM) {
	throw "CCCUSB::CMVUSB - claim failed for lack of memory";
    }
    usb_clear_halt(m_handle, ENDPOINT_IN);
    usb_clear_halt(m_handle, ENDPOINT_OUT);
   
    usleep(100);
}
