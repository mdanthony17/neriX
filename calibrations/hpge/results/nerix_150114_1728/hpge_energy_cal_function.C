{
//=========Macro generated from canvas: 4cb573e994464831ad9a6bfb774f60d2/
//=========  (Mon Jul 18 11:23:41 2016) by ROOT version5.34/26
   TCanvas *4cb573e994464831ad9a6bfb774f60d2 = new TCanvas("4cb573e994464831ad9a6bfb774f60d2", "",0,0,600,500);
   4cb573e994464831ad9a6bfb774f60d2->SetHighLightColor(2);
   4cb573e994464831ad9a6bfb774f60d2->Range(0.4590377,320.1157,2.110909,1465.421);
   4cb573e994464831ad9a6bfb774f60d2->SetFillColor(0);
   4cb573e994464831ad9a6bfb774f60d2->SetBorderMode(0);
   4cb573e994464831ad9a6bfb774f60d2->SetBorderSize(2);
   4cb573e994464831ad9a6bfb774f60d2->SetFrameBorderMode(0);
   4cb573e994464831ad9a6bfb774f60d2->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(3);
   gre->SetName("Graph0");
   gre->SetTitle("HPGe Energy Calibration");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(32);
   gre->SetPoint(0,0.7343811,511);
   gre->SetPointError(0,3.148995e-05,0);
   gre->SetPoint(1,0.9516878,661.657);
   gre->SetPointError(1,1.984539e-05,0);
   gre->SetPoint(2,1.835553,1274.537);
   gre->SetPointError(2,4.409124e-05,0);
   
   TH1F *Graph_Graph10 = new TH1F("Graph_Graph10","HPGe Energy Calibration",100,0.6242248,1.945722);
   Graph_Graph10->SetMinimum(434.6463);
   Graph_Graph10->SetMaximum(1350.891);
   Graph_Graph10->SetDirectory(0);
   Graph_Graph10->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph10->SetLineColor(ci);
   Graph_Graph10->GetXaxis()->SetTitle("Peak Voltage [V]");
   Graph_Graph10->GetXaxis()->SetLabelFont(42);
   Graph_Graph10->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph10->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph10->GetXaxis()->SetTitleFont(42);
   Graph_Graph10->GetYaxis()->SetTitle("Energy [keV]");
   Graph_Graph10->GetYaxis()->SetLabelFont(42);
   Graph_Graph10->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph10->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph10->GetYaxis()->SetTitleFont(42);
   Graph_Graph10->GetZaxis()->SetLabelFont(42);
   Graph_Graph10->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph10->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph10->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph10);
   
   gre->Draw("ap");
   
   TF1 *f_energy_cal = new TF1("f_energy_cal","pol1",0,2);
   f_energy_cal->SetFillColor(19);
   f_energy_cal->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_energy_cal->SetLineColor(ci);
   f_energy_cal->SetLineWidth(2);
   f_energy_cal->SetLineStyle(2);
   f_energy_cal->SetChisquare(0.7766055);
   f_energy_cal->SetNDF(1);
   f_energy_cal->GetXaxis()->SetLabelFont(42);
   f_energy_cal->GetXaxis()->SetLabelSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleFont(42);
   f_energy_cal->GetYaxis()->SetLabelFont(42);
   f_energy_cal->GetYaxis()->SetLabelSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleFont(42);
   f_energy_cal->SetParameter(0,1.771039);
   f_energy_cal->SetParError(0,0.03525316);
   f_energy_cal->SetParLimits(0,0,0);
   f_energy_cal->SetParameter(1,693.3925);
   f_energy_cal->SetParError(1,0.03321184);
   f_energy_cal->SetParLimits(1,0,0);
   f_energy_cal->Draw("same");
   
   TPaveText *pt = new TPaveText(0.2,0.67,0.55,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   TText *text = pt->AddText("Intercept = 1.7710e+00 #pm 3.5253e-02 keV");
   text = pt->AddText("Slope = 6.9339e+02 #pm 3.3212e-02 #frac{keV}{V}");
   pt->Draw();
   
   pt = new TPaveText(0.2333333,0.9326,0.7666667,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("HPGe Energy Calibration");
   pt->Draw();
   4cb573e994464831ad9a6bfb774f60d2->Modified();
   4cb573e994464831ad9a6bfb774f60d2->cd();
   4cb573e994464831ad9a6bfb774f60d2->SetSelected(4cb573e994464831ad9a6bfb774f60d2);
}
