{
//=========Macro generated from canvas: e43f2671c1da473eb1f2471e4bec0a0f/
//=========  (Tue Oct 27 09:51:55 2015) by ROOT version5.34/26
   TCanvas *e43f2671c1da473eb1f2471e4bec0a0f = new TCanvas("e43f2671c1da473eb1f2471e4bec0a0f", "",10,45,704,527);
   e43f2671c1da473eb1f2471e4bec0a0f->SetHighLightColor(2);
   e43f2671c1da473eb1f2471e4bec0a0f->Range(1.440143e+09,537500,1.447014e+09,2162500);
   e43f2671c1da473eb1f2471e4bec0a0f->SetFillColor(0);
   e43f2671c1da473eb1f2471e4bec0a0f->SetBorderMode(0);
   e43f2671c1da473eb1f2471e4bec0a0f->SetBorderSize(2);
   e43f2671c1da473eb1f2471e4bec0a0f->SetFrameBorderMode(0);
   e43f2671c1da473eb1f2471e4bec0a0f->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(8);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 10 - Gain run_15");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.441288e+09,1479767);
   gre->SetPointError(0,0,7083.005);
   gre->SetPoint(1,1.441805e+09,1537649);
   gre->SetPointError(1,0,6552.648);
   gre->SetPoint(2,1.44243e+09,1495716);
   gre->SetPointError(2,0,6645.821);
   gre->SetPoint(3,1.442847e+09,1466490);
   gre->SetPointError(3,0,9360.151);
   gre->SetPoint(4,1.443458e+09,1360348);
   gre->SetPointError(4,0,7273.738);
   gre->SetPoint(5,1.443971e+09,1439255);
   gre->SetPointError(5,0,6967.691);
   gre->SetPoint(6,1.444745e+09,1403975);
   gre->SetPointError(6,0,6443.322);
   gre->SetPoint(7,1.445869e+09,1343960);
   gre->SetPointError(7,0,6647.212);
   
   TH1F *Graph_Graph10 = new TH1F("Graph_Graph10","Channel 10 - Gain run_15",100,1.44083e+09,1.446327e+09);
   Graph_Graph10->SetMinimum(700000);
   Graph_Graph10->SetMaximum(2000000);
   Graph_Graph10->SetDirectory(0);
   Graph_Graph10->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph10->SetLineColor(ci);
   Graph_Graph10->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph10->GetXaxis()->SetTimeFormat("%m-%d%F1970-01-01 00:00:00s0 GMT");
   Graph_Graph10->GetXaxis()->SetLabelFont(42);
   Graph_Graph10->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph10->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph10->GetXaxis()->SetTitleFont(42);
   Graph_Graph10->GetYaxis()->SetTitle("SPE gain [electrons]");
   Graph_Graph10->GetYaxis()->SetLabelFont(42);
   Graph_Graph10->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph10->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph10->GetYaxis()->SetTitleFont(42);
   Graph_Graph10->GetZaxis()->SetLabelFont(42);
   Graph_Graph10->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph10->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph10->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph10);
   
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2757143,0.94,0.7242857,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 10 - Gain run_15");
   pt->Draw();
   e43f2671c1da473eb1f2471e4bec0a0f->Modified();
   e43f2671c1da473eb1f2471e4bec0a0f->cd();
   e43f2671c1da473eb1f2471e4bec0a0f->SetSelected(e43f2671c1da473eb1f2471e4bec0a0f);
}
