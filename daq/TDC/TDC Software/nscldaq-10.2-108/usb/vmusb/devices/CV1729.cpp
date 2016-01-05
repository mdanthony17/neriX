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

// Implementation of the C785 class VM-USB support for the CAEN V785.


#include <config.h>
#include <CV1729.h>


#include "CReadoutModule.h"
#include <CVMUSB.h>
#include <CVMUSBReadoutList.h>

#include <tcl.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <string>
#include <string.h>
#include <errno.h>
#include <iostream>
using namespace std;

/**
 * Local constants:
 */

/*  Use this like Const(name)  value;  */

#define Const(name) static const int name = 

// Register offsets:
// In the 1792a the registers that were 0x8xxx in the 1729
// became 0x00xx in the 1729a.

Const(RESET)           0x0800;
Const(LOADTHRESHOLD)   0x0900;
Const(STARTACQ)        0x1700;
Const(THRESHOLD)       0x0a00;
Const(DATABUFFER)      0x0d00;
Const(PRETRIGLSB)      0x1800;
Const(PRETRIGMSB)      0x1900;
Const(POSTTRIGLSB)     0X1a00;
Const(POSTTRIGMSB)     0x0b00;
Const(TRIGGERTYPE)     0x1d00;
Const(TRIGGERSRC)      0x1e00;
Const(TRIGGERREC)      0x2000;
Const(NUMCOLS)         0x2200;
Const(CHANMASKS)       0x2300;
Const(POSTSTOPLATENCY) 0x3000;
Const(POSTLATPRETRIG)  0x3100;
Const(INTERRUPT)       0x0000;
Const(SAMPLEFREQ)      0x0100;
Const(MODE)            0x0300;

// Bits in various registers:

// Trigger type register:

Const(TRGT_INTERNAL)     0x01;
Const(TRGT_EXTERNAL)     0x02;
Const(TRGT_BOTH)         0x03;
Const(TRGT_FALLING)      0x04;
Const(TRGT_ALLOWRANDOM)  0x08;
Const(TRGT_NOMASK)       0x10;
Const(TRGT_REQUIREVALID) 0x20;
Const(TRGT_BUSY_EN_IGNOR) 0x40;
Const(TRGT_BUSY_DISABLE) 0x80;

// Sampling frequency:

Const(SF_2GHZ)           1;
Const(SF_1GHZ)           2;
Const(SF_500MHZ)         4;
Const(SF_400MHZ)         5;
Const(SF_200MHz)        10;
Const(SF_100MHZ)        20;
Const(SF_50MHZ)         40;

// Mode register bits.

Const(MODE_EN_VME_IRQ)   1;
Const(MODE_14BIT_MODE)   2;
Const(MODE_AUTO_RESTART) 4;

// Configuration constraints.

static CConfigurableObject::limit ZERO(0); // Frequently used lower limit.

static CConfigurableObject::limit ThreshHi(0xfff);
static CConfigurableObject::Limits ThreshLimits(ZERO, ThreshHi);

static CConfigurableObject::limit TimeMax(0xffff);
static CConfigurableObject::Limits TimeLimit(ZERO, TimeMax); // Pretrig/posttrig


// For all enums, the first item will be the default:

static const char* triggerSource[4] = {
  "external",
  "internal",
  "both",
  NULL				// Terminates list.
};
static const unsigned triggerSourceMap[3] = { // Map enums -> values
  TRGT_EXTERNAL,
  TRGT_INTERNAL,
  TRGT_BOTH
};


static const char* triggerEdge[3] = {
  "rising",
  "falling",
  0
};
static const unsigned triggerEdgeMap[2] = {
  0,
  TRGT_FALLING
};


static const char* samplingFrequency[] = {
  "2ghz", "1ghz", "500mhz",  0
};
static const unsigned samplingMap[] = {
  SF_2GHZ, SF_1GHZ, SF_500MHZ
};



static CConfigurableObject::limit ChanMaskMax(0xf);
static CConfigurableObject::Limits ChanMask(ZERO, ChanMaskMax);

static CConfigurableObject::limit ColsMax(128);
static CConfigurableObject::Limits Columns(ZERO, ColsMax);

static CConfigurableObject::limit PostStopMax(0xff);
static CConfigurableObject::Limits PostStop(ZERO, PostStopMax);


// Address modifiers used to access the board:

static const uint8_t setupAmod(CVMUSBReadoutList::a32UserData);
static const uint8_t readAmod(CVMUSBReadoutList::a32UserBlock);

/*-------------------------------------------------------------------------------------------
 * Canonicals:
 */

/** 
 * Construction is just initializing the configuration pointer to null:
 */
CV1729::CV1729() :
  m_pConfiguration(0)
{
}
/**
 * Destruction is null:
 */
CV1729::~CV1729()
{
}

/* 
 * The only data we have is the configuration so clone that:
 */
