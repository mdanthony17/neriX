{
//=========Macro generated from canvas: 0ca982955d374102bc3daa048d49878e/
//=========  (Fri Aug 28 16:04:30 2015) by ROOT version5.34/26
   TCanvas *0ca982955d374102bc3daa048d49878e = new TCanvas("0ca982955d374102bc3daa048d49878e", "",10,45,704,527);
   0ca982955d374102bc3daa048d49878e->SetHighLightColor(2);
   0ca982955d374102bc3daa048d49878e->Range(1.440011e+09,1050000,1.440011e+09,2550000);
   0ca982955d374102bc3daa048d49878e->SetFillColor(0);
   0ca982955d374102bc3daa048d49878e->SetBorderMode(0);
   0ca982955d374102bc3daa048d49878e->SetBorderSize(2);
   0ca982955d374102bc3daa048d49878e->SetFrameBorderMode(0);
   0ca982955d374102bc3daa048d49878e->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(1);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 3 - Gain run_14");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.440011e+09,1764420);
   gre->SetPointError(0,0,18755.49);
   
   TH1F *Graph_Graph3 = new TH1F("Graph_Graph3","Channel 3 - Gain run_14",100,1.440011e+09,1.440011e+09);
   Graph_Graph3->SetMinimum(1200000);
   Graph_Graph3->SetMaximum(2400000);
   Graph_Graph3->SetDirectory(0);
   Graph_Graph3->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph3->SetLineColor(ci);
   Graph_Graph3->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph3->GetXaxis()->SetTimeFormat("%m-%d%F1970-01-01 00:00:00s0 GMT");
   Graph_Graph3->GetXaxis()->SetLabelFont(42);
   Graph_Graph3->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph3->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph3->GetXaxis()->SetTitleFont(42);
   Graph_Graph3->GetYaxis()->SetTitle("SPE gain [electrons]");
   Graph_Graph3->GetYaxis()->SetLabelFont(42);
   Graph_Graph3->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph3->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph3->GetYaxis()->SetTitleFont(42);
   Graph_Graph3->GetZaxis()->SetLabelFont(42);
   Graph_Graph3->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph3->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph3->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph3);
   
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2857143,0.94,0.7142857,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 3 - Gain run_14");
   pt->Draw();
   0ca982955d374102bc3daa048d49878e->Modified();
   0ca982955d374102bc3daa048d49878e->cd();
   0ca982955d374102bc3daa048d49878e->SetSelected(0ca982955d374102bc3daa048d49878e);
}
