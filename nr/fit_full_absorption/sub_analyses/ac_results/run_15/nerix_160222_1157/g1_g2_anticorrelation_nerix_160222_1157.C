{
//=========Macro generated from canvas: a1268c8cb39f4f81b205a198796c1c0d/
//=========  (Tue Feb 23 12:22:12 2016) by ROOT version5.34/26
   TCanvas *a1268c8cb39f4f81b205a198796c1c0d = new TCanvas("a1268c8cb39f4f81b205a198796c1c0d", "",10,45,704,527);
   a1268c8cb39f4f81b205a198796c1c0d->SetHighLightColor(2);
   a1268c8cb39f4f81b205a198796c1c0d->Range(375081.7,984.5131,819684.3,2725.785);
   a1268c8cb39f4f81b205a198796c1c0d->SetFillColor(0);
   a1268c8cb39f4f81b205a198796c1c0d->SetBorderMode(0);
   a1268c8cb39f4f81b205a198796c1c0d->SetBorderSize(2);
   a1268c8cb39f4f81b205a198796c1c0d->SetFrameBorderMode(0);
   a1268c8cb39f4f81b205a198796c1c0d->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,526320.2,2061.573);
   gre->SetPointError(0,2019.61,8.970716);
   gre->SetPoint(1,571723.1,1847.013);
   gre->SetPointError(1,1922.857,8.652706);
   gre->SetPoint(2,597597.2,1732.496);
   gre->SetPointError(2,1635.476,7.291365);
   gre->SetPoint(3,617617.9,1644.769);
   gre->SetPointError(3,1575.679,6.648979);
   gre->SetPoint(4,650726.1,1519.465);
   gre->SetPointError(4,1325.881,5.644438);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,419542,775224);
   Graph_Graph1->SetMinimum(1158.64);
   Graph_Graph1->SetMaximum(2551.657);
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
   
   
   TF1 *fG1 = new TF1("fG1","pol1",511525.5,664827.1);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(3.971041);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,4326.298);
   fG1->SetParError(0,68.11399);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.004326931);
   fG1->SetParError(1,0.0001121879);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0895 +/- 0.0042");
   text = pt->AddText("g2 = 20.6918 +/- 1.1024");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(597784.8,1750.206,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(704749.1,1287.378,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(466157.7,2319.689,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",419542,775224);
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
   fDraw->SetParameter(0,4326.298);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.004326931);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   a1268c8cb39f4f81b205a198796c1c0d->Modified();
   a1268c8cb39f4f81b205a198796c1c0d->cd();
   a1268c8cb39f4f81b205a198796c1c0d->SetSelected(a1268c8cb39f4f81b205a198796c1c0d);
}
