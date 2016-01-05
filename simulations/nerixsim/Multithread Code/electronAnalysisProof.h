//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jul 17 17:29:39 2013 by ROOT version 5.34/05
// from TTree t1/Tree containing event data for Nerix
// found on file: 11keVAnalysis-5mil.root
//////////////////////////////////////////////////////////

#ifndef electronAnalysisProof_h
#define electronAnalysisProof_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>

// Header file for the classes stored in the TTree if any.
#include <vector>
#include <vector>
#include <vector>
#include <vector>

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

using namespace std;

// Fixed size dimensions of array or collections stored in the TTree if any.

class electronAnalysisProof : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

    
    //variables used in my original code
    
    
    TFile * f;
    TTree * t1;
    int nEvents;
    int i;
    int maxEvents;
    int q;
    int count;
    float stepTime;
    int markerLXe;
    int markerLScint;
    int markerOtherVolume;
    int markerMultipleElastic;
    int markerInelastic;
    float firstTime;
    float firstTimeMult;
    float firstTimeOtherVolume;
    float firstTimeInelastic;
    float lastTime;
    string prevProc;
    int w;
    float tempTime;
    
    TCanvas * c1;
    TCanvas * c2;
    TCanvas * c3;
    
    TH1F *h1;
    TH1F *h2;
    TH1F *h3;
    TH1F *h4;
    TH1F *tot;
    
    TH1F *hscint1;
    TH1F *hscint2;
    TH1F *hscint3;
    TH1F *hscint4;
    TH1F *scinttot;
    
    TH1F *htime1;
    TH1F *htime2;
    TH1F *htime3;
    TH1F *htime4;
    TH1F *timetot;
    
    
    
    
    
   // Declaration of leaf types
   Int_t           eventid;
   vector<unsigned long> *enginestatus;
   Int_t           npmthits;
   vector<int>     *pmthits;
   vector<vector<float> > *pmthitsangles;
   vector<string>  *volumes;
   Float_t         etot;
   Int_t           nsteps;
   vector<int>     *trackid;
   vector<int>     *parentid;
   vector<string>  *type;
   vector<string>  *parenttype;
   vector<string>  *creaproc;
   vector<string>  *edproc;
   vector<float>   *xp;
   vector<float>   *yp;
   vector<float>   *zp;
   vector<float>   *ed;
   vector<float>   *ke;
   vector<float>   *time;
   Int_t           liqsciid;
   Float_t         etotliqsci;
   Int_t           trackidliqsci;
   vector<string>  *typeliqsci;
   Float_t         timeliqsci;
   Float_t         einliqsci;
   Float_t         eoutliqsci;
   Float_t         eotherliqsci;
   Int_t           geid;
   Float_t         etotge;
   Int_t           trackidge;
   vector<string>  *typege;
   Float_t         timege;
   Float_t         einge;
   Float_t         eoutge;
   Float_t         eotherge;
   vector<string>  *type_pri;
   Float_t         e_pri;
   Float_t         xp_pri;
   Float_t         yp_pri;
   Float_t         zp_pri;
   Float_t         ux_pri;
   Float_t         uy_pri;
   Float_t         uz_pri;

   // List of branches
   TBranch        *b_eventid;   //!
   TBranch        *b_enginestatus;   //!
   TBranch        *b_npmthits;   //!
   TBranch        *b_pmthits;   //!
   TBranch        *b_pmthitsangles;   //!
   TBranch        *b_volumes;   //!
   TBranch        *b_etot;   //!
   TBranch        *b_nsteps;   //!
   TBranch        *b_trackid;   //!
   TBranch        *b_parentid;   //!
   TBranch        *b_type;   //!
   TBranch        *b_parenttype;   //!
   TBranch        *b_creaproc;   //!
   TBranch        *b_edproc;   //!
   TBranch        *b_xp;   //!
   TBranch        *b_yp;   //!
   TBranch        *b_zp;   //!
   TBranch        *b_ed;   //!
   TBranch        *b_ke;   //!
   TBranch        *b_time;   //!
   TBranch        *b_liqsciid;   //!
   TBranch        *b_etotliqsci;   //!
   TBranch        *b_trackidliqsci;   //!
   TBranch        *b_typeliqsci;   //!
   TBranch        *b_timeliqsci;   //!
   TBranch        *b_einliqsci;   //!
   TBranch        *b_eoutliqsci;   //!
   TBranch        *b_eotherliqsci;   //!
   TBranch        *b_geid;   //!
   TBranch        *b_etotge;   //!
   TBranch        *b_trackidge;   //!
   TBranch        *b_typege;   //!
   TBranch        *b_timege;   //!
   TBranch        *b_einge;   //!
   TBranch        *b_eoutge;   //!
   TBranch        *b_eotherge;   //!
   TBranch        *b_type_pri;   //!
   TBranch        *b_e_pri;   //!
   TBranch        *b_xp_pri;   //!
   TBranch        *b_yp_pri;   //!
   TBranch        *b_zp_pri;   //!
   TBranch        *b_ux_pri;   //!
   TBranch        *b_uy_pri;   //!
   TBranch        *b_uz_pri;   //!

   electronAnalysisProof(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~electronAnalysisProof() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(electronAnalysisProof,0);
};

