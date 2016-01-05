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

 
// Class: CAccessListCheck                     //ANSI C++
//
// Provides access control list authentication.
// Maintains an stl map of text strings.  The interactor
// supplies a string which is hunted for in the
// map. Matches authenticate.  No-match fails.
//
// Author:
//     Ron Fox
//     NSCL
//     Michigan State University
//     East Lansing, MI 48824-1321
//     mailto: fox@nscl.msu.edu
// 
// (c) Copyright NSCL 1999, All rights reserved AccessListCheck.h
//

#ifndef __ACCESSLISTCHECK_H  //Required for current class
#define __ACCESSLISTCHECK_H

                               //Required for base classes
#ifndef __AUTHENTICATOR_H
#include "Authenticator.h"
#endif
              
#ifndef __STL_STRING
#include <string>
#define __STL_STRING
#endif                 
                               //Required for 1:1 association classes
#ifndef __STL_SET
#include <set>
#define __STL_STE
#endif

                        
class CAccessListCheck  : public CAuthenticator        
{                       
public:
  typedef std::set<std::string> StringMap;			
private:
  StringMap  m_AccessList; //1:1 association object data member      
			
public:

   // Constructors and other cannonical operations:

  CAccessListCheck ()  :
    CAuthenticator()
  { 
  } 
  ~ CAccessListCheck ( )  // Destructor 
  { }  
  CAccessListCheck (const StringMap& rSourceMap   ) 
      : CAuthenticator(),
	m_AccessList(rSourceMap)
	
  { 
  }      
  
   //Copy constructor 

  CAccessListCheck (const CAccessListCheck& aCAccessListCheck )   : 
    CAuthenticator (aCAccessListCheck) 
  {    
    m_AccessList = aCAccessListCheck.m_AccessList;
  }                                     

   // Operator= Assignment Operator 

  CAccessListCheck& operator= (const CAccessListCheck& aCAccessListCheck) {
    if(this != &aCAccessListCheck) {
      CAuthenticator::operator=(aCAccessListCheck);
      m_AccessList = aCAccessListCheck.m_AccessList;
    }
    return *this;
  }
 
   //Operator== Equality Operator  - disallowed in base class.
private:
  int operator== (const CAccessListCheck& aCAccessListCheck) const;
public:

// Selectors:

public:
                       
  StringMap getAccessList() const
  { 
    return m_AccessList;
  }
                       
// Mutators:

protected:
       
  void setAccessList(const StringMap& am_AccessList)
  { m_AccessList = am_AccessList;
  }

public:

 virtual   Bool_t Authenticate (CInteractor& rInteractor)    ;
 virtual   void AddAclEntry (const std::string& rEntry)    ;
 virtual   void DeleteAclEntry (const std::string& rEntry)    ;


};

#endif
