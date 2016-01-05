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


#ifndef __CBUFFERQUEUE_H
#define __CBUFFERQUEUE_H
using namespace std;

#ifndef __CGAURDEDOBJECT_H
#include <CGaurdedObject.h>
#endif

#ifndef __CRT_UNISTD_H
#include <unistd.h>
#ifndef __CRTUNISTD_H
#define __CRTUNISTD_H
#endif
#endif

#ifndef __STL_LIST
#include <list>
#ifndef __STL_LIST
#define __STL_LIST
#endif
#endif

#ifndef __PTHREAD_H
#include <pthread.h>
#ifndef __PTHREAD_H
#define __PTHREAD_H
#endif
#endif

/*!
   Implements a thread safe queue of arbitrary objects.
   in our application there are two uses:
   - To pass buffers of data around.
   - To pass commands and replies between threads.

    The data requirements of these two applications are vastly
    different so this is implemented as a template class.
*/

template<class T>
class CBufferQueue : public CGaurdedObject
{
private:
  size_t          m_nWakeLevel;         // Wakeup high water mark.
  pthread_cond_t  m_condition;          // To block on empty queue. 
  std::list<T>    m_queue;		// The queue queue -> push_back, dequeue->pop_front
public:
  CBufferQueue(size_t wakeLevel = 0);
  virtual ~CBufferQueue();

  // Buffer queues are not copyable.
private:
  CBufferQueue(const CBufferQueue<T>& rhs);
  CBufferQueue<T>& operator=(const CBufferQueue<T> rhs);
  int operator==(const CBufferQueue<T> rhs) const;
  int operator!=(const CBufferQueue<T> rhs) const;

public:
  void queue(T object);		//!< Add object to queue.
  T    get();			//!< dequeue object, blocking if needed.
  std::list<T> getAll();		//!< Emtpy the queue..
  void setWakeThreshold(size_t level);
  void wait();	     		//!< Wait for buffers.
  void wake();			//!< Wake buffer waiters.

};


// Templates are implemented in two ways... as inline code or
// using dymnamically generated repositories of  code.  The following
// ensures that inline will work.  I wish I knew how to distinguish between
// compilers that used and did not use repositories, but I don't so..
// this may require manual editing for repository compilers.
//
#ifndef  COMPILINGCBUFFERQUEUE
#include <CBufferQueue.cpp> 
#endif

#endif
