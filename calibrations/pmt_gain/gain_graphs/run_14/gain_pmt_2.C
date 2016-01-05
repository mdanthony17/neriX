{
//=========Macro generated from canvas: 2e7f770017354201a5865f61422ee6e1/
//=========  (Fri Aug 28 16:04:30 2015) by ROOT version5.34/26
   TCanvas *2e7f770017354201a5865f61422ee6e1 = new TCanvas("2e7f770017354201a5865f61422ee6e1", "",10,45,704,527);
   2e7f770017354201a5865f61422ee6e1->SetHighLightColor(2);
   2e7f770017354201a5865f61422ee6e1->Range(1.440011e+09,1050000,1.440011e+09,2550000);
   2e7f770017354201a5865f61422ee6e1->SetFillColor(0);
   2e7f770017354201a5865f61422ee6e1->SetBorderMode(0);
   2e7f770017354201a5865f61422ee6e1->SetBorderSize(2);
   2e7f770017354201a5865f61422ee6e1->SetFrameBorderMode(0);
   2e7f770017354201a5865f61422ee6e1->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(1);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 2 - Gain run_14");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.440011e+09,1597388);
   gre->SetPointError(0,0,18288.7);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Channel 2 - Gain run_14",100,1.440011e+09,1.440011e+09);
   Graph_Graph2->SetMinimum(1200000);
   Graph_Graph2->SetMaximum(2400000);
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
   TText *text = pt->AddText("Channel 2 - Gain run_14");
   pt->Draw();
   2e7f770017354201a5865f61422ee6e1->Modified();
   2e7f770017354201a5865f61422ee6e1->cd();
   2e7f770017354201a5865f61422ee6e1->SetSelected(2e7f770017354201a5865f61422ee6e1);
}
