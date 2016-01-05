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
#include <CMADCChain.h>
#include <CReadoutModule.h>
#include <CVMUSB.h>
#include <CVMUSBReadoutList.h>
#include <CMADC32.h>
#include <CConfiguration.h>
#include <Globals.h>
#include <tcl.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "MADC32Registers.h"


using namespace std;

static CConfigurableObject::limit Zero(0);
static CConfigurableObject::limit MaxWords(1024); // should fit in a VM-USB buffer with 12modules.
static CConfigurableObject::Limits WordLimit(Zero, MaxWords);

////////////////////////////////////////////////////////////////////////////////////////////
// Canonical method implementatinos.
////////////////////////////////////////////////////////////////////////////////////////////

/*!
  Constructino is a noop.
*/
CMADCChain::CMADCChain() :
  m_pConfig(0)
{
}
/*!
   Copy construction requires configuration duplication as well as duplicatino of the
   chain list.
*/
CMADCChain::CMADCChain(const CMADCChain& rhs) :
  m_Chain(rhs.m_Chain),
  m_pConfig(0)
{
  if (rhs.m_pConfig) {
    m_pConfig = new CReadoutModule(*(rhs.m_pConfig));
  }
}
/*!
  Configurations get destroyed by themseves so:
*/
CMADCChain::~CMADCChain()
{
}
/*!
   Assignment:
*/
CMADCChain&
CMADCChain::operator=(const CMADCChain& rhs)
{
  return *this;			// not really implemented or needed.
}
///////////////////////////////////////////////////////////////////////////
// Implementation of the CReadoutHardware interface.
///////////////////////////////////////////////////////////////////////////

/*!
   Called to attach the configuration object to us.
   We have to define the parameter switches.
   The only slightly whacky thing is that we define a 
   special validity checker for the -modules switch to ensure
   that we only are handed
   - defined module names.
   - modules that are in fact CMADC32 modules.
   @param configuration - The configuration object to attach to us.
*/
void
CMADCChain::onAttach(CReadoutModule& configuration)
{
  m_pConfig  = &configuration;
  configuration.addParameter("-cbltaddress",
			     CConfigurableObject::isInteger,
			     NULL, "0");
  configuration.addParameter("-mcastaddress",
			     CConfigurableObject::isInteger,
			     NULL, "0");
  configuration.addParameter("-maxwordspermodule",
			     CConfigurableObject::isInteger,
			     &WordLimit, "512");
  configuration.addParameter("-modules",
			     CMADCChain::moduleChecker,
			     NULL, "");
}
/*!
  Initialize prior to data taking.
  The modules must be programmed with their mcast/cblt address.
  We then use the MCAST adress to program maxwords/module
  We then synchronize startup by using mcast to:
  - Disabling data acquistion
  - Setting the maxwords/event.
  - Resetting the FIFO
  - Resetting the clocks
  - Resetting the readout
  - Starting the DAQ

  In theory this should ensure that all ADCS have synchronized clocks (if they have a common
clock source and divisor), and that no ADC can accept an event prior to any other adc.
   @param controller - reference to the VMUSB controller object.
*/
void
CMADCChain::Initialize(CVMUSB& controller)
{

  // Get meaningful values for all of the configuration parameters.

  uint32_t cbltAddress    = m_pConfig->getUnsignedParameter("-cbltaddress");
  uint32_t mcastAddress   = m_pConfig->getUnsignedParameter("-mcastaddress");
  int      wordsPerModule = m_pConfig->getIntegerParameter("-maxwordspermodule");
  list<string>  moduleNames = getModules();
  namesToList(moduleNames);

  // Visit each module initialize it  and program/enable it's mcast/cblt addresses:

  for (ChainListIterator i = m_Chain.begin(); i != m_Chain.end(); i++) {
    // Figure out the chain position value:

    CMADC32::ChainPosition pos = CMADC32::middle; // Most common case:
    ChainListIterator     next = i; next++;
    if(i == m_Chain.begin()) {
      cerr << "First in chain\n";
      pos = CMADC32::first;
    }
  
    if (next  == m_Chain.end()) {
      cerr << "Last In chain\n";
      pos = CMADC32::last;
    }
    (*i)->Initialize(controller);
    (*i)->setChainAddresses(controller, pos, cbltAddress, mcastAddress);

  }
  // Now we can prep the modules  via MCAST addressing.
  // This is done by having the first module in the chain use it's
  // timing settings, its multievent settings, our max transfer,
  // and its interrupt information
  //

  ChainListIterator first = m_Chain.begin();
  (*first)->initCBLTReadout(controller, mcastAddress, wordsPerModule);

  
}
/*!
  Create the readout list for the MADC32 chain  This will be a fifo block read at the
  cblt address with a count that is the largest event size * maxwords * num_modules + 1
  That should be sufficient to ensure there's a terminating 0xffffffff
  @param rdolist - CVMUSBReadoutList we are appending our instructions to.
*/
void
CMADCChain::addReadoutList(CVMUSBReadoutList& rdolist)
{
  uint32_t location = m_pConfig->getUnsignedParameter("-cbltaddress");
  uint32_t mcast    = m_pConfig->getUnsignedParameter("-mcastaddress");

  size_t   size     = m_pConfig->getIntegerParameter("-maxwordspermodule");
  list<string>  modnames;
  modnames = getModules();
  size              = size * 36 * (modnames.size() + 1);

  rdolist.addFifoRead32(location, cbltamod, size);

  // Broadcast readout_reswet:

  rdolist.addWrite16(mcast + ReadoutReset, initamod, (uint16_t)1);
}
/*!
 * Virtual copy constructor.
 */
