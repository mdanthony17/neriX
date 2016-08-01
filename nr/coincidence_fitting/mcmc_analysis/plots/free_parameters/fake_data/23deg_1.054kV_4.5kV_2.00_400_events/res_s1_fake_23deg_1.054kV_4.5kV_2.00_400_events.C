{
//=========Macro generated from canvas: Canvas_2TN6E8uiECNxavmwdgo4p9/
//=========  (Mon Feb 29 14:03:46 2016) by ROOT version5.34/28
   TCanvas *Canvas_2TN6E8uiECNxavmwdgo4p9 = new TCanvas("Canvas_2TN6E8uiECNxavmwdgo4p9", "",0,0,700,500);
   Canvas_2TN6E8uiECNxavmwdgo4p9->SetHighLightColor(2);
   Canvas_2TN6E8uiECNxavmwdgo4p9->Range(-0.25,-176.4218,2.25,1587.796);
   Canvas_2TN6E8uiECNxavmwdgo4p9->SetFillColor(0);
   Canvas_2TN6E8uiECNxavmwdgo4p9->SetBorderMode(0);
   Canvas_2TN6E8uiECNxavmwdgo4p9->SetBorderSize(2);
   Canvas_2TN6E8uiECNxavmwdgo4p9->SetFrameBorderMode(0);
   Canvas_2TN6E8uiECNxavmwdgo4p9->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(1,1308);
   h_res_s1->SetBinContent(2,1236);
   h_res_s1->SetBinContent(3,1242);
   h_res_s1->SetBinContent(4,1147);
   h_res_s1->SetBinContent(5,1015);
   h_res_s1->SetBinContent(6,979);
   h_res_s1->SetBinContent(7,768);
   h_res_s1->SetBinContent(8,587);
   h_res_s1->SetBinContent(9,434);
   h_res_s1->SetBinContent(10,401);
   h_res_s1->SetBinContent(11,323);
   h_res_s1->SetBinContent(12,218);
   h_res_s1->SetBinContent(13,144);
   h_res_s1->SetBinContent(14,128);
   h_res_s1->SetBinContent(15,74);
   h_res_s1->SetBinContent(16,48);
   h_res_s1->SetBinContent(17,52);
   h_res_s1->SetBinContent(18,39);
   h_res_s1->SetBinContent(19,31);
   h_res_s1->SetBinContent(20,14);
   h_res_s1->SetBinContent(21,21);
   h_res_s1->SetBinContent(22,7);
   h_res_s1->SetBinContent(23,1);
   h_res_s1->SetBinContent(24,20);
   h_res_s1->SetBinContent(27,3);
   h_res_s1->SetBinError(1,36.16628);
   h_res_s1->SetBinError(2,35.15679);
   h_res_s1->SetBinError(3,35.24202);
   h_res_s1->SetBinError(4,33.86739);
   h_res_s1->SetBinError(5,31.85906);
   h_res_s1->SetBinError(6,31.28898);
   h_res_s1->SetBinError(7,27.71281);
   h_res_s1->SetBinError(8,24.22808);
   h_res_s1->SetBinError(9,20.83267);
   h_res_s1->SetBinError(10,20.02498);
   h_res_s1->SetBinError(11,17.9722);
   h_res_s1->SetBinError(12,14.76482);
   h_res_s1->SetBinError(13,12);
   h_res_s1->SetBinError(14,11.31371);
   h_res_s1->SetBinError(15,8.602325);
   h_res_s1->SetBinError(16,6.928203);
   h_res_s1->SetBinError(17,7.211103);
   h_res_s1->SetBinError(18,6.244998);
   h_res_s1->SetBinError(19,5.567764);
   h_res_s1->SetBinError(20,3.741657);
   h_res_s1->SetBinError(21,4.582576);
   h_res_s1->SetBinError(22,2.645751);
   h_res_s1->SetBinError(23,1);
   h_res_s1->SetBinError(24,4.472136);
   h_res_s1->SetBinError(27,1.732051);
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
   TLine *line = new TLine(0.02518058,0,0.02518058,1308);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.08368473,0,0.08368473,1308);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1752,0,0.1752,1308);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3,0,0.3,1308);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.08^{-0.06}_{+0.09} ");
   pt->Draw();
   Canvas_2TN6E8uiECNxavmwdgo4p9->Modified();
   Canvas_2TN6E8uiECNxavmwdgo4p9->cd();
   Canvas_2TN6E8uiECNxavmwdgo4p9->SetSelected(Canvas_2TN6E8uiECNxavmwdgo4p9);
}
