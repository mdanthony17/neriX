{
//=========Macro generated from canvas: Canvas_Aa4qZH9fcZQfPNu7rojJoW/
//=========  (Fri Jul  8 05:16:17 2016) by ROOT version5.34/30
   TCanvas *Canvas_Aa4qZH9fcZQfPNu7rojJoW = new TCanvas("Canvas_Aa4qZH9fcZQfPNu7rojJoW", "",220,147,704,528);
   Canvas_Aa4qZH9fcZQfPNu7rojJoW->SetHighLightColor(2);
   Canvas_Aa4qZH9fcZQfPNu7rojJoW->Range(2.125,-712.5805,10.875,6413.224);
   Canvas_Aa4qZH9fcZQfPNu7rojJoW->SetFillColor(0);
   Canvas_Aa4qZH9fcZQfPNu7rojJoW->SetBorderMode(0);
   Canvas_Aa4qZH9fcZQfPNu7rojJoW->SetBorderSize(2);
   Canvas_Aa4qZH9fcZQfPNu7rojJoW->SetFrameBorderMode(0);
   Canvas_Aa4qZH9fcZQfPNu7rojJoW->SetFrameBorderMode(0);
   
   TH1F *h_charge_yield = new TH1F("h_charge_yield","charge_yield",100,3,10);
   h_charge_yield->SetBinContent(30,4);
   h_charge_yield->SetBinContent(32,65);
   h_charge_yield->SetBinContent(33,104);
   h_charge_yield->SetBinContent(34,272);
   h_charge_yield->SetBinContent(35,520);
   h_charge_yield->SetBinContent(36,796);
   h_charge_yield->SetBinContent(37,1706);
   h_charge_yield->SetBinContent(38,2571);
   h_charge_yield->SetBinContent(39,3512);
   h_charge_yield->SetBinContent(40,4751);
   h_charge_yield->SetBinContent(41,5300);
   h_charge_yield->SetBinContent(42,5224);
   h_charge_yield->SetBinContent(43,5356);
   h_charge_yield->SetBinContent(44,4909);
   h_charge_yield->SetBinContent(45,4046);
   h_charge_yield->SetBinContent(46,3449);
   h_charge_yield->SetBinContent(47,2651);
   h_charge_yield->SetBinContent(48,2044);
   h_charge_yield->SetBinContent(49,1371);
   h_charge_yield->SetBinContent(50,1022);
   h_charge_yield->SetBinContent(51,690);
   h_charge_yield->SetBinContent(52,406);
   h_charge_yield->SetBinContent(53,214);
   h_charge_yield->SetBinContent(54,83);
   h_charge_yield->SetBinContent(55,92);
   h_charge_yield->SetBinContent(56,28);
   h_charge_yield->SetBinContent(57,10);
   h_charge_yield->SetBinContent(58,4);
   h_charge_yield->SetBinError(30,2);
   h_charge_yield->SetBinError(32,8.062258);
   h_charge_yield->SetBinError(33,10.19804);
   h_charge_yield->SetBinError(34,16.49242);
   h_charge_yield->SetBinError(35,22.80351);
   h_charge_yield->SetBinError(36,28.21347);
   h_charge_yield->SetBinError(37,41.30375);
   h_charge_yield->SetBinError(38,50.70503);
   h_charge_yield->SetBinError(39,59.26213);
   h_charge_yield->SetBinError(40,68.9275);
   h_charge_yield->SetBinError(41,72.8011);
   h_charge_yield->SetBinError(42,72.27724);
   h_charge_yield->SetBinError(43,73.1847);
   h_charge_yield->SetBinError(44,70.06426);
   h_charge_yield->SetBinError(45,63.60818);
   h_charge_yield->SetBinError(46,58.72819);
   h_charge_yield->SetBinError(47,51.48786);
   h_charge_yield->SetBinError(48,45.21062);
   h_charge_yield->SetBinError(49,37.02702);
   h_charge_yield->SetBinError(50,31.96873);
   h_charge_yield->SetBinError(51,26.26785);
   h_charge_yield->SetBinError(52,20.14944);
   h_charge_yield->SetBinError(53,14.62874);
   h_charge_yield->SetBinError(54,9.110434);
   h_charge_yield->SetBinError(55,9.591663);
   h_charge_yield->SetBinError(56,5.291503);
   h_charge_yield->SetBinError(57,3.162278);
   h_charge_yield->SetBinError(58,2);
   h_charge_yield->SetEntries(51200);
   h_charge_yield->SetStats(0);
   h_charge_yield->SetFillStyle(0);
   h_charge_yield->SetMarkerStyle(20);
   h_charge_yield->SetMarkerSize(0);
   h_charge_yield->GetXaxis()->SetLabelFont(42);
   h_charge_yield->GetXaxis()->SetLabelSize(0.035);
   h_charge_yield->GetXaxis()->SetTitleSize(0.035);
   h_charge_yield->GetXaxis()->SetTitleFont(42);
   h_charge_yield->GetYaxis()->SetLabelFont(42);
   h_charge_yield->GetYaxis()->SetLabelSize(0.035);
   h_charge_yield->GetYaxis()->SetTitleSize(0.035);
   h_charge_yield->GetYaxis()->SetTitleFont(42);
   h_charge_yield->GetZaxis()->SetLabelFont(42);
   h_charge_yield->GetZaxis()->SetLabelSize(0.035);
   h_charge_yield->GetZaxis()->SetTitleSize(0.035);
   h_charge_yield->GetZaxis()->SetTitleFont(42);
   h_charge_yield->Draw("");
   
   TPaveText *pt = new TPaveText(0.3771429,0.935,0.6228571,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("charge_yield");
   pt->Draw();
   TLine *line = new TLine(5.701912,0,5.701912,5356);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(5.950129,0,5.950129,5356);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(6.232521,0,6.232521,5356);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("Q_{y} = 5.95^{-0.25}_{+0.28} #frac{electrons}{keV}");
   pt->Draw();
   Canvas_Aa4qZH9fcZQfPNu7rojJoW->Modified();
   Canvas_Aa4qZH9fcZQfPNu7rojJoW->cd();
   Canvas_Aa4qZH9fcZQfPNu7rojJoW->SetSelected(Canvas_Aa4qZH9fcZQfPNu7rojJoW);
}
