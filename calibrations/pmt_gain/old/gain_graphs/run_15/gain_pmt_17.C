{
//=========Macro generated from canvas: ed213bafb4e64d4caa004ad3bb87e121/
//=========  (Tue Oct 27 09:51:56 2015) by ROOT version5.34/26
   TCanvas *ed213bafb4e64d4caa004ad3bb87e121 = new TCanvas("ed213bafb4e64d4caa004ad3bb87e121", "",10,45,704,527);
   ed213bafb4e64d4caa004ad3bb87e121->SetHighLightColor(2);
   ed213bafb4e64d4caa004ad3bb87e121->Range(1.440255e+09,537500,1.446992e+09,2162500);
   ed213bafb4e64d4caa004ad3bb87e121->SetFillColor(0);
   ed213bafb4e64d4caa004ad3bb87e121->SetBorderMode(0);
   ed213bafb4e64d4caa004ad3bb87e121->SetBorderSize(2);
   ed213bafb4e64d4caa004ad3bb87e121->SetFrameBorderMode(0);
   ed213bafb4e64d4caa004ad3bb87e121->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(10);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 17 - Gain run_15");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.441378e+09,1415999);
   gre->SetPointError(0,0,46496.26);
   gre->SetPoint(1,1.441805e+09,1476518);
   gre->SetPointError(1,0,44038.3);
   gre->SetPoint(2,1.44243e+09,1436778);
   gre->SetPointError(2,0,52003.66);
   gre->SetPoint(3,1.442847e+09,1467842);
   gre->SetPointError(3,0,119397.5);
   gre->SetPoint(4,1.443458e+09,1503558);
   gre->SetPointError(4,0,11515.81);
   gre->SetPoint(5,1.443723e+09,1399249);
   gre->SetPointError(5,0,15608.32);
   gre->SetPoint(6,1.443971e+09,1359722);
   gre->SetPointError(6,0,48679.61);
   gre->SetPoint(7,1.444134e+09,1342163);
   gre->SetPointError(7,0,15803.89);
   gre->SetPoint(8,1.444745e+09,1396321);
   gre->SetPointError(8,0,83648.62);
   gre->SetPoint(9,1.445869e+09,1454957);
   gre->SetPointError(9,0,10764.69);
   
   TH1F *Graph_Graph17 = new TH1F("Graph_Graph17","Channel 17 - Gain run_15",100,1.440929e+09,1.446318e+09);
   Graph_Graph17->SetMinimum(700000);
   Graph_Graph17->SetMaximum(2000000);
   Graph_Graph17->SetDirectory(0);
   Graph_Graph17->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph17->SetLineColor(ci);
   Graph_Graph17->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph17->GetXaxis()->SetTimeFormat("%m-%d%F1970-01-01 00:00:00s0 GMT");
   Graph_Graph17->GetXaxis()->SetLabelFont(42);
   Graph_Graph17->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph17->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph17->GetXaxis()->SetTitleFont(42);
   Graph_Graph17->GetYaxis()->SetTitle("SPE gain [electrons]");
   Graph_Graph17->GetYaxis()->SetLabelFont(42);
   Graph_Graph17->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph17->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph17->GetYaxis()->SetTitleFont(42);
   Graph_Graph17->GetZaxis()->SetLabelFont(42);
   Graph_Graph17->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph17->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph17->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph17);
   
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2757143,0.94,0.7242857,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 17 - Gain run_15");
   pt->Draw();
   ed213bafb4e64d4caa004ad3bb87e121->Modified();
   ed213bafb4e64d4caa004ad3bb87e121->cd();
   ed213bafb4e64d4caa004ad3bb87e121->SetSelected(ed213bafb4e64d4caa004ad3bb87e121);
}
