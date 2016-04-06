{
//=========Macro generated from canvas: 10767305fab247cea5bebd6415e5a85d/
//=========  (Wed Mar  2 15:00:13 2016) by ROOT version5.34/26
   TCanvas *10767305fab247cea5bebd6415e5a85d = new TCanvas("10767305fab247cea5bebd6415e5a85d", "",10,45,704,527);
   10767305fab247cea5bebd6415e5a85d->SetHighLightColor(2);
   10767305fab247cea5bebd6415e5a85d->Range(80639.3,333.3532,185167.7,759.0406);
   10767305fab247cea5bebd6415e5a85d->SetFillColor(0);
   10767305fab247cea5bebd6415e5a85d->SetBorderMode(0);
   10767305fab247cea5bebd6415e5a85d->SetBorderSize(2);
   10767305fab247cea5bebd6415e5a85d->SetFrameBorderMode(0);
   10767305fab247cea5bebd6415e5a85d->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,105835.3,632.2841);
   gre->SetPointError(0,613.7458,2.515838);
   gre->SetPoint(1,118777.4,587.9867);
   gre->SetPointError(1,623.2255,2.46215);
   gre->SetPoint(2,125345.9,554.3472);
   gre->SetPointError(2,585.0067,2.524541);
   gre->SetPoint(3,133943,520.0312);
   gre->SetPointError(3,600.2721,2.40733);
   gre->SetPoint(4,144678.1,471.8949);
   gre->SetPointError(4,623.62,2.475949);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,91092.15,174714.9);
   Graph_Graph1->SetMinimum(375.922);
   Graph_Graph1->SetMaximum(716.4719);
   Graph_Graph1->SetDirectory(0);
   Graph_Graph1->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph1->SetLineColor(ci);
   Graph_Graph1->GetXaxis()->SetTitle("S2 at 163 keV Peak");
   Graph_Graph1->GetXaxis()->SetLabelFont(42);
   Graph_Graph1->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleOffset(1.12);
   Graph_Graph1->GetXaxis()->SetTitleFont(42);
   Graph_Graph1->GetYaxis()->SetTitle("S1 at 163 keV Peak [PE]");
   Graph_Graph1->GetYaxis()->SetLabelFont(42);
   Graph_Graph1->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleFont(42);
   Graph_Graph1->GetZaxis()->SetLabelFont(42);
   Graph_Graph1->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1);
   
   
   TF1 *fG1 = new TF1("fG1","pol1",101213.5,149309.7);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(4.304203);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,1078.139);
   fG1->SetParError(0,15.13823);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.004174605);
   fG1->SetParError(1,0.0001196696);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0901 +/- 0.0041");
   text = pt->AddText("g2 = 21.5874 +/- 1.1699");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(132453.4,527.8102,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(158831.7,417.6911,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(104556.7,644.2679,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",91092.15,174714.9);
   fDraw->SetFillColor(19);
   fDraw->SetFillStyle(0);
   fDraw->SetLineColor(2);
   fDraw->SetLineWidth(1);
   fDraw->SetLineStyle(9);
   fDraw->GetXaxis()->SetLabelFont(42);
   fDraw->GetXaxis()->SetLabelSize(0.035);
   fDraw->GetXaxis()->SetTitleSize(0.035);
   fDraw->GetXaxis()->SetTitleFont(42);
   fDraw->GetYaxis()->SetLabelFont(42);
   fDraw->GetYaxis()->SetLabelSize(0.035);
   fDraw->GetYaxis()->SetTitleSize(0.035);
   fDraw->GetYaxis()->SetTitleFont(42);
   fDraw->SetParameter(0,1078.139);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.004174605);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   10767305fab247cea5bebd6415e5a85d->Modified();
   10767305fab247cea5bebd6415e5a85d->cd();
   10767305fab247cea5bebd6415e5a85d->SetSelected(10767305fab247cea5bebd6415e5a85d);
}
