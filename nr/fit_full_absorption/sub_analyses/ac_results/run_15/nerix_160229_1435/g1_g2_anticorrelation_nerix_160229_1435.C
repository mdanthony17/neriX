{
//=========Macro generated from canvas: 1f4a1f548ac644fea7ded5a065b94b68/
//=========  (Wed Mar  2 15:08:29 2016) by ROOT version5.34/26
   TCanvas *1f4a1f548ac644fea7ded5a065b94b68 = new TCanvas("1f4a1f548ac644fea7ded5a065b94b68", "",10,45,704,527);
   1f4a1f548ac644fea7ded5a065b94b68->SetHighLightColor(2);
   1f4a1f548ac644fea7ded5a065b94b68->Range(275848,792.8844,605689.6,2113.919);
   1f4a1f548ac644fea7ded5a065b94b68->SetFillColor(0);
   1f4a1f548ac644fea7ded5a065b94b68->SetBorderMode(0);
   1f4a1f548ac644fea7ded5a065b94b68->SetBorderSize(2);
   1f4a1f548ac644fea7ded5a065b94b68->SetFrameBorderMode(0);
   1f4a1f548ac644fea7ded5a065b94b68->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,385784.5,1606.336);
   gre->SetPointError(0,2433.068,10.33107);
   gre->SetPoint(1,428026.7,1424.579);
   gre->SetPointError(1,1849.531,7.520267);
   gre->SetPoint(2,450727.9,1331.555);
   gre->SetPointError(2,1612.192,7.300535);
   gre->SetPoint(3,458424.7,1281.424);
   gre->SetPointError(3,1711.607,7.505445);
   gre->SetPoint(4,484237.5,1180.19);
   gre->SetPointError(4,1662.709,6.773179);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,308832.2,572705.4);
   Graph_Graph1->SetMinimum(924.9879);
   Graph_Graph1->SetMaximum(1981.816);
   Graph_Graph1->SetDirectory(0);
   Graph_Graph1->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph1->SetLineColor(ci);
   Graph_Graph1->GetXaxis()->SetTitle("S2 at 511 keV Peak");
   Graph_Graph1->GetXaxis()->SetLabelFont(42);
   Graph_Graph1->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleOffset(1.12);
   Graph_Graph1->GetXaxis()->SetTitleFont(42);
   Graph_Graph1->GetYaxis()->SetTitle("S1 at 511 keV Peak [PE]");
   Graph_Graph1->GetYaxis()->SetLabelFont(42);
   Graph_Graph1->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleFont(42);
   Graph_Graph1->GetZaxis()->SetLabelFont(42);
   Graph_Graph1->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1);
   
   
   TF1 *fG1 = new TF1("fG1","pol1",373096.5,496155.1);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(1.343312);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,3289.822);
   fG1->SetParError(0,75.55999);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.00436006);
   fG1->SetParError(1,0.0001679462);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0882 +/- 0.0044");
   text = pt->AddText("g2 = 20.2292 +/- 1.2681");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(439693.8,1380.7,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(520641.3,1027.764,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(343146.9,1801.651,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",308832.2,572705.4);
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
   fDraw->SetParameter(0,3289.822);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.00436006);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   1f4a1f548ac644fea7ded5a065b94b68->Modified();
   1f4a1f548ac644fea7ded5a065b94b68->cd();
   1f4a1f548ac644fea7ded5a065b94b68->SetSelected(1f4a1f548ac644fea7ded5a065b94b68);
}
