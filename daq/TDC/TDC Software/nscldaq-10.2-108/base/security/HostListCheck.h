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

// Class: CHostListCheck                     //ANSI C++
//
// Encapsulates host list authentication.
// {Add,Delete}AclEntry allow named hosts to be added.
//  gethostbyname is used to resolve these to IP adresses which,
//  represented in a.b.c.d form are entered in the map.
//  {Add,Delete}IpAddress are used to add an IP address.
//  The address is represented textually in a.b.c.d form and
//   entered in the map.
//
// Author:
//     Ron Fox
//     NSCL
//     Michigan State University
//     East Lansing, MI 48824-1321
//     mailto: fox@nscl.msu.edu
// 
// (c) Copyright NSCL 1999, All rights reserved HostListCheck.h
//

#ifndef __HOSTLISTCHECK_H  //Required for current class
#define __HOSTLISTCHECK_H

                               //Required for base classes
#ifndef __ACCESSLISTCHECK_H
#include "AccessListCheck.h"
#endif

#ifndef __STRINGINTERACTOR_H
#include "StringInteractor.h"
#endif


#ifndef __CRTL_IN_H
#ifdef WIN32
#include <winsock.h>
#else
#include <netinet/in.h>
#endif
#define __CRTL_IN_H
#endif

#ifndef __STL_STRING
#include <string>
#define __STL_STRING
#endif

#ifndef __CPP_IOSTREAM
#include <iostream>
#ifndef __CPP_IOSTREAM
#define __CPP_IOSTREAM
#endif
#endif
                               
class CHostListCheck  : public CAccessListCheck        
{                       
			
protected:

public:

   // Constructors and other cannonical operations:

  CHostListCheck ()  
  { 
  } 
   ~ CHostListCheck ( )  // Destructor 
  { }  
  
   //Copy constructor 

  CHostListCheck (const CHostListCheck& aCHostListCheck )   : 
    CAccessListCheck (aCHostListCheck) 
  { 
  }                                     

   // Operator= Assignment Operator 

  CHostListCheck& operator= (const CHostListCheck& aCHostListCheck) {
    if(this != &aCHostListCheck) {
      CAccessListCheck::operator=(aCHostListCheck);
    }
    return *this;
  }
 
   //Operator== Equality Operator 
private:
  int operator== (const CHostListCheck& aCHostListCheck) const;
public:
       
public:
  virtual Bool_t Authenticate(CInteractor& rInteractor) {
    return Authenticate(GetLine(rInteractor));
   } 
  Bool_t Authenticate(const std::string& rHostname);

  virtual   void AddAclEntry (const std::string& rHostname) {
    std::cerr << ">Warning, CHostListCheck::AddAclEntry called "
         << " Should call AddIpAddress\n";
    CAccessListCheck::AddAclEntry(rHostname);
  }
  virtual   void DeleteAclEntry (const std::string& rHostname) {
    std::cerr << ">Warning, CHostListCheck::DeleteAclEntry called "
         << " Should call DeleteIpAddress\n";
    CAccessListCheck::DeleteAclEntry(rHostname);
  }
  Bool_t Authenticate(in_addr Address) {
    CStringInteractor Host(EncodeAddress(Address));
    return CAccessListCheck::Authenticate(Host);
  }
  void AddIpAddress (in_addr Address)    ;
  void DeleteIpAddress (in_addr address)    ;
  
protected:
	std::string EncodeAddress(in_addr Address);

};

#endif
