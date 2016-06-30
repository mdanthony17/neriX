{
//=========Macro generated from canvas: 33cdb65f38bd4c8e89dbf1a50b2178e0/
//=========  (Fri Aug 28 16:04:34 2015) by ROOT version5.34/26
   TCanvas *33cdb65f38bd4c8e89dbf1a50b2178e0 = new TCanvas("33cdb65f38bd4c8e89dbf1a50b2178e0", "",10,45,704,527);
   33cdb65f38bd4c8e89dbf1a50b2178e0->SetHighLightColor(2);
   33cdb65f38bd4c8e89dbf1a50b2178e0->Range(1.440011e+09,1050000,1.440011e+09,2550000);
   33cdb65f38bd4c8e89dbf1a50b2178e0->SetFillColor(0);
   33cdb65f38bd4c8e89dbf1a50b2178e0->SetBorderMode(0);
   33cdb65f38bd4c8e89dbf1a50b2178e0->SetBorderSize(2);
   33cdb65f38bd4c8e89dbf1a50b2178e0->SetFrameBorderMode(0);
   33cdb65f38bd4c8e89dbf1a50b2178e0->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(1);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 13 - Gain run_14");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.440011e+09,1414780);
   gre->SetPointError(0,0,19409.16);
   
   TH1F *Graph_Graph13 = new TH1F("Graph_Graph13","Channel 13 - Gain run_14",100,1.440011e+09,1.440011e+09);
   Graph_Graph13->SetMinimum(1200000);
   Graph_Graph13->SetMaximum(2400000);
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
   TText *text = pt->AddText("Channel 13 - Gain run_14");
   pt->Draw();
   33cdb65f38bd4c8e89dbf1a50b2178e0->Modified();
   33cdb65f38bd4c8e89dbf1a50b2178e0->cd();
   33cdb65f38bd4c8e89dbf1a50b2178e0->SetSelected(33cdb65f38bd4c8e89dbf1a50b2178e0);
}
