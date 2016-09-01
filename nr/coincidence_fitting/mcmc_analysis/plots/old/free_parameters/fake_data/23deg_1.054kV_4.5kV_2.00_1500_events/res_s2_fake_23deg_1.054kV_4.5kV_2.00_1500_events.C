{
//=========Macro generated from canvas: Canvas_Ho2uqTC8okPpNFShVux4J6/
//=========  (Mon Feb 29 14:04:38 2016) by ROOT version5.34/28
   TCanvas *Canvas_Ho2uqTC8okPpNFShVux4J6 = new TCanvas("Canvas_Ho2uqTC8okPpNFShVux4J6", "",0,0,700,500);
   Canvas_Ho2uqTC8okPpNFShVux4J6->SetHighLightColor(2);
   Canvas_Ho2uqTC8okPpNFShVux4J6->Range(-0.25,-124.8644,2.25,1123.78);
   Canvas_Ho2uqTC8okPpNFShVux4J6->SetFillColor(0);
   Canvas_Ho2uqTC8okPpNFShVux4J6->SetBorderMode(0);
   Canvas_Ho2uqTC8okPpNFShVux4J6->SetBorderSize(2);
   Canvas_Ho2uqTC8okPpNFShVux4J6->SetFrameBorderMode(0);
   Canvas_Ho2uqTC8okPpNFShVux4J6->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,816);
   h_res_s2->SetBinContent(2,863);
   h_res_s2->SetBinContent(3,794);
   h_res_s2->SetBinContent(4,921);
   h_res_s2->SetBinContent(5,756);
   h_res_s2->SetBinContent(6,765);
   h_res_s2->SetBinContent(7,713);
   h_res_s2->SetBinContent(8,691);
   h_res_s2->SetBinContent(9,713);
   h_res_s2->SetBinContent(10,529);
   h_res_s2->SetBinContent(11,539);
   h_res_s2->SetBinContent(12,500);
   h_res_s2->SetBinContent(13,461);
   h_res_s2->SetBinContent(14,406);
   h_res_s2->SetBinContent(15,285);
   h_res_s2->SetBinContent(16,223);
   h_res_s2->SetBinContent(17,154);
   h_res_s2->SetBinContent(18,48);
   h_res_s2->SetBinContent(19,43);
   h_res_s2->SetBinContent(20,15);
   h_res_s2->SetBinContent(21,5);
   h_res_s2->SetBinError(1,28.56571);
   h_res_s2->SetBinError(2,29.37686);
   h_res_s2->SetBinError(3,28.17801);
   h_res_s2->SetBinError(4,30.34798);
   h_res_s2->SetBinError(5,27.49545);
   h_res_s2->SetBinError(6,27.65863);
   h_res_s2->SetBinError(7,26.70206);
   h_res_s2->SetBinError(8,26.28688);
   h_res_s2->SetBinError(9,26.70206);
   h_res_s2->SetBinError(10,23);
   h_res_s2->SetBinError(11,23.21637);
   h_res_s2->SetBinError(12,22.36068);
   h_res_s2->SetBinError(13,21.47091);
   h_res_s2->SetBinError(14,20.14944);
   h_res_s2->SetBinError(15,16.88194);
   h_res_s2->SetBinError(16,14.93318);
   h_res_s2->SetBinError(17,12.40967);
   h_res_s2->SetBinError(18,6.928203);
   h_res_s2->SetBinError(19,6.557439);
   h_res_s2->SetBinError(20,3.872983);
   h_res_s2->SetBinError(21,2.236068);
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
   TLine *line = new TLine(0.03882178,0,0.03882178,921);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1257504,0,0.1257504,921);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2405137,0,0.2405137,921);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,921);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.13^{-0.09}_{+0.11} ");
   pt->Draw();
   Canvas_Ho2uqTC8okPpNFShVux4J6->Modified();
   Canvas_Ho2uqTC8okPpNFShVux4J6->cd();
   Canvas_Ho2uqTC8okPpNFShVux4J6->SetSelected(Canvas_Ho2uqTC8okPpNFShVux4J6);
}
