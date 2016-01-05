{
//=========Macro generated from canvas: 820576092ebb4d0380bd12662bae3af3/
//=========  (Fri Aug 28 16:04:33 2015) by ROOT version5.34/26
   TCanvas *820576092ebb4d0380bd12662bae3af3 = new TCanvas("820576092ebb4d0380bd12662bae3af3", "",10,45,704,527);
   820576092ebb4d0380bd12662bae3af3->SetHighLightColor(2);
   820576092ebb4d0380bd12662bae3af3->Range(1.440011e+09,1050000,1.440011e+09,2550000);
   820576092ebb4d0380bd12662bae3af3->SetFillColor(0);
   820576092ebb4d0380bd12662bae3af3->SetBorderMode(0);
   820576092ebb4d0380bd12662bae3af3->SetBorderSize(2);
   820576092ebb4d0380bd12662bae3af3->SetFrameBorderMode(0);
   820576092ebb4d0380bd12662bae3af3->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(1);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 11 - Gain run_14");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.440011e+09,2104895);
   gre->SetPointError(0,0,26656.07);
   
   TH1F *Graph_Graph11 = new TH1F("Graph_Graph11","Channel 11 - Gain run_14",100,1.440011e+09,1.440011e+09);
   Graph_Graph11->SetMinimum(1200000);
   Graph_Graph11->SetMaximum(2400000);
   Graph_Graph11->SetDirectory(0);
   Graph_Graph11->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph11->SetLineColor(ci);
   Graph_Graph11->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph11->GetXaxis()->SetTimeFormat("%m-%d%F1970-01-01 00:00:00s0 GMT");
   Graph_Graph11->GetXaxis()->SetLabelFont(42);
   Graph_Graph11->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph11->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph11->GetXaxis()->SetTitleFont(42);
   Graph_Graph11->GetYaxis()->SetTitle("SPE gain [electrons]");
   Graph_Graph11->GetYaxis()->SetLabelFont(42);
   Graph_Graph11->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph11->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph11->GetYaxis()->SetTitleFont(42);
   Graph_Graph11->GetZaxis()->SetLabelFont(42);
   Graph_Graph11->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph11->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph11->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph11);
   
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2771428,0.94,0.7228572,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 11 - Gain run_14");
   pt->Draw();
   820576092ebb4d0380bd12662bae3af3->Modified();
   820576092ebb4d0380bd12662bae3af3->cd();
   820576092ebb4d0380bd12662bae3af3->SetSelected(820576092ebb4d0380bd12662bae3af3);
}
