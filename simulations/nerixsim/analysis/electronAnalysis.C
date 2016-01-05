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

using namespace std;

void electronAnalysis()
{
    
    //Load file and branches
    TFile * f = new TFile("11keVAnalysis5mil.root","read");
    TTree * t1 = (TTree*)f->Get("t1");
    TBranch *bEvent = t1->GetBranch("eventid");
    TBranch *bNSteps = t1->GetBranch("nsteps");
    TBranch *bTrack = t1->GetBranch("trackid");
    TBranch *bType = t1->GetBranch("type");
    TBranch *bVolumes = t1->GetBranch("volumes");
    TBranch *bCreaProc = t1->GetBranch("creaproc");
    TBranch *bEdProc = t1->GetBranch("edproc");
    TBranch *bTime = t1->GetBranch("time");
    TBranch *bETotLiqSci = t1->GetBranch("etotliqsci");
    TBranch *bETot = t1->GetBranch("etot");
    TBranch *bED = t1->GetBranch("ed");

    //Count the number of entries in the tree
    Long64_t nentries = t1->GetEntriesFast();
    //cout << nentries << endl;
    
    //Declare addresses for branches
    int event;
    int nsteps;
    vector<int> *track;
    vector<string> *type;
    vector<string> *volumes;
    vector<string> *creaproc;
    vector<string> *edproc;
    vector<float> *time;
    float etotliqsci;
    float etot;
    vector<float> *ed;
    
    //Set addresses for branches
    bEvent->SetAddress(&event);
    bNSteps->SetAddress(&nsteps);
    bTrack->SetAddress(&track);
    bType->SetAddress(&type);
    bVolumes->SetAddress(&volumes);
    bCreaProc->SetAddress(&creaproc);
    bEdProc->SetAddress(&edproc);
    bTime->SetAddress(&time);
    bED->SetAddress(&ed);
    bETotLiqSci->SetAddress(&etotliqsci);
    bETot->SetAddress(&etot);
    
    //Create canvas
    
    TCanvas * c1 = new TCanvas("etot","etot",200,10,1000,700);
    c1->SetTitle("Energy Deposited in LXe");
    c1->Divide(3,2);
    
    TCanvas * c2 = new TCanvas("escinttot","escinttot",200,10,1000,700);
    c2->SetTitle("Energy Deposited in Scintillator");
    c2->Divide(3,2);
    
    TCanvas * c3 = new TCanvas("time","time",200,10,1000,700);
    c3->SetTitle("Time Between First Collision and Last");
    c3->Divide(3,2);
    
    //Create histogram
    
    /*t1->Draw("etotliqsci>>height(500,2400,2550)","","goff");
    double integral = height->Integral();
    height->SetTitle("eDepLScint");
    height->GetXaxis()->SetTitle("Energy [keV]");
    height->GetYaxis()->SetTitle("Counts");
    height->GetYaxis()->SetTitleOffset(1.55);
    height->SetStats(0); 
    height->Draw();*/
    //height->Scale(1/(integral));
    
    TH1F *h1 = new TH1F("Pure Elastic Scatters","Pure Elastic Scatters",1000,0,100);
    TH1F *h2 = new TH1F("Multiple Elastic Scatters","Multiple Elastic Scatters",1000,0,100);
    TH1F *h3 = new TH1F("Energy Depostion in Other Volumes","Energy Depostion in Other Volumes",1000,0,100);
    TH1F *h4 = new TH1F("Inelastic Scatters","Inelastic Scatters",1000,0,100);
    TH1F *h5 = new TH1F("LXe without Lscint Elastic Scatters","LXe without Lscint Elastic Scatters",1000,0,100);
    TH1F *tot = new TH1F("Sum of Signal and Background","Sum of Signal and Background",1000,0,100);
    
    
    TH1F *hscint1 = new TH1F("Pure Elastic Scatters","Pure Elastic Scatters",1000,2450,2510);
    TH1F *hscint2 = new TH1F("Multiple Elastic Scatters","Multiple Elastic Scatters",1000,2450,2510);
    TH1F *hscint3 = new TH1F("Energy Depostion in Other Volumes","Energy Depostion in Other Volumes",1000,2450,2510);
    TH1F *hscint4 = new TH1F("Inelastic Scatters","Inelastic Scatters",1000,2450,2510);
    TH1F *hscint5 = new TH1F("LXe without Lscint Elastic Scatters","LXe without Lscint Elastic Scatters",1000,0,100);
    TH1F *scinttot = new TH1F("Sum of Signal and Background","Sum of Signal and Background",1000,2450,2510);
    
    
    TH1F *htime1 = new TH1F("Pure Elastic Scatters","Pure Elastic Scatters",200,0,20);
    TH1F *htime2 = new TH1F("Multiple Elastic Scatters","Multiple Elastic Scatters",200,0,20);
    TH1F *htime3 = new TH1F("Energy Depostion in Other Volumes","Energy Depostion in Other Volumes",200,0,20);
    TH1F *htime4 = new TH1F("Inelastic Scatters","Inelastic Scatters",200,0,20);
    TH1F *htime5 = new TH1F("LXe without Lscint Elastic Scatters","LXe without Lscint Elastic Scatters",1000,0,100);
    TH1F *timetot = new TH1F("Sum of Signal and Background","Sum of Signal and Background",200,0,20);
    
    
    
    
    //Set max on upcoming loop by looking at the max of eventid (not just nentries since some events are skipped)
    int nEvents = 0;
    
    for (int i = 0; i < nentries; i++) {
        bEvent->GetEntry(i);
        if (event > nEvents) {
            nEvents = event;
        }
    }
    
    //Input flux
    float flux = 1000; //neutrons/s/cm**2
    int numberOfNeutrons = nEvents;
    float area = 22.4168; //input area of isotropic "beam" (see calculations in notes and image file)
    float elapsedTime = numberOfNeutrons/(flux*area);
    
    

    
    //set max number of events
    //set to nEvents if you want all
    int maxEvents = nEvents;
    
    int q = 0;
    i = 0;
    int count = 1;
    float stepTime = 0;
    
    while (i < maxEvents) {
        
        bEvent->GetEntry(q);
        
        if (event == i) {
            
            bTrack->GetEntry(q); 
            bType->GetEntry(q);
            bCreaProc->GetEntry(q);
            bEdProc->GetEntry(q);
            bETotLiqSci->GetEntry(q);
            bTime->GetEntry(q);
            bVolumes->GetEntry(q);
            bED->GetEntry(q);
            bETot->GetEntry(q);
            
            if (volumes->size() > 0) {
                
                int markerLXe = 0;
                int markerLScint = 0;
                int markerOtherVolume = 0;
                
                for (int p = 0; p < volumes->size(); p++) {
                    if (volumes->at(p) == "LXe") {
                        markerLXe = 1;
                    }
                    if (volumes->at(p) == "LiquidScintillator") {
                        markerLScint = 1;
                    }
                    if (volumes->at(p) != "LXe" && volumes->at(p) != "LiquidScintillator") {
                        markerOtherVolume = 1;
                    }
                }
                
                int markerMultipleElastic = 0;
                int markerInelastic = 0;
                float firstTime = 0;
                float firstTimeMult = 0;
                float firstTimeOtherVolume = 0;
                float firstTimeInelastic = 0;
                float firstTimeElasticOnly = 0;
                float lastTime = 0;
                string prevProc = "";
                
                for (int w = 0; w < type->size(); w++) {
                    if (type->at(w) == "neutron") {
                        if (edproc->at(w) == prevProc && prevProc == "hadElastic") {
                            markerMultipleElastic = 1;
                            //check for multiple scatter (doesn't matter how many scatters)
                            //since transportation occurs between volumes we are guaranteed (as long as there is not
                            //an inelastic collision which we catch later) that there were >=2 elastic collisions
                            //in LXe (edproc is only for LXe if it doesn't read transportation)
                        }
                        
                        if (edproc == "inelastic" || edproc == "nCapture") {
                            markerInelastic = 1;
                            firstTimeInelastic = time->at(w);
                            //check for inelastic scatters or neutron captures
                        }
                        
                        //start time examination (except inelastic which is above)
                        
                        if (edproc->at(w) == "hadElastic" && markerLXe == 1 && markerLScint == 1 && firstTime == 0) {
                            //fills in time of first hit in LXe and LScint only and does not include multiple scatters
                            if (markerOtherVolume == 0) {
                                firstTime = time->at(w);
                            }
                            if (markerOtherVolume == 1) {
                                firstTimeOtherVolume = time->at(w);
                            }
                        }
                        
                        if (edproc->at(w) == "hadElastic" && markerLXe == 1 && markerLScint == 0 && firstTime == 0) {
                            firstTimeElasticOnly = time->at(w);
                        }
                        
                        if (firstTime != 0 || firstTimeMult != 0 || firstTimeInelastic != 0 || firstTimeOtherVolume != 0) {
                            float tempTime = time->at(w);
                            if (tempTime > lastTime) {
                                lastTime = tempTime;
                                //makes sure that you have last even if not saved in order
                            }
                        }
                        
                        prevProc = edproc->at(w);
                        
                    }
                }
                if (lastTime != 0 && firstTime != 0 && markerMultipleElastic == 0) {
                    htime1->Fill(lastTime - firstTime);
                    timetot->Fill(lastTime - firstTime);
                }
                if (lastTime != 0 && firstTime != 0 && markerMultipleElastic == 1) {
                    htime2->Fill(lastTime - firstTime);
                    timetot->Fill(lastTime - firstTime);
                }
                if (lastTime != 0 && firstTimeOtherVolume != 0 && markerOtherVolume == 1) {
                    htime3->Fill(lastTime - firstTimeOtherVolume);
                    timetot->Fill(lastTime - firstTimeOtherVolume);
                }
                if (lastTime != 0 && firstTimeInelastic != 0 && markerInelastic == 1) {
                    htime4->Fill(lastTime - firstTimeInelastic);
                    timetot->Fill(lastTime - firstTimeInelastic);
                }
                if (firstTimeElasticOnly != 0) {
                    htime5->Fill(firstTimeElasticOnly);
                    timetot->Fill(firstTimeElasticOnly);
                }
                if (markerLXe == 1 && markerLScint == 1 && markerMultipleElastic == 0 && markerOtherVolume == 0 && markerInelastic == 0) {
                    h1->Fill(etot);
                    tot->Fill(etot);
                    hscint1->Fill(etotliqsci);
                    scinttot->Fill(etotliqsci);
                }
                if (markerLXe == 1 && markerLScint == 1 && markerMultipleElastic == 1 && markerOtherVolume == 0 && markerInelastic == 0) {
                    h2->Fill(etot);
                    tot->Fill(etot);
                    hscint2->Fill(etotliqsci);
                    scinttot->Fill(etotliqsci);
                }
                if (markerLXe == 1 && markerLScint == 1 && markerOtherVolume == 1 && markerInelastic == 0) {
                    h3->Fill(etot);
                    tot->Fill(etot);
                    hscint3->Fill(etotliqsci);
                    scinttot->Fill(etotliqsci);
                }
                if (markerLXe == 1 && markerLScint == 1 && markerInelastic == 1) {
                    h4->Fill(etot);
                    tot->Fill(etot);
                    hscint4->Fill(etotliqsci);
                    scinttot->Fill(etotliqsci);
                }
                if (markerLXe == 1 && markerLScint == 0) {
                    h5->Fill(etot);
                    tot->Fill(etot);
                    hscint5->Fill(etotliqsci);
                    scinttot->Fill(etotliqsci);
                }
            }
            i++;
            q++;
        }
        else{
            i++;
        }
        if (i % 50000 == 0) {
            cout << "Entry " << i << " out of " << maxEvents << endl;
        }
    }
    
    c1->cd(1);
    h1->Draw();
    h1->Scale(1/(elapsedTime));
    c1->cd(2);
    h2->Draw();
    h2->Scale(1/(elapsedTime));
    c1->cd(3);
    h3->Draw();
    h3->Scale(1/(elapsedTime));
    c1->cd(4);
    h4->Draw();
    h4->Scale(1/(elapsedTime));
    c1->cd(5);
    h5->Draw();
    h5->Scale(1/(elapsedTime));
    c1->cd(6);
    tot->Draw();
    tot->Scale(1/(elapsedTime));
    
    c2->cd(1);
    hscint1->Draw();
    hscint1->Scale(1/(elapsedTime));
    c2->cd(2);
    hscint2->Draw();
    hscint2->Scale(1/(elapsedTime));
    c2->cd(3);
    hscint3->Draw();
    hscint3->Scale(1/(elapsedTime));
    c2->cd(4);
    hscint4->Draw();
    hscint4->Scale(1/(elapsedTime));
    c2->cd(5);
    hscint5->Draw();
    hscint5->Scale(1/(elapsedTime));
    c2->cd(6);
    scinttot->Draw();
    scinttot->Scale(1/(elapsedTime));
    
    c3->cd(1);
    htime1->Draw();
    htime1->Scale(1/(elapsedTime));
    c3->cd(2);
    htime2->Draw();
    htime2->Scale(1/(elapsedTime));
    c3->cd(3);
    htime3->Draw();
    htime3->Scale(1/(elapsedTime));
    c3->cd(4);
    htime4->Draw();
    htime4->Scale(1/(elapsedTime));
    c3->cd(5);
    htime5->Draw();
    htime5->Scale(1/(elapsedTime));
    c3->cd(6);
    timetot->Draw();
    timetot->Scale(1/(elapsedTime));

    //f->close();
    
    return;
}


