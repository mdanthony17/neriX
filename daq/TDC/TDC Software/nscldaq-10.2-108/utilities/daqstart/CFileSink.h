// Author:
//   Ron Fox
//   NSCL
//   Michigan State University
//   East Lansing, MI 48824-1321
//   mailto:fox@nscl.msu.edu
//


/*!
  \class CFileSink

  Sink That is attached to a file.

*/


#ifndef __CFILESINK_H  //Required for current class
#define __CFILESINK_H

//
// Include files:
//

                               //Required for base classes
#ifndef __CSINK_H     //CSink
#include "CSink.h"
#endif
 
class CFileSink  : public CSink        
{
private:
  

    int m_nFd;			//!< File descriptor of sink.  
   
public:
    //  Constructors and other canonical operations.
    //  You may need to adjust the parameters
    //  and the visibility esp. if you cannot
    // implement assignment/copy construction.
    // safely.

  CFileSink (std::string sCommand, 
	     std::string sFilename);		//!< Constructor.
  virtual  ~ CFileSink ( ); //!< Destructor.
  CFileSink (const CFileSink& rSource ); //!< Copy construction.
  CFileSink& operator= (const CFileSink& rhs); //!< Assignment.
  int operator== (const CFileSink& rhs) const; //!< == comparison.
  int operator!= (const CFileSink& rhs) const { //!< != comparison.
    return !(operator==(rhs));
  }

// Class operations:

public:

  virtual   CSink* clone();	//!< create duplicate of this.
  virtual   bool Log (const std::string& Message)   ; 


};

#endif
