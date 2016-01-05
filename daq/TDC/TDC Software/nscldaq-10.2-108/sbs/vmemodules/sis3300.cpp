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


//   Implementation of the SIS3300 readout class.
//   This software reads out the SIS 3300 flash adc .
//   See the doxygen html, or comments in the header for restrictions and
//   usage information.
//
//   Ron Fox
//   NSCL
//   Michigan State University
//   East Lansing, MI 48824-1321
//   mailto: fox@nscl.msu.edu
//
// Copyright (c) 2001 NSCL all rights reserved.
//
#include <config.h>

using namespace std;		// Needed here for spectrodaq



#include <sis3300.h>
#include <string>
#include <CVMEInterface.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>


#include <iostream>
#include <iomanip>



const char* sDevice="/dev/vme32d32";
const uint32_t nDeviceSize = 0x383000;
const uint32_t K = 1024L;

// Bits in the control register: Each control has a set/unset bit (J/K flip
// flop control).

const uint32_t CRLedOn                    =        1;
const uint32_t CRUserOutputOn             =        2;
const uint32_t CREnableTriggerOutput      =        4;
const uint32_t CRInvertTriggerOutput      = 0x000010;
const uint32_t CRTriggerOnArmedAndStarted = 0x000020;
const uint32_t CRLedOff                   = 0x010000;
const uint32_t CRUserOutputOff            = 0x020000;
const uint32_t CREnableUserOutput         = 0x040000;
const uint32_t CRNormalTriggerOutput      = 0x100000;
const uint32_t CRTriggerOnArmed           = 0x200000;

// Bits in the status register:

const uint32_t SRLedStatus            =        1;
const uint32_t SRUserOutputState      =        2;
const uint32_t SRTriggerOutputState   =        4; 
const uint32_t SRTriggerIsInverted    = 0x000010;
const uint32_t SRTriggerCondition     = 0x000020; //1: armed and started
const uint32_t SRUserInputCondition   = 0x010000;
const uint32_t SRP2_TEST_IN           = 0x020000;
const uint32_t SRP2_RESET_IN          = 0x040000;
const uint32_t SRP2_SAMPLE_IN         = 0X080000;


// Bits in the data acquisition control register:
//
const uint32_t DAQSampleBank1On       = 0x00000001;
const uint32_t DAQSampleBank2On       = 0x00000002;
const uint32_t DAQEnableHiRARCM       = 0x00000008;
const uint32_t DAQAutostartOn         = 0x00000010;
const uint32_t DAQMultiEventOn        = 0x00000020;
const uint32_t DAQStopDelayOn         = 0x00000080;
const uint32_t DAQStartDelayOn        = 0x00000040;
const uint32_t DAQEnableLemoStartStop = 0x00000100;
const uint32_t DAQEnableP2StartStop   = 0x00000200;
const uint32_t DAQEnableGateMode      = 0x00000400;
const uint32_t DAQEnableRandomClock   = 0x00000800;
const uint32_t DAQClockSetMask        = 0x00007000;
const uint32_t DAQDisableHiRARCM      = 0x00080000;
const uint32_t DAQClockSetShiftCount  = 12;
const uint32_t DAQSampleBank1Off      = 0x00010000;
const uint32_t DAQBusyStatus          = 0x00010000;
const uint32_t DAQSampleBank2Off      = 0x00020000;
const uint32_t DAQAutostartOff        = 0x00100000;
const uint32_t DAQMultiEventOff       = 0x00200000;
const uint32_t DAQStopDelayOff        = 0x00800000;
const uint32_t DAQStartDelayOff       = 0x00400000;
const uint32_t DAQDisableLemoStartStop= 0x01000000;
const uint32_t DAQDisableP2StartStop  = 0x02000000;
const uint32_t DAQDisableGateMode     = 0x04000000;
const uint32_t DAQDisableRandomClock  = 0x08000000;
const uint32_t DAQClockClearMask      = 0x70000000;
const uint32_t DAQCLockClearShiftCount= 28;


// Bits and fields in the event configuration register.
//

const uint32_t ECFGPageSizeMask      = 7;
const uint32_t ECFGPageSizeShiftCount= 0;
const uint32_t ECFGWrapMask          = 8;
const uint32_t ECFGWrapShiftCount    = 3;
const uint32_t ECFGRandomClock       = (1 << 11);

