{
//=========Macro generated from canvas: 8b4fa1458faf4020928b3bc1deed9626/
//=========  (Mon Jul 18 11:23:39 2016) by ROOT version5.34/26
   TCanvas *8b4fa1458faf4020928b3bc1deed9626 = new TCanvas("8b4fa1458faf4020928b3bc1deed9626", "",0,0,600,500);
   8b4fa1458faf4020928b3bc1deed9626->SetHighLightColor(2);
   8b4fa1458faf4020928b3bc1deed9626->Range(0.4589383,320.1157,2.111064,1465.421);
   8b4fa1458faf4020928b3bc1deed9626->SetFillColor(0);
   8b4fa1458faf4020928b3bc1deed9626->SetBorderMode(0);
   8b4fa1458faf4020928b3bc1deed9626->SetBorderSize(2);
   8b4fa1458faf4020928b3bc1deed9626->SetFrameBorderMode(0);
   8b4fa1458faf4020928b3bc1deed9626->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(3);
   gre->SetName("Graph0");
   gre->SetTitle("HPGe Energy Calibration");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(32);
   gre->SetPoint(0,0.7343428,511);
   gre->SetPointError(0,5.019631e-05,0);
   gre->SetPoint(1,0.9516947,661.657);
   gre->SetPointError(1,2.681273e-05,0);
   gre->SetPoint(2,1.835628,1274.537);
   gre->SetPointError(2,8.18961e-05,0);
   
   TH1F *Graph_Graph6 = new TH1F("Graph_Graph6","HPGe Energy Calibration",100,0.6241509,1.945851);
   Graph_Graph6->SetMinimum(434.6463);
   Graph_Graph6->SetMaximum(1350.891);
   Graph_Graph6->SetDirectory(0);
   Graph_Graph6->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph6->SetLineColor(ci);
   Graph_Graph6->GetXaxis()->SetTitle("Peak Voltage [V]");
   Graph_Graph6->GetXaxis()->SetLabelFont(42);
   Graph_Graph6->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph6->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph6->GetXaxis()->SetTitleFont(42);
   Graph_Graph6->GetYaxis()->SetTitle("Energy [keV]");
   Graph_Graph6->GetYaxis()->SetLabelFont(42);
   Graph_Graph6->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph6->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph6->GetYaxis()->SetTitleFont(42);
   Graph_Graph6->GetZaxis()->SetLabelFont(42);
   Graph_Graph6->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph6->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph6->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph6);
   
   gre->Draw("ap");
   
   TF1 *f_energy_cal = new TF1("f_energy_cal","pol1",0,2);
   f_energy_cal->SetFillColor(19);
   f_energy_cal->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_energy_cal->SetLineColor(ci);
   f_energy_cal->SetLineWidth(2);
   f_energy_cal->SetLineStyle(2);
   f_energy_cal->SetChisquare(1.052244);
   f_energy_cal->SetNDF(1);
   f_energy_cal->GetXaxis()->SetLabelFont(42);
   f_energy_cal->GetXaxis()->SetLabelSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleFont(42);
   f_energy_cal->GetYaxis()->SetLabelFont(42);
   f_energy_cal->GetYaxis()->SetLabelSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleFont(42);
   f_energy_cal->SetParameter(0,1.834683);
   f_energy_cal->SetParError(0,0.06045717);
   f_energy_cal->SetParLimits(0,0,0);
   f_energy_cal->SetParameter(1,693.3235);
   f_energy_cal->SetParError(1,0.05985312);
   f_energy_cal->SetParLimits(1,0,0);
   f_energy_cal->Draw("same");
   
   TPaveText *pt = new TPaveText(0.2,0.67,0.55,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   TText *text = pt->AddText("Intercept = 1.8347e+00 #pm 6.0457e-02 keV");
   text = pt->AddText("Slope = 6.9332e+02 #pm 5.9853e-02 #frac{keV}{V}");
   pt->Draw();
   
   pt = new TPaveText(0.2333333,0.9326,0.7666667,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("HPGe Energy Calibration");
   pt->Draw();
   8b4fa1458faf4020928b3bc1deed9626->Modified();
   8b4fa1458faf4020928b3bc1deed9626->cd();
   8b4fa1458faf4020928b3bc1deed9626->SetSelected(8b4fa1458faf4020928b3bc1deed9626);
}
