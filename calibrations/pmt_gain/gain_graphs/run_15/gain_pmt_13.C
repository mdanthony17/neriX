{
//=========Macro generated from canvas: 7b8e4d3c41c94601aa419283e8cace66/
//=========  (Tue Oct 27 09:51:55 2015) by ROOT version5.34/26
   TCanvas *7b8e4d3c41c94601aa419283e8cace66 = new TCanvas("7b8e4d3c41c94601aa419283e8cace66", "",10,45,704,527);
   7b8e4d3c41c94601aa419283e8cace66->SetHighLightColor(2);
   7b8e4d3c41c94601aa419283e8cace66->Range(1.440143e+09,537500,1.447014e+09,2162500);
   7b8e4d3c41c94601aa419283e8cace66->SetFillColor(0);
   7b8e4d3c41c94601aa419283e8cace66->SetBorderMode(0);
   7b8e4d3c41c94601aa419283e8cace66->SetBorderSize(2);
   7b8e4d3c41c94601aa419283e8cace66->SetFrameBorderMode(0);
   7b8e4d3c41c94601aa419283e8cace66->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(8);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 13 - Gain run_15");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.441288e+09,1343229);
   gre->SetPointError(0,0,5552.466);
   gre->SetPoint(1,1.441805e+09,1282620);
   gre->SetPointError(1,0,12762.98);
   gre->SetPoint(2,1.44243e+09,1227748);
   gre->SetPointError(2,0,20134.89);
   gre->SetPoint(3,1.442847e+09,1213436);
   gre->SetPointError(3,0,24399.86);
   gre->SetPoint(4,1.443458e+09,1237273);
   gre->SetPointError(4,0,13653.84);
   gre->SetPoint(5,1.443971e+09,1411431);
   gre->SetPointError(5,0,17189.05);
   gre->SetPoint(6,1.444745e+09,1239520);
   gre->SetPointError(6,0,38840.29);
   gre->SetPoint(7,1.445869e+09,1195296);
   gre->SetPointError(7,0,14462.96);
   
   TH1F *Graph_Graph13 = new TH1F("Graph_Graph13","Channel 13 - Gain run_15",100,1.44083e+09,1.446327e+09);
   Graph_Graph13->SetMinimum(700000);
   Graph_Graph13->SetMaximum(2000000);
   Graph_Graph13->SetDirectory(0);
   Graph_Graph13->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph13->SetLineColor(ci);
   Graph_Graph13->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph13->GetXaxis()->SetTimeFormat("%m-%d%F1970-01-01 00:00:00s0 GMT");
   Graph_Graph13->GetXaxis()->SetLabelFont(42);
   Graph_Graph13->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph13->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph13->GetXaxis()->SetTitleFont(42);
   Graph_Graph13->GetYaxis()->SetTitle("SPE gain [electrons]");
   Graph_Graph13->GetYaxis()->SetLabelFont(42);
   Graph_Graph13->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph13->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph13->GetYaxis()->SetTitleFont(42);
   Graph_Graph13->GetZaxis()->SetLabelFont(42);
   Graph_Graph13->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph13->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph13->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph13);
   
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2757143,0.94,0.7242857,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 13 - Gain run_15");
   pt->Draw();
   7b8e4d3c41c94601aa419283e8cace66->Modified();
   7b8e4d3c41c94601aa419283e8cace66->cd();
   7b8e4d3c41c94601aa419283e8cace66->SetSelected(7b8e4d3c41c94601aa419283e8cace66);
}