// Bits and fields in the threshold register.
const uint32_t THRLt                 =0x8000;
const uint32_t THRChannelShift    = 16;

// Bits and fields in the event directory longs:
//

const uint32_t EDIREndEventMask(0x1ffff);
const uint32_t EDIRWrapFlag(0x80000);

// HiRA firmware is a pre-requisite to using the HiRA
// indpendent random clock mode.

const uint32_t HIRAFWMAJOR = 0x13;
const uint32_t HIRAFWMINOR = 0x05;


/*!
  Constructs an object which provides access to the SIS3300.
  The initial configuration of the module will be:
  - Clock source:  Internal 100Mhz
  - Start/stop delay disabled
  - Gate mode disabled 
  - Start is trigger
  - Acquisition size is 128K
  - Page wrap disabled. 

  \param nBaseAddress - The VME base address of the module.
  */
CSIS3300::CSIS3300(uint32_t nBaseAddress,
		   unsigned int           nCrate) :
  m_nBase(nBaseAddress),
  m_nFd(0),
  m_pCsrs(0),
  m_pEi1(0),
  m_pEi2(0),
  m_pEi3(0),
  m_pEi4(0),
  m_pModuleId(0),
  m_pCsr(0),
  m_pAcqReg(0),
  m_pResetKey(0),
  m_pStart(0),
  m_pEventConfig(0),
  m_pStartDelay(0),
  m_pStopDelay(0),
  m_pAddressReg1(0),
  m_pAddressReg2(0),
  m_pAddressReg3(0),
  m_pAddressReg4(0),
  m_pEventDirectory1(0),
  m_pEventDirectory2(0),
  m_pEventDirectory3(0),  
  m_pEventDirectory4(0),
  m_fStartDelayEnabled(false),
  m_nStartDelayClocks(0),
  m_fStopDelayEnabled(false),
  m_nStopDelayClocks(0),
  m_fGateMode(false),
  m_fRandomClock(false),
  m_fLemoStartStop(false),
  m_fP2StartStop(false),
  m_fHiRA_RCM(false),
  m_fStopTrigger(false),
  m_fPageWrap(false)
{
  m_eClock = Internal100Mhz;
  m_pBank1Buffers[0] = 0;  
  m_pBank1Buffers[1] = 0;  
  m_pBank1Buffers[2] = 0;  
  m_pBank1Buffers[3] = 0;

  m_pThresholds[0]   = 0;
  m_pThresholds[1]   = 0;
  m_pThresholds[2]   = 0;
  m_pThresholds[3]   = 0;

  for(int i = 0; i < 8; i++) {	// This threshold is never made.
    m_fThresholdLt[i] = true;
    m_nThresholds[i]  = 0;
  }


  //  Event config register:






  // Map to the base address in A32/D32 space.


  void *p = CVMEInterface::Open(CVMEInterface::A32, nCrate);
  m_nFd = p;
  m_pCsrs  = (volatile uint32_t*)CVMEInterface::Map(p, m_nBase, 
							  0x3000);

  

  // Calculate and store the pointers to the useful module registers.

  // CSR Block:

  m_pModuleId    = m_pCsrs + (4/sizeof(uint32_t));

  // If the module is not an SIS 3300 or 3301 according to the
  // module Id register throw a hissy fit:

  uint32_t id    = *m_pModuleId;
  uint32_t model = (id >> 16) & 0xffff;
  if ((model != 0x3300) && (model != 0x3301)) {
    CVMEInterface::Unmap(m_nFd, (void*)m_pCsrs, 0x3000);
    CVMEInterface::Close(m_nFd);
    char error[1024];
    sprintf(error, "CSIS3300: Module at %08lx is not a 3300 nor a 3301\n",
	    (unsigned long)m_nBase);
    throw string(error);
  }
  

  m_pCsr         = m_pCsrs;
  m_pAcqReg      = m_pCsrs + (0x10/sizeof(uint32_t));
  m_pResetKey    = m_pCsrs + (0x20/sizeof(uint32_t));
  m_pStart       = m_pCsrs + (0x30/sizeof(uint32_t));
  m_pStop        = m_pCsrs + (0x34/sizeof(uint32_t));
  m_pStartDelay  = m_pCsrs + (0x14/sizeof(uint32_t));
  m_pStopDelay   = m_pCsrs + (0x18/sizeof(uint32_t));


  m_pEventConfig = (volatile uint32_t*)CVMEInterface::Map(m_nFd,
						    m_nBase + 0x100000, 0x10);


  //

  m_pEi1  = (volatile uint32_t*)CVMEInterface::Map(m_nFd, 
							m_nBase + 0x200000, 
							0x3000);

  m_pThresholds[0] = m_pEi1 + (4/sizeof(uint32_t));
  m_pAddressReg1    = m_pEi1 + (8/sizeof(uint32_t));
  m_pEventDirectory1= m_pEi1 + (0x1000/sizeof(uint32_t));

  m_pEi2 = (volatile uint32_t*)CVMEInterface::Map(m_nFd, 
						       m_nBase + 0x280000, 
						       0x3000);
  m_pThresholds[1] = m_pEi2 + (4/sizeof(uint32_t));
  m_pAddressReg2    = m_pEi2 + (8/sizeof(uint32_t));
  m_pEventDirectory2= m_pEi2 + (0x1000/sizeof(uint32_t));

  m_pEi3 = (volatile uint32_t*)CVMEInterface::Map(m_nFd, 
						       m_nBase + 0x300000, 
						       0x3000);
  m_pThresholds[2] = m_pEi3 + (4/sizeof(uint32_t));
  m_pAddressReg3    = m_pEi3 + (8/sizeof(uint32_t));
  m_pEventDirectory3= m_pEi3 + (0x1000/sizeof(uint32_t));
 
  m_pEi4 = (volatile uint32_t*)CVMEInterface::Map(m_nFd, 
						       m_nBase + 0x380000, 
						       0x3000);
  m_pThresholds[3] = m_pEi4 + (4/sizeof(uint32_t));
  m_pAddressReg4    = m_pEi4 + (8/sizeof(uint32_t));
  m_pEventDirectory4= m_pEi4 + (0x1000/sizeof(uint32_t));


  


  // Set module modes which should be done via internal functions to
  // ensure internal self consistency.

  SetSampleSize(Sample128K);

  
}

