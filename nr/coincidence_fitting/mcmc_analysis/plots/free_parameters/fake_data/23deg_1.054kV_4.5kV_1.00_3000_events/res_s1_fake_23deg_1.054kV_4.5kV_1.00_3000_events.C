{
//=========Macro generated from canvas: Canvas_FH7JjY3bxWKbcwsgtr8aZm/
//=========  (Mon Feb 29 14:03:57 2016) by ROOT version5.34/28
   TCanvas *Canvas_FH7JjY3bxWKbcwsgtr8aZm = new TCanvas("Canvas_FH7JjY3bxWKbcwsgtr8aZm", "",0,0,700,500);
   Canvas_FH7JjY3bxWKbcwsgtr8aZm->SetHighLightColor(2);
   Canvas_FH7JjY3bxWKbcwsgtr8aZm->Range(-0.25,-187.7299,2.25,1689.569);
   Canvas_FH7JjY3bxWKbcwsgtr8aZm->SetFillColor(0);
   Canvas_FH7JjY3bxWKbcwsgtr8aZm->SetBorderMode(0);
   Canvas_FH7JjY3bxWKbcwsgtr8aZm->SetBorderSize(2);
   Canvas_FH7JjY3bxWKbcwsgtr8aZm->SetFrameBorderMode(0);
   Canvas_FH7JjY3bxWKbcwsgtr8aZm->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(1,23);
   h_res_s1->SetBinContent(2,60);
   h_res_s1->SetBinContent(3,80);
   h_res_s1->SetBinContent(4,61);
   h_res_s1->SetBinContent(5,96);
   h_res_s1->SetBinContent(6,150);
   h_res_s1->SetBinContent(7,186);
   h_res_s1->SetBinContent(8,376);
   h_res_s1->SetBinContent(9,464);
   h_res_s1->SetBinContent(10,703);
   h_res_s1->SetBinContent(11,1052);
   h_res_s1->SetBinContent(12,1172);
   h_res_s1->SetBinContent(13,1263);
   h_res_s1->SetBinContent(14,1393);
   h_res_s1->SetBinContent(15,1111);
   h_res_s1->SetBinContent(16,808);
   h_res_s1->SetBinContent(17,550);
   h_res_s1->SetBinContent(18,386);
   h_res_s1->SetBinContent(19,174);
   h_res_s1->SetBinContent(20,62);
   h_res_s1->SetBinContent(21,25);
   h_res_s1->SetBinContent(22,35);
   h_res_s1->SetBinContent(23,10);
   h_res_s1->SetBinError(1,4.795832);
   h_res_s1->SetBinError(2,7.745967);
   h_res_s1->SetBinError(3,8.944272);
   h_res_s1->SetBinError(4,7.81025);
   h_res_s1->SetBinError(5,9.797959);
   h_res_s1->SetBinError(6,12.24745);
   h_res_s1->SetBinError(7,13.63818);
   h_res_s1->SetBinError(8,19.39072);
   h_res_s1->SetBinError(9,21.54066);
   h_res_s1->SetBinError(10,26.51415);
   h_res_s1->SetBinError(11,32.43455);
   h_res_s1->SetBinError(12,34.23449);
   h_res_s1->SetBinError(13,35.53871);
   h_res_s1->SetBinError(14,37.32292);
   h_res_s1->SetBinError(15,33.33167);
   h_res_s1->SetBinError(16,28.42534);
   h_res_s1->SetBinError(17,23.45208);
   h_res_s1->SetBinError(18,19.64688);
   h_res_s1->SetBinError(19,13.19091);
   h_res_s1->SetBinError(20,7.874008);
   h_res_s1->SetBinError(21,5);
   h_res_s1->SetBinError(22,5.91608);
   h_res_s1->SetBinError(23,3.162278);
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
   TLine *line = new TLine(0.1837599,0,0.1837599,1393);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2510372,0,0.2510372,1393);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3104416,0,0.3104416,1393);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3,0,0.3,1393);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.25^{-0.07}_{+0.06} ");
   pt->Draw();
   Canvas_FH7JjY3bxWKbcwsgtr8aZm->Modified();
   Canvas_FH7JjY3bxWKbcwsgtr8aZm->cd();
   Canvas_FH7JjY3bxWKbcwsgtr8aZm->SetSelected(Canvas_FH7JjY3bxWKbcwsgtr8aZm);
}