CV1729::CV1729(const CV1729& rhs) :
  m_pConfiguration(0)
{
  if (rhs.m_pConfiguration) {
    m_pConfiguration = new CReadoutModule(*(rhs.m_pConfiguration));
  }
}

/** Assignment is no-op and reall shoudld be discontinued:
 */
CV1729&
CV1729::operator=(const CV1729& rhs)
{
  return *this;
}



/*--------------------------------------------------------------------------------------------
* Object operations:
*/

/**
 * Attach the module to its configuration.  At this time we also need to define the set of
 * configuration options we support and their constraints.
 * @param configuration - The readout moduile that will hold our configuration.
 */
void
CV1729::onAttach(CReadoutModule& configuration)
{
  m_pConfiguration = &configuration;

  // Create and limit the configuraton parameters:

  m_pConfiguration->addParameter("-base",
				 CConfigurableObject::isInteger, NULL, "0");

  m_pConfiguration->addParameter("-irqenable",
				 CConfigurableObject::isBool, NULL, "false");
  m_pConfiguration->addParameter("-threshold",
				 CConfigurableObject::isInteger, &ThreshLimits, "4095");
  m_pConfiguration->addParameter("-pretrigger",
				 CConfigurableObject::isInteger, &TimeLimit, "10240");
  m_pConfiguration->addParameter("-posttrigger",
				 CConfigurableObject::isInteger, &TimeLimit, "64");

  m_pConfiguration->addEnumParameter("-triggersource",
				     triggerSource);
  m_pConfiguration->addEnumParameter("-triggeredge",
				     triggerEdge);
  m_pConfiguration->addParameter("-triggermask",
				 CConfigurableObject::isBool, NULL, "off");

  m_pConfiguration->addParameter("-triggerchannels",
				 CConfigurableObject::isInteger, &ChanMask, "0xf");


  m_pConfiguration->addParameter("-poststoplatency",
				       CConfigurableObject::isInteger, &PostStop, "4");
  m_pConfiguration->addParameter("-postlatencypretrig", 
					CConfigurableObject::isInteger, &PostStop, "1");

  m_pConfiguration->addEnumParameter("-samplingfreq", samplingFrequency);

  m_pConfiguration->addParameter("-delay", 
					CConfigurableObject::isInteger, NULL, "0");
  
}
/**
 * Initialize the module prior to data taking.  All of the configuration parameters are used
 * to prepare the module for data taking.  Afer module reset a list is used to set the module data
 * taking parameters and then a single shot used to start the acquisition.
 * The hope is that the VM-USB requires sufficient setup time that the ADC will be stabilized prior
 * to data taking.
 *
 * @param controller - reference to the VMUSB object that provides access to the controller.
 */
void
CV1729::Initialize(CVMUSB& controller)
{
  uint32_t base = m_pConfiguration->getUnsignedParameter("-base"); // module base address.

  controller.vmeWrite32(base+RESET, setupAmod, (uint32_t)0); // Reset board.
  usleep(500);					   // Wait a bit to ensure it complete.

  // After reset is done all of the parameters can be set by list operations:

  CVMUSBReadoutList setupList;
  setupList.addWrite32(base + THRESHOLD, setupAmod,
		       (uint32_t)(m_pConfiguration->getIntegerParameter("-threshold")));
  setupList.addWrite32(base + LOADTHRESHOLD, setupAmod, (uint32_t)0);

  // Pre and post trigger times are 2 separate 16b bit registers:

  uint32_t preTriggerTime  = m_pConfiguration->getIntegerParameter("-pretrigger");
  uint32_t postTriggerTime = m_pConfiguration->getIntegerParameter("-posttrigger");

  setupList.addWrite32(base + PRETRIGLSB, setupAmod, preTriggerTime & 0xff);
  setupList.addWrite32(base + PRETRIGMSB, setupAmod, (preTriggerTime >> 8) & 0xff);

  setupList.addWrite32(base + POSTTRIGLSB, setupAmod, postTriggerTime & 0xff);
  setupList.addWrite32(base + POSTTRIGMSB, setupAmod, (postTriggerTime >> 8) & 0xff);
  
  // Compute the value of the trigger type register:

  uint32_t triggerSourceReg = 0;
  triggerSourceReg |= enumToValue(m_pConfiguration->cget("-triggersource").c_str(),
			       triggerSource, triggerSourceMap);
  triggerSourceReg |= enumToValue(m_pConfiguration->cget("-triggeredge").c_str(),
			       triggerEdge, triggerEdgeMap);
  if (!m_pConfiguration->getBoolParameter("-triggermask")) {
    triggerSourceReg |= TRGT_NOMASK;
  }
  setupList.addWrite32(base+TRIGGERTYPE, setupAmod, triggerSourceReg);

  // Trigger source channel mask:

  setupList.addWrite32(base+TRIGGERSRC, setupAmod, 
		       (uint32_t)m_pConfiguration->getIntegerParameter("-triggerchannels"));

  // Columns to read:

  setupList.addWrite32(base+NUMCOLS, setupAmod,
		       (uint32_t)128);
  // Channel mask (affects total read size).


  // Post latency crap:

  setupList.addWrite32(base+POSTSTOPLATENCY, setupAmod,
		       (uint32_t)m_pConfiguration->getIntegerParameter("-poststoplatency"));
  
  setupList.addWrite32(base+POSTLATPRETRIG, setupAmod,
		       (uint32_t)m_pConfiguration->getIntegerParameter("-postlatencypretrig"));

  // And of course the sampling frequency:

  setupList.addWrite32(base + SAMPLEFREQ, setupAmod,
		      enumToValue(m_pConfiguration->cget("-samplingfreq").c_str(), 
				  samplingFrequency, samplingMap));

  // Set the interrupt enable bit if required:

  if (m_pConfiguration->getBoolParameter("-irqenable")) {
    setupList.addWrite32(base + MODE, setupAmod, (uint32_t)1);
  }
  else {
    setupList.addWrite32(base + MODE, setupAmod, (uint32_t)0);
  }

  // Do the setup:

  uint32_t rbuffer;
  size_t   readSize;

  int status = controller.executeList(setupList,
				      &rbuffer, 
				      sizeof(rbuffer),
				      &readSize);
  if(status != 0) {
    throw std::string(strerror(errno)) + std::string("CV1729::Initialize - list execution failed");
  }

  // start data taking:

  controller.vmeWrite32(base + STARTACQ, setupAmod, (uint32_t)0);
}

