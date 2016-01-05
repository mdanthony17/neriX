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
	Creational class for CAEN V785 module adc.  This class
	gets registered with the Module command object to 
	allow it to respond to commands to generate readout objects
	for the CAEN V785 module.  Our module type string is 
	caenv785.
*/
// Author:
//   Ron Fox
//   NSCL
//   Michigan State University
//   East Lansing, MI 48824-1321
//   mailto:fox@nscl.msu.edu
//
// Copyright 

#ifndef __CCAENV785CREATOR_H  //Required for current class
#define __CCAENV785CREATOR_H

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


 

class CCAENV785Creator  : public CModuleCreator        
{
public:
	//   Constructors and other cannonical operations.
	
  CCAENV785Creator (const char* recognize="caenv785");
  ~CCAENV785Creator ( );
  CCAENV785Creator (const CCAENV785Creator& aCCAENV785Creator );
  CCAENV785Creator& operator= (const CCAENV785Creator& aCCAENV785Creator);
  int operator== (const CCAENV785Creator& rhs) const;
  int operator!= (const CCAENV785Creator& rhs) const;

  // Class operations:

public:

   virtual   CReadableObject* Create (CTCLInterpreter& rInterp, 
						   CTCLResult& rResult, 
						   int nArgs, char** pArgs)   ; 
   virtual   std::string  Help (); 

};

#endif
