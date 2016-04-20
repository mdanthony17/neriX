{
//=========Macro generated from canvas: a016829d0c6b4e7db673d2f89bae7cc6/
//=========  (Tue Apr 12 09:46:59 2016) by ROOT version5.34/26
   TCanvas *a016829d0c6b4e7db673d2f89bae7cc6 = new TCanvas("a016829d0c6b4e7db673d2f89bae7cc6", "",10,45,704,527);
   a016829d0c6b4e7db673d2f89bae7cc6->SetHighLightColor(2);
   a016829d0c6b4e7db673d2f89bae7cc6->Range(181985.7,727.5761,399592.5,1939.8);
   a016829d0c6b4e7db673d2f89bae7cc6->SetFillColor(0);
   a016829d0c6b4e7db673d2f89bae7cc6->SetBorderMode(0);
   a016829d0c6b4e7db673d2f89bae7cc6->SetBorderSize(2);
   a016829d0c6b4e7db673d2f89bae7cc6->SetFrameBorderMode(0);
   a016829d0c6b4e7db673d2f89bae7cc6->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,254856.8,1464.483);
   gre->SetPointError(0,1547.27,8.75346);
   gre->SetPoint(1,288298.5,1285.676);
   gre->SetPointError(1,1283.364,6.947031);
   gre->SetPoint(2,300413.7,1200.488);
   gre->SetPointError(2,1142.403,6.607851);
   gre->SetPoint(3,309068.8,1129.907);
   gre->SetPointError(3,1126.95,6.261545);
   gre->SetPoint(4,327242.7,1037.669);
   gre->SetPointError(4,1080.514,5.619828);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,203746.4,377831.9);
   Graph_Graph1->SetMinimum(848.7984);
   Graph_Graph1->SetMaximum(1818.577);
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
   
   
   TF1 *fG1 = new TF1("fG1","pol1",245808.2,335824.5);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(5.361332);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,3018.845);
   fG1->SetParError(0,60.77776);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.006064483);
   fG1->SetParError(1,0.0002008412);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0809 +/- 0.0039");
   text = pt->AddText("g2 = 13.3459 +/- 0.7805");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(290079.9,1266.975,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(343483.5,943.1094,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(226384.8,1653.252,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",203746.4,377831.9);
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
   fDraw->SetParameter(0,3018.845);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.006064483);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   a016829d0c6b4e7db673d2f89bae7cc6->Modified();
   a016829d0c6b4e7db673d2f89bae7cc6->cd();
   a016829d0c6b4e7db673d2f89bae7cc6->SetSelected(a016829d0c6b4e7db673d2f89bae7cc6);
}
