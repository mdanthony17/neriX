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


// Author:
//   Ron Fox
//   NSCL
//   Michigan State University
//   East Lansing, MI 48824-1321
//   mailto:fox@nscl.msu.edu
//
// Copyright 

#ifndef __CCAENV775CREATOR_H  //Required for current class
#define __CCAENV775CREATOR_H

//
// Include files:
//

//Required for base classes

#ifndef __CMODULECREATOR_H     //CModuleCreator
#include "CModuleCreator.h"
#endif

#ifndef __STL_STRING
#include <string>
#ifndef __STL_STRING
#define __STL_STRING
#endif
#endif



/*!
  Creational class for CAEN V775 module adc.  This class
  gets registered with the Module command object to 
  allow it to respond to commands to generate readout objects
  for the CAEN V775 module.  Our module type string is 
  caenv775. 
*/
class CCAENV775Creator  : public CModuleCreator        
{
public:
  //   Constructors and other cannonical operations.
  
  CCAENV775Creator ();
  ~CCAENV775Creator ( );
  CCAENV775Creator (const CCAENV775Creator& aCCAENV775Creator );
  CCAENV775Creator& operator= (const CCAENV775Creator& aCCAENV775Creator);
  int operator== (const CCAENV775Creator& aCCAENV775Creator) const;
  
  
  // Class operations:
  
public:
  
  virtual   CReadableObject* Create (CTCLInterpreter& rInterp, 
				      CTCLResult& rResult, 
				      int nArgs, char** pArgs)   ; 
  virtual   std::string  Help (); 
  
};

#endif
