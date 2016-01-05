#ifndef __CPORTMANAGEREXCEPTION_H
#define __CPORTMANAGEREXCEPTION_H
/*!
 *   This is an exception class that 
 * provides support for the exceptions that can be thrown by the
 * CPortManager class.  These exceptions represnt problems in allcoating
 * ports via the port manager server and/or communicating with the server.
 */
// Author:
//    Ron Fox
//    NSCl
//    Michigan State University
//    East Lansing, MI 48824-1321
//

#ifndef  EXCEPTION_H
#include <Exception.h>
#endif

#ifndef __STL_STRING
#include <string>
#ifndef __STL_STRING
#define __STL_STRING
#endif
#endif

#ifndef OSTREAM_DAQH
#include <ostream>
#endif

class CPortManagerException : public CException
{
public:
	typedef enum _Reason {
		NoPorts,
		NotLocal,
		ConnectionFailed
	} Reason;
private:
	std::string    m_sHost;            //!< Host with the server.
	Reason         m_eReason;          //!< Why the operation threw.
	mutable std::string m_sReasonText; //!< Scratch storage fro ReasonText()

	// Constructors and other canonical operations:
	
public:
	CPortManagerException(std::string host,
						  Reason      why,
						  std::string doing);
	CPortManagerException(const CPortManagerException& rhs);			
	virtual ~CPortManagerException();
	
	CPortManagerException& operator=(const CPortManagerException& rhs);
	int operator==(const CPortManagerException& rhs);
	int operator!=(const CPortManagerException& rhs);
	
    // Overrides:
    
    virtual const char* ReasonText() const;
    virtual Int_t       ReasonCode() const;
    
    static std::string  ReasonCodeToText(int code);
   
 	
};
// Handy stream put operator for formatting the exception.

std::ostream& operator<<(std::ostream& f, const CPortManagerException& e); 

#endif
