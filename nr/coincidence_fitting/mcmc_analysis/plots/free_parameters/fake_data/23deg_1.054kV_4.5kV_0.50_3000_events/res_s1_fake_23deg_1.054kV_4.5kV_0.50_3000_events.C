{
//=========Macro generated from canvas: Canvas_E9vpUDWUhpauDphpeNCtfQ/
//=========  (Mon Feb 29 14:03:27 2016) by ROOT version5.34/28
   TCanvas *Canvas_E9vpUDWUhpauDphpeNCtfQ = new TCanvas("Canvas_E9vpUDWUhpauDphpeNCtfQ", "",0,0,700,500);
   Canvas_E9vpUDWUhpauDphpeNCtfQ->SetHighLightColor(2);
   Canvas_E9vpUDWUhpauDphpeNCtfQ->Range(-0.25,-137.6669,2.25,1239.002);
   Canvas_E9vpUDWUhpauDphpeNCtfQ->SetFillColor(0);
   Canvas_E9vpUDWUhpauDphpeNCtfQ->SetBorderMode(0);
   Canvas_E9vpUDWUhpauDphpeNCtfQ->SetBorderSize(2);
   Canvas_E9vpUDWUhpauDphpeNCtfQ->SetFrameBorderMode(0);
   Canvas_E9vpUDWUhpauDphpeNCtfQ->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(1,93);
   h_res_s1->SetBinContent(2,136);
   h_res_s1->SetBinContent(3,202);
   h_res_s1->SetBinContent(4,254);
   h_res_s1->SetBinContent(5,331);
   h_res_s1->SetBinContent(6,304);
   h_res_s1->SetBinContent(7,422);
   h_res_s1->SetBinContent(8,567);
   h_res_s1->SetBinContent(9,669);
   h_res_s1->SetBinContent(10,840);
   h_res_s1->SetBinContent(11,900);
   h_res_s1->SetBinContent(12,892);
   h_res_s1->SetBinContent(13,951);
   h_res_s1->SetBinContent(14,1017);
   h_res_s1->SetBinContent(15,731);
   h_res_s1->SetBinContent(16,559);
   h_res_s1->SetBinContent(17,519);
   h_res_s1->SetBinContent(18,359);
   h_res_s1->SetBinContent(19,203);
   h_res_s1->SetBinContent(20,166);
   h_res_s1->SetBinContent(21,66);
   h_res_s1->SetBinContent(22,30);
   h_res_s1->SetBinContent(23,14);
   h_res_s1->SetBinContent(24,5);
   h_res_s1->SetBinContent(25,10);
   h_res_s1->SetBinError(1,9.643651);
   h_res_s1->SetBinError(2,11.6619);
   h_res_s1->SetBinError(3,14.21267);
   h_res_s1->SetBinError(4,15.93738);
   h_res_s1->SetBinError(5,18.19341);
   h_res_s1->SetBinError(6,17.4356);
   h_res_s1->SetBinError(7,20.54264);
   h_res_s1->SetBinError(8,23.81176);
   h_res_s1->SetBinError(9,25.86503);
   h_res_s1->SetBinError(10,28.98275);
   h_res_s1->SetBinError(11,30);
   h_res_s1->SetBinError(12,29.86637);
   h_res_s1->SetBinError(13,30.83829);
   h_res_s1->SetBinError(14,31.89044);
   h_res_s1->SetBinError(15,27.03701);
   h_res_s1->SetBinError(16,23.64318);
   h_res_s1->SetBinError(17,22.78157);
   h_res_s1->SetBinError(18,18.9473);
   h_res_s1->SetBinError(19,14.24781);
   h_res_s1->SetBinError(20,12.8841);
   h_res_s1->SetBinError(21,8.124038);
   h_res_s1->SetBinError(22,5.477226);
   h_res_s1->SetBinError(23,3.741657);
   h_res_s1->SetBinError(24,2.236068);
   h_res_s1->SetBinError(25,3.162278);
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
   TLine *line = new TLine(0.1346047,0,0.1346047,1017);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2290135,0,0.2290135,1017);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3108351,0,0.3108351,1017);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3,0,0.3,1017);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.23^{-0.09}_{+0.08} ");
   pt->Draw();
   Canvas_E9vpUDWUhpauDphpeNCtfQ->Modified();
   Canvas_E9vpUDWUhpauDphpeNCtfQ->cd();
   Canvas_E9vpUDWUhpauDphpeNCtfQ->SetSelected(Canvas_E9vpUDWUhpauDphpeNCtfQ);
}
