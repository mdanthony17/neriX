

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

/*!
  \class CVmeModule
  \file CVmeModule.h

  Encapsulates a Vme module object. CVmeModules have access to
  memory which is mapped via the mmap(3) system service using 
  the CVME and CVMEptr objects. CVmeModules can write to and read 
  from these registers using poke and peek operations, respectively.

  \note
        While this module is not final, the data transfer methods are
       not virtual

  Author:
     Jason Venema
     NSCL
     Michigan State University
     East Lansing, MI 48824-1321
     mailto: venemaja@msu.edu
*/

#ifndef __VMEMODULE_H
#define __VMEMODULE_H


#ifndef __CVME_H
#include <CVME.h>
#endif

#ifndef __CRTL_STDINT_H
#include <stdint.h>
#ifndef __CRTL_STDINT_H
#define __CRTL_STDINT_H
#endif
#endif


class CVmeModule
{
 public:
  enum Space {
    a16d16 = 0,
    a24d16 = 1,
    a24d32 = 2,
    a32d32 = 3,
    geo    = 4
  };

private:
#if defined(HAVE_WIENERVME_INTERFACE) || defined(HAVE_WIENERUSBVME_INTERFACE)
   Space  m_nSpace;
   uint32_t m_nBase;
   uint32_t m_nLength;
   Int_t  m_nCrate;
   void*  m_pDriver;
#endif
#ifdef HAVE_VME_MAPPING
  CVME<UShort_t> m_CVME;  // the CVME map to the module
#endif

 public:
  // Default constructors
  CVmeModule(Space space, uint32_t base, uint32_t length, int nCrate = 0);
  // CVmeModule(CVME<uint16_t>& am_CVME);
  
  // Copy Constructor
  CVmeModule(const CVmeModule& aCVmeModule);

  // Destructor
  virtual ~CVmeModule() {}

  // Operator= assignment operator
  CVmeModule& operator= (const CVmeModule& aCVmeModule);

  // Operator== equality operator
  int operator== (const CVmeModule& aCVmeModule);

  // Mutator function

#if defined(HAVE_WIENERVME_INTERFACE) || defined(HAVE_WIENERUSBVME_INTERFACE)

 protected:
  void setDriver(void* pDriver) {
    m_pDriver = pDriver;
  }
 public:
  void* getDriver() {
    return m_pDriver;
  }
#endif

#ifdef HAVE_VME_MAPPING
 protected:
  void setCVME(CVME<uint16_t> aCVME)
    {
      m_CVME = aCVME;
    }


  CVME<uint16_t>& getCVME()
    {
      return m_CVME;
    }

#endif

  // Public member functions
public:
  uint8_t peekb(uint32_t offset=0);
  uint16_t peekw(uint32_t offset=0);
  ULong_t peekl(uint32_t offset=0);
  void pokeb(uint8_t byte, uint32_t nOffset);
  void pokew(uint16_t word, uint32_t nOffset);
  void pokel(ULong_t lword, uint32_t nOffset);
  
  uint32_t readl(void* pBuffer, uint32_t nOffset, size_t longs);
  uint32_t readw(void* pBuffer, uint32_t nOffset, size_t words);
  uint32_t readb(void* pBuffer, uint32_t nOffset, size_t bytes);
  
 
 // Utility:
protected:
   void CopyToMe(const CVmeModule& rModule);
};

#endif









