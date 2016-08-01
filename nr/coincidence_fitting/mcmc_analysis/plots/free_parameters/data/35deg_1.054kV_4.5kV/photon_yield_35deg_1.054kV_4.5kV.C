{
//=========Macro generated from canvas: 338aaa0b8d6043c28571f48b893d63f6/
//=========  (Tue Feb 23 11:19:35 2016) by ROOT version5.34/26
   TCanvas *338aaa0b8d6043c28571f48b893d63f6 = new TCanvas("338aaa0b8d6043c28571f48b893d63f6", "",658,141,704,527);
   338aaa0b8d6043c28571f48b893d63f6->SetHighLightColor(2);
   338aaa0b8d6043c28571f48b893d63f6->Range(2.5,-224.1526,17.5,2017.373);
   338aaa0b8d6043c28571f48b893d63f6->SetFillColor(0);
   338aaa0b8d6043c28571f48b893d63f6->SetBorderMode(0);
   338aaa0b8d6043c28571f48b893d63f6->SetBorderSize(2);
   338aaa0b8d6043c28571f48b893d63f6->SetFrameBorderMode(0);
   338aaa0b8d6043c28571f48b893d63f6->SetFrameBorderMode(0);
   
   TH1F *h_photon_yield = new TH1F("h_photon_yield","photon_yield",100,4,16);
   h_photon_yield->SetBinContent(76,5);
   h_photon_yield->SetBinContent(77,17);
   h_photon_yield->SetBinContent(78,34);
   h_photon_yield->SetBinContent(79,48);
   h_photon_yield->SetBinContent(80,79);
   h_photon_yield->SetBinContent(81,172);
   h_photon_yield->SetBinContent(82,255);
   h_photon_yield->SetBinContent(83,231);
   h_photon_yield->SetBinContent(84,428);
   h_photon_yield->SetBinContent(85,602);
   h_photon_yield->SetBinContent(86,628);
   h_photon_yield->SetBinContent(87,844);
   h_photon_yield->SetBinContent(88,1029);
   h_photon_yield->SetBinContent(89,1368);
   h_photon_yield->SetBinContent(90,1667);
   h_photon_yield->SetBinContent(91,1647);
   h_photon_yield->SetBinContent(92,1186);
   h_photon_yield->SetBinError(76,2.236068);
   h_photon_yield->SetBinError(77,4.123106);
   h_photon_yield->SetBinError(78,5.830952);
   h_photon_yield->SetBinError(79,6.928203);
   h_photon_yield->SetBinError(80,8.888194);
   h_photon_yield->SetBinError(81,13.11488);
   h_photon_yield->SetBinError(82,15.96872);
   h_photon_yield->SetBinError(83,15.19868);
   h_photon_yield->SetBinError(84,20.68816);
   h_photon_yield->SetBinError(85,24.53569);
   h_photon_yield->SetBinError(86,25.05993);
   h_photon_yield->SetBinError(87,29.05168);
   h_photon_yield->SetBinError(88,32.07803);
   h_photon_yield->SetBinError(89,36.98648);
   h_photon_yield->SetBinError(90,40.82891);
   h_photon_yield->SetBinError(91,40.58325);
   h_photon_yield->SetBinError(92,34.43835);
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
   TLine *line = new TLine(14.15159,0,14.15159,1667);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(14.62561,0,14.62561,1667);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(14.88778,0,14.88778,1667);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("P_{y} = 14.63^{-0.47}_{+0.26} #frac{photons}{keV}");
   pt->Draw();
   338aaa0b8d6043c28571f48b893d63f6->Modified();
   338aaa0b8d6043c28571f48b893d63f6->cd();
   338aaa0b8d6043c28571f48b893d63f6->SetSelected(338aaa0b8d6043c28571f48b893d63f6);
}
