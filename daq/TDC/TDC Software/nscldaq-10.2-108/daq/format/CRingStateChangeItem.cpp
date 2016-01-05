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


#include <config.h>
#include "CRingStateChangeItem.h"
#include <RangeError.h>
#include <sstream>
#include <string.h>


using namespace std;

////////////////////////////////////////////////////////////////////////////
//
// Constructors and canonicals.
//

/*!
   Construct a bare bone state change buffer.  The run number, time offset
   are both set to zero.  The timestamp is set to 'now'.
   The title is set to an emtpy string.
   \param reason - Reason for the state change.  This defaults to BEGIN_RUN.
*/
CRingStateChangeItem::CRingStateChangeItem(uint16_t reason) :
  CRingItem(reason, sizeof(StateChangeItem)) ,
  m_pItem(0)
{
  init();

  // Fill in the body:

  m_pItem->s_runNumber    = 0;
  m_pItem->s_timeOffset   = 0;
  m_pItem->s_Timestamp = static_cast<uint32_t>(time(NULL));
  memset(m_pItem->s_title, 0, TITLE_MAXSIZE+1);
}
/*!
   Fully specified construction the initial values of the various
   fields are specified by the constructor parameters. 

   \param reason     - Why the state change buffer is being emitted (the item type).
   \param runNumber  - Number of the run that is undegoing transitino.
   \param timeOffset - Number of seconds into the run at which this is being emitted.
   \param timestamp  - Absolute time to be recorded in the buffer.. tyically
                       this should be time(NULL).
   \param title      - Title string.  The length of this string must be at most
                       TITLE_MAXSIZE.

   \throw CRangeError - If the title string can't fit in s_title.
*/
CRingStateChangeItem::CRingStateChangeItem(uint16_t reason,
					   uint32_t runNumber,
					   uint32_t timeOffset,
					   time_t   timestamp,
					   std::string title) throw(CRangeError) :
  CRingItem(reason, sizeof(StateChangeItem)),
  m_pItem(0)

{
  init();

  // Everything should work just fine now:

  m_pItem->s_runNumber = runNumber;
  m_pItem->s_timeOffset= timeOffset;
  m_pItem->s_Timestamp = timestamp;
  setTitle(title);		// takes care of the exception.


}

/*!
   Constructor that copies/converts an existing ring item into a state change
   item. This is often used to wrap a state change object around a ring item just
   fetched from the ring, and determined by the caller to be a state change item.

   \param item - The source item.
   \throw bad_cast - the item is not a state change item.
*/
CRingStateChangeItem::CRingStateChangeItem(const CRingItem& item) throw(std::bad_cast) : 
  CRingItem(item)
{
  if (!isStateChange()) {
    throw bad_cast();
  }

  init();
}
/*!
  Copy construction.
  \param rhs - Basis for the construction.
*/
CRingStateChangeItem::CRingStateChangeItem(const CRingStateChangeItem& rhs) :
  CRingItem(rhs)
{

  init();

}

/*!
  Destruction just chains to the base class.

*/
CRingStateChangeItem::~CRingStateChangeItem()
{
}

/*!
  Assignment is base class assignment and then setting the item pointer:
  \param rhs - The item being assigned to us.
  \return CRingStateChangeItem&
  \retval *this
*/
CRingStateChangeItem& 
CRingStateChangeItem::operator=(const CRingStateChangeItem& rhs) 
{
  if (this != &rhs) {
    CRingItem::operator=(rhs);
    m_pItem = reinterpret_cast<pStateChangeItem>(getItemPointer());
  }
  return *this;
}
/*!
  Comparison is just the base class compare as comparing the item pointers is a bad
  idea since there could be two very different objects that have equal items.
 
  \param rhs - Right hand side object of the == operator.
  \return int
  \retval 0 not equal
  \retval 1 equal.
*/
int 
CRingStateChangeItem::operator==(const CRingStateChangeItem& rhs) const
{
  return CRingItem::operator==(rhs);
}

/*!
   Inequality is just the logical inverse of equality.
*/
int
CRingStateChangeItem::operator!=(const CRingStateChangeItem& rhs) const
{
  return !(*this == rhs);
}

