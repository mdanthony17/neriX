#include <cadef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>



const static float TIMEOUT(2.0); // Seconds.


/*!
   This function kludges around a problem with epics.  It's supposed
   to start up caRepeater, but evidently:
   - It does that by forking and making caRepeater the parent process.
   - the resulting caRepeater identifies to ps as us.
   - After our process exits, the resulting caRepeater doesn't work quite
     right but hangs around.

  What we do that seems to work around this is to manually start caRepeater.
  We assume that caRepeater is in PATH.

*/
void
startRepeater(int argc, char** argv)
{
  int pid = fork();
  if(pid < 0) {
    perror("Failed to fork in startRepeater()");
    exit(errno);
  }
  if (pid == 0) {		// child process
    if(daemon(0,0) < 0) {
      perror("Child could not setsid");
      exit(errno);
    }
    argv[0] = const_cast<char*>("caRepeater");
    int stat = execlp("caRepeater", 
		      "caRepeater", NULL);
    perror("Child: execlp failed!!");
    exit(errno);
  }
  usleep(1000);
}

/*!
  Initializes access to EPICS
  \throw fatal exception caught by epics
        on error.
*/
void EpicsInit(int argc, char** argv)
{

  startRepeater(argc, argv);
  int status = ca_task_initialize();
  SEVCHK(status, "Initialization failed");
  
}
/*!
    Shutdown access to epics.
    \throw fatal exception caught by epics
       on error.
*/
void EpicsShutdown()
{
  while(ca_test_io() == ECA_IOINPROGRESS) {
    ca_pend_io(1.0);
  }
  int  status     = ca_task_exit();
  SEVCHK(status,"Task exit call failed");
}
/*!
    Get the value of a channel in epics, along with its
    units.  The data are returned in a string of the
    form name:   value units
    \param output (char* [out]):
       String buffer to hold the output
    \param chan (const char* [in]):
       String containing the channel name.
       If there was a failure, this will contain an
       error message.

*/
void GetChannel(char* output, const char* chan)
{
  chid channel_id;
  chid units_id;

  char units[100];
  strcpy(units, chan);
  strcat(units,".EGU");

  int status = ca_search(chan, &channel_id);
  if(status != ECA_NORMAL) {
    sprintf(output, "%s: ca_search failed: %s",
	    chan, ca_message(status));
    return;
  }

   status     = ca_search(units, &units_id);
  if(status != ECA_NORMAL) {
    sprintf(output, "%s: ca_search failed: %s", 
	    chan, ca_message(status));
    return;
  }

  status    = ca_pend_io(TIMEOUT);
  if(status != ECA_NORMAL) {
    sprintf(output, "%s: ca_pend_io failed %s",
	    chan, ca_message(status));
    return;
  }


  char chanvalue[100];
  char unitsvalue[100];
  
  status = ca_get(DBR_STRING, channel_id, chanvalue);
  if(status != ECA_NORMAL) {
    sprintf(output, "%s: ca_get failed %s",
	    chan, ca_message(status));
    return;
  }

  status = ca_get(DBR_STRING, units_id, unitsvalue);
  if(status != ECA_NORMAL) {
    sprintf(output, "%s: ca_get failed %s",
	    units, ca_message(status));
    return;
  }


  status = ca_pend_io(TIMEOUT);
  if(status != ECA_NORMAL) {
    sprintf(output, "%s: ca_pend_io failed %s",
	    chan, ca_message(status));
    return;
  }

  sprintf(output, "%s: %s %s",
	  chan, chanvalue, unitsvalue);

  status    = ca_clear_channel(channel_id);
  SEVCHK(status, "Clear_channel on channel");

  status    = ca_clear_channel(units_id);
  SEVCHK(status, "Clear_Channel on units");

  status    = ca_pend_io(TIMEOUT);
  SEVCHK(status, "ca_pend_io after clear");



}
