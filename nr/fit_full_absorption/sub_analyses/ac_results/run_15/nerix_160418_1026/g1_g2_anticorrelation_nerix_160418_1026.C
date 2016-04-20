{
//=========Macro generated from canvas: 61edc96ad38746c783a5b1ee6380d6a2/
//=========  (Tue Apr 19 11:37:51 2016) by ROOT version5.34/26
   TCanvas *61edc96ad38746c783a5b1ee6380d6a2 = new TCanvas("61edc96ad38746c783a5b1ee6380d6a2", "",10,45,704,527);
   61edc96ad38746c783a5b1ee6380d6a2->SetHighLightColor(2);
   61edc96ad38746c783a5b1ee6380d6a2->Range(261657.5,947.2555,571813,2622.631);
   61edc96ad38746c783a5b1ee6380d6a2->SetFillColor(0);
   61edc96ad38746c783a5b1ee6380d6a2->SetBorderMode(0);
   61edc96ad38746c783a5b1ee6380d6a2->SetBorderSize(2);
   61edc96ad38746c783a5b1ee6380d6a2->SetFrameBorderMode(0);
   61edc96ad38746c783a5b1ee6380d6a2->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,375697.9,1921.714);
   gre->SetPointError(0,1574.321,8.990738);
   gre->SetPoint(1,410920.3,1709.078);
   gre->SetPointError(1,1381.769,7.669514);
   gre->SetPoint(2,427912.8,1608.05);
   gre->SetPointError(2,1251.397,7.036209);
   gre->SetPoint(3,443421.2,1516.782);
   gre->SetPointError(3,1183.158,6.815669);
   gre->SetPoint(4,461147.9,1410.848);
   gre->SetPointError(4,1034.804,5.931751);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,292673.1,540797.4);
   Graph_Graph1->SetMinimum(1114.793);
   Graph_Graph1->SetMaximum(2455.093);
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
   
   
   TF1 *fG1 = new TF1("fG1","pol1",365317.7,470988.7);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(0.03037099);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,4162.575);
   fG1->SetParError(0,72.77567);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.005967855);
   fG1->SetParError(1,0.0001683052);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0861 +/- 0.0041");
   text = pt->AddText("g2 = 14.4347 +/- 0.7931");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(417015.5,1683.971,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(491634,1238.659,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(325192.3,2231.903,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",292673.1,540797.4);
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
   fDraw->SetParameter(0,4162.575);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.005967855);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   61edc96ad38746c783a5b1ee6380d6a2->Modified();
   61edc96ad38746c783a5b1ee6380d6a2->cd();
   61edc96ad38746c783a5b1ee6380d6a2->SetSelected(61edc96ad38746c783a5b1ee6380d6a2);
}
