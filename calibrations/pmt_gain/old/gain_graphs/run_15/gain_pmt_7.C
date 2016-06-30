{
//=========Macro generated from canvas: d0bc452bd1ef414584a80e487dd353af/
//=========  (Tue Oct 27 09:51:54 2015) by ROOT version5.34/26
   TCanvas *d0bc452bd1ef414584a80e487dd353af = new TCanvas("d0bc452bd1ef414584a80e487dd353af", "",10,45,704,527);
   d0bc452bd1ef414584a80e487dd353af->SetHighLightColor(2);
   d0bc452bd1ef414584a80e487dd353af->Range(1.440143e+09,537500,1.447014e+09,2162500);
   d0bc452bd1ef414584a80e487dd353af->SetFillColor(0);
   d0bc452bd1ef414584a80e487dd353af->SetBorderMode(0);
   d0bc452bd1ef414584a80e487dd353af->SetBorderSize(2);
   d0bc452bd1ef414584a80e487dd353af->SetFrameBorderMode(0);
   d0bc452bd1ef414584a80e487dd353af->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(8);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 7 - Gain run_15");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.441288e+09,1460112);
   gre->SetPointError(0,0,7812.513);
   gre->SetPoint(1,1.441805e+09,1514902);
   gre->SetPointError(1,0,7547.759);
   gre->SetPoint(2,1.44243e+09,1439497);
   gre->SetPointError(2,0,20849.68);
   gre->SetPoint(3,1.442847e+09,1469189);
   gre->SetPointError(3,0,10868.62);
   gre->SetPoint(4,1.443458e+09,1467999);
   gre->SetPointError(4,0,7699.515);
   gre->SetPoint(5,1.443971e+09,1381124);
   gre->SetPointError(5,0,17011.65);
   gre->SetPoint(6,1.444745e+09,1324623);
   gre->SetPointError(6,0,16379.53);
   gre->SetPoint(7,1.445869e+09,1230191);
   gre->SetPointError(7,0,25082.11);
   
   TH1F *Graph_Graph7 = new TH1F("Graph_Graph7","Channel 7 - Gain run_15",100,1.44083e+09,1.446327e+09);
   Graph_Graph7->SetMinimum(700000);
   Graph_Graph7->SetMaximum(2000000);
   Graph_Graph7->SetDirectory(0);
   Graph_Graph7->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph7->SetLineColor(ci);
   Graph_Graph7->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph7->GetXaxis()->SetTimeFormat("%m-%d%F1970-01-01 00:00:00s0 GMT");
   Graph_Graph7->GetXaxis()->SetLabelFont(42);
   Graph_Graph7->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph7->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph7->GetXaxis()->SetTitleFont(42);
   Graph_Graph7->GetYaxis()->SetTitle("SPE gain [electrons]");
   Graph_Graph7->GetYaxis()->SetLabelFont(42);
   Graph_Graph7->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph7->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph7->GetYaxis()->SetTitleFont(42);
   Graph_Graph7->GetZaxis()->SetLabelFont(42);
   Graph_Graph7->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph7->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph7->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph7);
   
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2857143,0.94,0.7142857,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 7 - Gain run_15");
   pt->Draw();
   d0bc452bd1ef414584a80e487dd353af->Modified();
   d0bc452bd1ef414584a80e487dd353af->cd();
   d0bc452bd1ef414584a80e487dd353af->SetSelected(d0bc452bd1ef414584a80e487dd353af);
}
