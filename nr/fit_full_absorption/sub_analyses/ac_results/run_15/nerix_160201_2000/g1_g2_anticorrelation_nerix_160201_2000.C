{
//=========Macro generated from canvas: 057820e782184174ae2c2a15581716dd/
//=========  (Wed Feb 10 08:58:57 2016) by ROOT version5.34/26
   TCanvas *057820e782184174ae2c2a15581716dd = new TCanvas("057820e782184174ae2c2a15581716dd", "",10,45,704,527);
   057820e782184174ae2c2a15581716dd->SetHighLightColor(2);
   057820e782184174ae2c2a15581716dd->Range(73957.56,349.4881,197434.1,886.3099);
   057820e782184174ae2c2a15581716dd->SetFillColor(0);
   057820e782184174ae2c2a15581716dd->SetBorderMode(0);
   057820e782184174ae2c2a15581716dd->SetBorderSize(2);
   057820e782184174ae2c2a15581716dd->SetFrameBorderMode(0);
   057820e782184174ae2c2a15581716dd->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,102044,731.0436);
   gre->SetPointError(0,623.5005,2.953753);
   gre->SetPoint(1,122351.4,638.3283);
   gre->SetPointError(1,590.8786,2.745103);
   gre->SetPoint(2,131498.4,594.6851);
   gre->SetPointError(2,642.7258,2.435641);
   gre->SetPoint(3,140620.7,553.1763);
   gre->SetPointError(3,634.8268,2.489241);
   gre->SetPoint(4,156064.8,507.2519);
   gre->SetPointError(4,613.7711,2.623607);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,86305.22,185086.4);
   Graph_Graph1->SetMinimum(403.1703);
   Graph_Graph1->SetMaximum(832.6277);
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
   
   
   TF1 *fG1 = new TF1("fG1","pol1",95894.69,162204.4);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(18.89428);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,1156.287);
   fG1->SetParError(0,12.55656);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.004226312);
   fG1->SetParError(1,9.469084e-05);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0967 +/- 0.0044");
   text = pt->AddText("g2 = 22.8689 +/- 1.1521");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(140316.2,566.068,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(168260.4,447.967,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(110763.4,690.967,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",86305.22,185086.4);
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
   fDraw->SetParameter(0,1156.287);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.004226312);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   057820e782184174ae2c2a15581716dd->Modified();
   057820e782184174ae2c2a15581716dd->cd();
   057820e782184174ae2c2a15581716dd->SetSelected(057820e782184174ae2c2a15581716dd);
}
