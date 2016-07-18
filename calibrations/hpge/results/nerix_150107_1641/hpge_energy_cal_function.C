{
//=========Macro generated from canvas: 6943fa372fad4ecebee6e4ebd2a81a90/
//=========  (Mon Jul 18 11:23:40 2016) by ROOT version5.34/26
   TCanvas *6943fa372fad4ecebee6e4ebd2a81a90 = new TCanvas("6943fa372fad4ecebee6e4ebd2a81a90", "",0,0,600,500);
   6943fa372fad4ecebee6e4ebd2a81a90->SetHighLightColor(2);
   6943fa372fad4ecebee6e4ebd2a81a90->Range(0.459113,320.1157,2.110906,1465.421);
   6943fa372fad4ecebee6e4ebd2a81a90->SetFillColor(0);
   6943fa372fad4ecebee6e4ebd2a81a90->SetBorderMode(0);
   6943fa372fad4ecebee6e4ebd2a81a90->SetBorderSize(2);
   6943fa372fad4ecebee6e4ebd2a81a90->SetFrameBorderMode(0);
   6943fa372fad4ecebee6e4ebd2a81a90->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(3);
   gre->SetName("Graph0");
   gre->SetTitle("HPGe Energy Calibration");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(32);
   gre->SetPoint(0,0.7344493,511);
   gre->SetPointError(0,3.744731e-05,0);
   gre->SetPoint(1,0.9517872,661.657);
   gre->SetPointError(1,2.463854e-05,0);
   gre->SetPoint(2,1.83556,1274.537);
   gre->SetPointError(2,4.730183e-05,0);
   
   TH1F *Graph_Graph9 = new TH1F("Graph_Graph9","HPGe Energy Calibration",100,0.6242923,1.945726);
   Graph_Graph9->SetMinimum(434.6463);
   Graph_Graph9->SetMaximum(1350.891);
   Graph_Graph9->SetDirectory(0);
   Graph_Graph9->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph9->SetLineColor(ci);
   Graph_Graph9->GetXaxis()->SetTitle("Peak Voltage [V]");
   Graph_Graph9->GetXaxis()->SetLabelFont(42);
   Graph_Graph9->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph9->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph9->GetXaxis()->SetTitleFont(42);
   Graph_Graph9->GetYaxis()->SetTitle("Energy [keV]");
   Graph_Graph9->GetYaxis()->SetLabelFont(42);
   Graph_Graph9->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph9->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph9->GetYaxis()->SetTitleFont(42);
   Graph_Graph9->GetZaxis()->SetLabelFont(42);
   Graph_Graph9->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph9->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph9->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph9);
   
   gre->Draw("ap");
   
   TF1 *f_energy_cal = new TF1("f_energy_cal","pol1",0,2);
   f_energy_cal->SetFillColor(19);
   f_energy_cal->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_energy_cal->SetLineColor(ci);
   f_energy_cal->SetLineWidth(2);
   f_energy_cal->SetLineStyle(2);
   f_energy_cal->SetChisquare(3.315016);
   f_energy_cal->SetNDF(1);
   f_energy_cal->GetXaxis()->SetLabelFont(42);
   f_energy_cal->GetXaxis()->SetLabelSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleFont(42);
   f_energy_cal->GetYaxis()->SetLabelFont(42);
   f_energy_cal->GetYaxis()->SetLabelSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleFont(42);
   f_energy_cal->SetParameter(0,1.664834);
   f_energy_cal->SetParError(0,0.03999429);
   f_energy_cal->SetParLimits(0,0,0);
   f_energy_cal->SetParameter(1,693.4442);
   f_energy_cal->SetParError(1,0.03643399);
   f_energy_cal->SetParLimits(1,0,0);
   f_energy_cal->Draw("same");
   
   TPaveText *pt = new TPaveText(0.2,0.67,0.55,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   TText *text = pt->AddText("Intercept = 1.6648e+00 #pm 3.9994e-02 keV");
   text = pt->AddText("Slope = 6.9344e+02 #pm 3.6434e-02 #frac{keV}{V}");
   pt->Draw();
   
   pt = new TPaveText(0.2333333,0.9326,0.7666667,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("HPGe Energy Calibration");
   pt->Draw();
   6943fa372fad4ecebee6e4ebd2a81a90->Modified();
   6943fa372fad4ecebee6e4ebd2a81a90->cd();
   6943fa372fad4ecebee6e4ebd2a81a90->SetSelected(6943fa372fad4ecebee6e4ebd2a81a90);
}
