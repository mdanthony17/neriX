{
//=========Macro generated from canvas: b938bedf2e2443129ef781859041d116/
//=========  (Fri Aug 28 16:04:34 2015) by ROOT version5.34/26
   TCanvas *b938bedf2e2443129ef781859041d116 = new TCanvas("b938bedf2e2443129ef781859041d116", "",10,45,704,527);
   b938bedf2e2443129ef781859041d116->SetHighLightColor(2);
   b938bedf2e2443129ef781859041d116->Range(1.440011e+09,1050000,1.440011e+09,2550000);
   b938bedf2e2443129ef781859041d116->SetFillColor(0);
   b938bedf2e2443129ef781859041d116->SetBorderMode(0);
   b938bedf2e2443129ef781859041d116->SetBorderSize(2);
   b938bedf2e2443129ef781859041d116->SetFrameBorderMode(0);
   b938bedf2e2443129ef781859041d116->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(1);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 12 - Gain run_14");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.440011e+09,1818077);
   gre->SetPointError(0,0,25086.82);
   
   TH1F *Graph_Graph12 = new TH1F("Graph_Graph12","Channel 12 - Gain run_14",100,1.440011e+09,1.440011e+09);
   Graph_Graph12->SetMinimum(1200000);
   Graph_Graph12->SetMaximum(2400000);
   Graph_Graph12->SetDirectory(0);
   Graph_Graph12->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph12->SetLineColor(ci);
   Graph_Graph12->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph12->GetXaxis()->SetTimeFormat("%m-%d%F1970-01-01 00:00:00s0 GMT");
   Graph_Graph12->GetXaxis()->SetLabelFont(42);
   Graph_Graph12->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph12->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph12->GetXaxis()->SetTitleFont(42);
   Graph_Graph12->GetYaxis()->SetTitle("SPE gain [electrons]");
   Graph_Graph12->GetYaxis()->SetLabelFont(42);
   Graph_Graph12->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph12->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph12->GetYaxis()->SetTitleFont(42);
   Graph_Graph12->GetZaxis()->SetLabelFont(42);
   Graph_Graph12->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph12->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph12->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph12);
   
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2757143,0.94,0.7242857,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 12 - Gain run_14");
   pt->Draw();
   b938bedf2e2443129ef781859041d116->Modified();
   b938bedf2e2443129ef781859041d116->cd();
   b938bedf2e2443129ef781859041d116->SetSelected(b938bedf2e2443129ef781859041d116);
}