/*!
  Destroys the current sis3300.  We do the following:
  1. munmap 
  2. Ask the driver to forget the map.
  */
CSIS3300::~CSIS3300()
{
  CVMEInterface::Unmap(m_nFd, (void*)m_pCsrs, 0x3000);
  CVMEInterface::Unmap(m_nFd, (void*)m_pEventConfig, 0x10);
  CVMEInterface::Unmap(m_nFd, (void*)m_pEi1, 0x3000);
  CVMEInterface::Unmap(m_nFd, (void*)m_pEi2, 0x3000);
  CVMEInterface::Unmap(m_nFd, (void*)m_pEi3, 0x3000);
  CVMEInterface::Unmap(m_nFd, (void*)m_pEi4, 0x3000);
		      
  CVMEInterface::Close(m_nFd);
}
/*!
  Sets the clock sourc.  The clock source is determined by 
  the enumerator:  ClockSource It can be a range of internal frequencies
  or it can be an external clock on the front panel or p2 connector.
  
  \param eSource - clock source.

  */
void
CSIS3300::SetClock(ClockSource eClock)
{
  m_eClock = eClock;
}
/*!
  Sets the start delay:  The start pulse is first delayed by a fixed number
of clock pulses before being fed to the fadc logic:  This allows the adc to be 
operated in a pre-trigger mode (trigger comes before actual signal).

\param Enable [false] - If true the start delay is enabled, by default, or
                        if false, the start delay is disabled.
\param nClocks [0]    - Number of clocks by which to delay the delivery of
                        the start clock.
 */
