{
//=========Macro generated from canvas: 8ef6844ed54c4ff3821839bb6fc94754/
//=========  (Tue Feb 23 11:22:41 2016) by ROOT version5.34/26
   TCanvas *8ef6844ed54c4ff3821839bb6fc94754 = new TCanvas("8ef6844ed54c4ff3821839bb6fc94754", "",616,53,704,527);
   8ef6844ed54c4ff3821839bb6fc94754->SetHighLightColor(2);
   8ef6844ed54c4ff3821839bb6fc94754->Range(-0.25,-255.4943,2.25,2299.449);
   8ef6844ed54c4ff3821839bb6fc94754->SetFillColor(0);
   8ef6844ed54c4ff3821839bb6fc94754->SetBorderMode(0);
   8ef6844ed54c4ff3821839bb6fc94754->SetBorderSize(2);
   8ef6844ed54c4ff3821839bb6fc94754->SetFrameBorderMode(0);
   8ef6844ed54c4ff3821839bb6fc94754->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(1,159);
   h_res_s1->SetBinContent(2,394);
   h_res_s1->SetBinContent(3,674);
   h_res_s1->SetBinContent(4,1009);
   h_res_s1->SetBinContent(5,1496);
   h_res_s1->SetBinContent(6,1836);
   h_res_s1->SetBinContent(7,1903);
   h_res_s1->SetBinContent(8,1418);
   h_res_s1->SetBinContent(9,866);
   h_res_s1->SetBinContent(10,350);
   h_res_s1->SetBinContent(11,115);
   h_res_s1->SetBinContent(12,20);
   h_res_s1->SetBinError(1,12.60952);
   h_res_s1->SetBinError(2,19.84943);
   h_res_s1->SetBinError(3,25.96151);
   h_res_s1->SetBinError(4,31.76476);
   h_res_s1->SetBinError(5,38.67816);
   h_res_s1->SetBinError(6,42.84857);
   h_res_s1->SetBinError(7,43.62339);
   h_res_s1->SetBinError(8,37.65634);
   h_res_s1->SetBinError(9,29.42788);
   h_res_s1->SetBinError(10,18.70829);
   h_res_s1->SetBinError(11,10.72381);
   h_res_s1->SetBinError(12,4.472136);
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
   TLine *line = new TLine(0.06795164,0,0.06795164,1903);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1151198,0,0.1151198,1903);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1560908,0,0.1560908,1903);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.12^{-0.05}_{+0.04} ");
   pt->Draw();
   8ef6844ed54c4ff3821839bb6fc94754->Modified();
   8ef6844ed54c4ff3821839bb6fc94754->cd();
   8ef6844ed54c4ff3821839bb6fc94754->SetSelected(8ef6844ed54c4ff3821839bb6fc94754);
}
