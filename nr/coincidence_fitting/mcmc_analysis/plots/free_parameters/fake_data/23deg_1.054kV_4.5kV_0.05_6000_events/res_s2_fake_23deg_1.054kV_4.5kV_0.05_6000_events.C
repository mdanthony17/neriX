{
//=========Macro generated from canvas: Canvas_Zg7TmXarfBKcZ5cUQDTbVR/
//=========  (Mon Feb 29 14:02:33 2016) by ROOT version5.34/28
   TCanvas *Canvas_Zg7TmXarfBKcZ5cUQDTbVR = new TCanvas("Canvas_Zg7TmXarfBKcZ5cUQDTbVR", "",0,0,700,500);
   Canvas_Zg7TmXarfBKcZ5cUQDTbVR->SetHighLightColor(2);
   Canvas_Zg7TmXarfBKcZ5cUQDTbVR->Range(-0.25,-185.8677,2.25,1672.809);
   Canvas_Zg7TmXarfBKcZ5cUQDTbVR->SetFillColor(0);
   Canvas_Zg7TmXarfBKcZ5cUQDTbVR->SetBorderMode(0);
   Canvas_Zg7TmXarfBKcZ5cUQDTbVR->SetBorderSize(2);
   Canvas_Zg7TmXarfBKcZ5cUQDTbVR->SetFrameBorderMode(0);
   Canvas_Zg7TmXarfBKcZ5cUQDTbVR->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,494);
   h_res_s2->SetBinContent(2,719);
   h_res_s2->SetBinContent(3,1030);
   h_res_s2->SetBinContent(4,1191);
   h_res_s2->SetBinContent(5,1230);
   h_res_s2->SetBinContent(6,1379);
   h_res_s2->SetBinContent(7,1208);
   h_res_s2->SetBinContent(8,1187);
   h_res_s2->SetBinContent(9,815);
   h_res_s2->SetBinContent(10,529);
   h_res_s2->SetBinContent(11,275);
   h_res_s2->SetBinContent(12,123);
   h_res_s2->SetBinContent(13,55);
   h_res_s2->SetBinContent(15,5);
   h_res_s2->SetBinError(1,22.22611);
   h_res_s2->SetBinError(2,26.81418);
   h_res_s2->SetBinError(3,32.09361);
   h_res_s2->SetBinError(4,34.51087);
   h_res_s2->SetBinError(5,35.07136);
   h_res_s2->SetBinError(6,37.13489);
   h_res_s2->SetBinError(7,34.75629);
   h_res_s2->SetBinError(8,34.45287);
   h_res_s2->SetBinError(9,28.5482);
   h_res_s2->SetBinError(10,23);
   h_res_s2->SetBinError(11,16.58312);
   h_res_s2->SetBinError(12,11.09054);
   h_res_s2->SetBinError(13,7.416198);
   h_res_s2->SetBinError(15,2.236068);
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
   TLine *line = new TLine(0.04806136,0,0.04806136,1379);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1066135,0,0.1066135,1379);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.164266,0,0.164266,1379);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,1379);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.11^{-0.06}_{+0.06} ");
   pt->Draw();
   Canvas_Zg7TmXarfBKcZ5cUQDTbVR->Modified();
   Canvas_Zg7TmXarfBKcZ5cUQDTbVR->cd();
   Canvas_Zg7TmXarfBKcZ5cUQDTbVR->SetSelected(Canvas_Zg7TmXarfBKcZ5cUQDTbVR);
}
