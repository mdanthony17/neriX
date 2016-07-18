{
//=========Macro generated from canvas: 89097f94f1664c3490c6ce99517a9e01/
//=========  (Mon Jul 18 11:03:34 2016) by ROOT version5.34/26
   TCanvas *89097f94f1664c3490c6ce99517a9e01 = new TCanvas("89097f94f1664c3490c6ce99517a9e01", "",0,0,600,500);
   89097f94f1664c3490c6ce99517a9e01->SetHighLightColor(2);
   89097f94f1664c3490c6ce99517a9e01->Range(0.4595409,320.1157,2.112682,1465.421);
   89097f94f1664c3490c6ce99517a9e01->SetFillColor(0);
   89097f94f1664c3490c6ce99517a9e01->SetBorderMode(0);
   89097f94f1664c3490c6ce99517a9e01->SetBorderSize(2);
   89097f94f1664c3490c6ce99517a9e01->SetFrameBorderMode(0);
   89097f94f1664c3490c6ce99517a9e01->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(3);
   gre->SetName("Graph0");
   gre->SetTitle("HPGe Energy Calibration");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(32);
   gre->SetPoint(0,0.7350985,511);
   gre->SetPointError(0,3.408262e-05,0);
   gre->SetPoint(1,0.9525322,661.657);
   gre->SetPointError(1,1.683265e-05,0);
   gre->SetPoint(2,1.83711,1274.537);
   gre->SetPointError(2,4.861234e-05,0);
   
   TH1F *Graph_Graph3 = new TH1F("Graph_Graph3","HPGe Energy Calibration",100,0.624855,1.947368);
   Graph_Graph3->SetMinimum(434.6463);
   Graph_Graph3->SetMaximum(1350.891);
   Graph_Graph3->SetDirectory(0);
   Graph_Graph3->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph3->SetLineColor(ci);
   Graph_Graph3->GetXaxis()->SetTitle("Peak Voltage [V]");
   Graph_Graph3->GetXaxis()->SetLabelFont(42);
   Graph_Graph3->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph3->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph3->GetXaxis()->SetTitleFont(42);
   Graph_Graph3->GetYaxis()->SetTitle("Energy [keV]");
   Graph_Graph3->GetYaxis()->SetLabelFont(42);
   Graph_Graph3->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph3->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph3->GetYaxis()->SetTitleFont(42);
   Graph_Graph3->GetZaxis()->SetLabelFont(42);
   Graph_Graph3->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph3->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph3->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph3);
   
   gre->Draw("ap");
   
   TF1 *f_energy_cal = new TF1("f_energy_cal","pol1",0,2);
   f_energy_cal->SetFillColor(19);
   f_energy_cal->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_energy_cal->SetLineColor(ci);
   f_energy_cal->SetLineWidth(2);
   f_energy_cal->SetLineStyle(2);
   f_energy_cal->SetChisquare(0.07597454);
   f_energy_cal->SetNDF(1);
   f_energy_cal->GetXaxis()->SetLabelFont(42);
   f_energy_cal->GetXaxis()->SetLabelSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleFont(42);
   f_energy_cal->GetYaxis()->SetLabelFont(42);
   f_energy_cal->GetYaxis()->SetLabelSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleFont(42);
   f_energy_cal->SetParameter(0,1.688474);
   f_energy_cal->SetParError(0,0.03733768);
   f_energy_cal->SetParLimits(0,0,0);
   f_energy_cal->SetParameter(1,692.8552);
   f_energy_cal->SetParError(1,0.03628895);
   f_energy_cal->SetParLimits(1,0,0);
   f_energy_cal->Draw("same");
   
   TPaveText *pt = new TPaveText(0.2,0.67,0.55,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   TText *text = pt->AddText("Intercept = 1.6885e+00 #pm 3.7338e-02 keV");
   text = pt->AddText("Slope = 6.9286e+02 #pm 3.6289e-02 #frac{keV}{V}");
   pt->Draw();
   
   pt = new TPaveText(0.2333333,0.9326,0.7666667,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("HPGe Energy Calibration");
   pt->Draw();
   89097f94f1664c3490c6ce99517a9e01->Modified();
   89097f94f1664c3490c6ce99517a9e01->cd();
   89097f94f1664c3490c6ce99517a9e01->SetSelected(89097f94f1664c3490c6ce99517a9e01);
}
