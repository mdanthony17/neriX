{
//=========Macro generated from canvas: ffbf5a62ce414c249b5ad905abe76afc/
//=========  (Mon Apr 11 13:08:37 2016) by ROOT version5.34/26
   TCanvas *ffbf5a62ce414c249b5ad905abe76afc = new TCanvas("ffbf5a62ce414c249b5ad905abe76afc", "",10,45,704,527);
   ffbf5a62ce414c249b5ad905abe76afc->SetHighLightColor(2);
   ffbf5a62ce414c249b5ad905abe76afc->Range(263819.6,766.5489,576537.8,2122.315);
   ffbf5a62ce414c249b5ad905abe76afc->SetFillColor(0);
   ffbf5a62ce414c249b5ad905abe76afc->SetBorderMode(0);
   ffbf5a62ce414c249b5ad905abe76afc->SetBorderSize(2);
   ffbf5a62ce414c249b5ad905abe76afc->SetFrameBorderMode(0);
   ffbf5a62ce414c249b5ad905abe76afc->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,343028.8,1734.083);
   gre->SetPointError(0,1325.503,7.617851);
   gre->SetPoint(1,376161.6,1562.89);
   gre->SetPointError(1,1400.784,8.469125);
   gre->SetPoint(2,393876,1475.331);
   gre->SetPointError(2,1300.483,7.034175);
   gre->SetPoint(3,413656.5,1370.901);
   gre->SetPointError(3,1287.646,6.189691);
   gre->SetPoint(4,432005.4,1313.518);
   gre->SetPointError(4,1067.928,5.926361);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,295091.4,545266);
   Graph_Graph1->SetMinimum(902.1255);
   Graph_Graph1->SetMaximum(1986.738);
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
   
   
   TF1 *fG1 = new TF1("fG1","pol1",332566.3,442210.3);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(8.148491);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,3368.486);
   fG1->SetParError(0,51.94248);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.004789798);
   fG1->SetParError(1,0.0001302448);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0697 +/- 0.0032");
   text = pt->AddText("g2 = 14.5539 +/- 0.7831");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(420461.3,1362.722,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(495696.3,1002.362,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(327879.3,1806.126,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",295091.4,545266);
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
   fDraw->SetParameter(0,3368.486);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.004789798);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   ffbf5a62ce414c249b5ad905abe76afc->Modified();
   ffbf5a62ce414c249b5ad905abe76afc->cd();
   ffbf5a62ce414c249b5ad905abe76afc->SetSelected(ffbf5a62ce414c249b5ad905abe76afc);
}
