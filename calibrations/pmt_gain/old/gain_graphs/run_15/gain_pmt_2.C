{
//=========Macro generated from canvas: 8ec5e2cd5f8549ba9a32b8a4695e232a/
//=========  (Tue Oct 27 09:51:53 2015) by ROOT version5.34/26
   TCanvas *8ec5e2cd5f8549ba9a32b8a4695e232a = new TCanvas("8ec5e2cd5f8549ba9a32b8a4695e232a", "",10,45,704,527);
   8ec5e2cd5f8549ba9a32b8a4695e232a->SetHighLightColor(2);
   8ec5e2cd5f8549ba9a32b8a4695e232a->Range(1.440143e+09,537500,1.447014e+09,2162500);
   8ec5e2cd5f8549ba9a32b8a4695e232a->SetFillColor(0);
   8ec5e2cd5f8549ba9a32b8a4695e232a->SetBorderMode(0);
   8ec5e2cd5f8549ba9a32b8a4695e232a->SetBorderSize(2);
   8ec5e2cd5f8549ba9a32b8a4695e232a->SetFrameBorderMode(0);
   8ec5e2cd5f8549ba9a32b8a4695e232a->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(8);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 2 - Gain run_15");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.441288e+09,1172492);
   gre->SetPointError(0,0,4867.624);
   gre->SetPoint(1,1.441805e+09,1205409);
   gre->SetPointError(1,0,4872.481);
   gre->SetPoint(2,1.44243e+09,1171363);
   gre->SetPointError(2,0,5040.572);
   gre->SetPoint(3,1.442847e+09,1166672);
   gre->SetPointError(3,0,7271.295);
   gre->SetPoint(4,1.443458e+09,1195919);
   gre->SetPointError(4,0,5015.434);
   gre->SetPoint(5,1.443971e+09,1180409);
   gre->SetPointError(5,0,5372.867);
   gre->SetPoint(6,1.444745e+09,1139838);
   gre->SetPointError(6,0,5324.753);
   gre->SetPoint(7,1.445869e+09,1148404);
   gre->SetPointError(7,0,5491.722);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Channel 2 - Gain run_15",100,1.44083e+09,1.446327e+09);
   Graph_Graph2->SetMinimum(700000);
   Graph_Graph2->SetMaximum(2000000);
   Graph_Graph2->SetDirectory(0);
   Graph_Graph2->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph2->SetLineColor(ci);
   Graph_Graph2->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph2->GetXaxis()->SetTimeFormat("%m-%d%F1970-01-01 00:00:00s0 GMT");
   Graph_Graph2->GetXaxis()->SetLabelFont(42);
   Graph_Graph2->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph2->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph2->GetXaxis()->SetTitleFont(42);
   Graph_Graph2->GetYaxis()->SetTitle("SPE gain [electrons]");
   Graph_Graph2->GetYaxis()->SetLabelFont(42);
   Graph_Graph2->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph2->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph2->GetYaxis()->SetTitleFont(42);
   Graph_Graph2->GetZaxis()->SetLabelFont(42);
   Graph_Graph2->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph2->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph2->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph2);
   
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2857143,0.94,0.7142857,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 2 - Gain run_15");
   pt->Draw();
   8ec5e2cd5f8549ba9a32b8a4695e232a->Modified();
   8ec5e2cd5f8549ba9a32b8a4695e232a->cd();
   8ec5e2cd5f8549ba9a32b8a4695e232a->SetSelected(8ec5e2cd5f8549ba9a32b8a4695e232a);
}
