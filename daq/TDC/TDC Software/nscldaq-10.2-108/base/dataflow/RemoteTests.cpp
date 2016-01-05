// Template for a test suite.

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Asserter.h>
#include "Asserts.h"
#include <CRingMaster.h>
#include <CRingBuffer.h>
#include <CRemoteAccess.h>
#include <string.h>

using namespace std;

class RemoteTests : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(RemoteTests);
  CPPUNIT_TEST(sizeTest);
  CPPUNIT_TEST(consumerTest);
  CPPUNIT_TEST(mindataTest);
  CPPUNIT_TEST(timeoutTest);
  CPPUNIT_TEST(urllocal);
  CPPUNIT_TEST(urlremote);
  CPPUNIT_TEST_SUITE_END();


private:

public:
  void setUp() {
    CRingBuffer::create(string("urllocal"));
  }
  void tearDown() {
    CRingBuffer::remove(string("urllocal"));  
  }
protected:
  void sizeTest();
  void consumerTest();
  void mindataTest();
  void timeoutTest();
  void urllocal();
  void urlremote();
};

CPPUNIT_TEST_SUITE_REGISTRATION(RemoteTests);

// Set/get proxy ring size:

void RemoteTests::sizeTest() 
{
  size_t value = CRingAccess::getProxyRingSize();
  size_t old   = CRingAccess::setProxyRingSize(value*2);
  EQ(old, value);
  EQ(value*2, CRingAccess::getProxyRingSize());

  // Make this all idempotent by restoring the value:

  CRingAccess::setProxyRingSize(value);
}

// Set get maxconsumers.

void RemoteTests::consumerTest()
{
  size_t value = CRingAccess::getProxyMaxConsumers();
  size_t old   = CRingAccess::setProxyMaxConsumers(value+10);
  EQ(value, old);
  EQ(value+10, CRingAccess::getProxyMaxConsumers());
  CRingAccess::setProxyMaxConsumers(value);
}
// set get mindata:

void RemoteTests::mindataTest()
{
  size_t value = CRingAccess::getMinData();
  size_t old   = CRingAccess::setMinData(value/2);
  EQ(value, old);
  EQ(value/2, CRingAccess::getMinData());
  
  CRingAccess::setMinData(value);
}
// Get/set timeout:

void
RemoteTests::timeoutTest()
{
  unsigned value = CRingAccess::getTimeout();
  unsigned old   = CRingAccess::setTimeout(value*2);
  EQ(value, old);
  EQ(value*2, CRingAccess::getTimeout());
  CRingAccess::setTimeout(value);
}

// Open a local ring using the remote access software.
// - Create a local ring.
// - Attach as producer.
// - Attach via URL.
// - Produce some data,
// - Consume some data
// - check data matches.
// - destroy the ring.

void
RemoteTests::urllocal()
{


  char hostname[1000];
  gethostname(hostname, sizeof(hostname));
  string hostName(hostname);
	      


  // Encapsulate in a try block so we can show taht
  // exceptions were thrown, and also ensure we can
  // continue and delete the ring.
  //
  bool caught = false;
  CRingBuffer* pConsumer(0);
  try {
    CRingBuffer  producer("urllocal", CRingBuffer::producer);
    pConsumer = CRingAccess::daqConsumeFrom("tcp://localhost/urllocal");
   
    ASSERT(pConsumer);

    char putBuffer[100];
    char getBuffer[100];
    memset(getBuffer, 0, sizeof(getBuffer));
    for (int i =0; i < sizeof(putBuffer); i++) {
      putBuffer[i]  = i;
    }
    
    producer.put(putBuffer, sizeof(putBuffer));
    size_t n = pConsumer->get(getBuffer, sizeof(getBuffer), 1);
    EQ(sizeof(getBuffer), n);
    
    for(int i =0; i < sizeof(getBuffer); i++) {
      EQ(putBuffer[i], getBuffer[i]);
    }
    
  }
  catch(...) {
    
    caught = true;
  }
  if(pConsumer) {
    delete pConsumer;
  }

  ASSERT(!caught);

}
// same as urllocal but use for the host name hostname()
// so that we force the ringmaster to think this is a
// remote connection

void
RemoteTests::urlremote()
{


  char hostname[1000];
  gethostname(hostname, sizeof(hostname));
  string hostName(hostname);
	      


  // Encapsulate in a try block so we can show taht
  // exceptions were thrown, and also ensure we can
  // continue and delete the ring.
  //
  bool caught = false;
  CRingBuffer* pConsumer(0);
  try {
    CRingBuffer  producer("urllocal", CRingBuffer::producer);
    string url("tcp://");
    url += hostName;
    url += "/urllocal";
    pConsumer = CRingAccess::daqConsumeFrom(url);
   
    sleep(1);			//  Let everything settle in and start.
    ASSERT(pConsumer);

    char putBuffer[100];
    char getBuffer[100];
    memset(getBuffer, 0, sizeof(getBuffer));
    for (int i =0; i < sizeof(putBuffer); i++) {
      putBuffer[i]  = i;
    }
    
    producer.put(putBuffer, sizeof(putBuffer));
    size_t n = pConsumer->get(getBuffer, sizeof(getBuffer), 1);
    EQ(sizeof(getBuffer), n);
    
    for(int i =0; i < sizeof(getBuffer); i++) {
      EQ(putBuffer[i], getBuffer[i]);
    }
    
  }
  catch(...) {
    
    caught = true;
    delete pConsumer;
  }

  ASSERT(!caught);
  delete pConsumer;		// Destroy the proxy ring.
}
