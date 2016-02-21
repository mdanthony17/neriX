{
//=========Macro generated from canvas: 513ad02413f246a4931dd8a6fe1fc76d/
//=========  (Fri Feb 19 17:22:56 2016) by ROOT version5.34/26
   TCanvas *513ad02413f246a4931dd8a6fe1fc76d = new TCanvas("513ad02413f246a4931dd8a6fe1fc76d", "",10,45,704,527);
   513ad02413f246a4931dd8a6fe1fc76d->SetHighLightColor(2);
   513ad02413f246a4931dd8a6fe1fc76d->Range(-6.875,1.042539,11.875,5.231832);
   513ad02413f246a4931dd8a6fe1fc76d->SetFillColor(0);
   513ad02413f246a4931dd8a6fe1fc76d->SetBorderMode(0);
   513ad02413f246a4931dd8a6fe1fc76d->SetBorderSize(2);
   513ad02413f246a4931dd8a6fe1fc76d->SetLogy();
   513ad02413f246a4931dd8a6fe1fc76d->SetFrameBorderMode(0);
   513ad02413f246a4931dd8a6fe1fc76d->SetFrameBorderMode(0);
   
   TH1F *hGaus = new TH1F("hGaus","Time between Trigger and First S1 in Waveform - nerix_160219_1433",15,-5,10);
   hGaus->SetBinContent(0,386804);
   hGaus->SetBinContent(1,263);
   hGaus->SetBinContent(2,279);
   hGaus->SetBinContent(3,336);
   hGaus->SetBinContent(4,354);
   hGaus->SetBinContent(5,475);
   hGaus->SetBinContent(6,2543);
   hGaus->SetBinContent(7,34120);
   hGaus->SetBinContent(8,14183);
   hGaus->SetBinContent(9,732);
   hGaus->SetBinContent(10,157);
   hGaus->SetBinContent(11,118);
   hGaus->SetBinContent(12,67);
   hGaus->SetBinContent(13,73);
   hGaus->SetBinContent(14,71);
   hGaus->SetBinContent(15,66);
   hGaus->SetBinContent(16,991525);
   hGaus->SetBinError(0,621.9357);
   hGaus->SetBinError(1,16.21727);
   hGaus->SetBinError(2,16.70329);
   hGaus->SetBinError(3,18.3303);
   hGaus->SetBinError(4,18.81489);
   hGaus->SetBinError(5,21.79449);
   hGaus->SetBinError(6,50.42817);
   hGaus->SetBinError(7,184.716);
   hGaus->SetBinError(8,119.0924);
   hGaus->SetBinError(9,27.0555);
   hGaus->SetBinError(10,12.52996);
   hGaus->SetBinError(11,10.86278);
   hGaus->SetBinError(12,8.185353);
   hGaus->SetBinError(13,8.544004);
   hGaus->SetBinError(14,8.42615);
   hGaus->SetBinError(15,8.124038);
   hGaus->SetBinError(16,995.7535);
   hGaus->SetEntries(1432166);
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
   TText *text = pt->AddText("Time between Trigger and First S1 in Waveform - nerix_160219_1433");
   pt->Draw();
   TLine *line = new TLine(0,66,0,34120);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(6,66,6,34120);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   513ad02413f246a4931dd8a6fe1fc76d->Modified();
   513ad02413f246a4931dd8a6fe1fc76d->cd();
   513ad02413f246a4931dd8a6fe1fc76d->SetSelected(513ad02413f246a4931dd8a6fe1fc76d);
}
