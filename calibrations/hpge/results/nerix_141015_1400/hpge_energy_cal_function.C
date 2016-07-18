{
//=========Macro generated from canvas: 46ebc38de8d340ff897ae7cbafd1ca25/
//=========  (Mon Jul 18 11:03:34 2016) by ROOT version5.34/26
   TCanvas *46ebc38de8d340ff897ae7cbafd1ca25 = new TCanvas("46ebc38de8d340ff897ae7cbafd1ca25", "",0,0,600,500);
   46ebc38de8d340ff897ae7cbafd1ca25->SetHighLightColor(2);
   46ebc38de8d340ff897ae7cbafd1ca25->Range(0.4594824,320.1157,2.112384,1465.421);
   46ebc38de8d340ff897ae7cbafd1ca25->SetFillColor(0);
   46ebc38de8d340ff897ae7cbafd1ca25->SetBorderMode(0);
   46ebc38de8d340ff897ae7cbafd1ca25->SetBorderSize(2);
   46ebc38de8d340ff897ae7cbafd1ca25->SetFrameBorderMode(0);
   46ebc38de8d340ff897ae7cbafd1ca25->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(3);
   gre->SetName("Graph0");
   gre->SetTitle("HPGe Energy Calibration");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(32);
   gre->SetPoint(0,0.735001,511);
   gre->SetPointError(0,3.495549e-05,0);
   gre->SetPoint(1,0.9524996,661.657);
   gre->SetPointError(1,1.807554e-05,0);
   gre->SetPoint(2,1.836862,1274.537);
   gre->SetPointError(2,3.859412e-05,0);
   
   TH1F *Graph_Graph4 = new TH1F("Graph_Graph4","HPGe Energy Calibration",100,0.6247726,1.947094);
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
   f_energy_cal->SetChisquare(6.227567);
   f_energy_cal->SetNDF(1);
   f_energy_cal->GetXaxis()->SetLabelFont(42);
   f_energy_cal->GetXaxis()->SetLabelSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleFont(42);
   f_energy_cal->GetYaxis()->SetLabelFont(42);
   f_energy_cal->GetYaxis()->SetLabelSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleFont(42);
   f_energy_cal->SetParameter(0,1.606622);
   f_energy_cal->SetParError(0,0.03317921);
   f_energy_cal->SetParLimits(0,0,0);
   f_energy_cal->SetParameter(1,692.9839);
   f_energy_cal->SetParError(1,0.03022286);
   f_energy_cal->SetParLimits(1,0,0);
   f_energy_cal->Draw("same");
   
   TPaveText *pt = new TPaveText(0.2,0.67,0.55,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   TText *text = pt->AddText("Intercept = 1.6066e+00 #pm 3.3179e-02 keV");
   text = pt->AddText("Slope = 6.9298e+02 #pm 3.0223e-02 #frac{keV}{V}");
   pt->Draw();
   
   pt = new TPaveText(0.2333333,0.9326,0.7666667,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("HPGe Energy Calibration");
   pt->Draw();
   46ebc38de8d340ff897ae7cbafd1ca25->Modified();
   46ebc38de8d340ff897ae7cbafd1ca25->cd();
   46ebc38de8d340ff897ae7cbafd1ca25->SetSelected(46ebc38de8d340ff897ae7cbafd1ca25);
}