void
CSIS3300::SetStartDelay(bool Enable, unsigned int nClocks)
{
  m_fStartDelayEnabled = Enable;
  m_nStartDelayClocks  = nClocks;
}
/*!
  Sets the stop delay.  The stop pulse is first delayed by a fixed number
 of clock pulses before being fed into the fadc logic.  This allows the adc
to be operated in a post trigger mode (trigger comes after the actual
signal).  To operate in post trigger mode:
-# Set trigger on stop
-# enable wrap
-# Disable gate mode,
-# Disable HiRA Random Clock Mode,
-# Send the trigger into the stop input.
-# set the stop delay appropriately to sample the segment of the signal you
   want to see.
-# programmatically issue the start.

*/
void
CSIS3300::SetStopDelay(bool Enable, unsigned int nClocks)
{
  m_fStopDelayEnabled = Enable;
  m_nStopDelayClocks  = nClocks;
}
/*!
  Enables or disables gate mode.  In gate mode, the module
  will start on the falling edge of Start and stop on the rising edge.
  Note that the pre-post trigger modes are pretty useless in gatemode.

  \param Enable [false] - If true gate mode is enabled.
  */
void
CSIS3300::GateMode(bool Enable)
{
  m_fGateMode = Enable;
}
/*!
  Enables or disables random clock mode. In this mode, 
  the sampling is based on an arbitrary and external random
  clock.  (See SIS3300 Manual).  Note that pipelining must be
  taken into account, and the module must be
  disarmed during a read, and then re-armed after.

  \param Enable [false] - If true random clock mode is enabled.
  */
void
CSIS3300::RandomClock(bool Enable)
{
  m_fRandomClock = Enable;
}
/*!
   Enables or disables the lemo start/stop inputs.
   \param Enable [false] - if true, lemo start /stop inputs are enabled.
*/
void
CSIS3300::LemoStartStop(bool Enable)
{
   m_fLemoStartStop = Enable;
}
/*!
   Enables ore disables the P2 start/stop input.
*/
void
CSIS3300::P2StartStop(bool Enable)
{
   m_fP2StartStop = Enable;
}
/*!
  Enables or disables the special HiRA Random Clock mode.  
  In this mode, individual LEMO inputs act as independent clock
  inputs for corresponding independent ADC inputs.

  \param Enable [false] - If true HiRA Random Clock is enabled.

  \throw string
    If the module does not have the hira firmware installed a
    string exception describing this is thrown:
  */
void
CSIS3300::HiRA_RCM(bool Enable)
{
  if((!haveHiRAFirmware()) && Enable) {    
    throw string("Attempting to use HiRA Random clock with non HiRA firmware");
  }
  else {
    m_fHiRA_RCM = Enable;
  }
}
/*!
  Sets the trigger mode to trigger on stop.. If enabled, the module triggers
when the stop pulse occurs.  This is usually only used in a post trigger mode.
\param Enabel [true] - If true trigger on stop is used otherwise trigger on
                       start.
*/
void
CSIS3300::TriggerOnStop(bool Enable)
{
  m_fStopTrigger = Enable;
}
/*!
  Sets the number of samples in an event.  This is reflected by both 
m_ePagessize and m_nPagesize.   
\param ePagesize - One of the possible values of the enumerator SampleSize
                   indicating the number of samples in an event.
  */
void
CSIS3300::SetSampleSize(SampleSize ePagesize)
{
  m_ePagesize = ePagesize;
  switch (m_ePagesize) {
  case Sample128K:
    m_nPagesize = 128*K;
    break;
  case Sample16K:
    m_nPagesize = 16*K;
    break;
  case Sample4K:
    m_nPagesize = 4*K;
    break;
  case Sample2K:
    m_nPagesize = 2*K;
    break;
  case Sample1K:
    m_nPagesize = K;
    break;
  case Sample512:
    m_nPagesize = 512;
    break;
  case Sample256:
    m_nPagesize = 256;
    break;
  case Sample128:
    m_nPagesize = 128;
    break;
  }
}

/*!
  Sets the number of samples in an event.  This is reflected by both 
m_ePagessize and m_nPagesize.   
Modified from original version to allow any sample size.
/param ePagesize - Sample size.  Must be less than 128K.
*/
/********
void
CSIS3300::SetSampleSize(unsigned int ePagesize)
{
  m_ePagesize = ePagesize;
  if(m_ePagesize>128000) m_ePagesize = 128000;
  m_nPagesize = m_ePagesize;
}
*/

/*!
  Enable the page wrap.  In page wrap mode, there is no implied stop when an
event page is full.  The event just wraps around to the start of the event
in the event buffer.  This is used in post trigger mode:  In that mode, you
manually start acquisition and let the samples come in until the delayed stop
stops acquisition.  At that time the module contains a set of points prior to
and after the actual stop time.
\param Enable [true]  - If true enables the wrap, if false, disables.
*/
void
CSIS3300::EnableWrap(bool Enable)
{
  m_fPageWrap = Enable;
}

