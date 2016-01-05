#ifndef __CCONDITION_H
#define __CCONDITION_H
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

#ifndef __CRT_PTHREAD_H
#include <pthread.h>
#ifndef __CRT_PTHREAD_H
#define __CRT_PTHREAD_H
#endif
#endif

#ifndef __CRT_TIME
#include <time.h>
#ifndef __CRT_TIME
#define __CRT_TIME
#endif
#endif


class CMutex;

/*!
  Direct encapsulation of a pthread condition variable attributes block.  We are only going
  to define interfaces to the base level pthread attributes.
*/
struct CConditionAttributes
{
  pthread_condattr_t     m_attributes;

  CConditionAttributes();
  ~CConditionAttributes();

  void setShared();
  void setUnshared();
  bool isShared();

  static void throwifbad(int status, const char* msg);
};

/*!
  Direct encapsulation of a pthread condition variable.
  
*/
class CConditionVariable
{
public:
  pthread_cond_t    m_condition;

public:
  CConditionVariable();
  CConditionVariable(pthread_condattr_t& attr);
  CConditionVariable(CConditionAttributes& attr);
  virtual ~CConditionVariable();
private:
  CConditionVariable(const CConditionVariable& rhs);
  CConditionVariable& operator=(const CConditionVariable& rhs);
  int operator==(const CConditionVariable& rhs) const;
  int operator!=(const CConditionVariable& rhs) const;

  // Synchronization operations:

public:
  void signal();
  void broadcast();

  bool timedwait(CMutex& mutex,
		 const struct timespec* abstime);
  bool timedwait(pthread_mutex_t* mutex,
		 const struct timespec* abstime);

  bool timedwait(CMutex& mutex,
		 int     milliseconds);
  bool timedwait(pthread_mutex_t* mutex,
		  int    milliseconds);

  void wait(CMutex& mutex);
  void wait(pthread_mutex_t* mutex);


private:
  void create(pthread_condattr_t* pAttributes);
  struct timespec timeout(int milliseconds);
  

};
#endif
