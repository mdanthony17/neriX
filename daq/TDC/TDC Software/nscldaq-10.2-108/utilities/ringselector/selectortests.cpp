// Template for a test suite.

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Asserter.h>
#include "Asserts.h"
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <string>
#include <sys/wait.h>
#include <CRingBuffer.h>
#include <time.h>
#include <pwd.h>

#include <vector>

#include <DataFormat.h>
#include <CRingStateChangeItem.h>
#include <CRingPhysicsEventCountItem.h>
#include <CRingScalerItem.h>
#include <CRingTextItem.h>
#include <stdlib.h>
#include <os.h>


using namespace std;

pid_t childpid;


static void 
readItem(int fd, void* pBuffer)
{
  char* p = reinterpret_cast<char*>(pBuffer);

  // Read the header:

  size_t bytes = read(fd, p, sizeof(RingItemHeader));
  EQ(sizeof(RingItemHeader), bytes);
  pRingItemHeader h = reinterpret_cast<pRingItemHeader>(p);
  p += bytes;
  size_t remaining = h->s_size - sizeof(RingItemHeader);
  bytes = read(fd, p, remaining);
  ASSERT(remaining == bytes);

}



// Static utility program that will run a specified command
// with stdout and stderr -> a pipe.  The read end of the pipe is returned.
//

static int spawn(const char* command) 
{
  int pipes[2];
  if (pipe(pipes) == -1) {
    perror("Could not open pipes");
    throw "could not open pipes";
  }

  // I can never keep these straight so:

  int readPipe = pipes[0];
  int writePipe= pipes[1];

  if ((childpid = fork()) != 0) {
    // parent...will read from the pipe:

    sleep(1);			// Let child start.
    close(writePipe);
    return readPipe;

  }
  else {
    // child...


    close(readPipe);


    // Set the write pipe as our stdout/stderr:

    close(STDOUT_FILENO);
    dup2(writePipe, STDOUT_FILENO);

    system(command);
    exit(0);

  }
  
}

static void textItem(CRingBuffer& prod, int fd, bool check = true)
{
  vector<string>  items;
  items.push_back("String 1");
  items.push_back("String 2");
  items.push_back("The last string");

  CRingTextItem i(PACKET_TYPES, items);
  i.commitToRing(prod);

  if (check) {
    char buffer[2048];
    readItem(fd, buffer);
    pTextItem e = reinterpret_cast<pTextItem>(buffer);

    EQ(PACKET_TYPES, e->s_header.s_type);
    EQ((uint32_t)3,  e->s_stringCount);

    char* p = e->s_strings;
    string s1(p);
    p += s1.size() + 1;
    string s2(p);
    p += s2.size() +1;
    string s3(p);

    EQ(string("String 1"), s1);
    EQ(string("String 2"), s2);
    EQ(string("The last string"), s3);
  }
}

static void scaler(CRingBuffer& prod, int fd, bool check=true)
{
  vector<uint32_t> scalers;
  for (int i=0; i < 32; i++) {
    scalers.push_back(i);
  }
  CRingScalerItem i(0, 10, time_t(NULL), scalers);
  i.commitToRing(prod);

  if (check) {
    char buffer[1024];
    readItem(fd, buffer);

    pScalerItem e = reinterpret_cast<pScalerItem>(buffer);
    EQ(INCREMENTAL_SCALERS, e->s_header.s_type);
    EQ((uint32_t)0,         e->s_intervalStartOffset);
    EQ((uint32_t)10,        e->s_intervalEndOffset);
    EQ((uint32_t)32,        e->s_scalerCount);
    for (uint32_t i =0; i < 32; i++) {
      EQ(i, e->s_scalers[i]);
    }
  }

}

static void eventCount(CRingBuffer& prod, int fd, int count, bool check=true)
{
  CRingPhysicsEventCountItem i(count, 12);
  i.commitToRing(prod);

  if (check) {
    char buffer[1024];
    readItem(fd, buffer);
    pPhysicsEventCountItem e = reinterpret_cast<pPhysicsEventCountItem>(buffer);
    EQ(PHYSICS_EVENT_COUNT, e->s_header.s_type);
    EQ((uint32_t)12, e->s_timeOffset);
    EQ((uint64_t)count, e->s_eventCount);
  }
}


static void event(CRingBuffer& prod, int fd, bool check=true)
{
  CRingItem i(PHYSICS_EVENT);
  uint16_t* p = reinterpret_cast<uint16_t*>(i.getBodyPointer());
  *p++        = 11;
  for (int i =0; i < 10; i++) {
    *p++ = i;
  }
  i.setBodyCursor(p);
  i.commitToRing(prod);

  if (check) {
    char buffer[1024];
    readItem(fd, buffer);
    pPhysicsEventItem e = reinterpret_cast<pPhysicsEventItem>(buffer);
    EQ(PHYSICS_EVENT, e->s_header.s_type);
    EQ((uint16_t)11, e->s_body[0]);
    for (int i = 0; i < 10; i++) {
      EQ((uint16_t)i, e->s_body[i+1]);
    }
  }


}


static void beginRun(CRingBuffer& prod, int fd,  bool check = true)
{
  CRingStateChangeItem i(BEGIN_RUN, 1234, 0, time(NULL), "This is a title");
  i.commitToRing(prod);
  
  // Should now be able to read the item from the pipe and it should match
  // the item we put in.
  
  
  char buffer[1024];
 
  if (check) {
    // We should be able to get what we put in:
    
    readItem(fd, buffer);
    pStateChangeItem item = reinterpret_cast<pStateChangeItem>(buffer);
    EQ(BEGIN_RUN, item->s_header.s_type);
    EQ((uint32_t)1234,      item->s_runNumber);
    EQ((uint32_t)0,         item->s_timeOffset);
    EQ(string("This is a title"), string(item->s_title));
  }
}

