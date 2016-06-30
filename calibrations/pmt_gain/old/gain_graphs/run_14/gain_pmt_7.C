{
//=========Macro generated from canvas: e21ffec92b184368892522a8cd40e967/
//=========  (Fri Aug 28 16:04:32 2015) by ROOT version5.34/26
   TCanvas *e21ffec92b184368892522a8cd40e967 = new TCanvas("e21ffec92b184368892522a8cd40e967", "",10,45,704,527);
   e21ffec92b184368892522a8cd40e967->SetHighLightColor(2);
   e21ffec92b184368892522a8cd40e967->Range(1.440011e+09,1050000,1.440011e+09,2550000);
   e21ffec92b184368892522a8cd40e967->SetFillColor(0);
   e21ffec92b184368892522a8cd40e967->SetBorderMode(0);
   e21ffec92b184368892522a8cd40e967->SetBorderSize(2);
   e21ffec92b184368892522a8cd40e967->SetFrameBorderMode(0);
   e21ffec92b184368892522a8cd40e967->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(1);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 7 - Gain run_14");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.440011e+09,1874659);
   gre->SetPointError(0,0,35346.95);
   
   TH1F *Graph_Graph7 = new TH1F("Graph_Graph7","Channel 7 - Gain run_14",100,1.440011e+09,1.440011e+09);
   Graph_Graph7->SetMinimum(1200000);
   Graph_Graph7->SetMaximum(2400000);
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
   TText *text = pt->AddText("Channel 7 - Gain run_14");
   pt->Draw();
   e21ffec92b184368892522a8cd40e967->Modified();
   e21ffec92b184368892522a8cd40e967->cd();
   e21ffec92b184368892522a8cd40e967->SetSelected(e21ffec92b184368892522a8cd40e967);
}
