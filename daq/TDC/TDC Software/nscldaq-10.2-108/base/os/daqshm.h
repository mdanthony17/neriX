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
#ifndef __DAQSHM_H
#define __DAQSHM_H

#ifndef __STL_STRING
#include <string>
#ifndef __STL_STRING
#define __STL_STRING
#endif
#endif

#ifndef __STL_MAP
#include <map>
#ifndef __STL_MAP
#define __STL_MAP
#endif
#endif


#ifndef __CRT_STDDEF_H
#include <stddef.h>		// for size_t.
#ifndef __CRT_STDDEF_H
#define __CRT_STDDEF_H
#endif
#endif

#ifndef __CRT_UNISTD_H
#include <unistd.h>
#ifndef __CRT_UNISTD_H
#endif
#endif


/**
 * CDAQShm provides an operating system independent interface for'
 * creating and accessing shared memory segments.  The specific
 * implementations may be operating system dependent, however the class
 * provides a facade that is os independent.
 * 
 * A minimalist approach was taken, feel free to expand the interface definition
 * and implementations as special interests arise.
 */
class CDAQShm
{        
public:
  static bool        create(std::string name, size_t size, unsigned int flags);
  static void*       attach(std::string name);
  static bool        detach(void* pSharedMemory, std::string name, size_t size);
  static bool        remove(std::string name);
  static ssize_t     size(std::string name);
  static int         lastError();
  static std::string errorMessage(int errorCode);

private:
  static ssize_t fdSize(int fd);
public:
  static const int Success;
  static const int Exists;
  static const int NonExistent;
  static const int NoAccess;
  static const int NotAttached;
  static const int CheckOSError;
  
  static const int GroupRead;
  static const int GroupWrite;
  static const int OtherRead;
  static const int OtherWrite;



  typedef struct _attachInformation {
    void*        pMappingAddress;
    size_t       mapSize;
    unsigned int refcount;
  } attachInformation, *pAttachInformation;
  typedef std::map<std::string,attachInformation> Attachments;


private:

  static const char** m_ppMessages;
  static const int    m_nMessages;
  static       int    m_nLastError;
  static       Attachments  m_attachMap;
};

#endif
