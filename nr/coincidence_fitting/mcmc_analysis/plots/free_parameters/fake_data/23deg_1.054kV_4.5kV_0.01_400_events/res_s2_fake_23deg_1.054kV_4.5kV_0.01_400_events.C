{
//=========Macro generated from canvas: Canvas_TDUdQusXphhNNECLrDhZSe/
//=========  (Mon Feb 29 14:01:46 2016) by ROOT version5.34/28
   TCanvas *Canvas_TDUdQusXphhNNECLrDhZSe = new TCanvas("Canvas_TDUdQusXphhNNECLrDhZSe", "",0,0,700,500);
   Canvas_TDUdQusXphhNNECLrDhZSe->SetHighLightColor(2);
   Canvas_TDUdQusXphhNNECLrDhZSe->Range(-0.25,-128.8663,2.25,1159.796);
   Canvas_TDUdQusXphhNNECLrDhZSe->SetFillColor(0);
   Canvas_TDUdQusXphhNNECLrDhZSe->SetBorderMode(0);
   Canvas_TDUdQusXphhNNECLrDhZSe->SetBorderSize(2);
   Canvas_TDUdQusXphhNNECLrDhZSe->SetFrameBorderMode(0);
   Canvas_TDUdQusXphhNNECLrDhZSe->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,951);
   h_res_s2->SetBinContent(2,879);
   h_res_s2->SetBinContent(3,839);
   h_res_s2->SetBinContent(4,751);
   h_res_s2->SetBinContent(5,859);
   h_res_s2->SetBinContent(6,781);
   h_res_s2->SetBinContent(7,750);
   h_res_s2->SetBinContent(8,823);
   h_res_s2->SetBinContent(9,625);
   h_res_s2->SetBinContent(10,586);
   h_res_s2->SetBinContent(11,555);
   h_res_s2->SetBinContent(12,437);
   h_res_s2->SetBinContent(13,416);
   h_res_s2->SetBinContent(14,282);
   h_res_s2->SetBinContent(15,231);
   h_res_s2->SetBinContent(16,134);
   h_res_s2->SetBinContent(17,128);
   h_res_s2->SetBinContent(18,74);
   h_res_s2->SetBinContent(19,44);
   h_res_s2->SetBinContent(20,41);
   h_res_s2->SetBinContent(21,7);
   h_res_s2->SetBinContent(22,27);
   h_res_s2->SetBinContent(23,12);
   h_res_s2->SetBinContent(24,2);
   h_res_s2->SetBinContent(27,1);
   h_res_s2->SetBinContent(28,1);
   h_res_s2->SetBinContent(30,4);
   h_res_s2->SetBinError(1,30.83829);
   h_res_s2->SetBinError(2,29.64793);
   h_res_s2->SetBinError(3,28.9655);
   h_res_s2->SetBinError(4,27.40438);
   h_res_s2->SetBinError(5,29.3087);
   h_res_s2->SetBinError(6,27.94638);
   h_res_s2->SetBinError(7,27.38613);
   h_res_s2->SetBinError(8,28.68798);
   h_res_s2->SetBinError(9,25);
   h_res_s2->SetBinError(10,24.20744);
   h_res_s2->SetBinError(11,23.55844);
   h_res_s2->SetBinError(12,20.90454);
   h_res_s2->SetBinError(13,20.39608);
   h_res_s2->SetBinError(14,16.79286);
   h_res_s2->SetBinError(15,15.19868);
   h_res_s2->SetBinError(16,11.57584);
   h_res_s2->SetBinError(17,11.31371);
   h_res_s2->SetBinError(18,8.602325);
   h_res_s2->SetBinError(19,6.63325);
   h_res_s2->SetBinError(20,6.403124);
   h_res_s2->SetBinError(21,2.645751);
   h_res_s2->SetBinError(22,5.196152);
   h_res_s2->SetBinError(23,3.464102);
   h_res_s2->SetBinError(24,1.414214);
   h_res_s2->SetBinError(27,1);
   h_res_s2->SetBinError(28,1);
   h_res_s2->SetBinError(30,2);
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
   TLine *line = new TLine(0.03540751,0,0.03540751,951);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1216,0,0.1216,951);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.229741,0,0.229741,951);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,951);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.12^{-0.09}_{+0.11} ");
   pt->Draw();
   Canvas_TDUdQusXphhNNECLrDhZSe->Modified();
   Canvas_TDUdQusXphhNNECLrDhZSe->cd();
   Canvas_TDUdQusXphhNNECLrDhZSe->SetSelected(Canvas_TDUdQusXphhNNECLrDhZSe);
}
