{
//=========Macro generated from canvas: 028ba14c2c0a48f1a7d45c14e6d9dbb8/
//=========  (Tue Feb 23 07:33:19 2016) by ROOT version5.34/26
   TCanvas *028ba14c2c0a48f1a7d45c14e6d9dbb8 = new TCanvas("028ba14c2c0a48f1a7d45c14e6d9dbb8", "",306,97,704,527);
   028ba14c2c0a48f1a7d45c14e6d9dbb8->SetHighLightColor(2);
   028ba14c2c0a48f1a7d45c14e6d9dbb8->Range(-0.25,-389.0177,2.25,3501.159);
   028ba14c2c0a48f1a7d45c14e6d9dbb8->SetFillColor(0);
   028ba14c2c0a48f1a7d45c14e6d9dbb8->SetBorderMode(0);
   028ba14c2c0a48f1a7d45c14e6d9dbb8->SetBorderSize(2);
   028ba14c2c0a48f1a7d45c14e6d9dbb8->SetFrameBorderMode(0);
   028ba14c2c0a48f1a7d45c14e6d9dbb8->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,40);
   h_res_s2->SetBinContent(2,80);
   h_res_s2->SetBinContent(3,171);
   h_res_s2->SetBinContent(4,494);
   h_res_s2->SetBinContent(5,1300);
   h_res_s2->SetBinContent(6,2198);
   h_res_s2->SetBinContent(7,2910);
   h_res_s2->SetBinContent(8,2075);
   h_res_s2->SetBinContent(9,843);
   h_res_s2->SetBinContent(10,119);
   h_res_s2->SetBinContent(11,10);
   h_res_s2->SetBinError(1,6.324555);
   h_res_s2->SetBinError(2,8.944272);
   h_res_s2->SetBinError(3,13.0767);
   h_res_s2->SetBinError(4,22.22611);
   h_res_s2->SetBinError(5,36.05551);
   h_res_s2->SetBinError(6,46.88283);
   h_res_s2->SetBinError(7,53.94442);
   h_res_s2->SetBinError(8,45.55217);
   h_res_s2->SetBinError(9,29.03446);
   h_res_s2->SetBinError(10,10.90871);
   h_res_s2->SetBinError(11,3.162278);
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
   TLine *line = new TLine(0.09297169,0,0.09297169,2910);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1257526,0,0.1257526,2910);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1536756,0,0.1536756,2910);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1,0,0.1,2910);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.13^{-0.03}_{+0.03} ");
   pt->Draw();
   028ba14c2c0a48f1a7d45c14e6d9dbb8->Modified();
   028ba14c2c0a48f1a7d45c14e6d9dbb8->cd();
   028ba14c2c0a48f1a7d45c14e6d9dbb8->SetSelected(028ba14c2c0a48f1a7d45c14e6d9dbb8);
}
