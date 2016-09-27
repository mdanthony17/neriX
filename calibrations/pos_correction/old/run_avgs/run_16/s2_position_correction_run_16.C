{
//=========Macro generated from canvas: 957e05f5a6f94ab1916a52db114cc8fc/
//=========  (Tue Jun 14 16:20:22 2016) by ROOT version5.34/26
   TCanvas *957e05f5a6f94ab1916a52db114cc8fc = new TCanvas("957e05f5a6f94ab1916a52db114cc8fc", "",10,45,1604,1035);
   957e05f5a6f94ab1916a52db114cc8fc->SetHighLightColor(2);
   957e05f5a6f94ab1916a52db114cc8fc->Range(0,0,1,1);
   957e05f5a6f94ab1916a52db114cc8fc->SetFillColor(0);
   957e05f5a6f94ab1916a52db114cc8fc->SetBorderMode(0);
   957e05f5a6f94ab1916a52db114cc8fc->SetBorderSize(2);
   957e05f5a6f94ab1916a52db114cc8fc->SetFrameBorderMode(0);
  
// ------------>Primitives in pad: 957e05f5a6f94ab1916a52db114cc8fc_1
   TPad *957e05f5a6f94ab1916a52db114cc8fc_1 = new TPad("957e05f5a6f94ab1916a52db114cc8fc_1", "957e05f5a6f94ab1916a52db114cc8fc_1",0.01,0.51,0.49,0.99);
   957e05f5a6f94ab1916a52db114cc8fc_1->Draw();
   957e05f5a6f94ab1916a52db114cc8fc_1->cd();
   957e05f5a6f94ab1916a52db114cc8fc_1->Range(1.458957e+09,-125,1.464245e+09,1125);
   957e05f5a6f94ab1916a52db114cc8fc_1->SetFillColor(0);
   957e05f5a6f94ab1916a52db114cc8fc_1->SetBorderMode(0);
   957e05f5a6f94ab1916a52db114cc8fc_1->SetBorderSize(2);
   957e05f5a6f94ab1916a52db114cc8fc_1->SetFrameBorderMode(0);
   957e05f5a6f94ab1916a52db114cc8fc_1->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(7);
   gre->SetName("Graph0");
   gre->SetTitle("e_lifetime");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#ff00ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff00ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,123.9873);
   gre->SetPointError(0,0,55.85783);
   gre->SetPoint(1,1.46037e+09,167.4909);
   gre->SetPointError(1,0,124.5834);
   gre->SetPoint(2,1.46099e+09,135.5515);
   gre->SetPointError(2,0,71.79492);
   gre->SetPoint(3,1.4616e+09,125.3717);
   gre->SetPointError(3,0,67.54972);
   gre->SetPoint(4,1.462201e+09,75.54704);
   gre->SetPointError(4,0,21.56153);
   gre->SetPoint(5,1.462809e+09,209.8493);
   gre->SetPointError(5,0,188.2065);
   gre->SetPoint(6,1.463416e+09,181.8092);
   gre->SetPointError(6,0,151.9359);
   
   TH1F *Graph_Graph21 = new TH1F("Graph_Graph21","e_lifetime",100,1.459486e+09,1.463716e+09);
   Graph_Graph21->SetMinimum(0);
   Graph_Graph21->SetMaximum(1000);
   Graph_Graph21->SetDirectory(0);
   Graph_Graph21->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph21->SetLineColor(ci);
   Graph_Graph21->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph21->GetXaxis()->SetTimeFormat("%m-%d%F1995-01-01 00:00:00s0");
   Graph_Graph21->GetXaxis()->SetLabelFont(42);
   Graph_Graph21->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph21->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph21->GetXaxis()->SetTitleFont(42);
   Graph_Graph21->GetYaxis()->SetLabelFont(42);
   Graph_Graph21->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph21->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph21->GetYaxis()->SetTitleFont(42);
   Graph_Graph21->GetZaxis()->SetLabelFont(42);
   Graph_Graph21->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph21->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph21->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph21);
   
   gre->Draw("ap");
   
   TF1 *f_dv_0.345 = new TF1("f_dv_0.345","pol0",1.459486e+09,1.463716e+09);
   f_dv_0.345->SetFillColor(19);
   f_dv_0.345->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   f_dv_0.345->SetLineColor(ci);
   f_dv_0.345->SetLineWidth(2);
   f_dv_0.345->SetChisquare(2.628488);
   f_dv_0.345->SetNDF(6);
   f_dv_0.345->GetXaxis()->SetLabelFont(42);
   f_dv_0.345->GetXaxis()->SetLabelSize(0.035);
   f_dv_0.345->GetXaxis()->SetTitleSize(0.035);
   f_dv_0.345->GetXaxis()->SetTitleFont(42);
   f_dv_0.345->GetYaxis()->SetLabelFont(42);
   f_dv_0.345->GetYaxis()->SetLabelSize(0.035);
   f_dv_0.345->GetYaxis()->SetTitleSize(0.035);
   f_dv_0.345->GetYaxis()->SetTitleFont(42);
   f_dv_0.345->SetParameter(0,92.89575);
   f_dv_0.345->SetParError(0,18.19492);
   f_dv_0.345->SetParLimits(0,0,0);
   f_dv_0.345->Draw("same");
   
   TPaveText *pt = new TPaveText(0.15,0.8,0.5,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("V_{c} = 0.345kV: 9.29e+01 +/- 1.82e+01 mm/s");
   pt->Draw();
   
   pt = new TPaveText(0.4183854,0.94,0.5816146,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("e_lifetime");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph1");
   gre->SetTitle("e_lifetime");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,434.9095);
   gre->SetPointError(0,0,773.3518);
   gre->SetPoint(1,1.46037e+09,171.2835);
   gre->SetPointError(1,0,133.9619);
   gre->SetPoint(2,1.46099e+09,491.837);
   gre->SetPointError(2,0,941.8344);
   gre->SetPoint(3,1.4616e+09,279.3929);
   gre->SetPointError(3,0,320.3168);
   gre->SetPoint(4,1.462201e+09,334.1177);
   gre->SetPointError(4,0,379.4631);
   gre->SetPoint(5,1.462809e+09,452.282);
   gre->SetPointError(5,0,811.0082);
   gre->SetPoint(6,1.463416e+09,262.797);
   gre->SetPointError(6,0,283.2598);
   
   TH1F *Graph_Graph22 = new TH1F("Graph_Graph22","e_lifetime",100,1.459486e+09,1.463716e+09);
   Graph_Graph22->SetMinimum(0);
   Graph_Graph22->SetMaximum(1000);
   Graph_Graph22->SetDirectory(0);
   Graph_Graph22->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph22->SetLineColor(ci);
   Graph_Graph22->GetXaxis()->SetLabelFont(42);
   Graph_Graph22->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph22->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph22->GetXaxis()->SetTitleFont(42);
   Graph_Graph22->GetYaxis()->SetLabelFont(42);
   Graph_Graph22->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph22->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph22->GetYaxis()->SetTitleFont(42);
   Graph_Graph22->GetZaxis()->SetLabelFont(42);
   Graph_Graph22->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph22->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph22->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph22);
   
   gre->Draw("p ");
   
   TF1 *f_dv_0.7 = new TF1("f_dv_0.7","pol0",1.459486e+09,1.463716e+09);
   f_dv_0.7->SetFillColor(19);
   f_dv_0.7->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_dv_0.7->SetLineColor(ci);
   f_dv_0.7->SetLineWidth(2);
   f_dv_0.7->SetChisquare(0.5219676);
   f_dv_0.7->SetNDF(6);
   f_dv_0.7->GetXaxis()->SetLabelFont(42);
   f_dv_0.7->GetXaxis()->SetLabelSize(0.035);
   f_dv_0.7->GetXaxis()->SetTitleSize(0.035);
   f_dv_0.7->GetXaxis()->SetTitleFont(42);
   f_dv_0.7->GetYaxis()->SetLabelFont(42);
   f_dv_0.7->GetYaxis()->SetLabelSize(0.035);
   f_dv_0.7->GetYaxis()->SetTitleSize(0.035);
   f_dv_0.7->GetYaxis()->SetTitleFont(42);
   f_dv_0.7->SetParameter(0,222.3439);
   f_dv_0.7->SetParError(0,105.8824);
   f_dv_0.7->SetParLimits(0,0,0);
   f_dv_0.7->Draw("same");
   
   pt = new TPaveText(0.15,0.75,0.5,0.8,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 0.7kV: 2.22e+02 +/- 1.06e+02 mm/s");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph2");
   gre->SetTitle("e_lifetime");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#00cc00");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#00cc00");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,4999.999);
   gre->SetPointError(0,0,2511.195);
   gre->SetPoint(1,1.46037e+09,299.3691);
   gre->SetPointError(1,0,297.6579);
   gre->SetPoint(2,1.46099e+09,470.8197);
   gre->SetPointError(2,0,800.4553);
   gre->SetPoint(3,1.4616e+09,456.2523);
   gre->SetPointError(3,0,734.7712);
   gre->SetPoint(4,1.462201e+09,231.3023);
   gre->SetPointError(4,0,226.0383);
   gre->SetPoint(5,1.462809e+09,288.941);
   gre->SetPointError(5,0,325.0936);
   gre->SetPoint(6,1.463416e+09,779.6617);
   gre->SetPointError(6,0,2747.903);
   
   TH1F *Graph_Graph23 = new TH1F("Graph_Graph23","e_lifetime",100,1.459486e+09,1.463716e+09);
   Graph_Graph23->SetMinimum(0);
   Graph_Graph23->SetMaximum(1000);
   Graph_Graph23->SetDirectory(0);
   Graph_Graph23->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph23->SetLineColor(ci);
   Graph_Graph23->GetXaxis()->SetLabelFont(42);
   Graph_Graph23->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph23->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph23->GetXaxis()->SetTitleFont(42);
   Graph_Graph23->GetYaxis()->SetLabelFont(42);
   Graph_Graph23->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph23->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph23->GetYaxis()->SetTitleFont(42);
   Graph_Graph23->GetZaxis()->SetLabelFont(42);
   Graph_Graph23->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph23->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph23->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph23);
   
   gre->Draw("p ");
   
   TF1 *f_dv_1.054 = new TF1("f_dv_1.054","pol0",1.459486e+09,1.463716e+09);
   f_dv_1.054->SetFillColor(19);
   f_dv_1.054->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   f_dv_1.054->SetLineColor(ci);
   f_dv_1.054->SetLineWidth(2);
   f_dv_1.054->SetChisquare(3.717528);
   f_dv_1.054->SetNDF(6);
   f_dv_1.054->GetXaxis()->SetLabelFont(42);
   f_dv_1.054->GetXaxis()->SetLabelSize(0.035);
   f_dv_1.054->GetXaxis()->SetTitleSize(0.035);
   f_dv_1.054->GetXaxis()->SetTitleFont(42);
   f_dv_1.054->GetYaxis()->SetLabelFont(42);
   f_dv_1.054->GetYaxis()->SetLabelSize(0.035);
   f_dv_1.054->GetYaxis()->SetTitleSize(0.035);
   f_dv_1.054->GetYaxis()->SetTitleFont(42);
   f_dv_1.054->SetParameter(0,297.9225);
   f_dv_1.054->SetParError(0,150.7135);
   f_dv_1.054->SetParLimits(0,0,0);
   f_dv_1.054->Draw("same");
   
   pt = new TPaveText(0.15,0.7,0.5,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.054kV: 2.98e+02 +/- 1.51e+02 mm/s");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph3");
   gre->SetTitle("e_lifetime");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff9933");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff9933");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,868.4257);
   gre->SetPointError(0,0,2552.546);
   gre->SetPoint(1,1.46037e+09,429.7869);
   gre->SetPointError(1,0,789.1448);
   gre->SetPoint(2,1.46099e+09,441.8356);
   gre->SetPointError(2,0,879.6781);
   gre->SetPoint(3,1.4616e+09,408.4372);
   gre->SetPointError(3,0,690.0553);
   gre->SetPoint(4,1.462201e+09,409.0681);
   gre->SetPointError(4,0,721.2215);
   gre->SetPoint(5,1.462809e+09,1135.328);
   gre->SetPointError(5,0,3471.993);
   gre->SetPoint(6,1.463416e+09,1651.154);
   gre->SetPointError(6,0,3517.066);
   
   TH1F *Graph_Graph24 = new TH1F("Graph_Graph24","e_lifetime",100,1.459486e+09,1.463716e+09);
   Graph_Graph24->SetMinimum(0);
   Graph_Graph24->SetMaximum(1000);
   Graph_Graph24->SetDirectory(0);
   Graph_Graph24->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph24->SetLineColor(ci);
   Graph_Graph24->GetXaxis()->SetLabelFont(42);
   Graph_Graph24->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph24->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph24->GetXaxis()->SetTitleFont(42);
   Graph_Graph24->GetYaxis()->SetLabelFont(42);
   Graph_Graph24->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph24->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph24->GetYaxis()->SetTitleFont(42);
   Graph_Graph24->GetZaxis()->SetLabelFont(42);
   Graph_Graph24->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph24->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph24->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph24);
   
   gre->Draw("p ");
   
   TF1 *f_dv_1.5 = new TF1("f_dv_1.5","pol0",1.459486e+09,1.463716e+09);
   f_dv_1.5->SetFillColor(19);
   f_dv_1.5->SetFillStyle(0);

   ci = TColor::GetColor("#ff9933");
   f_dv_1.5->SetLineColor(ci);
   f_dv_1.5->SetLineWidth(2);
   f_dv_1.5->SetChisquare(0.1900495);
   f_dv_1.5->SetNDF(6);
   f_dv_1.5->GetXaxis()->SetLabelFont(42);
   f_dv_1.5->GetXaxis()->SetLabelSize(0.035);
   f_dv_1.5->GetXaxis()->SetTitleSize(0.035);
   f_dv_1.5->GetXaxis()->SetTitleFont(42);
   f_dv_1.5->GetYaxis()->SetLabelFont(42);
   f_dv_1.5->GetYaxis()->SetLabelSize(0.035);
   f_dv_1.5->GetYaxis()->SetTitleSize(0.035);
   f_dv_1.5->GetYaxis()->SetTitleFont(42);
   f_dv_1.5->SetParameter(0,451.27);
   f_dv_1.5->SetParError(0,371.7017);
   f_dv_1.5->SetParLimits(0,0,0);
   f_dv_1.5->Draw("same");
   
   pt = new TPaveText(0.15,0.65,0.5,0.7,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff9933");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.5kV: 4.51e+02 +/- 3.72e+02 mm/s");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph4");
   gre->SetTitle("e_lifetime");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff0000");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff0000");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,1050.663);
   gre->SetPointError(0,0,2954.801);
   gre->SetPoint(1,1.46037e+09,323.391);
   gre->SetPointError(1,0,442.1993);
   gre->SetPoint(2,1.46099e+09,1785.174);
   gre->SetPointError(2,0,2711.261);
   gre->SetPoint(3,1.4616e+09,554.855);
   gre->SetPointError(3,0,1231.708);
   gre->SetPoint(4,1.462201e+09,287.7782);
   gre->SetPointError(4,0,412.8251);
   gre->SetPoint(5,1.462809e+09,2448.836);
   gre->SetPointError(5,0,3674.787);
   gre->SetPoint(6,1.463416e+09,4999.999);
   gre->SetPointError(6,0,4953.601);
   
   TH1F *Graph_Graph25 = new TH1F("Graph_Graph25","e_lifetime",100,1.459486e+09,1.463716e+09);
   Graph_Graph25->SetMinimum(0);
   Graph_Graph25->SetMaximum(1000);
   Graph_Graph25->SetDirectory(0);
   Graph_Graph25->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph25->SetLineColor(ci);
   Graph_Graph25->GetXaxis()->SetLabelFont(42);
   Graph_Graph25->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph25->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph25->GetXaxis()->SetTitleFont(42);
   Graph_Graph25->GetYaxis()->SetLabelFont(42);
   Graph_Graph25->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph25->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph25->GetYaxis()->SetTitleFont(42);
   Graph_Graph25->GetZaxis()->SetLabelFont(42);
   Graph_Graph25->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph25->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph25->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph25);
   
   gre->Draw("p ");
   
   TF1 *f_dv_2.356 = new TF1("f_dv_2.356","pol0",1.459486e+09,1.463716e+09);
   f_dv_2.356->SetFillColor(19);
   f_dv_2.356->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   f_dv_2.356->SetLineColor(ci);
   f_dv_2.356->SetLineWidth(2);
   f_dv_2.356->SetChisquare(1.592428);
   f_dv_2.356->SetNDF(6);
   f_dv_2.356->GetXaxis()->SetLabelFont(42);
   f_dv_2.356->GetXaxis()->SetLabelSize(0.035);
   f_dv_2.356->GetXaxis()->SetTitleSize(0.035);
   f_dv_2.356->GetXaxis()->SetTitleFont(42);
   f_dv_2.356->GetYaxis()->SetLabelFont(42);
   f_dv_2.356->GetYaxis()->SetLabelSize(0.035);
   f_dv_2.356->GetYaxis()->SetTitleSize(0.035);
   f_dv_2.356->GetYaxis()->SetTitleFont(42);
   f_dv_2.356->SetParameter(0,371.1776);
   f_dv_2.356->SetParError(0,288.5998);
   f_dv_2.356->SetParLimits(0,0,0);
   f_dv_2.356->Draw("same");
   
   pt = new TPaveText(0.15,0.6,0.5,0.65,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 2.356kV: 3.71e+02 +/- 2.89e+02 mm/s");
   pt->Draw();
   957e05f5a6f94ab1916a52db114cc8fc_1->Modified();
   957e05f5a6f94ab1916a52db114cc8fc->cd();
  
// ------------>Primitives in pad: 957e05f5a6f94ab1916a52db114cc8fc_2
   957e05f5a6f94ab1916a52db114cc8fc_2 = new TPad("957e05f5a6f94ab1916a52db114cc8fc_2", "957e05f5a6f94ab1916a52db114cc8fc_2",0.51,0.51,0.99,0.99);
   957e05f5a6f94ab1916a52db114cc8fc_2->Draw();
   957e05f5a6f94ab1916a52db114cc8fc_2->cd();
   957e05f5a6f94ab1916a52db114cc8fc_2->Range(0,0,1,1);
   957e05f5a6f94ab1916a52db114cc8fc_2->SetFillColor(0);
   957e05f5a6f94ab1916a52db114cc8fc_2->SetBorderMode(0);
   957e05f5a6f94ab1916a52db114cc8fc_2->SetBorderSize(2);
   957e05f5a6f94ab1916a52db114cc8fc_2->SetFrameBorderMode(0);
   957e05f5a6f94ab1916a52db114cc8fc_2->Modified();
   957e05f5a6f94ab1916a52db114cc8fc->cd();
  
// ------------>Primitives in pad: 957e05f5a6f94ab1916a52db114cc8fc_3
   957e05f5a6f94ab1916a52db114cc8fc_3 = new TPad("957e05f5a6f94ab1916a52db114cc8fc_3", "957e05f5a6f94ab1916a52db114cc8fc_3",0.01,0.01,0.49,0.49);
   957e05f5a6f94ab1916a52db114cc8fc_3->Draw();
   957e05f5a6f94ab1916a52db114cc8fc_3->cd();
   957e05f5a6f94ab1916a52db114cc8fc_3->Range(1.458957e+09,0.75,1.464245e+09,1.25);
   957e05f5a6f94ab1916a52db114cc8fc_3->SetFillColor(0);
   957e05f5a6f94ab1916a52db114cc8fc_3->SetBorderMode(0);
   957e05f5a6f94ab1916a52db114cc8fc_3->SetBorderSize(2);
   957e05f5a6f94ab1916a52db114cc8fc_3->SetFrameBorderMode(0);
   957e05f5a6f94ab1916a52db114cc8fc_3->SetFrameBorderMode(0);
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph0");
   gre->SetTitle("s2_r_intercept");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff00ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff00ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,0.9681801);
   gre->SetPointError(0,0,0.0735858);
   gre->SetPoint(1,1.46037e+09,0.9736833);
   gre->SetPointError(1,0,0.06238538);
   gre->SetPoint(2,1.46099e+09,0.9662964);
   gre->SetPointError(2,0,0.06428773);
   gre->SetPoint(3,1.4616e+09,0.9731649);
   gre->SetPointError(3,0,0.07319737);
   gre->SetPoint(4,1.462201e+09,0.959707);
   gre->SetPointError(4,0,0.08064221);
   gre->SetPoint(5,1.462809e+09,0.9684833);
   gre->SetPointError(5,0,0.06874114);
   gre->SetPoint(6,1.463416e+09,0.9749098);
   gre->SetPointError(6,0,0.0719273);
   
   TH1F *Graph_Graph26 = new TH1F("Graph_Graph26","s2_r_intercept",100,1.459486e+09,1.463716e+09);
   Graph_Graph26->SetMinimum(0.8);
   Graph_Graph26->SetMaximum(1.2);
   Graph_Graph26->SetDirectory(0);
   Graph_Graph26->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph26->SetLineColor(ci);
   Graph_Graph26->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph26->GetXaxis()->SetTimeFormat("%m-%d%F1995-01-01 00:00:00s0");
   Graph_Graph26->GetXaxis()->SetLabelFont(42);
   Graph_Graph26->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph26->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph26->GetXaxis()->SetTitleFont(42);
   Graph_Graph26->GetYaxis()->SetLabelFont(42);
   Graph_Graph26->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph26->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph26->GetYaxis()->SetTitleFont(42);
   Graph_Graph26->GetZaxis()->SetLabelFont(42);
   Graph_Graph26->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph26->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph26->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph26);
   
   gre->Draw("ap");
   
   TF1 *f_dv_0.345 = new TF1("f_dv_0.345","pol0",1.459486e+09,1.463716e+09);
   f_dv_0.345->SetFillColor(19);
   f_dv_0.345->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   f_dv_0.345->SetLineColor(ci);
   f_dv_0.345->SetLineWidth(2);
   f_dv_0.345->SetChisquare(0.03042821);
   f_dv_0.345->SetNDF(6);
   f_dv_0.345->GetXaxis()->SetLabelFont(42);
   f_dv_0.345->GetXaxis()->SetLabelSize(0.035);
   f_dv_0.345->GetXaxis()->SetTitleSize(0.035);
   f_dv_0.345->GetXaxis()->SetTitleFont(42);
   f_dv_0.345->GetYaxis()->SetLabelFont(42);
   f_dv_0.345->GetYaxis()->SetLabelSize(0.035);
   f_dv_0.345->GetYaxis()->SetTitleSize(0.035);
   f_dv_0.345->GetYaxis()->SetTitleFont(42);
   f_dv_0.345->SetParameter(0,0.9695452);
   f_dv_0.345->SetParError(0,0.0264544);
   f_dv_0.345->SetParLimits(0,0,0);
   f_dv_0.345->Draw("same");
   
   pt = new TPaveText(0.15,0.8,0.5,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 0.345kV: 9.70e-01 +/- 2.65e-02 mm/s");
   pt->Draw();
   
   pt = new TPaveText(0.3832292,0.9379564,0.6167708,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("s2_r_intercept");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph1");
   gre->SetTitle("s2_r_intercept");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,0.9812169);
   gre->SetPointError(0,0,0.05007665);
   gre->SetPoint(1,1.46037e+09,0.9655362);
   gre->SetPointError(1,0,0.063477);
   gre->SetPoint(2,1.46099e+09,0.9753078);
   gre->SetPointError(2,0,0.05183554);
   gre->SetPoint(3,1.4616e+09,0.9751378);
   gre->SetPointError(3,0,0.06423032);
   gre->SetPoint(4,1.462201e+09,0.9729416);
   gre->SetPointError(4,0,0.05661126);
   gre->SetPoint(5,1.462809e+09,0.9701298);
   gre->SetPointError(5,0,0.05470859);
   gre->SetPoint(6,1.463416e+09,0.979196);
   gre->SetPointError(6,0,0.05685271);
   
   TH1F *Graph_Graph27 = new TH1F("Graph_Graph27","s2_r_intercept",100,1.459486e+09,1.463716e+09);
   Graph_Graph27->SetMinimum(0.8);
   Graph_Graph27->SetMaximum(1.2);
   Graph_Graph27->SetDirectory(0);
   Graph_Graph27->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph27->SetLineColor(ci);
   Graph_Graph27->GetXaxis()->SetLabelFont(42);
   Graph_Graph27->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph27->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph27->GetXaxis()->SetTitleFont(42);
   Graph_Graph27->GetYaxis()->SetLabelFont(42);
   Graph_Graph27->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph27->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph27->GetYaxis()->SetTitleFont(42);
   Graph_Graph27->GetZaxis()->SetLabelFont(42);
   Graph_Graph27->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph27->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph27->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph27);
   
   gre->Draw("p ");
   
   TF1 *f_dv_0.7 = new TF1("f_dv_0.7","pol0",1.459486e+09,1.463716e+09);
   f_dv_0.7->SetFillColor(19);
   f_dv_0.7->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_dv_0.7->SetLineColor(ci);
   f_dv_0.7->SetLineWidth(2);
   f_dv_0.7->SetChisquare(0.05215788);
   f_dv_0.7->SetNDF(6);
   f_dv_0.7->GetXaxis()->SetLabelFont(42);
   f_dv_0.7->GetXaxis()->SetLabelSize(0.035);
   f_dv_0.7->GetXaxis()->SetTitleSize(0.035);
   f_dv_0.7->GetXaxis()->SetTitleFont(42);
   f_dv_0.7->GetYaxis()->SetLabelFont(42);
   f_dv_0.7->GetYaxis()->SetLabelSize(0.035);
   f_dv_0.7->GetYaxis()->SetTitleSize(0.035);
   f_dv_0.7->GetYaxis()->SetTitleFont(42);
   f_dv_0.7->SetParameter(0,0.9746875);
   f_dv_0.7->SetParError(0,0.02123796);
   f_dv_0.7->SetParLimits(0,0,0);
   f_dv_0.7->Draw("same");
   
   pt = new TPaveText(0.15,0.75,0.5,0.8,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 0.7kV: 9.75e-01 +/- 2.12e-02 mm/s");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph2");
   gre->SetTitle("s2_r_intercept");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#00cc00");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#00cc00");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,0.9711897);
   gre->SetPointError(0,0,0.0485126);
   gre->SetPoint(1,1.46037e+09,0.9821226);
   gre->SetPointError(1,0,0.04679898);
   gre->SetPoint(2,1.46099e+09,0.9647452);
   gre->SetPointError(2,0,0.05046095);
   gre->SetPoint(3,1.4616e+09,0.9661528);
   gre->SetPointError(3,0,0.04756357);
   gre->SetPoint(4,1.462201e+09,0.9842405);
   gre->SetPointError(4,0,0.05480649);
   gre->SetPoint(5,1.462809e+09,0.9772123);
   gre->SetPointError(5,0,0.04680065);
   gre->SetPoint(6,1.463416e+09,0.9804772);
   gre->SetPointError(6,0,0.04883864);
   
   TH1F *Graph_Graph28 = new TH1F("Graph_Graph28","s2_r_intercept",100,1.459486e+09,1.463716e+09);
   Graph_Graph28->SetMinimum(0.8);
   Graph_Graph28->SetMaximum(1.2);
   Graph_Graph28->SetDirectory(0);
   Graph_Graph28->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph28->SetLineColor(ci);
   Graph_Graph28->GetXaxis()->SetLabelFont(42);
   Graph_Graph28->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph28->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph28->GetXaxis()->SetTitleFont(42);
   Graph_Graph28->GetYaxis()->SetLabelFont(42);
   Graph_Graph28->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph28->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph28->GetYaxis()->SetTitleFont(42);
   Graph_Graph28->GetZaxis()->SetLabelFont(42);
   Graph_Graph28->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph28->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph28->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph28);
   
   gre->Draw("p ");
   
   TF1 *f_dv_1.054 = new TF1("f_dv_1.054","pol0",1.459486e+09,1.463716e+09);
   f_dv_1.054->SetFillColor(19);
   f_dv_1.054->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   f_dv_1.054->SetLineColor(ci);
   f_dv_1.054->SetLineWidth(2);
   f_dv_1.054->SetChisquare(0.1484668);
   f_dv_1.054->SetNDF(6);
   f_dv_1.054->GetXaxis()->SetLabelFont(42);
   f_dv_1.054->GetXaxis()->SetLabelSize(0.035);
   f_dv_1.054->GetXaxis()->SetTitleSize(0.035);
   f_dv_1.054->GetXaxis()->SetTitleFont(42);
   f_dv_1.054->GetYaxis()->SetLabelFont(42);
   f_dv_1.054->GetYaxis()->SetLabelSize(0.035);
   f_dv_1.054->GetYaxis()->SetTitleSize(0.035);
   f_dv_1.054->GetYaxis()->SetTitleFont(42);
   f_dv_1.054->SetParameter(0,0.9750266);
   f_dv_1.054->SetParError(0,0.01849009);
   f_dv_1.054->SetParLimits(0,0,0);
   f_dv_1.054->Draw("same");
   
   pt = new TPaveText(0.15,0.7,0.5,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.054kV: 9.75e-01 +/- 1.85e-02 mm/s");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph3");
   gre->SetTitle("s2_r_intercept");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff9933");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff9933");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,0.9755486);
   gre->SetPointError(0,0,0.04545465);
   gre->SetPoint(1,1.46037e+09,0.9920693);
   gre->SetPointError(1,0,0.05013965);
   gre->SetPoint(2,1.46099e+09,0.972952);
   gre->SetPointError(2,0,0.04361323);
   gre->SetPoint(3,1.4616e+09,0.9749715);
   gre->SetPointError(3,0,0.05621844);
   gre->SetPoint(4,1.462201e+09,0.9822065);
   gre->SetPointError(4,0,0.04622239);
   gre->SetPoint(5,1.462809e+09,0.9610117);
   gre->SetPointError(5,0,0.05046467);
   gre->SetPoint(6,1.463416e+09,0.9805567);
   gre->SetPointError(6,0,0.0484602);
   
   TH1F *Graph_Graph29 = new TH1F("Graph_Graph29","s2_r_intercept",100,1.459486e+09,1.463716e+09);
   Graph_Graph29->SetMinimum(0.8);
   Graph_Graph29->SetMaximum(1.2);
   Graph_Graph29->SetDirectory(0);
   Graph_Graph29->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph29->SetLineColor(ci);
   Graph_Graph29->GetXaxis()->SetLabelFont(42);
   Graph_Graph29->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph29->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph29->GetXaxis()->SetTitleFont(42);
   Graph_Graph29->GetYaxis()->SetLabelFont(42);
   Graph_Graph29->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph29->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph29->GetYaxis()->SetTitleFont(42);
   Graph_Graph29->GetZaxis()->SetLabelFont(42);
   Graph_Graph29->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph29->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph29->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph29);
   
   gre->Draw("p ");
   
   TF1 *f_dv_1.5 = new TF1("f_dv_1.5","pol0",1.459486e+09,1.463716e+09);
   f_dv_1.5->SetFillColor(19);
   f_dv_1.5->SetFillStyle(0);

   ci = TColor::GetColor("#ff9933");
   f_dv_1.5->SetLineColor(ci);
   f_dv_1.5->SetLineWidth(2);
   f_dv_1.5->SetChisquare(0.2197025);
   f_dv_1.5->SetNDF(6);
   f_dv_1.5->GetXaxis()->SetLabelFont(42);
   f_dv_1.5->GetXaxis()->SetLabelSize(0.035);
   f_dv_1.5->GetXaxis()->SetTitleSize(0.035);
   f_dv_1.5->GetXaxis()->SetTitleFont(42);
   f_dv_1.5->GetYaxis()->SetLabelFont(42);
   f_dv_1.5->GetYaxis()->SetLabelSize(0.035);
   f_dv_1.5->GetYaxis()->SetTitleSize(0.035);
   f_dv_1.5->GetYaxis()->SetTitleFont(42);
   f_dv_1.5->SetParameter(0,0.9770648);
   f_dv_1.5->SetParError(0,0.0182257);
   f_dv_1.5->SetParLimits(0,0,0);
   f_dv_1.5->Draw("same");
   
   pt = new TPaveText(0.15,0.65,0.5,0.7,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff9933");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.5kV: 9.77e-01 +/- 1.82e-02 mm/s");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph4");
   gre->SetTitle("s2_r_intercept");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff0000");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff0000");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,0.969704);
   gre->SetPointError(0,0,0.04242926);
   gre->SetPoint(1,1.46037e+09,0.9896604);
   gre->SetPointError(1,0,0.04056621);
   gre->SetPoint(2,1.46099e+09,0.9768449);
   gre->SetPointError(2,0,0.04270024);
   gre->SetPoint(3,1.4616e+09,0.9767616);
   gre->SetPointError(3,0,0.04773435);
   gre->SetPoint(4,1.462201e+09,0.9839062);
   gre->SetPointError(4,0,0.04872326);
   gre->SetPoint(5,1.462809e+09,0.9839306);
   gre->SetPointError(5,0,0.04207933);
   gre->SetPoint(6,1.463416e+09,0.9742288);
   gre->SetPointError(6,0,0.03834609);
   
   TH1F *Graph_Graph30 = new TH1F("Graph_Graph30","s2_r_intercept",100,1.459486e+09,1.463716e+09);
   Graph_Graph30->SetMinimum(0.8);
   Graph_Graph30->SetMaximum(1.2);
   Graph_Graph30->SetDirectory(0);
   Graph_Graph30->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph30->SetLineColor(ci);
   Graph_Graph30->GetXaxis()->SetLabelFont(42);
   Graph_Graph30->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph30->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph30->GetXaxis()->SetTitleFont(42);
   Graph_Graph30->GetYaxis()->SetLabelFont(42);
   Graph_Graph30->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph30->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph30->GetYaxis()->SetTitleFont(42);
   Graph_Graph30->GetZaxis()->SetLabelFont(42);
   Graph_Graph30->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph30->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph30->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph30);
   
   gre->Draw("p ");
   
   TF1 *f_dv_2.356 = new TF1("f_dv_2.356","pol0",1.459486e+09,1.463716e+09);
   f_dv_2.356->SetFillColor(19);
   f_dv_2.356->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   f_dv_2.356->SetLineColor(ci);
   f_dv_2.356->SetLineWidth(2);
   f_dv_2.356->SetChisquare(0.1610083);
   f_dv_2.356->SetNDF(6);
   f_dv_2.356->GetXaxis()->SetLabelFont(42);
   f_dv_2.356->GetXaxis()->SetLabelSize(0.035);
   f_dv_2.356->GetXaxis()->SetTitleSize(0.035);
   f_dv_2.356->GetXaxis()->SetTitleFont(42);
   f_dv_2.356->GetYaxis()->SetLabelFont(42);
   f_dv_2.356->GetYaxis()->SetLabelSize(0.035);
   f_dv_2.356->GetYaxis()->SetTitleSize(0.035);
   f_dv_2.356->GetYaxis()->SetTitleFont(42);
   f_dv_2.356->SetParameter(0,0.9791982);
   f_dv_2.356->SetParError(0,0.01618693);
   f_dv_2.356->SetParLimits(0,0,0);
   f_dv_2.356->Draw("same");
   
   pt = new TPaveText(0.15,0.6,0.5,0.65,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 2.356kV: 9.79e-01 +/- 1.62e-02 mm/s");
   pt->Draw();
   957e05f5a6f94ab1916a52db114cc8fc_3->Modified();
   957e05f5a6f94ab1916a52db114cc8fc->cd();
  
// ------------>Primitives in pad: 957e05f5a6f94ab1916a52db114cc8fc_4
   957e05f5a6f94ab1916a52db114cc8fc_4 = new TPad("957e05f5a6f94ab1916a52db114cc8fc_4", "957e05f5a6f94ab1916a52db114cc8fc_4",0.51,0.01,0.99,0.49);
   957e05f5a6f94ab1916a52db114cc8fc_4->Draw();
   957e05f5a6f94ab1916a52db114cc8fc_4->cd();
   957e05f5a6f94ab1916a52db114cc8fc_4->Range(1.458957e+09,-0.225,1.464245e+09,0.525);
   957e05f5a6f94ab1916a52db114cc8fc_4->SetFillColor(0);
   957e05f5a6f94ab1916a52db114cc8fc_4->SetBorderMode(0);
   957e05f5a6f94ab1916a52db114cc8fc_4->SetBorderSize(2);
   957e05f5a6f94ab1916a52db114cc8fc_4->SetFrameBorderMode(0);
   957e05f5a6f94ab1916a52db114cc8fc_4->SetFrameBorderMode(0);
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph0");
   gre->SetTitle("s2_r_slope");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff00ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff00ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,0.08165956);
   gre->SetPointError(0,0,0.1681115);
   gre->SetPoint(1,1.46037e+09,0.07625884);
   gre->SetPointError(1,0,0.1398635);
   gre->SetPoint(2,1.46099e+09,0.1085688);
   gre->SetPointError(2,0,0.1460462);
   gre->SetPoint(3,1.4616e+09,0.07828684);
   gre->SetPointError(3,0,0.1602501);
   gre->SetPoint(4,1.462201e+09,0.1258711);
   gre->SetPointError(4,0,0.1825021);
   gre->SetPoint(5,1.462809e+09,0.0889582);
   gre->SetPointError(5,0,0.1633419);
   gre->SetPoint(6,1.463416e+09,0.09244044);
   gre->SetPointError(6,0,0.1730616);
   
   TH1F *Graph_Graph31 = new TH1F("Graph_Graph31","s2_r_slope",100,1.459486e+09,1.463716e+09);
   Graph_Graph31->SetMinimum(-0.15);
   Graph_Graph31->SetMaximum(0.45);
   Graph_Graph31->SetDirectory(0);
   Graph_Graph31->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph31->SetLineColor(ci);
   Graph_Graph31->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph31->GetXaxis()->SetTimeFormat("%m-%d%F1995-01-01 00:00:00s0");
   Graph_Graph31->GetXaxis()->SetLabelFont(42);
   Graph_Graph31->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph31->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph31->GetXaxis()->SetTitleFont(42);
   Graph_Graph31->GetYaxis()->SetLabelFont(42);
   Graph_Graph31->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph31->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph31->GetYaxis()->SetTitleFont(42);
   Graph_Graph31->GetZaxis()->SetLabelFont(42);
   Graph_Graph31->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph31->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph31->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph31);
   
   gre->Draw("ap");
   
   TF1 *f_dv_0.345 = new TF1("f_dv_0.345","pol0",1.459486e+09,1.463716e+09);
   f_dv_0.345->SetFillColor(19);
   f_dv_0.345->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   f_dv_0.345->SetLineColor(ci);
   f_dv_0.345->SetLineWidth(2);
   f_dv_0.345->SetChisquare(0.07144124);
   f_dv_0.345->SetNDF(6);
   f_dv_0.345->GetXaxis()->SetLabelFont(42);
   f_dv_0.345->GetXaxis()->SetLabelSize(0.035);
   f_dv_0.345->GetXaxis()->SetTitleSize(0.035);
   f_dv_0.345->GetXaxis()->SetTitleFont(42);
   f_dv_0.345->GetYaxis()->SetLabelFont(42);
   f_dv_0.345->GetYaxis()->SetLabelSize(0.035);
   f_dv_0.345->GetYaxis()->SetTitleSize(0.035);
   f_dv_0.345->GetYaxis()->SetTitleFont(42);
   f_dv_0.345->SetParameter(0,0.0919629);
   f_dv_0.345->SetParError(0,0.06049829);
   f_dv_0.345->SetParLimits(0,0,0);
   f_dv_0.345->Draw("same");
   
   pt = new TPaveText(0.15,0.8,0.5,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 0.345kV: 9.20e-02 +/- 6.05e-02 mm/s");
   pt->Draw();
   
   pt = new TPaveText(0.4086198,0.9379564,0.5913802,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("s2_r_slope");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph1");
   gre->SetTitle("s2_r_slope");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,0.07468975);
   gre->SetPointError(0,0,0.1099936);
   gre->SetPoint(1,1.46037e+09,0.1136576);
   gre->SetPointError(1,0,0.136195);
   gre->SetPoint(2,1.46099e+09,0.06885913);
   gre->SetPointError(2,0,0.1151361);
   gre->SetPoint(3,1.4616e+09,0.08682529);
   gre->SetPointError(3,0,0.1422421);
   gre->SetPoint(4,1.462201e+09,0.07862663);
   gre->SetPointError(4,0,0.1289778);
   gre->SetPoint(5,1.462809e+09,0.0736975);
   gre->SetPointError(5,0,0.1328115);
   gre->SetPoint(6,1.463416e+09,0.06691585);
   gre->SetPointError(6,0,0.143417);
   
   TH1F *Graph_Graph32 = new TH1F("Graph_Graph32","s2_r_slope",100,1.459486e+09,1.463716e+09);
   Graph_Graph32->SetMinimum(-0.15);
   Graph_Graph32->SetMaximum(0.45);
   Graph_Graph32->SetDirectory(0);
   Graph_Graph32->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph32->SetLineColor(ci);
   Graph_Graph32->GetXaxis()->SetLabelFont(42);
   Graph_Graph32->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph32->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph32->GetXaxis()->SetTitleFont(42);
   Graph_Graph32->GetYaxis()->SetLabelFont(42);
   Graph_Graph32->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph32->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph32->GetYaxis()->SetTitleFont(42);
   Graph_Graph32->GetZaxis()->SetLabelFont(42);
   Graph_Graph32->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph32->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph32->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph32);
   
   gre->Draw("p ");
   
   TF1 *f_dv_0.7 = new TF1("f_dv_0.7","pol0",1.459486e+09,1.463716e+09);
   f_dv_0.7->SetFillColor(19);
   f_dv_0.7->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_dv_0.7->SetLineColor(ci);
   f_dv_0.7->SetLineWidth(2);
   f_dv_0.7->SetChisquare(0.08565979);
   f_dv_0.7->SetNDF(6);
   f_dv_0.7->GetXaxis()->SetLabelFont(42);
   f_dv_0.7->GetXaxis()->SetLabelSize(0.035);
   f_dv_0.7->GetXaxis()->SetTitleSize(0.035);
   f_dv_0.7->GetXaxis()->SetTitleFont(42);
   f_dv_0.7->GetYaxis()->SetLabelFont(42);
   f_dv_0.7->GetYaxis()->SetLabelSize(0.035);
   f_dv_0.7->GetYaxis()->SetTitleSize(0.035);
   f_dv_0.7->GetYaxis()->SetTitleFont(42);
   f_dv_0.7->SetParameter(0,0.07952331);
   f_dv_0.7->SetParError(0,0.0484019);
   f_dv_0.7->SetParLimits(0,0,0);
   f_dv_0.7->Draw("same");
   
   pt = new TPaveText(0.15,0.75,0.5,0.8,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 0.7kV: 7.95e-02 +/- 4.84e-02 mm/s");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph2");
   gre->SetTitle("s2_r_slope");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#00cc00");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#00cc00");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,0.08678469);
   gre->SetPointError(0,0,0.1200765);
   gre->SetPoint(1,1.46037e+09,0.05214873);
   gre->SetPointError(1,0,0.1010242);
   gre->SetPoint(2,1.46099e+09,0.09210059);
   gre->SetPointError(2,0,0.1058597);
   gre->SetPoint(3,1.4616e+09,0.08350964);
   gre->SetPointError(3,0,0.1123897);
   gre->SetPoint(4,1.462201e+09,0.05645018);
   gre->SetPointError(4,0,0.123169);
   gre->SetPoint(5,1.462809e+09,0.07999685);
   gre->SetPointError(5,0,0.1079494);
   gre->SetPoint(6,1.463416e+09,0.06141189);
   gre->SetPointError(6,0,0.1079207);
   
   TH1F *Graph_Graph33 = new TH1F("Graph_Graph33","s2_r_slope",100,1.459486e+09,1.463716e+09);
   Graph_Graph33->SetMinimum(-0.15);
   Graph_Graph33->SetMaximum(0.45);
   Graph_Graph33->SetDirectory(0);
   Graph_Graph33->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph33->SetLineColor(ci);
   Graph_Graph33->GetXaxis()->SetLabelFont(42);
   Graph_Graph33->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph33->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph33->GetXaxis()->SetTitleFont(42);
   Graph_Graph33->GetYaxis()->SetLabelFont(42);
   Graph_Graph33->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph33->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph33->GetYaxis()->SetTitleFont(42);
   Graph_Graph33->GetZaxis()->SetLabelFont(42);
   Graph_Graph33->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph33->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph33->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph33);
   
   gre->Draw("p ");
   
   TF1 *f_dv_1.054 = new TF1("f_dv_1.054","pol0",1.459486e+09,1.463716e+09);
   f_dv_1.054->SetFillColor(19);
   f_dv_1.054->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   f_dv_1.054->SetLineColor(ci);
   f_dv_1.054->SetLineWidth(2);
   f_dv_1.054->SetChisquare(0.1308626);
   f_dv_1.054->SetNDF(6);
   f_dv_1.054->GetXaxis()->SetLabelFont(42);
   f_dv_1.054->GetXaxis()->SetLabelSize(0.035);
   f_dv_1.054->GetXaxis()->SetTitleSize(0.035);
   f_dv_1.054->GetXaxis()->SetTitleFont(42);
   f_dv_1.054->GetYaxis()->SetLabelFont(42);
   f_dv_1.054->GetYaxis()->SetLabelSize(0.035);
   f_dv_1.054->GetYaxis()->SetTitleSize(0.035);
   f_dv_1.054->GetYaxis()->SetTitleFont(42);
   f_dv_1.054->SetParameter(0,0.07293725);
   f_dv_1.054->SetParError(0,0.04176347);
   f_dv_1.054->SetParLimits(0,0,0);
   f_dv_1.054->Draw("same");
   
   pt = new TPaveText(0.15,0.7,0.5,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.054kV: 7.29e-02 +/- 4.18e-02 mm/s");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph3");
   gre->SetTitle("s2_r_slope");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff9933");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff9933");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,0.07707028);
   gre->SetPointError(0,0,0.1004342);
   gre->SetPoint(1,1.46037e+09,0.06777047);
   gre->SetPointError(1,0,0.1165885);
   gre->SetPoint(2,1.46099e+09,0.07279663);
   gre->SetPointError(2,0,0.08228916);
   gre->SetPoint(3,1.4616e+09,0.0863195);
   gre->SetPointError(3,0,0.1339404);
   gre->SetPoint(4,1.462201e+09,0.06554905);
   gre->SetPointError(4,0,0.09841715);
   gre->SetPoint(5,1.462809e+09,0.1008746);
   gre->SetPointError(5,0,0.1129183);
   gre->SetPoint(6,1.463416e+09,0.06834327);
   gre->SetPointError(6,0,0.1125653);
   
   TH1F *Graph_Graph34 = new TH1F("Graph_Graph34","s2_r_slope",100,1.459486e+09,1.463716e+09);
   Graph_Graph34->SetMinimum(-0.15);
   Graph_Graph34->SetMaximum(0.45);
   Graph_Graph34->SetDirectory(0);
   Graph_Graph34->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph34->SetLineColor(ci);
   Graph_Graph34->GetXaxis()->SetLabelFont(42);
   Graph_Graph34->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph34->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph34->GetXaxis()->SetTitleFont(42);
   Graph_Graph34->GetYaxis()->SetLabelFont(42);
   Graph_Graph34->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph34->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph34->GetYaxis()->SetTitleFont(42);
   Graph_Graph34->GetZaxis()->SetLabelFont(42);
   Graph_Graph34->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph34->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph34->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph34);
   
   gre->Draw("p ");
   
   TF1 *f_dv_1.5 = new TF1("f_dv_1.5","pol0",1.459486e+09,1.463716e+09);
   f_dv_1.5->SetFillColor(19);
   f_dv_1.5->SetFillStyle(0);

   ci = TColor::GetColor("#ff9933");
   f_dv_1.5->SetLineColor(ci);
   f_dv_1.5->SetLineWidth(2);
   f_dv_1.5->SetChisquare(0.07695115);
   f_dv_1.5->SetNDF(6);
   f_dv_1.5->GetXaxis()->SetLabelFont(42);
   f_dv_1.5->GetXaxis()->SetLabelSize(0.035);
   f_dv_1.5->GetXaxis()->SetTitleSize(0.035);
   f_dv_1.5->GetXaxis()->SetTitleFont(42);
   f_dv_1.5->GetYaxis()->SetLabelFont(42);
   f_dv_1.5->GetYaxis()->SetLabelSize(0.035);
   f_dv_1.5->GetYaxis()->SetTitleSize(0.035);
   f_dv_1.5->GetYaxis()->SetTitleFont(42);
   f_dv_1.5->SetParameter(0,0.07579652);
   f_dv_1.5->SetParError(0,0.0396303);
   f_dv_1.5->SetParLimits(0,0,0);
   f_dv_1.5->Draw("same");
   
   pt = new TPaveText(0.15,0.65,0.5,0.7,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff9933");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.5kV: 7.58e-02 +/- 3.96e-02 mm/s");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph4");
   gre->SetTitle("s2_r_slope");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff0000");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff0000");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,0.08806765);
   gre->SetPointError(0,0,0.1057417);
   gre->SetPoint(1,1.46037e+09,0.03912851);
   gre->SetPointError(1,0,0.1046357);
   gre->SetPoint(2,1.46099e+09,0.07824523);
   gre->SetPointError(2,0,0.102138);
   gre->SetPoint(3,1.4616e+09,0.08168052);
   gre->SetPointError(3,0,0.1139923);
   gre->SetPoint(4,1.462201e+09,0.0503257);
   gre->SetPointError(4,0,0.1131188);
   gre->SetPoint(5,1.462809e+09,0.04965448);
   gre->SetPointError(5,0,0.09193088);
   gre->SetPoint(6,1.463416e+09,0.07867568);
   gre->SetPointError(6,0,0.09788273);
   
   TH1F *Graph_Graph35 = new TH1F("Graph_Graph35","s2_r_slope",100,1.459486e+09,1.463716e+09);
   Graph_Graph35->SetMinimum(-0.15);
   Graph_Graph35->SetMaximum(0.45);
   Graph_Graph35->SetDirectory(0);
   Graph_Graph35->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph35->SetLineColor(ci);
   Graph_Graph35->GetXaxis()->SetLabelFont(42);
   Graph_Graph35->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph35->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph35->GetXaxis()->SetTitleFont(42);
   Graph_Graph35->GetYaxis()->SetLabelFont(42);
   Graph_Graph35->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph35->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph35->GetYaxis()->SetTitleFont(42);
   Graph_Graph35->GetZaxis()->SetLabelFont(42);
   Graph_Graph35->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph35->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph35->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph35);
   
   gre->Draw("p ");
   
   TF1 *f_dv_2.356 = new TF1("f_dv_2.356","pol0",1.459486e+09,1.463716e+09);
   f_dv_2.356->SetFillColor(19);
   f_dv_2.356->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   f_dv_2.356->SetLineColor(ci);
   f_dv_2.356->SetLineWidth(2);
   f_dv_2.356->SetChisquare(0.2103776);
   f_dv_2.356->SetNDF(6);
   f_dv_2.356->GetXaxis()->SetLabelFont(42);
   f_dv_2.356->GetXaxis()->SetLabelSize(0.035);
   f_dv_2.356->GetXaxis()->SetTitleSize(0.035);
   f_dv_2.356->GetXaxis()->SetTitleFont(42);
   f_dv_2.356->GetYaxis()->SetLabelFont(42);
   f_dv_2.356->GetYaxis()->SetLabelSize(0.035);
   f_dv_2.356->GetYaxis()->SetTitleSize(0.035);
   f_dv_2.356->GetYaxis()->SetTitleFont(42);
   f_dv_2.356->SetParameter(0,0.06609746);
   f_dv_2.356->SetParError(0,0.03909014);
   f_dv_2.356->SetParLimits(0,0,0);
   f_dv_2.356->Draw("same");
   
   pt = new TPaveText(0.15,0.6,0.5,0.65,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 2.356kV: 6.61e-02 +/- 3.91e-02 mm/s");
   pt->Draw();
   957e05f5a6f94ab1916a52db114cc8fc_4->Modified();
   957e05f5a6f94ab1916a52db114cc8fc->cd();
   957e05f5a6f94ab1916a52db114cc8fc->Modified();
   957e05f5a6f94ab1916a52db114cc8fc->cd();
   957e05f5a6f94ab1916a52db114cc8fc->SetSelected(957e05f5a6f94ab1916a52db114cc8fc);
}
