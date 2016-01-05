//-------------------------------------------------------------------
//  control.cpp   --   multiDAX: DAQ for neriX
//
//  the control library contains functions to control the multiDAX program
//
//  18.03.2008 MS  (12.04.2008)
//  modified:
//    13.06.2008 MS	Changed Trigger Rate to DAQ Rate
//    24.06.2008 MS	Store DAQ rate in log file (cnt[13])
//			start xed-files with event 0 (was -1)
//    07.07.2008 MS	create directory for data automatically
//    24.07.2008 MS	include more warning messages (no data read etc.)
//    04.11.2008 MS	add 'xe100_' in data path
//    07.07.2009 MS add options for noise analysis
//    06.08.2009 MS use only 31 bit of TTT
//    03.09.2009 MS b/w output mode possible
//                  generate event timestamp using computer time and TTT
//	  09.11.2009 MS make measurement timing more precise (by considerings ms)
//	  15.03.2010 MS all UNIX time stamps in UTC, everything else local time
//	  30.07.2013 GP multiDAX version
//-------------------------------------------------------------------

extern "C" {
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/stat.h>
#include <unistd.h>
#ifdef WIN32
#include <time.h>
#include <sys/timeb.h>
#include <conio.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif
}

#include "global.h"
#include "CAENVMElib.h"
#include "adc.h"
#include <libxdio.h>	// for Xe100 Data Storage

#include <TH1D.h>

int control_printlog(FILE *logfile, int time, const char *text);

