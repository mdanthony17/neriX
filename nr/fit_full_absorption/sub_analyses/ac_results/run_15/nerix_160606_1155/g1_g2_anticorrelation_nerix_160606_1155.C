{
//=========Macro generated from canvas: e5a7b2dd799e4c569d1d221804d5dbf5/
//=========  (Mon Jun  6 15:18:26 2016) by ROOT version5.34/26
   TCanvas *e5a7b2dd799e4c569d1d221804d5dbf5 = new TCanvas("e5a7b2dd799e4c569d1d221804d5dbf5", "",10,45,704,527);
   e5a7b2dd799e4c569d1d221804d5dbf5->SetHighLightColor(2);
   e5a7b2dd799e4c569d1d221804d5dbf5->Range(268336.7,878.2754,586409.3,2431.648);
   e5a7b2dd799e4c569d1d221804d5dbf5->SetFillColor(0);
   e5a7b2dd799e4c569d1d221804d5dbf5->SetBorderMode(0);
   e5a7b2dd799e4c569d1d221804d5dbf5->SetBorderSize(2);
   e5a7b2dd799e4c569d1d221804d5dbf5->SetFrameBorderMode(0);
   e5a7b2dd799e4c569d1d221804d5dbf5->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,381108.1,1788.436);
   gre->SetPointError(0,1988.71,11.99292);
   gre->SetPoint(1,411894.9,1648.204);
   gre->SetPointError(1,1341.369,8.760038);
   gre->SetPoint(2,429549.6,1538.145);
   gre->SetPointError(2,1290.66,7.748298);
   gre->SetPoint(3,443190.3,1473.227);
   gre->SetPointError(3,1169.419,6.849563);
   gre->SetPoint(4,463499,1354.054);
   gre->SetPointError(4,1171.349,6.717709);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,300144,554602.1);
   Graph_Graph1->SetMinimum(1033.613);
   Graph_Graph1->SetMaximum(2276.311);
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
   
   
   TF1 *fG1 = new TF1("fG1","pol1",370564.3,473225.4);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(2.460393);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,3859.452);
   fG1->SetParError(0,82.54643);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.00539554);
   fG1->SetParError(1,0.0001895091);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0799 +/- 0.0039");
   text = pt->AddText("g2 = 14.8031 +/- 0.8893");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(427660.5,1561.343,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(504183.7,1148.459,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(333493.3,2069.373,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",300144,554602.1);
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
   fDraw->SetParameter(0,3859.452);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.00539554);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   e5a7b2dd799e4c569d1d221804d5dbf5->Modified();
   e5a7b2dd799e4c569d1d221804d5dbf5->cd();
   e5a7b2dd799e4c569d1d221804d5dbf5->SetSelected(e5a7b2dd799e4c569d1d221804d5dbf5);
}
