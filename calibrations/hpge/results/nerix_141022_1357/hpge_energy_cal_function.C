{
//=========Macro generated from canvas: 2a44c62a6d434d53b3ed3d3ea367d1fd/
//=========  (Mon Jul 18 11:03:34 2016) by ROOT version5.34/26
   TCanvas *2a44c62a6d434d53b3ed3d3ea367d1fd = new TCanvas("2a44c62a6d434d53b3ed3d3ea367d1fd", "",0,0,600,500);
   2a44c62a6d434d53b3ed3d3ea367d1fd->SetHighLightColor(2);
   2a44c62a6d434d53b3ed3d3ea367d1fd->Range(0.4594997,320.1157,2.112465,1465.421);
   2a44c62a6d434d53b3ed3d3ea367d1fd->SetFillColor(0);
   2a44c62a6d434d53b3ed3d3ea367d1fd->SetBorderMode(0);
   2a44c62a6d434d53b3ed3d3ea367d1fd->SetBorderSize(2);
   2a44c62a6d434d53b3ed3d3ea367d1fd->SetFrameBorderMode(0);
   2a44c62a6d434d53b3ed3d3ea367d1fd->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(3);
   gre->SetName("Graph0");
   gre->SetTitle("HPGe Energy Calibration");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(32);
   gre->SetPoint(0,0.7350275,511);
   gre->SetPointError(0,3.367791e-05,0);
   gre->SetPoint(1,0.9525238,661.657);
   gre->SetPointError(1,1.567929e-05,0);
   gre->SetPoint(2,1.836928,1274.537);
   gre->SetPointError(2,4.268697e-05,0);
   
   TH1F *Graph_Graph5 = new TH1F("Graph_Graph5","HPGe Energy Calibration",100,0.6247962,1.947168);
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
   f_energy_cal->SetChisquare(5.40924);
   f_energy_cal->SetNDF(1);
   f_energy_cal->GetXaxis()->SetLabelFont(42);
   f_energy_cal->GetXaxis()->SetLabelSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleFont(42);
   f_energy_cal->GetYaxis()->SetLabelFont(42);
   f_energy_cal->GetYaxis()->SetLabelSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleFont(42);
   f_energy_cal->SetParameter(0,1.615604);
   f_energy_cal->SetParError(0,0.03397976);
   f_energy_cal->SetParLimits(0,0,0);
   f_energy_cal->SetParameter(1,692.9523);
   f_energy_cal->SetParError(1,0.03247692);
   f_energy_cal->SetParLimits(1,0,0);
   f_energy_cal->Draw("same");
   
   TPaveText *pt = new TPaveText(0.2,0.67,0.55,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   TText *text = pt->AddText("Intercept = 1.6156e+00 #pm 3.3980e-02 keV");
   text = pt->AddText("Slope = 6.9295e+02 #pm 3.2477e-02 #frac{keV}{V}");
   pt->Draw();
   
   pt = new TPaveText(0.2333333,0.9326,0.7666667,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("HPGe Energy Calibration");
   pt->Draw();
   2a44c62a6d434d53b3ed3d3ea367d1fd->Modified();
   2a44c62a6d434d53b3ed3d3ea367d1fd->cd();
   2a44c62a6d434d53b3ed3d3ea367d1fd->SetSelected(2a44c62a6d434d53b3ed3d3ea367d1fd);
}
