//-------------------------------------------------------------------
//  graph.cpp   --   multiDAX: DAQ for neriX
// 
//  the graph library contains functions to plot the waveforms
//
//  modified: 17.09.2008 MS for miniDAX
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
#include "control.h"

#include <TH1D.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TFile.h>
#include <TLatex.h>

//-------------------------------------------------------------------
// Initialize graphical Output
int graph_init(	TApplication *theApp, TCanvas *win, 
		TH1D *g, var_graphics& graphics)
{
  // set window and canvas settings
  win->cd();
  gStyle->SetOptStat(0000000);
  gStyle->SetOptFit(1100);
  gStyle->SetTitleFillColor(0);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetStatColor(0);
  win->SetFillColor(0);
  win->SetBorderMode(0);

  // configure histogram 
  char str[50];
  sprintf(str,"XENON100: PMT %d (Module %d.%d)",graphics.pmtNb, graphics.module, graphics.channel);
  g->SetTitle(str);
  g->SetLineColor(4);
  g->GetXaxis()->SetTitle("Sample");
  g->GetYaxis()->SetTitle("ADC Counts");
  
  // Startup Window
  TLatex * tex = new TLatex(0.0556465,0.379562,"DAX");
  tex->SetTextColor(2);
  tex->SetTextFont(112);
  tex->SetTextSize(0.729927);
  tex->SetLineWidth(2);
  tex->Draw();
  tex = new TLatex(0.1702128,0.8053528,"neriX");
  tex->SetTextColor(4);
  tex->SetTextFont(72);
  tex->SetTextSize(0.1216545);
  tex->SetLineWidth(2);
  tex->Draw();
  tex = new TLatex(0.0900164,0.233577," DAQ for neriX");
  tex->SetTextSize(0.136253);
  tex->SetLineWidth(2);
  tex->Draw();
  win->Modified();
  win->cd();
  win->SetSelected(win);  
  win->Update();
sleep(3); 
  return 0;
}

//-------------------------------------------------------------------
// Reconstruct waveform and display histogramm
int graph_display(TH1D *g, u_int32_t *buff, int channel)
{ 
  int pnt=0; 
  int CurrentChannel;
  int Size, cnt, wavecnt;
  int GoodWords;
  
  g->Reset();
  
  // error handling if there is an invalid entry after an event
  if (buff[0]==0xFFFFFFFF) pnt++;
    
  // check header
  if ((buff[pnt]>>20)==0xA00 && (buff[pnt+1]>>28)==0x0) {
    pnt++;
    int ChannelMask=buff[pnt] & 0xFF;	    	      pnt++;
    if ((ChannelMask>>channel)&1==0) return 0; 	      // no waveform available -> return
    pnt+=2; 

    for (int j=0; j<8; j++) { // read all channels
      // read only the channels given in ChannelMask
      if ((ChannelMask>>j)&1) CurrentChannel=j;
                              else continue;

      Size=buff[pnt];              // size of next waveform
      if (CurrentChannel!=channel) { pnt+=Size; continue; }
      			      else pnt++;
      if (j>channel) return 0;			      
			      
      cnt=2;                              // counter of waveform data
      wavecnt=0;                          // counter to reconstruct times within waveform
      while (cnt<=Size)
      {
        // check for invalids just after good samples
        if ((buff[pnt]>>28)==0x8) { // good data
          GoodWords=buff[pnt]&0xFFFFFFF;        pnt++;  cnt++;
                  
          // save waveform in histogram
          for (int i=0; i<GoodWords; i++) {
             g->SetBinContent(wavecnt,((buff[pnt]&0xFFFF)));
             g->SetBinContent(wavecnt+1,(((buff[pnt]>>16)&0xFFFF)));
             pnt++; wavecnt+=2; cnt++;
          }
        
	} else { // skip
          if (buff[pnt]!=0xFFFFFFFF) {
            wavecnt+=2*buff[pnt]+1;
            pnt++; cnt++;
          } else { errormsg(0,"skip"); return -1; }
        }
      } // end while(cnt...)
    } // end for-loop
  } // end Check Header 
  else { errormsg(0,"Header"); return -1; }

  return 0;
}

