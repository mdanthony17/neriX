/*
    This software is Copyright by the Board of Trustees of Michigan


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
#include "CMADC32.h"
#include "CReadoutModule.h"
#include <CVMUSB.h>
#include <CVMUSBReadoutList.h>

#include <tcl.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <set>

#include <iostream>
#include "MADC32Registers.h"


using namespace std;

//////////////////////////////////////////////////////////////////////
// Local constants.


/////////////////////////////////////////////////////////////////////////////////
// Data that drives parameter validity checks.

static CConfigurableObject::limit Zero(0);    // Useful for many of the integer limits.
static CConfigurableObject::limit Byte(0xff);
// Module id:

static CConfigurableObject::limit IdMax(255);
static CConfigurableObject::Limits IdLimits(Zero, IdMax);

// Interrupt priority level:

static CConfigurableObject::limit IPLMax(7);
static CConfigurableObject::Limits IPLLimit(Zero, IPLMax);

// Interrupt vector:

static CConfigurableObject::limit VectorMax(255);
static CConfigurableObject::Limits VectorLimit(Zero, VectorMax);

// List parameters have constraints on their sizes (HoldListSize),
// Value types, and parameters to the type checker (e.g. range limitations).
// These are encapsulated in isListParameter struct.
//
// Thresholds, are 32 element lists with values [0,0xfff].

static CConfigurableObject::limit ThresholdMax(0xfff);
static CConfigurableObject::Limits ThresholdLimits(Zero, ThresholdMax);
static CConfigurableObject::ListSizeConstraint ThresholdListSize = {32, 32};
static CConfigurableObject::TypeCheckInfo ThresholdValuesOk(CConfigurableObject::isInteger,
							    &ThresholdLimits);
static CConfigurableObject::isListParameter ThresholdValidity(ThresholdListSize,
							      ThresholdValuesOk);


// hold delays are a two element integer array, with no limits.


static CConfigurableObject::ListSizeConstraint HoldListSize = {2, 2};
static CConfigurableObject::Limits HoldLimits(Zero, Byte);
static CConfigurableObject::TypeCheckInfo HoldValueOk(CConfigurableObject::isInteger,
						      &HoldLimits);
static CConfigurableObject::isListParameter HoldValidity(Zero, Byte, 
							 HoldValueOk);

// Limits on irqthreshold:

static CConfigurableObject::limit irqThresholdMax(956); // that's what the manual says 
static CConfigurableObject::Limits irqThresholdLimits(Zero, irqThresholdMax);




// Note for all enums, the first item in the list is the default.


// Legal gatemode values for the enumerator:

static const char* GateModeValues[2] = {"common", "separate"};
static const int   GateModeNumValues = sizeof(GateModeValues)/sizeof(char*);

// Legal values for the adc input range:

static const char* InputRangeValues[3] = {"4v", "8v", "10v"};
static const int   InputRangeNumValues = sizeof(InputRangeValues)/sizeof(char*);

// Legal values for the timing source.

static const char* TimingSourceValues[2] = {"vme", "external"};
static const int   TimingSourceNumValues = sizeof(TimingSourceValues)/sizeof(char*);

// Legal values for the timing divisor (0-15)

static CConfigurableObject::limit divisorLimit(0xffff);
static CConfigurableObject::Limits DivisorLimits(Zero, divisorLimit);

// Legal values for the resolution...note in this case the default is explicitly defined as 8k

static const char* resolutions[5] = {
  "2k",
  "4k",
  "4khires",
  "8k",
  "8khires"
};
static const int resolutionsNumValues = sizeof(resolutions)/sizeof(char*);

////////////////////////////////////////////////////////////////////////////////
// Constructors and implemented canonical operations:

/* These are largely trivial in nature: */

CMADC32::CMADC32() :
  m_pConfiguration(0) 
{}


/*! Copy construction involves a deep copy */

