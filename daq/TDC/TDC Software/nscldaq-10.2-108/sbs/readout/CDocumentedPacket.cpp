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

static const char* Copyright = "(C) Copyright Michigan State University 2002, All rights reserved";   
//////////////////////////CDocumentedPacket.cpp file////////////////////////////////////

#include <config.h>
#include "CDocumentedPacket.h"                  
#include "CDocumentedPacketManager.h"
#include "CInvalidPacketStateException.h"
#include "CReadoutMain.h"

#include <stdio.h>
#include <time.h>

using namespace std;


/*!
   Default constructor.  This is called when declarations of the form e.g.:
   -  CDocumentedPacket  object;
   are performed.
*/
CDocumentedPacket::CDocumentedPacket (unsigned short nTag,
				      const string&  rName,
				      const string&  rDescription,
				      const string&  rVersion) :
  m_nTag(nTag),
  m_sName(rName),
  m_sDescription(rDescription),
  m_pHeaderPtr(0),
  m_sVersion(rVersion),
  m_fPacketInProgress(false)
{
  // Figure out the textual date for m_sInstantiationDate

  time_t t = time(NULL);
  m_sInstantiationDate = ctime(&t);

  // The Documented packet manager throws if there's a duplicate packet.

  CDocumentedPacketManager::getInstance()->AddPacket(*this);

} 

CDocumentedPacket::~CDocumentedPacket()
{
  if(m_fPacketInProgress) {
    throw CInvalidPacketStateException(m_fPacketInProgress,
			      "Packet must be closed to destroy");
  }
  CDocumentedPacketManager::getInstance()->DeletePacket(m_sName);
}

// Functions for class CDocumentedPacket

/*!
    Returns a formatted string describing the
    packet which can be inserted in a documentation
    buffer.  This is returned as a colon separated set of fields:
    - Packet id
    - Packet Name
    - Packet Description
    - Packet Version
    - Packet instantiation date.
*/
string 
CDocumentedPacket::Format()  
{
  char   PacketId[10];
  sprintf(PacketId, ":0x%x", m_nTag);

  string Output(m_sName);
  Output += PacketId;
  Output += ":";
  Output += m_sDescription;
  Output += ":";
  Output += m_sVersion;
  Output += ":";
  Output += m_sInstantiationDate;

  return Output;
}  

/*!
    Requests a packet of this type be opened.  
    Throws CInvalidPacketState if packet already open.
    m_fPacketInProgress -> true
    m_pHeader pointer is captured from the current pointer,
    Space is reserved for the word count,  and the id is written.
    Pointer to the next free spot in the buffer is returned.

	\param rPointer Current pointer into the buffer.
	      
	\returns Pointer to body of packet.

*/

unsigned short* 
CDocumentedPacket::Begin(unsigned short* rPointer)  
{
  if(m_fPacketInProgress) {
    throw CInvalidPacketStateException(m_fPacketInProgress, 
			      "Packet must be closed to BeginPacket");
  }
  m_fPacketInProgress = true;
  m_pHeaderPtr        = rPointer;
  

  unsigned short* p  = m_pHeaderPtr;

  *p = 0;			// Don't know word count yet so reserve a
  ++p;				// longword for it.
  *p = 0;
  ++p;			

  *p = m_nTag;			// The packet is tagged with our id.
  ++p;
  return p;			// Return 'pointer' to packet body.
 
}  

/*!
    Closes a packet.  If m_fPackteInProgress is false,
    throws CInvalidPacketState.  Otherwise:
    m_fPacketInProgress -> false, the Buffer pointer
    is used to determine, and fill in the word count of
    the packet.  A buffer pointer to the next slot
    is returned (to allow for the possibility there's trailer
    data too).
    

	\param rBuffer - Pointer to the first word after the packet.
	\returns Pointer to the next word in the buffer to fill.
	         In this version, that's just rBuffer, however,
		 this allows for a trailer as well as a header to be inserted.

*/

unsigned short* 
CDocumentedPacket::End(unsigned short* rBuffer)  
{
  if(!m_fPacketInProgress) {
    throw CInvalidPacketStateException(m_fPacketInProgress,
			      "Packet must be open to be ended.");
  }
  int size;

  size = (unsigned short)(rBuffer - m_pHeaderPtr);

  // set the size longword or word (depending on the jumbo-ness of the buffer)

  union {
    int   l;
    short w[2];
  } lw;
  lw.l = size;
  m_pHeaderPtr[0]   = lw.w[0];
  m_pHeaderPtr[1]   = lw.w[1]; 
  

  // The packet is no longer open.

  m_fPacketInProgress = false;
  return rBuffer;
}
/*!
    Get a pointer to the current packet.  This throws  CInvalidPacketState if there is not
   an open packet, and is therefore not a trivial selector.
*/
unsigned short* 
CDocumentedPacket::getHeaderPtr() const
{
   if(!m_fPacketInProgress) {
      throw CInvalidPacketStateException(m_fPacketInProgress, 
				 "Packet must be open to get header pointer");
   }
   return m_pHeaderPtr;
}
