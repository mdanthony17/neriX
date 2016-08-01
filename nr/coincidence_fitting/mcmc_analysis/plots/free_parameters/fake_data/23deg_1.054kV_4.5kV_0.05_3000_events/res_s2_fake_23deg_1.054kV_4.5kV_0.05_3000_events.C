{
//=========Macro generated from canvas: Canvas_XTPSTkSx4GGUTjq2VK2Z2E/
//=========  (Mon Feb 29 14:02:37 2016) by ROOT version5.34/28
   TCanvas *Canvas_XTPSTkSx4GGUTjq2VK2Z2E = new TCanvas("Canvas_XTPSTkSx4GGUTjq2VK2Z2E", "",0,0,700,500);
   Canvas_XTPSTkSx4GGUTjq2VK2Z2E->SetHighLightColor(2);
   Canvas_XTPSTkSx4GGUTjq2VK2Z2E->Range(-0.25,-177.8855,2.25,1600.969);
   Canvas_XTPSTkSx4GGUTjq2VK2Z2E->SetFillColor(0);
   Canvas_XTPSTkSx4GGUTjq2VK2Z2E->SetBorderMode(0);
   Canvas_XTPSTkSx4GGUTjq2VK2Z2E->SetBorderSize(2);
   Canvas_XTPSTkSx4GGUTjq2VK2Z2E->SetFrameBorderMode(0);
   Canvas_XTPSTkSx4GGUTjq2VK2Z2E->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,125);
   h_res_s2->SetBinContent(2,118);
   h_res_s2->SetBinContent(3,123);
   h_res_s2->SetBinContent(4,227);
   h_res_s2->SetBinContent(5,331);
   h_res_s2->SetBinContent(6,323);
   h_res_s2->SetBinContent(7,510);
   h_res_s2->SetBinContent(8,636);
   h_res_s2->SetBinContent(9,844);
   h_res_s2->SetBinContent(10,1065);
   h_res_s2->SetBinContent(11,1168);
   h_res_s2->SetBinContent(12,1319);
   h_res_s2->SetBinContent(13,1185);
   h_res_s2->SetBinContent(14,963);
   h_res_s2->SetBinContent(15,651);
   h_res_s2->SetBinContent(16,411);
   h_res_s2->SetBinContent(17,166);
   h_res_s2->SetBinContent(18,55);
   h_res_s2->SetBinContent(19,10);
   h_res_s2->SetBinContent(20,10);
   h_res_s2->SetBinError(1,11.18034);
   h_res_s2->SetBinError(2,10.86278);
   h_res_s2->SetBinError(3,11.09054);
   h_res_s2->SetBinError(4,15.06652);
   h_res_s2->SetBinError(5,18.19341);
   h_res_s2->SetBinError(6,17.9722);
   h_res_s2->SetBinError(7,22.58318);
   h_res_s2->SetBinError(8,25.21904);
   h_res_s2->SetBinError(9,29.05168);
   h_res_s2->SetBinError(10,32.63434);
   h_res_s2->SetBinError(11,34.17601);
   h_res_s2->SetBinError(12,36.31804);
   h_res_s2->SetBinError(13,34.42383);
   h_res_s2->SetBinError(14,31.03224);
   h_res_s2->SetBinError(15,25.5147);
   h_res_s2->SetBinError(16,20.27313);
   h_res_s2->SetBinError(17,12.8841);
   h_res_s2->SetBinError(18,7.416198);
   h_res_s2->SetBinError(19,3.162278);
   h_res_s2->SetBinError(20,3.162278);
   h_res_s2->SetEntries(10240);
   h_res_s2->SetStats(0);
   h_res_s2->SetFillColor(1);
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
   
   TPaveText *pt = new TPaveText(0.4314286,0.935,0.5685714,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("res_s2");
   pt->Draw();
   TLine *line = new TLine(0.1349475,0,0.1349475,1319);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2140068,0,0.2140068,1319);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2732469,0,0.2732469,1319);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,1319);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.21^{-0.08}_{+0.06} ");
   pt->Draw();
   Canvas_XTPSTkSx4GGUTjq2VK2Z2E->Modified();
   Canvas_XTPSTkSx4GGUTjq2VK2Z2E->cd();
   Canvas_XTPSTkSx4GGUTjq2VK2Z2E->SetSelected(Canvas_XTPSTkSx4GGUTjq2VK2Z2E);
}