CReadoutHardware*
CMADCChain::clone() const
{
  return new CMADCChain(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////
// Utility functions (private).
/////////////////////////////////////////////////////////////////////////////////////////

/**
 * Returns the list of module names in the -modules option.  This is returned as an
 * stl list<std::string> Validation is assumed to already have been done by moduleChecker.
 */
list<string>
CMADCChain::getModules()
{
  // These two are for Tcl_SplitList

  int    argc;
  const char** argv;
  list   <string>   Result;	                       // This is what we'll return.

  string sValue  = m_pConfig->cget("-modules"); // String valued parameter...
  Tcl_SplitList(NULL, sValue.c_str(), &argc, &argv);   // List has already been validated.

  assert(argc > 1);		                       // Validator should have ensured this.

  // Store the names in our result list

  for (int i=0; i < argc; i++) {
    Result.push_back(string(argv[i]));
  }

  // Free the argv Tcl_SplitList dynamically allocated and return Result:

  Tcl_Free((char*)argv);
  return Result;
}
/*
 * Takes a list of module names and turns them into a list of pointers to the modules.
 * The pointers are stored in m_Chain.
 */
void
CMADCChain::namesToList(list<string> moduleNames)
{
  CConfiguration*   pModules = Globals::pConfig;
  m_Chain.clear();


  list<string>::iterator pName = moduleNames.begin();
  while(pName != moduleNames.end()) {
    string name = *pName;
    CReadoutModule* pModule = pModules->findAdc(name);
    CMADC32*        pAdc    = dynamic_cast<CMADC32*>(pModule->getHardwarePointer());
    assert(pAdc);
    m_Chain.push_back(pAdc);

    pName++;
  }
}
/**
 * Checks that a module list consists of valid MADC32 modules.
 */
bool
CMADCChain::moduleChecker(string name, string value, void* arg)
{
  // value should be a properly formatted list of strings.
  // each string should be the name of an MADC32 module.

  int     argc;			// Returns from Tcl_SplitList
  const char**  argv;

  // Validate the list and get the elements in argv:

  int status = Tcl_SplitList(NULL, value.c_str(), &argc, &argv);
  if (status != TCL_OK) {
    return false;		// not a valid tcl list
  }

  // Check that we have enough elements to make a chain:

  if (argc < 2) {		// Note that zero length lists can be legal!
    Tcl_Free((char*)argv);
    return false;
  }
  // Now iterate through the list elements and that each is a known C785.
  // dynamic_cast is used to ensure the C785-edness of CReadoutModule
  // pointers as it uses RTTI to ensure the cast is truly compatible.
  
  CConfiguration* pConfiguration = Globals::pConfig;
  
  
  for (int i = 0; i < argc; i++) {
    string moduleName        = argv[i];
    CReadoutModule* pModule = pConfiguration->findAdc(moduleName);
    if (!pModule || !dynamic_cast<CMADC32*>(pModule->getHardwarePointer())) { // Not found, or not a C785.
      Tcl_Free((char*) argv);
      return false;
    } 
  }
  // Control only gets here if all validations passed
  
  Tcl_Free((char*)argv);
  return true;		       
}

