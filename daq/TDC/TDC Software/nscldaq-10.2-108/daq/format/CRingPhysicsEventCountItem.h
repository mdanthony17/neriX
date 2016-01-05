#ifndef __CRINGPHYSICSEVENTCOUNTERITEM_H
#define __CRINGPHYSICSEVENTCOUNTERITEM_H


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

#ifndef __CRINGITEM_H
#include "CRingItem.h"
#endif

#ifndef __DATAFORMAT_H
#include "DataFormat.h"
#endif

#ifndef __CRT_TIME_H
#include <time.h>
#ifndef __CRT_TIME_H
#define __CRT_TIME_H
#endif
#endif


#ifndef __CPPRTL_TYPEINFO
#include <typeinfo>
#ifndef __CPPRTL_TYPEINFO
#define __CPPRTL_TYPEINFO
#endif
#endif

#ifndef __STL_STRING
#include <string>
#ifndef __STL_STRING
#define __STL_STRING
#endif
#endif


/*!
   The physics event count item provides periodic informatino about how
   many events have been produced by the DAQ system.  This is provided
   so that applications that sample event data can determine what fraction
   of the data they've managed to keep up with.
   Applications can count the physics events they've seen,
   They can then divide this number by the number of events that
   have actually been generated to get an analysis fraction.

*/
class CRingPhysicsEventCountItem : public CRingItem
{
private:
  pPhysicsEventCountItem    m_pItem;

  // constructors and other canonicals:
public:
  CRingPhysicsEventCountItem();
  CRingPhysicsEventCountItem(uint64_t count,
			     uint32_t timeOffset);
  CRingPhysicsEventCountItem(uint64_t count, 
			     uint32_t timeoffset, 
			     time_t stamp);
  CRingPhysicsEventCountItem(const CRingItem& rhs)  throw(std::bad_cast);
  CRingPhysicsEventCountItem(const CRingPhysicsEventCountItem& rhs);

  virtual ~CRingPhysicsEventCountItem();

  CRingPhysicsEventCountItem& operator=(const CRingPhysicsEventCountItem& rhs);
  int operator==(const CRingPhysicsEventCountItem& rhs) const;
  int operator!=(const CRingPhysicsEventCountItem& rhs) const;


  // public interface:
public:
  uint32_t getTimeOffset() const;
  void     setTimeOffset(uint32_t offset);

  time_t   getTimestamp() const;
  void     setTimestamp(time_t stamp);

  uint64_t getEventCount() const;
  void     setEventCount(uint64_t count);

  // Virtual method overrides:


  virtual std::string typeName() const;	// Textual type of item.
  virtual std::string toString() const; // Provide string dump of the item.


private:
  void init();
};

#endif
