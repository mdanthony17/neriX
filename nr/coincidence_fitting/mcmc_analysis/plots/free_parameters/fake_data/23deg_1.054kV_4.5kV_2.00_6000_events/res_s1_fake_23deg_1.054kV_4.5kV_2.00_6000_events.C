{
//=========Macro generated from canvas: Canvas_bFVDdneKTd2RSU2V6difrc/
//=========  (Mon Feb 29 14:04:10 2016) by ROOT version5.34/28
   TCanvas *Canvas_bFVDdneKTd2RSU2V6difrc = new TCanvas("Canvas_bFVDdneKTd2RSU2V6difrc", "",0,0,700,500);
   Canvas_bFVDdneKTd2RSU2V6difrc->SetHighLightColor(2);
   Canvas_bFVDdneKTd2RSU2V6difrc->Range(-0.25,-186.7988,2.25,1681.189);
   Canvas_bFVDdneKTd2RSU2V6difrc->SetFillColor(0);
   Canvas_bFVDdneKTd2RSU2V6difrc->SetBorderMode(0);
   Canvas_bFVDdneKTd2RSU2V6difrc->SetBorderSize(2);
   Canvas_bFVDdneKTd2RSU2V6difrc->SetFrameBorderMode(0);
   Canvas_bFVDdneKTd2RSU2V6difrc->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(1,30);
   h_res_s1->SetBinContent(2,23);
   h_res_s1->SetBinContent(3,68);
   h_res_s1->SetBinContent(4,40);
   h_res_s1->SetBinContent(5,79);
   h_res_s1->SetBinContent(6,93);
   h_res_s1->SetBinContent(7,141);
   h_res_s1->SetBinContent(8,254);
   h_res_s1->SetBinContent(9,342);
   h_res_s1->SetBinContent(10,582);
   h_res_s1->SetBinContent(11,768);
   h_res_s1->SetBinContent(12,996);
   h_res_s1->SetBinContent(13,1253);
   h_res_s1->SetBinContent(14,1386);
   h_res_s1->SetBinContent(15,1272);
   h_res_s1->SetBinContent(16,982);
   h_res_s1->SetBinContent(17,851);
   h_res_s1->SetBinContent(18,467);
   h_res_s1->SetBinContent(19,322);
   h_res_s1->SetBinContent(20,141);
   h_res_s1->SetBinContent(21,110);
   h_res_s1->SetBinContent(22,25);
   h_res_s1->SetBinContent(23,10);
   h_res_s1->SetBinContent(25,5);
   h_res_s1->SetBinError(1,5.477226);
   h_res_s1->SetBinError(2,4.795832);
   h_res_s1->SetBinError(3,8.246211);
   h_res_s1->SetBinError(4,6.324555);
   h_res_s1->SetBinError(5,8.888194);
   h_res_s1->SetBinError(6,9.643651);
   h_res_s1->SetBinError(7,11.87434);
   h_res_s1->SetBinError(8,15.93738);
   h_res_s1->SetBinError(9,18.49324);
   h_res_s1->SetBinError(10,24.12468);
   h_res_s1->SetBinError(11,27.71281);
   h_res_s1->SetBinError(12,31.55947);
   h_res_s1->SetBinError(13,35.39774);
   h_res_s1->SetBinError(14,37.22902);
   h_res_s1->SetBinError(15,35.66511);
   h_res_s1->SetBinError(16,31.33688);
   h_res_s1->SetBinError(17,29.1719);
   h_res_s1->SetBinError(18,21.61018);
   h_res_s1->SetBinError(19,17.94436);
   h_res_s1->SetBinError(20,11.87434);
   h_res_s1->SetBinError(21,10.48809);
   h_res_s1->SetBinError(22,5);
   h_res_s1->SetBinError(23,3.162278);
   h_res_s1->SetBinError(25,2.236068);
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
   TLine *line = new TLine(0.1991808,0,0.1991808,1386);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2665079,0,0.2665079,1386);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3271173,0,0.3271173,1386);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3,0,0.3,1386);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.27^{-0.07}_{+0.06} ");
   pt->Draw();
   Canvas_bFVDdneKTd2RSU2V6difrc->Modified();
   Canvas_bFVDdneKTd2RSU2V6difrc->cd();
   Canvas_bFVDdneKTd2RSU2V6difrc->SetSelected(Canvas_bFVDdneKTd2RSU2V6difrc);
}
