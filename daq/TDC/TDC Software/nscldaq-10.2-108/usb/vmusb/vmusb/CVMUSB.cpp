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

// Implementation of the CVMUSB class.

#include "CVMUSB.h"
#include "CVMUSBReadoutList.h"
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

// Bulk transfer endpoints

static const int ENDPOINT_OUT(2);
static const int ENDPOINT_IN(0x86);

// Timeouts:

static const int DEFAULT_TIMEOUT(2000);	// ms.

// The register offsets:

static const unsigned int FIDRegister(0);       // Firmware id.
static const unsigned int GMODERegister(4);     // Global mode register.
static const unsigned int DAQSetRegister(8);    // DAQ settings register.
static const unsigned int LEDSrcRegister(0xc);	// LED source register.
static const unsigned int DEVSrcRegister(0x10);	// Device source register.
static const unsigned int DGGARegister(0x14);   // GDD A settings.
static const unsigned int DGGBRegister(0x18);   // GDD B settings.
static const unsigned int ScalerA(0x1c);        // Scaler A counter.
static const unsigned int ScalerB(0x20);        // Scaler B data.
static const unsigned int ExtractMask(0x24);    // CountExtract mask.
static const unsigned int ISV12(0x28);          // Interrupt 1/2 dispatch.
static const unsigned int ISV34(0x2c);          // Interrupt 3/4 dispatch.
static const unsigned int ISV56(0x30);          // Interrupt 5/6 dispatch.
static const unsigned int ISV78(0x34);          //  Interrupt 7/8 dispatch.
static const unsigned int DGGExtended(0x38);    // DGG Additional bits.
static const unsigned int USBSetup(0x3c);       // USB Bulk transfer setup. 
static const unsigned int USBVHIGH1(0x40);       // additional bits of some of the interrupt vectors.
static const unsigned int USBVHIGH2(0x44);       // additional bits of the other interrupt vectors.


// Bits in the list target address word:

static const uint16_t TAVcsID0(1); // Bit mask of Stack id bit 0.
static const uint16_t TAVcsSel(2); // Bit mask to select list dnload
static const uint16_t TAVcsWrite(4); // Write bitmask.
static const uint16_t TAVcsIMMED(8); // Target the VCS immediately.
static const uint16_t TAVcsID1(0x10);
static const uint16_t TAVcsID2(0x20);
static const uint16_t TAVcsID12MASK(0x30); // Mask for top 2 id bits
static const uint16_t TAVcsID12SHIFT(4);

//   The following flag determines if enumerate needs to init the libusb:

static bool usbInitialized(false);

/////////////////////////////////////////////////////////////////////
/*!
  Enumerate the Wiener/JTec VM-USB devices.
  This function returns a vector of usb_device descriptor pointers
  for each Wiener/JTec device on the bus.  The assumption is that
  some other luck function has initialized the libusb.
  It is perfectly ok for there to be no VM-USB device on the USB 
  subsystem.  In that case an empty vector is returned.
*/
vector<struct usb_device*>
CVMUSB::enumerate()
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
	  (pDesc->idProduct == USB_VMUSB_PRODUCT_ID)) {
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
CVMUSB::serialNo(struct usb_device* dev)
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
      throw std::string("usb_get_string_simple failed in CVMUSB::serialNo");
    }
				       
  } else {
    throw std::string("usb_open failed in CVMUSB::serialNo");
  }

}
////////////////////////////////////////////////////////////////////
/*!
  Construct the CVMUSB object.  This involves storing the
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
CVMUSB::CVMUSB(struct usb_device* device) :
    m_handle(0),
    m_device(device),
    m_timeout(DEFAULT_TIMEOUT)
{
    m_handle  = usb_open(m_device);
    if (!m_handle) {
	throw "CVMUSB::CVMUSB  - unable to open the device";
    }
    // Now claim the interface.. again this could in theory fail.. but.

    usb_set_configuration(m_handle, 1);
    int status = usb_claim_interface(m_handle, 
				     0);
    if (status == -EBUSY) {
	throw "CVMUSB::CVMUSB - some other process has already claimed";
    }

    if (status == -ENOMEM) {
	throw "CVMUSB::CMVUSB - claim failed for lack of memory";
    }
    // Errors we don't know about:

    if (status < 0) {
      std::string msg("Failed to claim the interface: ");
      msg += strerror(-status);
      throw msg;
    }
    usb_clear_halt(m_handle, ENDPOINT_IN);
    usb_clear_halt(m_handle, ENDPOINT_OUT);

    usleep(100);
    
    // Now set the irq mask so that all bits are set..that:
    // - is the only way to ensure the m_irqMask value matches the register.
    // - ensures m_irqMask actually gets set:

    writeIrqMask(0xff);
}
////////////////////////////////////////////////////////////////
/*!
    Destruction of the interface involves releasing the claimed
    interface, followed by closing the device.
*/
CVMUSB::~CVMUSB()
{
  if (m_handle) {
    usb_release_interface(m_handle, 0);
    usb_close(m_handle);
    usleep(5000); 
  }
}
/**
 * Close and re-open the VM-USB interface:
 * Onlly subclasses can implement this.
 */
