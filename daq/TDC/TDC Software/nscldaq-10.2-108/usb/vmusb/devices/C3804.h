#ifndef __C3804_H
#define __C3804_H
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

#ifndef __CREADOUTHARDWARE_H
#include "CReadoutHardware.h"
#endif

#ifndef __STL_STRING
#include <string>
#ifndef __STL_STRING
#define __STL_STRING
#endif
#endif



#ifndef __CRT_STDINT_H
#include <stdint.h>
#ifndef __CRT_STDINT_H
#define __CRT_STDINT_H
#endif
#endif

// Forward class definitions:

class CReadoutModule;
class CVMUSB;
class CVMUSBReadoutList;

/*!
  The C3804 class represents the SIS C3804 scaler module.  This module will be readout
during scaler events.  We will run the module as a software latched scaler, by
preceding a read with a clock shadow register. operation.  The Front panel will be run in
input mode 1, that is
\verbatim:
Input1:    Disable all counters.
Input2:    Clear all channels.
Input3:    Externally clock shadow registers.
Input4:    External test pulse (presumably counts all channels?

\verbatim.

The following configuration parameters are supported:

\endverbatim

Option       Value type       Default    Meaning
-base        integer          0          Base address of the module.
-refpulser   bool             false      enable reference pulser to channel 0 if true.
-disables    integer          0          Mask of channels in which to disable counting.
-autoclear   bool             true       Read and clear rather than allowing scalers to
                                         continuously count.

\endverbatim


*/


class C3804 : public CReadoutHardware
{
private:
  CReadoutModule*   m_pConfiguration;
public:
  C3804();
  C3804(const C3804& rhs);
  virtual ~C3804();
  C3804& operator=(const C3804& rhs);

private:
  int operator==(const C3804& rhs) const;
  int operator!=(const C3804& rhs) const;


public:
  virtual void onAttach(CReadoutModule& configuration);
  virtual void Initialize(CVMUSB& controller);
  virtual void addReadoutList(CVMUSBReadoutList& list);
  virtual CReadoutHardware* clone() const;

private:
  uint32_t   getIntegerParameter(std::string name) const;
  bool       getBoolParameter(std::string name)    const;

  // 32 bit transfers that throw if there's an error:

  void       checkRead(CVMUSB& controller, uint32_t address, uint32_t& value);
  void       checkWrite(CVMUSB& controller, uint32_t address, uint32_t value);
};   

#endif
