{
//=========Macro generated from canvas: c8f16865617746fca6a9a4cc62c72b10/
//=========  (Wed Feb 10 09:20:33 2016) by ROOT version5.34/26
   TCanvas *c8f16865617746fca6a9a4cc62c72b10 = new TCanvas("c8f16865617746fca6a9a4cc62c72b10", "",10,45,704,527);
   c8f16865617746fca6a9a4cc62c72b10->SetHighLightColor(2);
   c8f16865617746fca6a9a4cc62c72b10->Range(287887,802.6885,632123.8,2140.058);
   c8f16865617746fca6a9a4cc62c72b10->SetFillColor(0);
   c8f16865617746fca6a9a4cc62c72b10->SetBorderMode(0);
   c8f16865617746fca6a9a4cc62c72b10->SetBorderSize(2);
   c8f16865617746fca6a9a4cc62c72b10->SetFrameBorderMode(0);
   c8f16865617746fca6a9a4cc62c72b10->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,391878.7,1685.054);
   gre->SetPointError(0,2247.946,9.329995);
   gre->SetPoint(1,429491.1,1502.425);
   gre->SetPointError(1,2105.293,9.01576);
   gre->SetPoint(2,455368.7,1398.931);
   gre->SetPointError(2,2125.349,9.449414);
   gre->SetPoint(3,477639.1,1303.199);
   gre->SetPointError(3,1926.395,7.697664);
   gre->SetPoint(4,498210.8,1233.515);
   gre->SetPointError(4,1789.509,7.024503);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,322310.7,597700.2);
   Graph_Graph1->SetMinimum(936.4254);
   Graph_Graph1->SetMaximum(2006.321);
   Graph_Graph1->SetDirectory(0);
   Graph_Graph1->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph1->SetLineColor(ci);
   Graph_Graph1->GetXaxis()->SetTitle("S2 at 511 keV Peak");
   Graph_Graph1->GetXaxis()->SetLabelFont(42);
   Graph_Graph1->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleOffset(1.12);
   Graph_Graph1->GetXaxis()->SetTitleFont(42);
   Graph_Graph1->GetYaxis()->SetTitle("S1 at 511 keV Peak [PE]");
   Graph_Graph1->GetYaxis()->SetLabelFont(42);
   Graph_Graph1->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleFont(42);
   Graph_Graph1->GetZaxis()->SetLabelFont(42);
   Graph_Graph1->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1);
   
   
   TF1 *fG1 = new TF1("fG1","pol1",378593.7,511037.3);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(3.204751);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,3330.501);
   fG1->SetParError(0,66.6148);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.004229389);
   fG1->SetParError(1,0.000145287);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0893 +/- 0.0043");
   text = pt->AddText("g2 = 21.1121 +/- 1.2487");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(458883.5,1397.773,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(543363.8,1040.473,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(358122.9,1823.928,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",322310.7,597700.2);
   fDraw->SetFillColor(19);
   fDraw->SetFillStyle(0);
   fDraw->SetLineColor(2);
   fDraw->SetLineWidth(1);
   fDraw->SetLineStyle(9);
   fDraw->GetXaxis()->SetLabelFont(42);
   fDraw->GetXaxis()->SetLabelSize(0.035);
   fDraw->GetXaxis()->SetTitleSize(0.035);
   fDraw->GetXaxis()->SetTitleFont(42);
   fDraw->GetYaxis()->SetLabelFont(42);
   fDraw->GetYaxis()->SetLabelSize(0.035);
   fDraw->GetYaxis()->SetTitleSize(0.035);
   fDraw->GetYaxis()->SetTitleFont(42);
   fDraw->SetParameter(0,3330.501);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.004229389);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   c8f16865617746fca6a9a4cc62c72b10->Modified();
   c8f16865617746fca6a9a4cc62c72b10->cd();
   c8f16865617746fca6a9a4cc62c72b10->SetSelected(c8f16865617746fca6a9a4cc62c72b10);
}
