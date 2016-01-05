   

//! \class: CScalerBank           
//! \file:  .h
// Author:
//   Ron Fox
//   NSCL
//   Michigan State University
//   East Lansing, MI 48824-1321
//   mailto:fox@nscl.msu.edu
//
// Copyright 

#ifndef __CSCALERBANK_H  //Required for current class
#define __CSCALERBANK_H

//
// Include files:
//

#ifndef __DAQTYPES_H
#include <daqdatatypes.h>
#endif

                               //Required for base classes
#ifndef __CREADORDER_H     //*CReadOrder
#include "CReadOrder.h"
#endif


// Forward class references:

class CTCLInterpreter;
class CDigitizerDictionary;

 
/*!
Encapsulates a scaler bank.  Scaler banks
are objects that readout an ordered list of scalers.
They are like ReadOrder objects, but they supply
an {\em additional} Read member that can read data
into an ordinary UShort_t* buffer rathern than into
one of the specialized NSCL data buffer types.
*/
class CScalerBank  : public CReadOrder        
{


public:
//  Constructors and other cannonical functions:

  CScalerBank (CTCLInterpreter* pInterp,
	       CDigitizerDictionary* pDictionary);
  virtual ~CScalerBank ( );
private:
  CScalerBank (const CScalerBank& aCScalerBank );
  CScalerBank& operator= (const CScalerBank& aCScalerBank);
  int operator== (const CScalerBank& aCScalerBank) const;
  int operator!= (const CScalerBank& aCScalerBank) const;
public:

   // Class functions:
public:

    ULong_t* Read (ULong_t* pBuffer)   ; // 

};

#endif
