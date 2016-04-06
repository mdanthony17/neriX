{
//=========Macro generated from canvas: 49090a7b320b4bd8a233e6599927aea8/
//=========  (Thu Mar 31 13:43:38 2016) by ROOT version5.34/26
   TCanvas *49090a7b320b4bd8a233e6599927aea8 = new TCanvas("49090a7b320b4bd8a233e6599927aea8", "",10,45,704,527);
   49090a7b320b4bd8a233e6599927aea8->SetHighLightColor(2);
   49090a7b320b4bd8a233e6599927aea8->Range(188869.6,728.9789,414707.8,1943.54);
   49090a7b320b4bd8a233e6599927aea8->SetFillColor(0);
   49090a7b320b4bd8a233e6599927aea8->SetBorderMode(0);
   49090a7b320b4bd8a233e6599927aea8->SetBorderSize(2);
   49090a7b320b4bd8a233e6599927aea8->SetFrameBorderMode(0);
   49090a7b320b4bd8a233e6599927aea8->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,258182.2,1503.932);
   gre->SetPointError(0,1503.234,8.625697);
   gre->SetPoint(1,290853.4,1320.66);
   gre->SetPointError(1,1461.862,8.32446);
   gre->SetPoint(2,308606.5,1235.442);
   gre->SetPointError(2,1387.587,7.482783);
   gre->SetPoint(3,321412.8,1148.114);
   gre->SetPointError(3,1200.978,6.736373);
   gre->SetPoint(4,334854.3,1054.45);
   gre->SetPointError(4,1172.047,5.219162);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,211453.4,392124);
   Graph_Graph1->SetMinimum(850.435);
   Graph_Graph1->SetMaximum(1822.084);
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
   
   
   TF1 *fG1 = new TF1("fG1","pol1",248744.3,343961.1);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(4.658404);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,3024.666);
   fG1->SetParError(0,56.14531);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.005854711);
   fG1->SetParError(1,0.000180847);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0811 +/- 0.0039");
   text = pt->AddText("g2 = 13.8507 +/- 0.7856");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(301052.7,1269.417,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(356476.3,944.9277,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(234948.2,1656.44,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",211453.4,392124);
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
   fDraw->SetParameter(0,3024.666);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.005854711);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   49090a7b320b4bd8a233e6599927aea8->Modified();
   49090a7b320b4bd8a233e6599927aea8->cd();
   49090a7b320b4bd8a233e6599927aea8->SetSelected(49090a7b320b4bd8a233e6599927aea8);
}
