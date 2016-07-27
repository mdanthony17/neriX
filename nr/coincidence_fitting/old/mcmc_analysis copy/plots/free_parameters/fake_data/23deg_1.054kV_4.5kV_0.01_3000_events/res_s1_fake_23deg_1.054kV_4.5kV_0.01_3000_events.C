{
//=========Macro generated from canvas: Canvas_SgsqKYziamKUJdhsWQdWNJ/
//=========  (Mon Feb 29 14:02:12 2016) by ROOT version5.34/28
   TCanvas *Canvas_SgsqKYziamKUJdhsWQdWNJ = new TCanvas("Canvas_SgsqKYziamKUJdhsWQdWNJ", "",0,0,700,500);
   Canvas_SgsqKYziamKUJdhsWQdWNJ->SetHighLightColor(2);
   Canvas_SgsqKYziamKUJdhsWQdWNJ->Range(-0.25,-150.0603,2.25,1350.543);
   Canvas_SgsqKYziamKUJdhsWQdWNJ->SetFillColor(0);
   Canvas_SgsqKYziamKUJdhsWQdWNJ->SetBorderMode(0);
   Canvas_SgsqKYziamKUJdhsWQdWNJ->SetBorderSize(2);
   Canvas_SgsqKYziamKUJdhsWQdWNJ->SetFrameBorderMode(0);
   Canvas_SgsqKYziamKUJdhsWQdWNJ->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(1,25);
   h_res_s1->SetBinContent(2,25);
   h_res_s1->SetBinContent(3,75);
   h_res_s1->SetBinContent(4,96);
   h_res_s1->SetBinContent(5,110);
   h_res_s1->SetBinContent(6,183);
   h_res_s1->SetBinContent(7,208);
   h_res_s1->SetBinContent(8,249);
   h_res_s1->SetBinContent(9,449);
   h_res_s1->SetBinContent(10,555);
   h_res_s1->SetBinContent(11,681);
   h_res_s1->SetBinContent(12,783);
   h_res_s1->SetBinContent(13,988);
   h_res_s1->SetBinContent(14,1110);
   h_res_s1->SetBinContent(15,987);
   h_res_s1->SetBinContent(16,833);
   h_res_s1->SetBinContent(17,761);
   h_res_s1->SetBinContent(18,689);
   h_res_s1->SetBinContent(19,516);
   h_res_s1->SetBinContent(20,312);
   h_res_s1->SetBinContent(21,239);
   h_res_s1->SetBinContent(22,118);
   h_res_s1->SetBinContent(23,127);
   h_res_s1->SetBinContent(24,66);
   h_res_s1->SetBinContent(25,20);
   h_res_s1->SetBinContent(26,30);
   h_res_s1->SetBinContent(30,5);
   h_res_s1->SetBinError(1,5);
   h_res_s1->SetBinError(2,5);
   h_res_s1->SetBinError(3,8.660254);
   h_res_s1->SetBinError(4,9.797959);
   h_res_s1->SetBinError(5,10.48809);
   h_res_s1->SetBinError(6,13.52775);
   h_res_s1->SetBinError(7,14.42221);
   h_res_s1->SetBinError(8,15.77973);
   h_res_s1->SetBinError(9,21.18962);
   h_res_s1->SetBinError(10,23.55844);
   h_res_s1->SetBinError(11,26.09598);
   h_res_s1->SetBinError(12,27.98214);
   h_res_s1->SetBinError(13,31.43247);
   h_res_s1->SetBinError(14,33.31666);
   h_res_s1->SetBinError(15,31.41656);
   h_res_s1->SetBinError(16,28.86174);
   h_res_s1->SetBinError(17,27.58623);
   h_res_s1->SetBinError(18,26.24881);
   h_res_s1->SetBinError(19,22.71563);
   h_res_s1->SetBinError(20,17.66352);
   h_res_s1->SetBinError(21,15.45962);
   h_res_s1->SetBinError(22,10.86278);
   h_res_s1->SetBinError(23,11.26943);
   h_res_s1->SetBinError(24,8.124038);
   h_res_s1->SetBinError(25,4.472136);
   h_res_s1->SetBinError(26,5.477226);
   h_res_s1->SetBinError(30,2.236068);
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
   TLine *line = new TLine(0.1875013,0,0.1875013,1110);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2724865,0,0.2724865,1110);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.354335,0,0.354335,1110);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3,0,0.3,1110);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.27^{-0.08}_{+0.08} ");
   pt->Draw();
   Canvas_SgsqKYziamKUJdhsWQdWNJ->Modified();
   Canvas_SgsqKYziamKUJdhsWQdWNJ->cd();
   Canvas_SgsqKYziamKUJdhsWQdWNJ->SetSelected(Canvas_SgsqKYziamKUJdhsWQdWNJ);
}
