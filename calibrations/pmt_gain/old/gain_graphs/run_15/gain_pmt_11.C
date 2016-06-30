{
//=========Macro generated from canvas: 2752e7d8b75a4c9f8a809f9808e37b88/
//=========  (Tue Oct 27 09:51:55 2015) by ROOT version5.34/26
   TCanvas *2752e7d8b75a4c9f8a809f9808e37b88 = new TCanvas("2752e7d8b75a4c9f8a809f9808e37b88", "",10,45,704,527);
   2752e7d8b75a4c9f8a809f9808e37b88->SetHighLightColor(2);
   2752e7d8b75a4c9f8a809f9808e37b88->Range(1.440143e+09,537500,1.447014e+09,2162500);
   2752e7d8b75a4c9f8a809f9808e37b88->SetFillColor(0);
   2752e7d8b75a4c9f8a809f9808e37b88->SetBorderMode(0);
   2752e7d8b75a4c9f8a809f9808e37b88->SetBorderSize(2);
   2752e7d8b75a4c9f8a809f9808e37b88->SetFrameBorderMode(0);
   2752e7d8b75a4c9f8a809f9808e37b88->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(8);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 11 - Gain run_15");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.441288e+09,1497885);
   gre->SetPointError(0,0,13258.64);
   gre->SetPoint(1,1.441805e+09,1525214);
   gre->SetPointError(1,0,12658.59);
   gre->SetPoint(2,1.44243e+09,1509341);
   gre->SetPointError(2,0,13671.52);
   gre->SetPoint(3,1.442847e+09,1555305);
   gre->SetPointError(3,0,19411.95);
   gre->SetPoint(4,1.443458e+09,1486035);
   gre->SetPointError(4,0,16619.35);
   gre->SetPoint(5,1.443971e+09,1507851);
   gre->SetPointError(5,0,13747.38);
   gre->SetPoint(6,1.444745e+09,1485498);
   gre->SetPointError(6,0,15214.28);
   gre->SetPoint(7,1.445869e+09,1410752);
   gre->SetPointError(7,0,14038.77);
   
   TH1F *Graph_Graph11 = new TH1F("Graph_Graph11","Channel 11 - Gain run_15",100,1.44083e+09,1.446327e+09);
   Graph_Graph11->SetMinimum(700000);
   Graph_Graph11->SetMaximum(2000000);
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
   
   TPaveText *pt = new TPaveText(0.2771429,0.94,0.7228571,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 11 - Gain run_15");
   pt->Draw();
   2752e7d8b75a4c9f8a809f9808e37b88->Modified();
   2752e7d8b75a4c9f8a809f9808e37b88->cd();
   2752e7d8b75a4c9f8a809f9808e37b88->SetSelected(2752e7d8b75a4c9f8a809f9808e37b88);
}
