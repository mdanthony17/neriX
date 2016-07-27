{
//=========Macro generated from canvas: f3eee03a28064f3bb77a5597a439932b/
//=========  (Tue Feb 23 11:22:39 2016) by ROOT version5.34/26
   TCanvas *f3eee03a28064f3bb77a5597a439932b = new TCanvas("f3eee03a28064f3bb77a5597a439932b", "",842,169,704,527);
   f3eee03a28064f3bb77a5597a439932b->SetHighLightColor(2);
   f3eee03a28064f3bb77a5597a439932b->Range(2.5,-150.0603,17.5,1350.543);
   f3eee03a28064f3bb77a5597a439932b->SetFillColor(0);
   f3eee03a28064f3bb77a5597a439932b->SetBorderMode(0);
   f3eee03a28064f3bb77a5597a439932b->SetBorderSize(2);
   f3eee03a28064f3bb77a5597a439932b->SetFrameBorderMode(0);
   f3eee03a28064f3bb77a5597a439932b->SetFrameBorderMode(0);
   
   TH1F *h_photon_yield = new TH1F("h_photon_yield","photon_yield",100,4,16);
   h_photon_yield->SetBinContent(53,20);
   h_photon_yield->SetBinContent(54,25);
   h_photon_yield->SetBinContent(55,60);
   h_photon_yield->SetBinContent(56,70);
   h_photon_yield->SetBinContent(57,114);
   h_photon_yield->SetBinContent(58,193);
   h_photon_yield->SetBinContent(59,287);
   h_photon_yield->SetBinContent(60,451);
   h_photon_yield->SetBinContent(61,608);
   h_photon_yield->SetBinContent(62,765);
   h_photon_yield->SetBinContent(63,923);
   h_photon_yield->SetBinContent(64,1110);
   h_photon_yield->SetBinContent(65,1050);
   h_photon_yield->SetBinContent(66,972);
   h_photon_yield->SetBinContent(67,759);
   h_photon_yield->SetBinContent(68,756);
   h_photon_yield->SetBinContent(69,643);
   h_photon_yield->SetBinContent(70,444);
   h_photon_yield->SetBinContent(71,335);
   h_photon_yield->SetBinContent(72,226);
   h_photon_yield->SetBinContent(73,112);
   h_photon_yield->SetBinContent(74,95);
   h_photon_yield->SetBinContent(75,75);
   h_photon_yield->SetBinContent(76,72);
   h_photon_yield->SetBinContent(77,30);
   h_photon_yield->SetBinContent(78,25);
   h_photon_yield->SetBinContent(80,10);
   h_photon_yield->SetBinContent(81,5);
   h_photon_yield->SetBinContent(83,5);
   h_photon_yield->SetBinError(53,4.472136);
   h_photon_yield->SetBinError(54,5);
   h_photon_yield->SetBinError(55,7.745967);
   h_photon_yield->SetBinError(56,8.3666);
   h_photon_yield->SetBinError(57,10.67708);
   h_photon_yield->SetBinError(58,13.89244);
   h_photon_yield->SetBinError(59,16.94107);
   h_photon_yield->SetBinError(60,21.23676);
   h_photon_yield->SetBinError(61,24.65766);
   h_photon_yield->SetBinError(62,27.65863);
   h_photon_yield->SetBinError(63,30.38092);
   h_photon_yield->SetBinError(64,33.31666);
   h_photon_yield->SetBinError(65,32.4037);
   h_photon_yield->SetBinError(66,31.17691);
   h_photon_yield->SetBinError(67,27.54995);
   h_photon_yield->SetBinError(68,27.49545);
   h_photon_yield->SetBinError(69,25.35744);
   h_photon_yield->SetBinError(70,21.07131);
   h_photon_yield->SetBinError(71,18.30301);
   h_photon_yield->SetBinError(72,15.0333);
   h_photon_yield->SetBinError(73,10.58301);
   h_photon_yield->SetBinError(74,9.746794);
   h_photon_yield->SetBinError(75,8.660254);
   h_photon_yield->SetBinError(76,8.485281);
   h_photon_yield->SetBinError(77,5.477226);
   h_photon_yield->SetBinError(78,5);
   h_photon_yield->SetBinError(80,3.162278);
   h_photon_yield->SetBinError(81,2.236068);
   h_photon_yield->SetBinError(83,2.236068);
   h_photon_yield->SetEntries(10240);
   h_photon_yield->SetStats(0);
   h_photon_yield->SetFillStyle(0);
   h_photon_yield->SetMarkerStyle(20);
   h_photon_yield->SetMarkerSize(0);
   h_photon_yield->GetXaxis()->SetLabelFont(42);
   h_photon_yield->GetXaxis()->SetLabelSize(0.035);
   h_photon_yield->GetXaxis()->SetTitleSize(0.035);
   h_photon_yield->GetXaxis()->SetTitleFont(42);
   h_photon_yield->GetYaxis()->SetLabelFont(42);
   h_photon_yield->GetYaxis()->SetLabelSize(0.035);
   h_photon_yield->GetYaxis()->SetTitleSize(0.035);
   h_photon_yield->GetYaxis()->SetTitleFont(42);
   h_photon_yield->GetZaxis()->SetLabelFont(42);
   h_photon_yield->GetZaxis()->SetLabelSize(0.035);
   h_photon_yield->GetZaxis()->SetTitleSize(0.035);
   h_photon_yield->GetZaxis()->SetTitleFont(42);
   h_photon_yield->Draw("");
   
   TPaveText *pt = new TPaveText(0.3792857,0.9374,0.6207143,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("photon_yield");
   pt->Draw();
   TLine *line = new TLine(11.28056,0,11.28056,1110);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(11.73646,0,11.73646,1110);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(12.24376,0,12.24376,1110);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("P_{y} = 11.74^{-0.46}_{+0.51} #frac{photons}{keV}");
   pt->Draw();
   f3eee03a28064f3bb77a5597a439932b->Modified();
   f3eee03a28064f3bb77a5597a439932b->cd();
   f3eee03a28064f3bb77a5597a439932b->SetSelected(f3eee03a28064f3bb77a5597a439932b);
}
