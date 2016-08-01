{
//=========Macro generated from canvas: Canvas_PVm9gppDFMZertXuXYz9M4/
//=========  (Mon Feb 29 14:02:08 2016) by ROOT version5.34/28
   TCanvas *Canvas_PVm9gppDFMZertXuXYz9M4 = new TCanvas("Canvas_PVm9gppDFMZertXuXYz9M4", "",0,0,700,500);
   Canvas_PVm9gppDFMZertXuXYz9M4->SetHighLightColor(2);
   Canvas_PVm9gppDFMZertXuXYz9M4->Range(-0.25,-168.5698,2.25,1517.128);
   Canvas_PVm9gppDFMZertXuXYz9M4->SetFillColor(0);
   Canvas_PVm9gppDFMZertXuXYz9M4->SetBorderMode(0);
   Canvas_PVm9gppDFMZertXuXYz9M4->SetBorderSize(2);
   Canvas_PVm9gppDFMZertXuXYz9M4->SetFrameBorderMode(0);
   Canvas_PVm9gppDFMZertXuXYz9M4->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,958);
   h_res_s2->SetBinContent(2,1050);
   h_res_s2->SetBinContent(3,1134);
   h_res_s2->SetBinContent(4,978);
   h_res_s2->SetBinContent(5,1197);
   h_res_s2->SetBinContent(6,1249);
   h_res_s2->SetBinContent(7,1067);
   h_res_s2->SetBinContent(8,840);
   h_res_s2->SetBinContent(9,672);
   h_res_s2->SetBinContent(10,454);
   h_res_s2->SetBinContent(11,281);
   h_res_s2->SetBinContent(12,187);
   h_res_s2->SetBinContent(13,95);
   h_res_s2->SetBinContent(14,57);
   h_res_s2->SetBinContent(15,16);
   h_res_s2->SetBinContent(17,5);
   h_res_s2->SetBinError(1,30.95158);
   h_res_s2->SetBinError(2,32.4037);
   h_res_s2->SetBinError(3,33.67492);
   h_res_s2->SetBinError(4,31.27299);
   h_res_s2->SetBinError(5,34.59769);
   h_res_s2->SetBinError(6,35.34119);
   h_res_s2->SetBinError(7,32.66497);
   h_res_s2->SetBinError(8,28.98275);
   h_res_s2->SetBinError(9,25.92296);
   h_res_s2->SetBinError(10,21.30728);
   h_res_s2->SetBinError(11,16.76305);
   h_res_s2->SetBinError(12,13.67479);
   h_res_s2->SetBinError(13,9.746794);
   h_res_s2->SetBinError(14,7.549834);
   h_res_s2->SetBinError(15,4);
   h_res_s2->SetBinError(17,2.236068);
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
   TLine *line = new TLine(0.03276495,0,0.03276495,1249);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.09670844,0,0.09670844,1249);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1641321,0,0.1641321,1249);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,1249);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.10^{-0.06}_{+0.07} ");
   pt->Draw();
   Canvas_PVm9gppDFMZertXuXYz9M4->Modified();
   Canvas_PVm9gppDFMZertXuXYz9M4->cd();
   Canvas_PVm9gppDFMZertXuXYz9M4->SetSelected(Canvas_PVm9gppDFMZertXuXYz9M4);
}
