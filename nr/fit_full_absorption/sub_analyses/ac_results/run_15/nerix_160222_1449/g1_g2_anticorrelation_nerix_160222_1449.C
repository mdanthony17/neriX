{
//=========Macro generated from canvas: 97b831e6a650478ab84c65b79201004c/
//=========  (Tue Feb 23 12:30:25 2016) by ROOT version5.34/26
   TCanvas *97b831e6a650478ab84c65b79201004c = new TCanvas("97b831e6a650478ab84c65b79201004c", "",10,45,704,527);
   97b831e6a650478ab84c65b79201004c->SetHighLightColor(2);
   97b831e6a650478ab84c65b79201004c->Range(265821.6,845.011,583674.2,2252.895);
   97b831e6a650478ab84c65b79201004c->SetFillColor(0);
   97b831e6a650478ab84c65b79201004c->SetBorderMode(0);
   97b831e6a650478ab84c65b79201004c->SetBorderSize(2);
   97b831e6a650478ab84c65b79201004c->SetFrameBorderMode(0);
   97b831e6a650478ab84c65b79201004c->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,380804.8,1678.269);
   gre->SetPointError(0,2333.5,10.19417);
   gre->SetPoint(1,424788.9,1465.347);
   gre->SetPointError(1,1971.783,8.38122);
   gre->SetPoint(2,441855,1355.24);
   gre->SetPointError(2,1894.009,8.237562);
   gre->SetPoint(3,462586.7,1267.948);
   gre->SetPointError(3,1799.46,7.207591);
   gre->SetPoint(4,486665,1174.65);
   gre->SetPointError(4,1953.354,7.058543);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,297606.9,551888.9);
   Graph_Graph1->SetMinimum(985.7994);
   Graph_Graph1->SetMaximum(2112.106);
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
   
   
   TF1 *fG1 = new TF1("fG1","pol1",367456.6,499633.1);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(5.498668);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,3506.104);
   fG1->SetParError(0,76.71401);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.004821971);
   fG1->SetParError(1,0.0001718089);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0940 +/- 0.0046");
   text = pt->AddText("g2 = 19.4940 +/- 1.1804");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(423712,1471.471,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(501717.2,1095.333,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(330674.3,1920.097,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",297606.9,551888.9);
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
   fDraw->SetParameter(0,3506.104);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.004821971);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   97b831e6a650478ab84c65b79201004c->Modified();
   97b831e6a650478ab84c65b79201004c->cd();
   97b831e6a650478ab84c65b79201004c->SetSelected(97b831e6a650478ab84c65b79201004c);
}
