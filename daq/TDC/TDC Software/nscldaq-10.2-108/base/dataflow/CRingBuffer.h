#ifndef __CRINGBUFFER_H
#define __CRINGBUFFER_H

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

#ifndef __CRT_UNISTD_H
#include <unistd.h>
#ifndef __CRT_UNISTD_H
#define __CRT_UNISTD_H
#endif
#endif


#ifndef __STL_STRING
#include <string>
#ifndef __STL_STRING
#define __STL_STRING
#endif
#endif

#ifndef __STL_VECTOR
#include <vector>
#ifndef __STL_VECTOR
#define __STL_VECTOR
#endif
#endif


#ifndef __CRT_LIMITS_H
#include <limits.h>
#ifndef __CRT_LIMITS_H
#define __CRT_LIMITS_H
#endif
#endif

// Forward class/struct definitions.

typedef struct __RingBuffer        RingBuffer;
typedef struct __ClientInformation ClientInformation;
class CRingMaster;

/*!
   The ring buffer class manages a single producer multi-consumer ring  buffer.
   This class provides object oriented access to the ring buffer.
*/
class CRingBuffer
{
  // Public data/type definitions:

public:
  typedef enum __ClientMode {
    producer,
    consumer,
    manager
  } ClientMode;

  struct Usage {
    size_t                                 s_bufferSpace;
    size_t                                 s_putSpace;
    size_t                                 s_maxConsumers;
    pid_t                                  s_producer;
    size_t                                 s_maxGetSpace;
    size_t                                 s_minGetSpace;
    std::vector<std::pair<pid_t, size_t> > s_consumers;
  };

  class CRingBufferPredicate {
  public:
    virtual bool operator()(CRingBuffer& ring) = 0;
  };
  // Class data
private:
  static size_t m_defaultDataSize;     // Default ring buffer data segment size. 
  static size_t m_defaultMaxConsumers; // Default for maximun consumers allowed.
  static CRingMaster* m_pMaster;	       // Connection to the ring master daemon.
  static pid_t        m_myPid;	       // Pid so forks will make new ringmaster conns.
  // Member data
private:
  RingBuffer*         m_pRing;	       // Pointer to the actual ring.
  ClientInformation*  m_pClientInfo;   // Pointer to the object owner's client info.
  ClientMode          m_mode;	       // What sort of client this is.
  unsigned long       m_pollInterval;  // ms between blocking polls.
  std::string         m_ringName;      // Name of ring we're connected to.

  // Static member functions,
public:
  static void create(std::string name, 
		     size_t dataBytes = m_defaultDataSize,
		     size_t maxConsumer = m_defaultMaxConsumers,
		     bool   tempMasterConnection = false);
  static CRingBuffer* createAndProduce(std::string name,
				       size_t dataBytes = m_defaultDataSize,
				       size_t maxConsumer = m_defaultMaxConsumers,
				       bool   tempMasterConnection = false);
  static void remove(std::string name);
  static void format(std::string name,
		     size_t maxConsumer = m_defaultMaxConsumers);
  static bool isRing(std::string name);
  static void   setDefaultRingSize(size_t byteCount);
  static size_t getDefaultRingSize();
  static void   setDefaultMaxConsumers(size_t numConsumers);
  static size_t getDefaultMaxConsumers();
  static std::string defaultRing();
  static std::string defaultRingUrl();

  // Constructors and other canonicals.

  CRingBuffer(std::string name, ClientMode mode = consumer);
  virtual ~CRingBuffer();

  // Illegal canonicals (no implementation).
private:
  CRingBuffer(const CRingBuffer& rhs);
  CRingBuffer& operator=(const CRingBuffer& rhs);
  int operator==(const CRingBuffer& rhs) const;
  int operator!=(const CRingBuffer& rhs) const;

  // Manipulations on the ring buffer:
public:
  size_t put(void* pBuffer, size_t nBytes, unsigned long timeout=ULONG_MAX);
  size_t get(void* pBuffer, size_t maxBytes, size_t minBytes = 1, 
	     unsigned long timeout=ULONG_MAX);
  size_t peek(void* pBuffer, size_t maxbytes);
  void   skip(size_t nBytes);

  unsigned long setPollInterval(unsigned long newValue);
  unsigned long getPollInterval();

  // Inquiry functions.

  size_t availablePutSpace();
  size_t availableData();

  Usage getUsage();

  off_t getSlot();

  // blocking.

  int blockWhile(CRingBufferPredicate& pred, unsigned long timeout=ULONG_MAX);
  void pollblock();		// Block for the current poll interval.

  // Iteration (e.g. searching).

  void While(CRingBufferPredicate& pred);

  // management functions:

  void forceProducerRelease();
  void forceConsumerRelease(unsigned slot);

  // Utility funcionts:

private:
  size_t      availableData(ClientInformation* pInfo);
  void        unMapRing();
  void        allocateConsumer();
  size_t      difference(ClientInformation& producer, ClientInformation& consumer);
  void        Skip(size_t nBytes);

  static std::string shmName(std::string rawName);
  static RingBuffer* mapRingBuffer(std::string fullName);
  static bool        ringHeader(RingBuffer* p);

  std::string        modeString() const;

  static void connectToRingMaster();
  void        notifyConnection();
  void        notifyDisconnection();

};

#endif
