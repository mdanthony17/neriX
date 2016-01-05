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
#include "CMADCScaler.h"
#include "CReadoutModule.h"
#include <CVMUSB.h>
#include <CVMUSBReadoutList.h>

#include <tcl.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <vector>
#include <set>

using namespace std;


///////////////////////////////////////////////////////////////////////////////
// Local constants:
 

// address modifiers and offsets used:

static const uint8_t amod(CVMUSBReadoutList::a32UserData);

static const int daq_time_lo(0x60a0);
static const int daq_time_hi(0x60a2);

static const int time_0(0x60a8);
static const int time_1(0x60aa);

static const int time_reset(0x6090);


////////////////////////////////////////////////////////////////////////////
// Constructors and other canonicals


/*! normal constructor.. just clears the config pointer */

CMADCScaler::CMADCScaler() :
  m_pConfiguration(0)
{}
/*! copy constructor is adeep copy */

CMADCScaler::CMADCScaler(const CMADCScaler& rhs) :
  m_pConfiguration(0)
{
  if (rhs.m_pConfiguration) {
    m_pConfiguration = new CReadoutModule(*(rhs.m_pConfiguration));
  }
}

/*!  Destructor is a no-op...as configuration may attach and not be dynamic.
    This makes a slight memory leak of at most one longword per destrution,
    but modules are really never copy constructed.
 */
CMADCScaler::~CMADCScaler()
{}

CMADCScaler&
CMADCScaler::operator=(const CMADCScaler& rhs) {
  return *this;
}

//////////////////////////////////////////////////////////////////////////
// object operations.

/*!
  Attach a module to its configuration.
  Define the configuration parameters that we will accept:

  \param configuration - reference to the module's configuration object.
*/
void
CMADCScaler::onAttach(CReadoutModule& configuration)
{
  m_pConfiguration = &configuration;

  m_pConfiguration->addParameter("-base", CConfigurableObject::isInteger,
				 NULL, "0");

}
/*!
   To initialize the module, we just need to reset the times:

   \param controller - the object that connects us to the VM-USB.

*/
void
CMADCScaler::Initialize(CVMUSB& controller)
{
  uint32_t base = m_pConfiguration->getUnsignedParameter("-base");

  controller.vmeWrite16(base + time_reset, amod, (uint16_t)2); // Reset the ctr b block.
}
/*!
  Add the instructions to the readout list that actually read the module.

  \param list - The readout list being built up.
*/
void
CMADCScaler::addReadoutList(CVMUSBReadoutList& list)
{
  int base = m_pConfiguration->getUnsignedParameter("-base");

  list.addRead16((uint32_t)(base + daq_time_lo), (uint8_t)amod);
  list.addRead16((uint32_t)(base + daq_time_hi), (uint8_t)amod);

  list.addRead16((uint32_t)(base + time_0), (uint8_t)amod);
  list.addRead16((uint32_t)(base + time_1), (uint8_t)amod);

  // reset the time for incremental scalers....
  
  list.addWrite16((uint32_t)(base+time_reset), (uint8_t)amod, (uint16_t)2);

}

/*!
  Provide the virtual constructor clone method:

*/
CReadoutHardware*
CMADCScaler::clone() const
{
  return new CMADCScaler(*this);
}