CMADC32::CMADC32(const CMADC32& rhs) :
  m_pConfiguration(0)
{
  if (rhs.m_pConfiguration) {
    m_pConfiguration = new CReadoutModule(*(rhs.m_pConfiguration));
  }
}

CMADC32::~CMADC32() {}

CMADC32&
CMADC32::operator=(const CMADC32& rhs)
{
  return *this;
}
/////////////////////////////////////////////////////////////////////////////////
// Object operations:
//

/*!
   Attach the module to its configuration.
   This is called when the object has been created by the configuration software.
   we will register our configuration parameters, validators and limits.
   A pointer to the configuration object is retained so that The module configuration
   can be gotten when we need it.

   \param configuration - The Readout module that will hold our configuration.

*/
void
CMADC32::onAttach(CReadoutModule& configuration)
{

  m_pConfiguration = &configuration;

  // Create the configuration parameters.

  m_pConfiguration->addParameter("-base", CConfigurableObject::isInteger,
				 NULL, "0");
  m_pConfiguration->addParameter("-id",   CConfigurableObject::isInteger,
				 &IdLimits, "0");
  m_pConfiguration->addParameter("-ipl", CConfigurableObject::isInteger,
				 &IPLLimit, "0");
  m_pConfiguration->addParameter("-vector", CConfigurableObject::isInteger,
				 &VectorLimit, "0");
  m_pConfiguration->addParameter("-timestamp", CConfigurableObject::isBool,
				 NULL, "false");

  // Create the enumeration and register the -gatemode parameter.

  static CConfigurableObject::isEnumParameter ValidGateMode;
  for (int i=0; i < GateModeNumValues; i++) {
    ValidGateMode.insert(GateModeValues[i]);
  }
  m_pConfiguration->addParameter("-gatemode", CConfigurableObject::isEnum,
				 &ValidGateMode, GateModeValues[0]);

  // the hold delays and widths have the same list constraints.
  // just different default values.

  m_pConfiguration->addParameter("-holddelays", CConfigurableObject::isIntList,
				 &HoldValidity, "15");
  m_pConfiguration->addParameter("-holdwidths", CConfigurableObject::isIntList,
				 &HoldValidity, "20");

  m_pConfiguration->addParameter("-gategenerator", CConfigurableObject::isBool,
				 NULL, "false");
  // Input range:

  static CConfigurableObject::isEnumParameter ValidInputRange;
  for (int i = 0; i < InputRangeNumValues; i++) {
    ValidInputRange.insert(InputRangeValues[i]);
  }
  m_pConfiguration->addParameter("-inputrange", CConfigurableObject::isEnum,
				 &ValidInputRange, InputRangeValues[0]);


  m_pConfiguration->addParameter("-ecltermination", CConfigurableObject::isBool,
				 NULL, "true");
  m_pConfiguration->addParameter("-ecltiming",      CConfigurableObject::isBool,
				 NULL, "false");
  m_pConfiguration->addParameter("-nimtiming",      CConfigurableObject::isBool,
				 NULL, "false");
  // The timing source enum..

  static CConfigurableObject::isEnumParameter ValidTimingSource;
  for (int i = 0; i < TimingSourceNumValues; i++) {
    ValidTimingSource.insert(TimingSourceValues[i]);
  }
  m_pConfiguration->addParameter("-timingsource", CConfigurableObject::isEnum,
				 &ValidTimingSource, TimingSourceValues[0]);

  m_pConfiguration->addParameter("-timingdivisor", CConfigurableObject::isInteger,
				 &DivisorLimits, "15");


  m_pConfiguration->addParameter("-thresholds", CConfigurableObject::isIntList,
				 &ThresholdValidity, "0");

  m_pConfiguration->addParameter("-pulser", CConfigurableObject::isBool,
				 NULL, "false");


  // Parameters to support multi-event/irq mode

  m_pConfiguration->addParameter("-multievent", CConfigurableObject::isBool,
				 NULL, "false");
  m_pConfiguration->addParameter("-irqthreshold", CConfigurableObject::isInteger,
				 &irqThresholdLimits, "0");

  static CConfigurableObject::isEnumParameter validResolution;
  for(int i = 0; i < resolutionsNumValues; i++) {
    validResolution.insert(resolutions[i]);
  }
  m_pConfiguration->addParameter("-resolution", CConfigurableObject::isEnum,
				 &validResolution, "8k");

}
/*!
   Initialize the module prior to data taking.  We will get the initialization
   data from the configuration.  Unfortunately, there's no way to verify the
   base address we were given actually points to a module.

   \param CVMUSB&controller   References a VMSUB controller that will be used
          to initilize the module (the module is in a VME crate connected to that
          VMUSB object.
*/


