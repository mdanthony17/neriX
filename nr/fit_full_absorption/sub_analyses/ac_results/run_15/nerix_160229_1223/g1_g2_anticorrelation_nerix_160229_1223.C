{
//=========Macro generated from canvas: 0234dd7e3f3d4857be5dc23e134238c5/
//=========  (Wed Mar  2 15:13:16 2016) by ROOT version5.34/26
   TCanvas *0234dd7e3f3d4857be5dc23e134238c5 = new TCanvas("0234dd7e3f3d4857be5dc23e134238c5", "",10,45,704,527);
   0234dd7e3f3d4857be5dc23e134238c5->SetHighLightColor(2);
   0234dd7e3f3d4857be5dc23e134238c5->Range(377314.4,961.0547,824563.5,2660.836);
   0234dd7e3f3d4857be5dc23e134238c5->SetFillColor(0);
   0234dd7e3f3d4857be5dc23e134238c5->SetBorderMode(0);
   0234dd7e3f3d4857be5dc23e134238c5->SetBorderSize(2);
   0234dd7e3f3d4857be5dc23e134238c5->SetFrameBorderMode(0);
   0234dd7e3f3d4857be5dc23e134238c5->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,531159.4,2005.55);
   gre->SetPointError(0,2097.626,9.355845);
   gre->SetPoint(1,573241.4,1814.301);
   gre->SetPointError(1,1780.115,7.853228);
   gre->SetPoint(2,597617.6,1699.373);
   gre->SetPointError(2,1702.124,7.439122);
   gre->SetPoint(3,621690.2,1608.566);
   gre->SetPointError(3,1560.202,6.820158);
   gre->SetPoint(4,648130.1,1510.46);
   gre->SetPointError(4,1398.58,5.895215);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,422039.3,779838.6);
   Graph_Graph1->SetMinimum(1131.033);
   Graph_Graph1->SetMaximum(2490.858);
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
   
   
   TF1 *fG1 = new TF1("fG1","pol1",517015.1,661575.4);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(4.261698);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,4223.214);
   fG1->SetParError(0,73.07393);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.004198838);
   fG1->SetParError(1,0.0001204667);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0874 +/- 0.0041");
   text = pt->AddText("g2 = 20.8150 +/- 1.1478");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(601343.1,1708.503,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(708944.2,1256.703,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(468932.6,2264.416,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",422039.3,779838.6);
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
   fDraw->SetParameter(0,4223.214);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.004198838);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   0234dd7e3f3d4857be5dc23e134238c5->Modified();
   0234dd7e3f3d4857be5dc23e134238c5->cd();
   0234dd7e3f3d4857be5dc23e134238c5->SetSelected(0234dd7e3f3d4857be5dc23e134238c5);
}
