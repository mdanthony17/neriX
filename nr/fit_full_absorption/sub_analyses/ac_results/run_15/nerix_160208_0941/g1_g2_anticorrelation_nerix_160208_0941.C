{
//=========Macro generated from canvas: 1d3abc897e234c8c8585f923596a8211/
//=========  (Wed Feb 10 09:16:12 2016) by ROOT version5.34/26
   TCanvas *1d3abc897e234c8c8585f923596a8211 = new TCanvas("1d3abc897e234c8c8585f923596a8211", "",10,45,704,527);
   1d3abc897e234c8c8585f923596a8211->SetHighLightColor(2);
   1d3abc897e234c8c8585f923596a8211->Range(387974,950.9348,847858.5,2632.817);
   1d3abc897e234c8c8585f923596a8211->SetFillColor(0);
   1d3abc897e234c8c8585f923596a8211->SetBorderMode(0);
   1d3abc897e234c8c8585f923596a8211->SetBorderSize(2);
   1d3abc897e234c8c8585f923596a8211->SetFrameBorderMode(0);
   1d3abc897e234c8c8585f923596a8211->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,522699.9,2083.35);
   gre->SetPointError(0,2068.817,9.181822);
   gre->SetPoint(1,573368.6,1856.066);
   gre->SetPointError(1,1774.631,7.518584);
   gre->SetPoint(2,599634.5,1741.446);
   gre->SetPointError(2,1619.272,7.049924);
   gre->SetPoint(3,623926.4,1653.731);
   gre->SetPointError(3,1659.936,7.467148);
   gre->SetPoint(4,658050.2,1529.812);
   gre->SetPointError(4,1428.036,5.917234);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,433962.5,801870.1);
   Graph_Graph1->SetMinimum(1119.123);
   Graph_Graph1->SetMaximum(2464.629);
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
   
   
   TF1 *fG1 = new TF1("fG1","pol1",506746.4,673363);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(5.980619);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,4178.743);
   fG1->SetParError(0,60.3908);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.004040475);
   fG1->SetParError(1,9.929951e-05);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0865 +/- 0.0040");
   text = pt->AddText("g2 = 21.4031 +/- 1.1185");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(618331.9,1690.512,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(728972.8,1243.47,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(482180.5,2240.572,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",433962.5,801870.1);
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
   fDraw->SetParameter(0,4178.743);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.004040475);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   1d3abc897e234c8c8585f923596a8211->Modified();
   1d3abc897e234c8c8585f923596a8211->cd();
   1d3abc897e234c8c8585f923596a8211->SetSelected(1d3abc897e234c8c8585f923596a8211);
}
