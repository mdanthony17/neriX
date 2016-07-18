{
//=========Macro generated from canvas: 8d196b8732e842269d4321322460ef57/
//=========  (Mon Jul 18 11:03:35 2016) by ROOT version5.34/26
   TCanvas *8d196b8732e842269d4321322460ef57 = new TCanvas("8d196b8732e842269d4321322460ef57", "",0,0,600,500);
   8d196b8732e842269d4321322460ef57->SetHighLightColor(2);
   8d196b8732e842269d4321322460ef57->Range(0.4594543,320.1157,2.112321,1465.421);
   8d196b8732e842269d4321322460ef57->SetFillColor(0);
   8d196b8732e842269d4321322460ef57->SetBorderMode(0);
   8d196b8732e842269d4321322460ef57->SetBorderSize(2);
   8d196b8732e842269d4321322460ef57->SetFrameBorderMode(0);
   8d196b8732e842269d4321322460ef57->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(3);
   gre->SetName("Graph0");
   gre->SetTitle("HPGe Energy Calibration");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(32);
   gre->SetPoint(0,0.7349694,511);
   gre->SetPointError(0,3.726043e-05,0);
   gre->SetPoint(1,0.95244,661.657);
   gre->SetPointError(1,1.79124e-05,0);
   gre->SetPoint(2,1.836804,1274.537);
   gre->SetPointError(2,3.97978e-05,0);
   
   TH1F *Graph_Graph6 = new TH1F("Graph_Graph6","HPGe Energy Calibration",100,0.624741,1.947035);
   Graph_Graph6->SetMinimum(434.6463);
   Graph_Graph6->SetMaximum(1350.891);
   Graph_Graph6->SetDirectory(0);
   Graph_Graph6->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph6->SetLineColor(ci);
   Graph_Graph6->GetXaxis()->SetTitle("Peak Voltage [V]");
   Graph_Graph6->GetXaxis()->SetLabelFont(42);
   Graph_Graph6->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph6->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph6->GetXaxis()->SetTitleFont(42);
   Graph_Graph6->GetYaxis()->SetTitle("Energy [keV]");
   Graph_Graph6->GetYaxis()->SetLabelFont(42);
   Graph_Graph6->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph6->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph6->GetYaxis()->SetTitleFont(42);
   Graph_Graph6->GetZaxis()->SetLabelFont(42);
   Graph_Graph6->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph6->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph6->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph6);
   
   gre->Draw("ap");
   
   TF1 *f_energy_cal = new TF1("f_energy_cal","pol1",0,2);
   f_energy_cal->SetFillColor(19);
   f_energy_cal->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_energy_cal->SetLineColor(ci);
   f_energy_cal->SetLineWidth(2);
   f_energy_cal->SetLineStyle(2);
   f_energy_cal->SetChisquare(3.070458);
   f_energy_cal->SetNDF(1);
   f_energy_cal->GetXaxis()->SetLabelFont(42);
   f_energy_cal->GetXaxis()->SetLabelSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleFont(42);
   f_energy_cal->GetYaxis()->SetLabelFont(42);
   f_energy_cal->GetYaxis()->SetLabelSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleFont(42);
   f_energy_cal->SetParameter(0,1.632968);
   f_energy_cal->SetParError(0,0.03418307);
   f_energy_cal->SetParLimits(0,0,0);
   f_energy_cal->SetParameter(1,692.9937);
   f_energy_cal->SetParError(1,0.03125001);
   f_energy_cal->SetParLimits(1,0,0);
   f_energy_cal->Draw("same");
   
   TPaveText *pt = new TPaveText(0.2,0.67,0.55,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   TText *text = pt->AddText("Intercept = 1.6330e+00 #pm 3.4183e-02 keV");
   text = pt->AddText("Slope = 6.9299e+02 #pm 3.1250e-02 #frac{keV}{V}");
   pt->Draw();
   
   pt = new TPaveText(0.2333333,0.9326,0.7666667,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("HPGe Energy Calibration");
   pt->Draw();
   8d196b8732e842269d4321322460ef57->Modified();
   8d196b8732e842269d4321322460ef57->cd();
   8d196b8732e842269d4321322460ef57->SetSelected(8d196b8732e842269d4321322460ef57);
}
