{
//=========Macro generated from canvas: 85241cf7ff7741008bb3216c05346ff7/
//=========  (Mon Apr  4 15:57:55 2016) by ROOT version5.34/26
   TCanvas *85241cf7ff7741008bb3216c05346ff7 = new TCanvas("85241cf7ff7741008bb3216c05346ff7", "",10,45,704,527);
   85241cf7ff7741008bb3216c05346ff7->SetHighLightColor(2);
   85241cf7ff7741008bb3216c05346ff7->Range(254000.4,933.1539,555079.4,2583.588);
   85241cf7ff7741008bb3216c05346ff7->SetFillColor(0);
   85241cf7ff7741008bb3216c05346ff7->SetBorderMode(0);
   85241cf7ff7741008bb3216c05346ff7->SetBorderSize(2);
   85241cf7ff7741008bb3216c05346ff7->SetFrameBorderMode(0);
   85241cf7ff7741008bb3216c05346ff7->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,365530.5,1891.385);
   gre->SetPointError(0,1311.099,7.358098);
   gre->SetPoint(1,394537,1693.317);
   gre->SetPointError(1,1477.96,8.721353);
   gre->SetPoint(2,416809.2,1577.137);
   gre->SetPointError(2,1483.303,8.004059);
   gre->SetPoint(3,435827.6,1479.61);
   gre->SetPointError(3,1253.2,7.009686);
   gre->SetPoint(4,448706.3,1373.108);
   gre->SetPointError(4,1290.874,6.023146);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,284108.3,524971.5);
   Graph_Graph1->SetMinimum(1098.197);
   Graph_Graph1->SetMaximum(2418.545);
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
   
   
   TF1 *fG1 = new TF1("fG1","pol1",355641.7,458575);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(6.457322);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,4100.608);
   fG1->SetParError(0,68.04516);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.006056243);
   fG1->SetParError(1,0.0001635129);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0849 +/- 0.0040");
   text = pt->AddText("g2 = 14.0122 +/- 0.7575");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(404811.9,1658.902,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(477246.8,1220.219,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(315675.8,2198.677,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",284108.3,524971.5);
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
   fDraw->SetParameter(0,4100.608);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.006056243);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   85241cf7ff7741008bb3216c05346ff7->Modified();
   85241cf7ff7741008bb3216c05346ff7->cd();
   85241cf7ff7741008bb3216c05346ff7->SetSelected(85241cf7ff7741008bb3216c05346ff7);
}
