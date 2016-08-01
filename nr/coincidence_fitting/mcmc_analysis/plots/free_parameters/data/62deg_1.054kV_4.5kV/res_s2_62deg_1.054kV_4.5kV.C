{
//=========Macro generated from canvas: 7404476769ee45489d1d3953ca6c7718/
//=========  (Tue Feb 23 11:22:40 2016) by ROOT version5.34/26
   TCanvas *7404476769ee45489d1d3953ca6c7718 = new TCanvas("7404476769ee45489d1d3953ca6c7718", "",73,53,704,527);
   7404476769ee45489d1d3953ca6c7718->SetHighLightColor(2);
   7404476769ee45489d1d3953ca6c7718->Range(-0.25,-562.9303,2.25,5066.372);
   7404476769ee45489d1d3953ca6c7718->SetFillColor(0);
   7404476769ee45489d1d3953ca6c7718->SetBorderMode(0);
   7404476769ee45489d1d3953ca6c7718->SetBorderSize(2);
   7404476769ee45489d1d3953ca6c7718->SetFrameBorderMode(0);
   7404476769ee45489d1d3953ca6c7718->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,2515);
   h_res_s2->SetBinContent(2,4224);
   h_res_s2->SetBinContent(3,2938);
   h_res_s2->SetBinContent(4,548);
   h_res_s2->SetBinContent(5,15);
   h_res_s2->SetBinError(1,50.14978);
   h_res_s2->SetBinError(2,64.99231);
   h_res_s2->SetBinError(3,54.20332);
   h_res_s2->SetBinError(4,23.4094);
   h_res_s2->SetBinError(5,3.872983);
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
   TLine *line = new TLine(0.01294759,0,0.01294759,4224);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.03233428,0,0.03233428,4224);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05274908,0,0.05274908,4224);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.03^{-0.02}_{+0.02} ");
   pt->Draw();
   7404476769ee45489d1d3953ca6c7718->Modified();
   7404476769ee45489d1d3953ca6c7718->cd();
   7404476769ee45489d1d3953ca6c7718->SetSelected(7404476769ee45489d1d3953ca6c7718);
}
