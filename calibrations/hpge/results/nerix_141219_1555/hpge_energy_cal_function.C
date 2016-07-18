{
//=========Macro generated from canvas: 248e28b49fde4d0baa75d6cf46f8aee4/
//=========  (Mon Jul 18 11:23:40 2016) by ROOT version5.34/26
   TCanvas *248e28b49fde4d0baa75d6cf46f8aee4 = new TCanvas("248e28b49fde4d0baa75d6cf46f8aee4", "",0,0,600,500);
   248e28b49fde4d0baa75d6cf46f8aee4->SetHighLightColor(2);
   248e28b49fde4d0baa75d6cf46f8aee4->Range(0.4589745,320.1157,2.110727,1465.421);
   248e28b49fde4d0baa75d6cf46f8aee4->SetFillColor(0);
   248e28b49fde4d0baa75d6cf46f8aee4->SetBorderMode(0);
   248e28b49fde4d0baa75d6cf46f8aee4->SetBorderSize(2);
   248e28b49fde4d0baa75d6cf46f8aee4->SetFrameBorderMode(0);
   248e28b49fde4d0baa75d6cf46f8aee4->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(3);
   gre->SetName("Graph0");
   gre->SetTitle("HPGe Energy Calibration");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(32);
   gre->SetPoint(0,0.7343014,511);
   gre->SetPointError(0,3.487628e-05,0);
   gre->SetPoint(1,0.9516165,661.657);
   gre->SetPointError(1,2.005941e-05,0);
   gre->SetPoint(2,1.835395,1274.537);
   gre->SetPointError(2,3.977433e-05,0);
   
   TH1F *Graph_Graph7 = new TH1F("Graph_Graph7","HPGe Energy Calibration",100,0.6241497,1.945551);
   Graph_Graph7->SetMinimum(434.6463);
   Graph_Graph7->SetMaximum(1350.891);
   Graph_Graph7->SetDirectory(0);
   Graph_Graph7->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph7->SetLineColor(ci);
   Graph_Graph7->GetXaxis()->SetTitle("Peak Voltage [V]");
   Graph_Graph7->GetXaxis()->SetLabelFont(42);
   Graph_Graph7->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph7->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph7->GetXaxis()->SetTitleFont(42);
   Graph_Graph7->GetYaxis()->SetTitle("Energy [keV]");
   Graph_Graph7->GetYaxis()->SetLabelFont(42);
   Graph_Graph7->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph7->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph7->GetYaxis()->SetTitleFont(42);
   Graph_Graph7->GetZaxis()->SetLabelFont(42);
   Graph_Graph7->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph7->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph7->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph7);
   
   gre->Draw("ap");
   
   TF1 *f_energy_cal = new TF1("f_energy_cal","pol1",0,2);
   f_energy_cal->SetFillColor(19);
   f_energy_cal->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_energy_cal->SetLineColor(ci);
   f_energy_cal->SetLineWidth(2);
   f_energy_cal->SetLineStyle(2);
   f_energy_cal->SetChisquare(2.276309);
   f_energy_cal->SetNDF(1);
   f_energy_cal->GetXaxis()->SetLabelFont(42);
   f_energy_cal->GetXaxis()->SetLabelSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleFont(42);
   f_energy_cal->GetYaxis()->SetLabelFont(42);
   f_energy_cal->GetYaxis()->SetLabelSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleFont(42);
   f_energy_cal->SetParameter(0,1.767509);
   f_energy_cal->SetParError(0,0.03445377);
   f_energy_cal->SetParLimits(0,0,0);
   f_energy_cal->SetParameter(1,693.4531);
   f_energy_cal->SetParError(1,0.03113796);
   f_energy_cal->SetParLimits(1,0,0);
   f_energy_cal->Draw("same");
   
   TPaveText *pt = new TPaveText(0.2,0.67,0.55,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   TText *text = pt->AddText("Intercept = 1.7675e+00 #pm 3.4454e-02 keV");
   text = pt->AddText("Slope = 6.9345e+02 #pm 3.1138e-02 #frac{keV}{V}");
   pt->Draw();
   
   pt = new TPaveText(0.2333333,0.9326,0.7666667,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("HPGe Energy Calibration");
   pt->Draw();
   248e28b49fde4d0baa75d6cf46f8aee4->Modified();
   248e28b49fde4d0baa75d6cf46f8aee4->cd();
   248e28b49fde4d0baa75d6cf46f8aee4->SetSelected(248e28b49fde4d0baa75d6cf46f8aee4);
}
