{
//=========Macro generated from canvas: 1d8e7e635e3c4b599f816ae22a50fe5d/
//=========  (Wed May  4 14:01:37 2016) by ROOT version5.34/26
   TCanvas *1d8e7e635e3c4b599f816ae22a50fe5d = new TCanvas("1d8e7e635e3c4b599f816ae22a50fe5d", "",10,45,704,527);
   1d8e7e635e3c4b599f816ae22a50fe5d->SetHighLightColor(2);
   1d8e7e635e3c4b599f816ae22a50fe5d->Range(260611.4,867.0549,569526.8,2400.582);
   1d8e7e635e3c4b599f816ae22a50fe5d->SetFillColor(0);
   1d8e7e635e3c4b599f816ae22a50fe5d->SetBorderMode(0);
   1d8e7e635e3c4b599f816ae22a50fe5d->SetBorderSize(2);
   1d8e7e635e3c4b599f816ae22a50fe5d->SetFrameBorderMode(0);
   1d8e7e635e3c4b599f816ae22a50fe5d->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,369459.3,1790.764);
   gre->SetPointError(0,1764.412,11.47078);
   gre->SetPoint(1,400450.4,1610.311);
   gre->SetPointError(1,1461.093,8.592656);
   gre->SetPoint(2,418054.9,1506.728);
   gre->SetPointError(2,1318.574,7.24963);
   gre->SetPoint(3,435746.2,1431.694);
   gre->SetPointError(3,1193.795,6.890814);
   gre->SetPoint(4,449379.2,1342.585);
   gre->SetPointError(4,1227.955,7.234931);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,291503,538635.3);
   Graph_Graph1->SetMinimum(1020.408);
   Graph_Graph1->SetMaximum(2247.23);
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
   
   
   TF1 *fG1 = new TF1("fG1","pol1",359403.7,458898.4);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(2.901483);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,3810.145);
   fG1->SetParError(0,86.23922);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.005484506);
   fG1->SetParError(1,0.000203959);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0789 +/- 0.0039");
   text = pt->AddText("g2 = 14.3769 +/- 0.8878");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(415348.3,1541.396,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(489668.5,1133.786,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(323892.2,2042.936,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",291503,538635.3);
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
   fDraw->SetParameter(0,3810.145);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.005484506);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   1d8e7e635e3c4b599f816ae22a50fe5d->Modified();
   1d8e7e635e3c4b599f816ae22a50fe5d->cd();
   1d8e7e635e3c4b599f816ae22a50fe5d->SetSelected(1d8e7e635e3c4b599f816ae22a50fe5d);
}
