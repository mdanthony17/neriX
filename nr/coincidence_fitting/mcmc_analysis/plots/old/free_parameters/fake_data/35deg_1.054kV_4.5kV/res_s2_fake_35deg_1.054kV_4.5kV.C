{
//=========Macro generated from canvas: d2ca9bf0cc5146639278c2550dfb245e/
//=========  (Tue Feb 23 11:13:19 2016) by ROOT version5.34/26
   TCanvas *d2ca9bf0cc5146639278c2550dfb245e = new TCanvas("d2ca9bf0cc5146639278c2550dfb245e", "",71,123,704,527);
   d2ca9bf0cc5146639278c2550dfb245e->SetHighLightColor(2);
   d2ca9bf0cc5146639278c2550dfb245e->Range(-0.25,-173.8935,2.25,1565.041);
   d2ca9bf0cc5146639278c2550dfb245e->SetFillColor(0);
   d2ca9bf0cc5146639278c2550dfb245e->SetBorderMode(0);
   d2ca9bf0cc5146639278c2550dfb245e->SetBorderSize(2);
   d2ca9bf0cc5146639278c2550dfb245e->SetFrameBorderMode(0);
   d2ca9bf0cc5146639278c2550dfb245e->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,323);
   h_res_s2->SetBinContent(2,457);
   h_res_s2->SetBinContent(3,592);
   h_res_s2->SetBinContent(4,739);
   h_res_s2->SetBinContent(5,837);
   h_res_s2->SetBinContent(6,1074);
   h_res_s2->SetBinContent(7,1170);
   h_res_s2->SetBinContent(8,1289);
   h_res_s2->SetBinContent(9,1143);
   h_res_s2->SetBinContent(10,999);
   h_res_s2->SetBinContent(11,841);
   h_res_s2->SetBinContent(12,505);
   h_res_s2->SetBinContent(13,186);
   h_res_s2->SetBinContent(14,70);
   h_res_s2->SetBinContent(15,5);
   h_res_s2->SetBinContent(16,10);
   h_res_s2->SetBinError(1,17.9722);
   h_res_s2->SetBinError(2,21.37756);
   h_res_s2->SetBinError(3,24.33105);
   h_res_s2->SetBinError(4,27.18455);
   h_res_s2->SetBinError(5,28.93095);
   h_res_s2->SetBinError(6,32.77194);
   h_res_s2->SetBinError(7,34.20526);
   h_res_s2->SetBinError(8,35.90265);
   h_res_s2->SetBinError(9,33.80828);
   h_res_s2->SetBinError(10,31.60696);
   h_res_s2->SetBinError(11,29);
   h_res_s2->SetBinError(12,22.47221);
   h_res_s2->SetBinError(13,13.63818);
   h_res_s2->SetBinError(14,8.3666);
   h_res_s2->SetBinError(15,2.236068);
   h_res_s2->SetBinError(16,3.162278);
   h_res_s2->SetEntries(10240);
   h_res_s2->SetStats(0);
   h_res_s2->SetFillStyle(0);
   h_res_s2->SetMarkerStyle(20);
   h_res_s2->SetMarkerSize(0);
   h_res_s2->GetXaxis()->SetLabelFont(42);
   h_res_s2->GetXaxis()->SetLabelSize(0.035);
   h_res_s2->GetXaxis()->SetTitleSize(0.035);
   h_res_s2->GetXaxis()->SetTitleFont(42);
   h_res_s2->GetYaxis()->SetLabelFont(42);
   h_res_s2->GetYaxis()->SetLabelSize(0.035);
   h_res_s2->GetYaxis()->SetTitleSize(0.035);
   h_res_s2->GetYaxis()->SetTitleFont(42);
   h_res_s2->GetZaxis()->SetLabelFont(42);
   h_res_s2->GetZaxis()->SetLabelSize(0.035);
   h_res_s2->GetZaxis()->SetTitleSize(0.035);
   h_res_s2->GetZaxis()->SetTitleFont(42);
   h_res_s2->Draw("");
   
   TPaveText *pt = new TPaveText(0.4335714,0.94,0.5664286,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("res_s2");
   pt->Draw();
   TLine *line = new TLine(0.06693261,0,0.06693261,1289);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1387692,0,0.1387692,1289);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1997766,0,0.1997766,1289);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1,0,0.1,1289);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.14^{-0.07}_{+0.06} ");
   pt->Draw();
   d2ca9bf0cc5146639278c2550dfb245e->Modified();
   d2ca9bf0cc5146639278c2550dfb245e->cd();
   d2ca9bf0cc5146639278c2550dfb245e->SetSelected(d2ca9bf0cc5146639278c2550dfb245e);
}