/*!
  Returns the current clock source as the enumerated value.

  */
CSIS3300::ClockSource
CSIS3300::getCurrentClockSource()
{
  return m_eClock;
}
/*!
   Returns:
   - true if the module has start delays enabled.
   - false if start delay is disabled.
   */
bool
CSIS3300::isStartDelayEnabled()
{
  return m_fStartDelayEnabled;
}
/*!
  Returns
  - true if the module has stop delays enabled.
  - false if the stop delay is disabled.
  */
bool
CSIS3300::isStopDelayEnabled()
{
  return m_fStopDelayEnabled;
}
/*!
  Returns the number of clocks in the start delay.  
  \note
  If the start delay is disabled, this number is meaningless.

  */
unsigned int
CSIS3300::getStartDelayClocks()
{
  return m_nStartDelayClocks;
}
/*!
  Returns the number of clocks in the stop delay.
  \note
  If the stop delay is disabled, this number is meaningless.


  */
unsigned int
CSIS3300::getStopDelayClocks()
{
  return m_nStopDelayClocks;
}
/*!
  Sets the adc trigger thresholds. Each channel has a threshold register. 
  If the digitized value exceeds the threshold in any channel, a trigger
  output is generated.  This allows the ADC's to operate in free run
  self triggered mode.
  For each channel the caller must provide a flag, which if true, indicates
  the trigger fires for signals less than the threshold, or if false, indicates
  the trigger fires for signals greater than the threshold.  For each
  channel, the caller must provide a threshold value.  Note that for 
  bipolar input adc's zero level is 0x800.
  \param pLessThan - Array of 8 flages which if true cause the corresponding
          channel's threshold to be a "less than" trigger.
  \param pValues   - Array of 8 12 bit values which provide the thresholds.
          the top bites of each value will be discarded.
 */
void
CSIS3300::SetThresholds(bool* pLessthan, unsigned int* pValues)
{
  for(int i = 0; i < 8; i++) {
    m_fThresholdLt[i] = *pLessthan++;
    m_nThresholds[i]  = (*pValues++) & 0x3fff;
  }
}

/*!
  Initializes the data module for data acquisition.  Until this member is 
called, none of the modules registeres have been touched.   This member 
examines the member data and programs the module accordingly.
*/
void
CSIS3300::InitDaq()
{

  *m_pResetKey = 0;		// Reset the module.

  // Set up the Status register pointed to by m_pCsr:
  //   LED off, Output is trigger. Trigger output is univerted.
  //   Trigger on armed if trigger is start, or armed and started if trigger
  //   is stop.
  unsigned csrmask = CRLedOff | CRUserOutputOff | CREnableTriggerOutput |
                     CRNormalTriggerOutput;
  if(m_fStopTrigger) {
    csrmask |= CRTriggerOnArmedAndStarted;
  }
  else {
    csrmask |= CRTriggerOnArmed;
  }
  *m_pCsr = csrmask;
  
  // Set up the Acquisition control register *m_pAcqReg according to:
  // Enable sample clock for bank 1, Enable multievent mode,
  // m_fStartDelayEnabled, m_fStopDelayEnabled, Front panel start/stop enabled.
  // P2 disabled, m_fGateMode, m_eClock  
  //  Note that the rest has turned all the bits off:
  // Note: Multievent mode is required for page size confinement e.g.
  // for post trigger modes and should be harmless for the start modes.
  
  *m_pAcqReg = 0xffff0000;
	//  csrmask = DAQEnableLemoStartStop |     DAQMultiEventOn;
  csrmask = 0;

  if(m_fStartDelayEnabled) {
    csrmask |= DAQStartDelayOn;
    *m_pStartDelay = m_nStartDelayClocks;
  }
  if(m_fStopDelayEnabled) {
    csrmask |= DAQStopDelayOn;
    *m_pStopDelay = m_nStopDelayClocks;
    
  }
  if(m_fGateMode) {
    csrmask |= DAQEnableGateMode;
  }
  if(m_fRandomClock) {
    csrmask |= DAQEnableRandomClock;
  }
  if(m_fLemoStartStop) {
    csrmask |= DAQEnableLemoStartStop;
  }
  if(m_fP2StartStop) {
    csrmask |= DAQEnableP2StartStop;
  }

  /* Added by M. Famiano  to Enable the HiRA RCM */


  csrmask |= (m_eClock << DAQClockSetShiftCount) ;



  if(m_fHiRA_RCM) {
    csrmask |= DAQEnableHiRARCM;
  }
  /************************************************/

  *m_pAcqReg = csrmask;


  // Configure the global event configuration register.
  // We set the event size in this register from m_ePagesize and
  // we set the wrap bit from m_fPageWrap
  //
  csrmask = m_ePagesize << ECFGPageSizeShiftCount;
  if(m_fPageWrap) csrmask |= ECFGWrapMask;
  if(m_fRandomClock) csrmask |= ECFGRandomClock;

    
  *m_pEventConfig = csrmask;

  // Configure the channel thresholds.
  
  int tchan = 0;
  for(int i =0; i < 4; i++) {
    uint32_t eventhresh = m_nThresholds[tchan];
    if(m_fThresholdLt[tchan]) eventhresh |= THRLt;
    tchan++;
    uint32_t oddthresh = m_nThresholds[tchan];
    if(m_fThresholdLt[tchan]) oddthresh |= THRLt;
    tchan++;

    *(m_pThresholds[i]) = (oddthresh << THRChannelShift) | eventhresh;
  }
  
  // Enable sampling into bank1.
  
  *m_pAcqReg = DAQSampleBank1On;	// Arm sampling into bank1.
}

