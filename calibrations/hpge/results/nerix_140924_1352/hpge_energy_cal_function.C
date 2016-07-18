{
//=========Macro generated from canvas: fcf17dd594c740ac9068c91938d3af70/
//=========  (Mon Jul 18 11:03:33 2016) by ROOT version5.34/26
   TCanvas *fcf17dd594c740ac9068c91938d3af70 = new TCanvas("fcf17dd594c740ac9068c91938d3af70", "",0,0,600,500);
   fcf17dd594c740ac9068c91938d3af70->SetHighLightColor(2);
   fcf17dd594c740ac9068c91938d3af70->Range(0.459535,320.1157,2.112757,1465.421);
   fcf17dd594c740ac9068c91938d3af70->SetFillColor(0);
   fcf17dd594c740ac9068c91938d3af70->SetBorderMode(0);
   fcf17dd594c740ac9068c91938d3af70->SetBorderSize(2);
   fcf17dd594c740ac9068c91938d3af70->SetFrameBorderMode(0);
   fcf17dd594c740ac9068c91938d3af70->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(3);
   gre->SetName("Graph0");
   gre->SetTitle("HPGe Energy Calibration");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(32);
   gre->SetPoint(0,0.7351028,511);
   gre->SetPointError(0,3.088407e-05,0);
   gre->SetPoint(1,0.9526513,661.657);
   gre->SetPointError(1,1.814555e-05,0);
   gre->SetPoint(2,1.837178,1274.537);
   gre->SetPointError(2,4.136456e-05,0);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","HPGe Energy Calibration",100,0.6248572,1.947435);
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
   f_energy_cal->SetChisquare(8.526878);
   f_energy_cal->SetNDF(1);
   f_energy_cal->GetXaxis()->SetLabelFont(42);
   f_energy_cal->GetXaxis()->SetLabelSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleFont(42);
   f_energy_cal->GetYaxis()->SetLabelFont(42);
   f_energy_cal->GetYaxis()->SetLabelSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleFont(42);
   f_energy_cal->SetParameter(0,1.641143);
   f_energy_cal->SetParError(0,0.0333646);
   f_energy_cal->SetParLimits(0,0,0);
   f_energy_cal->SetParameter(1,692.842);
   f_energy_cal->SetParError(1,0.03133206);
   f_energy_cal->SetParLimits(1,0,0);
   f_energy_cal->Draw("same");
   
   TPaveText *pt = new TPaveText(0.2,0.67,0.55,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   TText *text = pt->AddText("Intercept = 1.6411e+00 #pm 3.3365e-02 keV");
   text = pt->AddText("Slope = 6.9284e+02 #pm 3.1332e-02 #frac{keV}{V}");
   pt->Draw();
   
   pt = new TPaveText(0.2333333,0.9326,0.7666667,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("HPGe Energy Calibration");
   pt->Draw();
   fcf17dd594c740ac9068c91938d3af70->Modified();
   fcf17dd594c740ac9068c91938d3af70->cd();
   fcf17dd594c740ac9068c91938d3af70->SetSelected(fcf17dd594c740ac9068c91938d3af70);
}
