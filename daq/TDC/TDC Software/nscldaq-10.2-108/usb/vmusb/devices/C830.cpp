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
#include "C830.h"
#include <CReadoutModule.h>
#include <CVMUSB.h>
#include <CVMUSBReadoutList.h>
#include <CConfiguration.h>
#include <stdlib.h>
#include <assert.h>
#include <set>

using namespace std;

// Register offsets for the V830 module

#define offset(name)  static const uint32_t name = 


offset(MEB)          0;		// Multi event buffer.
offset(COUNTERS)     0x1000;	// The live counters.
offset(ENABLES)      0x1100;    // Channel enables register (32bits)
offset(DWELLTIME)    0x1104;    // Periodic readout time (32 bits)
offset(CONTROL)      0x1108;    // Control register (16 bits).
offset(GEO)          0x1110;    // Geographical address (16 bits).
offset(IPL)          0x1112;    // Interrupt level (16 bits)
offset(VECTOR)       0x1114;    // Interrupt vector (16 bits)
offset(RESET)        0x1120;    // Module reset (16 bit key).
offset(CLEAR)        0x1122;    // Software clear (16 bit clear)
offset(TRIGGER)      0x1124;    // VME trigger (16 bit)
offset(ALMOSTFULL)   0x112c;     // # events in MEB before firing irq.


// Control register bits:

static const uint16_t NOTRIGGER(0);
static const uint16_t RANDOMTRIGGER(1);
static const uint16_t PERIODICTRIGGER(2);

static const uint16_t NARROW(4);
static const uint16_t HEADER(0x20);
static const uint16_t AUTORESET(0x80);

// Adress modifiers:

static const uint8_t configAmod(CVMUSBReadoutList::a32UserData);
static const uint8_t readAmod(CVMUSBReadoutList::a32UserBlock);

// Limits for the configuration parameters:


CConfigurableObject::limit zero(0);
CConfigurableObject::limit sixteenBits(0xffff);

CConfigurableObject::limit geoMax(0x1f);
CConfigurableObject::Limits geoRange(zero, geoMax);

CConfigurableObject::limit iplMax(0x7);
CConfigurableObject::Limits iplRange(zero, iplMax);

CConfigurableObject::limit vectorMax(0xff);
CConfigurableObject::Limits vectorRange(zero, vectorMax);

CConfigurableObject::Limits highwaterRange(zero, sixteenBits);


//////////////////////////////////////////////////////////////////////////
//////////////////////// Canonicals //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/*!
   Construction of the module. .Most of the real work is done at
   onAttach and initialization time, as well as by the configurator.
*/
C830::C830() :
  m_pConfiguration(0)
{

}
/*!
   In copy construction, we want to copy the configuration if it already exists,
   so that we have a faithful copy of the rhs object .. including configuration.
   This can create a small memory leak as the destructor does not delete this.
*/
C830::C830(const C830& rhs) :
  m_pConfiguration(0)
{
  if (rhs.m_pConfiguration) {
    m_pConfiguration = new CReadoutModule(*(rhs.m_pConfiguration));
  }
}
/*!
   Destructor...see above.
*/
C830::~C830()
{}

/*!
  Assignment returns self else copying the configuration can lead to an
  infinite recursion loop.
*/
C830&
C830::operator=(const C830& rhs)
{
  return *this;
}

//////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// CReadoutHardware interface implementations ///////////////
//////////////////////////////////////////////////////////////////////////////////////

