{
//=========Macro generated from canvas: 1f2810d43c2744b5a03d0191c135ffc4/
//=========  (Tue Sep 20 09:15:32 2016) by ROOT version5.34/26
   TCanvas *1f2810d43c2744b5a03d0191c135ffc4 = new TCanvas("1f2810d43c2744b5a03d0191c135ffc4", "",10,45,704,527);
   1f2810d43c2744b5a03d0191c135ffc4->SetHighLightColor(2);
   1f2810d43c2744b5a03d0191c135ffc4->Range(1.458197e+09,9.375,1.471042e+09,15.625);
   1f2810d43c2744b5a03d0191c135ffc4->SetFillColor(0);
   1f2810d43c2744b5a03d0191c135ffc4->SetBorderMode(0);
   1f2810d43c2744b5a03d0191c135ffc4->SetBorderSize(2);
   1f2810d43c2744b5a03d0191c135ffc4->SetFrameBorderMode(0);
   1f2810d43c2744b5a03d0191c135ffc4->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(9);
   gre->SetName("Graph0");
   gre->SetTitle("Gas Gain Run Average, Method Average - 0.00 < R < 0.85");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#ff00ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff00ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.460986e+09,13.49);
   gre->SetPointError(0,0,0.07);
   gre->SetPoint(1,1.461261e+09,12.84);
   gre->SetPointError(1,0,0.07);
   gre->SetPoint(2,1.462196e+09,12.83);
   gre->SetPointError(2,0,0.06);
   gre->SetPoint(3,1.462805e+09,12.8);
   gre->SetPointError(3,0,0.07);
   gre->SetPoint(4,1.46307e+09,13.01);
   gre->SetPointError(4,0,0.08);
   gre->SetPoint(5,1.465916e+09,13.8);
   gre->SetPointError(5,0,0.04);
   gre->SetPoint(6,1.468243e+09,13.43);
   gre->SetPointError(6,0,0.04);
   gre->SetPoint(7,1.468852e+09,13.44);
   gre->SetPointError(7,0,0.04);
   gre->SetPoint(8,1.469458e+09,13.25);
   gre->SetPointError(8,0,0.04);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","Gas Gain Run Average, Method Average - 0.00 < R < 0.85",100,1.459482e+09,1.469758e+09);
   Graph_Graph1->SetMinimum(10);
   Graph_Graph1->SetMaximum(15);
   Graph_Graph1->SetDirectory(0);
   Graph_Graph1->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph1->SetLineColor(ci);
   Graph_Graph1->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph1->GetXaxis()->SetTimeFormat("%m-%d%F1995-01-01 00:00:00s0");
   Graph_Graph1->GetXaxis()->SetLabelFont(42);
   Graph_Graph1->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleFont(42);
   Graph_Graph1->GetYaxis()->SetTitle("Gas gain [PE per extracted electron]");
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
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(2);
   grae->SetName("conf_0.345_(0.0, 0.85)");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#ff00ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3004);

   ci = TColor::GetColor("#ff00ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.459482e+09,13.33299);
   grae->SetPointError(0,0,0,0.01679245,0.01679245);
   grae->SetPoint(1,1.469758e+09,13.33299);
   grae->SetPointError(1,0,0,0.01679245,0.01679245);
   
   TH1F *Graph_conf_0.345_(0.0, 0.85)1 = new TH1F("Graph_conf_0.345_(0.0, 0.85)1","Graph",100,1.458454e+09,1.470785e+09);
   Graph_conf_0.345_(0.0, 0.85)1->SetMinimum(13.31284);
   Graph_conf_0.345_(0.0, 0.85)1->SetMaximum(13.35314);
   Graph_conf_0.345_(0.0, 0.85)1->SetDirectory(0);
   Graph_conf_0.345_(0.0, 0.85)1->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_conf_0.345_(0.0, 0.85)1->SetLineColor(ci);
   Graph_conf_0.345_(0.0, 0.85)1->GetXaxis()->SetLabelFont(42);
   Graph_conf_0.345_(0.0, 0.85)1->GetXaxis()->SetLabelSize(0.035);
   Graph_conf_0.345_(0.0, 0.85)1->GetXaxis()->SetTitleSize(0.035);
   Graph_conf_0.345_(0.0, 0.85)1->GetXaxis()->SetTitleFont(42);
   Graph_conf_0.345_(0.0, 0.85)1->GetYaxis()->SetLabelFont(42);
   Graph_conf_0.345_(0.0, 0.85)1->GetYaxis()->SetLabelSize(0.035);
   Graph_conf_0.345_(0.0, 0.85)1->GetYaxis()->SetTitleSize(0.035);
   Graph_conf_0.345_(0.0, 0.85)1->GetYaxis()->SetTitleFont(42);
   Graph_conf_0.345_(0.0, 0.85)1->GetZaxis()->SetLabelFont(42);
   Graph_conf_0.345_(0.0, 0.85)1->GetZaxis()->SetLabelSize(0.035);
   Graph_conf_0.345_(0.0, 0.85)1->GetZaxis()->SetTitleSize(0.035);
   Graph_conf_0.345_(0.0, 0.85)1->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_conf_0.345_(0.0, 0.85)1);
   
   grae->Draw("3 ");
   
   TPaveText *pt = new TPaveText(0.15,0.9374,0.85,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Gas Gain Run Average, Method Average - 0.00 < R < 0.85");
   pt->Draw();
   
   gre = new TGraphErrors(6);
   gre->SetName("Graph1");
   gre->SetTitle("Gas Gain Run Average, Method Average - 0.00 < R < 0.85");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459782e+09,12.97);
   gre->SetPointError(0,0,0.08);
   gre->SetPoint(1,1.460052e+09,12.76);
   gre->SetPointError(1,0,0.08);
   gre->SetPoint(2,1.460467e+09,12.92);
   gre->SetPointError(2,0,0.07);
   gre->SetPoint(3,1.460650e+09,13);
   gre->SetPointError(3,0,0.07);
   gre->SetPoint(4,1.463407e+09,12.98);
   gre->SetPointError(4,0,0.07);
   gre->SetPoint(5,1.466434e+09,13.76);
   gre->SetPointError(5,0,0.04);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Gas Gain Run Average, Method Average - 0.00 < R < 0.85",100,1.459482e+09,1.469758e+09);
   Graph_Graph2->SetMinimum(10);
   Graph_Graph2->SetMaximum(15);
   Graph_Graph2->SetDirectory(0);
   Graph_Graph2->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph2->SetLineColor(ci);
   Graph_Graph2->GetXaxis()->SetLabelFont(42);
   Graph_Graph2->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph2->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph2->GetXaxis()->SetTitleFont(42);
   Graph_Graph2->GetYaxis()->SetLabelFont(42);
   Graph_Graph2->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph2->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph2->GetYaxis()->SetTitleFont(42);
   Graph_Graph2->GetZaxis()->SetLabelFont(42);
   Graph_Graph2->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph2->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph2->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph2);
   
   gre->Draw("p ");
   
   grae = new TGraphAsymmErrors(2);
   grae->SetName("conf_1.054_(0.0, 0.85)");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#0000ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3005);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.459482e+09,13.26611);
   grae->SetPointError(0,0,0,0.02540212,0.02540212);
   grae->SetPoint(1,1.469758e+09,13.26611);
   grae->SetPointError(1,0,0,0.02540212,0.02540212);
   
   TH1F *Graph_conf_1.054_(0.0, 0.85)2 = new TH1F("Graph_conf_1.054_(0.0, 0.85)2","Graph",100,1.458454e+09,1.470785e+09);
   Graph_conf_1.054_(0.0, 0.85)2->SetMinimum(13.23563);
   Graph_conf_1.054_(0.0, 0.85)2->SetMaximum(13.29659);
   Graph_conf_1.054_(0.0, 0.85)2->SetDirectory(0);
   Graph_conf_1.054_(0.0, 0.85)2->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_conf_1.054_(0.0, 0.85)2->SetLineColor(ci);
   Graph_conf_1.054_(0.0, 0.85)2->GetXaxis()->SetLabelFont(42);
   Graph_conf_1.054_(0.0, 0.85)2->GetXaxis()->SetLabelSize(0.035);
   Graph_conf_1.054_(0.0, 0.85)2->GetXaxis()->SetTitleSize(0.035);
   Graph_conf_1.054_(0.0, 0.85)2->GetXaxis()->SetTitleFont(42);
   Graph_conf_1.054_(0.0, 0.85)2->GetYaxis()->SetLabelFont(42);
   Graph_conf_1.054_(0.0, 0.85)2->GetYaxis()->SetLabelSize(0.035);
   Graph_conf_1.054_(0.0, 0.85)2->GetYaxis()->SetTitleSize(0.035);
   Graph_conf_1.054_(0.0, 0.85)2->GetYaxis()->SetTitleFont(42);
   Graph_conf_1.054_(0.0, 0.85)2->GetZaxis()->SetLabelFont(42);
   Graph_conf_1.054_(0.0, 0.85)2->GetZaxis()->SetLabelSize(0.035);
   Graph_conf_1.054_(0.0, 0.85)2->GetZaxis()->SetTitleSize(0.035);
   Graph_conf_1.054_(0.0, 0.85)2->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_conf_1.054_(0.0, 0.85)2);
   
   grae->Draw("3 ");
   
   gre = new TGraphErrors(6);
   gre->SetName("Graph2");
   gre->SetTitle("Gas Gain Run Average, Method Average - 0.00 < R < 0.85");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#00ff00");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#00ff00");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.461857e+09,13.18);
   gre->SetPointError(0,0,0.07);
   gre->SetPoint(1,1.464104e+09,13.68);
   gre->SetPointError(1,0,0.04);
   gre->SetPoint(2,1.464705e+09,13.92);
   gre->SetPointError(2,0,0.03);
   gre->SetPoint(3,1.465224e+09,13.98);
   gre->SetPointError(3,0,0.04);
   gre->SetPoint(4,1.467039e+09,13.76);
   gre->SetPointError(4,0,0.04);
   gre->SetPoint(5,1.46773e+09,13.39);
   gre->SetPointError(5,0,0.04);
   
   TH1F *Graph_Graph3 = new TH1F("Graph_Graph3","Gas Gain Run Average, Method Average - 0.00 < R < 0.85",100,1.459482e+09,1.469758e+09);
   Graph_Graph3->SetMinimum(10);
   Graph_Graph3->SetMaximum(15);
   Graph_Graph3->SetDirectory(0);
   Graph_Graph3->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph3->SetLineColor(ci);
   Graph_Graph3->GetXaxis()->SetLabelFont(42);
   Graph_Graph3->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph3->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph3->GetXaxis()->SetTitleFont(42);
   Graph_Graph3->GetYaxis()->SetLabelFont(42);
   Graph_Graph3->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph3->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph3->GetYaxis()->SetTitleFont(42);
   Graph_Graph3->GetZaxis()->SetLabelFont(42);
   Graph_Graph3->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph3->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph3->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph3);
   
   gre->Draw("p ");
   
   grae = new TGraphAsymmErrors(2);
   grae->SetName("conf_2.356_(0.0, 0.85)");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#00ff00");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3006);

   ci = TColor::GetColor("#00ff00");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.459482e+09,13.73789);
   grae->SetPointError(0,0,0,0.01618981,0.01618981);
   grae->SetPoint(1,1.469758e+09,13.73789);
   grae->SetPointError(1,0,0,0.01618981,0.01618981);
   
   TH1F *Graph_conf_2.356_(0.0, 0.85)3 = new TH1F("Graph_conf_2.356_(0.0, 0.85)3","Graph",100,1.458454e+09,1.470785e+09);
   Graph_conf_2.356_(0.0, 0.85)3->SetMinimum(13.71847);
   Graph_conf_2.356_(0.0, 0.85)3->SetMaximum(13.75732);
   Graph_conf_2.356_(0.0, 0.85)3->SetDirectory(0);
   Graph_conf_2.356_(0.0, 0.85)3->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_conf_2.356_(0.0, 0.85)3->SetLineColor(ci);
   Graph_conf_2.356_(0.0, 0.85)3->GetXaxis()->SetLabelFont(42);
   Graph_conf_2.356_(0.0, 0.85)3->GetXaxis()->SetLabelSize(0.035);
   Graph_conf_2.356_(0.0, 0.85)3->GetXaxis()->SetTitleSize(0.035);
   Graph_conf_2.356_(0.0, 0.85)3->GetXaxis()->SetTitleFont(42);
   Graph_conf_2.356_(0.0, 0.85)3->GetYaxis()->SetLabelFont(42);
   Graph_conf_2.356_(0.0, 0.85)3->GetYaxis()->SetLabelSize(0.035);
   Graph_conf_2.356_(0.0, 0.85)3->GetYaxis()->SetTitleSize(0.035);
   Graph_conf_2.356_(0.0, 0.85)3->GetYaxis()->SetTitleFont(42);
   Graph_conf_2.356_(0.0, 0.85)3->GetZaxis()->SetLabelFont(42);
   Graph_conf_2.356_(0.0, 0.85)3->GetZaxis()->SetLabelSize(0.035);
   Graph_conf_2.356_(0.0, 0.85)3->GetZaxis()->SetTitleSize(0.035);
   Graph_conf_2.356_(0.0, 0.85)3->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_conf_2.356_(0.0, 0.85)3);
   
   grae->Draw("3 ");
   
   pt = new TPaveText(0.15,0.8,0.5,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 0.345kV: 13.33 +/- 0.02 PE");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.75,0.5,0.8,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.054kV: 13.27 +/- 0.03 PE");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.7,0.5,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#00ff00");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 2.356kV: 13.74 +/- 0.02 PE");
   pt->Draw();
   1f2810d43c2744b5a03d0191c135ffc4->Modified();
   1f2810d43c2744b5a03d0191c135ffc4->cd();
   1f2810d43c2744b5a03d0191c135ffc4->SetSelected(1f2810d43c2744b5a03d0191c135ffc4);
}
