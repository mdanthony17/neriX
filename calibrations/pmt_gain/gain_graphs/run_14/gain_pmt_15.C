{
//=========Macro generated from canvas: f786f26cc4574a71939a313baa32ee97/
//=========  (Fri Aug 28 16:04:35 2015) by ROOT version5.34/26
   TCanvas *f786f26cc4574a71939a313baa32ee97 = new TCanvas("f786f26cc4574a71939a313baa32ee97", "",10,45,704,527);
   f786f26cc4574a71939a313baa32ee97->SetHighLightColor(2);
   f786f26cc4574a71939a313baa32ee97->Range(1.440011e+09,1050000,1.440011e+09,2550000);
   f786f26cc4574a71939a313baa32ee97->SetFillColor(0);
   f786f26cc4574a71939a313baa32ee97->SetBorderMode(0);
   f786f26cc4574a71939a313baa32ee97->SetBorderSize(2);
   f786f26cc4574a71939a313baa32ee97->SetFrameBorderMode(0);
   f786f26cc4574a71939a313baa32ee97->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(1);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 15 - Gain run_14");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.440011e+09,1537872);
   gre->SetPointError(0,0,23562.68);
   
   TH1F *Graph_Graph15 = new TH1F("Graph_Graph15","Channel 15 - Gain run_14",100,1.440011e+09,1.440011e+09);
   Graph_Graph15->SetMinimum(1200000);
   Graph_Graph15->SetMaximum(2400000);
   Graph_Graph15->SetDirectory(0);
   Graph_Graph15->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph15->SetLineColor(ci);
   Graph_Graph15->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph15->GetXaxis()->SetTimeFormat("%m-%d%F1970-01-01 00:00:00s0 GMT");
   Graph_Graph15->GetXaxis()->SetLabelFont(42);
   Graph_Graph15->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph15->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph15->GetXaxis()->SetTitleFont(42);
   Graph_Graph15->GetYaxis()->SetTitle("SPE gain [electrons]");
   Graph_Graph15->GetYaxis()->SetLabelFont(42);
   Graph_Graph15->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph15->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph15->GetYaxis()->SetTitleFont(42);
   Graph_Graph15->GetZaxis()->SetLabelFont(42);
   Graph_Graph15->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph15->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph15->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph15);
   
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2757143,0.94,0.7242857,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 15 - Gain run_14");
   pt->Draw();
   f786f26cc4574a71939a313baa32ee97->Modified();
   f786f26cc4574a71939a313baa32ee97->cd();
   f786f26cc4574a71939a313baa32ee97->SetSelected(f786f26cc4574a71939a313baa32ee97);
}
