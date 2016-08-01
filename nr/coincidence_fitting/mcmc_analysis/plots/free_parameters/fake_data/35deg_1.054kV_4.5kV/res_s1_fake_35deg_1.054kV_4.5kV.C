{
//=========Macro generated from canvas: 72bfaa30b7944ab486adba85ae34a583/
//=========  (Tue Feb 23 11:13:20 2016) by ROOT version5.34/26
   TCanvas *72bfaa30b7944ab486adba85ae34a583 = new TCanvas("72bfaa30b7944ab486adba85ae34a583", "",531,47,704,527);
   72bfaa30b7944ab486adba85ae34a583->SetHighLightColor(2);
   72bfaa30b7944ab486adba85ae34a583->Range(-0.25,-160.7152,2.25,1446.436);
   72bfaa30b7944ab486adba85ae34a583->SetFillColor(0);
   72bfaa30b7944ab486adba85ae34a583->SetBorderMode(0);
   72bfaa30b7944ab486adba85ae34a583->SetBorderSize(2);
   72bfaa30b7944ab486adba85ae34a583->SetFrameBorderMode(0);
   72bfaa30b7944ab486adba85ae34a583->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(1,12);
   h_res_s1->SetBinContent(2,20);
   h_res_s1->SetBinContent(3,35);
   h_res_s1->SetBinContent(4,47);
   h_res_s1->SetBinContent(5,86);
   h_res_s1->SetBinContent(6,90);
   h_res_s1->SetBinContent(7,178);
   h_res_s1->SetBinContent(8,281);
   h_res_s1->SetBinContent(9,317);
   h_res_s1->SetBinContent(10,478);
   h_res_s1->SetBinContent(11,698);
   h_res_s1->SetBinContent(12,1118);
   h_res_s1->SetBinContent(13,1062);
   h_res_s1->SetBinContent(14,1190);
   h_res_s1->SetBinContent(15,1139);
   h_res_s1->SetBinContent(16,1056);
   h_res_s1->SetBinContent(17,805);
   h_res_s1->SetBinContent(18,686);
   h_res_s1->SetBinContent(19,381);
   h_res_s1->SetBinContent(20,222);
   h_res_s1->SetBinContent(21,123);
   h_res_s1->SetBinContent(22,76);
   h_res_s1->SetBinContent(23,80);
   h_res_s1->SetBinContent(24,35);
   h_res_s1->SetBinContent(25,15);
   h_res_s1->SetBinContent(27,5);
   h_res_s1->SetBinContent(28,5);
   h_res_s1->SetBinError(1,3.464102);
   h_res_s1->SetBinError(2,4.472136);
   h_res_s1->SetBinError(3,5.91608);
   h_res_s1->SetBinError(4,6.855655);
   h_res_s1->SetBinError(5,9.273618);
   h_res_s1->SetBinError(6,9.486833);
   h_res_s1->SetBinError(7,13.34166);
   h_res_s1->SetBinError(8,16.76305);
   h_res_s1->SetBinError(9,17.80449);
   h_res_s1->SetBinError(10,21.86321);
   h_res_s1->SetBinError(11,26.41969);
   h_res_s1->SetBinError(12,33.43651);
   h_res_s1->SetBinError(13,32.58834);
   h_res_s1->SetBinError(14,34.49638);
   h_res_s1->SetBinError(15,33.74907);
   h_res_s1->SetBinError(16,32.49615);
   h_res_s1->SetBinError(17,28.37252);
   h_res_s1->SetBinError(18,26.1916);
   h_res_s1->SetBinError(19,19.51922);
   h_res_s1->SetBinError(20,14.89966);
   h_res_s1->SetBinError(21,11.09054);
   h_res_s1->SetBinError(22,8.717798);
   h_res_s1->SetBinError(23,8.944272);
   h_res_s1->SetBinError(24,5.91608);
   h_res_s1->SetBinError(25,3.872983);
   h_res_s1->SetBinError(27,2.236068);
   h_res_s1->SetBinError(28,2.236068);
   h_res_s1->SetEntries(10240);
   h_res_s1->SetStats(0);
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
   
   TPaveText *pt = new TPaveText(0.4335714,0.94,0.5664286,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("res_s1");
   pt->Draw();
   TLine *line = new TLine(0.2024115,0,0.2024115,1190);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2717311,0,0.2717311,1190);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.339996,0,0.339996,1190);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3,0,0.3,1190);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.27^{-0.07}_{+0.07} ");
   pt->Draw();
   72bfaa30b7944ab486adba85ae34a583->Modified();
   72bfaa30b7944ab486adba85ae34a583->cd();
   72bfaa30b7944ab486adba85ae34a583->SetSelected(72bfaa30b7944ab486adba85ae34a583);
}
