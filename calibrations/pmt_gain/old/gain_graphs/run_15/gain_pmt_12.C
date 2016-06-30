{
//=========Macro generated from canvas: 16a9722b4add4e08b487f50eadec66d4/
//=========  (Tue Oct 27 09:51:55 2015) by ROOT version5.34/26
   TCanvas *16a9722b4add4e08b487f50eadec66d4 = new TCanvas("16a9722b4add4e08b487f50eadec66d4", "",10,45,704,527);
   16a9722b4add4e08b487f50eadec66d4->SetHighLightColor(2);
   16a9722b4add4e08b487f50eadec66d4->Range(1.440143e+09,537500,1.447014e+09,2162500);
   16a9722b4add4e08b487f50eadec66d4->SetFillColor(0);
   16a9722b4add4e08b487f50eadec66d4->SetBorderMode(0);
   16a9722b4add4e08b487f50eadec66d4->SetBorderSize(2);
   16a9722b4add4e08b487f50eadec66d4->SetFrameBorderMode(0);
   16a9722b4add4e08b487f50eadec66d4->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(8);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 12 - Gain run_15");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.441288e+09,1409912);
   gre->SetPointError(0,0,6156.799);
   gre->SetPoint(1,1.441805e+09,1412391);
   gre->SetPointError(1,0,5973.095);
   gre->SetPoint(2,1.44243e+09,1415667);
   gre->SetPointError(2,0,5707.191);
   gre->SetPoint(3,1.442847e+09,1434543);
   gre->SetPointError(3,0,8227.446);
   gre->SetPoint(4,1.443458e+09,1326417);
   gre->SetPointError(4,0,35415.1);
   gre->SetPoint(5,1.443971e+09,1338133);
   gre->SetPointError(5,0,6283.589);
   gre->SetPoint(6,1.444745e+09,1306249);
   gre->SetPointError(6,0,12724.22);
   gre->SetPoint(7,1.445869e+09,1327783);
   gre->SetPointError(7,0,6002.341);
   
   TH1F *Graph_Graph12 = new TH1F("Graph_Graph12","Channel 12 - Gain run_15",100,1.44083e+09,1.446327e+09);
   Graph_Graph12->SetMinimum(700000);
   Graph_Graph12->SetMaximum(2000000);
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
   TText *text = pt->AddText("Channel 12 - Gain run_15");
   pt->Draw();
   16a9722b4add4e08b487f50eadec66d4->Modified();
   16a9722b4add4e08b487f50eadec66d4->cd();
   16a9722b4add4e08b487f50eadec66d4->SetSelected(16a9722b4add4e08b487f50eadec66d4);
}
