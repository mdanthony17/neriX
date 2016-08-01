{
//=========Macro generated from canvas: Canvas_b7Wz6eTdNqyRDPpzFuRokZ/
//=========  (Mon Feb 29 14:03:01 2016) by ROOT version5.34/28
   TCanvas *Canvas_b7Wz6eTdNqyRDPpzFuRokZ = new TCanvas("Canvas_b7Wz6eTdNqyRDPpzFuRokZ", "",0,0,700,500);
   Canvas_b7Wz6eTdNqyRDPpzFuRokZ->SetHighLightColor(2);
   Canvas_b7Wz6eTdNqyRDPpzFuRokZ->Range(-0.25,-148.9945,2.25,1340.951);
   Canvas_b7Wz6eTdNqyRDPpzFuRokZ->SetFillColor(0);
   Canvas_b7Wz6eTdNqyRDPpzFuRokZ->SetBorderMode(0);
   Canvas_b7Wz6eTdNqyRDPpzFuRokZ->SetBorderSize(2);
   Canvas_b7Wz6eTdNqyRDPpzFuRokZ->SetFrameBorderMode(0);
   Canvas_b7Wz6eTdNqyRDPpzFuRokZ->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,622);
   h_res_s2->SetBinContent(2,649);
   h_res_s2->SetBinContent(3,745);
   h_res_s2->SetBinContent(4,852);
   h_res_s2->SetBinContent(5,964);
   h_res_s2->SetBinContent(6,1102);
   h_res_s2->SetBinContent(7,1044);
   h_res_s2->SetBinContent(8,955);
   h_res_s2->SetBinContent(9,881);
   h_res_s2->SetBinContent(10,908);
   h_res_s2->SetBinContent(11,566);
   h_res_s2->SetBinContent(12,397);
   h_res_s2->SetBinContent(13,281);
   h_res_s2->SetBinContent(14,175);
   h_res_s2->SetBinContent(15,66);
   h_res_s2->SetBinContent(16,15);
   h_res_s2->SetBinContent(17,17);
   h_res_s2->SetBinContent(19,1);
   h_res_s2->SetBinError(1,24.93993);
   h_res_s2->SetBinError(2,25.47548);
   h_res_s2->SetBinError(3,27.29469);
   h_res_s2->SetBinError(4,29.18904);
   h_res_s2->SetBinError(5,31.04835);
   h_res_s2->SetBinError(6,33.19639);
   h_res_s2->SetBinError(7,32.31099);
   h_res_s2->SetBinError(8,30.90307);
   h_res_s2->SetBinError(9,29.68164);
   h_res_s2->SetBinError(10,30.13304);
   h_res_s2->SetBinError(11,23.79075);
   h_res_s2->SetBinError(12,19.92486);
   h_res_s2->SetBinError(13,16.76305);
   h_res_s2->SetBinError(14,13.22876);
   h_res_s2->SetBinError(15,8.124038);
   h_res_s2->SetBinError(16,3.872983);
   h_res_s2->SetBinError(17,4.123106);
   h_res_s2->SetBinError(19,1);
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
   TLine *line = new TLine(0.04958819,0,0.04958819,1102);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1235632,0,0.1235632,1102);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1975736,0,0.1975736,1102);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,1102);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.12^{-0.07}_{+0.07} ");
   pt->Draw();
   Canvas_b7Wz6eTdNqyRDPpzFuRokZ->Modified();
   Canvas_b7Wz6eTdNqyRDPpzFuRokZ->cd();
   Canvas_b7Wz6eTdNqyRDPpzFuRokZ->SetSelected(Canvas_b7Wz6eTdNqyRDPpzFuRokZ);
}
