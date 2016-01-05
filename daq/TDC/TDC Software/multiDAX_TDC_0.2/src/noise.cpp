//-------------------------------------------------------------------
//  noise.cpp   --   DAX: DAQ for XENON100 
// 
//  the noise library contains functions to make a noise analysis
//
//  07.07.2009 MS  (07.07.09)
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
}

#include "global.h"
#include "CAENVMElib.h"
#include "control.h"
#include "adc.h"
#include "graph.h"

#include <TH1D.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TFile.h>
#include <TLatex.h>
#include <TGraph.h>

//-------------------------------------------------------------------
// noise analysis of  baseline
int noise_analysis(crate& crt, digitizer& adc, TApplication *theApp, TCanvas *noiseWin)	
{
  int blt_bytes; 
  int minval, val, pnt;
  int maxIteration=NOISE_ITERATIONS;		// number of iterations to get statistics
  double mean;  
  u_int32_t *base;

  char filename[100];
  TH1D *noisehist = new TH1D("noisehist","noisehist",200,-100,100);
  TH1F *hist[adc.NumberOfADCs][8];
  int samples=8192;             // lenght of waveforms in samples
  int piecesPerWaveform = (int)samples/NOISE_SAMPLESIZE;

  char msg[100];

  // allocate memory for buffer
  if ( (base = (u_int32_t *)malloc(adc.BufferSize)) == NULL) {  
    sprintf(msg,":::: ERROR: Can't allocate memory buffer of %d kB ::::", adc.BufferSize/1024);
    errormsg(0,msg);
    return -1;
  }
  
  // initialize noise histograms
  for (int i=0; i<adc.NumberOfADCs; i++) {
	for (int j=0; j<8; j++) {
	  sprintf(msg,"hist[%d][%d]",i,j);
	  hist[i][j] = new TH1F(msg,msg,60,0,60);
	  hist[i][j]->SetLineColor(4);
	  hist[i][j]->SetLineWidth(2);
	}
  }

  col("green"); 
  printf("\nNoise analysis started...\n"); 
  col("black");

  for (int iteration=0; iteration<maxIteration; iteration++) {
    printf("  *\n");
    usleep(200000);
    // enable adcs to get event
    adc_enable(crt,adc);

    // internal trigger
    u_int32_t data = 0x1;
    for (int i=0; i<adc.NumberOfADCs; i++) adc_writereg(crt,adc,i,SoftwareTrigger,data);  

    // disable adcs 
    adc_disable(crt,adc);		  
		  
    // read the event
    for (int i=0; i<adc.NumberOfADCs; i++) { // loop over all boards
      
	  // Read data from module i in MBLT mode into buff
      blt_bytes=adc_mblt(crt,adc,i,base);
      if (blt_bytes<0) { errormsg(0,":::: ERROR ::::"); return -1; }
      
	  // analyze Event and calculate Baseline
      if (base[0]&0xFFFFFFF!=0x8004) {
        col("red"); printf(":::: Problem with module %d ::::\n",i); col("black");
        errormsg(0,":::: ERROR: wrong Buffer Organization setting (expect 8192 samples) ::::");
        return -1;
      } 
    
      pnt=4;
      for (int channel=0; channel<8; channel++) {
        minval=16384; mean=0.;
        for (int j=0; j<piecesPerWaveform; j++) {
		  noisehist->Reset();	
		  for (int l=0; l<NOISE_SAMPLESIZE/2; l++) {	
            for (int k=0; k<2; k++) {
              if (k==0) val=base[pnt+j*NOISE_SAMPLESIZE/2+l]&0xFFFF;
	               else val=(base[pnt+j*NOISE_SAMPLESIZE/2+l]>>16)&0xFFFF;
	          noisehist->Fill((val-adc.baseline)*(-1.));	// approx. subtract baseline and invert waveform
		    }
		  }
		  // get max spike in this sample
          int max;
	      for (max=200; max>=100; max--) 
			if (noisehist->GetBinContent(max)>0) break;
		  hist[i][channel]->Fill(max-100-noisehist->GetMean()); 
        } 
        pnt=pnt+(int)(samples/2);		    
      
	  } // end: for (channel)          
    } // end: for loop over boards	
  } // end for(iteration)
    	  
  free(base);

  // display noise results ----------------------------------------
  noiseWin->cd();
  hist[0][0]->Draw();
  noiseWin->SetLogy();

  // create filename for pdf output  
  char timestr[200];
  char str2[40]; 
  control_gettimestring(timestr,'f');
  control_gettimestring(str2,'l');
  sprintf(filename,"noise_%s.pdf",timestr);
  
  // generate gif pics of histograms
  int m, c;
  float x[242], y[242];
  int maxval;
  col("green"); printf("Generate Noise Spectra and create pdf\n"); col("black");
  for (int pmtNum=1; pmtNum<=adc.pmtmax; pmtNum++) { 
    control_getadc(adc,pmtNum,m,c);
    sprintf(timestr,"Noise: Max. Peak Spectrum PMT %d",pmtNum);
	hist[m][c]->SetTitle(timestr);
	hist[m][c]->GetXaxis()->SetTitle("ADC Counts");
	hist[m][c]->GetYaxis()->SetTitle("a.u.");
    hist[m][c]->GetYaxis()->SetTitleOffset(1.2);
    // find highest noise peak
	for (maxval=60; maxval>=0; maxval--) 
	  if (hist[m][c]->GetBinContent(maxval)>0) break;
	x[pmtNum-1]=pmtNum;
	y[pmtNum-1]=maxval;

	hist[m][c]->Draw();

	sprintf(timestr,"xe100_noise%d.png",pmtNum);
    noiseWin->SaveAs(timestr);
  }  
  
  // generate summay histogram
  noiseWin->SetLogy(0);	// linear scale
  TGraph graphAll(242,x,y);
  graphAll.SetTitle("Maximum Noise Signal vs. PMT Number");
  graphAll.GetXaxis()->SetTitle("PMT");
  graphAll.GetYaxis()->SetTitle("Max. Noise Peak");
  graphAll.GetYaxis()->SetTitleOffset(1.2);
  graphAll.SetMarkerStyle(20);
  graphAll.SetMarkerColor(4);
  graphAll.Draw("ap"); 
  noiseWin->SaveAs("xe100_noiseall.png");

  // generate latex file
  FILE *nfile;
  nfile=fopen("tmp_latex.tex","w");
  fprintf(nfile,"\\documentclass[a4paper,10pt]{article}\n");
  fprintf(nfile,"\\usepackage{graphicx}\n");
  fprintf(nfile,"\\usepackage{a4wide}\n\n");
  fprintf(nfile,"\\begin{document}\n");
  fprintf(nfile,"\\section*{XENON100 Noise: %s}\n\n",str2);
  fprintf(nfile,".\\\\\n");
  for (int PmtNum=1; PmtNum<=242; PmtNum++) 
    fprintf(nfile,"\\includegraphics*[width=3.7cm]{xe100_noise%d.png}\n",PmtNum);
  fprintf(nfile,"\\section*{}\n");
  fprintf(nfile,"\\includegraphics*[width=8cm]{xe100_noiseall.png}\n");
  fprintf(nfile,"\\end{document}\n");
  fclose(nfile);

  // compile
  col("gray"); printf(".\n");
  system("pdflatex tmp_latex.tex");
  sprintf(timestr,"mv tmp_latex.pdf %s",filename);
  system(timestr);

  // clean up
  system("rm xe100_noise*.png");
  system("rm tmp_latex.*");

  // summary
  col("green"); printf("\n\n\n\n*** SUMMARY OF NOISE ANALYSIS ***\n\n"); col("black");
  printf("  noise spectra stored in %s\n",filename);
  printf("  PMTs with max. noise > 30:\n");
  int cnt=0;
  for (int i=0; i<=242; i++) {
     if (y[i]>30.) { 
	   printf("    PMT %3d: %.1f",(int)x[i],y[i]);
	   cnt++;
	 }
	 if (cnt==4) {cnt=0; printf("\n"); }
  }
  printf("\n");
  
  // delete histograms
  for (int i=0; i<adc.NumberOfADCs; i++) {
    for (int j=0; j<8; j++)
	  delete hist[i][j];
  }

  return 0;
}


