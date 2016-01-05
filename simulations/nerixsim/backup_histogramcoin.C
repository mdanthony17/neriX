#include "TH1.h"
#include "TList.h"
#include "TF1.h"
#include "TPave.h"
#include "TPaveText.h"
#include "TText.h"
#include "TAxis.h"
#include "TObject.h"
#include "TDirectory.h"
#include <TStyle.h>
#include <TCanvas.h>
#include "TApplication.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH2.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TBufferFile.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "include/lscint_settings.hh"

using namespace std;

void histogramCoincidenceAnalysis(double newBinSize = .01)
{
    TString fname = "rCoincidenceAnalysis" + energy_distance + ".root";
//    TString fname = "rCoincidenceAnalysis" + energy_distance + "_reduced.root";
    long int nevents = 3e6;
    int fit_bound_low = recoil_energy-1;
    int fit_bound_high = recoil_energy+1;
    
    //Load file and branches
    TFile * f1 = new TFile(fname, "read");
    
    //f1->ls();
    
    TH1F * h1 = (TH1F*)f1->Get("Pure Elastic Scatters");
    TH1F * h2 = (TH1*)f1->Get("Multiple Elastic Scatters");
    TH1F * h3 = (TH1*)f1->Get("Energy Depostion in Other Volumes");
    TH1F * h4 = (TH1*)f1->Get("Inelastic Scatters");
    TH1F * h5 = (TH1*)f1->Get("Chance Scatters");
    TH1F * tot = (TH1*)f1->Get("Sum of Signal and Background");
    
    //Create canvas
    
    TCanvas * c1 = new TCanvas("etot","etot",200,10,1000,700);
    c1->SetTitle("Energy Deposited in LXe");
    c1->Divide(3,2);
    
    TCanvas * c2 = new TCanvas("Total Energy Spectrum","Total Energy Spectrum",200,10,1000,700);
    c2->SetTitle("Energy Deposited in LXe for True and Accidental Scatters");
	
		
	//calculate scales required
	float totalRate = 1000000; //neutrons/s
    float rateLXe = totalRate*solid_angle_fraction_lxe; //solid angle/4*Pi  ----- need to change for each distance
    float rateLScint = 16*totalRate*solid_angle_fraction_lscint; //solid angle/4*Pi  ----- need to change for each distance
    float efficiencyLXe = .3; //approximate
    float efficiencyLScint = .849608;
    int numberOfNeutronsAtLXe = nevents; //need to take from coincidenceAnalysis
    float elapsedTime = numberOfNeutronsAtLXe/(rateLXe);//"beam" is incident on LXe (not isotropic)
	float elapsedTimeDays = elapsedTime * .000011574;//constant is days in a second
	
    
    //Coincidence Factor
    float combinedWidthNanoseconds = 10000;
    float combinedWidthSeconds = .000000001*combinedWidthNanoseconds;
    float coincidenceFactor = rateLScint*efficiencyLScint*combinedWidthSeconds;
    
    cout << "Elapsed Time: " << elapsedTime << " seconds" << endl;
	cout << "Elapsed Time: " << elapsedTimeDays << " days" << endl;
    cout << "Coincidence Factor: " << coincidenceFactor << endl;
	
    
    c1->cd(1);
    h1->SetTitle("True Elastic Scatters");
    h1->GetXaxis()->SetTitle("Energy [keV]");
    h1->GetYaxis()->SetTitle("Rate [events/(day)]");
    h1->GetYaxis()->SetTitleOffset(1.55);
    h1->SetStats(0);
    h1->Draw();
	h1->Scale(1/(elapsedTimeDays));
    
    c1->cd(2);
    h2->SetTitle("Multiple Elastic Scatters");
    h2->GetXaxis()->SetTitle("Energy [keV]");
    h2->GetYaxis()->SetTitle("Rate [events/(day)]");
    h2->GetYaxis()->SetTitleOffset(1.55);
    h2->SetStats(0);
    h2->Draw();
	h2->Scale(1/(elapsedTimeDays));
    
    c1->cd(3);
    h3->SetTitle("Elastic Scatters in LXe and Other Volumes");
    h3->GetXaxis()->SetTitle("Energy [keV]");
    h3->GetYaxis()->SetTitle("Rate [events/(day)]");
    h3->GetYaxis()->SetTitleOffset(1.55);
    h3->SetStats(0);
    h3->Draw();
	h3->Scale(1/(elapsedTimeDays));
    
    c1->cd(4);
    h4->SetTitle("Inelastic Scatters");
    h4->GetXaxis()->SetTitle("Energy [keV]");
    h4->GetYaxis()->SetTitle("Rate [events/(day)]");
    h4->GetYaxis()->SetTitleOffset(1.55);
    h4->SetStats(0);
    h4->Draw();
	h4->Scale(1/(elapsedTimeDays));
    
    c1->cd(5);
    h5->SetTitle("Accidental Coincidence Scatters");
    h5->GetXaxis()->SetTitle("Energy [keV]");
    h5->GetYaxis()->SetTitle("Rate [events/(day)]");
    h5->GetYaxis()->SetTitleOffset(1.55);
    h5->SetStats(0);
    h5->Draw();
	h5->Scale(coincidenceFactor/(elapsedTimeDays));
	
	tot->Add(h1);
    tot->Add(h2);
    tot->Add(h3);
    tot->Add(h4);
    tot->Add(h5);
    int binGrowthFactor = (int)((newBinSize/(tot->GetXaxis()->GetBinWidth(0)))+.5);
    tot->Rebin(binGrowthFactor);
    
    float hoursForThousandTrueScatters = 8 * 1000/((h1->Integral())/24);
	//8 accounts for the extra detectors
    
    cout << "True Coincidences per Day: " << (h1->Integral()) << endl;
    cout << "Accidental Coincidences per Day: " << (h5->Integral()) << endl;
    cout << "Hours per 1000 True Scatters: " << hoursForThousandTrueScatters << endl;
    
    c1->cd(6);
    tot->SetTitle("neriX Energy Spectrum");
    tot->GetXaxis()->SetTitle("Energy [keV]");
    tot->GetYaxis()->SetTitle("Rate [events/(day)]");
    tot->GetYaxis()->SetTitleOffset(1.55);
    tot->SetStats(0);
    tot->Draw();
    
    
    //Save image of spectrum
    c2->cd(0);
    tot->Draw();
    
    
    //-----
    //Fit Process
    
   //  //Preliminary Fit
    TF1 *testGaus = new TF1("testGaus", "gaus(0) + pol1(3)", fit_bound_low-1, fit_bound_high+1);
    testGaus->SetParameter(1, recoil_energy);
    testGaus->SetParameter(0, tot->GetMaximum());
    testGaus->SetParameter(2, .5);
    testGaus->SetParameter(4, -.1);
    testGaus->SetParameter(3, 9.0);
//     tot->Fit("testGaus", "Rm");
    Double_t testMean = testGaus->GetParameter(1);
    Double_t testWidth = testGaus->GetParameter(2);
    
    //Primary Fit
    TF1 *primFit = new TF1("primFit", "gaus(0) + pol1(3)", testMean - 2*testWidth, testMean + 2*testWidth);
    primFit->SetParameter(1, testGaus->GetParameter(1));
//     tot->Fit("primFit", "Rm");
	Double_t height = primFit->GetParameter(0);
    Double_t mean = primFit->GetParameter(1);
    Double_t width = primFit->GetParameter(2);
	
	//Draw Gaussian with parameters
	TF1 * totFit = new TF1("totFit", "gaus(0) + pol1(3)", fit_bound_low-1, fit_bound_high+1);
	totFit->SetParameter(0, height);
	totFit->SetParameter(1, mean);
	totFit->SetParameter(2, width);
	totFit->SetParameter(3, primFit->GetParameter(3));
	totFit->SetParameter(4, primFit->GetParameter(4));
	totFit->Draw("SAME");
    	cout << "Sigma/Mean = " << totFit->GetParameter(2)/totFit->GetParameter(1)<<endl;
	cout << "Linear Background slope,intercept = " << totFit->GetParameter(4) << "," << totFit->GetParameter(3) << endl;
    //-----
    
    float daysForThousandTrueScatters = hoursForThousandTrueScatters / 24;
    //float daysForThousandTrueScatters = 8 * (1000)/((h1->Integral(100 * (mean - width),100 * (mean + width))));
	//the eight accounts for the 16 detectors in the simulation
    //cout << daysForThousandTrueScatters << endl;
    
    TPaveText *box = new TPaveText(0.7,0.8,0.98,0.93,"brNDC");
    //box->SetFillColor(18);
    box->SetTextAlign(12);
    
    ostringstream strs1;
    strs1 << mean;
    string sPeak = "Peak: ";
    string strs2 = strs1.str();
    sPeak.append(strs2);
    sPeak.append(" keV");
    const char * cPeak = sPeak.c_str();
    
    ostringstream strs3;
    strs3 << width;
    string sWidth = "Width: ";
    string strs4 = strs3.str();
    sWidth.append(strs4);
    sWidth.append(" keV");
    const char * cWidth = sWidth.c_str();
    
    ostringstream strs5;
    strs5 << daysForThousandTrueScatters;
    string sHours = "Days per Thousand True Scatters: ";
    string strs6 = strs5.str();
    sHours.append(strs6);
    const char * cHours = sHours.c_str();
    
    box->AddText("Nerix Coincidence Analysis");
    box->AddText("");
    //box->AddText("511 keV");
    box->AddText(cPeak);
    box->AddText(cWidth);
    box->AddText(cHours);
    box->Draw();
    
    
    
     //TImage *img = TImage::Create();
     //img->FromPad(c2);
     //img->WriteImage("energySpectrum" + energy_distance + ".jpg");
   c2->Print("testsave.png"); 
    //delete h1;
    //delete h2;
    //delete h3;
    //delete h4;
    //delete h5;
    //delete tot;
    delete c1;
    //delete c2;
    
    //f1->Close();
    
    return;
}


