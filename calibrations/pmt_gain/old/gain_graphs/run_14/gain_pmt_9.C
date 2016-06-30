{
//=========Macro generated from canvas: 74297b6229bc427289aa0e99781e2948/
//=========  (Fri Aug 28 16:04:32 2015) by ROOT version5.34/26
   TCanvas *74297b6229bc427289aa0e99781e2948 = new TCanvas("74297b6229bc427289aa0e99781e2948", "",10,45,704,527);
   74297b6229bc427289aa0e99781e2948->SetHighLightColor(2);
   74297b6229bc427289aa0e99781e2948->Range(1.440011e+09,1050000,1.440011e+09,2550000);
   74297b6229bc427289aa0e99781e2948->SetFillColor(0);
   74297b6229bc427289aa0e99781e2948->SetBorderMode(0);
   74297b6229bc427289aa0e99781e2948->SetBorderSize(2);
   74297b6229bc427289aa0e99781e2948->SetFrameBorderMode(0);
   74297b6229bc427289aa0e99781e2948->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(1);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 9 - Gain run_14");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.440011e+09,1981551);
   gre->SetPointError(0,0,20242.72);
   
   TH1F *Graph_Graph9 = new TH1F("Graph_Graph9","Channel 9 - Gain run_14",100,1.440011e+09,1.440011e+09);
   Graph_Graph9->SetMinimum(1200000);
   Graph_Graph9->SetMaximum(2400000);
   Graph_Graph9->SetDirectory(0);
   Graph_Graph9->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph9->SetLineColor(ci);
   Graph_Graph9->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph9->GetXaxis()->SetTimeFormat("%m-%d%F1970-01-01 00:00:00s0 GMT");
   Graph_Graph9->GetXaxis()->SetLabelFont(42);
   Graph_Graph9->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph9->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph9->GetXaxis()->SetTitleFont(42);
   Graph_Graph9->GetYaxis()->SetTitle("SPE gain [electrons]");
   Graph_Graph9->GetYaxis()->SetLabelFont(42);
   Graph_Graph9->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph9->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph9->GetYaxis()->SetTitleFont(42);
   Graph_Graph9->GetZaxis()->SetLabelFont(42);
   Graph_Graph9->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph9->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph9->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph9);
   
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2857143,0.94,0.7142857,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 9 - Gain run_14");
   pt->Draw();
   74297b6229bc427289aa0e99781e2948->Modified();
   74297b6229bc427289aa0e99781e2948->cd();
   74297b6229bc427289aa0e99781e2948->SetSelected(74297b6229bc427289aa0e99781e2948);
}
