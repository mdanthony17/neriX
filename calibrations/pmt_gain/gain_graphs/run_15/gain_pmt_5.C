{
//=========Macro generated from canvas: d5c3bdce5ae84f96bf1db9dc86082178/
//=========  (Tue Oct 27 09:51:54 2015) by ROOT version5.34/26
   TCanvas *d5c3bdce5ae84f96bf1db9dc86082178 = new TCanvas("d5c3bdce5ae84f96bf1db9dc86082178", "",10,45,704,527);
   d5c3bdce5ae84f96bf1db9dc86082178->SetHighLightColor(2);
   d5c3bdce5ae84f96bf1db9dc86082178->Range(1.440143e+09,537500,1.447014e+09,2162500);
   d5c3bdce5ae84f96bf1db9dc86082178->SetFillColor(0);
   d5c3bdce5ae84f96bf1db9dc86082178->SetBorderMode(0);
   d5c3bdce5ae84f96bf1db9dc86082178->SetBorderSize(2);
   d5c3bdce5ae84f96bf1db9dc86082178->SetFrameBorderMode(0);
   d5c3bdce5ae84f96bf1db9dc86082178->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(8);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 5 - Gain run_15");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.441288e+09,1497796);
   gre->SetPointError(0,0,8245.231);
   gre->SetPoint(1,1.441805e+09,1537241);
   gre->SetPointError(1,0,7823.879);
   gre->SetPoint(2,1.44243e+09,1573555);
   gre->SetPointError(2,0,7747.588);
   gre->SetPoint(3,1.442847e+09,1492268);
   gre->SetPointError(3,0,13259.25);
   gre->SetPoint(4,1.443458e+09,1536678);
   gre->SetPointError(4,0,8525.107);
   gre->SetPoint(5,1.443971e+09,1492774);
   gre->SetPointError(5,0,8312.371);
   gre->SetPoint(6,1.444745e+09,1494858);
   gre->SetPointError(6,0,8335.26);
   gre->SetPoint(7,1.445869e+09,1471793);
   gre->SetPointError(7,0,8431.138);
   
   TH1F *Graph_Graph5 = new TH1F("Graph_Graph5","Channel 5 - Gain run_15",100,1.44083e+09,1.446327e+09);
   Graph_Graph5->SetMinimum(700000);
   Graph_Graph5->SetMaximum(2000000);
   Graph_Graph5->SetDirectory(0);
   Graph_Graph5->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph5->SetLineColor(ci);
   Graph_Graph5->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph5->GetXaxis()->SetTimeFormat("%m-%d%F1970-01-01 00:00:00s0 GMT");
   Graph_Graph5->GetXaxis()->SetLabelFont(42);
   Graph_Graph5->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph5->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph5->GetXaxis()->SetTitleFont(42);
   Graph_Graph5->GetYaxis()->SetTitle("SPE gain [electrons]");
   Graph_Graph5->GetYaxis()->SetLabelFont(42);
   Graph_Graph5->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph5->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph5->GetYaxis()->SetTitleFont(42);
   Graph_Graph5->GetZaxis()->SetLabelFont(42);
   Graph_Graph5->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph5->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph5->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph5);
   
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2857143,0.94,0.7142857,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 5 - Gain run_15");
   pt->Draw();
   d5c3bdce5ae84f96bf1db9dc86082178->Modified();
   d5c3bdce5ae84f96bf1db9dc86082178->cd();
   d5c3bdce5ae84f96bf1db9dc86082178->SetSelected(d5c3bdce5ae84f96bf1db9dc86082178);
}
