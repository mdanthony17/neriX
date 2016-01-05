//-------------------------------------------------------------------
//  control.h   --   DAX: DAQ for XENON100 
// 
//  the control library contains functions to control the DAX program
//
//  17.01.2008 MS
//-------------------------------------------------------------------

#include <libxdio.h>

//-------------------------------------------------------------------
// returns time in milliseconds
int32_t get_time();

//-------------------------------------------------------------------
// changes the screen output color
// (you have to reset it to "black" manually afterwards)
void col(const char *str);

//-------------------------------------------------------------------
// Print the Start Screen
int control_printtitle(const char *SwRelease);	// version date

//-------------------------------------------------------------------
// display help menu for oscilloscope mode
int control_scopehelp();

//-------------------------------------------------------------------
// Print Error Message on Screen
int errormsg(	int flag, 			// 1(0): write (not) to logfile
		const char *s);			// string with message

//-------------------------------------------------------------------
// print something in logfile (without knowing the logfile handler)
int control_log(FILE *file, int mode, const char *s);

//-------------------------------------------------------------------
// returns a string with current date and time
int control_gettimestring(char *timestr, const char format);

//-------------------------------------------------------------------
// generates logfile
int control_openlog(FILE **logfile, 		// File handle
			char *FileName,		// Filename
			const char *SwRelease);	// to print logfile header	

//-------------------------------------------------------------------
// printf to logfile (with current time)
int control_printlog(	FILE *logfile,		// File handle
			int time,		// 1..print time stamp, 0 else
			const char *text);		// text to print

//-------------------------------------------------------------------
// printf counter values to logfile 
int control_cnts2log(	FILE *logfile, 		// File handle
			scaler& s);		// Scaler Info
			
//-------------------------------------------------------------------
// Parse the command line and set the variables
int control_cmdline( 	int argc, 		// cmd line arg
			char *argv[], 		// cmd line arg
			char *envp[], 		// cmd line arg
			int& time, 		// measuring time
			u_int32_t& NoE, 	// number of events
			var_graphics& graphics, // graphical display?
			int& baseline, 		// baseline adjustment?
			digitizer& adc,		// for BoardInfo?
			char *OutFileName,	// filename for data storage
			char *XmlFileName);	// xml-file 

//-------------------------------------------------------------------
// Read configuration file and set up VME connection
// returns -1 on error
int control_configfile(	char *ConfigFileName,	// file to read
    			crate& crt,		// holds crate infos 
			digitizer& adc,		// holds adc infos 
			scaler& s,		// holds scaler infos
			multipurposemodule& mp, // holds v1495 module
			int& WriteToFile);	// how is the data stored?
			
//-------------------------------------------------------------------
// pre-defined configurations for automatic baseline determination
// returns -1 on error
int control_configbaseline(crate& crt, 		// crate info
			digitizer& adc);	// adc info
			
//-------------------------------------------------------------------
// open file to store data
// returns -1 if file cannot be opened
int control_openfile(	FILE **ofile, 		// file handle
			char *FileName, 	// file name
			int& Filecounter,	// ...
			int WriteToFile,	// binary or ASCII file?
			digitizer& adc,		// adc information
			FILE *logfile);		// to write filename to logfile

//-------------------------------------------------------------------
// open XE100 data format file to store data without TDC included
// returns -1 if file cannot be opened
int control_openxdio(	xdio_file_t **ofile,
			xdio_chunk_att_t **xatt,
			char *FileName,
			int& FileCounter, 
			int WriteToFile, 
			digitizer& adc,
			FILE *logfile,
			int EventsPerFile, 
			timeandtrigger& t);

//-------------------------------------------------------------------
// open XE100 data format file to store data with TDC included
// returns -1 if file cannot be opened
int control_openxdio_tdc(	xdio_file_t **ofile,
			xdio_chunk_att_t **xatt,
			xdio_chunk_att_t **xatt_tdc,
			char *FileName,
			int& FileCounter, 
			int WriteToFile, 
			digitizer& adc,
			timedc& tdc,
			FILE *logfile, 
			int EventsPerFile, 
			timeandtrigger& t);

//-------------------------------------------------------------------
// generate file with basic information to be read by Slow Control
int control_scfile(	int time, 		// time to calculate average
			float average,		// average count rate
			int stopped);		// DAQ stopped flag  

//-------------------------------------------------------------------
// calculate and print count and trigger rates
// returns -1 if measurement should be stopped since no data is seen
int control_calcrate(	timeandtrigger& t,	// holds time and trigger info
			scaler& s,		// scaler info
			int FileEvtCounter);	// number of Events in the current file

//-------------------------------------------------------------------
// writes data to file
// returns the number of new triggers on success
// returns -1 if failure
int control_writedata(	digitizer& adc,		// adc info
			int i,			// current module
			FILE *ofile,		// file handle
			int WriteToFile,	// binary or ASCII data?
			int blt_bytes,		// number of bytes read
			u_int32_t *buff);	// data to write
			
//-------------------------------------------------------------------
// writes data to file: XENON100 file format
// returns the number of new triggers on success
// returns -1 if failure
int control_writexdio(	digitizer& adc,		// adc info
  		    timeandtrigger& t,      // time information
			int i,			// current module
			xdio_file_t *ofile, 	// file handle
			xdio_chunk_att_t *xatt, // attribute list
			int WriteToFile,	// binary or ASCII data?
			int blt_bytes,		// number of bytes read
			u_int32_t *buff);     // buffer to store data for adc


//-------------------------------------------------------------------
// writes data to file: XENON100 file format
// returns the number of new triggers on success
// returns -1 if failure
int control_writexdio_tdc(	digitizer& adc,		// adc info
			timedc& tdc,	// tdc info
  		    timeandtrigger& t,      // time information
			int i,			// current module
			xdio_file_t *ofile, 	// file handle
			xdio_chunk_att_t *xatt, // attribute list
			xdio_chunk_att_t *xatt_tdc, // TDC attribute list
			int WriteToFile,	// binary or ASCII data?
			int blt_bytes,		// number of bytes read
			u_int32_t *buff,
			u_int32_t *buff_tdc);	// data to write


//-------------------------------------------------------------------
// return trigger time from data
u_int32_t control_getTTT(u_int32_t *buff);

//-------------------------------------------------------------------
// generates a filename using path and the current dat/time
int control_generatefilename(char *OutFileName, // string to store filename
			char *path);		// path to store data

//-------------------------------------------------------------------
// calculate and print time in "good" format
int control_calctime(timeandtrigger& t, int32_t ms, char *text);

//-------------------------------------------------------------------
// set variables to default values
int control_setdefault(	digitizer& adc,		// adc info
			scaler& s,		// scaler info
			timeandtrigger& t,	// timing info
			crate& crt,		// holds crate infos 
			multipurposemodule& mp, // holds v1495 module
			var_graphics& g);	// graphics output 

//-------------------------------------------------------------------
// extracts module and channel of a given PMT number
int control_getadc(	digitizer& adc, 	// adc info
			int pmt, 		// PMT number on
			int& module,  		// ...module
			int& channel);		// ...channel

