{
//=========Macro generated from canvas: e8608783812f411483c2980b2f555286/
//=========  (Fri Aug 28 16:04:31 2015) by ROOT version5.34/26
   TCanvas *e8608783812f411483c2980b2f555286 = new TCanvas("e8608783812f411483c2980b2f555286", "",10,45,704,527);
   e8608783812f411483c2980b2f555286->SetHighLightColor(2);
   e8608783812f411483c2980b2f555286->Range(1.440011e+09,1050000,1.440011e+09,2550000);
   e8608783812f411483c2980b2f555286->SetFillColor(0);
   e8608783812f411483c2980b2f555286->SetBorderMode(0);
   e8608783812f411483c2980b2f555286->SetBorderSize(2);
   e8608783812f411483c2980b2f555286->SetFrameBorderMode(0);
   e8608783812f411483c2980b2f555286->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(1);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 5 - Gain run_14");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.440011e+09,2009440);
   gre->SetPointError(0,0,28656.23);
   
   TH1F *Graph_Graph5 = new TH1F("Graph_Graph5","Channel 5 - Gain run_14",100,1.440011e+09,1.440011e+09);
   Graph_Graph5->SetMinimum(1200000);
   Graph_Graph5->SetMaximum(2400000);
   Graph_Graph5->SetDirectory(0);
   Graph_Graph5->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph5->SetLineColor(ci);
   Graph_Graph5->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph5->GetXaxis()->SetTimeFormat("%m-%d%F1970-01-01 00:00:00s0 GMT");
   Graph_Graph5->GetXaxis()->SetLabelFont(42);
   Graph_Graph5->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph5->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph5->GetXaxis()->SetTitleFont(42);
   Graph_Graph5->GetYaxis()->SetTitle("SPE gain [electrons]");
   Graph_Graph5->GetYaxis()->SetLabelFont(42);
   Graph_Graph5->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph5->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph5->GetYaxis()->SetTitleFont(42);
   Graph_Graph5->GetZaxis()->SetLabelFont(42);
   Graph_Graph5->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph5->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph5->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph5);
   
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2857143,0.94,0.7142857,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 5 - Gain run_14");
   pt->Draw();
   e8608783812f411483c2980b2f555286->Modified();
   e8608783812f411483c2980b2f555286->cd();
   e8608783812f411483c2980b2f555286->SetSelected(e8608783812f411483c2980b2f555286);
}
