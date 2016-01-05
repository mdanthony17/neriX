#ifndef __CSCALER_H
#define __CSCALER_H
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
#ifndef __COBJECT_H
#include "CObject.h"
#endif

#ifndef __STL_VECTOR
#include <vector>
#ifndef __STL_VECTOR
#define __STL_VECTOR
#endif
#endif

#ifndef __CRT_STDINT_H
#include <stdint.h>
#ifndef __CRT_STDINT_H
#define __CRT_STDINT_H
#endif
#endif

/*!
   Base class for classes that read a scaler module.
   While it's possible to write derived classes to read
   several modules, this is what the CScalerBank class is for,
   to serve as a container for other scaler module readout objects.

   This is an abstract base class.  All methods are
   virtual, however default actions for
   - initialize
   - clear
   - disable

   Are all no-ops. read is pure virtual.

   The signature of the read operation assumes that scaler reads are not particularly
   performance critical because they happen only every few seconds.

*/
class CScaler : public CObject
{
public:
  virtual void initialize();
  virtual void clear();
  virtual void disable();
  virtual std::vector<uint32_t> read() = 0;

  virtual bool isComposite() const;
};

#endif
