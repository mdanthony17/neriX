{
//=========Macro generated from canvas: 09521533027546f89a03835c2cb04c9c/
//=========  (Mon Jul 18 11:03:33 2016) by ROOT version5.34/26
   TCanvas *09521533027546f89a03835c2cb04c9c = new TCanvas("09521533027546f89a03835c2cb04c9c", "",0,0,600,500);
   09521533027546f89a03835c2cb04c9c->SetHighLightColor(2);
   09521533027546f89a03835c2cb04c9c->Range(0.4594982,320.1157,2.112735,1465.421);
   09521533027546f89a03835c2cb04c9c->SetFillColor(0);
   09521533027546f89a03835c2cb04c9c->SetBorderMode(0);
   09521533027546f89a03835c2cb04c9c->SetBorderSize(2);
   09521533027546f89a03835c2cb04c9c->SetFrameBorderMode(0);
   09521533027546f89a03835c2cb04c9c->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(3);
   gre->SetName("Graph0");
   gre->SetTitle("HPGe Energy Calibration");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(32);
   gre->SetPoint(0,0.7350672,511);
   gre->SetPointError(0,2.961595e-05,0);
   gre->SetPoint(1,0.9525737,661.657);
   gre->SetPointError(1,1.683334e-05,0);
   gre->SetPoint(2,1.83714,1274.537);
   gre->SetPointError(2,5.509438e-05,0);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","HPGe Energy Calibration",100,0.6248219,1.947411);
   Graph_Graph1->SetMinimum(434.6463);
   Graph_Graph1->SetMaximum(1350.891);
   Graph_Graph1->SetDirectory(0);
   Graph_Graph1->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph1->SetLineColor(ci);
   Graph_Graph1->GetXaxis()->SetTitle("Peak Voltage [V]");
   Graph_Graph1->GetXaxis()->SetLabelFont(42);
   Graph_Graph1->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleFont(42);
   Graph_Graph1->GetYaxis()->SetTitle("Energy [keV]");
   Graph_Graph1->GetYaxis()->SetLabelFont(42);
   Graph_Graph1->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleFont(42);
   Graph_Graph1->GetZaxis()->SetLabelFont(42);
   Graph_Graph1->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1);
   
   gre->Draw("ap");
   
   TF1 *f_energy_cal = new TF1("f_energy_cal","pol1",0,2);
   f_energy_cal->SetFillColor(19);
   f_energy_cal->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_energy_cal->SetLineColor(ci);
   f_energy_cal->SetLineWidth(2);
   f_energy_cal->SetLineStyle(2);
   f_energy_cal->SetChisquare(2.736288);
   f_energy_cal->SetNDF(1);
   f_energy_cal->GetXaxis()->SetLabelFont(42);
   f_energy_cal->GetXaxis()->SetLabelSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleFont(42);
   f_energy_cal->GetYaxis()->SetLabelFont(42);
   f_energy_cal->GetYaxis()->SetLabelSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleFont(42);
   f_energy_cal->SetParameter(0,1.702897);
   f_energy_cal->SetParError(0,0.03898951);
   f_energy_cal->SetParLimits(0,0,0);
   f_energy_cal->SetParameter(1,692.8225);
   f_energy_cal->SetParError(1,0.03925949);
   f_energy_cal->SetParLimits(1,0,0);
   f_energy_cal->Draw("same");
   
   TPaveText *pt = new TPaveText(0.2,0.67,0.55,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   TText *text = pt->AddText("Intercept = 1.7029e+00 #pm 3.8990e-02 keV");
   text = pt->AddText("Slope = 6.9282e+02 #pm 3.9259e-02 #frac{keV}{V}");
   pt->Draw();
   
   pt = new TPaveText(0.2333333,0.9326,0.7666667,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("HPGe Energy Calibration");
   pt->Draw();
   09521533027546f89a03835c2cb04c9c->Modified();
   09521533027546f89a03835c2cb04c9c->cd();
   09521533027546f89a03835c2cb04c9c->SetSelected(09521533027546f89a03835c2cb04c9c);
}
