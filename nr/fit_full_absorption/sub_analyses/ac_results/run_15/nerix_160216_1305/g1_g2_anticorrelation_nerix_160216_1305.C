{
//=========Macro generated from canvas: c6eab6bff7ab408caff11561b5325ef2/
//=========  (Wed Feb 17 15:26:31 2016) by ROOT version5.34/26
   TCanvas *c6eab6bff7ab408caff11561b5325ef2 = new TCanvas("c6eab6bff7ab408caff11561b5325ef2", "",10,45,704,527);
   c6eab6bff7ab408caff11561b5325ef2->SetHighLightColor(2);
   c6eab6bff7ab408caff11561b5325ef2->Range(378584.8,987.0051,827339.9,2732.684);
   c6eab6bff7ab408caff11561b5325ef2->SetFillColor(0);
   c6eab6bff7ab408caff11561b5325ef2->SetBorderMode(0);
   c6eab6bff7ab408caff11561b5325ef2->SetBorderSize(2);
   c6eab6bff7ab408caff11561b5325ef2->SetFrameBorderMode(0);
   c6eab6bff7ab408caff11561b5325ef2->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,533042,2066.289);
   gre->SetPointError(0,2098.213,9.265924);
   gre->SetPoint(1,573334.3,1858.433);
   gre->SetPointError(1,1826.644,8.045276);
   gre->SetPoint(2,600882.5,1747.801);
   gre->SetPointError(2,1915.446,8.325206);
   gre->SetPoint(3,623860.4,1647.115);
   gre->SetPointError(3,1582.499,6.500216);
   gre->SetPoint(4,654202.4,1536.368);
   gre->SetPointError(4,1400.462,5.936071);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,423460.3,782464.4);
   Graph_Graph1->SetMinimum(1161.573);
   Graph_Graph1->SetMaximum(2558.116);
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
   
   
   TF1 *fG1 = new TF1("fG1","pol1",518477.9,668068.8);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(6.967778);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,4337.249);
   fG1->SetParError(0,69.72363);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.004297744);
   fG1->SetParError(1,0.0001142145);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0898 +/- 0.0042");
   text = pt->AddText("g2 = 20.8851 +/- 1.1213");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(603367.9,1754.636,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(711331.2,1290.637,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(470511.5,2325.56,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",423460.3,782464.4);
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
   fDraw->SetParameter(0,4337.249);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.004297744);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   c6eab6bff7ab408caff11561b5325ef2->Modified();
   c6eab6bff7ab408caff11561b5325ef2->cd();
   c6eab6bff7ab408caff11561b5325ef2->SetSelected(c6eab6bff7ab408caff11561b5325ef2);
}
