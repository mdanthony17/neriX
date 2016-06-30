//To run, first load with [].L (macro name, i.e. of this file).C 
//then call []ReadFile("path to data, including extension")

void ReadFile(TString file) {
 gROOT->SetStyle("Plain");
 TCanvas *canvas = new TCanvas("canvas", "", 800, 600);
 //canvas->SetLogy();

TTree *tree = new TTree();
tree->ReadFile(file.Data(), "w:x");


 cout << endl <<endl <<file<<endl<<endl;

//this is configured for reading in tab-delimited data in four columns
 float w, x, y, z;
 tree->SetBranchAddress("w", &w);
 tree->SetBranchAddress("x", &x);
// tree->SetBranchAddress("y", &y);
// tree->SetBranchAddress("z", &z);

 TH1F *histo = new TH1F("Stats", "Histogram of...", 400, -20, 55000);
 histo->GetXaxis()->SetTitle("");
 histo->GetXaxis()->CenterTitle(true);
 histo->GetYaxis()->SetTitle("");
 histo->GetYaxis()->CenterTitle(true);

 int imax = tree->GetEntries();

 cout<< imax<<endl<<endl;
 
//note that axes can be rescaled as they are filled, if desired
 for (int i=0; i<imax; i++) {
   tree->GetEntry(i);
   histo->Fill(w,x);
  // cout << i << "\t x=" << x << "\ty=" << y << endl;
 }
 histo->Draw();

//  tree->GetEntry(tree->GetEntries()-1);
//  TH2F *frame = new TH2F("frame", ";x;y", 10, 0, x, 10, 0, y);
//  frame->Draw();
//  graph->Draw("pl same");

//  canvas->Modified();
//  canvas->Update();
}