static void pauseRun(CRingBuffer& prod, int fd, bool check=true)
{
  CRingStateChangeItem i(PAUSE_RUN, 1234, 15, time(NULL), "This is a title");
  i.commitToRing(prod);
  
  // Should now be able to read the item from the pipe and it should match
  // the item we put in.
  
  
  char buffer[1024];
 
  if (check) {
    // We should be able to get what we put in:
    
    readItem(fd, buffer);
    pStateChangeItem item = reinterpret_cast<pStateChangeItem>(buffer);
    EQ(PAUSE_RUN, item->s_header.s_type);
    EQ((uint32_t)1234,      item->s_runNumber);
    EQ((uint32_t)15,         item->s_timeOffset);
    EQ(string("This is a title"), string(item->s_title));
  }
}

static void resumeRun(CRingBuffer& prod, int fd, bool check = true) 
{
  CRingStateChangeItem i(RESUME_RUN, 1234, 15, time(NULL), "This is a title");
  i.commitToRing(prod);
  
  // Should now be able to read the item from the pipe and it should match
  // the item we put in.
  
  
  char buffer[1024];
 
  if (check) {
    // We should be able to get what we put in:
    
    readItem(fd, buffer);
    pStateChangeItem item = reinterpret_cast<pStateChangeItem>(buffer);
    EQ(RESUME_RUN, item->s_header.s_type);
    EQ((uint32_t)1234,      item->s_runNumber);
    EQ((uint32_t)15,         item->s_timeOffset);
    EQ(string("This is a title"), string(item->s_title));
  }
}


static void endRun(CRingBuffer& prod, int fd, bool check = true)
{
  CRingStateChangeItem i(END_RUN, 1234, 25, time(NULL), "This is a title");
  i.commitToRing(prod);
  
  // Should now be able to read the item from the pipe and it should match
  // the item we put in.
  
  
  char buffer[1024];
 
  if (check) {
    // We should be able to get what we put in:
    
    readItem(fd, buffer);
    pStateChangeItem item = reinterpret_cast<pStateChangeItem>(buffer);
    EQ(END_RUN, item->s_header.s_type);
    EQ((uint32_t)1234,       item->s_runNumber);
    EQ((uint32_t)25,         item->s_timeOffset);
    EQ(string("This is a title"), string(item->s_title));
  }
}

class rseltests : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(rseltests);
  CPPUNIT_TEST(all);
  CPPUNIT_TEST(exclude);
  CPPUNIT_TEST(only);
  CPPUNIT_TEST_SUITE_END();


private:

public:
  void setUp() {
    if (!CRingBuffer::isRing(Os::whoami())) {
      CRingBuffer::create(Os::whoami());
    }
  }
  void tearDown() {
  }
protected:
  void all();
  void exclude();
  void only();
};

CPPUNIT_TEST_SUITE_REGISTRATION(rseltests);


// Starts up the ring selector with no selectivity and
// ensures we can send data to that ring and get it back
// on the pipe connecting us to the child process.
// The preprocessor symobl BINDIR isthe directory in which
// the ringselector was installed.
//

void rseltests::all() 
{
  
  string programName = BINDIR;
  programName       += "/ringselector";

  int fd = spawn(programName.c_str());

  
  try {
    // attach to our ring as a producer.
    
    CRingBuffer prod(Os::whoami(), CRingBuffer::producer);
    
    // Make a begin_run item, commit it.
    
    beginRun(prod, fd);
    for (int i =0; i < 100; i++) {
      event(prod,fd);
    }
    eventCount(prod, fd, 100);
    scaler(prod, fd);
    pauseRun(prod, fd);
    resumeRun(prod, fd);
    textItem(prod, fd);
    endRun(prod, fd);
    


  }    
  catch (...) {
    kill(childpid, SIGTERM);
    
    int status;
    wait(&status);
    throw;
  }
  
  
  // Cleanup by killing the child.
  
  kill(childpid, SIGTERM);
  
  int status;
  wait(&status);
  close(fd);

}

// build use the --exclude switch to not accept BEGIN_RUN items.

void rseltests::exclude()
{
  string programName = BINDIR;
  programName       += "/ringselector --exclude=BEGIN_RUN";
  int fd             = spawn(programName.c_str());

  try {
    
    CRingBuffer prod(Os::whoami(), CRingBuffer::producer);
    

    beginRun(prod, fd, false);
    endRun(prod,fd);		// Should be the first one back from the program.
  }
  catch (...) {
    kill (childpid, SIGTERM);
    int s;
    wait(&s);
    throw;
  }

  kill (childpid, SIGTERM);
  int s;
  wait(&s);
}

// Build using the --accept switch...
void rseltests::only()
{
  string programName = BINDIR;
  programName       += "/ringselector --accept=BEGIN_RUN"; // only begin runs.
  int  fd            = spawn(programName.c_str());
  
  try {
    CRingBuffer prod(Os::whoami(), CRingBuffer::producer);

    beginRun(prod,fd);		// Should be fine.
    eventCount(prod, fd, 100, false);
    scaler(prod, fd, false);
    pauseRun(prod, fd, false);
    resumeRun(prod, fd, false);
    beginRun(prod,fd);

  }
  catch (...) {
    kill (childpid, SIGTERM);
    int s;
    wait(&s);
    throw;
  }

  kill (childpid, SIGTERM);
  int s;
  wait(&s);
}
// don't know how to test for sampling.
