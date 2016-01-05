{
//=========Macro generated from canvas: 036aee1cc9cb4ba4923d718d1d059bd3/
//=========  (Tue Oct 27 09:51:54 2015) by ROOT version5.34/26
   TCanvas *036aee1cc9cb4ba4923d718d1d059bd3 = new TCanvas("036aee1cc9cb4ba4923d718d1d059bd3", "",10,45,704,527);
   036aee1cc9cb4ba4923d718d1d059bd3->SetHighLightColor(2);
   036aee1cc9cb4ba4923d718d1d059bd3->Range(1.440143e+09,537500,1.447014e+09,2162500);
   036aee1cc9cb4ba4923d718d1d059bd3->SetFillColor(0);
   036aee1cc9cb4ba4923d718d1d059bd3->SetBorderMode(0);
   036aee1cc9cb4ba4923d718d1d059bd3->SetBorderSize(2);
   036aee1cc9cb4ba4923d718d1d059bd3->SetFrameBorderMode(0);
   036aee1cc9cb4ba4923d718d1d059bd3->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(8);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 9 - Gain run_15");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.441288e+09,1438329);
   gre->SetPointError(0,0,7335.638);
   gre->SetPoint(1,1.441805e+09,1220817);
   gre->SetPointError(1,0,72550.61);
   gre->SetPoint(2,1.44243e+09,1375143);
   gre->SetPointError(2,0,17233.91);
   gre->SetPoint(3,1.442847e+09,1394512);
   gre->SetPointError(3,0,8985.322);
   gre->SetPoint(4,1.443458e+09,1363699);
   gre->SetPointError(4,0,6579.096);
   gre->SetPoint(5,1.443971e+09,1272549);
   gre->SetPointError(5,0,47013.49);
   gre->SetPoint(6,1.444745e+09,1221141);
   gre->SetPointError(6,0,14720.24);
   gre->SetPoint(7,1.445869e+09,1187209);
   gre->SetPointError(7,0,13848.33);
   
   TH1F *Graph_Graph9 = new TH1F("Graph_Graph9","Channel 9 - Gain run_15",100,1.44083e+09,1.446327e+09);
   Graph_Graph9->SetMinimum(700000);
   Graph_Graph9->SetMaximum(2000000);
   Graph_Graph9->SetDirectory(0);
   Graph_Graph9->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph9->SetLineColor(ci);
   Graph_Graph9->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph9->GetXaxis()->SetTimeFormat("%m-%d%F1970-01-01 00:00:00s0 GMT");
   Graph_Graph9->GetXaxis()->SetLabelFont(42);
   Graph_Graph9->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph9->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph9->GetXaxis()->SetTitleFont(42);
   Graph_Graph9->GetYaxis()->SetTitle("SPE gain [electrons]");
   Graph_Graph9->GetYaxis()->SetLabelFont(42);
   Graph_Graph9->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph9->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph9->GetYaxis()->SetTitleFont(42);
   Graph_Graph9->GetZaxis()->SetLabelFont(42);
   Graph_Graph9->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph9->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph9->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph9);
   
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2857143,0.94,0.7142857,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 9 - Gain run_15");
   pt->Draw();
   036aee1cc9cb4ba4923d718d1d059bd3->Modified();
   036aee1cc9cb4ba4923d718d1d059bd3->cd();
   036aee1cc9cb4ba4923d718d1d059bd3->SetSelected(036aee1cc9cb4ba4923d718d1d059bd3);
}
