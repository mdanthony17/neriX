{
//=========Macro generated from canvas: 6f88259ceb3e4868b083bc561e728b7d/
//=========  (Mon Jul 18 11:23:38 2016) by ROOT version5.34/26
   TCanvas *6f88259ceb3e4868b083bc561e728b7d = new TCanvas("6f88259ceb3e4868b083bc561e728b7d", "",0,0,600,500);
   6f88259ceb3e4868b083bc561e728b7d->SetHighLightColor(2);
   6f88259ceb3e4868b083bc561e728b7d->Range(0.4591796,320.1157,2.111929,1465.421);
   6f88259ceb3e4868b083bc561e728b7d->SetFillColor(0);
   6f88259ceb3e4868b083bc561e728b7d->SetBorderMode(0);
   6f88259ceb3e4868b083bc561e728b7d->SetBorderSize(2);
   6f88259ceb3e4868b083bc561e728b7d->SetFrameBorderMode(0);
   6f88259ceb3e4868b083bc561e728b7d->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(3);
   gre->SetName("Graph0");
   gre->SetTitle("HPGe Energy Calibration");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(32);
   gre->SetPoint(0,0.7346716,511);
   gre->SetPointError(0,3.371393e-05,0);
   gre->SetPoint(1,0.9522241,661.657);
   gre->SetPointError(1,2.206033e-05,0);
   gre->SetPoint(2,1.836418,1274.537);
   gre->SetPointError(2,5.25993e-05,0);
   
   TH1F *Graph_Graph3 = new TH1F("Graph_Graph3","HPGe Energy Calibration",100,0.6244546,1.946654);
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
   f_energy_cal->SetChisquare(19.7594);
   f_energy_cal->SetNDF(1);
   f_energy_cal->GetXaxis()->SetLabelFont(42);
   f_energy_cal->GetXaxis()->SetLabelSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleFont(42);
   f_energy_cal->GetYaxis()->SetLabelFont(42);
   f_energy_cal->GetYaxis()->SetLabelSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleFont(42);
   f_energy_cal->SetParameter(0,1.756976);
   f_energy_cal->SetParError(0,0.04029328);
   f_energy_cal->SetParLimits(0,0,0);
   f_energy_cal->SetParameter(1,693.0524);
   f_energy_cal->SetParError(1,0.03878715);
   f_energy_cal->SetParLimits(1,0,0);
   f_energy_cal->Draw("same");
   
   TPaveText *pt = new TPaveText(0.2,0.67,0.55,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   TText *text = pt->AddText("Intercept = 1.7570e+00 #pm 4.0293e-02 keV");
   text = pt->AddText("Slope = 6.9305e+02 #pm 3.8787e-02 #frac{keV}{V}");
   pt->Draw();
   
   pt = new TPaveText(0.2333333,0.9326,0.7666667,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("HPGe Energy Calibration");
   pt->Draw();
   6f88259ceb3e4868b083bc561e728b7d->Modified();
   6f88259ceb3e4868b083bc561e728b7d->cd();
   6f88259ceb3e4868b083bc561e728b7d->SetSelected(6f88259ceb3e4868b083bc561e728b7d);
}
