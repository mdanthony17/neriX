//-------------------------------------------------------------------
//  xml.cpp   --   DAX: DAQ for XENON100 
// 
//  the xml library contains functions to read the XML settings file
//
//  04.12.2007 MS
//  last revision: 21.03.2008 MS  -- now with proper error handling
//  modified: 17.04.2008 MS
//  modified: 21.04.2008 MS  -  Trigger Latency
//  modified: 20.05.2008 MS  -  Individual ZLE Thresholds
//  modified: 13.06.2008 MS  -  Phillips Hex pattern included
//  modified: 21.07.2008 MS  -	read discriminator settings and 
//				set discriminator
//  modified: 29.07,2013 GP  -  merged in various DAX options
//-------------------------------------------------------------------

extern "C" {
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
#include "keyb.h"   // getch() and kbhit() for Linux
#include "adc.h"
#include "control.h"
#include "xmlParser.h"


//-------------------------------------------------------------------
// display xml parsing error message
int xml_error(const char *node)
{
  char txt[120];
  sprintf(txt,":::: WARNING: missing information in xml-file (%s) ::::",node);
  errormsg(0,txt);
  return 0;
}

//-------------------------------------------------------------------
// reads the XML settings file
int xml_readsettings(char filename[100], crate& crt, digitizer& adc, scaler& s,
	multipurposemodule& mp, timeandtrigger& t, var_graphics& graphics,
	u_int32_t& NoE, int& EventsPerFile, char *path, int& WriteToFile)
{
  int temp;  
  char txt[2048];
  u_int32_t data, data2, addr;
  u_int32_t datas=0;
  const char *xstr;
  txt[0]='\0';
  int bufSize=-1;

  printf("Reading User Configuration File %s\n",filename);
  
  // open the XML file -------------------------------------------------------
  XMLNode xMainNode=XMLNode::openFileHelper(filename,"settings");

  printf("Settings File %s\n", filename);

  xstr=xMainNode.getAttribute("author");
  if (xstr) {
	strcpy(txt,xstr); 
	printf("  (Author: %s,",txt);	  
  } else xml_error("author"); 
  
  xstr=xMainNode.getAttribute("date");		
  if (xstr) {
	strcpy(txt,xstr);
	printf("  Date: %s)\n",txt);
  } else xml_error("date"); 

  // parse global DAQ settings -----------------------------------------------
  XMLNode xNode=xMainNode.getChildNode("global");
  printf("Global Settings:\n");
  
  xstr=xNode.getChildNode("source").getText();
  if (xstr) {
    strcpy(txt,xstr); 
    col("gray"); printf("  Source:      %s\n",txt); col("black");
  } else xml_error("source");

  xstr=xNode.getChildNode("notes").getText();
  if (xstr) {
	strcpy(txt,xstr); 
    col("gray"); printf("  Comments:    %s\n",txt); col("black");
  } else xml_error("notes");

  xstr=xNode.getChildNode("path").getText();
  if (xstr) {
	strcpy(txt,xstr); 
    printf("  Path:        %s\n",txt);
    strcpy(path,txt);
  } else xml_error("path");

  xstr=xNode.getChildNode("daq_version").getText();
  if (xstr) {
	strcpy(txt,xstr); 
    col("gray"); printf("  DAQ Version: %s\n",txt); col("black"); 
  } else xml_error("daq_version");

  xstr=xNode.getChildNode("nb_evts").getText();
  if (xstr) {
	strcpy(txt,xstr); 
    if (strstr(txt, "-1")!=NULL) printf("  NoF Events:  infinite\n");
       		            else printf("  NoF Events:  %s\n",txt);
    NoE=atoi(txt); 
  } else xml_error("nb_evts");

  xstr=xNode.getChildNode("store_data").getText();
  if (xstr) {
    strcpy(txt,xstr); 
    WriteToFile=atoi(txt); 
    if (WriteToFile<0 || WriteToFile>3) WriteToFile=3;	// Xe100 as default
    printf("  File Format: ");
    switch (WriteToFile) {
      case 0: printf("Data NOT stored\n"); break;
      case 1: printf("old CAEN format\n"); break;
      case 2: printf("ASCII\n"); break;
      case 3: printf("neriX format\n"); break;
    }
	if (WriteToFile!=3) {
	  errormsg(0,":::: WARNING: Do you really want to store data NOT in neriX format? (y/n)? ::::");
	  char cc;
	  while (1) {
        cc = 0;
        if (kbhit()) cc = getch();
        if (cc == 'Y' || cc == 'y') break;
		if (cc == 'N' || cc == 'n') return -2;
	  }
	}  
  } else xml_error("store_data");

  xstr=xNode.getChildNode("compress").getText();
  if (xstr) {
	strcpy(txt,xstr);
	adc.compression=atoi(txt);
    printf("  Compression: ");
	switch (adc.compression) {
	  case 0: printf("None\n"); break;
	  case 1: printf("gzip\n"); break;
	  case 2: printf("bzip2\n"); break;
	  default:
	    errormsg(0,":::: WARNING: Compression Mode is not valid; NO compression used! ::::");
		adc.compression=0;
	  break;
	}
  } else xml_error("compress");

  xstr=xNode.getChildNode("nb_evts_per_file").getText();
  if (xstr) {
	strcpy(txt,xstr); 
    printf("  Events/File: %s\n",txt);
    EventsPerFile=atoi(txt);
  } else xml_error("nb_evts_per_file");

  xstr=xNode.getChildNode("displaytime").getText();
  if (xstr) {
	strcpy(txt,xstr); 
    printf("  DisplayTime: %s s\n",txt);
    t.DisplayTime=atoi(txt);  
  } else xml_error("displaytime");
    
  xstr=xNode.getChildNode("user").getText();
  if (xstr) {
	strcpy(txt,xstr); 
    col("gray"); printf("  User:        %s\n",txt); col("black");
  } else xml_error("user");
  
  
  // parse global ADC settings -----------------------------------------------
  xNode=xMainNode.getChildNode("adc").getChildNode("global");
  printf("ADC: Global settings:\n");
  
  xstr=xNode.getChildNode("nb_chs").getText();
  if (xstr) {
	strcpy(txt,xstr); 
    adc.NbChannels=atoi(txt);
    col("gray"); printf("  Channels:    %s\n",txt); col("black");	
  } else xml_error("nb_chs");
  
  xstr=xNode.getChildNode("nb_samples_per_evt").getText();
  if (xstr) {
	strcpy(txt,xstr); 
    bufSize=atoi(txt);
    float usec;
    switch (bufSize) {
      case 512: data=0x0; usec=5242.88; break;
      case 256: data=0x1; usec=2621.44; break;
      case 128: data=0x2; usec=1310.72; break;
      case  64: data=0x3; usec=655.36; break;
      case  32: data=0x4; usec=327.98; break;
      case  16: data=0x5; usec=163.84; break;
      case   8: data=0x6; usec=81.92; break;
      case   4: data=0x7; usec=40.96; break;
      case   2: data=0x8; usec=20.48; break;
      case   1: data=0x9; usec=10.24; break;
      case   0: data=0xA; usec=5.12; break;  // 0.5 k
      default:  data=0x4; 
      	        errormsg(0,":::: WARNING: Set Samples/Evt to 32k ::::");
				bufSize=32;
	                  break; 
    }
    printf("  Samples/Evt: %sk (%.2f us)\n",txt,usec);
    for (int i=0; i<adc.NumberOfADCs; i++) {
      if (adc_writereg(crt,adc,i,BlockOrganizationReg,data)<0) return -1; 
    }  
  } else xml_error("nb_samples_per_evt");

  xstr=xNode.getChildNode("custom_size").getText();
  if (xstr) {
	strcpy(txt,xstr);
	temp=(int)(atoi(txt)/2.);	// 2 samples per memory location
    // catch possible errors
	if (temp!=0 && bufSize==-1) {
	  errormsg(0,":::: ERROR: define 'nb_samples_per_event' before 'custom_size' ::::");
	  return -1;
	}
	bufSize=bufSize*1024;
    if (bufSize==0) bufSize=512;
    if (temp*2>bufSize) {
	  errormsg(0,":::: ERROR: custom window must be smaller than nb_samples_per_event*1024 ::::");	
	  return -1;
	}
	printf("  Custom Size: %d samples\n",temp*2);
	data=temp;
	adc.EventLength=data*2;
    for (int i=0; i<adc.NumberOfADCs; i++) {
	  if (adc_writereg(crt,adc,i,CustomWindow,data)<0) return -1;
	}
  } else xml_error("custom_size");

  xstr=xNode.getChildNode("posttrigger").getText();
  if (xstr) {
	strcpy(txt,xstr); 
    printf("  Posttrigger: %s\n",txt);
    temp=atoi(txt);
    data=(temp-adc.TriggerLatency)/2;	// convert in 32bit words words  
    for (int i=0; i<adc.NumberOfADCs; i++) {
      if (adc_writereg(crt,adc,i,PostTriggerReg,data)<0) return -1; 
    }  
  } else xml_error("posttrigger");

  xstr=xNode.getChildNode("baseline").getText();
  if (xstr) {
	strcpy(txt,xstr);
    printf("  Baseline:    %s\n",txt);
    adc.baseline=atoi(txt);
  } else xml_error("baseline");

  xstr=xNode.getChildNode("trigger").getText();
  if (xstr) {
    strcpy(txt,xstr); 
    adc.trig=atoi(txt);
    if (adc.trig!=0) printf("  Trigger:     S%d\n",adc.trig);
  	        else printf("  Trigger:     external\n");
    if (adc.trig<0 || adc.trig>2) {
      errormsg(0,":::: WARNING: This trigger mode does not exist. Set to S2 trigger. ::::\n");
      adc.trig=2;
    }  
    if (adc.trig==1) data=0x0;	// S1: Majority Mode
                else data=0x2;  // S2: Analog Sum (also used if external trigger, adc.trig=0
    for (int i=0; i<adc.NumberOfADCs; i++) {
      if (adc_writereg(crt,adc,i,MonitorModeReg,data)<0) return -1; 
    }  
  } else xml_error("trigger");

  if (adc.trig!=0) {
    xstr=xNode.getChildNode("trigmask").getText();
    if (xstr) {
      strcpy(txt,xstr);
      sscanf(txt, "%x", &adc.trigmask);
      printf("  TriggerMask: %08X\n",adc.trigmask); 
    } else xml_error("trigmask");
  } else { // external trigger -> disable all internal triggering
    adc.trigmask=0x0;
    printf("  TriggerMask: %08X\n",adc.trigmask); 
  }

  if (adc.trig==1) {
    xstr=xNode.getChildNode("s1_thresh").getText();
    if (xstr) {
      strcpy(txt,xstr); 
      temp=atoi(txt);
      printf("  S1 threshold %s\n",txt); 
      data=adc.baseline-temp;
      for (int i=0; i<adc.NumberOfADCs; i++) {
        if (adc_writereg(crt,adc,i,S1Threshold,data)<0) return -1; 
      }
    } else xml_error("s1_thresh");
  }

  xstr=xNode.getChildNode("sampling_freq").getText();
  if (xstr) {
	strcpy(txt,xstr); 
    adc.sampling_freq=atoi(txt);
    col("gray"); printf("  Frequency:   %s Hz\n",txt); col("black");
  } else xml_error("sampling_freq");
  
  xstr=xNode.getChildNode("sample_size").getText();
  if (xstr) {
	strcpy(txt,xstr); 
    adc.sample_size=atoi(txt);
    col("gray"); printf("  Sample Size: %s bit\n",txt); col("black");
  } else xml_error("sample_size");

  xstr=xNode.getChildNode("voltage_range").getText();
  if (xstr) {
	strcpy(txt,xstr); 
    adc.voltage_range=atof(txt);
    col("gray"); printf("  Volt. Range: %s V\n",txt); col("black");
  } else xml_error("voltage_range");
  
  xstr=xNode.getChildNode("busy").getText();
  if (xstr) {
	strcpy(txt,xstr); 
    adc.busy=atoi(txt);
    if (adc.busy) printf("  BUSY:        On\n");
  	         else printf("  BUSY:        Off\n");
    if (adc.busy!=0) adc.busy=1;

    for (int i=0; i<adc.NumberOfADCs; i++) {	
      if (adc.busy) {
        if (adc_writereg(crt,adc,i,FrontPanelIOData,0x40)<0) return -1; // enable busy output
      } else {
        if (adc_writereg(crt,adc,i,FrontPanelIOData,0x0)<0) return -1;  // disable busy output 
      }
    }
  } else xml_error("busy");

  
  // ADC: parse ZLE settings -----------------------------------------------
  xNode=xMainNode.getChildNode("adc").getChildNode("zle_settings");
  printf("ADC: ZLE Settings:\n");
  
  // This XML-entry sets the ZLE mode and takes also care of the 
  // trigger mask, that is read above
  xstr=xNode.getChildNode("zle").getText();
  if (xstr) {
    strcpy(txt,xstr); 
    temp=atoi(txt);  
    // Falling + Sequential + External signal + Non Overlap Triggers + AnalogSum
    if (temp==1) { 	// ZLE
      data=0x200D0; 
      printf("  ZLE:         On\n");	    
    } else { 		// no ZLE encoding	
      data=0xD0; 	
      printf("  ZLE:         Off\n");
    }   

    int trg;  
    for (int i=0; i<adc.NumberOfADCs; i++) {	
      if (adc.trigmask>>i&0x1) { data2=data; trg=1; }
      		  	  else { data2=data-0x80; trg=0; } // no Analog Sum Output
      if (adc_writereg(crt,adc,i,ChannelConfigReg,data2)<0) return -1; 
      if (trg==0) {
        if (adc.trig==1) 
          if (adc_writereg(crt,adc,i,MonitorModeReg,0x2)<0) return -1; // set to Analog Monitor Mode
								       // in order to disable channels
        if (adc_writereg(crt,adc,i,AnalogMonitorSettings,0x0)<0) return -1;  // disable all 8 channels
      }
    }
  } else xml_error("zle");
  
  xstr=xNode.getChildNode("threshold").getText();
  if (xstr) {
	strcpy(txt,xstr); 
    printf("  Threshold:   %s\n",txt);
    temp=atoi(txt);
    data=adc.baseline-temp;
    data=data+0x80000000;
    for (int i=0; i<adc.NumberOfADCs; i++) {
      if (adc_writereg(crt,adc,i,ZLEThreshold,data)<0) return -1; 
    }  
  } else xml_error("threshold");
  
  xstr=xNode.getChildNode("n_pre").getText();
  if (xstr) {
	strcpy(txt,xstr); 
    printf("  N_pre:       %s\n",txt);
    temp = atoi(txt)/2;  
    datas = temp;  
  } else xml_error("n_pre");
   
  xstr=xNode.getChildNode("n_post").getText();
  if (xstr) {
	strcpy(txt,xstr); 
    printf("  N_post:      %s\n",txt);
    temp = atoi(txt)/2;
	if (datas==0) datas=temp; 	// in case n_post comes before n_pre in xml-file
								// --> use same settings for both
    data = (datas<<16) + temp;
    for (int i=0; i<adc.NumberOfADCs; i++) {
      if (adc_writereg(crt,adc,i,LookBackForward,data)<0) return -1; 
    }  
  } else xml_error("n_post");
    
  // special thresholds for particular PMTs
  xNode=xMainNode.getChildNode("adc").getChildNode("zle_settings").getChildNode("special_thresholds");
  int specialNb=xNode.nChildNode("pmt_thresh");

  int tmp1=0;
  int tmp2=0;
  int pmt, threshold, module, channel;
  printf("  Special ZLE Thresholds:\n");	  
  for (int i=0; i<specialNb; i++) {
    xstr=xNode.getChildNode("pmt_thresh",&tmp1).getAttribute("pmtNb");
    if (xstr) pmt=atoi(xstr);
         else xml_error("pmt_thresh"); 
    xstr=xNode.getChildNode("pmt_thresh",&tmp2).getAttribute("value");
    if (xstr) threshold=atoi(xstr);
         else xml_error("pmt_thresh"); 
    printf("    PMT%3d:    %d\n",pmt,threshold);
    if (control_getadc(adc, pmt, module, channel)==0) {
      addr=SingleZLEThresh+channel*0x100;
      data=adc.baseline-threshold;
      data=data+0x80000000;
      if (adc_writereg(crt,adc,module,addr,data)<0) return -1;
    } else errormsg(0,":::: WARNING: PMT does not exist ::::");
  }

  // ADC: parse waveform display options
  xNode=xMainNode.getChildNode("adc").getChildNode("graphics");
  
  xstr=xNode.getChildNode("xdisplay").getText();
  if (xstr) {
    strcpy(txt,xstr); 
    graphics.x_display = atoi(txt);    
  } else xml_error("xdisplay");

  xstr=xNode.getChildNode("ydisplay").getText();
  if (xstr) {
    strcpy(txt,xstr); 
    graphics.y_display = atoi(txt);    
  } else xml_error("ydisplay");

  xstr=xNode.getChildNode("xaxis_low").getText();
  if (xstr) {
    strcpy(txt,xstr); 
    graphics.x_low = atoi(txt);    
  } else xml_error("xaxis_low");
  
  xstr=xNode.getChildNode("xaxis_high").getText();
  if (xstr) {
    strcpy(txt,xstr); 
    graphics.x_high = atoi(txt);    
  } else xml_error("xaxis_high");

  xstr=xNode.getChildNode("yaxis_low").getText();
  if (xstr) {
    strcpy(txt,xstr); 
    graphics.y_low = atoi(txt);    
  } else xml_error("yaxis_low");
  
  xstr=xNode.getChildNode("yaxis_high").getText();
  if (xstr) {
    strcpy(txt,xstr); 
    graphics.y_high = atoi(txt);    
  } else xml_error("yaxis_high");

  xstr=xNode.getChildNode("xautorange").getText();
  if (xstr) {
    strcpy(txt,xstr); 
    graphics.x_autorange = atoi(txt);    
  } else xml_error("xautorange");

  return 0;
}

	
//-------------------------------------------------------------------
// stores a copy of the XML file with the data
int xml_storefile(char *filename, char *OutFileName)
{
  char cmnd[300];
  sprintf(cmnd,"cp %s %s.xml",filename,OutFileName);
  if (system(cmnd)!=0) errormsg(0,":::: WARNING: Could not save a copy of XML-file ::::");
  
  return 0;
}


