#include <config.h>
#include "CChannel.h"
#include <stdio.h>


using namespace std;






/**
 * Construct a channel.  This member function 
 * initializes the data associated with a channel, but does not actually
 * connect it to EPICS.  See the Connect function to do that.
 *
 * @param name
 *     Name of the channel.
 *
 * Note that if the channel does not exist, it is not an error: it will just
 * never be marked connected.
 */
CChannel::CChannel(string name) :
  m_sName(name),
  m_fConnected(false),
  m_nChannel(0),
  m_fUpdateHandlerEstablished(false),
  m_fConnectionHandlerEstablished(false),
  m_sValue(""),
  m_LastUpdateTime(0),
  m_pConverter(0),
  m_pHandler(0),
  m_pHandlerData(0),
  m_eventHandlerId(0)
{
}
/**
 *  Destroys a channel, must cancel all events etc.
 */
CChannel::~CChannel()
{
  if (m_fConnected || (m_fUpdateHandlerEstablished)) {
    ca_clear_event(m_eventHandlerId);
    ca_clear_channel(m_nChannel);
    doEvents(0.1);
  }
  delete m_pConverter;
}

/**
 *   Return the name associated with the channel.
 * 
 * \return string
 * \retval Name of the channel associated with this object.
 */
string
CChannel::getName() const
{
  return m_sName;
}

/**
 * Tell the caller if this channel is connected to the EPICS event system.
 * \return bool
 * \retval true  - Connected to the system.
 * \retval false - Not connected to the system.
 */
bool
CChannel::isConnected() const
{
  return m_fConnected;
}

/**
 * Connect the channel to epics.  We do a ca_search_and_connect
 * specifying ourselves as the 'channel data' and a static member
 * as the handler.
 */
void
CChannel::Connect()
{
  if(!m_fConnectionHandlerEstablished) {
    ca_search_and_connect(m_sName.c_str(), &m_nChannel, StateHandler, (void*)this);
  }
  m_fConnectionHandlerEstablished = true;
}

/**
 * Returns the time at which this channel last received an update.
 */
time_t
CChannel::getLastUpdate() const
{
  return m_LastUpdateTime;
}

/**
 *  Returns the stringified version of the most recent channel value:
 */
string
CChannel::getValue() const
{
  return m_sValue;
}


/*!
  Sets a handler slot for channel value notifications:
  \param handler : CChannel::Slot 
     Function called when the channel value changes. If this is null,
     then notification is disabled.
  \param data : void*
     Data passed as the second parameter of the handler.
     The first parameter of the handler is a pointer to the channel.

*/
void
CChannel::setSlot(CChannel::Slot handler, void* data)
{
  m_pHandler     = handler;
  m_pHandlerData = data;
}

/*!
   set the channel to a string value..no-op if not connected.
*/
string
CChannel::operator=(string value)
{
  if(m_fConnected) {
    ca_put(DBF_STRING, m_nChannel, const_cast<char*>(value.c_str()));
    ca_flush_io();
  }
  return value;
}
/*!
   Set the channel to an integer value.
*/

int
CChannel::operator=(int value)
{
  if (m_fConnected) {
    ca_put(DBF_INT, m_nChannel, &value);
    ca_flush_io();
  }
  return value;
}

/*!
  Set channel to a double  value.
*/
double
CChannel::operator=(double value)
{
  if (m_fConnected) {
    ca_put(DBF_DOUBLE, m_nChannel, &value);
    ca_flush_io();
  }
  return value;

}


/**
 * This function is a class level function that processes EPICS events
 * for some fixed number of seconds.
 */
void
CChannel::doEvents(float seconds)
{
  ca_pend_event(seconds);
}


/**
 * This function is a class level function that gets the
 * channel state change events.  The channel has associated with it
 * a pointer to a CChannel object.  We use this to establish
 * object context and manipulate the actual connection status.
 *
 */
void
CChannel::StateHandler(connection_handler_args args)
{
  chid            id = args.chid;
  long            op = args.op;
  CChannel* pChannel = (CChannel*)ca_puser(id);

  // What we do now depends on the what's happened:

  if (op == CA_OP_CONN_UP) {	// Just connected...
    pChannel->m_fConnected = true;
    if(!pChannel->m_fUpdateHandlerEstablished) {
      pChannel->m_pConverter = CConversionFactory::Converter(ca_field_type(id));
      ca_add_event(pChannel->m_pConverter->requestType(), 
		   id, UpdateHandler, (void*)pChannel, 
		   &(pChannel->m_eventHandlerId));
      pChannel->m_fUpdateHandlerEstablished;
    }
  }
  else if (op == CA_OP_CONN_DOWN) { // just disconnected
    pChannel->m_fConnected = false;
    delete pChannel->m_pConverter;
    pChannel->m_pConverter = 0;
  }
  else {			// none of the above.
    // TODO: Figure out appropriate error handling here.
  }
}
/**
 * Called in response to a channel update event.
 * - args.usr  - points to the object associated with the event.
 * - args.dbr  - is a const char* for the value....
 *               but only if:
 * - args.status - ECA_NORMAL for dbr to be valid else something else.
 *
 */
void
CChannel::UpdateHandler(event_handler_args args)
{
  if(args.status == ECA_NORMAL) {
    CChannel* pChannel = (CChannel*)args.usr;
    if(pChannel->m_pConverter) {
      time_t    now      = time(NULL); // Last update time.
      pChannel->m_LastUpdateTime = now;
      pChannel->m_sValue = (*(pChannel->m_pConverter))(args);

      // If necessary, invoke the user's update handler.

      if(pChannel->m_pHandler) {
	(pChannel->m_pHandler)(pChannel, pChannel->m_pHandlerData);
      }
    }
  }
  else {
    // TODO:  Figure out appropriate error action if any.
  }
}



/*!
 *  Connection factory converter instantiator.
 *   \param type - the underlying epics data type of the channel.
 */
CConverter*
CConversionFactory::Converter(short type) {
  switch (type) {
  case DBF_STRING:
    return new CStringConverter;
    break;
  case DBF_CHAR:
  case DBF_SHORT:
  case DBF_LONG:
  case DBF_ENUM:
    return new CIntegerConverter;
    break;
  case DBF_FLOAT:
  case DBF_DOUBLE:
    return new CFloatConverter;
    break;
    // Default is a string as well:
  default:
    return new CStringConverter;
  }
}

/////////////////////////////////////////////////////////////
/*!
   Return the request type appropriate to a string converter
   (DBF_STRING)
*/
short
CStringConverter::requestType()
{
  return DBF_STRING;
}
/*!
   Convert a string data type to a string... basically
   just casting/assignment
*/
string
CStringConverter::operator()(event_handler_args args)
{
  return string((const char*)(args.dbr));
}
////////////////////////////////////////////////////////////
/*!
   Return the request type appropriate to an integer converter
   (DBF_LONG)
*/
short
CIntegerConverter::requestType()
{
  return DBF_LONG;
}
/*!
   Return the stringified version of an integer value.
*/
string
CIntegerConverter::operator()(event_handler_args args)
{
  char buffer[100];
  sprintf(buffer, "%ld", *((long*)(args.dbr)));
  return string(buffer);
}
////////////////////////////////////////////////////////////


/*!
    Return the request type appropriate to a floating point value
   conversion (DBF_DOUBLE)
*/
short
CFloatConverter::requestType()
{
  return DBF_DOUBLE;
}
/*!
   Return the stringified version of a double channel
*/
string
CFloatConverter::operator()(event_handler_args args)
{
  char buffer[100];
  sprintf(buffer, "%g", (*(double*)(args.dbr)));
  return string(buffer);
}


