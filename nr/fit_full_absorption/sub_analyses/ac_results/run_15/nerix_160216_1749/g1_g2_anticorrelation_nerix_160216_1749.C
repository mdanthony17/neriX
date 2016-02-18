{
//=========Macro generated from canvas: bc7ce65422994955996518e4cd7c35bc/
//=========  (Wed Feb 17 15:17:40 2016) by ROOT version5.34/26
   TCanvas *bc7ce65422994955996518e4cd7c35bc = new TCanvas("bc7ce65422994955996518e4cd7c35bc", "",10,45,704,527);
   bc7ce65422994955996518e4cd7c35bc->SetHighLightColor(2);
   bc7ce65422994955996518e4cd7c35bc->Range(70099.95,336.6825,187730.1,838.9302);
   bc7ce65422994955996518e4cd7c35bc->SetFillColor(0);
   bc7ce65422994955996518e4cd7c35bc->SetBorderMode(0);
   bc7ce65422994955996518e4cd7c35bc->SetBorderSize(2);
   bc7ce65422994955996518e4cd7c35bc->SetFrameBorderMode(0);
   bc7ce65422994955996518e4cd7c35bc->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,96774.38,692.7788);
   gre->SetPointError(0,661.2845,2.813661);
   gre->SetPoint(1,117384.9,613.466);
   gre->SetPointError(1,678.0438,2.908215);
   gre->SetPoint(2,126584.3,579.7675);
   gre->SetPointError(2,690.811,2.811511);
   gre->SetPoint(3,133269.7,528.7656);
   gre->SetPointError(3,730.9075,2.660686);
   gre->SetPoint(4,147010.3,483.9205);
   gre->SetPointError(4,645.312,2.452823);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,81862.96,175967.1);
   Graph_Graph1->SetMinimum(386.9073);
   Graph_Graph1->SetMaximum(788.7054);
   Graph_Graph1->SetDirectory(0);
   Graph_Graph1->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph1->SetLineColor(ci);
   Graph_Graph1->GetXaxis()->SetTitle("S2 at 163 keV Peak");
   Graph_Graph1->GetXaxis()->SetLabelFont(42);
   Graph_Graph1->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleOffset(1.12);
   Graph_Graph1->GetXaxis()->SetTitleFont(42);
   Graph_Graph1->GetYaxis()->SetTitle("S1 at 163 keV Peak [PE]");
   Graph_Graph1->GetYaxis()->SetLabelFont(42);
   Graph_Graph1->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleFont(42);
   Graph_Graph1->GetZaxis()->SetLabelFont(42);
   Graph_Graph1->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1);
   
   
   TF1 *fG1 = new TF1("fG1","pol1",90958.85,152809.8);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(17.68561);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,1109.645);
   fG1->SetParError(0,13.07947);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.004266022);
   fG1->SetParError(1,0.0001036776);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0928 +/- 0.0042");
   text = pt->AddText("g2 = 21.7421 +/- 1.1187");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(133402.7,543.234,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(159970.1,429.897,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(105306,663.0949,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",81862.96,175967.1);
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
   fDraw->SetParameter(0,1109.645);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.004266022);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   bc7ce65422994955996518e4cd7c35bc->Modified();
   bc7ce65422994955996518e4cd7c35bc->cd();
   bc7ce65422994955996518e4cd7c35bc->SetSelected(bc7ce65422994955996518e4cd7c35bc);
}
