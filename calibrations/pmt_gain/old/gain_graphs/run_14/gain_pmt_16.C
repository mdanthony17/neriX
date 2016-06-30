{
//=========Macro generated from canvas: 7d0e6b77545f4a21bfb21c44865c0075/
//=========  (Fri Aug 28 16:04:35 2015) by ROOT version5.34/26
   TCanvas *7d0e6b77545f4a21bfb21c44865c0075 = new TCanvas("7d0e6b77545f4a21bfb21c44865c0075", "",10,45,704,527);
   7d0e6b77545f4a21bfb21c44865c0075->SetHighLightColor(2);
   7d0e6b77545f4a21bfb21c44865c0075->Range(1.440011e+09,1050000,1.440011e+09,2550000);
   7d0e6b77545f4a21bfb21c44865c0075->SetFillColor(0);
   7d0e6b77545f4a21bfb21c44865c0075->SetBorderMode(0);
   7d0e6b77545f4a21bfb21c44865c0075->SetBorderSize(2);
   7d0e6b77545f4a21bfb21c44865c0075->SetFrameBorderMode(0);
   7d0e6b77545f4a21bfb21c44865c0075->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(1);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 16 - Gain run_14");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.440011e+09,1743921);
   gre->SetPointError(0,0,28624.37);
   
   TH1F *Graph_Graph16 = new TH1F("Graph_Graph16","Channel 16 - Gain run_14",100,1.440011e+09,1.440011e+09);
   Graph_Graph16->SetMinimum(1200000);
   Graph_Graph16->SetMaximum(2400000);
   Graph_Graph16->SetDirectory(0);
   Graph_Graph16->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph16->SetLineColor(ci);
   Graph_Graph16->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph16->GetXaxis()->SetTimeFormat("%m-%d%F1970-01-01 00:00:00s0 GMT");
   Graph_Graph16->GetXaxis()->SetLabelFont(42);
   Graph_Graph16->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph16->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph16->GetXaxis()->SetTitleFont(42);
   Graph_Graph16->GetYaxis()->SetTitle("SPE gain [electrons]");
   Graph_Graph16->GetYaxis()->SetLabelFont(42);
   Graph_Graph16->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph16->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph16->GetYaxis()->SetTitleFont(42);
   Graph_Graph16->GetZaxis()->SetLabelFont(42);
   Graph_Graph16->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph16->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph16->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph16);
   
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2757143,0.94,0.7242857,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 16 - Gain run_14");
   pt->Draw();
   7d0e6b77545f4a21bfb21c44865c0075->Modified();
   7d0e6b77545f4a21bfb21c44865c0075->cd();
   7d0e6b77545f4a21bfb21c44865c0075->SetSelected(7d0e6b77545f4a21bfb21c44865c0075);
}
