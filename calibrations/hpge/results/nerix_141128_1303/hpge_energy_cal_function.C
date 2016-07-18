{
//=========Macro generated from canvas: fa5e9ab84b904d709ba10ba1d8c435a1/
//=========  (Mon Jul 18 11:23:38 2016) by ROOT version5.34/26
   TCanvas *fa5e9ab84b904d709ba10ba1d8c435a1 = new TCanvas("fa5e9ab84b904d709ba10ba1d8c435a1", "",0,0,600,500);
   fa5e9ab84b904d709ba10ba1d8c435a1->SetHighLightColor(2);
   fa5e9ab84b904d709ba10ba1d8c435a1->Range(0.4594057,320.1157,2.112037,1465.421);
   fa5e9ab84b904d709ba10ba1d8c435a1->SetFillColor(0);
   fa5e9ab84b904d709ba10ba1d8c435a1->SetBorderMode(0);
   fa5e9ab84b904d709ba10ba1d8c435a1->SetBorderSize(2);
   fa5e9ab84b904d709ba10ba1d8c435a1->SetFrameBorderMode(0);
   fa5e9ab84b904d709ba10ba1d8c435a1->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(3);
   gre->SetName("Graph0");
   gre->SetTitle("HPGe Energy Calibration");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(32);
   gre->SetPoint(0,0.7348796,511);
   gre->SetPointError(0,3.528139e-05,0);
   gre->SetPoint(1,0.9522565,661.657);
   gre->SetPointError(1,1.961092e-05,0);
   gre->SetPoint(2,1.836547,1274.537);
   gre->SetPointError(2,5.151918e-05,0);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","HPGe Energy Calibration",100,0.6246688,1.946774);
   Graph_Graph2->SetMinimum(434.6463);
   Graph_Graph2->SetMaximum(1350.891);
   Graph_Graph2->SetDirectory(0);
   Graph_Graph2->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph2->SetLineColor(ci);
   Graph_Graph2->GetXaxis()->SetTitle("Peak Voltage [V]");
   Graph_Graph2->GetXaxis()->SetLabelFont(42);
   Graph_Graph2->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph2->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph2->GetXaxis()->SetTitleFont(42);
   Graph_Graph2->GetYaxis()->SetTitle("Energy [keV]");
   Graph_Graph2->GetYaxis()->SetLabelFont(42);
   Graph_Graph2->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph2->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph2->GetYaxis()->SetTitleFont(42);
   Graph_Graph2->GetZaxis()->SetLabelFont(42);
   Graph_Graph2->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph2->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph2->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph2);
   
   gre->Draw("ap");
   
   TF1 *f_energy_cal = new TF1("f_energy_cal","pol1",0,2);
   f_energy_cal->SetFillColor(19);
   f_energy_cal->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_energy_cal->SetLineColor(ci);
   f_energy_cal->SetLineWidth(2);
   f_energy_cal->SetLineStyle(2);
   f_energy_cal->SetChisquare(0.002552605);
   f_energy_cal->SetNDF(1);
   f_energy_cal->GetXaxis()->SetLabelFont(42);
   f_energy_cal->GetXaxis()->SetLabelSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleFont(42);
   f_energy_cal->GetYaxis()->SetLabelFont(42);
   f_energy_cal->GetYaxis()->SetLabelSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleFont(42);
   f_energy_cal->SetParameter(0,1.672918);
   f_energy_cal->SetParError(0,0.03979664);
   f_energy_cal->SetParLimits(0,0,0);
   f_energy_cal->SetParameter(1,693.0743);
   f_energy_cal->SetParError(1,0.03838978);
   f_energy_cal->SetParLimits(1,0,0);
   f_energy_cal->Draw("same");
   
   TPaveText *pt = new TPaveText(0.2,0.67,0.55,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   TText *text = pt->AddText("Intercept = 1.6729e+00 #pm 3.9797e-02 keV");
   text = pt->AddText("Slope = 6.9307e+02 #pm 3.8390e-02 #frac{keV}{V}");
   pt->Draw();
   
   pt = new TPaveText(0.2333333,0.9326,0.7666667,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("HPGe Energy Calibration");
   pt->Draw();
   fa5e9ab84b904d709ba10ba1d8c435a1->Modified();
   fa5e9ab84b904d709ba10ba1d8c435a1->cd();
   fa5e9ab84b904d709ba10ba1d8c435a1->SetSelected(fa5e9ab84b904d709ba10ba1d8c435a1);
}