int graph_displayNoZLE(TH1D *g, u_int32_t *buff, int channel)
{  
  int pnt=0; 
  int CurrentChannel;
  int Size, cnt, wavecnt;
  
  g->Reset();
  
  // error handling if there is an invalid entry after an event
  if (buff[0]==0xFFFFFFFF) pnt++;
    
  // check header
  if ((buff[pnt]>>20)==0xA00 && (buff[pnt+1]>>28)==0x0) {
    Size=((buff[pnt]&0xFFFFFFF)-4);                   // size of full waveform (all channels)
    pnt++;
    int ChannelMask=buff[pnt] & 0xFF;	    	      pnt++;    
    if ((ChannelMask>>channel)&1==0) return 0; 	      // no waveform available -> return
    
    // Get size of one waveform by dividing through the number of channels
    cnt=0;
    for (int j=0; j<8; j++) if ((ChannelMask>>j)&1) cnt++;
    Size=Size/cnt;
    
    pnt+=2; // ignore EventConter and TTT

    for (int j=0; j<8; j++) { // read all channels
      // read only the channels given in ChannelMask
      if ((ChannelMask>>j)&1) CurrentChannel=j;
                              else continue;

      if (CurrentChannel!=channel) { pnt+=Size; continue; }
      			      else pnt++;
      if (j>channel) return 0;			      
			      
      cnt=0;                              // counter of waveform data
      wavecnt=0;                          // counter to reconstruct times within waveform
      while (cnt<Size)
      {
        // save waveform in histogram
        g->SetBinContent(wavecnt,((buff[pnt]&0xFFFF)));
        g->SetBinContent(wavecnt+1,(((buff[pnt]>>16)&0xFFFF)));
        pnt++; wavecnt+=2; cnt++;              
      } // end while(cnt...)
    } // end for-loop
  } // end Check Header 
  else return -1; 

  return 0;
}

//-------------------------------------------------------------------
// prepare graph for display
int graph_edit( TH1D *g, var_graphics& graphics, digitizer& adc)
{
  int min,max;

  // x-axis
  switch (graphics.x_display) {
    case 1:  // set manually (via xml-file)  
      g->GetXaxis()->SetRangeUser(graphics.x_low,graphics.x_high);
      break;
    case 2:  // set automatically
      min=g->GetMinimumBin();
      g->GetXaxis()->SetRange(min-graphics.x_autorange,min+graphics.x_autorange);
      break;
    case 0:
      g->GetXaxis()->SetRangeUser(0,adc.NbSamplesEvt-1);
      break;
    default: // do nothing; display all
      break;
  }

  // y-axis
  switch (graphics.y_display) {
    case 1:  // set manually (via xml-file)  
      g->GetYaxis()->SetRangeUser(graphics.y_low,graphics.y_high);
      break;
    case 2:  // set automatically
      min=(int)g->GetMinimum();
      max=(int)g->GetMaximum();
      g->GetYaxis()->SetRange(min,max);
      break;
    case 0:
      g->GetYaxis()->SetRangeUser(0,16384);
      break;
    default: // do nothing; display all
      break;
  }

  return 0;
}

