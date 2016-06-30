{
//=========Macro generated from canvas: 29c36caeefee4508a08ce3c28c8834d6/
//=========  (Fri Aug 28 16:04:29 2015) by ROOT version5.34/26
   TCanvas *29c36caeefee4508a08ce3c28c8834d6 = new TCanvas("29c36caeefee4508a08ce3c28c8834d6", "",10,45,704,527);
   29c36caeefee4508a08ce3c28c8834d6->SetHighLightColor(2);
   29c36caeefee4508a08ce3c28c8834d6->Range(1.440011e+09,1050000,1.440011e+09,2550000);
   29c36caeefee4508a08ce3c28c8834d6->SetFillColor(0);
   29c36caeefee4508a08ce3c28c8834d6->SetBorderMode(0);
   29c36caeefee4508a08ce3c28c8834d6->SetBorderSize(2);
   29c36caeefee4508a08ce3c28c8834d6->SetFrameBorderMode(0);
   29c36caeefee4508a08ce3c28c8834d6->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(1);
   gre->SetName("Graph0");
   gre->SetTitle("Channel 1 - Gain run_14");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetPoint(0,1.440011e+09,1763068);
   gre->SetPointError(0,0,22158.53);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","Channel 1 - Gain run_14",100,1.440011e+09,1.440011e+09);
   Graph_Graph1->SetMinimum(1200000);
   Graph_Graph1->SetMaximum(2400000);
   Graph_Graph1->SetDirectory(0);
   Graph_Graph1->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph1->SetLineColor(ci);
   Graph_Graph1->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph1->GetXaxis()->SetTimeFormat("%m-%d%F1970-01-01 00:00:00s0 GMT");
   Graph_Graph1->GetXaxis()->SetLabelFont(42);
   Graph_Graph1->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleFont(42);
   Graph_Graph1->GetYaxis()->SetTitle("SPE gain [electrons]");
   Graph_Graph1->GetYaxis()->SetLabelFont(42);
   Graph_Graph1->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleFont(42);
   Graph_Graph1->GetZaxis()->SetLabelFont(42);
   Graph_Graph1->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1);
   
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2857143,0.94,0.7142857,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Channel 1 - Gain run_14");
   pt->Draw();
   29c36caeefee4508a08ce3c28c8834d6->Modified();
   29c36caeefee4508a08ce3c28c8834d6->cd();
   29c36caeefee4508a08ce3c28c8834d6->SetSelected(29c36caeefee4508a08ce3c28c8834d6);
}
