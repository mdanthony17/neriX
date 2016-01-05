{
//=========Macro generated from canvas: 2eb963832827430a93f3c66717dbb71e/
//=========  (Fri Aug 28 16:04:33 2015) by ROOT version5.34/26
   TCanvas *2eb963832827430a93f3c66717dbb71e = new TCanvas("2eb963832827430a93f3c66717dbb71e", "",10,45,704,527);
   2eb963832827430a93f3c66717dbb71e->SetHighLightColor(2);
   2eb963832827430a93f3c66717dbb71e->Range(1.440011e+09,1050000,1.440011e+09,2550000);
   2eb963832827430a93f3c66717dbb71e->SetFillColor(0);
   2eb963832827430a93f3c66717dbb71e->SetBorderMode(0);
   2eb963832827430a93f3c66717dbb71e->SetBorderSize(2);
   2eb963832827430a93f3c66717dbb71e->SetFrameBorderMode(0);
   2eb963832827430a93f3c66717dbb71e->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(1);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 10 - Gain run_14");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.440011e+09,2036150);
   gre->SetPointError(0,0,23061.17);
   
   TH1F *Graph_Graph10 = new TH1F("Graph_Graph10","Channel 10 - Gain run_14",100,1.440011e+09,1.440011e+09);
   Graph_Graph10->SetMinimum(1200000);
   Graph_Graph10->SetMaximum(2400000);
   Graph_Graph10->SetDirectory(0);
   Graph_Graph10->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph10->SetLineColor(ci);
   Graph_Graph10->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph10->GetXaxis()->SetTimeFormat("%m-%d%F1970-01-01 00:00:00s0 GMT");
   Graph_Graph10->GetXaxis()->SetLabelFont(42);
   Graph_Graph10->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph10->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph10->GetXaxis()->SetTitleFont(42);
   Graph_Graph10->GetYaxis()->SetTitle("SPE gain [electrons]");
   Graph_Graph10->GetYaxis()->SetLabelFont(42);
   Graph_Graph10->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph10->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph10->GetYaxis()->SetTitleFont(42);
   Graph_Graph10->GetZaxis()->SetLabelFont(42);
   Graph_Graph10->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph10->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph10->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph10);
   
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2757143,0.94,0.7242857,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 10 - Gain run_14");
   pt->Draw();
   2eb963832827430a93f3c66717dbb71e->Modified();
   2eb963832827430a93f3c66717dbb71e->cd();
   2eb963832827430a93f3c66717dbb71e->SetSelected(2eb963832827430a93f3c66717dbb71e);
}
