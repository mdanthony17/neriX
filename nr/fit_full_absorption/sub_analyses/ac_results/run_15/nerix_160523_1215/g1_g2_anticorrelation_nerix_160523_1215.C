{
//=========Macro generated from canvas: e8a87c6a272b4086b0273581f922af0d/
//=========  (Mon May 23 14:47:49 2016) by ROOT version5.34/26
   TCanvas *e8a87c6a272b4086b0273581f922af0d = new TCanvas("e8a87c6a272b4086b0273581f922af0d", "",10,45,704,527);
   e8a87c6a272b4086b0273581f922af0d->SetHighLightColor(2);
   e8a87c6a272b4086b0273581f922af0d->Range(256783.7,902.5657,561161.9,2498.9);
   e8a87c6a272b4086b0273581f922af0d->SetFillColor(0);
   e8a87c6a272b4086b0273581f922af0d->SetBorderMode(0);
   e8a87c6a272b4086b0273581f922af0d->SetBorderSize(2);
   e8a87c6a272b4086b0273581f922af0d->SetFrameBorderMode(0);
   e8a87c6a272b4086b0273581f922af0d->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,369240.4,1826.583);
   gre->SetPointError(0,1866.258,11.64908);
   gre->SetPoint(1,404674.8,1619.303);
   gre->SetPointError(1,1464.477,8.582061);
   gre->SetPoint(2,422483.8,1517.778);
   gre->SetPointError(2,1270.616,7.28329);
   gre->SetPoint(3,437454.4,1440.196);
   gre->SetPointError(3,1504.627,8.794884);
   gre->SetPoint(4,451217.9,1347.892);
   gre->SetPointError(4,1261.317,6.967292);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,287221.5,530724.1);
   Graph_Graph1->SetMinimum(1062.199);
   Graph_Graph1->SetMaximum(2339.266);
   Graph_Graph1->SetDirectory(0);
   Graph_Graph1->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph1->SetLineColor(ci);
   Graph_Graph1->GetXaxis()->SetTitle("S2 at 662 keV Peak");
   Graph_Graph1->GetXaxis()->SetLabelFont(42);
   Graph_Graph1->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleOffset(1.12);
   Graph_Graph1->GetXaxis()->SetTitleFont(42);
   Graph_Graph1->GetYaxis()->SetTitle("S1 at 662 keV Peak [PE]");
   Graph_Graph1->GetYaxis()->SetLabelFont(42);
   Graph_Graph1->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleFont(42);
   Graph_Graph1->GetZaxis()->SetLabelFont(42);
   Graph_Graph1->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1);
   
   
   TF1 *fG1 = new TF1("fG1","pol1",358863.6,460989.7);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(0.6724134);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,3966.192);
   fG1->SetParError(0,91.42256);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.00579423);
   fG1->SetParError(1,0.0002153842);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0821 +/- 0.0041");
   text = pt->AddText("g2 = 14.1658 +/- 0.8768");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(409247.8,1604.524,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(482476.4,1180.221,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(319135,2126.606,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",287221.5,530724.1);
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
   fDraw->SetParameter(0,3966.192);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.00579423);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   e8a87c6a272b4086b0273581f922af0d->Modified();
   e8a87c6a272b4086b0273581f922af0d->cd();
   e8a87c6a272b4086b0273581f922af0d->SetSelected(e8a87c6a272b4086b0273581f922af0d);
}
