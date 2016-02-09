{
//=========Macro generated from canvas: f4840552f7aa46c79dd4dfba953d9d10/
//=========  (Wed Feb  3 15:46:17 2016) by ROOT version5.34/26
   TCanvas *f4840552f7aa46c79dd4dfba953d9d10 = new TCanvas("f4840552f7aa46c79dd4dfba953d9d10", "",10,45,704,527);
   f4840552f7aa46c79dd4dfba953d9d10->SetHighLightColor(2);
   f4840552f7aa46c79dd4dfba953d9d10->Range(-6.875,0.7862023,11.875,5.187013);
   f4840552f7aa46c79dd4dfba953d9d10->SetFillColor(0);
   f4840552f7aa46c79dd4dfba953d9d10->SetBorderMode(0);
   f4840552f7aa46c79dd4dfba953d9d10->SetBorderSize(2);
   f4840552f7aa46c79dd4dfba953d9d10->SetLogy();
   f4840552f7aa46c79dd4dfba953d9d10->SetFrameBorderMode(0);
   f4840552f7aa46c79dd4dfba953d9d10->SetFrameBorderMode(0);
   
   TH1F *hGaus = new TH1F("hGaus","Time between Trigger and First S1 in Waveform - nerix_160105_0939",15,-5,10);
   hGaus->SetBinContent(0,100377);
   hGaus->SetBinContent(1,89);
   hGaus->SetBinContent(2,85);
   hGaus->SetBinContent(3,107);
   hGaus->SetBinContent(4,135);
   hGaus->SetBinContent(5,209);
   hGaus->SetBinContent(6,1989);
   hGaus->SetBinContent(7,29299);
   hGaus->SetBinContent(8,12581);
   hGaus->SetBinContent(9,554);
   hGaus->SetBinContent(10,106);
   hGaus->SetBinContent(11,65);
   hGaus->SetBinContent(12,55);
   hGaus->SetBinContent(13,42);
   hGaus->SetBinContent(14,40);
   hGaus->SetBinContent(15,42);
   hGaus->SetBinContent(16,738148);
   hGaus->SetBinError(0,316.8233);
   hGaus->SetBinError(1,9.433981);
   hGaus->SetBinError(2,9.219544);
   hGaus->SetBinError(3,10.34408);
   hGaus->SetBinError(4,11.61895);
   hGaus->SetBinError(5,14.45683);
   hGaus->SetBinError(6,44.59821);
   hGaus->SetBinError(7,171.1695);
   hGaus->SetBinError(8,112.1651);
   hGaus->SetBinError(9,23.5372);
   hGaus->SetBinError(10,10.29563);
   hGaus->SetBinError(11,8.062258);
   hGaus->SetBinError(12,7.416198);
   hGaus->SetBinError(13,6.480741);
   hGaus->SetBinError(14,6.324555);
   hGaus->SetBinError(15,6.480741);
   hGaus->SetBinError(16,859.1554);
   hGaus->SetEntries(883923);
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
   TText *text = pt->AddText("Time between Trigger and First S1 in Waveform - nerix_160105_0939");
   pt->Draw();
   TLine *line = new TLine(0,40,0,29299);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(6,40,6,29299);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   f4840552f7aa46c79dd4dfba953d9d10->Modified();
   f4840552f7aa46c79dd4dfba953d9d10->cd();
   f4840552f7aa46c79dd4dfba953d9d10->SetSelected(f4840552f7aa46c79dd4dfba953d9d10);
}