/*!
   onAttach is called when the configuration entity is attached to this
   device.  The configuration entry must be populated with the options
   recognized by the driver for this device specifically:
\verbatim


Name        Type/range      Default       Purpose
-channels   integer         0xffffffff    Selectively enable channels.
-dwelltime  integer         0             If in periodic trigger mode, the time between triggers
-header     bool            false         If true data will include a header.
-trigger    random |        vme           Specifies how the module triggers an event. random
            periodic |                    means the external latch signal latches the scalers
            vme                           to an event, periodic means the scaler is triggered every
                                          'dwelltime' * 400ns.  vme means the readout list triggers
                                          the scaler.
-wide      bool             true          true  - Scalers are 32 bit value, false, 26 bit values.
                                          note that 26 bit wide scaler readout is tagged in the
                                          top bits with the channel number.
-autoreset bool              true         If true, the latch operation also clears the counters.
-geo       integer (0-0x1f)  0            The geograhpical address of the module (from PAUX or
                                          as programmed, see below).
-setgeo    bool              false        If true, the geographical address is programmed,
                                          this can only be done for modules without the PAUX
                                          connector...otherwise, the geo address comes from
                                          the PAUX connector.
-ipl       integer 0-7       0            If non zero enables the module to interrupt when
                                          the highwater mark is reached.
-vector    integer 0-255     0xdd         Status/id value to interrupt on.
-highwatermark integer (0-0xffff) 1       How many events need to be in the module to interrupt.

\endverbatim

\param configuration : CReadoutModule&
    The configuration that will be attached to this module.

*/
void C830::onAttach(CReadoutModule& configuration)
{
  m_pConfiguration = &configuration; 

  // Set up the options and their validators.

  m_pConfiguration->addParameter("-base",      CConfigurableObject::isInteger, NULL, "0");
  m_pConfiguration->addParameter("-channels",  CConfigurableObject::isInteger, NULL, "0xffffffff");
  m_pConfiguration->addParameter("-dwelltime", CConfigurableObject::isInteger, NULL, "0");
  m_pConfiguration->addParameter("-header",    CConfigurableObject::isBool,    NULL, "false");

  // Need the list oif valid trigger strings:

  static CConfigurableObject::isEnumParameter validTriggers;
  validTriggers.insert("random");
  validTriggers.insert("periodic");
  validTriggers.insert("vme");

  m_pConfiguration->addParameter("-trigger",   CConfigurableObject::isEnum,   &validTriggers , "vme");
  m_pConfiguration->addParameter("-wide",      CConfigurableObject::isBool,   NULL,            "true");
  m_pConfiguration->addParameter("-autoreset", CConfigurableObject::isBool,   NULL,            "true");
  m_pConfiguration->addParameter("-geo",       CConfigurableObject::isInteger,&geoRange,       "0");
  m_pConfiguration->addParameter("-setgeo",    CConfigurableObject::isBool,   NULL,            "false");
  m_pConfiguration->addParameter("-ipl",       CConfigurableObject::isInteger, &iplRange,      "0");
  m_pConfiguration->addParameter("-vector",    CConfigurableObject::isInteger, &vectorRange,   "0");
  m_pConfiguration->addParameter("-highwatermark",
				               CConfigurableObject::isInteger, &highwaterRange, "1");
  

}
/*!
    Initialize the module as desribed in the configuration that is attached to us.
    The configuration has been set up by the script which describes the hardware that
    makes up the DAQ system.

    \param controller : CVMUSB&
       The handle that allows us to communicate with the VM-USB controller that takes data
       for us.

    \note  In a departure from the 'normal' way of doing things as many one-shot things.
           we'll take the trouble to do the configuration as an immediate list of operations.
           .. except for resetting the module which may require some recovery time and hence is done
	   as a separate operation.  Hopefully this will improve initialization performance.

*/
void
C830::Initialize(CVMUSB& controller)
{
  // Just fetch the parameters first:

  uint32_t  baseAddress       = getIntegerParameter("-base");
  uint32_t  channelEnables    = getIntegerParameter("-channels");   
  uint32_t  dwellTime         = getIntegerParameter("-dwelltime");   
  bool      enableHeaders     = getBooleanParameter("-header");
  TriggerSource latchSource   = getTriggerSource();
  bool      isWide            = getBooleanParameter("-wide");
  bool      autoReset         = getBooleanParameter("-autoreset");
  uint32_t  geo               = getIntegerParameter("-geo");
  bool      setGeo            = getBooleanParameter("-setgeo"); 
  uint32_t  ipl               = getIntegerParameter("-ipl");
  uint32_t  statusId            = getIntegerParameter("-vector");
  uint32_t  highWaterMark     = getIntegerParameter("-highwatermark");

  // Reset the module:

  controller.vmeWrite16(baseAddress + RESET, configAmod, (uint16_t)0);

  // Create and build the list to initialize the module completely.

  CVMUSBReadoutList   initList;

  initList.addWrite32(baseAddress+ENABLES, configAmod, channelEnables);
  initList.addWrite32(baseAddress+DWELLTIME, configAmod, dwellTime);

  // Need to figure out the control register.

  uint16_t controlRegister = 0;
  if (enableHeaders) controlRegister |= HEADER;
  switch (latchSource) {
  case random:
  case vme:
    controlRegister |= RANDOMTRIGGER;
    break;
  case periodic:
    controlRegister |= PERIODICTRIGGER;
    break;
  }
  if (!isWide)   controlRegister |= NARROW;
  //  if (autoReset) controlRegister |= AUTORESET;

  initList.addWrite16(baseAddress+CONTROL, configAmod, controlRegister);
  if(setGeo) {
    initList.addWrite16(baseAddress+GEO, configAmod, geo);
  }
  initList.addWrite32(baseAddress+ALMOSTFULL, configAmod, highWaterMark); // Before enabling interrupts.
  initList.addWrite16(baseAddress+VECTOR, configAmod, static_cast<uint16_t>(statusId));
  initList.addWrite16(baseAddress+IPL,    configAmod, static_cast<uint16_t>(ipl));

  // Fire off the list.. it's all writes so the buffer is just a dummy long.
  

  size_t dummyLong;

  controller.executeList(initList,
			 &dummyLong, sizeof(size_t), &dummyLong);


}

