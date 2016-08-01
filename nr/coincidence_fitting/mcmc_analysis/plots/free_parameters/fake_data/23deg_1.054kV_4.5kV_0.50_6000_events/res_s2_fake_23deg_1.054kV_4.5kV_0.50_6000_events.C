{
//=========Macro generated from canvas: Canvas_kBjrwRQikoXxLgfwv4SQsN/
//=========  (Mon Feb 29 14:03:23 2016) by ROOT version5.34/28
   TCanvas *Canvas_kBjrwRQikoXxLgfwv4SQsN = new TCanvas("Canvas_kBjrwRQikoXxLgfwv4SQsN", "",0,0,700,500);
   Canvas_kBjrwRQikoXxLgfwv4SQsN->SetHighLightColor(2);
   Canvas_kBjrwRQikoXxLgfwv4SQsN->Range(-0.25,-167.638,2.25,1508.742);
   Canvas_kBjrwRQikoXxLgfwv4SQsN->SetFillColor(0);
   Canvas_kBjrwRQikoXxLgfwv4SQsN->SetBorderMode(0);
   Canvas_kBjrwRQikoXxLgfwv4SQsN->SetBorderSize(2);
   Canvas_kBjrwRQikoXxLgfwv4SQsN->SetFrameBorderMode(0);
   Canvas_kBjrwRQikoXxLgfwv4SQsN->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,285);
   h_res_s2->SetBinContent(2,355);
   h_res_s2->SetBinContent(3,530);
   h_res_s2->SetBinContent(4,796);
   h_res_s2->SetBinContent(5,764);
   h_res_s2->SetBinContent(6,1065);
   h_res_s2->SetBinContent(7,1093);
   h_res_s2->SetBinContent(8,1242);
   h_res_s2->SetBinContent(9,1214);
   h_res_s2->SetBinContent(10,1072);
   h_res_s2->SetBinContent(11,788);
   h_res_s2->SetBinContent(12,572);
   h_res_s2->SetBinContent(13,289);
   h_res_s2->SetBinContent(14,110);
   h_res_s2->SetBinContent(15,29);
   h_res_s2->SetBinContent(16,36);
   h_res_s2->SetBinError(1,16.88194);
   h_res_s2->SetBinError(2,18.84144);
   h_res_s2->SetBinError(3,23.02173);
   h_res_s2->SetBinError(4,28.21347);
   h_res_s2->SetBinError(5,27.64055);
   h_res_s2->SetBinError(6,32.63434);
   h_res_s2->SetBinError(7,33.06055);
   h_res_s2->SetBinError(8,35.24202);
   h_res_s2->SetBinError(9,34.8425);
   h_res_s2->SetBinError(10,32.74141);
   h_res_s2->SetBinError(11,28.07134);
   h_res_s2->SetBinError(12,23.91652);
   h_res_s2->SetBinError(13,17);
   h_res_s2->SetBinError(14,10.48809);
   h_res_s2->SetBinError(15,5.385165);
   h_res_s2->SetBinError(16,6);
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
   TLine *line = new TLine(0.07151156,0,0.07151156,1242);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1437359,0,0.1437359,1242);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2049706,0,0.2049706,1242);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,1242);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.14^{-0.07}_{+0.06} ");
   pt->Draw();
   Canvas_kBjrwRQikoXxLgfwv4SQsN->Modified();
   Canvas_kBjrwRQikoXxLgfwv4SQsN->cd();
   Canvas_kBjrwRQikoXxLgfwv4SQsN->SetSelected(Canvas_kBjrwRQikoXxLgfwv4SQsN);
}
