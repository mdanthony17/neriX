{
//=========Macro generated from canvas: 43e6fd071f9b40ec9bda878b1d128053/
//=========  (Wed May 11 11:30:14 2016) by ROOT version5.34/26
   TCanvas *43e6fd071f9b40ec9bda878b1d128053 = new TCanvas("43e6fd071f9b40ec9bda878b1d128053", "",10,45,704,527);
   43e6fd071f9b40ec9bda878b1d128053->SetHighLightColor(2);
   43e6fd071f9b40ec9bda878b1d128053->Range(262325.5,834.3957,573272.7,2310.16);
   43e6fd071f9b40ec9bda878b1d128053->SetFillColor(0);
   43e6fd071f9b40ec9bda878b1d128053->SetBorderMode(0);
   43e6fd071f9b40ec9bda878b1d128053->SetBorderSize(2);
   43e6fd071f9b40ec9bda878b1d128053->SetFrameBorderMode(0);
   43e6fd071f9b40ec9bda878b1d128053->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,361054.1,1776.765);
   gre->SetPointError(0,1441.507,8.401088);
   gre->SetPoint(1,395154,1591.285);
   gre->SetPointError(1,1429.844,8.614274);
   gre->SetPoint(2,413693.6,1492.575);
   gre->SetPointError(2,1215,6.924082);
   gre->SetPoint(3,428795.9,1423.481);
   gre->SetPointError(3,1376.232,7.345525);
   gre->SetPoint(4,446874.9,1323.898);
   gre->SetPointError(4,1178.031,5.776908);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,293420.2,542178);
   Graph_Graph1->SetMinimum(981.9722);
   Graph_Graph1->SetMaximum(2162.584);
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
   
   
   TF1 *fG1 = new TF1("fG1","pol1",350768.5,456896.9);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(0.7008402);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,3666.629);
   fG1->SetParError(0,64.13806);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.005243436);
   fG1->SetParError(1,0.0001542827);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0759 +/- 0.0036");
   text = pt->AddText("g2 = 14.4715 +/- 0.8044");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(418080.1,1483.336,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(492889,1091.08,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(326022.5,1965.985,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",293420.2,542178);
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
   fDraw->SetParameter(0,3666.629);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.005243436);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   43e6fd071f9b40ec9bda878b1d128053->Modified();
   43e6fd071f9b40ec9bda878b1d128053->cd();
   43e6fd071f9b40ec9bda878b1d128053->SetSelected(43e6fd071f9b40ec9bda878b1d128053);
}