void
CMADC32::Initialize(CVMUSB& controller)
{
  // Locate the module and reset it and the fifo.
  // These operations are the only individual operations and are done
  // in case time is needed between reset and the next operations on the module.
  // The remaining operations on the module will be done in 
  // a list so that they don't take so damned much time.
 

  uint32_t base = m_pConfiguration->getUnsignedParameter("-base");
  controller.vmeWrite16(base + Reset,    initamod, (uint16_t)1);
  sleep(1);
  controller.vmeWrite16(base + StartAcq, initamod, (uint16_t)0);
  controller.vmeWrite16(base + ReadoutReset, initamod, (uint16_t)1);



  CVMUSBReadoutList list;	// Initialization instructions will be added to this.

  // First disable the interrupts so that we can't get any spurious ones during init.

  list.addWrite16(base + Ipl, initamod, (uint16_t)0);
  list.addDelay(MADCDELAY);
  // Now retrieve the configuration parameters:

  uint16_t    id          = m_pConfiguration->getIntegerParameter("-id");
  uint8_t     ipl         = m_pConfiguration->getIntegerParameter("-ipl");
  uint8_t     ivector     = m_pConfiguration->getIntegerParameter("-vector");
  bool        timestamp   = m_pConfiguration->getBoolParameter("-timestamp");
  string      gatemode    = m_pConfiguration->cget("-gatemode");
  vector<int> holddelays  = m_pConfiguration->getIntegerList("-holddelays");
  vector<int> holdwidths  = m_pConfiguration->getIntegerList("-holdwidths");
  bool        gdg         = m_pConfiguration->getBoolParameter("-gategenerator");
  string      inputrange  = m_pConfiguration->cget("-inputrange");
  bool        termination = m_pConfiguration->getBoolParameter("-ecltermination");
  bool        ecltimeinput= m_pConfiguration->getBoolParameter("-ecltiming");
  bool        nimtimeinput= m_pConfiguration->getBoolParameter("-nimtiming");
  string      timesource  = m_pConfiguration->cget("-timingsource");
  int         timedivisor = m_pConfiguration->getIntegerParameter("-timingdivisor");
  vector<int> thresholds  = m_pConfiguration->getIntegerList("-thresholds");
  bool        pulser      = m_pConfiguration->getBoolParameter("-pulser");
  bool        multiEvent  = m_pConfiguration->getBoolParameter("-multievent");
  string      resolution  = m_pConfiguration->cget("-resolution");
  int         irqThreshold= m_pConfiguration->getIntegerParameter("-irqthreshold");

  // module ID:

  list.addWrite16(base + ModuleId, initamod, id); // Module id.
  list.addDelay(MADCDELAY);


  // Write the thresholds.

  for (int i =0; i < 32; i++) {
    list.addWrite16(base + Thresholds + i*sizeof(uint16_t), initamod, (uint16_t)thresholds[i]);
    list.addDelay(MADCDELAY);
  }


  list.addWrite16(base + MarkType, initamod, (uint16_t)(timestamp ? 1 : 0)); 
  list.addDelay(MADCDELAY);

  if (gatemode == string("separate")) {
    list.addWrite16(base + BankOperation, initamod, (uint16_t)1);
  }
  else {
    list.addWrite16(base  + BankOperation, initamod, (uint16_t)0);
  }									
  list.addDelay(MADCDELAY);

  // If the gate generator is on, we need to program the hold delays and widths
  // as well as enable it.

  if(gdg) {
    list.addWrite16(base + HoldDelay0, initamod, (uint16_t)holddelays[0]);
    list.addDelay(MADCDELAY);
    list.addWrite16(base + HoldDelay1, initamod, (uint16_t)holddelays[1]);
    list.addDelay(MADCDELAY);

    list.addWrite16(base + HoldWidth0, initamod, (uint16_t)holdwidths[0]);
    list.addDelay(MADCDELAY);
    list.addWrite16(base + HoldWidth1, initamod, (uint16_t)holdwidths[1]);
    list.addDelay(MADCDELAY);
    
    list.addWrite16(base + EnableGDG, initamod, (uint16_t)1);
    list.addDelay(MADCDELAY);

  } else {
    list.addWrite16(base + EnableGDG, initamod, (uint16_t)0);
    list.addDelay(MADCDELAY);
  }
  
  if (pulser) {
    list.addWrite16(base+TestPulser, initamod, (uint16_t)2);
  }
  else {
    list.addWrite16(base+TestPulser, initamod,(uint16_t)0);
  }
  list.addDelay(MADCDELAY);

  // Set the input range:

  if (inputrange == string("4v")) {
    list.addWrite16(base + InputRange, initamod,(uint16_t) 0);
  }
  else if (inputrange == string("8v")) {
    list.addWrite16(base + InputRange, initamod, (uint16_t)1);
  }
  else {			// 10V
    list.addWrite16(base + InputRange, initamod, (uint16_t)2);
  }
  list.addDelay(MADCDELAY);

  // Set the timing divisor, and clear the timestamp:


  list.addWrite16(base + TimingDivisor, initamod, (uint16_t)timedivisor);
  list.addDelay(MADCDELAY);
  list.addWrite16(base + TimestampReset, initamod, (uint16_t)3); // Reset both counters.
  list.addDelay(MADCDELAY);

  // Turn on or off ECL termination.  In fact the module provides much more control
  // over this featuer.. but for now we're all or nothing.

  if(termination) {
    list.addWrite16(base + ECLTermination, initamod, (uint16_t)0xf);
  }
  else {
    list.addWrite16(base + ECLTermination, initamod, (uint16_t)0);
  }
  list.addDelay(MADCDELAY);

  // Control which external sources can provide the timebase for the timestamp:

  if (ecltimeinput) {
    list.addWrite16(base + ECLGate1OrTiming, initamod, (uint16_t)1);
    list.addDelay(MADCDELAY);
    list.addWrite16(base + ECLFCOrTimeReset, initamod, (uint16_t)1);
    list.addDelay(MADCDELAY);
  }
  else {
    list.addWrite16(base + ECLGate1OrTiming, initamod, (uint16_t)0);
    list.addDelay(MADCDELAY);
    list.addWrite16(base + ECLFCOrTimeReset, initamod, (uint16_t)0);
    list.addDelay(MADCDELAY);
  }

  if (nimtimeinput) {
    list.addWrite16(base + NIMGate1OrTiming, initamod, (uint16_t)1);
    list.addDelay(MADCDELAY);
    list.addWrite16(base + NIMFCOrTimeReset, initamod, (uint16_t)1);
    list.addDelay(MADCDELAY);
  }
  else {
    list.addWrite16(base + NIMGate1OrTiming, initamod, (uint16_t)0);
    list.addDelay(MADCDELAY);
    list.addWrite16(base + NIMFCOrTimeReset, initamod, (uint16_t)0);
    list.addDelay(MADCDELAY);
  }

  // Source of the timebase:

  if(timesource == string("vme") ) {
    list.addWrite16(base + TimingSource, initamod, (uint16_t)0);
  }
  else {
    list.addWrite16(base + TimingSource, initamod, (uint16_t)1);
  }
  list.addDelay(MADCDELAY);
  
  // Ensure that busy is on the busy connector:

  list.addWrite16(base + NIMBusyFunction, initamod, (uint16_t)0);
  list.addDelay(MADCDELAY);

  // Process -resolution, -irqthreshold and -multievent

  list.addWrite16(base + Resolution, initamod, (uint16_t)resolutionValue(resolution));
  list.addDelay(MADCDELAY);
  list.addWrite16(base + WithdrawIrqOnEmpty, initamod, (uint16_t)1);
  if(multiEvent) {
    list.addWrite16(base + MultiEvent, initamod, (uint16_t)7);
  }
  else {
    list.addWrite16(base + MultiEvent, initamod, (uint16_t)0);
  }
  list.addDelay(MADCDELAY);

  // Finally clear the converter and set the IPL which enables interrupts if
  // the IPL is non-zero, and does no harm if it is zero.


  list.addWrite16(base + Vector,   initamod, (uint16_t)ivector);
  list.addDelay(MADCDELAY);
  list.addWrite16(base + Ipl, initamod, (uint16_t)ipl);
  list.addWrite16(base + IrqThreshold, initamod, (uint16_t)irqThreshold);
  list.addDelay(MADCDELAY);


  // Now reset again and start daq:


  list.addWrite16(base + ReadoutReset, initamod, (uint16_t)1);
  list.addDelay(MADCDELAY);
  list.addWrite16(base + InitFifo,     initamod, (uint16_t)0);
  list.addDelay(MADCDELAY);
  list.addWrite16(base + StartAcq, initamod, (uint16_t)1 );




  //  Execute the list to initialize the module:


  char readBuffer[100];		// really a dummy as these are all write...
  size_t bytesRead;
  int status = controller.executeList(list, readBuffer, sizeof(readBuffer), &bytesRead);
  if (status != 0) {
     throw string("List excecution to initialize an MADC32 failed");
   }

}
/*!
  Add instructions to read out the ADC for a event. Since we're only working in
  single even tmode, we'll just read 'too many' words and let the
  BERR terminate for us.  This ensures that we'll have that 0xfff at the end of 
  the data.
  \param list  - The VMUSB read9out list that's being built for this stack.
*/
void
CMADC32::addReadoutList(CVMUSBReadoutList& list)
{
  // Need the base:

  uint32_t base = m_pConfiguration->getUnsignedParameter("-base");

  list.addFifoRead32(base + eventBuffer, readamod, (size_t)45);
  list.addWrite16(base + ReadoutReset, initamod, (uint16_t)1);
  list.addDelay(5);
}

