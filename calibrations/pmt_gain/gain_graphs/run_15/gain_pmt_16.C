{
//=========Macro generated from canvas: 52b7fb20eed1479a9281e5c0ae91a671/
//=========  (Tue Oct 27 09:51:56 2015) by ROOT version5.34/26
   TCanvas *52b7fb20eed1479a9281e5c0ae91a671 = new TCanvas("52b7fb20eed1479a9281e5c0ae91a671", "",10,45,704,527);
   52b7fb20eed1479a9281e5c0ae91a671->SetHighLightColor(2);
   52b7fb20eed1479a9281e5c0ae91a671->Range(1.440143e+09,537500,1.447014e+09,2162500);
   52b7fb20eed1479a9281e5c0ae91a671->SetFillColor(0);
   52b7fb20eed1479a9281e5c0ae91a671->SetBorderMode(0);
   52b7fb20eed1479a9281e5c0ae91a671->SetBorderSize(2);
   52b7fb20eed1479a9281e5c0ae91a671->SetFrameBorderMode(0);
   52b7fb20eed1479a9281e5c0ae91a671->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(8);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 16 - Gain run_15");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.441288e+09,1475594);
   gre->SetPointError(0,0,11777.33);
   gre->SetPoint(1,1.441805e+09,1532699);
   gre->SetPointError(1,0,8548.442);
   gre->SetPoint(2,1.44243e+09,1459284);
   gre->SetPointError(2,0,9518.571);
   gre->SetPoint(3,1.442847e+09,1464726);
   gre->SetPointError(3,0,11357);
   gre->SetPoint(4,1.443458e+09,1515454);
   gre->SetPointError(4,0,7963.859);
   gre->SetPoint(5,1.443971e+09,1494234);
   gre->SetPointError(5,0,9452.138);
   gre->SetPoint(6,1.444745e+09,1473648);
   gre->SetPointError(6,0,9312.451);
   gre->SetPoint(7,1.445869e+09,1489922);
   gre->SetPointError(7,0,9391.563);
   
   TH1F *Graph_Graph16 = new TH1F("Graph_Graph16","Channel 16 - Gain run_15",100,1.44083e+09,1.446327e+09);
   Graph_Graph16->SetMinimum(700000);
   Graph_Graph16->SetMaximum(2000000);
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
   TText *text = pt->AddText("Channel 16 - Gain run_15");
   pt->Draw();
   52b7fb20eed1479a9281e5c0ae91a671->Modified();
   52b7fb20eed1479a9281e5c0ae91a671->cd();
   52b7fb20eed1479a9281e5c0ae91a671->SetSelected(52b7fb20eed1479a9281e5c0ae91a671);
}