/**
 * Add items to the readout list for this module.
 * In order to allow the user to diagnose timing issues, we are going to 
 * first read the interrupt register, and put that in the readout list.
 * that should be nonzero if the module has data and zero otherwise..
 * @param list   - The CVMUSBReadout list reference we are adding to.
 */
void
CV1729::addReadoutList(CVMUSBReadoutList& list)
{
  uint32_t base     = m_pConfiguration->getUnsignedParameter("-base");
  int      cols     = 128;;
  int      delay    = m_pConfiguration->getIntegerParameter("-delay"); 
  delay = delay*5;						       // VM-USB wants it in 200ns units./

  // The delay count can only be 8 bits worth of counter:

  while(delay) {
    int thisDelay = delay;
    if (thisDelay > 0xff) {
      thisDelay = 0xff;
    }
    list.addDelay(thisDelay);
    delay -= thisDelay;
  }

  // Read the interrupt register (does that make a 16 bit data item?.

  list.addRead32(base + INTERRUPT, setupAmod);

  // Read the trigger rec to get the column the trigger was in.

  list.addRead32(base + TRIGGERREC, setupAmod);

  // Figure out the size of the block read
  // Each column is 20*(# of channels) words
  // There's a header consisting of:
  //  First sample for each channel,
  //  Verniers for each channel.
  // Reset baseline for each channel
  // And a trailer consisting of:
  // Trigger_rec
  // Valp_cp
  // Vali_cp

  int channels = 4;		// Count the channels.
  int totalTransferCount = (20*channels*cols + 3*channels)/2  + 2;

  cout << "Channels: " << channels << " Columns: " << cols << endl;
  cout << "Total transfer count is " << totalTransferCount << " longs\n";


  list.addFifoRead32(base + DATABUFFER, readAmod, (size_t)totalTransferCount);

  // reset the interrupt bit:

  list.addWrite32(base + INTERRUPT, setupAmod, (uint32_t)0);
  list.addWrite32(base + STARTACQ, setupAmod, (uint32_t)1);

  

}

/**
 * Clone the object:  This is basically a virtual constructor:
 * @return CReadoutHardware*
 */
CReadoutHardware*
CV1729::clone() const
{
  return new CV1729(*this);
}
/*--------------------------------------------------------------------------------------------
 * Private utilities
 */

/**
 *  map an enumerator value to its meaning.
 * @param pValue        Textual value of the enumerator to map.
 * @param ppLegalValues Null terminated list of legal values.
 * @param pMap          Pointer to map of values e.g. ppLegalValues[i] => pMap[i]
 * @return unsigned
 * @retval The unsigned value that corresponds to pValue using the ppLegalValues -> pMap mapping
 * @throws std::string if pValue is not an element of ppLegalValues.
 */
unsigned
CV1729::enumToValue(const char* pValue, const char** ppLegalValues, const unsigned int* pMap)
{
  int i =  0;
  while (*ppLegalValues) {
    if (strcmp(pValue, ppLegalValues[i]) == 0) {
      return pMap[i];
    }
    i++;
  }
  string msg = "No matching value for : ";
  msg       += pValue;
  msg       += " in CV1729::enumToValue";
  throw msg;
}
