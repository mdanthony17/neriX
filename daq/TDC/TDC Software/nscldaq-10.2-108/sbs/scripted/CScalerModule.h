/*
    This software is Copyright by the Board of Trustees of Michigan
    State University (c) Copyright 2009.

    You may use this software under the terms of the GNU public license
    (GPL).  The terms of this license are described at:

     http://www.gnu.org/licenses/gpl.txt

     Author:
             Ron Fox
	     NSCL
	     Michigan State University
	     East Lansing, MI 48824-1321
*/



/*!
Abstract base class for a scaler module.  The only
difference between a scaler module and a digitizer
is an overload for the Read member function that
allows the module to read into a buffer that is
pointed to by a long*
*/
#ifndef __CSCALERMODULE_H  //Required for current class
#define __CSCALERMODULE_H

//
// Include files:
//

#ifndef __DAQTYPES_H
#include <daqdatatypes.h>
#endif

                               //Required for base classes
#ifndef __CDIGITIZERMODULE_H   // CReadableObject
#include "CReadableObject.h"
#endif
 
#ifndef __STL_STRING
#include <string>
#ifndef __STL_STRING
#define __STL_STRING
#endif
#endif

// Forward class definitions:

class CTCLInterpreter;
 

class CScalerModule  : public CReadableObject        
{

public:
	// Constructors and other cannonical member functions:

  CScalerModule (const std::string& rName,
		 CTCLInterpreter& rInterp);
 ~ CScalerModule ( );  
  
  // Copy like operations are illegal and therefore comparison makes no
  // sense either:
  
private: 
   CScalerModule (const CScalerModule& aCScalerModule );
   CScalerModule& operator= (const CScalerModule& aCScalerModule);
   int operator== (const CScalerModule& aCScalerModule) const;
   int operator!= (const CScalerModule& aCScalerModule) const;
public:

public:

  // Class operations:

public:

  virtual   ULong_t* Read (ULong_t* pBuffer)   = 0 ; // 

};

#endif