// Cloning supports a virtual copy constructor.

CReadoutHardware*
CMADC32::clone() const
{
  return new CMADC32(*this);
}
////////////////////////////////////////////////////////////////////////////////////////////
//
// Code here provides support for the madcchain pseudo module that use these devices
// in CBLT mode.
//

/*!
   Set up the chain/mcast addresses.
   @param controller - Handle to VM_USB controller object.
   @param position   - A value from the position enumerator CMADC32::ChainPosition
   @param cbltBase   - Base address for CBLT transfers.
   @param mcastBase  - Base address for multicast transfers.

   Note that both mcast and cblt are enabled for now.
*/
void
CMADC32::setChainAddresses(CVMUSB&                controller,
			   CMADC32::ChainPosition position,
			   uint32_t               cbltBase,
			   uint32_t               mcastBase)
{

  uint32_t base = m_pConfiguration->getIntegerParameter("-base");

  cerr << "Position: " << position << endl;

  // Compute the value of the control register..though we will first program
  // the addresses then the control register:

  uint16_t controlRegister = MCSTENB | CBLTENB; // This much is invariant.
  switch (position) {
  case first:
    controlRegister |= FIRSTENB | LASTDIS;
    cerr << "First\n";
    break;
  case middle:
    controlRegister |= FIRSTDIS | LASTDIS;
    cerr << "Middle\n";
    break;
  case last:
    controlRegister |= FIRSTDIS | LASTENB;
    cerr << "Last\n";
    break;
  }
  cerr << "Setting chain address with " << hex << controlRegister << dec << endl;

  // program the registers, note that the address registers take only the top 8 bits.

  controller.vmeWrite16(base + CbltAddress, initamod, (uint16_t)(cbltBase >> 24));
  controller.vmeWrite16(base + McstAddress, initamod, (uint16_t)(mcastBase >> 24));
  controller.vmeWrite16(base + CbltMcstControl, initamod, (uint16_t)(controlRegister));

}

