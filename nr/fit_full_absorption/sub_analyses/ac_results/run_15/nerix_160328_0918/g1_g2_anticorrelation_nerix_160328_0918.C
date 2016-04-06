{
//=========Macro generated from canvas: d0166bef7ca943f58be1df5f1eda0119/
//=========  (Tue Mar 29 11:45:53 2016) by ROOT version5.34/26
   TCanvas *d0166bef7ca943f58be1df5f1eda0119 = new TCanvas("d0166bef7ca943f58be1df5f1eda0119", "",10,45,704,527);
   d0166bef7ca943f58be1df5f1eda0119->SetHighLightColor(2);
   d0166bef7ca943f58be1df5f1eda0119->Range(166083.1,639.8914,534211.3,2389.704);
   d0166bef7ca943f58be1df5f1eda0119->SetFillColor(0);
   d0166bef7ca943f58be1df5f1eda0119->SetBorderMode(0);
   d0166bef7ca943f58be1df5f1eda0119->SetBorderSize(2);
   d0166bef7ca943f58be1df5f1eda0119->SetFrameBorderMode(0);
   d0166bef7ca943f58be1df5f1eda0119->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,238695,1946.587);
   gre->SetPointError(0,2183.785,12.04552);
   gre->SetPoint(1,268634.8,1765.728);
   gre->SetPointError(1,1531.77,8.520756);
   gre->SetPoint(2,297069.1,1614.131);
   gre->SetPointError(2,1385.089,7.651966);
   gre->SetPoint(3,318752.7,1513.786);
   gre->SetPointError(3,1271.072,6.810324);
   gre->SetPoint(4,346160.1,1416.692);
   gre->SetPointError(4,1064.575,5.5749);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,202895.9,497398.5);
   Graph_Graph1->SetMinimum(814.8727);
   Graph_Graph1->SetMaximum(2214.723);
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
   
   
   TF1 *fG1 = new TF1("fG1","pol1",225439.9,358296);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(16.92161);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,3042.689);
   fG1->SetParError(0,40.31786);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.0047429);
   fG1->SetParError(1,0.0001292503);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0630 +/- 0.0029");
   text = pt->AddText("g2 = 13.2763 +/- 0.7071");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(383550.1,1230.921,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(452180.5,905.4141,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(299095.7,1631.439,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",202895.9,497398.5);
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
   fDraw->SetParameter(0,3042.689);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.0047429);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   d0166bef7ca943f58be1df5f1eda0119->Modified();
   d0166bef7ca943f58be1df5f1eda0119->cd();
   d0166bef7ca943f58be1df5f1eda0119->SetSelected(d0166bef7ca943f58be1df5f1eda0119);
}
