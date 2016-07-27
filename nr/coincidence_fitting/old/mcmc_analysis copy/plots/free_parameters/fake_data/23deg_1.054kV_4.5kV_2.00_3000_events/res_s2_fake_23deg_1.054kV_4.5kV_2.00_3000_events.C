{
//=========Macro generated from canvas: Canvas_PNEBot87odfRwYxGAApkJL/
//=========  (Mon Feb 29 14:04:36 2016) by ROOT version5.34/28
   TCanvas *Canvas_PNEBot87odfRwYxGAApkJL = new TCanvas("Canvas_PNEBot87odfRwYxGAApkJL", "",0,0,700,500);
   Canvas_PNEBot87odfRwYxGAApkJL->SetHighLightColor(2);
   Canvas_PNEBot87odfRwYxGAApkJL->Range(-0.25,-142.5986,2.25,1283.387);
   Canvas_PNEBot87odfRwYxGAApkJL->SetFillColor(0);
   Canvas_PNEBot87odfRwYxGAApkJL->SetBorderMode(0);
   Canvas_PNEBot87odfRwYxGAApkJL->SetBorderSize(2);
   Canvas_PNEBot87odfRwYxGAApkJL->SetFrameBorderMode(0);
   Canvas_PNEBot87odfRwYxGAApkJL->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,308);
   h_res_s2->SetBinContent(2,408);
   h_res_s2->SetBinContent(3,491);
   h_res_s2->SetBinContent(4,545);
   h_res_s2->SetBinContent(5,601);
   h_res_s2->SetBinContent(6,668);
   h_res_s2->SetBinContent(7,785);
   h_res_s2->SetBinContent(8,706);
   h_res_s2->SetBinContent(9,928);
   h_res_s2->SetBinContent(10,882);
   h_res_s2->SetBinContent(11,1054);
   h_res_s2->SetBinContent(12,901);
   h_res_s2->SetBinContent(13,711);
   h_res_s2->SetBinContent(14,560);
   h_res_s2->SetBinContent(15,380);
   h_res_s2->SetBinContent(16,150);
   h_res_s2->SetBinContent(17,117);
   h_res_s2->SetBinContent(18,30);
   h_res_s2->SetBinContent(19,15);
   h_res_s2->SetBinError(1,17.54993);
   h_res_s2->SetBinError(2,20.19901);
   h_res_s2->SetBinError(3,22.15852);
   h_res_s2->SetBinError(4,23.34524);
   h_res_s2->SetBinError(5,24.5153);
   h_res_s2->SetBinError(6,25.8457);
   h_res_s2->SetBinError(7,28.01785);
   h_res_s2->SetBinError(8,26.57066);
   h_res_s2->SetBinError(9,30.46309);
   h_res_s2->SetBinError(10,29.69848);
   h_res_s2->SetBinError(11,32.46537);
   h_res_s2->SetBinError(12,30.01666);
   h_res_s2->SetBinError(13,26.66458);
   h_res_s2->SetBinError(14,23.66432);
   h_res_s2->SetBinError(15,19.49359);
   h_res_s2->SetBinError(16,12.24745);
   h_res_s2->SetBinError(17,10.81665);
   h_res_s2->SetBinError(18,5.477226);
   h_res_s2->SetBinError(19,3.872983);
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
   TLine *line = new TLine(0.07545541,0,0.07545541,1054);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1731034,0,0.1731034,1054);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2494188,0,0.2494188,1054);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,1054);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.17^{-0.10}_{+0.08} ");
   pt->Draw();
   Canvas_PNEBot87odfRwYxGAApkJL->Modified();
   Canvas_PNEBot87odfRwYxGAApkJL->cd();
   Canvas_PNEBot87odfRwYxGAApkJL->SetSelected(Canvas_PNEBot87odfRwYxGAApkJL);
}