///////////////////////////////////////////////////////////////////////////////
//
// Accessor methods.

/*!
   Set the run number for the item.
   \param run - new run number.
*/
void
CRingStateChangeItem::setRunNumber(uint32_t run)
{
  m_pItem->s_runNumber = run;
}
/*!
  \return uint32_t
  \retval the item's current run number.
*/
uint32_t
CRingStateChangeItem::getRunNumber() const
{
  return m_pItem->s_runNumber;
}

/*!
   Set the elapsed run time field for the item.
   \param offset - seconds into the run.
*/
void
CRingStateChangeItem::setElapsedTime(uint32_t offset)
{
  m_pItem->s_timeOffset = offset;
}
/*!
  \return uint32_t
  \retval time offset field of the item.
*/
uint32_t
CRingStateChangeItem::getElapsedTime() const
{
  return m_pItem->s_timeOffset;
}
/*!
  Set the title string.
  \param title - new title string.
  \throw CRangeError - if the title string is too long to fit.
*/
void
CRingStateChangeItem::setTitle(string title)  throw(CRangeError)
{
  // Ensure the title is small enough.

  if(title.size() > TITLE_MAXSIZE) {
    throw CRangeError(0, TITLE_MAXSIZE, title.size(),
		      "Checking size of title against TITLE_MAXSIZE");
  }
  strcpy(m_pItem->s_title, title.c_str());
}

/*!
    \return std::string
    \retval the title string of the item.
*/
string
CRingStateChangeItem::getTitle() const
{
  return string(m_pItem->s_title);
}
/*!
   Set the timestamp
   \param stamp  - new timestamp.
*/
void
CRingStateChangeItem::setTimestamp(time_t stamp)
{
  m_pItem->s_Timestamp  = stamp;
}
/*!
    \return time_t
    \retval timestamp giving absolute time of the item.
*/
time_t
CRingStateChangeItem::getTimestamp() const
{
  return m_pItem->s_Timestamp;
}

////////////////////////////////////////////////////////////////////////////////
//
// Virtual method overrides:

/**
 * typeName
 *
 *  Stringify the type of the item.
 *
 * @return std::string - the item type
 */
std::string
CRingStateChangeItem::typeName() const
{
  switch (type()) {
  case BEGIN_RUN:
    return " Begin Run ";
  case END_RUN:
    return " End Run ";
  case PAUSE_RUN:
    return " Pause Run ";
  case RESUME_RUN:
    return " Resume Run ";
  }
}
/**
 * toString
 *
 * Returns a string that is the ascified body of the item.
 *
 * @return std::string - ascified version of the item.
 */
std::string
CRingStateChangeItem::toString() const
{
  std::ostringstream out;		//  Build up via outputting to this psuedo stream.

  uint32_t run       = getRunNumber();
  uint32_t elapsed   = getElapsedTime();
  string   title     = getTitle();
  string   timestamp = timeString(getTimestamp());

  out <<  timestamp << " : Run State change : " << typeName();
  out << " at " << elapsed << " seconds into the run\n";
  out << "Title     : " << title << std::endl;
  out << "Run Number: " << run   << endl;


  return out.str();
}
    
///////////////////////////////////////////////////////////////////////////////
// Private utilities.

/* 
 *  Initialize member data construction time.
 */
void 
CRingStateChangeItem::init()
{
  m_pItem = reinterpret_cast<pStateChangeItem>(getItemPointer());
  uint8_t* pCursor = reinterpret_cast<uint8_t*>(getBodyPointer());
  pCursor         += sizeof(StateChangeItem) - sizeof(RingItemHeader);
  setBodyCursor(pCursor);
  updateSize();

}

/*
 * Evaluate wheter or the type of the current ring item is 
 * a state change or not.
 */
bool
CRingStateChangeItem::isStateChange()
{
  int t = type();
  return (
	  (t == BEGIN_RUN )              ||
	  (t == END_RUN)                 ||
	  (t == PAUSE_RUN)               ||
	  (t == RESUME_RUN));
}
