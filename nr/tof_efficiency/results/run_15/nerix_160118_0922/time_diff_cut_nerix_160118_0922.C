{
//=========Macro generated from canvas: 165aff87ca3d47b592cb9fbe07d858d8/
//=========  (Wed Feb  3 15:50:20 2016) by ROOT version5.34/26
   TCanvas *165aff87ca3d47b592cb9fbe07d858d8 = new TCanvas("165aff87ca3d47b592cb9fbe07d858d8", "",10,45,704,527);
   165aff87ca3d47b592cb9fbe07d858d8->SetHighLightColor(2);
   165aff87ca3d47b592cb9fbe07d858d8->Range(-6.875,0.7658755,11.875,5.369953);
   165aff87ca3d47b592cb9fbe07d858d8->SetFillColor(0);
   165aff87ca3d47b592cb9fbe07d858d8->SetBorderMode(0);
   165aff87ca3d47b592cb9fbe07d858d8->SetBorderSize(2);
   165aff87ca3d47b592cb9fbe07d858d8->SetLogy();
   165aff87ca3d47b592cb9fbe07d858d8->SetFrameBorderMode(0);
   165aff87ca3d47b592cb9fbe07d858d8->SetFrameBorderMode(0);
   
   TH1F *hGaus = new TH1F("hGaus","Time between Trigger and First S1 in Waveform - nerix_160118_0922",15,-5,10);
   hGaus->SetBinContent(0,134594);
   hGaus->SetBinContent(1,137);
   hGaus->SetBinContent(2,126);
   hGaus->SetBinContent(3,167);
   hGaus->SetBinContent(4,212);
   hGaus->SetBinContent(5,361);
   hGaus->SetBinContent(6,3316);
   hGaus->SetBinContent(7,42648);
   hGaus->SetBinContent(8,16005);
   hGaus->SetBinContent(9,783);
   hGaus->SetBinContent(10,151);
   hGaus->SetBinContent(11,78);
   hGaus->SetBinContent(12,63);
   hGaus->SetBinContent(13,47);
   hGaus->SetBinContent(14,45);
   hGaus->SetBinContent(15,40);
   hGaus->SetBinContent(16,915567);
   hGaus->SetBinError(0,366.8705);
   hGaus->SetBinError(1,11.7047);
   hGaus->SetBinError(2,11.22497);
   hGaus->SetBinError(3,12.92285);
   hGaus->SetBinError(4,14.56022);
   hGaus->SetBinError(5,19);
   hGaus->SetBinError(6,57.58472);
   hGaus->SetBinError(7,206.5139);
   hGaus->SetBinError(8,126.5109);
   hGaus->SetBinError(9,27.98214);
   hGaus->SetBinError(10,12.28821);
   hGaus->SetBinError(11,8.831761);
   hGaus->SetBinError(12,7.937254);
   hGaus->SetBinError(13,6.855655);
   hGaus->SetBinError(14,6.708204);
   hGaus->SetBinError(15,6.324555);
   hGaus->SetBinError(16,956.8527);
   hGaus->SetEntries(1114340);
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
   TText *text = pt->AddText("Time between Trigger and First S1 in Waveform - nerix_160118_0922");
   pt->Draw();
   TLine *line = new TLine(0,40,0,42648);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(6,40,6,42648);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   165aff87ca3d47b592cb9fbe07d858d8->Modified();
   165aff87ca3d47b592cb9fbe07d858d8->cd();
   165aff87ca3d47b592cb9fbe07d858d8->SetSelected(165aff87ca3d47b592cb9fbe07d858d8);
}
