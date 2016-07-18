{
//=========Macro generated from canvas: 70d91c9dde55446aad0eb5875a173c77/
//=========  (Mon Jul 18 11:23:39 2016) by ROOT version5.34/26
   TCanvas *70d91c9dde55446aad0eb5875a173c77 = new TCanvas("70d91c9dde55446aad0eb5875a173c77", "",0,0,600,500);
   70d91c9dde55446aad0eb5875a173c77->SetHighLightColor(2);
   70d91c9dde55446aad0eb5875a173c77->Range(0.4591653,320.1157,2.111818,1465.421);
   70d91c9dde55446aad0eb5875a173c77->SetFillColor(0);
   70d91c9dde55446aad0eb5875a173c77->SetBorderMode(0);
   70d91c9dde55446aad0eb5875a173c77->SetBorderSize(2);
   70d91c9dde55446aad0eb5875a173c77->SetFrameBorderMode(0);
   70d91c9dde55446aad0eb5875a173c77->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(3);
   gre->SetName("Graph0");
   gre->SetTitle("HPGe Energy Calibration");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(32);
   gre->SetPoint(0,0.73464,511);
   gre->SetPointError(0,3.265953e-05,0);
   gre->SetPoint(1,0.9520409,661.657);
   gre->SetPointError(1,1.82377e-05,0);
   gre->SetPoint(2,1.836334,1274.537);
   gre->SetPointError(2,4.118577e-05,0);
   
   TH1F *Graph_Graph5 = new TH1F("Graph_Graph5","HPGe Energy Calibration",100,0.6244306,1.946552);
   Graph_Graph5->SetMinimum(434.6463);
   Graph_Graph5->SetMaximum(1350.891);
   Graph_Graph5->SetDirectory(0);
   Graph_Graph5->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph5->SetLineColor(ci);
   Graph_Graph5->GetXaxis()->SetTitle("Peak Voltage [V]");
   Graph_Graph5->GetXaxis()->SetLabelFont(42);
   Graph_Graph5->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph5->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph5->GetXaxis()->SetTitleFont(42);
   Graph_Graph5->GetYaxis()->SetTitle("Energy [keV]");
   Graph_Graph5->GetYaxis()->SetLabelFont(42);
   Graph_Graph5->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph5->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph5->GetYaxis()->SetTitleFont(42);
   Graph_Graph5->GetZaxis()->SetLabelFont(42);
   Graph_Graph5->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph5->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph5->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph5);
   
   gre->Draw("ap");
   
   TF1 *f_energy_cal = new TF1("f_energy_cal","pol1",0,2);
   f_energy_cal->SetFillColor(19);
   f_energy_cal->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_energy_cal->SetLineColor(ci);
   f_energy_cal->SetLineWidth(2);
   f_energy_cal->SetLineStyle(2);
   f_energy_cal->SetChisquare(0.3868901);
   f_energy_cal->SetNDF(1);
   f_energy_cal->GetXaxis()->SetLabelFont(42);
   f_energy_cal->GetXaxis()->SetLabelSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleFont(42);
   f_energy_cal->GetYaxis()->SetLabelFont(42);
   f_energy_cal->GetYaxis()->SetLabelSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleFont(42);
   f_energy_cal->SetParameter(0,1.836805);
   f_energy_cal->SetParError(0,0.03383273);
   f_energy_cal->SetParLimits(0,0,0);
   f_energy_cal->SetParameter(1,693.0632);
   f_energy_cal->SetParError(1,0.03153286);
   f_energy_cal->SetParLimits(1,0,0);
   f_energy_cal->Draw("same");
   
   TPaveText *pt = new TPaveText(0.2,0.67,0.55,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   TText *text = pt->AddText("Intercept = 1.8368e+00 #pm 3.3833e-02 keV");
   text = pt->AddText("Slope = 6.9306e+02 #pm 3.1533e-02 #frac{keV}{V}");
   pt->Draw();
   
   pt = new TPaveText(0.2333333,0.9326,0.7666667,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("HPGe Energy Calibration");
   pt->Draw();
   70d91c9dde55446aad0eb5875a173c77->Modified();
   70d91c9dde55446aad0eb5875a173c77->cd();
   70d91c9dde55446aad0eb5875a173c77->SetSelected(70d91c9dde55446aad0eb5875a173c77);
}
