{
//=========Macro generated from canvas: c5bb01b3c97642e695193a549c945f32/
//=========  (Fri Aug 28 16:04:36 2015) by ROOT version5.34/26
   TCanvas *c5bb01b3c97642e695193a549c945f32 = new TCanvas("c5bb01b3c97642e695193a549c945f32", "",10,45,704,527);
   c5bb01b3c97642e695193a549c945f32->SetHighLightColor(2);
   c5bb01b3c97642e695193a549c945f32->Range(1.440011e+09,1050000,1.440011e+09,2550000);
   c5bb01b3c97642e695193a549c945f32->SetFillColor(0);
   c5bb01b3c97642e695193a549c945f32->SetBorderMode(0);
   c5bb01b3c97642e695193a549c945f32->SetBorderSize(2);
   c5bb01b3c97642e695193a549c945f32->SetFrameBorderMode(0);
   c5bb01b3c97642e695193a549c945f32->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(1);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 17 - Gain run_14");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.440011e+09,1909607);
   gre->SetPointError(0,0,34035.69);
   
   TH1F *Graph_Graph17 = new TH1F("Graph_Graph17","Channel 17 - Gain run_14",100,1.440011e+09,1.440011e+09);
   Graph_Graph17->SetMinimum(1200000);
   Graph_Graph17->SetMaximum(2400000);
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
   TText *text = pt->AddText("Channel 17 - Gain run_14");
   pt->Draw();
   c5bb01b3c97642e695193a549c945f32->Modified();
   c5bb01b3c97642e695193a549c945f32->cd();
   c5bb01b3c97642e695193a549c945f32->SetSelected(c5bb01b3c97642e695193a549c945f32);
}