/*!
   Add the readout list for this device to the set of devices in the system.
   - If the latch source is VME, we add a request to latch to the list.
   - We then add a block transfer, however the block transfer count (number of longs)
     depends on a few thing:
     - The number of bits set in the -channels configuration option, is the number of channels read.
     - If -header is true, we must add another longword for the header.

     \param list : CVMUSBReadoutList
       The VM-USB list that we will be appending to.

*/
void
C830::addReadoutList(CVMUSBReadoutList& list)
{
  uint32_t baseAddress = getIntegerParameter("-base"); // Need to know where the module lives too.
  uint32_t enables     = getIntegerParameter("-channels");
  bool     headers     = getBooleanParameter("-header");

  // compute the number of longwords to read from the MEB:


  size_t readSize = headers ? 1 : 0;  // Header or no header long.
  for (uint32_t i =0; i < 32; i++) {
    if (enables & (1 << i)) readSize++;
  }

  //  readSize += 32;		// Assume all channels enabled.

  // If the trigger is VME, we do a latch and delay 1.2usec (1usec is enough).
  // prior to doing the block read.

  if (getTriggerSource() == vme) {
    list.addDelay(50);		// Seems to need a settled bus?
    list.addWrite16(baseAddress + TRIGGER, configAmod, (uint16_t)0);
    list.addDelay(75);		// 200ns units.
  }
  // Add the transfer from the MEB

  //  list.addBlockRead32(baseAddress + MEB, readAmod, readSize);

  for (int i =0; i < readSize; i++) {
    list.addRead32(baseAddress + MEB + i*sizeof(long),
		   configAmod);
  }
  list.addWrite16(baseAddress + CLEAR, configAmod, (uint16_t)0);
  
}

/*!
  Produce a dynamically allocated, faithful copy of *this.
  This is effectively a virtualized copy constructor.

*/
CReadoutHardware*
C830::clone() const
{
  return new C830(*this);
}
//////////////////////////////////////////////////////////////////////
/////////////////////// Support functions ////////////////////////////
//////////////////////////////////////////////////////////////////////


// Return the value of an integer parameter.
// Parameters:
//    std::string name - name of the parameter.
// Returns:
//    value
// Throws a string exception (from cget) if there is no such parameter.
// caller is responsible for ensuring the parameter is an int.
//
unsigned int
C830::getIntegerParameter(string name) const
{
  string sValue =  m_pConfiguration->cget(name);
  unsigned int    value  = strtoul(sValue.c_str(), NULL, 0);

  return value;
}

//  Return the value of a bool parameter.
// Parameters:
//    std::string name - name of the parameter.
// Returns:
//   true if the value is one of: true, yes, 1, on, enabled.
bool
C830::getBooleanParameter(string name) const
{
  string sValue = m_pConfiguration->cget(name);
  set<string> trueValues;
  trueValues.insert("true");
  trueValues.insert("yes");
  trueValues.insert("yes");
  trueValues.insert("1");
  trueValues.insert("on");
  trueValues.insert("enabled");


  return (trueValues.count(sValue) != 0);
}
//
//  Return the source of the latch trigger for the module.
//  Since the enum has been validated to be a valid value,
//  and since the config parameter has been initialized to a valid value,
//  we must have a correct value for the trigger type:

C830::TriggerSource
C830::getTriggerSource() const
{
  string sValue = m_pConfiguration->cget("-trigger");
  if (sValue == "random") return random;
  if (sValue == "periodic") return periodic;
  if (sValue == "vme")      return vme;

  assert(0);			// should not happen!
}
