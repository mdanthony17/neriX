{
//=========Macro generated from canvas: Canvas_pk8whpjmGnPWV7FvzGL8vV/
//=========  (Mon Feb 29 14:02:37 2016) by ROOT version5.34/28
   TCanvas *Canvas_pk8whpjmGnPWV7FvzGL8vV = new TCanvas("Canvas_pk8whpjmGnPWV7FvzGL8vV", "",0,0,700,500);
   Canvas_pk8whpjmGnPWV7FvzGL8vV->SetHighLightColor(2);
   Canvas_pk8whpjmGnPWV7FvzGL8vV->Range(-0.25,-132.6004,2.25,1193.404);
   Canvas_pk8whpjmGnPWV7FvzGL8vV->SetFillColor(0);
   Canvas_pk8whpjmGnPWV7FvzGL8vV->SetBorderMode(0);
   Canvas_pk8whpjmGnPWV7FvzGL8vV->SetBorderSize(2);
   Canvas_pk8whpjmGnPWV7FvzGL8vV->SetFrameBorderMode(0);
   Canvas_pk8whpjmGnPWV7FvzGL8vV->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(1,189);
   h_res_s1->SetBinContent(2,208);
   h_res_s1->SetBinContent(3,336);
   h_res_s1->SetBinContent(4,316);
   h_res_s1->SetBinContent(5,377);
   h_res_s1->SetBinContent(6,378);
   h_res_s1->SetBinContent(7,539);
   h_res_s1->SetBinContent(8,802);
   h_res_s1->SetBinContent(9,703);
   h_res_s1->SetBinContent(10,923);
   h_res_s1->SetBinContent(11,889);
   h_res_s1->SetBinContent(12,979);
   h_res_s1->SetBinContent(13,903);
   h_res_s1->SetBinContent(14,798);
   h_res_s1->SetBinContent(15,637);
   h_res_s1->SetBinContent(16,429);
   h_res_s1->SetBinContent(17,311);
   h_res_s1->SetBinContent(18,185);
   h_res_s1->SetBinContent(19,135);
   h_res_s1->SetBinContent(20,93);
   h_res_s1->SetBinContent(21,48);
   h_res_s1->SetBinContent(22,27);
   h_res_s1->SetBinContent(23,15);
   h_res_s1->SetBinContent(24,10);
   h_res_s1->SetBinContent(25,10);
   h_res_s1->SetBinError(1,13.74773);
   h_res_s1->SetBinError(2,14.42221);
   h_res_s1->SetBinError(3,18.3303);
   h_res_s1->SetBinError(4,17.77639);
   h_res_s1->SetBinError(5,19.41649);
   h_res_s1->SetBinError(6,19.44222);
   h_res_s1->SetBinError(7,23.21637);
   h_res_s1->SetBinError(8,28.3196);
   h_res_s1->SetBinError(9,26.51415);
   h_res_s1->SetBinError(10,30.38092);
   h_res_s1->SetBinError(11,29.8161);
   h_res_s1->SetBinError(12,31.28898);
   h_res_s1->SetBinError(13,30.04996);
   h_res_s1->SetBinError(14,28.24889);
   h_res_s1->SetBinError(15,25.23886);
   h_res_s1->SetBinError(16,20.71232);
   h_res_s1->SetBinError(17,17.63519);
   h_res_s1->SetBinError(18,13.60147);
   h_res_s1->SetBinError(19,11.61895);
   h_res_s1->SetBinError(20,9.643651);
   h_res_s1->SetBinError(21,6.928203);
   h_res_s1->SetBinError(22,5.196152);
   h_res_s1->SetBinError(23,3.872983);
   h_res_s1->SetBinError(24,3.162278);
   h_res_s1->SetBinError(25,3.162278);
   h_res_s1->SetEntries(10240);
   h_res_s1->SetStats(0);
   h_res_s1->SetFillColor(1);
   h_res_s1->SetFillStyle(0);
   h_res_s1->SetMarkerStyle(20);
   h_res_s1->SetMarkerSize(0);
   h_res_s1->GetXaxis()->SetLabelFont(42);
   h_res_s1->GetXaxis()->SetLabelSize(0.035);
   h_res_s1->GetXaxis()->SetTitleSize(0.035);
   h_res_s1->GetXaxis()->SetTitleFont(42);
   h_res_s1->GetYaxis()->SetLabelFont(42);
   h_res_s1->GetYaxis()->SetLabelSize(0.035);
   h_res_s1->GetYaxis()->SetTitleSize(0.035);
   h_res_s1->GetYaxis()->SetTitleFont(42);
   h_res_s1->GetZaxis()->SetLabelFont(42);
   h_res_s1->GetZaxis()->SetLabelSize(0.035);
   h_res_s1->GetZaxis()->SetTitleSize(0.035);
   h_res_s1->GetZaxis()->SetTitleFont(42);
   h_res_s1->Draw("");
   
   TPaveText *pt = new TPaveText(0.4342857,0.935,0.5657143,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("res_s1");
   pt->Draw();
   TLine *line = new TLine(0.1106963,0,0.1106963,979);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2078515,0,0.2078515,979);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.288535,0,0.288535,979);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3,0,0.3,979);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.21^{-0.10}_{+0.08} ");
   pt->Draw();
   Canvas_pk8whpjmGnPWV7FvzGL8vV->Modified();
   Canvas_pk8whpjmGnPWV7FvzGL8vV->cd();
   Canvas_pk8whpjmGnPWV7FvzGL8vV->SetSelected(Canvas_pk8whpjmGnPWV7FvzGL8vV);
}
