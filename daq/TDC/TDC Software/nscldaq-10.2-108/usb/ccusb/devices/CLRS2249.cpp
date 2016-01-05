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
#include "CLRS2249.h"

#include "CReadoutModule.h"
#include <CCCUSB.h>
#include <CCCUSBReadoutList.h>

#include <tcl.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <set>

#include <iostream>

using namespace std;


/************************************************************************/
/*                        Local constants                               */
/************************************************************************/

// Configuration value constants and limits:

static CConfigurableObject::limit One(1); 
static CConfigurableObject::limit Zero(0);
static CConfigurableObject::limit FULL16(0xffff);
static CConfigurableObject::limit LastSlot(23);

static CConfigurableObject::Limits SlotLimits(One, LastSlot); // CAMAC crate.
static CConfigurableObject::Limits IdLimits(Zero, FULL16);

/*************************************************************************/
/*    Canonical member implementation                                    */
/*************************************************************************/

/*!
   Construction - initialize the configuration to null
*/
CLRS2249::CLRS2249()
{
  m_pConfiguration = 0;

}
/*!
   Copy construction
*/
CLRS2249::CLRS2249(const CLRS2249& rhs)
{
  m_pConfiguration = 0;

  // If the sourcde has a configuration deep copy it:
  if (rhs.m_pConfiguration) {
    m_pConfiguration = new CReadoutModule(*(rhs.m_pConfiguration)); 
  }
}
/*!
   Assignemnt:
*/
CLRS2249&
CLRS2249::operator=(const CLRS2249& rhs)
{
  if ((this != &rhs) && (rhs.m_pConfiguration)) {
    m_pConfiguration = new CReadoutModule(*(rhs.m_pConfiguration));
  }
  return *this;
}
/*!
   Destruction is a no-op ast here are currently issues with deleting a
   configuration. This implies a memory leaK!!
*/
CLRS2249::~CLRS2249()
{
}

/********************************************************************/
/*   Implementing the CReadoutHardware interface                    */
/*******************************************************************/

/*!
   Attach the module to a configuration.  This is when the 
   module registers its options as well as validators
   @param configuration - Configuration object to be used by this object.
*/
void
CLRS2249::onAttach(CReadoutModule& configuration)
{
  m_pConfiguration = &configuration; // store for later.

  // Create the configuration options:

  configuration.addParameter("-slot", CConfigurableObject::isInteger,
			     &SlotLimits, "0");
  configuration.addParameter("-id", CConfigurableObject::isInteger,
			     &IdLimits,"0");
}

/*!
   Called just prior to data taking.  The module must be initialized
   Configuration parameter errors are also checked at this time.
   Errors are thrown as exceptions to ensure that data taking is not started.
   @param controller CCUSB Controller object
*/
void
CLRS2249::Initialize(CCCUSB& controller)
{
  // Validate parameters.

  int slot = getIntegerParameter("-slot");
  if(!slot) {
    throw "An LRS2249 module has not had its slot configured (-slot missing)";
  }

  // Clear the module counters.
  // Can't use checked control because there's no Q:

  checkedControl(controller,
		 slot, 0, 9,
		 string("Failed to initialize LRS 2249 n=%d, a=%d f=%d (%d)"),
		 false);

}
/*!
   Add the readout commands to the readout list.
   There's some conditionalization:
   - the function used to do the read depends on the value of -cumulative
   - whether or not the marker is inserted depends on the value of -insertid.

   @param list CCUSBReadout list that drives the read.
*/
void
CLRS2249::addReadoutList(CCCUSBReadoutList& list)
{
  int slot         = getIntegerParameter("-slot");
  int id           = getIntegerParameter("-id");


  list.addMarker(id);

  // Do a Qscan limited to 12 channels to read the data:

  list.addQScan(slot, 0, 2, 12);


    
}
/*!
   Clone self:
*/
CReadoutHardware*
CLRS2249::clone() const
{
  return new CLRS2249(*this);
}
