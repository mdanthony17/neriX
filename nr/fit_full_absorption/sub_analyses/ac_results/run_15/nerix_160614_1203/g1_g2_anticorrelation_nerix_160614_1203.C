{
//=========Macro generated from canvas: eed62b0ad4c948179ea9650e3e2b4164/
//=========  (Wed Jun 15 14:57:52 2016) by ROOT version5.34/26
   TCanvas *eed62b0ad4c948179ea9650e3e2b4164 = new TCanvas("eed62b0ad4c948179ea9650e3e2b4164", "",10,45,704,527);
   eed62b0ad4c948179ea9650e3e2b4164->SetHighLightColor(2);
   eed62b0ad4c948179ea9650e3e2b4164->Range(266047.4,876.1917,581406.4,2425.879);
   eed62b0ad4c948179ea9650e3e2b4164->SetFillColor(0);
   eed62b0ad4c948179ea9650e3e2b4164->SetBorderMode(0);
   eed62b0ad4c948179ea9650e3e2b4164->SetBorderSize(2);
   eed62b0ad4c948179ea9650e3e2b4164->SetFrameBorderMode(0);
   eed62b0ad4c948179ea9650e3e2b4164->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,372197.7,1811.674);
   gre->SetPointError(0,1537.57,9.102007);
   gre->SetPoint(1,405764.5,1668.1);
   gre->SetPointError(1,1401.057,8.389078);
   gre->SetPoint(2,425414.6,1550.98);
   gre->SetPointError(2,1204.471,8.389799);
   gre->SetPoint(3,441136.5,1445.217);
   gre->SetPointError(3,1187.429,6.779134);
   gre->SetPoint(4,458459.8,1359.764);
   gre->SetPointError(4,1112.892,6.517752);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,297583.3,549870.5);
   Graph_Graph1->SetMinimum(1031.16);
   Graph_Graph1->SetMaximum(2270.91);
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
   
   
   TF1 *fG1 = new TF1("fG1","pol1",361768.9,468463.9);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(7.594391);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,3850.296);
   fG1->SetParError(0,71.29991);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.005429058);
   fG1->SetParError(1,0.0001664273);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0797 +/- 0.0038");
   text = pt->AddText("g2 = 14.6768 +/- 0.8303");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(424011.9,1557.639,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(499882.3,1145.734,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(330648.1,2064.464,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",297583.3,549870.5);
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
   fDraw->SetParameter(0,3850.296);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.005429058);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   eed62b0ad4c948179ea9650e3e2b4164->Modified();
   eed62b0ad4c948179ea9650e3e2b4164->cd();
   eed62b0ad4c948179ea9650e3e2b4164->SetSelected(eed62b0ad4c948179ea9650e3e2b4164);
}
