{
//=========Macro generated from canvas: Canvas_PmyQGnLcPm36VMX7zovmPH/
//=========  (Mon Feb 29 14:02:48 2016) by ROOT version5.34/28
   TCanvas *Canvas_PmyQGnLcPm36VMX7zovmPH = new TCanvas("Canvas_PmyQGnLcPm36VMX7zovmPH", "",0,0,700,500);
   Canvas_PmyQGnLcPm36VMX7zovmPH->SetHighLightColor(2);
   Canvas_PmyQGnLcPm36VMX7zovmPH->Range(-0.25,-128.3328,2.25,1154.995);
   Canvas_PmyQGnLcPm36VMX7zovmPH->SetFillColor(0);
   Canvas_PmyQGnLcPm36VMX7zovmPH->SetBorderMode(0);
   Canvas_PmyQGnLcPm36VMX7zovmPH->SetBorderSize(2);
   Canvas_PmyQGnLcPm36VMX7zovmPH->SetFrameBorderMode(0);
   Canvas_PmyQGnLcPm36VMX7zovmPH->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,705);
   h_res_s2->SetBinContent(2,804);
   h_res_s2->SetBinContent(3,825);
   h_res_s2->SetBinContent(4,726);
   h_res_s2->SetBinContent(5,908);
   h_res_s2->SetBinContent(6,947);
   h_res_s2->SetBinContent(7,825);
   h_res_s2->SetBinContent(8,698);
   h_res_s2->SetBinContent(9,716);
   h_res_s2->SetBinContent(10,660);
   h_res_s2->SetBinContent(11,697);
   h_res_s2->SetBinContent(12,540);
   h_res_s2->SetBinContent(13,393);
   h_res_s2->SetBinContent(14,285);
   h_res_s2->SetBinContent(15,182);
   h_res_s2->SetBinContent(16,157);
   h_res_s2->SetBinContent(17,48);
   h_res_s2->SetBinContent(18,58);
   h_res_s2->SetBinContent(19,41);
   h_res_s2->SetBinContent(20,18);
   h_res_s2->SetBinContent(21,5);
   h_res_s2->SetBinContent(24,2);
   h_res_s2->SetBinError(1,26.55184);
   h_res_s2->SetBinError(2,28.35489);
   h_res_s2->SetBinError(3,28.72281);
   h_res_s2->SetBinError(4,26.94439);
   h_res_s2->SetBinError(5,30.13304);
   h_res_s2->SetBinError(6,30.77337);
   h_res_s2->SetBinError(7,28.72281);
   h_res_s2->SetBinError(8,26.41969);
   h_res_s2->SetBinError(9,26.75818);
   h_res_s2->SetBinError(10,25.69047);
   h_res_s2->SetBinError(11,26.40076);
   h_res_s2->SetBinError(12,23.2379);
   h_res_s2->SetBinError(13,19.82423);
   h_res_s2->SetBinError(14,16.88194);
   h_res_s2->SetBinError(15,13.49074);
   h_res_s2->SetBinError(16,12.52996);
   h_res_s2->SetBinError(17,6.928203);
   h_res_s2->SetBinError(18,7.615773);
   h_res_s2->SetBinError(19,6.403124);
   h_res_s2->SetBinError(20,4.242641);
   h_res_s2->SetBinError(21,2.236068);
   h_res_s2->SetBinError(24,1.414214);
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
   TLine *line = new TLine(0.04288873,0,0.04288873,947);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1249697,0,0.1249697,947);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2237348,0,0.2237348,947);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,947);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.12^{-0.08}_{+0.10} ");
   pt->Draw();
   Canvas_PmyQGnLcPm36VMX7zovmPH->Modified();
   Canvas_PmyQGnLcPm36VMX7zovmPH->cd();
   Canvas_PmyQGnLcPm36VMX7zovmPH->SetSelected(Canvas_PmyQGnLcPm36VMX7zovmPH);
}
