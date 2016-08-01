{
//=========Macro generated from canvas: Canvas_PnuTSMC9v8FCgzXKRwM9M8/
//=========  (Mon Feb 29 14:02:33 2016) by ROOT version5.34/28
   TCanvas *Canvas_PnuTSMC9v8FCgzXKRwM9M8 = new TCanvas("Canvas_PnuTSMC9v8FCgzXKRwM9M8", "",0,0,700,500);
   Canvas_PnuTSMC9v8FCgzXKRwM9M8->SetHighLightColor(2);
   Canvas_PnuTSMC9v8FCgzXKRwM9M8->Range(-0.25,-199.9641,2.25,1799.677);
   Canvas_PnuTSMC9v8FCgzXKRwM9M8->SetFillColor(0);
   Canvas_PnuTSMC9v8FCgzXKRwM9M8->SetBorderMode(0);
   Canvas_PnuTSMC9v8FCgzXKRwM9M8->SetBorderSize(2);
   Canvas_PnuTSMC9v8FCgzXKRwM9M8->SetFrameBorderMode(0);
   Canvas_PnuTSMC9v8FCgzXKRwM9M8->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(4,30);
   h_res_s1->SetBinContent(5,5);
   h_res_s1->SetBinContent(6,35);
   h_res_s1->SetBinContent(7,76);
   h_res_s1->SetBinContent(8,110);
   h_res_s1->SetBinContent(9,236);
   h_res_s1->SetBinContent(10,254);
   h_res_s1->SetBinContent(11,453);
   h_res_s1->SetBinContent(12,772);
   h_res_s1->SetBinContent(13,1068);
   h_res_s1->SetBinContent(14,1485);
   h_res_s1->SetBinContent(15,1481);
   h_res_s1->SetBinContent(16,1371);
   h_res_s1->SetBinContent(17,1089);
   h_res_s1->SetBinContent(18,683);
   h_res_s1->SetBinContent(19,444);
   h_res_s1->SetBinContent(20,230);
   h_res_s1->SetBinContent(21,238);
   h_res_s1->SetBinContent(22,57);
   h_res_s1->SetBinContent(23,85);
   h_res_s1->SetBinContent(24,18);
   h_res_s1->SetBinContent(25,10);
   h_res_s1->SetBinContent(26,5);
   h_res_s1->SetBinContent(27,5);
   h_res_s1->SetBinError(4,5.477226);
   h_res_s1->SetBinError(5,2.236068);
   h_res_s1->SetBinError(6,5.91608);
   h_res_s1->SetBinError(7,8.717798);
   h_res_s1->SetBinError(8,10.48809);
   h_res_s1->SetBinError(9,15.36229);
   h_res_s1->SetBinError(10,15.93738);
   h_res_s1->SetBinError(11,21.2838);
   h_res_s1->SetBinError(12,27.78489);
   h_res_s1->SetBinError(13,32.68027);
   h_res_s1->SetBinError(14,38.5357);
   h_res_s1->SetBinError(15,38.48376);
   h_res_s1->SetBinError(16,37.02702);
   h_res_s1->SetBinError(17,33);
   h_res_s1->SetBinError(18,26.13427);
   h_res_s1->SetBinError(19,21.07131);
   h_res_s1->SetBinError(20,15.16575);
   h_res_s1->SetBinError(21,15.42725);
   h_res_s1->SetBinError(22,7.549834);
   h_res_s1->SetBinError(23,9.219544);
   h_res_s1->SetBinError(24,4.242641);
   h_res_s1->SetBinError(25,3.162278);
   h_res_s1->SetBinError(26,2.236068);
   h_res_s1->SetBinError(27,2.236068);
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
   TLine *line = new TLine(0.2311181,0,0.2311181,1485);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2880486,0,0.2880486,1485);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3442999,0,0.3442999,1485);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3,0,0.3,1485);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.29^{-0.06}_{+0.06} ");
   pt->Draw();
   Canvas_PnuTSMC9v8FCgzXKRwM9M8->Modified();
   Canvas_PnuTSMC9v8FCgzXKRwM9M8->cd();
   Canvas_PnuTSMC9v8FCgzXKRwM9M8->SetSelected(Canvas_PnuTSMC9v8FCgzXKRwM9M8);
}