/*The following functions added by Mike Famiano */

/*!
  Light On.  Turns the User Led On.
*/
void
CSIS3300::LightOn()
{
  *m_pCsrs = 0x1;
}
/*!
  Light Off.  Turns the User Led Off.
*/
void
CSIS3300::LightOff()
{
  *m_pCsrs = 0x10000;
}

/*!
  User Out Enable.  Enables the User Output (Trigger Out Off).
*/
void
CSIS3300::EnableUserOut()
{
  *m_pCsrs = 0x2;
}

/*!
  User Out Disable.  Disables the User Output (Trigger Out On).
*/
void
CSIS3300::DisableUserOut()
{
  *m_pCsrs = 0x4;
}

/*!
  User Out Strobe.  Strobes the User Output for time microseconds.
  User Out must be enabled.
  \param time - Length of Strobed pulse in microseconds.
*/
void
CSIS3300::StrobeUserOut(int time)
{
 *m_pCsrs = 0x2;
 usleep(time);
 *m_pCsrs = 0;
}

/*!
  Get User Input.  Checks the status of the user input port.
  /return 0 - NIM Low.
  /return 1 - NIM High.
*/
unsigned int
CSIS3300::GetUserInput()
{
  return (*m_pCsrs&SRUserInputCondition)>>16;
}

/*!
  Starts sampling.  This is used in stop trigger mode.  In that case,
  sampling could be manually started and then stopped due to an external 
  signal (e.g. posttrigger).
  */
void
CSIS3300::StartSampling()
{
  *m_pStart = 0;
}

/*!
  Stops sampling.  This is used in stop trigger mode.  In that case,
  sampling could be manually started and then stopped due to an external 
  signal (e.g. posttrigger).
  */
void
CSIS3300::StopSampling()
{
  *m_pStop = 0;
}


/*The Previous functions added by Mike Famiano */

/*!
   Waits until data taking for an event is done.  This happens when
   the system becomes diarmed, as evidenced by the fact that the
   memory bank1 sample clock is no longer enabled.
   \param timeout - Number of loop passes to take while waiting.
   \return bool - True if the wait resulted in a done module.
   */
bool
CSIS3300::WaitUntilDone(int timeout)
{
  int i;
  for(i = 0; i < timeout; i ++) {
    if(((*m_pAcqReg) & DAQBusyStatus) == 0) break;
  }

  if(i >= timeout) return false;

  uint32_t  last = *m_pAddressReg1;
  while(last != *m_pAddressReg1) 
    last = *m_pAddressReg1;		// Wait for address reg. to stabilize.

  *m_pAcqReg = DAQSampleBank1Off | DAQSampleBank2Off ; // Disable sampling

  return true;

}
/*!
  Returns the number digitized addresses (events) in a memory bank.
*/

