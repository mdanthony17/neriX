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

#ifndef __CMADC32_H
#define __CMADC32_h

#ifndef __CREADOUTHARDWARE_H
#include "CReadoutHardware.h"
#endif

#ifndef __CRT_STDINT_H
#include <stdint.h>
#ifndef __CRT_STDINT_H
#define __CRT_STDINT_H
#endif
#endif

#ifndef __STL_STRING
#include <string>
#ifndef __STL_STRING
#define __STL_STRING
#endif
#endif

#ifndef __STL_VECTOR
#include <vector>
#ifndef __STL_VECTOR
#define __STL_VECTOR
#endif
#endif


// Forward class definitions:

class CReadoutModule;
class CVMUSB;
class CVMUSBReadoutList;


/*!
   The MADC32 is a 32 channel ADC module produced by Mesytec.
   This module will be used in single event mode.
   The following configuration parameters can be sued to tailor
   the module:

\verbatim
   Name                 Value type          Description
   -base                integer             Base address of the module in VME space.
   -id                  integer [0-255]     Module id (part of the module header).
   -ipl                 integer [0-7]       Interrupt priority level 0 means disabled.
   -vector              integer [0-255]     Interrupt vector.
   -timestamp           bool  (false)       If true enables the module timestamp.
   -gatemode            enum (separate,common)  Determines if the bank gates are
                                            independent or common.
   -holddelays          int[2]              Delay between trigger and gate for each bank.
   -holdwidths          int[2]              Lengths of generated gates.
   -gategenerator       bool                Enable gate generator (hold stuff)
   -inputrange          enum (4v,8v,10v)    ADC input range.
   -ecltermination      bool                Enable termination of the ECL inputs.
   -ecltming            bool                Enables ECL timestamp inputs
                                            (oscillator and reset).
   -nimtiming           bool                Enables NIM input for timestamp inputs
                                            (oscillator & rset).
   -timingsource        enum (vme,external)  Determines where timestamp source is.
   -timingdivisor       int [0-15]          Divisor (2^n) of timestamp clock
   -thresholds          int[32] [0-4095]    Threshold settings (0 means unused).
   -multievent          bool (false)        Enable/disablen multi-event mode.
   -irqthreshold        integer 0           # Events before interrupt.
   -resolution          enum (8k)           2k 4k 4khires 8k 8khires ..
                                            possible ADC resolution values.

\endverbatim
*/
class CMADC32 : public CReadoutHardware
{
public:
  enum ChainPosition {
    first,
    middle,
    last
  };
private:
  CReadoutModule*     m_pConfiguration;
public:
  CMADC32();
  CMADC32(const CMADC32& rhs);
  virtual ~CMADC32();
  CMADC32& operator=(const CMADC32& rhs);
private:
  int operator==(CMADC32& rhs) const;
  int operator!=(CMADC32& rhs) const;

  // The interface for CReadoutHardware:

public:
  virtual void onAttach(CReadoutModule& configuration);
  virtual void Initialize(CVMUSB& controller);
  virtual void addReadoutList(CVMUSBReadoutList& list);
  virtual CReadoutHardware* clone() const;

  // The following functions are used by the madcchain module.
  //
  void setChainAddresses(CVMUSB& controller,
			 ChainPosition position,
			 uint32_t      cbltBase,
			 uint32_t      mcastBase);

  void initCBLTReadout(CVMUSB& controller, uint32_t cbltAddress, int wordsPermodule);
  // Utilities:

  int resolutionValue(std::string selector); // Resolution string to register value.
};



#endif
