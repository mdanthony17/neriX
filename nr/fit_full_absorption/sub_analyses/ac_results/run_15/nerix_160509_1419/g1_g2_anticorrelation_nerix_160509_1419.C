{
//=========Macro generated from canvas: b2920c0b3dee458fae9e97bfc2e4041f/
//=========  (Wed May 11 11:46:45 2016) by ROOT version5.34/26
   TCanvas *b2920c0b3dee458fae9e97bfc2e4041f = new TCanvas("b2920c0b3dee458fae9e97bfc2e4041f", "",10,45,704,527);
   b2920c0b3dee458fae9e97bfc2e4041f->SetHighLightColor(2);
   b2920c0b3dee458fae9e97bfc2e4041f->Range(201070.6,649.0261,441498.1,1730.377);
   b2920c0b3dee458fae9e97bfc2e4041f->SetFillColor(0);
   b2920c0b3dee458fae9e97bfc2e4041f->SetBorderMode(0);
   b2920c0b3dee458fae9e97bfc2e4041f->SetBorderSize(2);
   b2920c0b3dee458fae9e97bfc2e4041f->SetFrameBorderMode(0);
   b2920c0b3dee458fae9e97bfc2e4041f->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,262792.6,1422.042);
   gre->SetPointError(0,1548.266,7.717045);
   gre->SetPoint(1,286792.8,1276.644);
   gre->SetPointError(1,1557.087,8.592314);
   gre->SetPoint(2,307649.8,1160.267);
   gre->SetPointError(2,1342.933,6.941894);
   gre->SetPoint(3,323759.2,1118.494);
   gre->SetPointError(3,1124.526,6.723526);
   gre->SetPoint(4,338123.4,1043.379);
   gre->SetPointError(4,1091.991,5.529281);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,225113.4,417455.4);
   Graph_Graph1->SetMinimum(757.1611);
   Graph_Graph1->SetMaximum(1622.242);
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
   
   
   TF1 *fG1 = new TF1("fG1","pol1",253447.2,347012.5);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(12.96728);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,2692.927);
   fG1->SetParError(0,50.19746);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.004896277);
   fG1->SetParError(1,0.0001608737);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0722 +/- 0.0034");
   text = pt->AddText("g2 = 14.7455 +/- 0.8528");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(320500.8,1130.19,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(379504.9,841.2901,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(250126,1474.765,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",225113.4,417455.4);
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
   fDraw->SetParameter(0,2692.927);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.004896277);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   b2920c0b3dee458fae9e97bfc2e4041f->Modified();
   b2920c0b3dee458fae9e97bfc2e4041f->cd();
   b2920c0b3dee458fae9e97bfc2e4041f->SetSelected(b2920c0b3dee458fae9e97bfc2e4041f);
}
