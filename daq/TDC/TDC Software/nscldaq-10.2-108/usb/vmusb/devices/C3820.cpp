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
#include "C3820.h"
#include "CReadoutModule.h"
#include <CVMUSB.h>
#include <CVMUSBReadoutList.h>

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

using namespace std;

//////////////////////////////////////////////////////////////////////////
//////////////////////////  Constants ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// To reduce the development time, only the required consts are created.
//

// Register offsets of the SIS 3820 scaler module; These are byte offsets.

static const uint32_t CSR        = 0x00000000;     // Control status register.
static const uint32_t ModuleID   = 0x00000004;     // Firmware and ident.

static const uint32_t AcqMode    = 0x00000100; // Acquisition mode register.

static const uint32_t KeyReset   = 0x00000400;     // Writing here resets.
static const uint32_t KeyLNE     = 0x00000410;
static const uint32_t KeyArm     = 0x00000414;
static const uint32_t KeyEnable  = 0x00000418;

static const uint32_t ShadowCounters = 0x00000800;

static const uint32_t SDRAM      = 0x00800000;

// Register bits:
//
static const uint32_t idMask     = 0xffff0000;
static const uint32_t idValue    = 0x38200000;



// Acquisition mode bits/fields and other values:

static const uint32_t acqNonClearing   = 0x00000001; //  Don't clear on latch.
static const uint32_t acq32Bit         = 0x00000000; // Data format is 32bits.
static const uint32_t acq24bit         = 0x00000004;
static const uint32_t acq16bit         = 0x00000008;
static const uint32_t acq8bit          = 0x0000000c; 
static const uint32_t acqLNEVME        = 0x00000000;
static const uint32_t acqLNEFP         = 0x00000010;
static const uint32_t acqLNE10Mhz      = 0x00000020;
static const uint32_t acqLNEChanN      = 0x00000030;
static const uint32_t acqLNEPresetN    = 0x00000040;
static const uint32_t acqArmWithFP     = 0x00000000;
static const uint32_t acqArmWithChN    = 0x00000100;
static const uint32_t acqSRAMMemory    = 0x00001000;
static const uint32_t acqAddMode       = 0x00002000;
static const uint32_t acqInpNone       = 0x00000000;
static const uint32_t acqInpLNEInhLNE  = 0x00010000;
static const uint32_t acqInpLNEInhboth = 0x00020000;
static const uint32_t acqInpLNEInhCount= 0x00030000;
static const uint32_t acqInpLNEInh4s   = 0x00040000;
static const uint32_t acqInpInvert     = 0x00080000;
static const uint32_t acqOutModeled    = 0x00000000;
static const uint32_t acqOutMode50Mhz  = 0x00100000;
static const uint32_t acqOutInvert     = 0x00800000;
static const uint32_t acqModeLatch     = 0x00000000;
static const uint32_t acqModeMCS       = 0x20000000;
static const uint32_t acqModeHisto     = 0x30000000;
static const uint32_t acqModeTest      = 0x70000000;



//////////////////////////////////////////////////////////////////////////
////////////////////// Constructors and canonicals ///////////////////////
//////////////////////////////////////////////////////////////////////////

/*!
   Construction is a no-op.
*/
C3820::C3820() {}
C3820::C3820(const C3820& rhs) {} // m_pConfiguration gets set by onAttach next.
C3820::~C3820() {}

C3820&
C3820::operator=(const C3820& rhs) {
  return *this;
}


/////////////////////////////////////////////////////////////////////////
//////////////////////// Object operations //////////////////////////////
/////////////////////////////////////////////////////////////////////////

/*!
   Called when the object is attached to a configuration.  We
   establish our configuration parameter.
   \param configuration : CReadoutModule& 
      Reference to the module that holds our configuration.
*/
void
C3820::onAttach(CReadoutModule& configuration)
{
  m_pConfiguration = &configuration;

  // Add the base parameter:

  m_pConfiguration->addParameter("-base",
				 CConfigurableObject::isInteger, 
				 NULL, "0");
}
/*!
   Called to setup the module for data taking. We'll make a small setup list
   and submit it for immediate execution.  The base we will fetch from the
   configuration.
   \param controller : CVMUSB&
      Reference to the controller.<
*/
void
C3820::Initialize(CVMUSB& controller)
{
  // First figure out our base address:

  uint32_t base = getBase();

  // Ensure that this module is an SIS 3820 and if so reset it. 
  // these are done as single shot operations so that the reset is conditional
  // on the correct module being installed, and in case there is a delay required
  // between reset and next acces...hopefully the USB turnaround will take care of
  // it or, if not, we can insert a usleep as needed.
  //

  uint32_t id;
  int status = controller.vmeRead32(base+ModuleID,  CVMUSBReadoutList::a32UserData,
				    &id);
  if (status) {
    throw string("C3820::Initialize Single shot vme to read id register failed");
  }
  if ((id & idMask) != idValue) {
    char msg[1000];
    sprintf(msg, "C3820::Initialized, module @ 0x%08x is not an SIS3820 scaler",
	    base);
    throw string(msg);
  }
  status = controller.vmeWrite32(base+KeyReset, CVMUSBReadoutList::a32UserData, 
				static_cast<uint32_t>(0));
  if(status) {
    throw string("C3820::Initialize single shot write to key-reset faileed");
  }


  //  Now set up the initialization list:

  CVMUSBReadoutList initList;
  initList.addWrite32(base+AcqMode, CVMUSBReadoutList::a32UserData,
		      acq32Bit    | acqLNEVME     | acqArmWithFP | acqSRAMMemory |
		       acqInpLNEInh4s | acqOutModeled | acqModeLatch);
  initList.addWrite32(base+KeyArm, CVMUSBReadoutList::a32UserData, (uint32_t)0);
  initList.addWrite32(base+KeyEnable, CVMUSBReadoutList::a32UserData, (uint32_t)0);


  uint32_t inBuffer[100];
  size_t   bytesRead;
  status = controller.executeList(initList,
				      &inBuffer, sizeof(inBuffer), &bytesRead);
  if (status) {
    throw string("C3820::Could not initialize via executeList.");
  }
}
/*!
  Add the event read of this module to the list.  All we need to do is
  read 32 longwords from the sdram memory.
  \param list : CVMUSBReadoutList&
  The list to add to.
*/

void
C3820::addReadoutList(CVMUSBReadoutList& list)
{
  uint32_t base = getBase();
  list.addWrite32(base+KeyLNE, CVMUSBReadoutList::a32UserData, (uint32_t)0);
  list.addBlockRead32(base+ShadowCounters, CVMUSBReadoutList::a32UserBlock,
		      (uint32_t)32);
}

/*!
   Create a dynamic copy of *this.
*/
CReadoutHardware*
C3820::clone() const
{
  return new C3820(*this);
}
///////////////////////////////////////////////////////////////////////////
////////////////////////// Private utilities /////////////////////////////
//////////////////////////////////////////////////////////////////////////
uint32_t
C3820::getBase() const
{
  string baseString = m_pConfiguration->cget("-base");
  uint32_t base     = strtoul(baseString.c_str(), NULL, 0); // must work!
  return base;
}
