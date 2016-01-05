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

#ifndef __CNADC2530_H
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
   The Hytec NADC 2530 is an 8 channel high resolution 13 bit ADC.  The ADC
   can run in single-event, multi-event, mode which the Hytec manual refers to as 
   'list mode'.  The module can also operate in autonomous histogramming mode,
   in which the module generates histograms that can be read out....effectively making it
   a multichannel analyzer front end.

   This module supports  list mode.

   Configuration parameters are:
\verbatim

Parameter        Value Type             Value meaning
-csr             integer                Module register base
-memory          integer                Desired event memory base.
-ipl             integer (0..7)         Interrupt priority level.  0 disables interrupts.
-vector          integer (0..65535)     Interrupt vector.
-lld             float(0-819.1)         Low level discriminator in mV .1mV resolution.
-hld             float(0?..8.191)       High level discrminator in V .1V resolution.
-events          integer                Number of events between data ready interrupts.
                                        !!!!If not using interrupts set this to 1!!!!!!!
                                        so that you can identify the ADC.
// New!

-zerosuppress    bool                   If true, enable zero supression (default true).
-id              integer(0..65535)      16 bit marker word inserted in front of ADC data

*/


class CNADC2530 : public CReadoutHardware
{
private:
  CReadoutModule*   m_pConfiguration;
  uint32_t          m_csr;
  uint16_t          m_csrValue;
  uint32_t          m_eventBase;
  int               m_eventCount;

public:
  // Class canonicals.
  
  CNADC2530();
  CNADC2530(const CNADC2530& rhs);
  virtual ~CNADC2530();

  CNADC2530& operator=(const CNADC2530& rhs);
private:
  int operator==(const CNADC2530& rhs);
  int operator!=(const CNADC2530& rhs);


  // The interface of the abstract base class, which we must implement:

  // overridable : operations on constructed objectgs:

public:
  virtual void onAttach(CReadoutModule& configuration);
  virtual void Initialize(CVMUSB& controller);
  virtual void addReadoutList(CVMUSBReadoutList& list);
  virtual CReadoutHardware* clone() const;

  // utililites:

private:
  uint16_t lldToRegister(double lld);
  uint16_t hldToRegister(double hld);
  
};

#endif