/*!
   Set up data taking for CBLT readout with the timestamp parameters we are using and
   the mcast address for the chain
   @param controller - CVMUSB controller reference.
   @param mcast  - Multicast address used to program the chain.
   @param rdoSize - Words per module.
*/
void
CMADC32::initCBLTReadout(CVMUSB& controller, uint32_t mcast, int rdoSize)
{
  // We need our timing source
  // IRQThreshold
  // VECTOR
  // IPL
  // Timestamp on/off

  // Assumptions:  Internal oscillator reset if using timestamp
  //               ..else no reset.
  //               most modulep arameters are already set up.


  int irqThreshold   = m_pConfiguration->getIntegerParameter("-irqthreshold");
  int vector         = m_pConfiguration->getIntegerParameter("-vector");
  int ipl            = m_pConfiguration->getIntegerParameter("-ipl");
  bool timestamping  = m_pConfiguration->getBoolParameter("-timestamp");
  
  // Stop acquistiion
  // ..and clear buffer memory:
  controller.vmeWrite16(mcast + StartAcq, initamod, (uint16_t)0);
  controller.vmeWrite16(mcast + InitFifo, initamod, (uint16_t)0);

  // Set stamping

  if(timestamping) {
    // Oscillator sources are assumed to already be set.
    // Reset the timer:

    controller.vmeWrite16(mcast + MarkType,       initamod, (uint16_t)1); // Show timestamp, not event count.
    controller.vmeWrite16(mcast + TimestampReset, initamod, (uint16_t)3); // reset all counter.
  }
  else {
    controller.vmeWrite16(mcast + MarkType,       initamod, (uint16_t)0); // Use Eventcounter.
    controller.vmeWrite16(mcast + EventCounterReset, initamod, (uint16_t)0); // Reset al event counters.
  }
  // Set multievent mode
  
  controller.vmeWrite16(mcast + MultiEvent, initamod, (uint16_t)3);      // Multi event mode 3.
  controller.vmeWrite16(mcast + IrqThreshold, initamod, (uint16_t)irqThreshold);
  controller.vmeWrite16(mcast + MaxTransfer, initamod,  (uint16_t)rdoSize);

  // Set the IRQ

  controller.vmeWrite16(mcast + Vector, initamod, (uint16_t)vector);
  controller.vmeWrite16(mcast + Ipl,    initamod, (uint16_t)ipl);
  controller.vmeWrite16(mcast + IrqThreshold, initamod, (uint16_t)irqThreshold);

  // Init the buffer and start data taking.

  controller.vmeWrite16(mcast + InitFifo, initamod, (uint16_t)0);
  controller.vmeWrite16(mcast + ReadoutReset, initamod, (uint16_t)0);
  controller.vmeWrite16(mcast + StartAcq , initamod, (uint16_t)1);
}


///////////////////////////////////////////////////////////////////////////////////////////////
//
// Private utilities:
//

/*
** convert the resolution string into a register value.
*/
int
CMADC32::resolutionValue(string selector)
{
  for (int i =0; i < resolutionsNumValues; i++) {
    if (selector == resolutions[i]) {
      return i;
    }
  }
  // enum checking should prevent this so:

  string msg = "Invalid value for resolution parameter: ";
  msg       +=  selector;
  throw msg;
}
