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

#ifndef __C977_H
#define __C977_H

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
   The V977 is a 16 bit input register that can be read during a physics
trigger.  Individual signals can be masked and a gate can be used to strobe signals 
in. The configuration of this module probably exposes more parameters than you'd 
ever want.  Suitable defaults should make this easy to use as a pattern register
to be read as part of an event.

Configuration parametrs:

\verbatim
Parameter               Value Type          Value meaning
-base                   integer             Module base address (rotary switches)
-inputmask              uint16_t            Value to program in the input mask register
                                            defaults to zero
-readmode               singlehit|multihit  Determines which register is read.
-outputmask             uint16_t            Value programmed in output mask.
-interruptmask          uint16_t            Value programmed in interrupt mask register.
-readandclear           bool                Determines if the read-clear registers
                                            are used rather than the read registers.
-ipl                    (0-7)               Interrupt level 0 is disabled.
-vector                 (0-255)             Interrupt vector.
-pattern                 bool               Pattern bit is set in the control register
-gate                    bool               Gate mask bit is not set in the control reg.
-ormask                  bool               Ormask bit is set in the control register
\endverbatim

Defaults:
   - -base       - 0
   - -inputmask  - 0
   - -readmode   - singlehit
   - -outputmask - 0
   - -interruptmask - 0
   - -readandclear - true
   - -ipl        - 0
   - -vector     - 0
   - -pattern    - false
   - -gatemask   - false
   - -ormask     - false.
*/

class CV977 : public CReadoutHardware
{
private:
  CReadoutModule*    m_pConfiguration;

  // Canonicals:

public:
  CV977();
  CV977(const CV977& rhs);
  CV977& operator=(const CV977& rhs);
  ~CV977();
  
  // unsupported canonicals:
private:
  int operator==(const CV977& rhs) const;
  int operator!=(const CV977& rhs) const;

  // CReadoutHardware mandatory interface:

public:
  virtual void onAttach(CReadoutModule& configuration);
  virtual void Initialize(CVMUSB& controller);
  virtual void addReadoutList(CVMUSBReadoutList& list);
  virtual CReadoutHardware* clone() const;

};

#endif
