{
//=========Macro generated from canvas: e89362edbb0544f4809828b25d0de2e5/
//=========  (Wed Feb  3 15:51:30 2016) by ROOT version5.34/26
   TCanvas *e89362edbb0544f4809828b25d0de2e5 = new TCanvas("e89362edbb0544f4809828b25d0de2e5", "",10,45,704,527);
   e89362edbb0544f4809828b25d0de2e5->SetHighLightColor(2);
   e89362edbb0544f4809828b25d0de2e5->Range(-6.875,0.7817923,11.875,5.343938);
   e89362edbb0544f4809828b25d0de2e5->SetFillColor(0);
   e89362edbb0544f4809828b25d0de2e5->SetBorderMode(0);
   e89362edbb0544f4809828b25d0de2e5->SetBorderSize(2);
   e89362edbb0544f4809828b25d0de2e5->SetLogy();
   e89362edbb0544f4809828b25d0de2e5->SetFrameBorderMode(0);
   e89362edbb0544f4809828b25d0de2e5->SetFrameBorderMode(0);
   
   TH1F *hGaus = new TH1F("hGaus","Time between Trigger and First S1 in Waveform - nerix_160127_1300",15,-5,10);
   hGaus->SetBinContent(0,99095);
   hGaus->SetBinContent(1,120);
   hGaus->SetBinContent(2,118);
   hGaus->SetBinContent(3,134);
   hGaus->SetBinContent(4,159);
   hGaus->SetBinContent(5,258);
   hGaus->SetBinContent(6,2689);
   hGaus->SetBinContent(7,40553);
   hGaus->SetBinContent(8,15242);
   hGaus->SetBinContent(9,825);
   hGaus->SetBinContent(10,133);
   hGaus->SetBinContent(11,75);
   hGaus->SetBinContent(12,48);
   hGaus->SetBinContent(13,56);
   hGaus->SetBinContent(14,41);
   hGaus->SetBinContent(15,41);
   hGaus->SetBinContent(16,1092959);
   hGaus->SetBinError(0,314.7936);
   hGaus->SetBinError(1,10.95445);
   hGaus->SetBinError(2,10.86278);
   hGaus->SetBinError(3,11.57584);
   hGaus->SetBinError(4,12.60952);
   hGaus->SetBinError(5,16.06238);
   hGaus->SetBinError(6,51.85557);
   hGaus->SetBinError(7,201.3778);
   hGaus->SetBinError(8,123.4585);
   hGaus->SetBinError(9,28.72281);
   hGaus->SetBinError(10,11.53256);
   hGaus->SetBinError(11,8.660254);
   hGaus->SetBinError(12,6.928203);
   hGaus->SetBinError(13,7.483315);
   hGaus->SetBinError(14,6.403124);
   hGaus->SetBinError(15,6.403124);
   hGaus->SetBinError(16,1045.447);
   hGaus->SetEntries(1252546);
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
   TText *text = pt->AddText("Time between Trigger and First S1 in Waveform - nerix_160127_1300");
   pt->Draw();
   TLine *line = new TLine(0,41,0,40553);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(6,41,6,40553);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   e89362edbb0544f4809828b25d0de2e5->Modified();
   e89362edbb0544f4809828b25d0de2e5->cd();
   e89362edbb0544f4809828b25d0de2e5->SetSelected(e89362edbb0544f4809828b25d0de2e5);
}
