{
//=========Macro generated from canvas: f2896a49eec849e1aba7bdd4b9d9edc2/
//=========  (Tue Apr  5 15:49:50 2016) by ROOT version5.34/26
   TCanvas *f2896a49eec849e1aba7bdd4b9d9edc2 = new TCanvas("f2896a49eec849e1aba7bdd4b9d9edc2", "",10,45,704,527);
   f2896a49eec849e1aba7bdd4b9d9edc2->SetHighLightColor(2);
   f2896a49eec849e1aba7bdd4b9d9edc2->Range(-6.875,1.575565,11.875,4.63247);
   f2896a49eec849e1aba7bdd4b9d9edc2->SetFillColor(0);
   f2896a49eec849e1aba7bdd4b9d9edc2->SetBorderMode(0);
   f2896a49eec849e1aba7bdd4b9d9edc2->SetBorderSize(2);
   f2896a49eec849e1aba7bdd4b9d9edc2->SetLogy();
   f2896a49eec849e1aba7bdd4b9d9edc2->SetFrameBorderMode(0);
   f2896a49eec849e1aba7bdd4b9d9edc2->SetFrameBorderMode(0);
   
   TH1F *hGaus = new TH1F("hGaus","Time between Trigger and First S1 in Waveform - nerix_160405_1054",15,-5,10);
   hGaus->SetBinContent(0,249203);
   hGaus->SetBinContent(1,303);
   hGaus->SetBinContent(2,280);
   hGaus->SetBinContent(3,290);
   hGaus->SetBinContent(4,273);
   hGaus->SetBinContent(5,297);
   hGaus->SetBinContent(6,353);
   hGaus->SetBinContent(7,743);
   hGaus->SetBinContent(8,11095);
   hGaus->SetBinContent(9,11072);
   hGaus->SetBinContent(10,2511);
   hGaus->SetBinContent(11,240);
   hGaus->SetBinContent(12,195);
   hGaus->SetBinContent(13,199);
   hGaus->SetBinContent(14,165);
   hGaus->SetBinContent(15,202);
   hGaus->SetBinContent(16,1720976);
   hGaus->SetBinError(0,499.2024);
   hGaus->SetBinError(1,17.4069);
   hGaus->SetBinError(2,16.7332);
   hGaus->SetBinError(3,17.02939);
   hGaus->SetBinError(4,16.52271);
   hGaus->SetBinError(5,17.23369);
   hGaus->SetBinError(6,18.78829);
   hGaus->SetBinError(7,27.25803);
   hGaus->SetBinError(8,105.3328);
   hGaus->SetBinError(9,105.2236);
   hGaus->SetBinError(10,50.10988);
   hGaus->SetBinError(11,15.49193);
   hGaus->SetBinError(12,13.96424);
   hGaus->SetBinError(13,14.10674);
   hGaus->SetBinError(14,12.84523);
   hGaus->SetBinError(15,14.21267);
   hGaus->SetBinError(16,1311.86);
   hGaus->SetEntries(1998397);
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
   TText *text = pt->AddText("Time between Trigger and First S1 in Waveform - nerix_160405_1054");
   pt->Draw();
   TLine *line = new TLine(0,165,0,11095);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(6,165,6,11095);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   f2896a49eec849e1aba7bdd4b9d9edc2->Modified();
   f2896a49eec849e1aba7bdd4b9d9edc2->cd();
   f2896a49eec849e1aba7bdd4b9d9edc2->SetSelected(f2896a49eec849e1aba7bdd4b9d9edc2);
}
