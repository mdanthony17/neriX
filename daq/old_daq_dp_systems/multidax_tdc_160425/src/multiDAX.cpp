/******************************************************************************
 
 multiDAX  -- DAQ for neriX
 
 *****************************************************************************
 Version 1.1.7m1
 30.07.2013	GP -- 	multiDAX version based on DAX 1.1.7
 *****************************************************************************
 DAX History:
 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 Version 0.1:
 29.11.2007 	MS -- 	modular version of DAQTest
 03.12.2007 	MS -- 	DAQTest fully modularized
 automatic baseline determination included
 separate ini-file for baselines
 04.12.2007	MS -- 	xml-file for user settings; program further modularized
 05.12.2007  MS --	basic log file included
 10.12.2007  MS --	Scaler included -- counts written to log-file
 Graphical output of a channel during DAQ possible
 11.12.2007  MS --   colorful output; start screen; errormsg included
 Graph. output now also supports non ZLE data
 14.12.2007  MS --	code ZLE status in 3rd header word
 11.01.2008  MS --	internal debugging option for V1724
 more settings possible via xml-File
 16.01.2008  MS --   setting of HE Veto via V1495 possible
 24.01.2008  MS --	scaler software trigger -> read all channels simultaneously
 08.02.2008  MS --   small changes and cosmetics
 10.03.2008  MS --   moving program to new DAQ machine (64bit)
 18.03.2008  MS --	errormsg printed to logfile
 19.03.2008  MS --	information file for slow control
 21.03.2008  MS -- 	xml-file: adc busy, error handling
 25.03.2008  MS --	disable analog sum output for external trigger (LED)
 26.03.2008  MS --	modifications for automatic baseline setting
 28.03.2008  MS --	increase performance of Oscilloscope Mode
 Version 1.0:
 28.03.2008  MS -- 	update to XENON100 file format
 02.04.2008  MS --   Osc. mode, cosmetics, bug fixes for Xe100 format
 03.04.2008	MS -- 	bug fixes in automatic baseline adjustment
 09.04.2008  MS --   bug fix in baseline adjustment; baseline analysis added
 12.04.2008  MS --   S1 trigger threshold setting via xml-file possible
 17.04.2008  MS --   trigger latency in post trigger setting
 21.04.2008  MS -- 	trigger latency can be adjusted in DAXConfig.ini
 05.05.2008  MS -- 	write to Conf Reload Register possible (via ini-file)
 20.05.2008  MS -- 	v1.0.1 set individual thresholds via xml-file
 10.06.2008  MS -- 	v1.0.2 print dead and lifetime to log-file
 13.06.2008  MS --   v1.0.3 Phillips in log File, changed Trg -> DAQ in output
 24.06.2008  MS --   v1.0.4 Store DAQ rate in log file; start xed-files with event 0 (was -1)
 27.06.2008  MS --   v1.0.5 catch ctrl-c
 01.07.2008  MS -- 	v1.0.6 write UNIX time and DeltaTime to CNT in logfile
 07.07.2008  MS --	v1.0.7 create directory for data automatically
 24.07.2008  MS --	v1.0.8 set discriminator via xml; debugging options for missing events
 26.08.2008  MS --	v1.0.9 timing bug in -b option fixed
 22.10.2008  MS --	v1.0.10 allows to change S2 trigger options
 Version 1.1:
 04.11.2008  MS --	v1.1.0 change readout/store structure to avoid incomplete events
 fixed some bugs in interrupt reading
 change naming convention for data path
 change online display (average, trigger rate)
 04.02.2009  MS --	v1.1.1 added a few test options for the analog output (not used!)
 30.06.2009	MS --	v1.1.2 runtime check of misaligned events on ADCs
 changed time in dax2sc to UNIX time
 07.07.2009 	MS --	v1.1.3 noise analysis included
 13.07.2009  MS --	v1.1.4 proper event time stamp (using computer time and TTT)
 fixed bug in n_pre/n_post assignment
 security question when data not stored in xe100 format
 option to disable color output
 gamma: time stamp included, compression possible
 delta: bugfix in time stamp
 epsilon: allow custom size events
 09.11.2009 	MS --	v1.1.5 allow longer xml-strings
 better control of measuring time
 16.11.2009: increase iterations for noise analysis
 15.03.2010	MS --	v1.1.6 fix time stamp issues (UTC instead of locTime)
 09.06.2010  MS --	v1.1.7 optimize identification of non-synchronization
 
 MS -- Marc Schumann, Rice University, marc.schumann@rice.edu
 *****************************************************************************
 based on:		DAQTest.cpp, version from 28.11.2007 by MS
 which is based on:	Wave Dump, v1.4 (17.09.2007) by C. Tintori (c.tintori@caen.it)
 ******************************************************************************/

extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
}

#include "global.h"	// define data structures
#include "keyb.h"	// getch() and kbhit() for Linux
#include "CAENVMElib.h"	// CAEN VME library
#include "control.h"	// functions to control the program
#include "adc.h"	// functions to deal with the CAEN fADCs
#include "bridge.h"	// functions to control the v2718 VME-PCI bridge
#include "xml.h"	// analyze the XML file with the user settings
#include "graph.h"	// functions for the graphics display
#include <libxdio.h>	// for Xe100 Data Storage
#include <signal.h>	// to catch for Ctrl-C
#include "noise.h"	// to perform noise analysis
#include "tdc.h"

#include <sstream>
#include <iostream>
#include <TApplication.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TStyle.h>
#include <TGraph.h>

#define SwRelease  "1.1.7m1 (30/07/2013)"	// program version (date)

//--------------------------------------------------------------------------
// catch CTRL-C, otherwise files etc. are not closed properly
void sigproc(int sig)
{
	signal(SIGINT, sigproc);
	errormsg(0,":::: WARNING: please use 'q' to stop the DAQ ::::");
}

//--------------------------------------------------------------------------
// MAIN
int main(int argc, char *argv[], char *envp[] )
{
    signal(SIGINT, sigproc);
	
    // ------------------------------------------------------------------
    // Define variables
    
    // stuff for reading ini files
    char ConfigFileName[100]   = "multiDAXConfig.ini";
    char BaselineFileName[100] = "multiDAXBaseline.ini";
    char PMTMappingFileName[100] = "multiDAXPMT.ini";
    char OutFileName[100]="";
    char XmlFileName[100]="";
    
    // variables to hold information
    crate crt;			// variable to store information on Crates
    digitizer adc;		// variable to store information on ADCs
	timedc tdc;			// variable to store information on TDC
    timeandtrigger t;		// variable for timing and trigger counting purposes
    scaler s;			// variable to hold scaler information
    multipurposemodule mp;	// variable to hold multipurpose model info
	
    // stuff for writing data
    int FileCounter=0;
    int FileEvtCounter=-1;
    int EventsPerFile=100;
	int useTDC=0; // check whether to use TDC
    FILE *ofile;		// file to store data
    FILE *logfile;		// log information (times, scalers, etc)
	
    // when XE100 data file format is used
    xdio_file_t *xfile;
    xdio_chunk_att_t *xatt;
    xdio_chunk_att_t *xatt_tdc;
	
    // stuff for reading data
    u_int32_t *buff[ADC_MAX];
	u_int32_t *tdcDataBuffer;		// buffer for TDC data
    int EnableInt, EnableOLIrq, EnableBerr, EvAlign; // to read from VME Control Register
    char c;
    int quit;
    int ret;
    int blt_bytes;
    CAEN_BYTE IrqMask, IrqCheckMask;
    int dataReady;
    int dataAvailable[ADC_MAX];
    int dataIncomplete;
    int bytes[ADC_MAX];
    int firstEvent;
    int syncProblem;
	int occupancy_exceeded=0;
	int skipRest;
	
    // Variables for Program Options
    int WriteToFile=3;		// specify data format (default=Xe100 format 3)
    int time, baseline;
    var_graphics graphics;
    u_int32_t NoE;
    char text[100];
    int32_t ms;
	
	int bytesCopied_tdc; // to reset tdc buffer
	
    // ------------------------------------------------------------------
    // Start with main program
	
    // Program Title
    control_printtitle(SwRelease);
	
    // Command Line Parameter processing
    control_cmdline( argc, argv, envp, time, NoE, graphics, baseline, adc,
					OutFileName, XmlFileName);
	
    // read hardware configuration file (set default values first)
    control_setdefault(adc, s, t, crt, mp, graphics);
    if (control_configfile(ConfigFileName, crt, adc, s, mp, WriteToFile)<0) return -1;
	
    // read baseline config file (baseline settings for all channels)
    ret=control_configfile(BaselineFileName, crt, adc, s, mp, WriteToFile);
    if (ret<0) { if (ret!=-3) goto exit_prog; }
	
    // read PMT mapping file
    ret=control_configfile(PMTMappingFileName, crt, adc, s, mp, WriteToFile);
	
    // read XML file if defined; these settings overwrite the *.ini settings!
    if (strlen(XmlFileName)!=0) {
		char path[100];
		if (xml_readsettings(XmlFileName,crt,adc, s, mp, t, graphics, NoE, EventsPerFile, useTDC, path, WriteToFile)!=0) {
			adc_closecrates(crt);
			exit(-1);
		}
		if (strlen(OutFileName)==0) control_generatefilename(OutFileName, path);
		xml_storefile(XmlFileName, OutFileName);
    } else if (strlen(OutFileName)==0) strcpy(OutFileName, "dummy");
	
    if (WriteToFile == 0) errormsg(0,":::: WARNING: No data is written! ::::");
	
    // open log-file
    if (control_openlog(&logfile,OutFileName,SwRelease)<0) goto exit_prog;
    control_log(logfile,0,"store logfile handler");  	// store the logfile filehandle as static
	// in this function (used to write to logfile)
    // give information to Slow Control -- DAQ is running
    control_scfile(10,0,0);
	
    // different pre-defined configuration (no ZLE etc.) for baseline determination
    if (baseline) {
		if (control_configbaseline(crt, adc)<0) goto exit_prog;
    }
	
    // Initialize the boards and set variables for acquisition
    //   adc.BoardInfo==1: Information on the modules is printed to screen
    adc_init(crt, adc, EnableInt, EnableOLIrq, EnableBerr, EvAlign);
    if (adc.BoardInfo) goto exit_prog;
	
	// set the VME-PCI bridge output multiplexer (OUT0 source is the output register [0])
	if ((ret=CAENVME_WriteRegister(crt.handle[0], cvOutMuxRegSet, 0x0003)) != cvSuccess)
	{
		char msg[256];
		sprintf(msg,":::: ERROR: Could not set V2718 output multiplexer! (code %d) ::::", ret);
		errormsg(0,msg);
		goto exit_prog;
	}
	
	// set the OUT0 to true (accept all triggers)
    if ((ret=CAENVME_SetOutputRegister(crt.handle[0], cvOut0Bit)) != cvSuccess)
	{
		char msg[256];
		sprintf(msg,":::: ERROR: Could not set V2718 output register! (code %d) ::::", ret);
		errormsg(0,msg);
		goto exit_prog;
    }
	
    // open root output to display waveforms graphically
    TApplication *theApp;
    TCanvas *win;
    TH1D *g;
	
	if (baseline==3) graphics.on=1;
    if (graphics.on)
	{
		if (baseline!=3) theApp = new TApplication("App", &argc, argv);
		if (baseline==3) win = new TCanvas("win","multiDAX -- DAQ for neriX",0,0,400,400);
		else win = new TCanvas("win","multiDAX -- DAQ for neriX",1);
		g = new TH1D("g","g",adc.NbSamplesEvt-1,0,adc.NbSamplesEvt-1);
		if (graphics.pmtNb>0) control_getadc(adc, graphics.pmtNb, graphics.module, graphics.channel);
		graph_init(theApp, win, g, graphics);
    }
	
    if (!EnableBerr)   // BERR must be enabled!
	{
		errormsg(0,":::: ERROR: BERR must be enabled to run the program! ::::");
		goto exit_prog;
    }
    
    // wait some time
    sleep(1);
    printf("\n%d Boards Configured\n",adc.NumberOfADCs);
	
    //  Allocate Memory
    //    Set maximum buffer size for event readout based on current configuration
    //    calculate needed size in Bytes first and allocate space for one more BLT
    adc.BufferSize = (EvAlign&&EnableBerr) ? (adc.ExpectedEvSize* EvAlign*4):(adc.ExpectedEvSize*4);
    adc.BufferSize += adc.BltSize;
	
	//    printf("adc.BufferSize: %d", adc.BufferSize);
	
    for (int i=0; i<adc.NumberOfADCs; i++)
	{
		if ( (buff[i] = (u_int32_t *)malloc(adc.BufferSize)) == NULL)
		{
			sprintf(text,":::: ERROR: Can't allocate memory buffer %d of %d kB ::::", i,adc.BufferSize/1024);
			errormsg(0,text);
			goto exit_prog;
		}
    }
	
	
	
    // Automatic baseline determination
    if (baseline) {
		if (baseline==1)
		{
			if (adc_getbaseline(crt,adc)<0) errormsg(0,":::: ERROR in baseline determination ::::");
		}
		else if (baseline==2)
		{
			if (adc_analyzebaseline(crt,adc)<0) errormsg(0,":::: ERROR in baseline analysis ::::");
		}
		else
		{
			if (noise_analysis(crt,adc,theApp,win)<0) errormsg(0,":::: ERROR in noise analysis ::::");
		}
		goto exit_prog;
    }
	
    // ------------------------------------------------------------------
    // Readout
    
    // Initialize time and trigger counters
    t.totnb=0;
    t.last_totnb=0;
    t.tottrg=0;
    t.last_tottrg=0;
    for (int i=0; i<10; i++) t.store10[i]=0;
    t.last_ave=0;
    t.MeasSeconds=0;
	
    // Start Acquisition
    adc_enable(crt,adc);
	t.StartTime=control_gettimestring(text,'u');
    col("green"); printf("\nAcquisition started. [q] quit\n"); col("black");
    control_printlog(logfile,1,"DAQ started");
    t.PreviousTime = get_time();
    quit = 0;
    firstEvent=1;
    syncProblem=0;
	
	
	// ---- Start TDC initialization
	
	if (useTDC)
	{
		// variables for the tdc
		
		// initialize tdc
		tdc_init(crt, tdc);
		
		// malloc space for the data
		
		if ((tdcDataBuffer = (u_int32_t *)malloc(tdc.BufferSize*sizeof(char))) == NULL)
		{
			printf(":::: ERROR: Can't allocate memory buffer of %d B ::::", tdc.BufferSize);
			exit(1);
		}
		
		bytesCopied_tdc = tdc.BufferSize;
				
		
		printf("Finished initializing TDC - save to xdio\n");
	}
	
	// ---- End TDC initialization
	
	
	
	
    // Readout Loop ----------------------------------------------------------
    while(!quit) {
		
		if (skipRest)
			adc_enable(crt,adc);
		
		if (syncProblem)  // restart DAQ when synchronization problems showed up
		{
			firstEvent=1;
			syncProblem=0;
			adc_enable(crt,adc);
			usleep(10000);
			t.StartTime=control_gettimestring(text,'u');
			t.reps=0;
		}
		
        // open file if data should be stored
		if (WriteToFile>0 && FileEvtCounter==-1)
		{
			if (WriteToFile!=3) {
				if (control_openfile(&ofile,OutFileName,FileCounter,WriteToFile,adc,logfile)<0) goto exit_prog;
			}
			else
			{
				if (useTDC)
				{
					if (control_openxdio_tdc(&xfile,&xatt, &xatt_tdc, OutFileName,FileCounter,WriteToFile,adc,tdc,logfile,EventsPerFile,t)<0) goto exit_prog;
				}
				else
				{
					if (control_openxdio(&xfile,&xatt, OutFileName,FileCounter,WriteToFile,adc,logfile,EventsPerFile,t)<0) goto exit_prog;
				}
			}
			FileCounter++;
			FileEvtCounter=0;
		}
		
		// Check keyboard commands
        c = 0;
        if (kbhit()) c = getch();
        if (c == 'q' || c == 'Q') quit = 1;
		if (graphics.on) graph_checkkey(graphics, adc, c, g, OutFileName);
		
		// Calculate throughput rate (every second) and read scaler
		if (control_calcrate(t, s, FileEvtCounter)<0) goto exit_prog;
		
		// Inhibit triggers if the buffer occupancy is too high
		int occupancy;
		occupancy = adc_eventstored(crt,adc);
		//occupancy = 0;
		//printf("DEBUG: occupancy %d/%d\n", occupancy, adc.NbMemoryBlocks);
		float factor = 0.5;
		// if the memory is too full, block triggers with OUT0
		if ((occupancy > factor*adc.NbMemoryBlocks) && (!occupancy_exceeded))
			//    if ((occupancy > 0) && (!occupancy_exceeded))
		{
			occupancy_exceeded = 1;
			printf("DEBUG: occupancy exceeded %d/%d\n", occupancy, adc.NbMemoryBlocks);
			
			if ((ret=CAENVME_ClearOutputRegister(crt.handle[0], cvOut0Bit)) != cvSuccess)
			{
				char msg[256];
				sprintf(msg,":::: ERROR: Could not clear V2718 output register! (code %d) ::::", ret);
				errormsg(0,msg);
				goto exit_prog;
			}
		}
		else if((occupancy == 0) && (occupancy_exceeded))
		{
			occupancy_exceeded = 0;
			printf("DEBUG: occupancy cleared\n");
			
			if ((ret=CAENVME_SetOutputRegister(crt.handle[0], cvOut0Bit)) != cvSuccess)
			{
				char msg[256];
				sprintf(msg,":::: ERROR: Could not set V2718 output register! (code %d) ::::", ret);
				errormsg(0,msg);
				goto exit_prog;
			}
		}
		
		// Interrupts:
		// If enabled, wait for the interrupt request from the digitizer. In this mode,
		// there is no CPU time wasting because the process sleeps until there are at least
		// N event available for reading, where N is the content of the register Interrupt
		// Event Number (0xEF18)
		//EnableInt = enable interrupts
		if (EnableInt)
		{
			if (EnableOLIrq) IrqMask = 0x01; // IRQ1 is used when interrupt generated via OLINK
			else IrqMask = 0xFF; // All VME Interrupt Enabled
			
			CAENVME_IRQEnable(crt.handle[0], IrqMask); // Enable IRQs
			ret = CAENVME_IRQWait(crt.handle[0], IrqMask, 1000); // Wait for IRQ (max 1sec)
			if (ret) continue;
			
			ret=CAENVME_IRQCheck(crt.handle[0], &IrqCheckMask);
			CAENVME_IRQDisable(crt.handle[0], IrqMask); // Disable IRQs
		}
		
		dataReady=0;
		dataIncomplete=0;
		skipRest=0;
		for (int i=0; i<adc.NumberOfADCs; i++)  // loop over all boards -- readout
		{
			// Check acquisition status
			// u_int32_t data;
			// if((CAENVME_ReadCycle(crt.handle[adc.Crate[i]], adc.Address[i]+0x8104,
			//         &data, cvA32_U_DATA, cvD32) != cvSuccess) || !(0x04 & data))
			// {
			//     continue;
			// }
			
			// Read data from module i in MBLT mode into buff[i]
			blt_bytes=adc_mblt(crt,adc,i,buff[i]);
			if (blt_bytes<0) goto exit_prog;
			
			if ((blt_bytes>0) && ((blt_bytes<=16) || ((0xF0000000 & buff[i][0])!=0xa0000000)))
			{
				//printf("ADC%d status: %08Lx\n", i, data);
				skipRest=1;
				dataReady=0;
			}
			
			if (!skipRest) // make sure data is ready
			{
				bytes[i]=blt_bytes;
				t.totnb += blt_bytes;	  // Total number of bytes transferred
				
				// this statement is important to ensure that reading/writing
				// always starts from module 0; i.e., the program starts again
				// with the first module when nothing was read
				// (probably not neccessary when interrupts work properly)
				if (!EnableInt && blt_bytes==0)
				{
					if (i==0) break;
					else dataAvailable[i]=0;
				}
				else if (!EnableInt && blt_bytes>0)
				{
					dataReady=1;
					dataAvailable[i]=1;
					//if (i == 0) { printf("### Event %d ###\n", FileEvtCounter); printf("BLT:  "); }
					//printf("%8Ld B  ", bytes[i]);
					//if (i == adc.NumberOfADCs-1) { printf("\n"); }
				}
				
				// handle the same with interrupts
				if (EnableInt && blt_bytes==0)
				{
					if (i==0) break;
					else dataAvailable[i]=0;
				}
				else if (EnableInt && blt_bytes>0)
				{
					dataReady=1;
					dataAvailable[i]=1;
				}
				
				if (dataAvailable[i]!=1) dataIncomplete++;
			}
		} // end: for loop over boards -- readout
		
		if (skipRest)
		{
			adc_disable(crt,adc);
			usleep(10000);
		}
		
		// ------ start tdc data collection
		if (useTDC && dataReady)
		{
			tdc_clear_data_buffer(tdc, tdcDataBuffer, tdc.BufferSize);
			//tdc_enable(crt, tdc);
			// may need to add a first event clear
			//usleep(10);
			bytesCopied_tdc = tdc_mblt(crt, tdc, tdcDataBuffer);
			//printf("\nBytes copied: %d", bytesCopied_tdc);
			
			//tdc_disable(crt, tdc);
		}
		// ------ end tdc data collection
		
		// write data loop
		if (dataReady) {
			if (dataIncomplete==0) 	// all modules have data
			{
				
				//          printf("DATA:  ");
				//          for (int i=0; i<adc.NumberOfADCs; i++)
				//              printf("0x%08Lx 0x%08Lx 0x%08Lx 0x%08Lx  |  ", buff[i][0], buff[i][1], buff[i][2], buff[i][3]);
				//          printf("\n");
				
				// check for module synchronization
				if (firstEvent)  // initialize old values for first event of entire set
				{
					for (int i=0; i<adc.NumberOfADCs; i++) adc.ttt0[i]=control_getTTT(buff[i]);
					firstEvent=0;
				}
				int deltaT=0;
				int max=0;
				u_int32_t newTTT;
				for (int i=0; i<adc.NumberOfADCs; i++)  // loop over all boards -- check TTT
				{
					newTTT=control_getTTT(buff[i]);
					if (newTTT<adc.ttt0[i]) adc.ttt0[i]=adc.ttt0[i]-2147483648;  // subtract 2^31 when counter starts from zero
					if (i==0) deltaT=newTTT-adc.ttt0[i];
					if (abs(newTTT-adc.ttt0[i]-deltaT)>max) max=abs(newTTT-adc.ttt0[i]-deltaT); // get max deviation from deltaT of first board
					adc.ttt0[i]=newTTT;	// store new as old
				}
				//        printf("TTT:  ");
				//        for (int i=0; i<adc.NumberOfADCs; i++) { printf("%0.9d  ", adc.ttt0[i]); }
				//        printf("\n");
				
				// check if TTT difference is too large (as a false trigger would come with dt ~ trigger holdoff)
				if (max<10000)
				{
					// do nothing, everything ok
				}
				else
				{
					printf("TTT:  ");
					for (int i=0; i<adc.NumberOfADCs; i++) { printf("%.9d  ", adc.ttt0[i]); }
					printf("\n");
					char errstr[100];
					adc_disable(crt,adc); // stop DAQ
					sprintf(errstr,":::: WARNING: Modules are not synchronized (Event: %d) ::::",FileEvtCounter);
					errormsg(1,errstr);
					sprintf(errstr,":::: WARNING:   max dt=%d  ::::",max);
					errormsg(1,errstr);
					syncProblem=1;
				}
				
				
				// -- end: check module synchronization
				
				// write if data is ok
				if (!syncProblem)
				{
					for (int i=0; i<adc.NumberOfADCs; i++) { // loop over all boards -- write data
						
						// Write Data
						if (WriteToFile!=3) {
							ret=control_writedata(adc,i,ofile,WriteToFile,bytes[i],buff[i]);
							if (ret>=0) {
								t.tottrg+=ret; FileEvtCounter+=ret;
							} else goto exit_prog;
						} else { // Xe100 files
							if (useTDC)
								ret=control_writexdio_tdc(adc,tdc,t,i,xfile,xatt,xatt_tdc,WriteToFile,bytes[i],buff[i],tdcDataBuffer);
							else
								ret=control_writexdio(adc,t,i,xfile,xatt,WriteToFile,bytes[i],buff[i]);
							if (ret>=0) {
								t.tottrg+=ret; FileEvtCounter+=ret;
							} else goto exit_prog;
						}
						
						// Display Waveform?
						if (graphics.on && graphics.module==i) {
							if (adc.ZLE) ret=graph_display(g,buff[i],graphics.channel);
							else ret=graph_displayNoZLE(g,buff[i],graphics.channel);
							if (ret<0) errormsg(1,":::: WARNING: Cannot display waveform! ::::");
							else {
								graph_edit(g,graphics,adc);
								g->Draw();
								win->Update();
							}
						}
						
					} // end: for loop over boards -- write data
				} // end: if(!syncProblem)
				
			}
			else
			{
				if (dataIncomplete!=adc.NumberOfADCs)
				{
					// generate warning message when not all modules have data
					// (but ignore the case when all modules have nothing)
					char errstr[100];
					char modStr[5];
					char collectStr[100]="";
					
					for (int j=0; j<adc.NumberOfADCs; j++)
					{
						if (dataAvailable[j]!=1)
						{
							sprintf(modStr,"%d ",j);
							strcat(collectStr,modStr);
						}
					}
					sprintf(errstr,":::: WARNING: modules %s returns nothing (evt: %d) ::::",collectStr,FileEvtCounter);
					errormsg(1,errstr);
				}
			} // end: if dataIncomplete
		} // end: data ready
		
		// max Number of Events per File reached?
		if (WriteToFile>0 && FileEvtCounter==EventsPerFile)
		{
			if (WriteToFile!=3)
			{
				if (fclose(ofile)!=0)
				{
					errormsg(1,":::: ERROR: cannot close data file! ::::");
					goto exit_prog;
				}
			}
			else xdio_close(xfile);
			FileEvtCounter=-1;	// flag to create new file
		}
        
		// Check if Number of Events to be measured are reached
		if (NoE>0 && t.tottrg==NoE) quit=1;
		// Check if time to be measured is reached
		if (time>0 && t.MeasSeconds>=time) quit=1;
		
    } // end: readout-loop ------------------------------------------------------
    
    // print summary and update logfile
    ms = get_time()-t.PreviousTime;
    control_calctime(t,ms,text);
    col("blue"); printf("  %s\n",text);
    control_printlog(logfile,1,"DAQ stopped");
    control_printlog(logfile,0,text);
    sprintf(text,"Total Number of Events Measured = %d", t.tottrg);
    printf("  %s\n\n",text); col("black");
    control_printlog(logfile,0,text);
    
    // close data-file(s)
    if (WriteToFile>0 && quit && FileEvtCounter>=0)
	{
		if (WriteToFile!=3)
		{
			if (fclose(ofile)!=0) errormsg(1,":::: ERROR: cannot close data file! ::::\n");
		}
		else xdio_close(xfile);
		
    }
    
    for (int i=0; i<adc.NumberOfADCs; i++) free(buff[i]);
	
	if (useTDC)
	{
		// turn off tdc data collection
		tdc_disable(crt, tdc);
		
		// free malloced tdc memory
		free(tdcDataBuffer);
	}
	
	// terminate program
exit_prog:
    adc_closecrates(crt);
    if (!baseline && !adc.BoardInfo) fclose(logfile);
    control_scfile(0,0,1);	// information to Slow Control -- DAQ is stopped
	
    return 0;
}