//-------------------------------------------------------------------
// keys for Oscilloscope mode
int graph_checkkey(var_graphics& graphics, digitizer& adc, char c, TH1D *g, char *OutFileName)
{
  char str[100];
  static int counter = 0; 
  static TH1D *hist = new TH1D("hist","hist",1000,15384,16384);

  // increase channel
  if (c == '+' || c == '=') {
    // in channel display mode
    if (graphics.pmtNb==-1) { 
      graphics.channel++;  
      if (graphics.channel>7) {
        graphics.module++;
        graphics.channel=0;
        if (graphics.module>adc.NumberOfADCs-1) graphics.module=0;
      }
    } else { 
      // in PMT display mode
      graphics.pmtNb++;
      if (graphics.pmtNb>adc.pmtmax) graphics.pmtNb=0;
      control_getadc(adc, graphics.pmtNb, graphics.module, graphics.channel);
    } 
    sprintf(str,"XENON100: PMT %d (Module %d.%d)",graphics.pmtNb, graphics.module, graphics.channel);
    g->SetTitle(str);
  }
  
  // decrease channel
  if (c == '-' || c == '_') {
    // in channel display mode
    if (graphics.pmtNb==-1) { 
      graphics.channel--;  
      if (graphics.channel<0) {
        graphics.module--;
        graphics.channel=7;
        if (graphics.module<0) graphics.module=adc.NumberOfADCs-1;
      }
    } else {
      // in PMT display mode
      graphics.pmtNb--;
      if (graphics.pmtNb<1) graphics.pmtNb=adc.pmtmax;
      control_getadc(adc, graphics.pmtNb, graphics.module, graphics.channel);
    }
    sprintf(str,"XENON100: PMT %d (Module %d.%d)",graphics.pmtNb, graphics.module, graphics.channel);
    g->SetTitle(str);
  } 

  // change display mode y-axis
  if (c == '1' || c == '2' || c == '0' || c == 'u' || c == 'd' || c == 'U' || c == 'D') {
    switch (c) {
      case '0': graphics.y_display=0; 	
		col("green"); printf("y-axis: standard display mode\n"); col("black");
	break;
      case '1': graphics.y_display=1; 	
		col("green"); printf("y-axis: manual display mode\n"); col("black");
	break;
      case '2': graphics.y_display=2; 	
		col("green"); printf("y-axis: automatic display mode\n"); col("black");
	break;
      case 'u': if (graphics.y_display==1) graphics.y_low+=10;	break;
      case 'U': if (graphics.y_display==1) graphics.y_high+=10;	break;
      case 'd': if (graphics.y_display==1) graphics.y_low-=10;	break;
      case 'D': if (graphics.y_display==1) graphics.y_high-=10;	break;
    }
  }

  // change display mode x-axis
  if (c == '3' || c == '4' || c == '5' || c == 'l' || c == 'r' || c == 'L' || c == 'R' || 'o') {
    switch (c) {
      case '3': graphics.x_display=0; 	
		col("green"); printf("x-axis: standard display mode\n"); col("black");
	break;
      case '4': graphics.x_display=1; 	
		col("green"); printf("x-axis: manual display mode\n"); col("black");
	break;
      case '5': graphics.x_display=2; 	
		col("green"); printf("x-axis: automatic display mode\n"); col("black");
	break;
      case 'l': if (graphics.x_display==1) graphics.x_low-=50;	break;
      case 'L': if (graphics.x_display==1) graphics.x_high-=50;	break;
      case 'r': if (graphics.x_display==1) graphics.x_low+=50;	break;
      case 'R': if (graphics.x_display==1) graphics.x_high+=50;	break;
      case 'o': control_scopehelp(); break;	// show online help display
    }
  }

  // store waveform in file
  if (c == 's' || c == 'S') {
    TFile *f;
    sprintf(str,"%s_plot_%02d.root",OutFileName,counter);
    f = new TFile (str,"recreate");
    if (f==NULL) errormsg(0,":::: WARNING: cannot store waveform! ::::");
    else {
      col("green"); printf("store waveform to file %s\n",str); col("black");
      f->cd();
      g->Write();
      f->Close();
      counter++;
    }
  }

  // print baseline information
  if (c == 'b' || c == 'B') {
    hist->Reset();
    for (int k=0; k<adc.NbSamplesEvt-1; k++) hist->Fill(g->GetBinContent(k));
    col("green"); printf("Waveform PMT %d: mean = %.1lf, RMS = %.6lf\n",
		adc.pmt[graphics.module][graphics.channel],hist->GetMean(), hist->GetRMS());      
    col("black");
  }

  return 0;
}



