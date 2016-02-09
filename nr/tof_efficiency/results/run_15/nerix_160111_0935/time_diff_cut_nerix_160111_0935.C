{
//=========Macro generated from canvas: 83c17d5342184191b404ac570e34b907/
//=========  (Wed Feb  3 15:48:42 2016) by ROOT version5.34/26
   TCanvas *83c17d5342184191b404ac570e34b907 = new TCanvas("83c17d5342184191b404ac570e34b907", "",10,45,704,527);
   83c17d5342184191b404ac570e34b907->SetHighLightColor(2);
   83c17d5342184191b404ac570e34b907->Range(-6.875,0.8396612,11.875,5.367567);
   83c17d5342184191b404ac570e34b907->SetFillColor(0);
   83c17d5342184191b404ac570e34b907->SetBorderMode(0);
   83c17d5342184191b404ac570e34b907->SetBorderSize(2);
   83c17d5342184191b404ac570e34b907->SetLogy();
   83c17d5342184191b404ac570e34b907->SetFrameBorderMode(0);
   83c17d5342184191b404ac570e34b907->SetFrameBorderMode(0);
   
   TH1F *hGaus = new TH1F("hGaus","Time between Trigger and First S1 in Waveform - nerix_160111_0935",15,-5,10);
   hGaus->SetBinContent(0,121371);
   hGaus->SetBinContent(1,106);
   hGaus->SetBinContent(2,112);
   hGaus->SetBinContent(3,127);
   hGaus->SetBinContent(4,145);
   hGaus->SetBinContent(5,328);
   hGaus->SetBinContent(6,2998);
   hGaus->SetBinContent(7,43166);
   hGaus->SetBinContent(8,18257);
   hGaus->SetBinContent(9,815);
   hGaus->SetBinContent(10,144);
   hGaus->SetBinContent(11,89);
   hGaus->SetBinContent(12,65);
   hGaus->SetBinContent(13,51);
   hGaus->SetBinContent(14,47);
   hGaus->SetBinContent(15,46);
   hGaus->SetBinContent(16,906113);
   hGaus->SetBinError(0,348.3834);
   hGaus->SetBinError(1,10.29563);
   hGaus->SetBinError(2,10.58301);
   hGaus->SetBinError(3,11.26943);
   hGaus->SetBinError(4,12.04159);
   hGaus->SetBinError(5,18.11077);
   hGaus->SetBinError(6,54.754);
   hGaus->SetBinError(7,207.7643);
   hGaus->SetBinError(8,135.1185);
   hGaus->SetBinError(9,28.5482);
   hGaus->SetBinError(10,12);
   hGaus->SetBinError(11,9.433981);
   hGaus->SetBinError(12,8.062258);
   hGaus->SetBinError(13,7.141428);
   hGaus->SetBinError(14,6.855655);
   hGaus->SetBinError(15,6.78233);
   hGaus->SetBinError(16,951.8997);
   hGaus->SetEntries(1093980);
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
   TText *text = pt->AddText("Time between Trigger and First S1 in Waveform - nerix_160111_0935");
   pt->Draw();
   TLine *line = new TLine(0,46,0,43166);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(6,46,6,43166);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   83c17d5342184191b404ac570e34b907->Modified();
   83c17d5342184191b404ac570e34b907->cd();
   83c17d5342184191b404ac570e34b907->SetSelected(83c17d5342184191b404ac570e34b907);
}
