{
//=========Macro generated from canvas: 900a1e26fe68428595ef2157fb0cb31b/
//=========  (Wed Feb 10 09:13:12 2016) by ROOT version5.34/26
   TCanvas *900a1e26fe68428595ef2157fb0cb31b = new TCanvas("900a1e26fe68428595ef2157fb0cb31b", "",10,45,704,527);
   900a1e26fe68428595ef2157fb0cb31b->SetHighLightColor(2);
   900a1e26fe68428595ef2157fb0cb31b->Range(383573.9,998.0268,838242.8,2763.199);
   900a1e26fe68428595ef2157fb0cb31b->SetFillColor(0);
   900a1e26fe68428595ef2157fb0cb31b->SetBorderMode(0);
   900a1e26fe68428595ef2157fb0cb31b->SetBorderSize(2);
   900a1e26fe68428595ef2157fb0cb31b->SetFrameBorderMode(0);
   900a1e26fe68428595ef2157fb0cb31b->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,539570,2080.554);
   gre->SetPointError(0,2234.026,10.00657);
   gre->SetPoint(1,581779.8,1888.473);
   gre->SetPointError(1,1832.24,8.085267);
   gre->SetPoint(2,610836.4,1751.013);
   gre->SetPointError(2,1858.533,8.067863);
   gre->SetPoint(3,634800.2,1665.674);
   gre->SetPointError(3,1554.481,6.994148);
   gre->SetPoint(4,661354.4,1552.849);
   gre->SetPointError(4,1451.642,6.097924);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,429040.8,792775.9);
   Graph_Graph1->SetMinimum(1174.544);
   Graph_Graph1->SetMaximum(2586.682);
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
   
   
   TF1 *fG1 = new TF1("fG1","pol1",524789,675353.1);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(2.417648);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,4385.682);
   fG1->SetParError(0,74.43186);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.004289212);
   fG1->SetParError(1,0.0001201894);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0908 +/- 0.0043");
   text = pt->AddText("g2 = 21.1603 +/- 1.1573");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(611319.2,1774.229,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(720705.4,1305.049,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(476712,2351.529,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",429040.8,792775.9);
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
   fDraw->SetParameter(0,4385.682);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.004289212);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   900a1e26fe68428595ef2157fb0cb31b->Modified();
   900a1e26fe68428595ef2157fb0cb31b->cd();
   900a1e26fe68428595ef2157fb0cb31b->SetSelected(900a1e26fe68428595ef2157fb0cb31b);
}
