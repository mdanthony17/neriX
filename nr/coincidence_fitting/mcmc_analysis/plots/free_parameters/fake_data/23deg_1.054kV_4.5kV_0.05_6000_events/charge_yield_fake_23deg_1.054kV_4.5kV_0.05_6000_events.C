{
//=========Macro generated from canvas: Canvas_yRGVWKuskP9nZAfMYb6NSa/
//=========  (Mon Feb 29 14:02:33 2016) by ROOT version5.34/28
   TCanvas *Canvas_yRGVWKuskP9nZAfMYb6NSa = new TCanvas("Canvas_yRGVWKuskP9nZAfMYb6NSa", "",0,0,700,500);
   Canvas_yRGVWKuskP9nZAfMYb6NSa->SetHighLightColor(2);
   Canvas_yRGVWKuskP9nZAfMYb6NSa->Range(2.125,-136.6004,10.875,1229.403);
   Canvas_yRGVWKuskP9nZAfMYb6NSa->SetFillColor(0);
   Canvas_yRGVWKuskP9nZAfMYb6NSa->SetBorderMode(0);
   Canvas_yRGVWKuskP9nZAfMYb6NSa->SetBorderSize(2);
   Canvas_yRGVWKuskP9nZAfMYb6NSa->SetFrameBorderMode(0);
   Canvas_yRGVWKuskP9nZAfMYb6NSa->SetFrameBorderMode(0);
   
   TH1F *h_charge_yield = new TH1F("h_charge_yield","charge_yield",100,3,10);
   h_charge_yield->SetBinContent(39,10);
   h_charge_yield->SetBinContent(40,25);
   h_charge_yield->SetBinContent(41,30);
   h_charge_yield->SetBinContent(42,65);
   h_charge_yield->SetBinContent(43,100);
   h_charge_yield->SetBinContent(44,195);
   h_charge_yield->SetBinContent(45,192);
   h_charge_yield->SetBinContent(46,370);
   h_charge_yield->SetBinContent(47,562);
   h_charge_yield->SetBinContent(48,693);
   h_charge_yield->SetBinContent(49,792);
   h_charge_yield->SetBinContent(50,1009);
   h_charge_yield->SetBinContent(51,960);
   h_charge_yield->SetBinContent(52,987);
   h_charge_yield->SetBinContent(53,934);
   h_charge_yield->SetBinContent(54,826);
   h_charge_yield->SetBinContent(55,723);
   h_charge_yield->SetBinContent(56,491);
   h_charge_yield->SetBinContent(57,396);
   h_charge_yield->SetBinContent(58,282);
   h_charge_yield->SetBinContent(59,159);
   h_charge_yield->SetBinContent(60,132);
   h_charge_yield->SetBinContent(61,113);
   h_charge_yield->SetBinContent(62,74);
   h_charge_yield->SetBinContent(63,55);
   h_charge_yield->SetBinContent(64,30);
   h_charge_yield->SetBinContent(65,10);
   h_charge_yield->SetBinContent(67,10);
   h_charge_yield->SetBinContent(70,5);
   h_charge_yield->SetBinContent(71,5);
   h_charge_yield->SetBinContent(73,5);
   h_charge_yield->SetBinError(39,3.162278);
   h_charge_yield->SetBinError(40,5);
   h_charge_yield->SetBinError(41,5.477226);
   h_charge_yield->SetBinError(42,8.062258);
   h_charge_yield->SetBinError(43,10);
   h_charge_yield->SetBinError(44,13.96424);
   h_charge_yield->SetBinError(45,13.85641);
   h_charge_yield->SetBinError(46,19.23538);
   h_charge_yield->SetBinError(47,23.70654);
   h_charge_yield->SetBinError(48,26.32489);
   h_charge_yield->SetBinError(49,28.14249);
   h_charge_yield->SetBinError(50,31.76476);
   h_charge_yield->SetBinError(51,30.98387);
   h_charge_yield->SetBinError(52,31.41656);
   h_charge_yield->SetBinError(53,30.56141);
   h_charge_yield->SetBinError(54,28.74022);
   h_charge_yield->SetBinError(55,26.88866);
   h_charge_yield->SetBinError(56,22.15852);
   h_charge_yield->SetBinError(57,19.89975);
   h_charge_yield->SetBinError(58,16.79286);
   h_charge_yield->SetBinError(59,12.60952);
   h_charge_yield->SetBinError(60,11.48913);
   h_charge_yield->SetBinError(61,10.63015);
   h_charge_yield->SetBinError(62,8.602325);
   h_charge_yield->SetBinError(63,7.416198);
   h_charge_yield->SetBinError(64,5.477226);
   h_charge_yield->SetBinError(65,3.162278);
   h_charge_yield->SetBinError(67,3.162278);
   h_charge_yield->SetBinError(70,2.236068);
   h_charge_yield->SetBinError(71,2.236068);
   h_charge_yield->SetBinError(73,2.236068);
   h_charge_yield->SetEntries(10240);
   h_charge_yield->SetStats(0);
   h_charge_yield->SetFillColor(1);
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
   TLine *line = new TLine(6.297996,0,6.297996,1009);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(6.578298,0,6.578298,1009);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(6.869794,0,6.869794,1009);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(6.78,0,6.78,1009);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("Q_{y} = 6.58^{-0.28}_{+0.29} #frac{electrons}{keV}");
   pt->Draw();
   Canvas_yRGVWKuskP9nZAfMYb6NSa->Modified();
   Canvas_yRGVWKuskP9nZAfMYb6NSa->cd();
   Canvas_yRGVWKuskP9nZAfMYb6NSa->SetSelected(Canvas_yRGVWKuskP9nZAfMYb6NSa);
}
