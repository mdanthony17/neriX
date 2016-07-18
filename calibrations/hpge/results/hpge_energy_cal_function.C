{
//=========Macro generated from canvas: bdb42406a59d45f59c42902ac246c7ae/
//=========  (Fri Jul  8 12:37:53 2016) by ROOT version5.34/26
   TCanvas *bdb42406a59d45f59c42902ac246c7ae = new TCanvas("bdb42406a59d45f59c42902ac246c7ae", "",10,45,604,527);
   bdb42406a59d45f59c42902ac246c7ae->SetHighLightColor(2);
   bdb42406a59d45f59c42902ac246c7ae->Range(-0.2422978,-165,2.251873,1557);
   bdb42406a59d45f59c42902ac246c7ae->SetFillColor(0);
   bdb42406a59d45f59c42902ac246c7ae->SetBorderMode(0);
   bdb42406a59d45f59c42902ac246c7ae->SetBorderSize(2);
   bdb42406a59d45f59c42902ac246c7ae->SetFrameBorderMode(0);
   bdb42406a59d45f59c42902ac246c7ae->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(4);
   gre->SetName("Graph0");
   gre->SetTitle("HPGe Energy Calibration");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(32);
   gre->SetPoint(0,0.1734034,122);
   gre->SetPointError(0,6.078086e-06,0);
   gre->SetPoint(1,0.1941432,137);
   gre->SetPointError(1,8.456652e-06,0);
   gre->SetPoint(2,0.9520195,662);
   gre->SetPointError(2,1.674324e-05,0);
   gre->SetPoint(3,1.836139,1270);
   gre->SetPointError(3,3.852809e-05,0);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","HPGe Energy Calibration",100,0.007119266,2.002456);
   Graph_Graph1->SetMinimum(7.2);
   Graph_Graph1->SetMaximum(1384.8);
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
   f_energy_cal->SetChisquare(26255.36);
   f_energy_cal->SetNDF(2);
   f_energy_cal->GetXaxis()->SetLabelFont(42);
   f_energy_cal->GetXaxis()->SetLabelSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleSize(0.035);
   f_energy_cal->GetXaxis()->SetTitleFont(42);
   f_energy_cal->GetYaxis()->SetLabelFont(42);
   f_energy_cal->GetYaxis()->SetLabelSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleSize(0.035);
   f_energy_cal->GetYaxis()->SetTitleFont(42);
   f_energy_cal->SetParameter(0,2.294288);
   f_energy_cal->SetParError(0,0.004457915);
   f_energy_cal->SetParLimits(0,0,0);
   f_energy_cal->SetParameter(1,691.8725);
   f_energy_cal->SetParError(1,0.01147331);
   f_energy_cal->SetParLimits(1,0,0);
   f_energy_cal->Draw("same");
   
   TPaveText *pt = new TPaveText(0.2,0.67,0.55,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   TText *text = pt->AddText("Intercept = 2.29e+00 #pm 4.46e-03 keV");
   text = pt->AddText("Slope = 6.92e+02 #pm 1.15e-02 #frac{keV}{V}");
   pt->Draw();
   
   pt = new TPaveText(0.2425,0.9374,0.7575,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("HPGe Energy Calibration");
   pt->Draw();
   bdb42406a59d45f59c42902ac246c7ae->Modified();
   bdb42406a59d45f59c42902ac246c7ae->cd();
   bdb42406a59d45f59c42902ac246c7ae->SetSelected(bdb42406a59d45f59c42902ac246c7ae);
}
