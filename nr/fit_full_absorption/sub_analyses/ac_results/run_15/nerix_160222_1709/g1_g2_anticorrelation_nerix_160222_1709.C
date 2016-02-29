{
//=========Macro generated from canvas: c0bc6073e6d1475f962e34c6fbfc118a/
//=========  (Tue Feb 23 12:31:06 2016) by ROOT version5.34/26
   TCanvas *c0bc6073e6d1475f962e34c6fbfc118a = new TCanvas("c0bc6073e6d1475f962e34c6fbfc118a", "",10,45,704,527);
   c0bc6073e6d1475f962e34c6fbfc118a->SetHighLightColor(2);
   c0bc6073e6d1475f962e34c6fbfc118a->Range(74148.39,344.7485,181807.4,819.4196);
   c0bc6073e6d1475f962e34c6fbfc118a->SetFillColor(0);
   c0bc6073e6d1475f962e34c6fbfc118a->SetBorderMode(0);
   c0bc6073e6d1475f962e34c6fbfc118a->SetBorderSize(2);
   c0bc6073e6d1475f962e34c6fbfc118a->SetFrameBorderMode(0);
   c0bc6073e6d1475f962e34c6fbfc118a->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,99605.72,678.7517);
   gre->SetPointError(0,647.3869,2.886872);
   gre->SetPoint(1,117021.3,608.8226);
   gre->SetPointError(1,658.6791,3.162492);
   gre->SetPoint(2,127967.9,559.709);
   gre->SetPointError(2,665.4511,2.617862);
   gre->SetPoint(3,133921,525.1698);
   gre->SetPointError(3,541.9853,2.299371);
   gre->SetPoint(4,144396.9,482.6062);
   gre->SetPointError(4,652.5166,2.33132);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,84914.3,171041.5);
   Graph_Graph1->SetMinimum(392.2156);
   Graph_Graph1->SetMaximum(771.9524);
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
   
   
   TF1 *fG1 = new TF1("fG1","pol1",94349.22,149658.5);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(4.134164);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,1124.869);
   fG1->SetParError(0,14.59254);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.004449086);
   fG1->SetParError(1,0.0001147533);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0940 +/- 0.0043");
   text = pt->AddText("g2 = 21.1336 +/- 1.1086");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(129668.6,550.6871,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(155492.3,435.7951,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(102358.4,672.1924,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",84914.3,171041.5);
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
   fDraw->SetParameter(0,1124.869);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.004449086);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   c0bc6073e6d1475f962e34c6fbfc118a->Modified();
   c0bc6073e6d1475f962e34c6fbfc118a->cd();
   c0bc6073e6d1475f962e34c6fbfc118a->SetSelected(c0bc6073e6d1475f962e34c6fbfc118a);
}