#endif

#ifdef electronAnalysisProof_cxx
void electronAnalysisProof::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   enginestatus = 0;
   pmthits = 0;
   pmthitsangles = 0;
   volumes = 0;
   trackid = 0;
   parentid = 0;
   type = 0;
   parenttype = 0;
   creaproc = 0;
   edproc = 0;
   xp = 0;
   yp = 0;
   zp = 0;
   ed = 0;
   ke = 0;
   time = 0;
   typeliqsci = 0;
   typege = 0;
   type_pri = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("eventid", &eventid, &b_eventid);
   fChain->SetBranchAddress("enginestatus", &enginestatus, &b_enginestatus);
   fChain->SetBranchAddress("npmthits", &npmthits, &b_npmthits);
   fChain->SetBranchAddress("pmthits", &pmthits, &b_pmthits);
   fChain->SetBranchAddress("pmthitsangles", &pmthitsangles, &b_pmthitsangles);
   fChain->SetBranchAddress("volumes", &volumes, &b_volumes);
   fChain->SetBranchAddress("etot", &etot, &b_etot);
   fChain->SetBranchAddress("nsteps", &nsteps, &b_nsteps);
   fChain->SetBranchAddress("trackid", &trackid, &b_trackid);
   fChain->SetBranchAddress("parentid", &parentid, &b_parentid);
   fChain->SetBranchAddress("type", &type, &b_type);
   fChain->SetBranchAddress("parenttype", &parenttype, &b_parenttype);
   fChain->SetBranchAddress("creaproc", &creaproc, &b_creaproc);
   fChain->SetBranchAddress("edproc", &edproc, &b_edproc);
   fChain->SetBranchAddress("xp", &xp, &b_xp);
   fChain->SetBranchAddress("yp", &yp, &b_yp);
   fChain->SetBranchAddress("zp", &zp, &b_zp);
   fChain->SetBranchAddress("ed", &ed, &b_ed);
   fChain->SetBranchAddress("ke", &ke, &b_ke);
   fChain->SetBranchAddress("time", &time, &b_time);
   fChain->SetBranchAddress("liqsciid", &liqsciid, &b_liqsciid);
   fChain->SetBranchAddress("etotliqsci", &etotliqsci, &b_etotliqsci);
   fChain->SetBranchAddress("trackidliqsci", &trackidliqsci, &b_trackidliqsci);
   fChain->SetBranchAddress("typeliqsci", &typeliqsci, &b_typeliqsci);
   fChain->SetBranchAddress("timeliqsci", &timeliqsci, &b_timeliqsci);
   fChain->SetBranchAddress("einliqsci", &einliqsci, &b_einliqsci);
   fChain->SetBranchAddress("eoutliqsci", &eoutliqsci, &b_eoutliqsci);
   fChain->SetBranchAddress("eotherliqsci", &eotherliqsci, &b_eotherliqsci);
   fChain->SetBranchAddress("geid", &geid, &b_geid);
   fChain->SetBranchAddress("etotge", &etotge, &b_etotge);
   fChain->SetBranchAddress("trackidge", &trackidge, &b_trackidge);
   fChain->SetBranchAddress("typege", &typege, &b_typege);
   fChain->SetBranchAddress("timege", &timege, &b_timege);
   fChain->SetBranchAddress("einge", &einge, &b_einge);
   fChain->SetBranchAddress("eoutge", &eoutge, &b_eoutge);
   fChain->SetBranchAddress("eotherge", &eotherge, &b_eotherge);
   fChain->SetBranchAddress("type_pri", &type_pri, &b_type_pri);
   fChain->SetBranchAddress("e_pri", &e_pri, &b_e_pri);
   fChain->SetBranchAddress("xp_pri", &xp_pri, &b_xp_pri);
   fChain->SetBranchAddress("yp_pri", &yp_pri, &b_yp_pri);
   fChain->SetBranchAddress("zp_pri", &zp_pri, &b_zp_pri);
   fChain->SetBranchAddress("ux_pri", &ux_pri, &b_ux_pri);
   fChain->SetBranchAddress("uy_pri", &uy_pri, &b_uy_pri);
   fChain->SetBranchAddress("uz_pri", &uz_pri, &b_uz_pri);
}

Bool_t electronAnalysisProof::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef electronAnalysisProof_cxx
