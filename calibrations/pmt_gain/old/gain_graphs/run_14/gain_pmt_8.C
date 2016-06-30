{
//=========Macro generated from canvas: b1cfd997ee4b48709da31e695da30bba/
//=========  (Fri Aug 28 16:04:32 2015) by ROOT version5.34/26
   TCanvas *b1cfd997ee4b48709da31e695da30bba = new TCanvas("b1cfd997ee4b48709da31e695da30bba", "",10,45,704,527);
   b1cfd997ee4b48709da31e695da30bba->SetHighLightColor(2);
   b1cfd997ee4b48709da31e695da30bba->Range(1.440011e+09,1050000,1.440011e+09,2550000);
   b1cfd997ee4b48709da31e695da30bba->SetFillColor(0);
   b1cfd997ee4b48709da31e695da30bba->SetBorderMode(0);
   b1cfd997ee4b48709da31e695da30bba->SetBorderSize(2);
   b1cfd997ee4b48709da31e695da30bba->SetFrameBorderMode(0);
   b1cfd997ee4b48709da31e695da30bba->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(1);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 8 - Gain run_14");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.440011e+09,1461432);
   gre->SetPointError(0,0,32317.93);
   
   TH1F *Graph_Graph8 = new TH1F("Graph_Graph8","Channel 8 - Gain run_14",100,1.440011e+09,1.440011e+09);
   Graph_Graph8->SetMinimum(1200000);
   Graph_Graph8->SetMaximum(2400000);
   Graph_Graph8->SetDirectory(0);
   Graph_Graph8->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph8->SetLineColor(ci);
   Graph_Graph8->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph8->GetXaxis()->SetTimeFormat("%m-%d%F1970-01-01 00:00:00s0 GMT");
   Graph_Graph8->GetXaxis()->SetLabelFont(42);
   Graph_Graph8->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph8->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph8->GetXaxis()->SetTitleFont(42);
   Graph_Graph8->GetYaxis()->SetTitle("SPE gain [electrons]");
   Graph_Graph8->GetYaxis()->SetLabelFont(42);
   Graph_Graph8->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph8->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph8->GetYaxis()->SetTitleFont(42);
   Graph_Graph8->GetZaxis()->SetLabelFont(42);
   Graph_Graph8->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph8->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph8->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph8);
   
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2857143,0.94,0.7142857,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 8 - Gain run_14");
   pt->Draw();
   b1cfd997ee4b48709da31e695da30bba->Modified();
   b1cfd997ee4b48709da31e695da30bba->cd();
   b1cfd997ee4b48709da31e695da30bba->SetSelected(b1cfd997ee4b48709da31e695da30bba);
}
