{
//=========Macro generated from canvas: 820be06a2e9743ba830cd2a2223b885d/
//=========  (Mon Mar 21 14:49:26 2016) by ROOT version5.34/26
   TCanvas *820be06a2e9743ba830cd2a2223b885d = new TCanvas("820be06a2e9743ba830cd2a2223b885d", "",10,45,704,527);
   820be06a2e9743ba830cd2a2223b885d->SetHighLightColor(2);
   820be06a2e9743ba830cd2a2223b885d->Range(-6.875,0.721626,11.875,5.397072);
   820be06a2e9743ba830cd2a2223b885d->SetFillColor(0);
   820be06a2e9743ba830cd2a2223b885d->SetBorderMode(0);
   820be06a2e9743ba830cd2a2223b885d->SetBorderSize(2);
   820be06a2e9743ba830cd2a2223b885d->SetLogy();
   820be06a2e9743ba830cd2a2223b885d->SetFrameBorderMode(0);
   820be06a2e9743ba830cd2a2223b885d->SetFrameBorderMode(0);
   
   TH1F *hGaus = new TH1F("hGaus","Time between Trigger and First S1 in Waveform - nerix_160209_0955",15,-5,10);
   hGaus->SetBinContent(0,89376);
   hGaus->SetBinContent(1,97);
   hGaus->SetBinContent(2,111);
   hGaus->SetBinContent(3,103);
   hGaus->SetBinContent(4,133);
   hGaus->SetBinContent(5,155);
   hGaus->SetBinContent(6,297);
   hGaus->SetBinContent(7,6116);
   hGaus->SetBinContent(8,44661);
   hGaus->SetBinContent(9,8721);
   hGaus->SetBinContent(10,559);
   hGaus->SetBinContent(11,127);
   hGaus->SetBinContent(12,69);
   hGaus->SetBinContent(13,51);
   hGaus->SetBinContent(14,37);
   hGaus->SetBinContent(15,49);
   hGaus->SetBinContent(16,896926);
   hGaus->SetBinError(0,298.9582);
   hGaus->SetBinError(1,9.848858);
   hGaus->SetBinError(2,10.53565);
   hGaus->SetBinError(3,10.14889);
   hGaus->SetBinError(4,11.53256);
   hGaus->SetBinError(5,12.4499);
   hGaus->SetBinError(6,17.23369);
   hGaus->SetBinError(7,78.20486);
   hGaus->SetBinError(8,211.3315);
   hGaus->SetBinError(9,93.38629);
   hGaus->SetBinError(10,23.64318);
   hGaus->SetBinError(11,11.26943);
   hGaus->SetBinError(12,8.306624);
   hGaus->SetBinError(13,7.141428);
   hGaus->SetBinError(14,6.082763);
   hGaus->SetBinError(15,7);
   hGaus->SetBinError(16,947.0618);
   hGaus->SetEntries(1047588);
   hGaus->SetStats(0);
   hGaus->SetFillStyle(0);
   hGaus->SetMarkerStyle(20);
   hGaus->SetMarkerSize(0);
   hGaus->GetXaxis()->SetTitle("TrigLeftEdge[] - S1sPeak[S1Order[0]]");
   hGaus->GetXaxis()->SetLabelFont(42);
   hGaus->GetXaxis()->SetLabelSize(0.035);
   hGaus->GetXaxis()->SetTitleSize(0.035);
   hGaus->GetXaxis()->SetTitleFont(42);
   hGaus->GetYaxis()->SetTitle("Counts");
   hGaus->GetYaxis()->SetLabelFont(42);
   hGaus->GetYaxis()->SetLabelSize(0.035);
   hGaus->GetYaxis()->SetTitleSize(0.035);
   hGaus->GetYaxis()->SetTitleFont(42);
   hGaus->GetZaxis()->SetLabelFont(42);
   hGaus->GetZaxis()->SetLabelSize(0.035);
   hGaus->GetZaxis()->SetTitleSize(0.035);
   hGaus->GetZaxis()->SetTitleFont(42);
   hGaus->Draw("");
   
   TPaveText *pt = new TPaveText(0.15,0.9374,0.85,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Time between Trigger and First S1 in Waveform - nerix_160209_0955");
   pt->Draw();
   TLine *line = new TLine(0,37,0,44661);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(6,37,6,44661);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   820be06a2e9743ba830cd2a2223b885d->Modified();
   820be06a2e9743ba830cd2a2223b885d->cd();
   820be06a2e9743ba830cd2a2223b885d->SetSelected(820be06a2e9743ba830cd2a2223b885d);
}
