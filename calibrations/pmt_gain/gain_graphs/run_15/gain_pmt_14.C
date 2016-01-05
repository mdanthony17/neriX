{
//=========Macro generated from canvas: 4a2043aab2bc4ae3825ff7df03b7528a/
//=========  (Tue Oct 27 09:51:56 2015) by ROOT version5.34/26
   TCanvas *4a2043aab2bc4ae3825ff7df03b7528a = new TCanvas("4a2043aab2bc4ae3825ff7df03b7528a", "",10,45,704,527);
   4a2043aab2bc4ae3825ff7df03b7528a->SetHighLightColor(2);
   4a2043aab2bc4ae3825ff7df03b7528a->Range(1.440143e+09,537500,1.447014e+09,2162500);
   4a2043aab2bc4ae3825ff7df03b7528a->SetFillColor(0);
   4a2043aab2bc4ae3825ff7df03b7528a->SetBorderMode(0);
   4a2043aab2bc4ae3825ff7df03b7528a->SetBorderSize(2);
   4a2043aab2bc4ae3825ff7df03b7528a->SetFrameBorderMode(0);
   4a2043aab2bc4ae3825ff7df03b7528a->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(7);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 14 - Gain run_15");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.441288e+09,1445862);
   gre->SetPointError(0,0,7989.647);
   gre->SetPoint(1,1.441805e+09,1301250);
   gre->SetPointError(1,0,12657.95);
   gre->SetPoint(2,1.44243e+09,1273005);
   gre->SetPointError(2,0,10278.44);
   gre->SetPoint(3,1.443458e+09,1473553);
   gre->SetPointError(3,0,7422.243);
   gre->SetPoint(4,1.443971e+09,1280054);
   gre->SetPointError(4,0,32308.28);
   gre->SetPoint(5,1.444745e+09,1265674);
   gre->SetPointError(5,0,15847.21);
   gre->SetPoint(6,1.445869e+09,1239314);
   gre->SetPointError(6,0,12441.89);
   
   TH1F *Graph_Graph14 = new TH1F("Graph_Graph14","Channel 14 - Gain run_15",100,1.44083e+09,1.446327e+09);
   Graph_Graph14->SetMinimum(700000);
   Graph_Graph14->SetMaximum(2000000);
   Graph_Graph14->SetDirectory(0);
   Graph_Graph14->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph14->SetLineColor(ci);
   Graph_Graph14->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph14->GetXaxis()->SetTimeFormat("%m-%d%F1970-01-01 00:00:00s0 GMT");
   Graph_Graph14->GetXaxis()->SetLabelFont(42);
   Graph_Graph14->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph14->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph14->GetXaxis()->SetTitleFont(42);
   Graph_Graph14->GetYaxis()->SetTitle("SPE gain [electrons]");
   Graph_Graph14->GetYaxis()->SetLabelFont(42);
   Graph_Graph14->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph14->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph14->GetYaxis()->SetTitleFont(42);
   Graph_Graph14->GetZaxis()->SetLabelFont(42);
   Graph_Graph14->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph14->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph14->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph14);
   
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2757143,0.94,0.7242857,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 14 - Gain run_15");
   pt->Draw();
   4a2043aab2bc4ae3825ff7df03b7528a->Modified();
   4a2043aab2bc4ae3825ff7df03b7528a->cd();
   4a2043aab2bc4ae3825ff7df03b7528a->SetSelected(4a2043aab2bc4ae3825ff7df03b7528a);
}
