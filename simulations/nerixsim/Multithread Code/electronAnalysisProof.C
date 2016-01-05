#define electronAnalysisProof_cxx
// The class definition in electronAnalysisProof.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("electronAnalysisProof.C")
// Root > T->Process("electronAnalysisProof.C","some options")
// Root > T->Process("electronAnalysisProof.C+")
//

#include "electronAnalysisProof.h"
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


void electronAnalysisProof::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

    

}

void electronAnalysisProof::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
    
    f = new TFile("11keVAnalysis-5mil.root","read");
    t1 = (TTree*)f->Get("t1");
    
    Long64_t nentries = t1->GetEntriesFast();
    
    c1 = new TCanvas("etot","etot",200,10,1000,700);
    c1->SetTitle("Energy Deposited in LXe");
    c1->Divide(3,2);
    
    c2 = new TCanvas("escinttot","escinttot",200,10,1000,700);
    c2->SetTitle("Energy Deposited in Scintillator");
    c2->Divide(3,2);
    
    c3 = new TCanvas("time","time",200,10,1000,700);
    c3->SetTitle("Time Between First Collision and Last");
    c3->Divide(3,2);
    
    
    h1 = new TH1F("Pure Elastic Scatters","Pure Elastic Scatters",1000,0,100);
    h2 = new TH1F("Multiple Elastic Scatters","Multiple Elastic Scatters",1000,0,100);
    h3 = new TH1F("Energy Depostion in Other Volumes","Energy Depostion in Other Volumes",1000,0,100);
    h4 = new TH1F("Inelastic Scatters","Inelastic Scatters",1000,0,100);
    tot = new TH1F("Sum of Signal and Background","Sum of Signal and Background",1000,0,100);
    
    
    hscint1 = new TH1F("Pure Elastic Scatters","Pure Elastic Scatters",1000,2450,2510);
    hscint2 = new TH1F("Multiple Elastic Scatters","Multiple Elastic Scatters",1000,2450,2510);
    hscint3 = new TH1F("Energy Depostion in Other Volumes","Energy Depostion in Other Volumes",1000,2450,2510);
    hscint4 = new TH1F("Inelastic Scatters","Inelastic Scatters",1000,2450,2510);
    scinttot = new TH1F("Sum of Signal and Background","Sum of Signal and Background",1000,2450,2510);
    
    
    htime1 = new TH1F("Pure Elastic Scatters","Pure Elastic Scatters",200,0,20);
    htime2 = new TH1F("Multiple Elastic Scatters","Multiple Elastic Scatters",200,0,20);
    htime3 = new TH1F("Energy Depostion in Other Volumes","Energy Depostion in Other Volumes",200,0,20);
    htime4 = new TH1F("Inelastic Scatters","Inelastic Scatters",200,0,20);
    timetot = new TH1F("Sum of Signal and Background","Sum of Signal and Background",200,0,20);
    
    
    
    
    //Set max on upcoming loop by looking at the max of eventid (not just nentries since some events are skipped)
    nEvents = 0;
    
    for (i = 0; i < nentries; i++) {
        b_eventid->GetEntry(i);
        if (eventid > nEvents) {
            nEvents = eventid;
        }
    }
    
    
    //set max number of events
    //set to nEvents if you want all
    maxEvents = nEvents;
    


}

Bool_t electronAnalysisProof::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either electronAnalysisProof::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.
    
    
    
    
    
    
    
    q = 0;
    i = 0;
    count = 1;
    stepTime = 0;
    
    
    b_eventid->GetEntry(q);
    
    if (eventid == entry) {
        
        b_trackid->GetEntry(q);
        b_type->GetEntry(q);
        b_creaproc->GetEntry(q);
        b_edproc->GetEntry(q);
        b_etotliqsci->GetEntry(q);
        b_time->GetEntry(q);
        b_volumes->GetEntry(q);
        b_ed->GetEntry(q);
        b_etot->GetEntry(q);
        
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
            
            markerMultipleElastic = 0;
            markerInelastic = 0;
            firstTime = 0;
            firstTimeMult = 0;
            firstTimeOtherVolume = 0;
            firstTimeInelastic = 0;
            lastTime = 0;
            prevProc = "";
            
            for (w = 0; w < type->size(); w++) {
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
                    
                    if (firstTime != 0 || firstTimeMult != 0 || firstTimeInelastic != 0 || firstTimeOtherVolume != 0) {
                        tempTime = time->at(w);
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
        }
        entry++;
        q++;
    }
    else{
        entry++;
    }
    if (i % 50000 == 0) {
        cout << "Entry " << entry << " out of " << maxEvents << endl;
    }


   return kTRUE;
}

void electronAnalysisProof::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
    
    
    
    
    

}

void electronAnalysisProof::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
    
    
    
    
    c1->cd(1);
    h1->Draw();
    c1->cd(2);
    h2->Draw();
    c1->cd(3);
    h3->Draw();
    c1->cd(4);
    h4->Draw();
    c1->cd(5);
    tot->Draw();
    
    c2->cd(1);
    hscint1->Draw();
    c2->cd(2);
    hscint2->Draw();
    c2->cd(3);
    hscint3->Draw();
    c2->cd(4);
    hscint4->Draw();
    c2->cd(5);
    scinttot->Draw();
    
    c3->cd(1);
    htime1->Draw();
    c3->cd(2);
    htime2->Draw();
    c3->cd(3);
    htime3->Draw();
    c3->cd(4);
    htime4->Draw();
    c3->cd(5);
    timetot->Draw();

}
