{
//=========Macro generated from canvas: a35364bdddd84bc79b99de75b1a2ac99/
//=========  (Mon May 16 18:20:31 2016) by ROOT version5.34/26
   TCanvas *a35364bdddd84bc79b99de75b1a2ac99 = new TCanvas("a35364bdddd84bc79b99de75b1a2ac99", "",10,45,704,527);
   a35364bdddd84bc79b99de75b1a2ac99->SetHighLightColor(2);
   a35364bdddd84bc79b99de75b1a2ac99->Range(186679.1,735.3722,409898.2,1960.585);
   a35364bdddd84bc79b99de75b1a2ac99->SetFillColor(0);
   a35364bdddd84bc79b99de75b1a2ac99->SetBorderMode(0);
   a35364bdddd84bc79b99de75b1a2ac99->SetBorderSize(2);
   a35364bdddd84bc79b99de75b1a2ac99->SetFrameBorderMode(0);
   a35364bdddd84bc79b99de75b1a2ac99->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,261696.8,1473.375);
   gre->SetPointError(0,2012.195,10.60554);
   gre->SetPoint(1,295438.6,1301.763);
   gre->SetPointError(1,1555.713,8.722347);
   gre->SetPoint(2,309319.8,1209.471);
   gre->SetPointError(2,1327.198,7.421999);
   gre->SetPoint(3,317802.2,1140.745);
   gre->SetPointError(3,1480.159,6.370798);
   gre->SetPoint(4,336246.5,1041.823);
   gre->SetPointError(4,991.5976,6.181446);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,209001,387576.3);
   Graph_Graph1->SetMinimum(857.8935);
   Graph_Graph1->SetMaximum(1838.064);
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
   
   
   TF1 *fG1 = new TF1("fG1","pol1",251929.2,344993.5);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(3.805202);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,3051.193);
   fG1->SetParError(0,70.49194);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.005975358);
   fG1->SetParError(1,0.000224251);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0818 +/- 0.0041");
   text = pt->AddText("g2 = 13.6901 +/- 0.8506");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(297561.2,1280.55,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(352342.1,953.215,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(232223.4,1670.967,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",209001,387576.3);
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
   fDraw->SetParameter(0,3051.193);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.005975358);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   a35364bdddd84bc79b99de75b1a2ac99->Modified();
   a35364bdddd84bc79b99de75b1a2ac99->cd();
   a35364bdddd84bc79b99de75b1a2ac99->SetSelected(a35364bdddd84bc79b99de75b1a2ac99);
}
