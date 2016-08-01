{
//=========Macro generated from canvas: Canvas_RY4qShbQghT8PeXC3SzwB8/
//=========  (Mon Feb 29 14:03:35 2016) by ROOT version5.34/28
   TCanvas *Canvas_RY4qShbQghT8PeXC3SzwB8 = new TCanvas("Canvas_RY4qShbQghT8PeXC3SzwB8", "",0,0,700,500);
   Canvas_RY4qShbQghT8PeXC3SzwB8->SetHighLightColor(2);
   Canvas_RY4qShbQghT8PeXC3SzwB8->Range(-0.25,-129.6665,2.25,1166.999);
   Canvas_RY4qShbQghT8PeXC3SzwB8->SetFillColor(0);
   Canvas_RY4qShbQghT8PeXC3SzwB8->SetBorderMode(0);
   Canvas_RY4qShbQghT8PeXC3SzwB8->SetBorderSize(2);
   Canvas_RY4qShbQghT8PeXC3SzwB8->SetFrameBorderMode(0);
   Canvas_RY4qShbQghT8PeXC3SzwB8->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,532);
   h_res_s2->SetBinContent(2,601);
   h_res_s2->SetBinContent(3,606);
   h_res_s2->SetBinContent(4,589);
   h_res_s2->SetBinContent(5,665);
   h_res_s2->SetBinContent(6,957);
   h_res_s2->SetBinContent(7,787);
   h_res_s2->SetBinContent(8,931);
   h_res_s2->SetBinContent(9,851);
   h_res_s2->SetBinContent(10,837);
   h_res_s2->SetBinContent(11,801);
   h_res_s2->SetBinContent(12,713);
   h_res_s2->SetBinContent(13,453);
   h_res_s2->SetBinContent(14,397);
   h_res_s2->SetBinContent(15,206);
   h_res_s2->SetBinContent(16,159);
   h_res_s2->SetBinContent(17,75);
   h_res_s2->SetBinContent(18,46);
   h_res_s2->SetBinContent(19,17);
   h_res_s2->SetBinContent(20,13);
   h_res_s2->SetBinContent(21,4);
   h_res_s2->SetBinError(1,23.06513);
   h_res_s2->SetBinError(2,24.5153);
   h_res_s2->SetBinError(3,24.61707);
   h_res_s2->SetBinError(4,24.26932);
   h_res_s2->SetBinError(5,25.78759);
   h_res_s2->SetBinError(6,30.93542);
   h_res_s2->SetBinError(7,28.05352);
   h_res_s2->SetBinError(8,30.51229);
   h_res_s2->SetBinError(9,29.1719);
   h_res_s2->SetBinError(10,28.93095);
   h_res_s2->SetBinError(11,28.30194);
   h_res_s2->SetBinError(12,26.70206);
   h_res_s2->SetBinError(13,21.2838);
   h_res_s2->SetBinError(14,19.92486);
   h_res_s2->SetBinError(15,14.3527);
   h_res_s2->SetBinError(16,12.60952);
   h_res_s2->SetBinError(17,8.660254);
   h_res_s2->SetBinError(18,6.78233);
   h_res_s2->SetBinError(19,4.123106);
   h_res_s2->SetBinError(20,3.605551);
   h_res_s2->SetBinError(21,2);
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
   TLine *line = new TLine(0.05634191,0,0.05634191,957);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1482277,0,0.1482277,957);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2327585,0,0.2327585,957);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,957);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.15^{-0.09}_{+0.08} ");
   pt->Draw();
   Canvas_RY4qShbQghT8PeXC3SzwB8->Modified();
   Canvas_RY4qShbQghT8PeXC3SzwB8->cd();
   Canvas_RY4qShbQghT8PeXC3SzwB8->SetSelected(Canvas_RY4qShbQghT8PeXC3SzwB8);
}