uint32_t
CSIS3300::EventNumber(int bank)
{
  uint32_t last;
  if(bank==1)last = *m_pAddressReg1;
  else if(bank==2)last = *m_pAddressReg2;
  else if(bank==3)last = *m_pAddressReg3;
  else last = *m_pAddressReg4;

  return last;
}

/*!
   Clears the Module. This case that means re-enabling the
   Bank 1 sampling bit in the DAQ register.
   */
void 
CSIS3300::ClearDaq()
{

  *m_pAcqReg = DAQSampleBank1On; // Enable bank1.
}
/*!
 
   Disarms the module for write to sample on Bank 1.  In this
case, the internal digitization and read is then controlled 
via the internal clock.  This is important in the case of
a random external clock or gated mode. 
*/
void 
CSIS3300::DisArm1()
{

  *m_pAcqReg = DAQSampleBank1Off; // Enable bank2.
}
/*!
 
   Disarms the module for write to sample on Bank 2.  The 
utility is similar to that for disarming Bank 1. 
*/
void 
CSIS3300::DisArm2()
{

  *m_pAcqReg = DAQSampleBank2Off; // Enable bank1.
}
/*!
 
   Arms the module for write to sample on Bank 1. 
*/
void 
CSIS3300::Arm1()
{

  *m_pAcqReg = DAQSampleBank1On; // Enable bank1.
}
/*!
 
   Arms the module for write to sample on Bank 2. 
*/
void 
CSIS3300::Arm2()
{

  *m_pAcqReg = DAQSampleBank2On; // Enable bank1.
}
/*!
  Utility function to read a full event.  This breaks into three
  cases:
  -# m_fPageWrap false.  In this case, the event directory
     entry determines the end of the event as well as the 
     number of samples.  Note that we assume that the wrap bit
     in the event directory allows us to differentiate between
     0 samples and 128Ksamples.
   -# m_fPageWrap true, but the event directory wrap bit is false.
      Again, the number of samples is determined by the address pointer.
   -# m_fPageWrap true and event directory wrap bit is true.  In this
      case, a full m_nPagesize samples have been taken and the
      address pointer indicates the start of event.  The data procede
      circularly in the first m_nPagesize words of the buffer memory
      since we don't support multi-event mode yet.

   \param pBuffer     -  Points to the target buffer (ordinary memory.
                         the caller must ensure this is at least a page long.
   \param pAddressReg - Pointer to the address index register.
   \param nBase       - Pointer to the start of event memory.

   \note
   -  Event memory can only be read as longwords, but the event buffer is a
      word buffer
   - The event pointer will be updated to reflect the words read in..

   Returns the number of words read. 

   */
unsigned int 
CSIS3300::ReadAGroup(void* pbuffer,
			  volatile uint32_t* pAddressReg,
			  uint32_t           nBase)
{

  uint32_t nPagesize(m_nPagesize);	// Max conversion count.

  // Decode the event directory entry:

  uint32_t    AddressRegister = *pAddressReg;
  bool           fWrapped      = (*pAddressReg & EDIRWrapFlag ) != 0;
  uint32_t  nEventEnd     = (*pAddressReg & EDIREndEventMask);
  nEventEnd                   &= (nPagesize-1);	// Wrap the pointer to pagesize
  uint32_t  nLongs(0);
  uint32_t* Samples((uint32_t*)pbuffer);

  // The three cases above break into two cases: fWrapped true or not.

  if(fWrapped) {
    //  Full set of samples... 

    nLongs = nPagesize;
    
    // Breaks down into two reads:

    // The first read is from nEventEnd -> nPagesize.

    int nReadSize = (nPagesize - nEventEnd);
    if(nReadSize > 0) {
      CVMEInterface::Read((void*)m_nFd,
			  nBase + nEventEnd*sizeof(uint32_t),
			  Samples,
			  nReadSize*sizeof(uint32_t));
    }

    // The second read, if necessary, is from 0 ->nEventEnd-1.
    
    uint32_t nOffset =  nReadSize; // Offset into Samples where data goes.
    nReadSize = nPagesize - nReadSize;  // Size of remaining read.
    if(nReadSize > 0) {
      CVMEInterface::Read((void*)m_nFd,
			  nBase,
			  &(Samples[nOffset]),
			  nReadSize*sizeof(uint32_t));
    }
    nLongs = nPagesize;
  }
  else {			
    // Only 0 - nEventEnd to read...
    if(nEventEnd > 0) {
      CVMEInterface::Read((void*)m_nFd,
			  nBase, Samples, 
			  (nEventEnd*sizeof(uint32_t)));
      nLongs = nEventEnd;
    } 
    else {			// nothing to read...
      nLongs = 0;
    }
  }

  return nLongs*sizeof(uint32_t)/sizeof(uint16_t);

}



