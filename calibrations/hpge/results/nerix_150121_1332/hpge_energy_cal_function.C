{
//=========Macro generated from canvas: de7b29e5e68149c9b5db38a3180d9b14/
//=========  (Mon Jul 18 11:23:41 2016) by ROOT version5.34/26
   TCanvas *de7b29e5e68149c9b5db38a3180d9b14 = new TCanvas("de7b29e5e68149c9b5db38a3180d9b14", "",0,0,600,500);
   de7b29e5e68149c9b5db38a3180d9b14->SetHighLightColor(2);
   de7b29e5e68149c9b5db38a3180d9b14->Range(0.4590708,320.1157,2.110806,1465.421);
   de7b29e5e68149c9b5db38a3180d9b14->SetFillColor(0);
   de7b29e5e68149c9b5db38a3180d9b14->SetBorderMode(0);
   de7b29e5e68149c9b5db38a3180d9b14->SetBorderSize(2);
   de7b29e5e68149c9b5db38a3180d9b14->SetFrameBorderMode(0);
   de7b29e5e68149c9b5db38a3180d9b14->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(3);
   gre->SetName("Graph0");
   gre->SetTitle("HPGe Energy Calibration");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(32);
   gre->SetPoint(0,0.7343989,511);
   gre->SetPointError(0,3.887496e-05,0);
   gre->SetPoint(1,0.9516486,661.657);
   gre->SetPointError(1,1.751763e-05,0);
   gre->SetPoint(2,1.835471,1274.537);
   gre->SetPointError(2,4.560433e-05,0);
   
   TH1F *Graph_Graph11 = new TH1F("Graph_Graph11","HPGe Energy Calibration",100,0.6242443,1.945633);
   Graph_Graph11->SetMinimum(434.6463);
   Graph_Graph11->SetMaximum(1350.891);
   Graph_Graph11->SetDirectory(0);
   Graph_Graph11->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph11->SetLineColor(ci);
   Graph_Graph11->GetXaxis()->SetTitle("Peak Voltage [V]");
   Graph_Graph11->GetXaxis()->SetLabelFont(42);
   Graph_Graph11->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph11->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph11->GetXaxis()->SetTitleFont(42);
   Graph_Graph11->GetYaxis()->SetTitle("Energy [keV]");
   Graph_Graph11->GetYaxis()->SetLabelFont(42);
   Graph_Graph11->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph11->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph11->GetYaxis()->SetTitleFont(42);
   Graph_Graph11->GetZaxis()->SetLabelFont(42);
   Graph_Graph11->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph11->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph11->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph11);
   
   gre->Draw("ap");
   
   TF1 *f_energy_cal = new TF1("f_energy_cal","pol1",0,2);
   f_energy_cal->SetFillColor(19);
   f_energy_cal->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_energy_cal->SetLineColor(ci);
   f_energy_cal->SetLineWidth(2);
   f_energy_cal->SetLineStyle(2);
   f_energy_cal->SetChisquare(0.04641022);
   f_energy_cal->SetNDF(1);
   f_energy_cal->GetXaxis()->SetLabelFont(42);
   f_energy_cal->GetXaxis()->SetLabelSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleFont(42);
   f_energy_cal->GetYaxis()->SetLabelFont(42);
   f_energy_cal->GetYaxis()->SetLabelSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleFont(42);
   f_energy_cal->SetParameter(0,1.739264);
   f_energy_cal->SetParError(0,0.03720959);
   f_energy_cal->SetParLimits(0,0,0);
   f_energy_cal->SetParameter(1,693.4456);
   f_energy_cal->SetParError(1,0.03516486);
   f_energy_cal->SetParLimits(1,0,0);
   f_energy_cal->Draw("same");
   
   TPaveText *pt = new TPaveText(0.2,0.67,0.55,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   TText *text = pt->AddText("Intercept = 1.7393e+00 #pm 3.7210e-02 keV");
   text = pt->AddText("Slope = 6.9345e+02 #pm 3.5165e-02 #frac{keV}{V}");
   pt->Draw();
   
   pt = new TPaveText(0.2333333,0.9326,0.7666667,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("HPGe Energy Calibration");
   pt->Draw();
   de7b29e5e68149c9b5db38a3180d9b14->Modified();
   de7b29e5e68149c9b5db38a3180d9b14->cd();
   de7b29e5e68149c9b5db38a3180d9b14->SetSelected(de7b29e5e68149c9b5db38a3180d9b14);
}
