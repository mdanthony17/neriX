{
//=========Macro generated from canvas: 02d7c9d3369a416595313102101529fb/
//=========  (Mon Jul 18 11:23:41 2016) by ROOT version5.34/26
   TCanvas *02d7c9d3369a416595313102101529fb = new TCanvas("02d7c9d3369a416595313102101529fb", "",0,0,600,500);
   02d7c9d3369a416595313102101529fb->SetHighLightColor(2);
   02d7c9d3369a416595313102101529fb->Range(0.4590552,320.1157,2.110704,1465.421);
   02d7c9d3369a416595313102101529fb->SetFillColor(0);
   02d7c9d3369a416595313102101529fb->SetBorderMode(0);
   02d7c9d3369a416595313102101529fb->SetBorderSize(2);
   02d7c9d3369a416595313102101529fb->SetFrameBorderMode(0);
   02d7c9d3369a416595313102101529fb->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(3);
   gre->SetName("Graph0");
   gre->SetTitle("HPGe Energy Calibration");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(32);
   gre->SetPoint(0,0.7343694,511);
   gre->SetPointError(0,3.93194e-05,0);
   gre->SetPoint(1,0.951665,661.657);
   gre->SetPointError(1,1.77266e-05,0);
   gre->SetPoint(2,1.835384,1274.537);
   gre->SetPointError(2,4.549543e-05,0);
   
   TH1F *Graph_Graph12 = new TH1F("Graph_Graph12","HPGe Energy Calibration",100,0.6242201,1.945539);
   Graph_Graph12->SetMinimum(434.6463);
   Graph_Graph12->SetMaximum(1350.891);
   Graph_Graph12->SetDirectory(0);
   Graph_Graph12->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph12->SetLineColor(ci);
   Graph_Graph12->GetXaxis()->SetTitle("Peak Voltage [V]");
   Graph_Graph12->GetXaxis()->SetLabelFont(42);
   Graph_Graph12->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph12->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph12->GetXaxis()->SetTitleFont(42);
   Graph_Graph12->GetYaxis()->SetTitle("Energy [keV]");
   Graph_Graph12->GetYaxis()->SetLabelFont(42);
   Graph_Graph12->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph12->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph12->GetYaxis()->SetTitleFont(42);
   Graph_Graph12->GetZaxis()->SetLabelFont(42);
   Graph_Graph12->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph12->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph12->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph12);
   
   gre->Draw("ap");
   
   TF1 *f_energy_cal = new TF1("f_energy_cal","pol1",0,2);
   f_energy_cal->SetFillColor(19);
   f_energy_cal->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_energy_cal->SetLineColor(ci);
   f_energy_cal->SetLineWidth(2);
   f_energy_cal->SetLineStyle(2);
   f_energy_cal->SetChisquare(1.754663);
   f_energy_cal->SetNDF(1);
   f_energy_cal->GetXaxis()->SetLabelFont(42);
   f_energy_cal->GetXaxis()->SetLabelSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleFont(42);
   f_energy_cal->GetYaxis()->SetLabelFont(42);
   f_energy_cal->GetYaxis()->SetLabelSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleFont(42);
   f_energy_cal->SetParameter(0,1.681612);
   f_energy_cal->SetParError(0,0.03732663);
   f_energy_cal->SetParLimits(0,0,0);
   f_energy_cal->SetParameter(1,693.5036);
   f_energy_cal->SetParError(1,0.03516636);
   f_energy_cal->SetParLimits(1,0,0);
   f_energy_cal->Draw("same");
   
   TPaveText *pt = new TPaveText(0.2,0.67,0.55,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   TText *text = pt->AddText("Intercept = 1.6816e+00 #pm 3.7327e-02 keV");
   text = pt->AddText("Slope = 6.9350e+02 #pm 3.5166e-02 #frac{keV}{V}");
   pt->Draw();
   
   pt = new TPaveText(0.2333333,0.9326,0.7666667,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("HPGe Energy Calibration");
   pt->Draw();
   02d7c9d3369a416595313102101529fb->Modified();
   02d7c9d3369a416595313102101529fb->cd();
   02d7c9d3369a416595313102101529fb->SetSelected(02d7c9d3369a416595313102101529fb);
}
