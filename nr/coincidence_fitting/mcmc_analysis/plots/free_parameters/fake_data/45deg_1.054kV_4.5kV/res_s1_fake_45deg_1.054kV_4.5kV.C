{
//=========Macro generated from canvas: 27dcde787cbb4607b59ddfe6d5b344da/
//=========  (Fri Feb 12 14:32:45 2016) by ROOT version5.34/26
   TCanvas *27dcde787cbb4607b59ddfe6d5b344da = new TCanvas("27dcde787cbb4607b59ddfe6d5b344da", "",740,45,704,527);
   27dcde787cbb4607b59ddfe6d5b344da->SetHighLightColor(2);
   27dcde787cbb4607b59ddfe6d5b344da->Range(-0.25,-181.7439,2.25,1635.695);
   27dcde787cbb4607b59ddfe6d5b344da->SetFillColor(0);
   27dcde787cbb4607b59ddfe6d5b344da->SetBorderMode(0);
   27dcde787cbb4607b59ddfe6d5b344da->SetBorderSize(2);
   27dcde787cbb4607b59ddfe6d5b344da->SetFrameBorderMode(0);
   27dcde787cbb4607b59ddfe6d5b344da->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(1,4);
   h_res_s1->SetBinContent(2,26);
   h_res_s1->SetBinContent(3,45);
   h_res_s1->SetBinContent(4,50);
   h_res_s1->SetBinContent(5,68);
   h_res_s1->SetBinContent(6,72);
   h_res_s1->SetBinContent(7,158);
   h_res_s1->SetBinContent(8,278);
   h_res_s1->SetBinContent(9,297);
   h_res_s1->SetBinContent(10,549);
   h_res_s1->SetBinContent(11,681);
   h_res_s1->SetBinContent(12,1083);
   h_res_s1->SetBinContent(13,1172);
   h_res_s1->SetBinContent(14,1348);
   h_res_s1->SetBinContent(15,1289);
   h_res_s1->SetBinContent(16,985);
   h_res_s1->SetBinContent(17,857);
   h_res_s1->SetBinContent(18,546);
   h_res_s1->SetBinContent(19,357);
   h_res_s1->SetBinContent(20,201);
   h_res_s1->SetBinContent(21,83);
   h_res_s1->SetBinContent(22,59);
   h_res_s1->SetBinContent(23,22);
   h_res_s1->SetBinContent(24,5);
   h_res_s1->SetBinContent(25,5);
   h_res_s1->SetBinError(1,2);
   h_res_s1->SetBinError(2,5.09902);
   h_res_s1->SetBinError(3,6.708204);
   h_res_s1->SetBinError(4,7.071068);
   h_res_s1->SetBinError(5,8.246211);
   h_res_s1->SetBinError(6,8.485281);
   h_res_s1->SetBinError(7,12.56981);
   h_res_s1->SetBinError(8,16.67333);
   h_res_s1->SetBinError(9,17.23369);
   h_res_s1->SetBinError(10,23.43075);
   h_res_s1->SetBinError(11,26.09598);
   h_res_s1->SetBinError(12,32.90897);
   h_res_s1->SetBinError(13,34.23449);
   h_res_s1->SetBinError(14,36.71512);
   h_res_s1->SetBinError(15,35.90265);
   h_res_s1->SetBinError(16,31.38471);
   h_res_s1->SetBinError(17,29.27456);
   h_res_s1->SetBinError(18,23.36664);
   h_res_s1->SetBinError(19,18.89444);
   h_res_s1->SetBinError(20,14.17745);
   h_res_s1->SetBinError(21,9.110434);
   h_res_s1->SetBinError(22,7.681146);
   h_res_s1->SetBinError(23,4.690416);
   h_res_s1->SetBinError(24,2.236068);
   h_res_s1->SetBinError(25,2.236068);
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
   TLine *line = new TLine(0.2023836,0,0.2023836,1348);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.269451,0,0.269451,1348);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3318282,0,0.3318282,1348);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3,0,0.3,1348);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.27^{-0.07}_{+0.06} ");
   pt->Draw();
   27dcde787cbb4607b59ddfe6d5b344da->Modified();
   27dcde787cbb4607b59ddfe6d5b344da->cd();
   27dcde787cbb4607b59ddfe6d5b344da->SetSelected(27dcde787cbb4607b59ddfe6d5b344da);
}
