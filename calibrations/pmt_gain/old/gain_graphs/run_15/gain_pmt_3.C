{
//=========Macro generated from canvas: 223cef44a12a416ba407c65a21bb6160/
//=========  (Tue Oct 27 09:51:53 2015) by ROOT version5.34/26
   TCanvas *223cef44a12a416ba407c65a21bb6160 = new TCanvas("223cef44a12a416ba407c65a21bb6160", "",10,45,704,527);
   223cef44a12a416ba407c65a21bb6160->SetHighLightColor(2);
   223cef44a12a416ba407c65a21bb6160->Range(1.440143e+09,537500,1.447014e+09,2162500);
   223cef44a12a416ba407c65a21bb6160->SetFillColor(0);
   223cef44a12a416ba407c65a21bb6160->SetBorderMode(0);
   223cef44a12a416ba407c65a21bb6160->SetBorderSize(2);
   223cef44a12a416ba407c65a21bb6160->SetFrameBorderMode(0);
   223cef44a12a416ba407c65a21bb6160->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(8);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 3 - Gain run_15");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.441288e+09,1313240);
   gre->SetPointError(0,0,5724.61);
   gre->SetPoint(1,1.441805e+09,1306294);
   gre->SetPointError(1,0,5804.021);
   gre->SetPoint(2,1.44243e+09,1348254);
   gre->SetPointError(2,0,5067.442);
   gre->SetPoint(3,1.442847e+09,1141143);
   gre->SetPointError(3,0,97151.34);
   gre->SetPoint(4,1.443458e+09,1262585);
   gre->SetPointError(4,0,13808.31);
   gre->SetPoint(5,1.443971e+09,1319236);
   gre->SetPointError(5,0,5736.268);
   gre->SetPoint(6,1.444745e+09,1327570);
   gre->SetPointError(6,0,4964.473);
   gre->SetPoint(7,1.445869e+09,1275675);
   gre->SetPointError(7,0,5416.354);
   
   TH1F *Graph_Graph3 = new TH1F("Graph_Graph3","Channel 3 - Gain run_15",100,1.44083e+09,1.446327e+09);
   Graph_Graph3->SetMinimum(700000);
   Graph_Graph3->SetMaximum(2000000);
   Graph_Graph3->SetDirectory(0);
   Graph_Graph3->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph3->SetLineColor(ci);
   Graph_Graph3->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph3->GetXaxis()->SetTimeFormat("%m-%d%F1970-01-01 00:00:00s0 GMT");
   Graph_Graph3->GetXaxis()->SetLabelFont(42);
   Graph_Graph3->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph3->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph3->GetXaxis()->SetTitleFont(42);
   Graph_Graph3->GetYaxis()->SetTitle("SPE gain [electrons]");
   Graph_Graph3->GetYaxis()->SetLabelFont(42);
   Graph_Graph3->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph3->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph3->GetYaxis()->SetTitleFont(42);
   Graph_Graph3->GetZaxis()->SetLabelFont(42);
   Graph_Graph3->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph3->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph3->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph3);
   
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2857143,0.94,0.7142857,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 3 - Gain run_15");
   pt->Draw();
   223cef44a12a416ba407c65a21bb6160->Modified();
   223cef44a12a416ba407c65a21bb6160->cd();
   223cef44a12a416ba407c65a21bb6160->SetSelected(223cef44a12a416ba407c65a21bb6160);
}
