{
//=========Macro generated from canvas: a24a9c5750334f0196b476263c2aec12/
//=========  (Tue Oct 27 09:51:53 2015) by ROOT version5.34/26
   TCanvas *a24a9c5750334f0196b476263c2aec12 = new TCanvas("a24a9c5750334f0196b476263c2aec12", "",10,45,704,527);
   a24a9c5750334f0196b476263c2aec12->SetHighLightColor(2);
   a24a9c5750334f0196b476263c2aec12->Range(1.440143e+09,537500,1.447014e+09,2162500);
   a24a9c5750334f0196b476263c2aec12->SetFillColor(0);
   a24a9c5750334f0196b476263c2aec12->SetBorderMode(0);
   a24a9c5750334f0196b476263c2aec12->SetBorderSize(2);
   a24a9c5750334f0196b476263c2aec12->SetFrameBorderMode(0);
   a24a9c5750334f0196b476263c2aec12->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(8);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 4 - Gain run_15");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.441288e+09,1257101);
   gre->SetPointError(0,0,4816.048);
   gre->SetPoint(1,1.441805e+09,1280024);
   gre->SetPointError(1,0,4760.346);
   gre->SetPoint(2,1.44243e+09,1262157);
   gre->SetPointError(2,0,4847.418);
   gre->SetPoint(3,1.442847e+09,1259848);
   gre->SetPointError(3,0,7069.373);
   gre->SetPoint(4,1.443458e+09,1238292);
   gre->SetPointError(4,0,4836.858);
   gre->SetPoint(5,1.443971e+09,1248185);
   gre->SetPointError(5,0,5107.339);
   gre->SetPoint(6,1.444745e+09,1233733);
   gre->SetPointError(6,0,4935.193);
   gre->SetPoint(7,1.445869e+09,1216640);
   gre->SetPointError(7,0,4982.077);
   
   TH1F *Graph_Graph4 = new TH1F("Graph_Graph4","Channel 4 - Gain run_15",100,1.44083e+09,1.446327e+09);
   Graph_Graph4->SetMinimum(700000);
   Graph_Graph4->SetMaximum(2000000);
   Graph_Graph4->SetDirectory(0);
   Graph_Graph4->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph4->SetLineColor(ci);
   Graph_Graph4->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph4->GetXaxis()->SetTimeFormat("%m-%d%F1970-01-01 00:00:00s0 GMT");
   Graph_Graph4->GetXaxis()->SetLabelFont(42);
   Graph_Graph4->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph4->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph4->GetXaxis()->SetTitleFont(42);
   Graph_Graph4->GetYaxis()->SetTitle("SPE gain [electrons]");
   Graph_Graph4->GetYaxis()->SetLabelFont(42);
   Graph_Graph4->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph4->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph4->GetYaxis()->SetTitleFont(42);
   Graph_Graph4->GetZaxis()->SetLabelFont(42);
   Graph_Graph4->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph4->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph4->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph4);
   
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2857143,0.94,0.7142857,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 4 - Gain run_15");
   pt->Draw();
   a24a9c5750334f0196b476263c2aec12->Modified();
   a24a9c5750334f0196b476263c2aec12->cd();
   a24a9c5750334f0196b476263c2aec12->SetSelected(a24a9c5750334f0196b476263c2aec12);
}
