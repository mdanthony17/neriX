#include "ringtostdoutsw.h"
#include "CRingBuffer.h"
#include "Exception.h"

#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <errno.h>
#include <string.h>
#include <io.h>

using namespace std;

/**************************************************************
 * integerize:                                                *
 *   Utility function that converts a string to an integer    *
 *   while allowing the suffixes k, m, g to be kilo, mega     *
 *   and giga multipliers respectively.  Computer sizes e.g.  *
 *   k = 1024, m = 1024*1024, g = 1024*1024*1024              *
 *                                                            *
 * On failure, this will print the usage and exit with a fail *
 * code.                                                      *
 *                                                            *
 * Parameters:                                                *
 *   const char* str  - The input string                      *
 * Returns:                                                   *
 *   The integer result                                       *
 *************************************************************/

static int 
integerize(const char* str)
{
  char* endptr;

  // Decode the front part:

  int base = strtol(str, &endptr, 0); // Allow hex etc.
  
  // It's an error to not have at least one digit:

  if (endptr == str) {
    cmdline_parser_print_help();
    exit(EXIT_FAILURE);
  }

  // If the entire string was converted return it:

  if(strlen(endptr) == 0) {
    return base;
  }
  // If there's more than one extra char, that's an error too:

  if (strlen(endptr) > 1) {
    cmdline_parser_print_help();
    exit(EXIT_FAILURE);
  }

  // One extra character can be the multiplier

  char multiplier = *endptr;
  int value;

  switch (multiplier) {
  case 'k':
    value = base*1024;
    break;
  case 'm':
    value = base*1024*1024;
    break;
  case 'g':
    value = base*1024*1024*1024;
    break;

  default:
    cmdline_parser_print_help();
    exit(EXIT_FAILURE);
  }
  return value;
}
/********************************************************************
 * writeData                                                        *
 *   Writes a block of data to an output file descriptor.   If      *
 *   necessary, several writes are done until the data are all      *
 *   written or a failure occurs.  In this application, failure     *
 *   results in process exit.                                       *
 * Parameters:                                                      *
 *   int     fd       - File descriptor to which to write the data  *
 *   void*   pData    - Pointer to the data to write.               *
 *   size_t  size     - Number of bytes to write.                   *
 *******************************************************************/
void
writeData(int fd, void* pData, size_t size)
{

  try {
    io::writeData(fd, pData, size);
  }
  catch (int err) {
    std::string msg = "Write to output failed: ";
    if (err) {
      msg += strerror(err);
    } else {
      msg += ("End of file on output");
    }
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
  }
}

/********************************************************************
 * mainLoop                                                         *
 *  The application main loop.  We get data from the ring and shoot *
 *  it to stdout.  We attempt to read data in chunks of mindata     *
 *  but if that times out within the timeout period, we just read   *
 *  what's there and shovel it on out the pipe.                     *
 *                                                                  *
 * Parameters:                                                      *
 *   std::string ringname  - Name of the ring we must attach to     *
 *   int         timeout   - ms to wait for the whole mindata chunk *
 *   size_t      mindata   - Minimum desired data chunk             *
 *******************************************************************/

static void
mainLoop(string ring, int timeout, size_t mindata)
{
  // Attach to the ring. If we fail, report the error and exti.

 

  CRingBuffer* pSource;
  try {
    pSource = new CRingBuffer(ring);
  }
  catch (CException& error) {
    cerr << "ringtostdout Failed attach to " << ring << ":\n";
    cerr << error.ReasonText() << endl;
    exit(EXIT_FAILURE);
  }
  catch (string msg) {
    cerr << "ringtostdout Failed attach to " << ring << ":\n";
    cerr << msg << endl;
    exit(EXIT_FAILURE);
  }
  catch (const char* msg) {
    cerr << "ringtostdout Failed attach to " << ring << ":\n";
    cerr << msg <<endl;
    exit(EXIT_FAILURE);
  }
  catch (...) {
    cerr << "ringtostdout Failed attach to " << ring << endl;
    exit(EXIT_FAILURE);
  }

  CRingBuffer& source(*pSource);
  CRingBuffer::Usage use = source.getUsage();
  if (mindata > use.s_putSpace/2) {
    mindata = use.s_putSpace/2;
  }

  // Create our data buffer:

  char* pData = new char[mindata];
  while (1) {
    size_t gotten = source.get(pData, mindata, mindata,timeout);
    if (gotten < 0) {
      delete pSource;
      return;
    }
    if (gotten == 0) {
      gotten = source.get(pData, mindata, 1, 0); // Get what is there to get...
    }
    if  (gotten > 0) {
      writeData(STDOUT_FILENO, pData, gotten);
    }
  }
}

/*!
   The entry point parses the parameters and invokes
  the main loop which does all the grunt work.
*/

int main(int argc, char** argv)
{
  struct gengetopt_args_info parsed;

  int status = cmdline_parser(argc, argv, &parsed);
  if (status == EXIT_FAILURE) {
    exit(status);
  }

  // There should be exactly one parameter, that is not a switch,
  // the ring name:

  if (parsed.inputs_num != 1) {
    cmdline_parser_print_help();
    exit(EXIT_FAILURE);
  }
  // extract the rin name, the timeout and the size from the 
  // parameters/defaults:
  //

  string ringname = parsed.inputs[0];
  int    timeout  = parsed.timeout_arg;
  size_t mindata  = integerize(parsed.mindata_arg);


  mainLoop(ringname, timeout, mindata);
  
}
