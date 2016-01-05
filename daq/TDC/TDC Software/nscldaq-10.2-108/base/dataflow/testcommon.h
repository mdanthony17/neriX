#ifndef __TESTCOMMON_H
#define __TESTCOMMON_H

#include <string>

// file containing test code that's factored out.

//  Default the directory that has shared memory special files
//  to the one used by linux.
//  This can be overridden for other systems and places.
//


#ifndef SHM_DIRECTORY
#define SHM_DIRECTORY "/dev/shm/"
#endif


void* mapRingBuffer(const char* name);
std::string shmName(std::string name);

#endif
