{
//=========Macro generated from canvas: 79a9bba60da7485c9c787f058314c678/
//=========  (Tue Oct 27 09:51:53 2015) by ROOT version5.34/26
   TCanvas *79a9bba60da7485c9c787f058314c678 = new TCanvas("79a9bba60da7485c9c787f058314c678", "",10,45,704,527);
   79a9bba60da7485c9c787f058314c678->SetHighLightColor(2);
   79a9bba60da7485c9c787f058314c678->Range(1.440143e+09,537500,1.447014e+09,2162500);
   79a9bba60da7485c9c787f058314c678->SetFillColor(0);
   79a9bba60da7485c9c787f058314c678->SetBorderMode(0);
   79a9bba60da7485c9c787f058314c678->SetBorderSize(2);
   79a9bba60da7485c9c787f058314c678->SetFrameBorderMode(0);
   79a9bba60da7485c9c787f058314c678->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(8);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 1 - Gain run_15");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.441288e+09,1219511);
   gre->SetPointError(0,0,5594.654);
   gre->SetPoint(1,1.441805e+09,1292666);
   gre->SetPointError(1,0,5336.634);
   gre->SetPoint(2,1.44243e+09,1279805);
   gre->SetPointError(2,0,5605.585);
   gre->SetPoint(3,1.442847e+09,1235926);
   gre->SetPointError(3,0,610659.9);
   gre->SetPoint(4,1.443458e+09,1185960);
   gre->SetPointError(4,0,16248.18);
   gre->SetPoint(5,1.443971e+09,1220168);
   gre->SetPointError(5,0,6325.567);
   gre->SetPoint(6,1.444745e+09,1283523);
   gre->SetPointError(6,0,5586.808);
   gre->SetPoint(7,1.445869e+09,1225392);
   gre->SetPointError(7,0,6045.733);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","Channel 1 - Gain run_15",100,1.44083e+09,1.446327e+09);
   Graph_Graph1->SetMinimum(700000);
   Graph_Graph1->SetMaximum(2000000);
   Graph_Graph1->SetDirectory(0);
   Graph_Graph1->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph1->SetLineColor(ci);
   Graph_Graph1->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph1->GetXaxis()->SetTimeFormat("%m-%d%F1970-01-01 00:00:00s0 GMT");
   Graph_Graph1->GetXaxis()->SetLabelFont(42);
   Graph_Graph1->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleFont(42);
   Graph_Graph1->GetYaxis()->SetTitle("SPE gain [electrons]");
   Graph_Graph1->GetYaxis()->SetLabelFont(42);
   Graph_Graph1->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleFont(42);
   Graph_Graph1->GetZaxis()->SetLabelFont(42);
   Graph_Graph1->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1);
   
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2857143,0.94,0.7142857,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 1 - Gain run_15");
   pt->Draw();
   79a9bba60da7485c9c787f058314c678->Modified();
   79a9bba60da7485c9c787f058314c678->cd();
   79a9bba60da7485c9c787f058314c678->SetSelected(79a9bba60da7485c9c787f058314c678);
}
