{
//=========Macro generated from canvas: dd97a6e623874b6da240f2afe5abb81a/
//=========  (Tue Apr  5 15:19:50 2016) by ROOT version5.34/26
   TCanvas *dd97a6e623874b6da240f2afe5abb81a = new TCanvas("dd97a6e623874b6da240f2afe5abb81a", "",10,45,704,527);
   dd97a6e623874b6da240f2afe5abb81a->SetHighLightColor(2);
   dd97a6e623874b6da240f2afe5abb81a->Range(192042.1,725.8653,421673.8,1935.239);
   dd97a6e623874b6da240f2afe5abb81a->SetFillColor(0);
   dd97a6e623874b6da240f2afe5abb81a->SetBorderMode(0);
   dd97a6e623874b6da240f2afe5abb81a->SetBorderSize(2);
   dd97a6e623874b6da240f2afe5abb81a->SetFrameBorderMode(0);
   dd97a6e623874b6da240f2afe5abb81a->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,265723.4,1490.932);
   gre->SetPointError(0,1389.422,7.941182);
   gre->SetPoint(1,291527.3,1334.869);
   gre->SetPointError(1,1228.699,6.972125);
   gre->SetPoint(2,312402.6,1234.831);
   gre->SetPointError(2,1008.491,6.029742);
   gre->SetPoint(3,320155.6,1149.4);
   gre->SetPointError(3,1269.251,6.226976);
   gre->SetPoint(4,338859.7,1075.865);
   gre->SetPointError(4,981.6717,5.000491);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,215005.3,398710.7);
   Graph_Graph1->SetMinimum(846.8027);
   Graph_Graph1->SetMaximum(1814.301);
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
   
   
   TF1 *fG1 = new TF1("fG1","pol1",256783.3,347392.1);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(11.9111);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,3011.747);
   fG1->SetParError(0,54.83735);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.005733399);
   fG1->SetParError(1,0.0001752684);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0807 +/- 0.0038");
   text = pt->AddText("g2 = 14.0834 +/- 0.7947");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(306109.6,1263.996,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(362464.2,940.8919,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(238894.8,1649.365,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",215005.3,398710.7);
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
   fDraw->SetParameter(0,3011.747);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.005733399);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   dd97a6e623874b6da240f2afe5abb81a->Modified();
   dd97a6e623874b6da240f2afe5abb81a->cd();
   dd97a6e623874b6da240f2afe5abb81a->SetSelected(dd97a6e623874b6da240f2afe5abb81a);
}
