{
//=========Macro generated from canvas: a594e27b132b4c5c84c6f8f1584c39bd/
//=========  (Fri Aug 28 16:04:30 2015) by ROOT version5.34/26
   TCanvas *a594e27b132b4c5c84c6f8f1584c39bd = new TCanvas("a594e27b132b4c5c84c6f8f1584c39bd", "",10,45,704,527);
   a594e27b132b4c5c84c6f8f1584c39bd->SetHighLightColor(2);
   a594e27b132b4c5c84c6f8f1584c39bd->Range(1.440011e+09,1050000,1.440011e+09,2550000);
   a594e27b132b4c5c84c6f8f1584c39bd->SetFillColor(0);
   a594e27b132b4c5c84c6f8f1584c39bd->SetBorderMode(0);
   a594e27b132b4c5c84c6f8f1584c39bd->SetBorderSize(2);
   a594e27b132b4c5c84c6f8f1584c39bd->SetFrameBorderMode(0);
   a594e27b132b4c5c84c6f8f1584c39bd->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(1);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 4 - Gain run_14");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.440011e+09,1659006);
   gre->SetPointError(0,0,16699.84);
   
   TH1F *Graph_Graph4 = new TH1F("Graph_Graph4","Channel 4 - Gain run_14",100,1.440011e+09,1.440011e+09);
   Graph_Graph4->SetMinimum(1200000);
   Graph_Graph4->SetMaximum(2400000);
   Graph_Graph4->SetDirectory(0);
   Graph_Graph4->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph4->SetLineColor(ci);
   Graph_Graph4->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph4->GetXaxis()->SetTimeFormat("%m-%d%F1970-01-01 00:00:00s0 GMT");
   Graph_Graph4->GetXaxis()->SetLabelFont(42);
   Graph_Graph4->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph4->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph4->GetXaxis()->SetTitleFont(42);
   Graph_Graph4->GetYaxis()->SetTitle("SPE gain [electrons]");
   Graph_Graph4->GetYaxis()->SetLabelFont(42);
   Graph_Graph4->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph4->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph4->GetYaxis()->SetTitleFont(42);
   Graph_Graph4->GetZaxis()->SetLabelFont(42);
   Graph_Graph4->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph4->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph4->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph4);
   
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2857143,0.94,0.7142857,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 4 - Gain run_14");
   pt->Draw();
   a594e27b132b4c5c84c6f8f1584c39bd->Modified();
   a594e27b132b4c5c84c6f8f1584c39bd->cd();
   a594e27b132b4c5c84c6f8f1584c39bd->SetSelected(a594e27b132b4c5c84c6f8f1584c39bd);
}
