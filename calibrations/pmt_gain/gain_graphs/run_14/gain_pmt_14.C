{
//=========Macro generated from canvas: 568a60bf1ea24c4b8ca4c0c646c3f892/
//=========  (Fri Aug 28 16:04:35 2015) by ROOT version5.34/26
   TCanvas *568a60bf1ea24c4b8ca4c0c646c3f892 = new TCanvas("568a60bf1ea24c4b8ca4c0c646c3f892", "",10,45,704,527);
   568a60bf1ea24c4b8ca4c0c646c3f892->SetHighLightColor(2);
   568a60bf1ea24c4b8ca4c0c646c3f892->Range(1.440011e+09,1050000,1.440011e+09,2550000);
   568a60bf1ea24c4b8ca4c0c646c3f892->SetFillColor(0);
   568a60bf1ea24c4b8ca4c0c646c3f892->SetBorderMode(0);
   568a60bf1ea24c4b8ca4c0c646c3f892->SetBorderSize(2);
   568a60bf1ea24c4b8ca4c0c646c3f892->SetFrameBorderMode(0);
   568a60bf1ea24c4b8ca4c0c646c3f892->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(1);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 14 - Gain run_14");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.440011e+09,1601941);
   gre->SetPointError(0,0,20682.29);
   
   TH1F *Graph_Graph14 = new TH1F("Graph_Graph14","Channel 14 - Gain run_14",100,1.440011e+09,1.440011e+09);
   Graph_Graph14->SetMinimum(1200000);
   Graph_Graph14->SetMaximum(2400000);
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
   TText *text = pt->AddText("Channel 14 - Gain run_14");
   pt->Draw();
   568a60bf1ea24c4b8ca4c0c646c3f892->Modified();
   568a60bf1ea24c4b8ca4c0c646c3f892->cd();
   568a60bf1ea24c4b8ca4c0c646c3f892->SetSelected(568a60bf1ea24c4b8ca4c0c646c3f892);
}
