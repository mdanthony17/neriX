{
//=========Macro generated from canvas: fcd0ee386cd64c8ba5f5647a9ee6f098/
//=========  (Tue Oct 27 09:51:54 2015) by ROOT version5.34/26
   TCanvas *fcd0ee386cd64c8ba5f5647a9ee6f098 = new TCanvas("fcd0ee386cd64c8ba5f5647a9ee6f098", "",10,45,704,527);
   fcd0ee386cd64c8ba5f5647a9ee6f098->SetHighLightColor(2);
   fcd0ee386cd64c8ba5f5647a9ee6f098->Range(1.440143e+09,537500,1.447014e+09,2162500);
   fcd0ee386cd64c8ba5f5647a9ee6f098->SetFillColor(0);
   fcd0ee386cd64c8ba5f5647a9ee6f098->SetBorderMode(0);
   fcd0ee386cd64c8ba5f5647a9ee6f098->SetBorderSize(2);
   fcd0ee386cd64c8ba5f5647a9ee6f098->SetFrameBorderMode(0);
   fcd0ee386cd64c8ba5f5647a9ee6f098->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(8);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 6 - Gain run_15");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.441288e+09,1216197);
   gre->SetPointError(0,0,13213.94);
   gre->SetPoint(1,1.441805e+09,1326720);
   gre->SetPointError(1,0,10435.09);
   gre->SetPoint(2,1.44243e+09,1297779);
   gre->SetPointError(2,0,10379.28);
   gre->SetPoint(3,1.442847e+09,1447531);
   gre->SetPointError(3,0,14232.96);
   gre->SetPoint(4,1.443458e+09,1427414);
   gre->SetPointError(4,0,10225.23);
   gre->SetPoint(5,1.443971e+09,1380881);
   gre->SetPointError(5,0,24404.28);
   gre->SetPoint(6,1.444745e+09,1314120);
   gre->SetPointError(6,0,11083.6);
   gre->SetPoint(7,1.445869e+09,1395453);
   gre->SetPointError(7,0,24336.39);
   
   TH1F *Graph_Graph6 = new TH1F("Graph_Graph6","Channel 6 - Gain run_15",100,1.44083e+09,1.446327e+09);
   Graph_Graph6->SetMinimum(700000);
   Graph_Graph6->SetMaximum(2000000);
   Graph_Graph6->SetDirectory(0);
   Graph_Graph6->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph6->SetLineColor(ci);
   Graph_Graph6->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph6->GetXaxis()->SetTimeFormat("%m-%d%F1970-01-01 00:00:00s0 GMT");
   Graph_Graph6->GetXaxis()->SetLabelFont(42);
   Graph_Graph6->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph6->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph6->GetXaxis()->SetTitleFont(42);
   Graph_Graph6->GetYaxis()->SetTitle("SPE gain [electrons]");
   Graph_Graph6->GetYaxis()->SetLabelFont(42);
   Graph_Graph6->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph6->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph6->GetYaxis()->SetTitleFont(42);
   Graph_Graph6->GetZaxis()->SetLabelFont(42);
   Graph_Graph6->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph6->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph6->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph6);
   
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2857143,0.94,0.7142857,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 6 - Gain run_15");
   pt->Draw();
   fcd0ee386cd64c8ba5f5647a9ee6f098->Modified();
   fcd0ee386cd64c8ba5f5647a9ee6f098->cd();
   fcd0ee386cd64c8ba5f5647a9ee6f098->SetSelected(fcd0ee386cd64c8ba5f5647a9ee6f098);
}