/*!
  Reads all data groups.  The groups are read in numerical order.
  Note that within each group, the adc's are interspersed as they are
  in the event memory.
  \param  pBuffer - a DAQWordBufferPtr passed by reference which 
                    describes where the data should be stored.
		   
   \note
    pBuffer is updated to reflect the data read in.

    \return  The number of words read is returned.

  */

// With ordinary buffers:

unsigned int
CSIS3300::ReadAllGroups(void* pBuff)
{
  int nRead(0);
  int nSegSize;

  unsigned short *pBuffer;

  nSegSize = ReadGroup1(pBuffer);
  pBuffer += nSegSize;
  nRead   += nSegSize;
  nSegSize = ReadGroup2(pBuffer);
  pBuffer += nSegSize;
  nRead   += nSegSize;
  nSegSize = ReadGroup3(pBuffer);
  pBuffer += nSegSize;
  nRead   += nSegSize;
  nSegSize = ReadGroup4(pBuffer);
  pBuffer += nSegSize;
  nRead   += nSegSize;

  return nRead;
}

/*!
   Read the first group of adc's  This comprises channel 0, 1
   \param pBuffer - A DAQWordBufferPtr which points to the buffer.

   \note pBuffer is updated to reflect the data read.
   \return The number of words read in.

   */

// Now with ordinary buffers:

unsigned int
CSIS3300::ReadGroup1(void* pBuffer)
{
  uint32_t endaddress = *m_pAddressReg1;
  return ReadAGroup(pBuffer, m_pEventDirectory1, m_nBase + 0x400000);
}
/*!
   Read the second group of adc's  This comprises channel 2,3
   \param pBuffer - A DAQWordBufferPtr which points to the buffer.

   \note pBuffer is updated to reflect the data read.
   \return The number of words read in.

   */

// Now with ordinary buffers:

unsigned int
CSIS3300::ReadGroup2(void* pBuffer)
{
  uint32_t endaddress = *m_pAddressReg1;
  return ReadAGroup(pBuffer, m_pEventDirectory2, m_nBase+0x480000);

}
/*!
   Read the third  group of adc's  This comprises channel 4,5
   \param pBuffer - A DAQWordBufferPtr which points to the buffer.

   \note pBuffer is updated to reflect the data read.
   \return The number of words read in.

   */

// Now with ordinary buffers:

unsigned int 
CSIS3300::ReadGroup3(void* pBuffer)
{
  uint32_t endaddress = *m_pAddressReg1; // For debugging.
  return ReadAGroup(pBuffer, m_pEventDirectory3, m_nBase + 0x500000);
}

/*!
   Read the fourth  group of adc's  This comprises channel 6,7
   \param pBuffer - A DAQWordBufferPtr which points to the buffer.

   \note pBuffer is updated to reflect the data read.
   \return The number of words read in.

   */

// With ordinary buffers:

unsigned int
CSIS3300::ReadGroup4(void* pBuffer)
{
  uint32_t endaddress = *m_pAddressReg1; // For debugging.
  return ReadAGroup(pBuffer, m_pEventDirectory4, m_nBase + 0x580000 );
}


/*!
   Reset the module by writing a 0 to the reset key register.
   use this function instead of getting the pointer via
   getResetKeyRegister()
*/
void 
CSIS3300::Reset()
{
  *m_pResetKey = 0;
}
/*!
  Return true if the module has the HiRA firmware installed.
*/
bool
CSIS3300::haveHiRAFirmware() const
{

  // Read the module firmware..


  unsigned int major = getFirmwareMajor();
  unsigned int minor = getFirmwareMinor();

  return ((HIRAFWMAJOR == major));

}
