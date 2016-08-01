{
//=========Macro generated from canvas: a9e5ab6e300b41b484ab54fc568e646e/
//=========  (Tue Feb 23 11:19:36 2016) by ROOT version5.34/26
   TCanvas *a9e5ab6e300b41b484ab54fc568e646e = new TCanvas("a9e5ab6e300b41b484ab54fc568e646e", "",703,54,704,527);
   a9e5ab6e300b41b484ab54fc568e646e->SetHighLightColor(2);
   a9e5ab6e300b41b484ab54fc568e646e->Range(-0.25,-330.3128,2.25,2972.815);
   a9e5ab6e300b41b484ab54fc568e646e->SetFillColor(0);
   a9e5ab6e300b41b484ab54fc568e646e->SetBorderMode(0);
   a9e5ab6e300b41b484ab54fc568e646e->SetBorderSize(2);
   a9e5ab6e300b41b484ab54fc568e646e->SetFrameBorderMode(0);
   a9e5ab6e300b41b484ab54fc568e646e->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(1,4);
   h_res_s1->SetBinContent(2,7);
   h_res_s1->SetBinContent(3,24);
   h_res_s1->SetBinContent(4,44);
   h_res_s1->SetBinContent(5,69);
   h_res_s1->SetBinContent(6,156);
   h_res_s1->SetBinContent(7,394);
   h_res_s1->SetBinContent(8,1061);
   h_res_s1->SetBinContent(9,1788);
   h_res_s1->SetBinContent(10,2467);
   h_res_s1->SetBinContent(11,2290);
   h_res_s1->SetBinContent(12,1356);
   h_res_s1->SetBinContent(13,512);
   h_res_s1->SetBinContent(14,58);
   h_res_s1->SetBinContent(15,10);
   h_res_s1->SetBinError(1,2);
   h_res_s1->SetBinError(2,2.645751);
   h_res_s1->SetBinError(3,4.898979);
   h_res_s1->SetBinError(4,6.63325);
   h_res_s1->SetBinError(5,8.306624);
   h_res_s1->SetBinError(6,12.49);
   h_res_s1->SetBinError(7,19.84943);
   h_res_s1->SetBinError(8,32.57299);
   h_res_s1->SetBinError(9,42.28475);
   h_res_s1->SetBinError(10,49.6689);
   h_res_s1->SetBinError(11,47.85394);
   h_res_s1->SetBinError(12,36.82391);
   h_res_s1->SetBinError(13,22.62742);
   h_res_s1->SetBinError(14,7.615773);
   h_res_s1->SetBinError(15,3.162278);
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
   TLine *line = new TLine(0.1575336,0,0.1575336,2467);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1927523,0,0.1927523,2467);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2245404,0,0.2245404,2467);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.19^{-0.04}_{+0.03} ");
   pt->Draw();
   a9e5ab6e300b41b484ab54fc568e646e->Modified();
   a9e5ab6e300b41b484ab54fc568e646e->cd();
   a9e5ab6e300b41b484ab54fc568e646e->SetSelected(a9e5ab6e300b41b484ab54fc568e646e);
}
