{
//=========Macro generated from canvas: 8db14acf3f484b5581ca979a98bf0dc8/
//=========  (Fri Aug 28 16:04:31 2015) by ROOT version5.34/26
   TCanvas *8db14acf3f484b5581ca979a98bf0dc8 = new TCanvas("8db14acf3f484b5581ca979a98bf0dc8", "",10,45,704,527);
   8db14acf3f484b5581ca979a98bf0dc8->SetHighLightColor(2);
   8db14acf3f484b5581ca979a98bf0dc8->Range(1.440011e+09,1050000,1.440011e+09,2550000);
   8db14acf3f484b5581ca979a98bf0dc8->SetFillColor(0);
   8db14acf3f484b5581ca979a98bf0dc8->SetBorderMode(0);
   8db14acf3f484b5581ca979a98bf0dc8->SetBorderSize(2);
   8db14acf3f484b5581ca979a98bf0dc8->SetFrameBorderMode(0);
   8db14acf3f484b5581ca979a98bf0dc8->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(1);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 6 - Gain run_14");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.440011e+09,1838302);
   gre->SetPointError(0,0,33046.28);
   
   TH1F *Graph_Graph6 = new TH1F("Graph_Graph6","Channel 6 - Gain run_14",100,1.440011e+09,1.440011e+09);
   Graph_Graph6->SetMinimum(1200000);
   Graph_Graph6->SetMaximum(2400000);
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
   TText *text = pt->AddText("Channel 6 - Gain run_14");
   pt->Draw();
   8db14acf3f484b5581ca979a98bf0dc8->Modified();
   8db14acf3f484b5581ca979a98bf0dc8->cd();
   8db14acf3f484b5581ca979a98bf0dc8->SetSelected(8db14acf3f484b5581ca979a98bf0dc8);
}
