{
//=========Macro generated from canvas: daa6d70d2c4c437fa4d0c0e6e2aa7b13/
//=========  (Tue Oct 27 09:51:54 2015) by ROOT version5.34/26
   TCanvas *daa6d70d2c4c437fa4d0c0e6e2aa7b13 = new TCanvas("daa6d70d2c4c437fa4d0c0e6e2aa7b13", "",10,45,704,527);
   daa6d70d2c4c437fa4d0c0e6e2aa7b13->SetHighLightColor(2);
   daa6d70d2c4c437fa4d0c0e6e2aa7b13->Range(1.440143e+09,537500,1.447014e+09,2162500);
   daa6d70d2c4c437fa4d0c0e6e2aa7b13->SetFillColor(0);
   daa6d70d2c4c437fa4d0c0e6e2aa7b13->SetBorderMode(0);
   daa6d70d2c4c437fa4d0c0e6e2aa7b13->SetBorderSize(2);
   daa6d70d2c4c437fa4d0c0e6e2aa7b13->SetFrameBorderMode(0);
   daa6d70d2c4c437fa4d0c0e6e2aa7b13->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(8);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 8 - Gain run_15");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.441288e+09,1045628);
   gre->SetPointError(0,0,30410.31);
   gre->SetPoint(1,1.441805e+09,1203916);
   gre->SetPointError(1,0,13818.08);
   gre->SetPoint(2,1.44243e+09,1154642);
   gre->SetPointError(2,0,12577.73);
   gre->SetPoint(3,1.442847e+09,1113748);
   gre->SetPointError(3,0,30839);
   gre->SetPoint(4,1.443458e+09,1160050);
   gre->SetPointError(4,0,48294.2);
   gre->SetPoint(5,1.443971e+09,1302370);
   gre->SetPointError(5,0,7950.019);
   gre->SetPoint(6,1.444745e+09,1170365);
   gre->SetPointError(6,0,15516.8);
   gre->SetPoint(7,1.445869e+09,1130533);
   gre->SetPointError(7,0,38781.59);
   
   TH1F *Graph_Graph8 = new TH1F("Graph_Graph8","Channel 8 - Gain run_15",100,1.44083e+09,1.446327e+09);
   Graph_Graph8->SetMinimum(700000);
   Graph_Graph8->SetMaximum(2000000);
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
   TText *text = pt->AddText("Channel 8 - Gain run_15");
   pt->Draw();
   daa6d70d2c4c437fa4d0c0e6e2aa7b13->Modified();
   daa6d70d2c4c437fa4d0c0e6e2aa7b13->cd();
   daa6d70d2c4c437fa4d0c0e6e2aa7b13->SetSelected(daa6d70d2c4c437fa4d0c0e6e2aa7b13);
}