//-------------------------------------------------------------------
// returns time in milliseconds
int32_t get_time()
{
    int32_t time_ms;
	
#ifdef WIN32
    struct _timeb timebuffer;
	
    _ftime( &timebuffer );
    time_ms = (int32_t)timebuffer.time * 1000 + (int32_t)timebuffer.millitm;
#else
    struct timeval t1;
    struct timezone tz;
	
    gettimeofday(&t1, &tz);
    time_ms = (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
#endif
	
    return time_ms;
}

//-------------------------------------------------------------------
// changes the screen output color
// (you have to reset it to "black" manually afterwards)
void col(const char *str)
{
	static int color;
	
	// set color or b/w mode
	if (strstr(str, "color")!=NULL) color=1;
	if (strstr(str, "nocol")!=NULL) color=0;
	
	if (color) {
		if (strstr(str, "black")!=NULL) printf("\033[22;30m");
		if (strstr(str, "red")!=NULL) printf("\033[22;31m");
		if (strstr(str, "green")!=NULL) printf("\033[22;32m");
		if (strstr(str, "brown")!=NULL) printf("\033[22;33m");
		if (strstr(str, "blue")!=NULL) printf("\033[22;34m");
		if (strstr(str, "magenta")!=NULL) printf("\033[22;35m");
		if (strstr(str, "cyan")!=NULL) printf("\033[22;36m");
		if (strstr(str, "gray")!=NULL) printf("\033[22;37m");
		if (strstr(str, "dark gray")!=NULL) printf("\033[01;30m");
		if (strstr(str, "light red")!=NULL) printf("\033[01;31m");
		if (strstr(str, "light green")!=NULL) printf("\033[01;32m");
		if (strstr(str, "yellow")!=NULL) printf("\033[01;33m");
		if (strstr(str, "light blue")!=NULL) printf("\033[01;34m");
		if (strstr(str, "light magenta")!=NULL) printf("\033[01;35m");
		if (strstr(str, "light cyan")!=NULL) printf("\033[01;36m");
		if (strstr(str, "white")!=NULL) printf("\033[01;37m");
	}
}

//-------------------------------------------------------------------
// print something in logfile (without knowing the logfile handler)
int control_log(FILE *file, int mode, const char *s)
{
	static FILE *lf;
	
	if (mode==0) { // initialize function; store file handler
		lf=file;
	} else {	 // write something to logfile
		control_printlog(lf,1,s);
	}
	
	return 0;
}

//-------------------------------------------------------------------
// Print Error Message on Screen
int errormsg(int flag, const char *s)
{
	col("red");
	printf("%s\n",s);
	col("black");
	
	// write to logfile
	if (flag) {
		FILE *temp;
		control_log(temp,1,s);
	}
	
	return 0;
}

//-------------------------------------------------------------------
// Print the Start Screen
int control_printtitle(const char *SwRelease)
{
	col("blue");
	printf("\n");
	printf("*****************************************************************\n");
	printf("                   multiDAX  --  DAQ for neriX                   \n");
	printf("                   version: %s                             \n",SwRelease);
	printf("*****************************************************************\n");
	col("black");
	return 0;
}

//-------------------------------------------------------------------
// display help menu for oscilloscope mode
int control_scopehelp()
{
	col("green"); printf("Quick Reference for Oscilloscope Mode:\n"); col("black");
	printf("  In oscilloscope mode (-g or -m), you can use the following keys\n");
	printf("  in order to change the display during run time:\n\n");
	printf("  <+>  switch to next PMT/channel\n");
	printf("  <->  switch to previous PMT/channel\n");
	printf("  <s>  store current waveform to root-file\n\n");
	printf("  <0>  standard display mode for y-axis\n");
	printf("  <1>  manual display mode for y-axis\n");
	printf("  <2>  automatic display mode for y-axis\n");
	printf("  <3>  standard display mode for x-axis\n");
	printf("  <4>  manual display mode for x-axis\n");
	printf("  <5>  automatic display mode for x-axis\n\n");
	printf("  in manual display mode only:\n");
	printf("  <u/U> 'up':    increase lower/upper y-display limit\n");
	printf("  <d/D> 'down':  decrease lower/upper y-display limit\n");
	printf("  <r/R> 'right': increase lower/upper x-display limit\n");
	printf("  <l/L> 'left':  decrease lower/upper x-display limit\n\n");
	printf("  <b>  calculate baseline characteristics\n");
	printf("  <o>  show this help menu\n");
	return 0;
}

//-------------------------------------------------------------------
// returns a string with current date and time
int control_gettimestring(char *timestr, const char format)
{
	time_t curtime;
	struct tm *loctime;
	struct tm *utc;
	int ret=0;
	char tmpstr[35];
	
	// v1.1.6: all time stamps in seconds in UTC;
	//         all detailed times in local time
	
	curtime = time (NULL);		// Get the current time.
	utc = gmtime(&curtime);			// convert to UTC
	if (format=='v') sprintf(tmpstr,"%s",asctime(utc));
	loctime = localtime (&curtime);	// Convert it to local time representation.
	if (format=='f')
		strftime(timestr, 100, "%y%m%d_%H%M", loctime);	// formatted print for filenames
	if (format=='l')
		strftime(timestr, 100, "%a %b %d %Y, %X", loctime);	// long format for logfile
	if (format=='s')
		strftime(timestr, 100, "%X", loctime);	// short format for logfile
	if (format=='v')
		sprintf(timestr,"\nLocal Time: %sUTC: %s",
				asctime(loctime),tmpstr);
	if (format=='u')
		ret=timegm(gmtime(&curtime));			// Unix Time Format in UTC
	
	srand(curtime);			// random generator seed
	return ret;
}

//-------------------------------------------------------------------
// generates logfile
int control_openlog(FILE **logfile, char *FileName, const char *SwRelease)
{
	char LogFileName[100];
	char timestr[100];
	
	sprintf(LogFileName,"%s.log",FileName);
	*logfile=fopen(LogFileName,"w");
	if (*logfile==NULL) {
		errormsg(0,":::: ERROR: cannot open log file! ::::\n");
		return -1;
	}
	
	control_gettimestring(timestr,'v');
	fprintf(*logfile,"**Logfile********************************************************\n");
	fprintf(*logfile,"                 multiDAX  --  DAQ for neriX                   \n");
	fprintf(*logfile,"                    version: %s                             \n",SwRelease);
	fprintf(*logfile,"*****************************************************************\n");
	fprintf(*logfile,"program started: %s\n",timestr);
	return 0;
}

//-------------------------------------------------------------------
// print to logfile
int control_printlog(FILE *logfile, int time, const char *text)
{
	char timestr[100]="";
	
	if (time) {
		control_gettimestring(timestr,'s');
		fprintf(logfile,"%s: %s\n",timestr,text);
	} else fprintf(logfile,"%s\n",text);
	
	return 0;
}

//-------------------------------------------------------------------
// printf counter values to logfile
int control_cnts2log(FILE *logfile, scaler& s)
{
	char str[100];
	float delta=(s.stop-s.old)/1000.;
	int now;
	
	sprintf(str,"Scaler Values: (Delta = %.3f s)",delta);
	control_printlog(logfile, 1, str);
	
	now=control_gettimestring(str, 'u');
	
	fprintf(logfile, "CNT0 %d %.3f ",now, delta);
	for (int i=0; i<8; i++) fprintf(logfile,"%9u ", s.cnt[i]); fprintf(logfile,"\n");
	fprintf(logfile, "CNT1 %d %.3f ",now, delta);
	for (int i=8; i<16; i++) fprintf(logfile,"%9u ", s.cnt[i]); fprintf(logfile,"\n");
	fprintf(logfile, "CNT2 %d %.3f ",now, delta);
	for (int i=16; i<24; i++) fprintf(logfile,"%9u ", s.cnt[i]); fprintf(logfile,"\n");
	fprintf(logfile, "CNT3 %d %.3f ",now, delta);
	for (int i=24; i<32; i++) fprintf(logfile,"%9u ", s.cnt[i]); fprintf(logfile,"\n");
	
	// timing with external counter
	s.totaltime+=s.cnt[s.totalcnt]/1.e6;
	s.lifetime+=s.cnt[s.lifecnt]/1.e6;
	s.deadtime+=s.cnt[s.deadcnt]/1.e6;
	
	return 0;
}

//-------------------------------------------------------------------
// Process command line options
int control_cmdline( int argc, char *argv[], char *envp[], int& time, u_int32_t& NoE,
					var_graphics& graphics, int& baseline, digitizer& adc,
					char *OutFileName, char *XmlFileName)
{
	extern char *optarg;          /* options processing */
	int c;                        /* dummy for options */
	int errflag=0;                /* error flag */
	int someArgs=0;
	double x;
	
	// initialize variables
	time=0;
	NoE=0;
	graphics.on=0;
	baseline=0;
	adc.BoardInfo=0;
	col("color");
	
	//  search for possible options
	while ( ( c = getopt( argc, argv, "t:n:f:g:m:x:ibahowzc" ) ) != -1 )  {
		someArgs=1;
		switch ( c ) {
			case 'f':
				sprintf(OutFileName,"%s",optarg);
				break;
			case 'x':
				sprintf(XmlFileName,"%s",optarg);
				break;
			case 't':
				time = atoi( optarg );
				if ( time <= 0 ) { errormsg(0,"Measuring Time must be greater than 0"); errflag++; }
				break;
			case 'n':
				NoE = atoi( optarg );
				if ( NoE <= 0 ) { errormsg(0,"Number of Events must be greater that 0"); errflag++; }
				break;
			case 'm':
				graphics.on = 1;
				x=atof(optarg);
				graphics.module=(int)x;
				for (int i=0; i<9; i++)
					if ((x-graphics.module)*10>(i-0.1) && (x-graphics.module*10)<(i+0.1)) graphics.channel=i;
				if ( graphics.module < 0 ) { errormsg(0,":::: ERROR: Board identifier must be positive ::::"); errflag++;  }
				graphics.pmtNb=-1;
				break;
			case 'g':
				graphics.on = 1;
				graphics.pmtNb=atoi(optarg);
				if ( graphics.pmtNb < 1 ) { errormsg(0,":::: ERROR: PMT number must be positive ::::"); errflag++;  }
				break;
			case 'i':
				adc.BoardInfo = 1;
				break;
			case 'b':
				baseline = 1;		// baseline adjustment
				break;
			case 'a':
				baseline = 2;		// baseline analysis
				break;
			case 'z':
				baseline = 3;		// noise analysis
				break;
			case 'c':			// disable color mode
				col("nocol");
				break;
			case 'h':
				errflag++;
				break;
			case 'w':
				// don't take this too serious (-:
				errormsg(0,"missed WIMPs protocol:");
				int i;
				char txt[100];
				control_gettimestring(txt,'f');
				i = (int)((float)rand()/2147483647.*10.);
				printf("  You missed %d WIMPs since stopping DAQ.\n  Better you start it again...\n",i);
				exit(0);
				break;
			case 'o':
				control_scopehelp();
				exit(0);
			default:
				errflag++;
				break;
        }
	}
	if (errflag || someArgs==0) {
		printf("usage: %s [-f file|-x file|-t time|-n evts|-g p|-m m.c|-i|-b|-a|-h]\n", argv[0] );
		printf("\t-f write to file_*.xen \n" );
		printf("\t-x read settings from XML-file file\n");
		printf("\t-t measure for time seconds\n" );
		printf("\t-n measure evts events\n" );
		printf("\t-g Oscilloscope: display PMT p (SLOW!)\n");
		printf("\t-m Oscilloscope: display ADC module.channel (SLOW!)\n" );
		printf("\t-i displays hardware information\n" );
		printf("\t-b adjust baselines automatically\n" );
		printf("\t-a analyze baseline (mean, RMS, spikes)\n");
		printf("\t-z perform noise analysis\n");
		printf("\t-o show help for oscilloscope mode\n");
		printf("\t-c disable color output\n");
		printf("\t-h display this help\n\n" );
		exit( 2 );
	}
	
	if (graphics.on) errormsg(0,":::: WARNING: Graphics output is slow! ::::\n");
	
	// wait to display program title
	sleep(1);
	return 0;
}

//-------------------------------------------------------------------
// Integer form of power function
int power(int base, int exponent)
{
	int result=1;
	for (int i=0; i<exponent; i++) result=result*base;
	return result;
}


//-------------------------------------------------------------------
// Read configuration file and set up VME connection
// returns -1 on error
int control_configfile(	char *ConfigFileName, crate& crt,
					   digitizer& adc, scaler& s,
					   multipurposemodule& mp, int& WriteToFile)
{
	FILE *f_ini;
	char str[100];
	int link, bdnum;
	u_int32_t addr, dummy;
	u_int32_t data;
	char tmp[100];
	CVBoardTypes BType;
	int board;
	int32_t temp_long;
	int i;
	int pmtNb, module, channel;
	
	if ( (f_ini = fopen(ConfigFileName, "r")) == NULL ) {
		sprintf(str,":::: Can't open Configuration File %s ::::", ConfigFileName);
		errormsg(0,str);
		return -3;
	}
	else printf("Reading Configuration File %s\n", ConfigFileName);
	
	while(!feof(f_ini)) {
		fscanf(f_ini, "%s", str);
		if (str[0] == '#')
			fgets(str, 1000, f_ini);
		else
		{
			// LINK: Open VME master
			if (strstr(str, "LINK")!=NULL) {
				fscanf(f_ini, "%s", tmp);
				if (strstr(tmp, "V1718")!=NULL)
					BType = cvV1718;
				else if (strstr(tmp, "V2718")!=NULL)
					BType = cvV2718;
				else {
					errormsg(0,":::: ERROR: Invalid VME crate controller type ::::");
					return -1;
				}
				fscanf(f_ini, "%d", &link);
				fscanf(f_ini, "%d", &bdnum);
				if (CAENVME_Init(BType, link, bdnum, &temp_long) != cvSuccess) {
					errormsg(0,":::: ERROR: VME connection cannot be established ::::");
					return -1;
				}
				else {
					crt.handle[crt.NumberOfCrates]=temp_long;
					crt.NumberOfCrates++;
					if (crt.NumberOfCrates>CRATE_MAX) {
						errormsg(0,":::: ERROR: too many crates ::::\n");
						return -1;
					}
				}
			}
			
			// Write Data to output Files (0=Don't write, 1=binary, 2=text file, 3=Xe100)
			if (strstr(str, "WRITE_TO_FILE")!=NULL) {
				fscanf(f_ini, "%d", &WriteToFile);
				if (WriteToFile<0 || WriteToFile>3) {
					WriteToFile=3;
					errormsg(0,":::: WARNING: invalid WRITE_TO_FILE option; use XENON100 format :::");
				}
			}
			
			// Readout Blt Size (in Bytes)
			if (strstr(str, "BLT_SIZE")!=NULL)
				fscanf(f_ini, "%d", &adc.BltSize);
			
			// Base Addresses
			if (strstr(str, "BASE_ADDRESS")!=NULL) {
                fscanf(f_ini, "%x", (int *)&dummy);
				if (adc.NumberOfADCs==0) {
					adc.Address[0]=dummy;
					fscanf(f_ini, "%d", &adc.Id[0]);
				} else {
					for (int i=0; i<adc.NumberOfADCs; i++)
						if (adc.Address[i]==dummy) {
							errormsg(0,":::: ERROR: two identical BaseAddresses given in Config-file! ::::");
							return -1;
						}
					adc.Address[adc.NumberOfADCs]=dummy;
					fscanf(f_ini, "%d", &adc.Id[adc.NumberOfADCs]);
				}
				fscanf(f_ini, "%d", &i);
				if (i>crt.NumberOfCrates-1) {
					sprintf(str, ":::: ERROR: Crate %d (board %3d) does not exist! ::::",i,adc.Id[adc.NumberOfADCs]);
					errormsg(0,str);
					return -1;
				} else adc.Crate[adc.NumberOfADCs]=i;
				if (adc.BoardInfo) printf("  Module %3d (%d): BaseAdress[%2d] = 0x%08X\n",
										  adc.Id[adc.NumberOfADCs],adc.Crate[adc.NumberOfADCs],
										  adc.NumberOfADCs,adc.Address[adc.NumberOfADCs]);
				
				adc.NumberOfADCs++;
				if (adc.NumberOfADCs>ADC_MAX) {
					errormsg(0,":::: ERROR: too many ADC modules ::::");
					return -1;
				}
			}
			
			// ADC Trigger Latency
			if (strstr(str,"TRIG_LATENCY")!=NULL) {
				fscanf(f_ini, "%d", &adc.TriggerLatency);
			}
			
			// Generic VME Write
			// this is written to all of the boards
			if (strstr(str, "WRITE_REGISTER")!=NULL) {
				fscanf(f_ini, "%x", (int *)&addr);
				fscanf(f_ini, "%x", (int *)&data);
				for (int i=0; i<adc.NumberOfADCs; i++) {
					if (adc_writereg(crt,adc,i,addr,data)<0) return -1;
				}
			}
			
			// generic VME Write to individual module
			// this is only written to the module specified
			// usage: WRITE_MODULE <module> <address> <data>
			if (strstr(str, "WRITE_MODULE")!=NULL) {
				fscanf(f_ini, "%d", &board);
				
				// get the BaseAddress from the board identifier
				int i=0;
				while (i<adc.NumberOfADCs+1) {
					if (board==adc.Id[i]) break;
					i++;
				}
                fscanf(f_ini, "%x", (int *)&addr);
                fscanf(f_ini, "%x", (int *)&data);
				if (i<adc.NumberOfADCs) {
					if (adc_writereg(crt,adc,i,addr,data)<0) return -1;
                } else {
					if ((addr&0x00000F00)==0) {
						sprintf(str, ":::: WARNING: unknown board identifier %d ::::",board);
						errormsg(0,str);
					}
				}
				if (addr==0xEF34) {		// stop program after Configuration ROM update
					sprintf(str, ":::: Configuration ROM updated, board %d ::::",board);
					errormsg(0,str);
					return -2;
				}
			}
			
			// read PMT to channel mapping
			if (strstr(str, "PMT")!=NULL) {
				fscanf(f_ini, "%d", &pmtNb);
				fscanf(f_ini, "%d", &module);
				fscanf(f_ini, "%d", &channel);
				if (module<0 || module>ADC_MAX-1 || channel<0 || channel>7) {
					sprintf(str, ":::: WARNING: error in PMT mapping (PMT %d %d %d) ::::",pmtNb,module,channel);
					errormsg(0,str);
				}
				else {
					if (adc.pmt[module][channel]<0) {
						adc.pmt[module][channel]=pmtNb;
						if (pmtNb>adc.pmtmax) adc.pmtmax=pmtNb;
						// generate channel mask
						adc.mask[module]+=power(2,channel);
					} else {
						sprintf(str, ":::: WARNING: channel %d.%d already occupied with PMT %d ::::",
								module,channel,adc.pmt[module][channel]);
						errormsg(0,str);
					}
				}
				sprintf(str,"dummy string");
			}
		}
	}
	fclose (f_ini);
	
	return 0;
}


//-------------------------------------------------------------------
// pre-defined configurations for automatic baseline determination
// returns -1 on error
int control_configbaseline(crate& crt, digitizer& adc)
{
	for (int i=0; i<adc.NumberOfADCs; i++) {
		// digitize everything, no ZLE
		if (adc_writereg(crt,adc,i,ChannelConfigReg,0x50)<0) return -1;
        
		// shorten the time window to 0x6 = 8192 Samples
		if (adc_writereg(crt,adc,i,BlockOrganizationReg,0x6)<0) return -1;
	}
	return 0;
}


//-------------------------------------------------------------------
// open file to store data
int control_openfile(FILE **ofile, char *FileName, int& FileCounter, int WriteToFile,
					 digitizer& adc, FILE *logfile)
{
	char OutFileName[100];
	char text[120];
	u_int32_t header[5];
	
	sprintf(OutFileName,"%s_%05d.xen",FileName,FileCounter);
	*ofile=fopen(OutFileName,"w");
	if (*ofile==NULL) {
		errormsg(1,":::: ERROR: cannot open data file! ::::");
		return -1;
	}
	
	// write a header to the file
	header[0]=3; 			// length of header (including this word)
	header[1]=adc.NumberOfADCs;	// Number of ADCs read
	if (adc.EventLength) header[2]=adc.EventLength;
	else  header[2]=adc.NbSamplesEvt;	// maximal length of Event
	if (adc.ZLE==1) header[2]+=0x80000000;	// ZLE activated
	if (WriteToFile==1) fwrite(header, sizeof(u_int32_t), header[0], *ofile);
	if (WriteToFile==2) for (int i=0; i<(int)header[0]; i++) fprintf(*ofile,"%d\n",header[i]);
	
	printf("Write to file: %s\n",OutFileName);
	sprintf(text,"data file: %s",OutFileName);
	control_printlog(logfile,1,text);
	
	return 0;
}

//-------------------------------------------------------------------
// open xdio-file to store data without TDC (XENON100 format)
int control_openxdio(xdio_file_t **ofile, xdio_chunk_att_t **xatt,
					 char *FileName, int& FileCounter, int WriteToFile,
					 digitizer& adc,
					 FILE *logfile, int EventsPerFile, timeandtrigger& t)
{
	char OutFileName[100];
	char text[120];
	time_t curtime;
	int window;
	
	curtime = time (NULL);
	
	sprintf(OutFileName,"%s_%06d.xed",FileName,FileCounter);
	*ofile = xdio_open_write(OutFileName, curtime, t.tottrg, EventsPerFile, 1);
	if (*ofile==NULL) {
		errormsg(1,":::: ERROR: cannot open data file! ::::");
		return -1;
	}
	
	int compress = 0;
	switch (adc.compression) {
		case 0: compress=0; break;
		case 1:
			if (adc.ZLE) compress=XDIO_F_ZLE0_GZIP;
			else compress=XDIO_F_RAW0_GZIP;
			break;
		case 2:
			if (adc.ZLE) compress=XDIO_F_ZLE0_BZIP2;
			else compress=XDIO_F_RAW0_BZIP2;
			break;
	}
	
	if (adc.EventLength) window=adc.EventLength;	// custom sized window
	else window=adc.NbSamplesEvt;	// standard CAEN window
	
	if (adc.ZLE) *xatt = xdio_create_chunk_zle0_attributes(adc.sample_size, compress, window,
														   adc.voltage_range, adc.sampling_freq, adc.NbChannels);
	else *xatt = xdio_create_chunk_raw0_attributes(adc.sample_size, compress, window,
												   adc.voltage_range, adc.sampling_freq, adc.NbChannels);
	
	printf("Write to file: %s\n",OutFileName);
	sprintf(text,"data file: %s",OutFileName);
	control_printlog(logfile,1,text);
	
	return 0;
}


//-------------------------------------------------------------------
// open xdio-file to store data with TDC (XENON100 format)
int control_openxdio_tdc(xdio_file_t **ofile, xdio_chunk_att_t **xatt,
					 xdio_chunk_att_t **xatt_tdc,
					 char *FileName, int& FileCounter, int WriteToFile,
					 digitizer& adc, timedc& tdc,
					 FILE *logfile, int EventsPerFile, timeandtrigger& t)
{
	char OutFileName[100];
	char text[120];
	time_t curtime;
	int window;
	
	curtime = time (NULL);
	
	sprintf(OutFileName,"%s_%06d.xed",FileName,FileCounter);
	*ofile = xdio_open_write(OutFileName, curtime, t.tottrg, EventsPerFile, 1);
	if (*ofile==NULL) {
		errormsg(1,":::: ERROR: cannot open data file! ::::");
		return -1;
	}
	
	int compress = 0;
	switch (adc.compression) {
		case 0: compress=0; break;
		case 1:
			if (adc.ZLE) compress=XDIO_F_ZLE0_GZIP;
			else compress=XDIO_F_RAW0_GZIP;
			break;
		case 2:
			if (adc.ZLE) compress=XDIO_F_ZLE0_BZIP2;
			else compress=XDIO_F_RAW0_BZIP2;
			break;
	}
	
	if (adc.EventLength) window=adc.EventLength;	// custom sized window
	else window=adc.NbSamplesEvt;	// standard CAEN window
	
	if (adc.ZLE) *xatt = xdio_create_chunk_zle0_attributes(adc.sample_size, compress, window,
														   adc.voltage_range, adc.sampling_freq, adc.NbChannels);
	else *xatt = xdio_create_chunk_raw0_attributes(adc.sample_size, compress, window,
												   adc.voltage_range, adc.sampling_freq, adc.NbChannels);
				
	*xatt_tdc = xdio_create_chunk_raw0_attributes(tdc.sample_size,
														0, // compression  = 0
														tdc.NbSamplesEvt, // window size of 1 (1 sample)
													    tdc.time_range, // max time of tdc (Full Scale Range)
													    tdc.sampling_freq,
													    tdc.NbChannels);
	
	printf("Write to file: %s\n",OutFileName);
	sprintf(text,"data file: %s",OutFileName);
	control_printlog(logfile,1,text);
	
	return 0;
}

//-------------------------------------------------------------------
// generate file with basic information to be read by Slow Control
int control_scfile(int time, float average, int stopped)
{
	FILE *scfile;
	char txt[100];
	int now;
	
	scfile=fopen(SC_FILENAME,"w");
	if (scfile!=NULL) {
		if (stopped) {
			fprintf(scfile,"stopped\n");
		} else {
			now=control_gettimestring(txt,'u');
			fprintf(scfile,"%d\n",now);
			fprintf(scfile,"AveragingTime %d\n",time);
			fprintf(scfile,"Average %f\n",average);
		}
		fclose(scfile);
	}
	else
		errormsg(1,":::: WARNING: Cannot create file to pass information to Slow Control! ::::");
	
	return 0;
}


//-------------------------------------------------------------------
// calculate and print count and trigger rates
int control_calcrate(timeandtrigger& t, scaler &s, int FileEvtCounter)
{
	int32_t ElapsedTime;
	float TPrate, TRGrate, av10;
	static int FileECOld;
	static int nodata;
	static int display;
	static float Rate, Trgs;
	
	t.CurrentTime = get_time(); // Time in milliseconds
	ElapsedTime = t.CurrentTime - t.PreviousTime + t.msTime; // take care of remaining ms
	
	if (ElapsedTime > 1000) {
		t.PreviousTime = t.CurrentTime;
		t.MeasSeconds++;
		s.timecnt++;
		t.msTime=ElapsedTime-1000;
		while (t.msTime>1000) {
			t.msTime=t.msTime-1000;
			t.MeasSeconds++;
		}
		
		// init static variables when the function is called first
		if (t.MeasSeconds==1) {
			FileECOld=-1;
			nodata=0;
			display=0;
			Rate=0.;
			Trgs=0.;
		}
		display++;
	    
		TPrate = ((float)(t.totnb - t.last_totnb) / ElapsedTime)*1000.0;     // Bytes/second
		TRGrate = ((float)(t.tottrg - t.last_tottrg) / ElapsedTime)*1000.0;  // Triggers/second
		s.events+=(t.tottrg - t.last_tottrg);				 // Nb of Events
		Rate+=TPrate;
		Trgs+=TRGrate;
		
		if (display >= t.DisplayTime) {
			// calculate average count rate
			for (int i=9; i>0; i--) t.store10[i]=t.store10[i-1];
			t.store10[0]=t.tottrg - t.last_ave;
			av10=0;
			for (int i=0; i<10; i++) av10+=t.store10[i];
			av10=av10/(t.DisplayTime*10.);
			
			printf("%6ld MB @ %.4fMB/s  DAQ = %.1f Hz  %ds Ave = %.1f Hz  Evts = %d\n",
				   t.totnb/1048576, Rate/display/1048576, Trgs/display, t.DisplayTime*10, av10, t.tottrg);
			//      printf("%6ld MB @ %.4fMB/s  DAQ = %.1f Hz  %ds Ave = %.1f Hz  Evts = %d\n",
			//                t.totnb/1048576, Rate/display/1048576, Trgs/display, t.DisplayTime*10, av10, t.tottrg);
			
			control_scfile(t.DisplayTime*10,av10,0);
			
			display=0;
			t.last_ave=t.tottrg;
			Rate=0.;
			Trgs=0.;
		}
		
		// check if events are recorded
		if (FileEvtCounter==FileECOld) nodata++;
		else {
			FileECOld=FileEvtCounter;
			nodata=0;
		}
		// exit if no counts for NO_DATA_TIME seconds
		if (nodata==NO_DATA_TIME) {
			errormsg(1,":::: ERROR: timeout; no trigger seen for too long time ::::");
			return -1;
		}
		
		t.last_totnb = t.totnb;
		t.last_tottrg = t.tottrg;
	}
	return nodata;
}

//-------------------------------------------------------------------
// writes data to file
// returns the number of new triggers on success
// returns -1 if failure
int control_writedata(	digitizer& adc, int i, FILE *ofile,
					  int WriteToFile, int blt_bytes,	u_int32_t *buff)
{
	int reduce;
	int retval=0;
	int32_t pnt;
    
	if (WriteToFile==1 && buff[0] != FILLER && blt_bytes>0) { // write binary data
		
		// check if last read word is not valid
		if (buff[blt_bytes/4-1]==FILLER) reduce=1;
		else reduce=0;
		
		// Counters for Event/Trigger
		if ((buff[0]>>20)==0xA00 && (buff[1]>>28)==0x0 && i==0 && blt_bytes>0) {
			retval++;
		}
	    
		// code board identifier in ChannelMask Word
		buff[1]=buff[1]&0xF0FFFFFF;
		buff[1]=buff[1]+(adc.Id[i]*0x100000);
		
		fwrite(buff, sizeof(u_int32_t), blt_bytes/sizeof(u_int32_t)-reduce, ofile);
	}
	else if (WriteToFile==2) { // just write plain data stream (text)
		pnt = 0;
		while ((pnt < (blt_bytes/4)) && (buff[pnt] != FILLER)) {
			
			if ((buff[pnt]>>20)==0xA00 && (buff[pnt+1]>>28)==0x0) { // TTT can also start with 0xA
				if (i==0) { // count only triggers on one board
					retval++;
				}
				if (WriteToFile==2) fprintf(ofile,"#%d#",adc.Id[i]);
			}
			
			// fprintf(ofile,"%5d\n%5d\n", (buff[pnt]>>16)&0xFFFF, buff[pnt]&0xFFFF);
			fprintf(ofile,"%08X\n",buff[pnt]);
			pnt++;
		} // end: Analyze-Data while-loop
	}
	else { // store nothing but record triggers
		if ((buff[0]>>20)==0xA00 && (buff[1]>>28)==0x0 && i==0 && blt_bytes>0) retval++;
	} // end: if (WriteToFile==1)
	
	return retval;
}


//-------------------------------------------------------------------
// writes data to file: XENON100 file format without TDC
// returns the number of new triggers on success
// returns -1 if failure
int control_writexdio(	digitizer& adc,		// adc info
					  timeandtrigger& t,		// time information
					  int i,			// current module
					  xdio_file_t *ofile, 	// file handle
					  xdio_chunk_att_t *xatt, // attribute list
					  int WriteToFile,	// binary or ASCII data?
					  int blt_bytes,		// number of bytes read
					  u_int32_t *buff)	// data to write
{
	static xdio_event_t *xevent;
	static xdio_chunk_t *xchunk;
	int retval=0;
	// DEBUG
	static int stored=1;
	static u_int32_t EventCounter;
	// DEBUG
	static int32_t oldSecs=0;
	
	// this has to be done only once per event
	if (i==0) {
		if (stored!=1) errormsg(1,":::: WARNING: old event not stored yet ::::");
		stored=0;
		
		// create an event
		xevent = xdio_create_event();
		
		// store time of event: use computer time and trigger time tag of event
		if ((buff[3]&0x7FFFFFFF)<t.lastTTT) t.reps++;	// account for finite lenght of TTT counter
		
		double fracpart, intpart;
		fracpart = modf (t.reps*21.47483648 + (buff[3]&0x7FFFFFFF)/1.e8 , &intpart);
		int32_t secs = t.StartTime + (int)(intpart);
		if (intpart<0) printf("intpart = %d\n",intpart);
		int32_t usecs = (int)(fracpart*1.e6);
		xdio_set_trigger_time(secs, usecs, xevent);
		t.lastTTT=buff[3]&0x7FFFFFFF;
		if (secs<oldSecs) printf("---- %d %d\n",oldSecs,secs);
		oldSecs=secs;
		EventCounter = buff[2];
		
		// create chunk
		if (adc.ZLE) xchunk = xdio_create_chunk("zle0", xatt);
	    else xchunk = xdio_create_chunk("raw0", xatt);
		
		// Counters for Event/Trigger
		//    if ((buff[0]>>20)==0xA00 && (buff[1]>>28)==0x0 && blt_bytes>0) retval++;
		//    else errormsg(1,":::: WARNING: found invalid event signature ::::");
	}
	
	/*  if (buff[2]!=EventCounter) {
	 errormsg(1,":::: WARNING: Event Counters do not coincide! ::::");
	 char errstr[100];
	 sprintf(errstr,":::: mod: %d  EvtShould: %d  EvtIs:: %d::::",i,EventCounter,buff[2]);
	 errormsg(1,errstr);
	 }
	 */
	
	if (blt_bytes==0) {
		errormsg(1,":::: WARNING: No data read ::::");
		buff[0]=0;
	}
	
	// write data to chunk
	xdio_attach_multi_data_src(	adc.pmt[i][0]-1, 		// PMT number - 1
							   adc.mask[i] & (buff[1]&0xFF),	// channels to read
							   (int8_t *)(buff+4), 		// pointer to data (w/o header)
							   ((buff[0]&0xFFFFFFF)*4)-16, 	// size of data (w/o header)
							   xchunk);			// chunk handle
	
	// add chunk to event
	if (i==adc.NumberOfADCs-1) {
		
		// Counters for Event/Trigger -- moved to here from i==0 part
		if ((buff[0]>>20)==0xA00 && (buff[1]>>28)==0x0 && blt_bytes>0) retval++;
		else {
			errormsg(1,":::: WARNING: found invalid event signature ::::");
			printf("%08Lx %08Lx\n",buff[0],buff[1]);
		}
		
		// add the chunk to the event
		xdio_add_chunk(xchunk, xevent);
		
		// add the event to the file
		xdio_add_event(xevent, ofile);
		
		// DEBUG
		stored=1;
	}
	
	return retval;
}



//-------------------------------------------------------------------
// writes data to file: XENON100 file format with TDC
// returns the number of new triggers on success
// returns -1 if failure
int control_writexdio_tdc(	digitizer& adc,		// adc info
						timedc& tdc,		// tdc info
					  timeandtrigger& t,		// time information
					  int i,			// current module
					  xdio_file_t *ofile, 	// file handle
					  xdio_chunk_att_t *xatt, // attribute list
					  xdio_chunk_att_t *xatt_tdc, // tdc attricute list
					  int WriteToFile,	// binary or ASCII data?
					  int blt_bytes,		// number of bytes read
					  u_int32_t *buff,
					  u_int32_t *buff_tdc)	// data to write
{
	static xdio_event_t *xevent;
	static xdio_chunk_t *xchunk;
	static xdio_chunk_t *xchunk_tdc;
	int retval=0;
	// DEBUG
	static int stored=1;
	static u_int32_t EventCounter;
	// DEBUG
	static int32_t oldSecs=0;
	
	// this has to be done only once per event
	if (i==0) {
		if (stored!=1) errormsg(1,":::: WARNING: old event not stored yet ::::");
		stored=0;
		
		// create an event
		xevent = xdio_create_event();
		
		// store time of event: use computer time and trigger time tag of event
		if ((buff[3]&0x7FFFFFFF)<t.lastTTT) t.reps++;	// account for finite lenght of TTT counter
		
		double fracpart, intpart;
		fracpart = modf (t.reps*21.47483648 + (buff[3]&0x7FFFFFFF)/1.e8 , &intpart);
		int32_t secs = t.StartTime + (int)(intpart);
		if (intpart<0) printf("intpart = %d\n",intpart);
		int32_t usecs = (int)(fracpart*1.e6);
		xdio_set_trigger_time(secs, usecs, xevent);
		t.lastTTT=buff[3]&0x7FFFFFFF;
		if (secs<oldSecs) printf("---- %d %d\n",oldSecs,secs);
		oldSecs=secs;
		EventCounter = buff[2];
		
		// create chunk
		if (adc.ZLE) xchunk = xdio_create_chunk("zle0", xatt);
	    else xchunk = xdio_create_chunk("raw0", xatt);
		
		// Counters for Event/Trigger
		//    if ((buff[0]>>20)==0xA00 && (buff[1]>>28)==0x0 && blt_bytes>0) retval++;
		//    else errormsg(1,":::: WARNING: found invalid event signature ::::");
	}
	
	/*  if (buff[2]!=EventCounter) {
	 errormsg(1,":::: WARNING: Event Counters do not coincide! ::::");
	 char errstr[100];
	 sprintf(errstr,":::: mod: %d  EvtShould: %d  EvtIs:: %d::::",i,EventCounter,buff[2]);
	 errormsg(1,errstr);
	 }
	 */
	
	if (blt_bytes==0) {
		errormsg(1,":::: WARNING: No data read ::::");
		buff[0]=0;
	}
	
	// write data to chunk
	xdio_attach_multi_data_src(	adc.pmt[i][0]-1, 		// PMT number - 1 to get channel							number - will be 24 for tdc
							   adc.mask[i] & (buff[1]&0xFF),	// channels to read
							   (int8_t *)(buff+4), 		// pointer to data first 32 bit word that isn't header
							   ((buff[0]&0xFFFFFFF)*4)-16, 	// size of data (w/o header) - keeps event size in header in words and multiplies it by 4 to get number of bytes and then subtract header size (16 bytes)
							   xchunk);			// chunk handle
	
	
	// collect on the last module only to avoid duplication
	// going to attach each channel individually by starting with 0x0 array
	// and then going through buffer word by word and taking that place in
	// the array with the actual data
	// need to make sure tdc.sample_size is reset back to 16 bits
	
	
	// add chunk to event
	if (i==adc.NumberOfADCs-1)
	{
		xchunk_tdc = xdio_create_chunk("raw0", xatt_tdc);
		u_int32_t data_tdc[tdc.NbChannels];
		
		for (int j = 0; j < tdc.NbChannels; j++)
		{
			data_tdc[j] = 0x0;
		}
		
		//printf("\n\nNew event\n");
		
		//for (int k = 0; k < tdc.BufferSize/32; k++) //check all of malloced memory
		for (int k = 0; k < tdc.BufferSize/4; k++) //check all of malloced memory
		{
			int32_t word = buff_tdc[k];
			//printf("\nBuffer location = %p\n", buff_tdc + k);
			//printf("\nword before = %lX\n", word);
			if(!word) break; // check if you are finished reading data
			//if((word & 0xFF000000 != 0xF8000000) && (word & 0xFF000000 != 0xF0000000)) continue; // skip this part of buffer if it is a header or end of block
			if((word & 0x7000 == 0x2000) && (word & 0x7000 == 0x4000)) continue;
			// bits 24-26 identify the type of data
			//if((word & 0xFF000000 == 0xFA000000) && (word & 0xFF000000 == 0xFC000000)) continue; // skip this part of buffer if it is a header or end of block
			
			//printf("\nchannel = %lX\n", word);
			
			// now we know that we only have data
			u_int32_t channelBinary = word & 0x00FF0000;
			int channel = 0;
			switch (channelBinary)
			{
			  case 0x000A0000:
				channel = 5;
				break;
			  case 0x00080000:
				channel = 4;
				//printf("Channel 4 data: %lX\n", word & 0xFFF);
				break;
			  case 0x00060000:
				channel = 3;
				break;
			  case 0x00040000:
				channel = 2;
				break;
			  case 0x00020000:
				channel = 1;
				break;
			  case 0x00000000:
				channel = 0;
				break;
			}

			// data defaults to 0x0 so this if statement
			// prevents overwriting good data
			if(word & 0xFFF)
			{
				data_tdc[channel] = word & 0xFFF;
			}
			//printf("\nword = %lX\n", word);
			//printf("\ndata_tdc for channel %d = %lX\n", channel, data_tdc[channel]);
		}
		
		//printf("\ndata_tdc ptr = %p\n", data_tdc);
		for (int channel = 0; channel < tdc.NbChannels; channel++)
		{
			// do not need to move pointer over two
			// manually because astro18 is little endian
			int8_t *startOfData = (int8_t *)(data_tdc + channel);// +2; //point to second half of 32 bit word
			//printf("startOfData ptr = %p\n", startOfData);
			//printf("Data for channel %d = %hX\n", channel, *((int16_t *)startOfData));
			xdio_attach_data_src(channel, (int8_t *)startOfData, 2, xchunk_tdc);
			// size of data is 2 bytes (16 bits)
		}
		
		// 141014 - checked the data with printf's and it looks good so now to xerawdp
	
	
	
		// --------- add event now that second chunk is created
		
		// Counters for Event/Trigger -- moved to here from i==0 part
		if ((buff[0]>>20)==0xA00 && (buff[1]>>28)==0x0 && blt_bytes>0) retval++;
		else {
			errormsg(1,":::: WARNING: found invalid event signature ::::");
			printf("%08Lx %08Lx\n",buff[0],buff[1]);
		}
		
		// add the chunks to the event
		xdio_add_chunk(xchunk, xevent);
		xdio_add_chunk(xchunk_tdc, xevent);
		
		// add the event to the file
		xdio_add_event(xevent, ofile);
		
		// DEBUG
		stored=1;
	}
	
	return retval;
}

//-------------------------------------------------------------------
// return trigger time from data
u_int32_t control_getTTT(u_int32_t *buff)
{
	return buff[3]&0x7FFFFFFF;
}

//-------------------------------------------------------------------
// generates a filename using path and the current dat/time
int control_generatefilename(char *OutFileName, char *path)
{
	char timestr[100];
	char cmnd[300];
	
	control_gettimestring(timestr,'f');
	
	sprintf(cmnd,"mkdir %snerix_%s",path,timestr);
	if (system(cmnd)!=0) errormsg(0,":::: WARNING: Could not create data directory ::::");
	
	//sprintf(OutFileName,"%s%s/%s_%s",path,timestr,FILE_ID,timestr);
	sprintf(OutFileName,"%snerix_%s/%s_%s",path,timestr,FILE_ID,timestr);
	return 0;
}

//-------------------------------------------------------------------
// calculate and print time in "good" format
int control_calctime(timeandtrigger& t, int32_t ms, char *text)
{
	int days=0;
	int hours;
	int minutes;
	int seconds;
	
	hours=(int)((t.MeasSeconds+ms/1000.)/3600.);
	minutes=(int)(((t.MeasSeconds+ms/1000.)-(hours*3600))/60.);  seconds=(int)(t.MeasSeconds+ms/1000.)-(hours*3600)-(minutes*60);
	days=(int)(hours/24);
	hours=hours-(days*24);
	
	//   sprintf(text,"Total Measuring Time = %d.%03ld s = %.2f h",
	//     t.MeasSeconds,ms,(t.MeasSeconds+ms/1000.)/3600.);
    
	if (days>0)
		sprintf(text,"Total Measuring Time = %d.%03d s = %02d days, %02d:%02d:%02d.%03d h",
				t.MeasSeconds,ms,days,hours,minutes,seconds,ms);
	else
		sprintf(text,"Total Measuring Time = %d.%03d s = %02d:%02d:%02d.%03d h",
				t.MeasSeconds,ms,hours,minutes,seconds,ms);
	
	return 0;
}

//-------------------------------------------------------------------
// set variables to default values
int control_setdefault(	digitizer& adc, scaler& s, timeandtrigger& t,
					   crate& crt, multipurposemodule& mp, var_graphics& g)
{
	// adcs
	adc.NumberOfADCs=0;
	adc.BltSize=256;
	adc.baseline=16000;
	adc.busy=1;
	adc.TriggerLatency=54;	// experimentally determined value
	adc.compression=0;		// no compression
	adc.EventLength=0;		// custom event size disabled
	
	// initialize PMT mapping
	for (int i=0; i<ADC_MAX; i++) {
		for (int j=0; j<8; j++) adc.pmt[i][j]=-1;
		adc.mask[i]=0x0;
	}
	adc.pmtmax=0;
	
	// crate
	crt.NumberOfCrates=0;
	
	// time and trigger
	t.DisplayTime=3;
	t.lastTTT=0;
	t.reps=0;
	t.msTime=0;
	
	// general purpose module
	mp.HEVeto=1;
	
	// scaler
	s.scalertime=60;		// read every x seconds
	s.totaltime=0.;		// for timing with ext. counter
	s.lifetime=0.;
	s.deadtime=0.;
	s.totalcnt=29;		// default values for counter channels
	s.lifecnt=30;
	s.deadcnt=31;
	s.cnt[14]=0;
	
	// graphics
	g.x_display=0;		// standard
	g.y_display=0;		// standard
	g.x_low=0;
	g.x_high=32767;
	g.y_low=0;
	g.y_high=16384;
	g.x_autorange=300;		// range for automatic x-display
	
	return 0;
}


//-------------------------------------------------------------------
// extracts module and channel of a given PMT number
int control_getadc(digitizer& adc, int pmt, int& module, int& channel)
{
	
	for (module=0; module<ADC_MAX; module++)
		for (channel=0; channel<8; channel++) {
			if (adc.pmt[module][channel]==pmt) return 0;
		}
	
	return -1;
}


