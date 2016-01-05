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

#ifndef __CCAENCHAIN_H
#define __CCAENCHAIN_H

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

#ifndef __STL_LIST
#include <list>
#ifndef __STL_LIST
#define __STL_LIST
#endif
#endif


class CReadouModule;
class CVMUSB;
class CVMUSBReadoutList;
class C785;

/*!
  This class implements a CAEN chain.  A CAEN chain represents a 
  CBLT Readout chain of CAEN 32 channel digitizers (785, 775, 792, 862).
  This module has an ordered list of C785 modules added to it by name
  (see -modules below).  The list generated is a block transfer to the
  BCLT/MCST readout address defined by the -base that is large enough
  for the largest potential transfer the chain could generate.

  Configuration parameters are:

  \verbatim
  Parameter             value type           Value Meaning
  -base                 integer              MCST/CBLT address of the chain.
  -modules              list of strings      names of the C785 modules to be
                                             included in the chain.
\endverbatim

\note  -modules must all be existing C785 modules.
\note  The first module in -modules should be the left most in the chain
       the last should be the rightmost in the chain.  It is strongly recommended
       that the chain modules be simply listed from left to right in the VME
       crate
\note  The CAEN CBLT transfer system requires that all modules in a chain
       be in contiguous slots in the VME crate.
*/
class CCAENChain : public CReadoutHardware
{
private:
  typedef std::list<C785*>     ChainList;
  typedef ChainList::iterator  ChainListIterator;

private:
  ChainList         m_Chain;
  CReadoutModule*   m_pConfiguration;

  int               m_moduleCount; // Saved at init time.
  uint32_t          m_baseAddress; // Saved at init time.

  // class canonicals:

public:
  CCAENChain();
  CCAENChain(const CCAENChain& rhs);
  virtual ~CCAENChain();
  CCAENChain& operator=(const CCAENChain& rhs);

  // Disallowed canonicals:

private:
  int operator==(const CCAENChain& rhs) const;
  int operator!=(const CCAENChain& rhs) const;
public:


  // overridable oeprations on constructed objects:

public:
  virtual void onAttach(CReadoutModule& configuration);
  virtual void Initialize(CVMUSB& controller);
  virtual void addReadoutList(CVMUSBReadoutList& list);
  virtual CReadoutHardware* clone() const;

  // utilities:

  uint32_t               getCBLTAddress();
  std::list<std::string> getModules();
  
  static bool moduleChecker(std::string name, std::string value, void* arg);
  
};
#endif