void
CVMUSB::reconnect()
{}

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
CVMUSB::writeActionRegister(uint16_t value)
{
    char outPacket[100];


    // Build up the output packet:

    char* pOut = outPacket;
    
    pOut = static_cast<char*>(addToPacket16(pOut, 5)); // Select Register block for transfer.
    pOut = static_cast<char*>(addToPacket16(pOut, 10)); // Select action register wthin block.
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

////////////////////////////////////////////////////////////////////////
/*!
   Read the firmware id register.  This is register 0.
   Doing this is a matter of building a CVMUSBReadoutList to do the
   job and then submitting it for immediate execution.

   \return uint16_t
   \retval The value of the firmware Id register.

*/
int
CVMUSB::readFirmwareID()
{
    return readRegister(FIDRegister);
}

////////////////////////////////////////////////////////////////////////

/*!
   Write the global mode register (GMODERegister).
  \param value :uint16_t 
     The 16 bit global mode register value.
*/
void
CVMUSB::writeGlobalMode(uint16_t value)
{
    writeRegister(GMODERegister, static_cast<uint32_t>(value));
}

/*!
    Read the global mode register (GMODERegistesr).
    \return uint16_t  
    \retval the value of the register.
*/
int
CVMUSB::readGlobalMode()
{
    return static_cast<uint16_t>(readRegister(GMODERegister));
}

/////////////////////////////////////////////////////////////////////////

/*!
   Write the data acquisition settings register.
  (DAQSetRegister)
  \param value : uint32_t
     The value to write to the register
*/
void
CVMUSB::writeDAQSettings(uint32_t value)
{
    writeRegister(DAQSetRegister, value);
}
/*!
  Read the data acquisition settings register.
  \return uint32_t
  \retval The value read from the register.
*/
uint32_t
CVMUSB::readDAQSettings()
{
    return readRegister(DAQSetRegister);
}
//////////////////////////////////////////////////////////////////////////
/*!
  Write the LED source register.  All of the LEDs on the VMUSB
  are essentially user programmable.   The LED Source register determines
  which LEDS display what.  The register is made up of a bunch of
  3 bit fields, invert and latch bits.  These fields are defined by the
  constants in the CVMUSB::LedSourceRegister class.  For example
  CVMUSB::LedSourceRegister::topYellowInFifoFull ored in to this register
  will make the top yellow led light when the input fifo is full.


  \param value : uint32_t
     The value to write to the LED Src register.
*/
void
CVMUSB::writeLEDSource(uint32_t value)
{
    writeRegister(LEDSrcRegister, value);
}
/*!
   Read the LED Source register.  
   \return uint32_t
   \retval The current value of the LED source register.
*/
int
CVMUSB::readLEDSource()
{
    return readRegister(LEDSrcRegister);
}
/////////////////////////////////////////////////////////////////////////
/*!
   Write the device source register.  This register determines the
   source of the start for the gate and delay generators. What makes
   scalers increment and which signals are routed to the NIM out
   lemo connectors.
   \param value :uint32_t
      The new value to write to this register.   This is a bitmask that
      consists of code fields along with latch and invert bits.
      These bits are defined in the CVMUSB::DeviceSourceRegister
      class.  e.g. CVMUSB::DeviceSourceRegister::nimO2UsbTrigger1 
      ored into value will cause the O2 to pulse when a USB initiated
      trigger is produced.
*/
void
CVMUSB::writeDeviceSource(uint32_t value)
{
    writeRegister(DEVSrcRegister, value);
}
/*!
   Read the device source register.
   \return uint32_t
   \retval current value of the register.
*/
int
CVMUSB::readDeviceSource()
{
    return readRegister(DEVSrcRegister);
}
/////////////////////////////////////////////////////////////////////////
/*!
     Write the gate width and delay for delay and gate generator A
     \param value : uint32_t 
       The gate and delay generator value.  This value is split into 
       two fields, the gate width and delay.  Note that the width is
       further modified by the value written to the DGGExtended register.
*/
void
CVMUSB::writeDGG_A(uint32_t value)
{
    writeRegister(DGGARegister, value);
}
/*!
   Read the register controlling the delay and fine width of 
   Delay and Gate register A.
   \return uint32_t
   \retval  register value.
*/
uint32_t
CVMUSB::readDGG_A()
{
    return readRegister(DGGARegister);
}
/*!
  Write the gate with and delay for the B dgg. See writeDGG_A for
  details.
*/
void
CVMUSB::writeDGG_B(uint32_t value)
{
    writeRegister(DGGBRegister, value);
}
/*!
   Reads the control register for the B channel DGG.  See readDGG_A
   for details.
*/
 uint32_t
 CVMUSB::readDGG_B()
{
    return readRegister(DGGBRegister);
}

/*!
   Write the DGG Extension register.  This register was added
   to provide longer gate widths.  It contains the high order
   bits of the widths for both the A and B channels of DGG.
*/
void
CVMUSB::writeDGG_Extended(uint32_t value)
{
    writeRegister(DGGExtended, value);
}
/*!
   Read the value of the DGG extension register.
*/
uint32_t
CVMUSB::readDGG_Extended()
{
    return readRegister(DGGExtended);
}

//////////////////////////////////////////////////////////////////////////
/*!
   Read the A scaler channel
   \return uint32_t
   \retval the value read from the A channel.
*/
uint32_t
CVMUSB::readScalerA()
{
    return readRegister(ScalerA);
}
/*!
   Read the B scaler channel
*/
uint32_t
CVMUSB::readScalerB()
{
    return readRegister(ScalerB);
}


/////////////////////////////////////////////////////////////////////

/*!
    Write an interrupt service vector register.  The
    ISV's allow the application to specific a list to be associated
    with specific VME interrupts.  Each ISV register
    contains a pair of ISV specifications. See the
    CVMSUSB::ISVregister class for bit/mask/shift definitions in this
    register.
    \param which : int
      Specifies which of the ISV registers to write.  This is a value
      between 1 and 4.  1 is ISV12, 2 ISV34 ... 4 ISV78.
    \param value : uint32_t
       The new value to write to the register.

    \throw string  - if the which value is illegal.
*/
void
CVMUSB::writeVector(int which, uint32_t value)
{
    unsigned int regno = whichToISV(which);
    writeRegister(regno, value);
    
    // Horrible kluge... 
    // set the tops of the vectors to 0xfffffff in keeping with what 8bit
    // interrupters (the only type  I know of) require).

    writeRegister(USBVHIGH1, 0xffffffff);
    writeRegister(USBVHIGH2, 0xffffffff);
}


/*!
  Read an interrupt service vector register.  See writeVector for
  a discussion that describes these registers.
  \param which : int
      Specifies the ISV register to to read.
  \return int
  \retval the register contents.
 
  \throw string  - if thwhich is illegal.
*/
int
CVMUSB::readVector(int which)
{
    unsigned int regno = whichToISV(which);
    return readRegister(regno);
}

/*!
   Write the interrupt mask.  This 8 bit register has bits that are set for
   each interrupt level that should be ignored by the VM-USB.
   @param mask Interrupt mask to write.
*/
void
CVMUSB::writeIrqMask(uint8_t mask)
{
  // Well this is a typical VM-USB abomination:
  // - First save the global (mode?) register as we're going to wipe it out
  // - Next write the irqmask | 0x8000 to the glboal mode register.
  // - Write a USB trigger to the action register.
  // - Clear the action register(?)
  // - Write 0 to the global mode register.
  // - restore the saved global mode register.

  uint16_t oldGlobalMode = readGlobalMode();
  uint16_t gblmask      = mask;
  gblmask              |= 0x8000;
  writeGlobalMode(gblmask);
  writeActionRegister(2);	// Hopefully bit 1 numbered from zero not one.
  uint32_t maskValue         = readFirmwareID(); // should have the mask.
  writeGlobalMode(0);			    // Turn off this nonesense

  writeGlobalMode(oldGlobalMode); // restor the old globalmode.

  m_irqMask = mask;
}
/*!
   Read the interrupt mask.  This 8 bit register has bits set for each
   interrupt level that should be ignoered by the VM-USB.
   @return uint8_t
   @retval contents of the mask register.
*/;
int
CVMUSB::readIrqMask()
{
  // Since the interrupt mask register appears cleverly crafted so that you
  // can't actually read it without destroying it, we're just going to use
  // a copy of the value:

  return m_irqMask;
}


///////////////////////////////////////////////////////////////////////
/*!
    write the bluk transfer setup register.  This register
    sets up a few of the late breaking data taking parameters
    that are built to allow data to flow through the USB more
    effectively.  For bit/mask/shift-count definitions of this
    register see CVMUSB::TransferSetup.
    \param value : uint32_t
      The value to write to the register.
*/
void
CVMUSB::writeBulkXferSetup(uint32_t value)
{
    writeRegister(USBSetup, value);
}
/*!
   Read the bulk transfer setup register.
*/
int
CVMUSB::readBulkXferSetup()
{
    return readRegister(USBSetup);
}


/**! create an empty CVMUSBReadoutList
*/
CVMUSBReadoutList*
CVMUSB::createReadoutList() const
{
    return new CVMUSBReadoutList;
}


/////////////////////////////////////////////////////////////////////////
/////////////////////////// VME Transfer Ops ////////////////////////////
/////////////////////////////////////////////////////////////////////////

/*!
   Write a 32 bit word to the VME for some specific address modifier.
   This is done by creating a list, inserting a single
   32bit write into it and executing the list.
   \param address  : uint32_t 
      Address to which the write is done.
   \param aModifier : uint8_t
      Address modifier for the operation.  This should not be a block transfer
      address modifier... those may not work for single shots and in any
      event would not pay.
    \param data   : uint32_t
      The datum to write.
 
   \return int
   \retval 0   - Success.
   \retval -1  - USB write failed.
   \retval -2  - USB read failed.
   \retval -3  - VME Bus error.

    \note In case of failure, errno, contains the reason.
*/
int
CVMUSB::vmeWrite32(uint32_t address, uint8_t aModifier, uint32_t data)
{
  CVMUSBReadoutList list;

  list.addWrite32(address, aModifier, data);
  return doVMEWrite(list);

}
/*!
   Write a 16 bit word to the VME.  This is identical to vmeWrite32,
   however the data is 16 bits wide.
*/
int
CVMUSB::vmeWrite16(uint32_t address, uint8_t aModifier, uint16_t data)
{
  CVMUSBReadoutList list;
  list.addWrite16(address, aModifier, data);
  return doVMEWrite(list);
}
/*!
  Do an 8 bit write to the VME bus.
*/
int
CVMUSB::vmeWrite8(uint32_t address, uint8_t aModifier, uint8_t data)
{
  CVMUSBReadoutList list;
  list.addWrite8(address, aModifier, data);
  return doVMEWrite(list);
}

/*!
   Read a 32 bit word from the VME.  This is done by creating a list,
   inserting a single VME read operation in it and executing the list in 
   immediate mode.
   \param address : uint32_t
      The address to read.
   \param amod    : uint8_t
      The address modifier that selects the address space used for the
      transfer.  You should not use a block transfer modifier for this.
   \param data    : uint32_t*
      Pointer to the location in which the data will be placed.

   \return int
    \retval  0    - All went well.
    \retval -1    - The usb_bulk_write failed.
    \retval -2    - The usb_bulk_read failed.
*/
int
CVMUSB::vmeRead32(uint32_t address, uint8_t aModifier, uint32_t* data)
{
  CVMUSBReadoutList list;
  list.addRead32(address, aModifier);
  uint32_t      lData;
  int status = doVMERead(list, &lData);
  *data      = lData;
  return status;
}

/*!
    Read a 16 bit word from the VME.  This is just like the previous
    function but the data transfer width is 16 bits.
*/
int
CVMUSB::vmeRead16(uint32_t address, uint8_t aModifier, uint16_t* data)
{
  CVMUSBReadoutList list;
  list.addRead16(address, aModifier);
  uint32_t lData;
  int      status = doVMERead(list, &lData);
  *data = static_cast<uint16_t>(lData);

  return status;
}
/*!
   Read an 8 bit byte from the VME... see vmeRead32 for information about
   this.
*/
int
CVMUSB::vmeRead8(uint32_t address, uint8_t aModifier, uint8_t* data)
{
  CVMUSBReadoutList list;
  list.addRead8(address, aModifier);
  uint32_t lData;
  int      status = doVMERead(list, &lData);
  *data  = static_cast<uint8_t>(lData);
  return status;
}

//////////////////////////////////////////////////////////////////////////
/////////////////////////// Block read operations ////////////////////////
/////////////////////////////////////////////////////////////////////////

/*!
    Read a block of longwords from the VME.  It is the caller's responsibility
    to:
    - Ensure that the starting address of the transfer is block aligned
      (that is a multiple of 0x100).
    - That the address modifier is  block transfer modifier such as
      CVMUSBReadoutList::a32UserBlock.
    The list construction takes care of the case where the count spans block
    boundaries or requires a trailing partial block transfer.  Note that
    transfers are done in 32 bit width.

    \param baseAddress : uint32_t
      Fisrt transfer address of the transfer.
    \param aModifier   : uint8_t
      Address modifier of the transfer.  See above for restrictions.
    \param data        : void*
       Buffer to hold the data read.
    \param transferCount : size_t
       Number of \em Longwords to transfer.
    \param countTransferred : size_t*
        Pointer to a size_t into which will be written the number of longwords
        actually transferred.  This can be used to determine if the list
        exited prematurely (e.g. due to a bus error).

     \return int
     \retval 0  - Successful completion.  Note that a BERR in the middle
                  of the transfer is successful completion.  The countTransferred
                  will be less than transferCount in that case.
     \retval -1 - Failure on usb_bulk_write, the actual cause of the error is
                  in errno, countTransferred will be 0.
     \retval -2 - Failure on the usb_bulk_write, The actual error will be in 
                  errno.  countTransferred will be 0.

*/
int
CVMUSB::vmeBlockRead(uint32_t baseAddress, uint8_t aModifier,
		     void*    data,        size_t transferCount, 
		     size_t*  countTransferred)
{
  // Create the list:

  CVMUSBReadoutList list;
  list.addBlockRead32(baseAddress, aModifier, transferCount);


  *countTransferred = 0;
  int status = executeList(list, data, transferCount*sizeof(uint32_t),
			   countTransferred);
  *countTransferred = *countTransferred/sizeof(uint32_t); // bytes -> transfers.
  return status;
}
/*!
   Do a 32 bit wide block read from a FIFO.  The only difference between
   this and vmeBlockRead is that the address read from is the same for the
   entire block transfer.
*/
int 
CVMUSB::vmeFifoRead(uint32_t address, uint8_t aModifier,
		    void*    data,    size_t transferCount, size_t* countTransferred)
{
  CVMUSBReadoutList list;
  list.addFifoRead32(address, aModifier, transferCount);

  *countTransferred = 0;
  int status =  executeList(list, data, transferCount*sizeof(uint32_t), 
			    countTransferred);
  *countTransferred = *countTransferred/sizeof(uint32_t); // bytes -> transferrs.
  return status;
}


// Variable block read support:
//

/*!
  Do an 8 bit read in to the number data regiser. I believe we don't
  get this data into the output buffer.
  \param address (uint32_t)   - Address from which the read is done.
  \param mask    (uint32_t)   - Count extract mask.
  \param amod    (uint8_t)    - Address modifier for the transfer.
*/
int
CVMUSB::vmeReadBlockCount8(uint32_t address, uint32_t mask, uint8_t amod)
{
  CVMUSBReadoutList list;
  uint8_t           data;
  size_t            readCount;

  list.addBlockCountRead8(address, mask, amod);
  int status = executeList(list, &data, sizeof(uint8_t), &readCount);
  return status;
}
/*!
  Do a 16 bit read to the number data register. See above.
  \param address (uint32_t)   - Address from which the read is done.
  \param mask    (uint32_t)   - Count extract mask.
  \param amod    (uint8_t)    - Address modifier for the transfer.
*/
int
CVMUSB::vmeReadBlockCount16(uint32_t address, uint32_t mask, uint8_t amod)
{
  CVMUSBReadoutList list;
  uint8_t           data;
  size_t            readCount;

  list.addBlockCountRead16(address, mask, amod);
  int status = executeList(list, &data, sizeof(uint8_t), &readCount);
  return status;
}
/*!
    Do a 32 bit read to the number data register.  See above:

  \param address (uint32_t)   - Address from which the read is done.
  \param mask    (uint32_t)   - Count extract mask.
  \param amod    (uint8_t)    - Address modifier for the transfer.
*/
int
CVMUSB::vmeReadBlockCount32(uint32_t address, uint32_t mask, uint8_t amod)
{
  CVMUSBReadoutList list;
  uint8_t           data;
  size_t            readCount;

  list.addBlockCountRead32(address, mask, amod);
  int status = executeList(list, &data, sizeof(uint8_t), &readCount);
  return status;
}
/*!
   Do a variable length block transfer.  This transfer must have been
   set up by doing a call to one of the vmeReadBlockCountxx functions.
   furthermore, no other block transfer can have taken place since that
   operation.  It is _STRONGLY_ recommended that unless prohibited by the
   hardware, a vmeReadBlockCountxx be _IMMEDIATELY_ followed by a
   variable block read or variable fifo read.
   \param address (uint32_t)  Starting address of the block transfer.
   \param amod    (uint8_t)   Address modifier for the transfer.
   \param data    (void*)     Pointer to the buffer to receive the transfer.
   \param maxCount (size_t)   Size of the buffer in longwords.
   \param countTransferred (size_t*) Pointer to where the actual transfer count is stored.

*/
int 
CVMUSB::vmeVariableBlockRead(uint32_t address, uint8_t amod,
			      void* data, size_t maxCount, size_t* countTransferred)
{
  CVMUSBReadoutList list;
  list.addMaskedCountBlockRead32(address, amod);
  *countTransferred = 0;	// In case of failure.
  int status = executeList(list, data, maxCount*sizeof(uint32_t), countTransferred);
  *countTransferred = *countTransferred/sizeof(uint32_t);

  return status;
}
/*!
   See above, however the address pointer is not incremented between block transfers.
   \param address (uint32_t)  Starting address of the block transfer.  In this case, this is
                              the only address from which data are transferred.
   \param amod    (uint8_t)   Address modifier for the transfer.
   \param data    (void*)     Pointer to the buffer to receive the transfer.
   \param maxCount (size_t)   Size of the buffer in longwords.
   \param countTransferred (size_t*) Pointer to where the actual transfer count is stored.

*/

int 
CVMUSB::vmeVariableFifoRead(uint32_t address, uint8_t amod, 
			    void* data, size_t maxCount, size_t* countTransferred)
{
  CVMUSBReadoutList list;
  list.addMaskedCountFifoRead32(address, amod);
  *countTransferred = 0;	// In case of failure.
  int status = executeList(list, data, maxCount*sizeof(uint32_t), countTransferred);
  *countTransferred = *countTransferred/sizeof(uint32_t);

  return status;
}

//////////////////////////////////////////////////////////////////////////
/////////////////////////// List operations  ////////////////////////////
/////////////////////////////////////////////////////////////////////////
  
/*!
    Execute a list immediately.  It is the caller's responsibility
    to ensure that no data taking is in progress and that data taking
    has run down (the last buffer was received).  
    The list is transformed into an out packet to the VMUSB and
    transaction is called to execute it and to get the response back.
    \param list  : CVMUSBReadoutList&
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
CVMUSB::executeList(CVMUSBReadoutList&     list,
		   void*                  pReadoutBuffer,
		   size_t                 readBufferSize,
		   size_t*                bytesRead)
{
  size_t outSize;
  uint16_t* outPacket = listToOutPacket(TAVcsWrite | TAVcsIMMED,
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

// SWIG wrappable version of execute list.

std::vector<uint8_t> 
CVMUSB::executeList(CVMUSBReadoutList& list, int maxBytes)
{
  uint8_t data[maxBytes];
  size_t     nRead;
  std::vector<uint8_t> result;

  int status = this->executeList(list, data, maxBytes, &nRead);

  if (status == 0) {
    for (int i = 0; i < nRead; i++) {
      result.push_back(data[i]);
    }
  }

  return result;
}

/*!
   Load a list into the VM-USB for later execution.
   It is the callers responsibility to:
   -  keep track of the lists and their  storage requirements, so that 
      they are not loaded on top of or overlapping
      each other, or so that the available list memory is not exceeded.
   - Ensure that the list number is a valid value (0-7).
   - The listOffset is valid and that there is room in the list memory
     following it for the entire list being loaded.
   This code just load the list, it does not attach it to any specific trigger.
   that is done via register operations performed after all the required lists
   are in place.
    
   \param listNumber : uint8_t  
      Number of the list to load. 
   \param list       : CVMUSBReadoutList
      The constructed list.
   \param listOffset : off_t
      The offset in list memory at which the list is loaded.
      Question for the Wiener/Jtec guys... is this offset a byte or long
      offset... I'm betting it's a longword offset.
*/
int
CVMUSB::loadList(uint8_t  listNumber, CVMUSBReadoutList& list, off_t listOffset)
{
  // Need to construct the TA field, straightforward except for the list number
  // which is splattered all over creation.
  
  uint16_t ta = TAVcsSel | TAVcsWrite;
  if (listNumber & 1)  ta |= TAVcsID0;
  if (listNumber & 2)  ta |= TAVcsID1; // Probably the simplest way for this
  if (listNumber & 4)  ta |= TAVcsID2; // few bits.

  size_t   packetSize;
  uint16_t* outPacket = listToOutPacket(ta, list, &packetSize, listOffset);
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
CVMUSB::usbRead(void* data, size_t bufferSize, size_t* transferCount, int timeout)
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
CVMUSB::setDefaultTimeout(int ms)
{
  m_timeout = ms;
}


////////////////////////////////////////////////////////////////////////
/////////////////////////////// Utility methods ////////////////////////
////////////////////////////////////////////////////////////////////////
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
CVMUSB::transaction(void* writePacket, size_t writeSize,
		    void* readPacket,  size_t readSize)
{
    int status = usb_bulk_write(m_handle, ENDPOINT_OUT,
				static_cast<char*>(writePacket), writeSize, 
				m_timeout);
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
    return status;
}


////////////////////////////////////////////////////////////////////////
/*
   Build up a packet by adding a 16 bit word to it;
   the datum is packed low endianly into the packet.

*/
void*
CVMUSB::addToPacket16(void* packet, uint16_t datum)
{
    uint8_t* pPacket = static_cast<uint8_t*>(packet);

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
CVMUSB::addToPacket32(void* packet, uint32_t datum)
{
    uint8_t* pPacket = static_cast<uint8_t*>(packet);

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
CVMUSB::getFromPacket16(void* packet, uint16_t* datum)
{
    uint8_t* pPacket = static_cast<uint8_t*>(packet);

    uint16_t low = *pPacket++;
    uint16_t high= *pPacket++;

    *datum =  (low | (high << 8));

    return static_cast<void*>(pPacket);
	
}
/*!
   Same as above but a 32 bit item is returned.
*/
void*
CVMUSB::getFromPacket32(void* packet, uint32_t* datum)
{
    uint8_t* pPacket = static_cast<uint8_t*>(packet);

    uint32_t b0  = *pPacket++;
    uint32_t b1  = *pPacket++;
    uint32_t b2  = *pPacket++;
    uint32_t b3  = *pPacket++;

    *datum = b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);


    return static_cast<void*>(pPacket);
}
/*
   Reading a register is just creating the appropriate CVMUSBReadoutList
   and executing it immediatly.
*/
uint32_t
CVMUSB::readRegister(unsigned int address)
{
    CVMUSBReadoutList list;
    uint32_t          data;
    size_t            count;
    list.addRegisterRead(address);

    int status = executeList(list,
			     &data,
			     sizeof(data),
			     &count);
    if (status == -1) {
	char message[100];
	sprintf(message, "CVMUSB::readRegister USB write failed: %s",
		strerror(errno));
	throw string(message);
    }
    if (status == -2) {
	char message[100];
	sprintf(message, "CVMUSB::readRegister USB read failed %s",
		strerror(errno));
	throw string(message);

    }

    return data;
			     

    
}
/*
  
   Writing a register is just creating the appropriate list and
   executing it immediately:
*/
void
CVMUSB::writeRegister(unsigned int address, uint32_t data)
{
    CVMUSBReadoutList list;
    uint32_t          rdstatus;
    size_t            rdcount;
    list.addRegisterWrite(address, data);

    int status = executeList(list,
			     &rdstatus, 
			     sizeof(rdstatus),
			     &rdcount);

    if (status == -1) {
	char message[100];
	sprintf(message, "CVMUSB::writeRegister USB write failed: %s",
		strerror(errno));
	throw string(message);
    }
    if (status == -2) {
	char message[100];
	sprintf(message, "CVMUSB::writeRegister USB read failed %s",
		strerror(errno));
	throw string(message);

    }
    
}

/*   

    Convert an ISV which value to a register number...
    or throw a string if the register selector is invalid.

*/
unsigned int
CVMUSB::whichToISV(int which)
{
    switch (which) {
	case 1:
	    return ISV12;
	case 2:
	    return ISV34;
	case 3:
	    return ISV56;
	case 4:
	    return ISV78;
	default:
	{
	    char msg[100];
	    sprintf(msg, "CVMUSB::whichToISV - invalid isv register %d",
		    which);
	    throw string(msg);
	}
    }
}
// If the write list has already been created, this fires it off and returns
// the appropriate status:
//
int
CVMUSB::doVMEWrite(CVMUSBReadoutList& list)
{
  uint16_t reply;
  size_t   replyBytes;
  int status = executeList(list, &reply, sizeof(reply), &replyBytes);
  // Bus error:
  if ((status == 0) && (reply == 0)) {
    status = -3;
  }
  return status;
}

// Common code to do a single shot vme read operation:
int
CVMUSB::doVMERead(CVMUSBReadoutList& list, uint32_t* datum)
{
  size_t actualRead;
  int status = executeList(list, datum, sizeof(uint32_t), &actualRead);
  return status;
}

//  Utility to create a stack from a transfer address word and
//  a CVMUSBReadoutList and an optional list offset (for non VCG lists).
//  Parameters:
//     uint16_t ta               The transfer address word.
//     CVMUSBReadoutList& list:  The list of operations to create a stack from.
//     size_t* outSize:          Pointer to be filled in with the final out packet size
//     off_t   offset:           If VCG bit is clear and VCS is set, the bottom
//                               16 bits of this are put in as the stack load
//                               offset. Otherwise, this is ignored and
//                               the list lize is treated as a 32 bit value.
//  Returns:
//     A uint16_t* for the list. The result is dynamically allocated
//     and must be released via delete []p e.g.
//
uint16_t*
CVMUSB::listToOutPacket(uint16_t ta, CVMUSBReadoutList& list,
			size_t* outSize, off_t offset)
{
    int listLongwords = list.size();
    int listShorts    = listLongwords*sizeof(uint32_t)/sizeof(uint16_t);
    int packetShorts    = (listShorts + 3);
    uint16_t* outPacket = new uint16_t[packetShorts];
    uint16_t* p         = outPacket;
    
    // Fill the outpacket:

    p = static_cast<uint16_t*>(addToPacket16(p, ta)); 
    //
    // The next two words depend on which bits are set in the ta
    //
    if(ta & TAVcsIMMED) {
      p = static_cast<uint16_t*>(addToPacket32(p, listShorts+1)); // 32 bit size.
    }
    else {
      p = static_cast<uint16_t*>(addToPacket16(p, listShorts+1)); // 16 bits only.
      p = static_cast<uint16_t*>(addToPacket16(p, offset));       // list load offset. 
    }

    vector<uint32_t> stack = list.get();
    for (int i = 0; i < listLongwords; i++) {
	p = static_cast<uint16_t*>(addToPacket32(p, stack[i]));
    }
    *outSize = packetShorts*sizeof(short);
    return outPacket;
}
