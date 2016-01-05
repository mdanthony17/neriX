// Template for a test suite.

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Asserter.h>
#include "Asserts.h"

#include <config.h>

#include "CDesiredTypesPredicate.h"
#include <CRingBuffer.h>
#include <DataFormat.h>


class desiredtests : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(desiredtests);
  CPPUNIT_TEST(selecttest);
  //  CPPUNIT_TEST(sampletest); /* Changed how sampling worked which invalidates */
  CPPUNIT_TEST_SUITE_END();


private:


public:
  void setUp() {
    CRingBuffer::create("pred");
  }
  void tearDown() {
    CRingBuffer::remove("pred");
  }
protected:
  void selecttest();
  void sampletest();
};

CPPUNIT_TEST_SUITE_REGISTRATION(desiredtests);

// Check that we will select the right types of things.

void desiredtests::selecttest() {
  CRingBuffer prod("pred", CRingBuffer::producer);
  CRingBuffer cons("pred", CRingBuffer::consumer);
  
  CDesiredTypesPredicate p;
  p.addDesiredType(123);
  
  RingItemHeader h = {sizeof(RingItemHeader), 0};
  prod.put(&h, sizeof(h));	// should not make the cut.
  ASSERT(p(cons));
  EQ((size_t)0, cons.availableData());
  
  h.s_type = 123;
  prod.put(&h, sizeof(h));
  ASSERT(!p(cons));
  EQ(sizeof(h), cons.availableData());
  
}

// Check that sampling behaves correctly.

void desiredtests::sampletest() {
  CRingBuffer prod("pred", CRingBuffer::producer);
  CRingBuffer cons("pred", CRingBuffer::consumer);

  
  CDesiredTypesPredicate p;
  p.addDesiredType(123, true);
  
  RingItemHeader h = {sizeof(RingItemHeader), 123};
  prod.put(&h, sizeof(h));	// should not make the cut.
  prod.put(&h, sizeof(h));

  ASSERT(p(cons));
  EQ(sizeof(h), cons.availableData());

  ASSERT(!p(cons));
  EQ(sizeof(h), cons.availableData());
   

}
