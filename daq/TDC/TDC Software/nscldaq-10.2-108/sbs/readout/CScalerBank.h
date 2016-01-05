#ifndef __CSCALERBANK_H
#define __CSCALERBANK_H
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

#ifndef __CCOMPOSITE_H
#include "CComposite.h"
#endif

#ifndef __CSCALER_H
#include "CScaler.h"
#endif

#ifndef __CRT_STDDEF_H
#include <stddef.h>
#ifndef __CRT_STDDEF_H
#define __CRT_STDDEF_H
#endif
#endif

#ifndef __CRT_STDINT_H
#include <stdint.h>
#ifndef __CRT_STDINT_H
#define __CRT_STDINT_H
#endif
#endif

#ifndef __STL_VECTOR
#include <vector>
#ifndef __STL_VECTOR
#define __STL_VECTOR
#endif
#endif

/*!
  CScalerBank is a container or CScaler objects,and CScalerBank objects.
  The CComposite class is inherited but  re-exposed via a type safe
  adaptor that ensures only scaler modules can be placed in the composite.
  The CScalerObject is publicly inherited so scaler banks can be treated as any
  other scale object, delegating scaler operations to its children and
  containng instances of other banks to provide a hierarchical assembly of scalers.

*/
class CScalerBank :  public CComposite, public CScaler
{
public:
  typedef CComposite::objectIterator ScalerIterator;

  class CVisitor {
  public:
    virtual void operator()(CScaler* pScaler) = 0;
  };


public:

  // Members inherited from CScaler:

  virtual void initialize();
  virtual void clear();
  virtual void disable();
  virtual std::vector<uint32_t> read();

  // Type safe adapter to CComposite:

  void AddScalerModule(CScaler* pScaler);
  void DeleteScaler(CScaler* pScaler);
  ScalerIterator begin();
  ScalerIterator end();

  virtual bool   isComposite() const;
  
  void visit(CVisitor& visitor); //!< shallow visitation.
};

#endif
