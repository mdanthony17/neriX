{
//=========Macro generated from canvas: d8913ce4150c4b1195ffe858d8d23084/
//=========  (Tue Oct 27 09:51:56 2015) by ROOT version5.34/26
   TCanvas *d8913ce4150c4b1195ffe858d8d23084 = new TCanvas("d8913ce4150c4b1195ffe858d8d23084", "",10,45,704,527);
   d8913ce4150c4b1195ffe858d8d23084->SetHighLightColor(2);
   d8913ce4150c4b1195ffe858d8d23084->Range(1.440143e+09,537500,1.447014e+09,2162500);
   d8913ce4150c4b1195ffe858d8d23084->SetFillColor(0);
   d8913ce4150c4b1195ffe858d8d23084->SetBorderMode(0);
   d8913ce4150c4b1195ffe858d8d23084->SetBorderSize(2);
   d8913ce4150c4b1195ffe858d8d23084->SetFrameBorderMode(0);
   d8913ce4150c4b1195ffe858d8d23084->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(7);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 15 - Gain run_15");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.441288e+09,1537115);
   gre->SetPointError(0,0,6262.865);
   gre->SetPoint(1,1.441805e+09,1587953);
   gre->SetPointError(1,0,5612.713);
   gre->SetPoint(2,1.44243e+09,1501760);
   gre->SetPointError(2,0,11920.31);
   gre->SetPoint(3,1.443458e+09,1379402);
   gre->SetPointError(3,0,31278.84);
   gre->SetPoint(4,1.443971e+09,1571311);
   gre->SetPointError(4,0,34233.33);
   gre->SetPoint(5,1.444745e+09,1395551);
   gre->SetPointError(5,0,12344.26);
   gre->SetPoint(6,1.445869e+09,1365292);
   gre->SetPointError(6,0,15229.49);
   
   TH1F *Graph_Graph15 = new TH1F("Graph_Graph15","Channel 15 - Gain run_15",100,1.44083e+09,1.446327e+09);
   Graph_Graph15->SetMinimum(700000);
   Graph_Graph15->SetMaximum(2000000);
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
   TText *text = pt->AddText("Channel 15 - Gain run_15");
   pt->Draw();
   d8913ce4150c4b1195ffe858d8d23084->Modified();
   d8913ce4150c4b1195ffe858d8d23084->cd();
   d8913ce4150c4b1195ffe858d8d23084->SetSelected(d8913ce4150c4b1195ffe858d8d23084);
}
