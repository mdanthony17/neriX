{
//=========Macro generated from canvas: 1e3c83806926411485287af635b123d2/
//=========  (Mon Jul 18 11:23:40 2016) by ROOT version5.34/26
   TCanvas *1e3c83806926411485287af635b123d2 = new TCanvas("1e3c83806926411485287af635b123d2", "",0,0,600,500);
   1e3c83806926411485287af635b123d2->SetHighLightColor(2);
   1e3c83806926411485287af635b123d2->Range(0.4589605,320.1157,2.110909,1465.421);
   1e3c83806926411485287af635b123d2->SetFillColor(0);
   1e3c83806926411485287af635b123d2->SetBorderMode(0);
   1e3c83806926411485287af635b123d2->SetBorderSize(2);
   1e3c83806926411485287af635b123d2->SetFrameBorderMode(0);
   1e3c83806926411485287af635b123d2->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(3);
   gre->SetName("Graph0");
   gre->SetTitle("HPGe Energy Calibration");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(32);
   gre->SetPoint(0,0.7343194,511);
   gre->SetPointError(0,3.41646e-05,0);
   gre->SetPoint(1,0.9516751,661.657);
   gre->SetPointError(1,2.281921e-05,0);
   gre->SetPoint(2,1.835539,1274.537);
   gre->SetPointError(2,4.502305e-05,0);
   
   TH1F *Graph_Graph8 = new TH1F("Graph_Graph8","HPGe Energy Calibration",100,0.6241553,1.945714);
   Graph_Graph8->SetMinimum(434.6463);
   Graph_Graph8->SetMaximum(1350.891);
   Graph_Graph8->SetDirectory(0);
   Graph_Graph8->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph8->SetLineColor(ci);
   Graph_Graph8->GetXaxis()->SetTitle("Peak Voltage [V]");
   Graph_Graph8->GetXaxis()->SetLabelFont(42);
   Graph_Graph8->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph8->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph8->GetXaxis()->SetTitleFont(42);
   Graph_Graph8->GetYaxis()->SetTitle("Energy [keV]");
   Graph_Graph8->GetYaxis()->SetLabelFont(42);
   Graph_Graph8->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph8->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph8->GetYaxis()->SetTitleFont(42);
   Graph_Graph8->GetZaxis()->SetLabelFont(42);
   Graph_Graph8->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph8->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph8->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph8);
   
   gre->Draw("ap");
   
   TF1 *f_energy_cal = new TF1("f_energy_cal","pol1",0,2);
   f_energy_cal->SetFillColor(19);
   f_energy_cal->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_energy_cal->SetLineColor(ci);
   f_energy_cal->SetLineWidth(2);
   f_energy_cal->SetLineStyle(2);
   f_energy_cal->SetChisquare(3.524657);
   f_energy_cal->SetNDF(1);
   f_energy_cal->GetXaxis()->SetLabelFont(42);
   f_energy_cal->GetXaxis()->SetLabelSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleFont(42);
   f_energy_cal->GetYaxis()->SetLabelFont(42);
   f_energy_cal->GetYaxis()->SetLabelSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleFont(42);
   f_energy_cal->SetParameter(0,1.809518);
   f_energy_cal->SetParError(0,0.03736668);
   f_energy_cal->SetParLimits(0,0,0);
   f_energy_cal->SetParameter(1,693.3731);
   f_energy_cal->SetParError(1,0.03437983);
   f_energy_cal->SetParLimits(1,0,0);
   f_energy_cal->Draw("same");
   
   TPaveText *pt = new TPaveText(0.2,0.67,0.55,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   TText *text = pt->AddText("Intercept = 1.8095e+00 #pm 3.7367e-02 keV");
   text = pt->AddText("Slope = 6.9337e+02 #pm 3.4380e-02 #frac{keV}{V}");
   pt->Draw();
   
   pt = new TPaveText(0.2333333,0.9326,0.7666667,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("HPGe Energy Calibration");
   pt->Draw();
   1e3c83806926411485287af635b123d2->Modified();
   1e3c83806926411485287af635b123d2->cd();
   1e3c83806926411485287af635b123d2->SetSelected(1e3c83806926411485287af635b123d2);
}
