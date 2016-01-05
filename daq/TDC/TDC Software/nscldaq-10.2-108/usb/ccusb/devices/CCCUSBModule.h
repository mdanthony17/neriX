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
#ifndef __CCCUSBMODULE_H
#define __CCUSBMODULE_H

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
class CCCUSB;
class CCCUSBReadoutList;

/*!
 Supplies support for the internal resources of the CC-USB module. Use this module as any other
module and stick it into a stack.. it won't generate a readout list.  The initialization, however 
is capable of setting up specific internal module devices.   These are managed via configuration options
just like any 'normal' module:
Note: We can't use CCCUSB as the name for this class since that class already exists in the 
      system and represents the CCUSB controller for primitive operations.

\verbatim

Name             Type        Default         Meaning
-gdgasource      enum        disabled        Source of gate and delay generator A start.. This can be:
                                             disabled, in1, in2, in3, event, eventend, usbtrigger, or pulser
-gdbbsource      enum        disabled        Source of gate for GDG B start, can have any of the values
                                             as -gdgasource.
-gdgawidth       integer     1               GDGA gate width in 10ns units. between (1 and 65535).
-gdgadelay       integer     0               GDGB gate delay in 10ns units between (0 and 2^24-1).
-gdgbwidth       integer     1               GDB gate width in 10ns units, between 1 and 65535
-gdgbdelay       integer     0               GDGB delay in 10ns units between 0 and 2^24-1.
-out1            enum        busy            Source of O1 signal one of:
                                             busy, event, gdga, gdgb
-out1latch       bool        false           Output 1 is latched.
-out1invert      bool        false           Output 1 is inverted.

-out2            enum        trigger         Source of O2 signal.  One of:
                                             acquire, event, gdga gdgb
-out2latch       bool        false
-out2invert      bool        false
-out3            enum        busyend         Source of O3 trigger.. one of:
                                             busyend, busy, gdga, gdgb
-out3latch       bool        false
-out3invert      bool        false

--- new ---

-redled          enum        event          Source of the red led this is one of:
                                            event, busy, usboutnotempty usbinnotfull
-redinvert       bool        false          Invert the sense of the Red LED
-redlatch        bool        false          Latch the state of the red led.

-greenled        enum        acquire        Source of the green LED this is one of:
                                            acquire, nimi1, usbinnotempty, usbtrigger
-greeninvert      bool       false          If true, the green led state is inverted.
-greenlatch       bool       false          If true the green led is latched.

-yellowled        enum        nimi2         Source of the yellow led. One of:
                                            nimi2, nimi3, busy, usbinfifonotempty
-yellowinvert     bool        false         If true inverts the sense of the yellow led.
-yellowlatch      bool        false         If true latches the yellow led when the condition hits.

-scalera          enum       disabled       Enum that defines what makes scaler A count:
                                            disabled, i1, i2, i3, event, carryb, dgga, dggb
-scalerb          enum       disabled       Enum that defines what makes scaler B count:
                                            disbabled, i1, i2, i3, event carrya, dgga, dggb

-readscalers      bool       false          If true the module scalers are read (first A then B).
-incremental      bool       true           If true, the scalers are cleared after the read.

-transfer_after_timeout bool false          If true, the module emits packet end signal after 
                                            the time specified by -transfertimeout
-transfer_after_nevents bool false          If true, the module emits packet end signal after
                                            the number of events specified by -transferafter 

-transfertimeout integer    0               Number of seconds in excess of 1 second after
                                            which a packet end signal is issued. limited to [0-15]
-transferafter   integer    0               Number of events after which the packet end signal 
                                            is issued. Must be in range [0,255].


\endverbatim

*/
class CCCUSBModule : public CReadoutHardware
{
private:
  uint32_t m_deviceSource;

  // class canonicals:
  //    Implemented:


  
public:
  CCCUSBModule();
  CCCUSBModule(const CCCUSBModule& rhs);
  virtual ~CCCUSBModule();
  CCCUSBModule& operator=(const CCCUSBModule& rhs);
  
  //    Unimplemented:
private:
  int operator==(const CCCUSBModule& rhs) const;
  int operator!=(const CCCUSBModule& rhs) const;

  // THis module implements the CReadoutHardware interface below:

public:
  virtual void onAttach(CReadoutModule& configuration);
  virtual void Initialize(CCCUSB& controller);
  virtual void addReadoutList(CCCUSBReadoutList& list);
  virtual CReadoutHardware* clone() const;


  // Module utility functions:
private:
  static int enumIndex(const char**  values, std::string parameter);
  void configureOutput(CCCUSB& controller);
  void configureGdg1(CCCUSB& controller);
  void configureGdg2(CCCUSB& controller);
  void configureDevices(CCCUSB& controller);
  void configureLED(CCCUSB& controller);
  void configureBulkTransfer(CCCUSB& controller);
};

#endif
