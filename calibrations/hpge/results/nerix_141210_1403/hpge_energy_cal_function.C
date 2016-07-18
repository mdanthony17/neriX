{
//=========Macro generated from canvas: 361afdbd565b4546953ebbd46a568f67/
//=========  (Mon Jul 18 11:23:38 2016) by ROOT version5.34/26
   TCanvas *361afdbd565b4546953ebbd46a568f67 = new TCanvas("361afdbd565b4546953ebbd46a568f67", "",0,0,600,500);
   361afdbd565b4546953ebbd46a568f67->SetHighLightColor(2);
   361afdbd565b4546953ebbd46a568f67->Range(0.4592774,320.1157,2.11188,1465.421);
   361afdbd565b4546953ebbd46a568f67->SetFillColor(0);
   361afdbd565b4546953ebbd46a568f67->SetBorderMode(0);
   361afdbd565b4546953ebbd46a568f67->SetBorderSize(2);
   361afdbd565b4546953ebbd46a568f67->SetFrameBorderMode(0);
   361afdbd565b4546953ebbd46a568f67->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(3);
   gre->SetName("Graph0");
   gre->SetTitle("HPGe Energy Calibration");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(32);
   gre->SetPoint(0,0.73475,511);
   gre->SetPointError(0,3.876406e-05,0);
   gre->SetPoint(1,0.9521762,661.657);
   gre->SetPointError(1,1.753316e-05,0);
   gre->SetPoint(2,1.836395,1274.537);
   gre->SetPointError(2,5.136056e-05,0);
   
   TH1F *Graph_Graph4 = new TH1F("Graph_Graph4","HPGe Energy Calibration",100,0.6245377,1.94662);
   Graph_Graph4->SetMinimum(434.6463);
   Graph_Graph4->SetMaximum(1350.891);
   Graph_Graph4->SetDirectory(0);
   Graph_Graph4->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph4->SetLineColor(ci);
   Graph_Graph4->GetXaxis()->SetTitle("Peak Voltage [V]");
   Graph_Graph4->GetXaxis()->SetLabelFont(42);
   Graph_Graph4->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph4->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph4->GetXaxis()->SetTitleFont(42);
   Graph_Graph4->GetYaxis()->SetTitle("Energy [keV]");
   Graph_Graph4->GetYaxis()->SetLabelFont(42);
   Graph_Graph4->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph4->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph4->GetYaxis()->SetTitleFont(42);
   Graph_Graph4->GetZaxis()->SetLabelFont(42);
   Graph_Graph4->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph4->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph4->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph4);
   
   gre->Draw("ap");
   
   TF1 *f_energy_cal = new TF1("f_energy_cal","pol1",0,2);
   f_energy_cal->SetFillColor(19);
   f_energy_cal->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_energy_cal->SetLineColor(ci);
   f_energy_cal->SetLineWidth(2);
   f_energy_cal->SetLineStyle(2);
   f_energy_cal->SetChisquare(2.233914);
   f_energy_cal->SetNDF(1);
   f_energy_cal->GetXaxis()->SetLabelFont(42);
   f_energy_cal->GetXaxis()->SetLabelSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleFont(42);
   f_energy_cal->GetYaxis()->SetLabelFont(42);
   f_energy_cal->GetYaxis()->SetLabelSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleFont(42);
   f_energy_cal->SetParameter(0,1.707158);
   f_energy_cal->SetParError(0,0.04006595);
   f_energy_cal->SetParLimits(0,0,0);
   f_energy_cal->SetParameter(1,693.1054);
   f_energy_cal->SetParError(1,0.03878139);
   f_energy_cal->SetParLimits(1,0,0);
   f_energy_cal->Draw("same");
   
   TPaveText *pt = new TPaveText(0.2,0.67,0.55,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   TText *text = pt->AddText("Intercept = 1.7072e+00 #pm 4.0066e-02 keV");
   text = pt->AddText("Slope = 6.9311e+02 #pm 3.8781e-02 #frac{keV}{V}");
   pt->Draw();
   
   pt = new TPaveText(0.2333333,0.9326,0.7666667,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("HPGe Energy Calibration");
   pt->Draw();
   361afdbd565b4546953ebbd46a568f67->Modified();
   361afdbd565b4546953ebbd46a568f67->cd();
   361afdbd565b4546953ebbd46a568f67->SetSelected(361afdbd565b4546953ebbd46a568f67);
}
