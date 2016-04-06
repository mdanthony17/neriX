{
//=========Macro generated from canvas: e82364a96bb547a4a4f7fe4910654075/
//=========  (Thu Mar  3 14:57:39 2016) by ROOT version5.34/26
   TCanvas *e82364a96bb547a4a4f7fe4910654075 = new TCanvas("e82364a96bb547a4a4f7fe4910654075", "",10,45,704,527);
   e82364a96bb547a4a4f7fe4910654075->SetHighLightColor(2);
   e82364a96bb547a4a4f7fe4910654075->Range(-6.875,0.738455,11.875,4.980134);
   e82364a96bb547a4a4f7fe4910654075->SetFillColor(0);
   e82364a96bb547a4a4f7fe4910654075->SetBorderMode(0);
   e82364a96bb547a4a4f7fe4910654075->SetBorderSize(2);
   e82364a96bb547a4a4f7fe4910654075->SetLogy();
   e82364a96bb547a4a4f7fe4910654075->SetFrameBorderMode(0);
   e82364a96bb547a4a4f7fe4910654075->SetFrameBorderMode(0);
   
   TH1F *hGaus = new TH1F("hGaus","Time between Trigger and First S1 in Waveform - nerix_160223_0947",15,-5,10);
   hGaus->SetBinContent(0,186652);
   hGaus->SetBinContent(1,184);
   hGaus->SetBinContent(2,176);
   hGaus->SetBinContent(3,186);
   hGaus->SetBinContent(4,215);
   hGaus->SetBinContent(5,311);
   hGaus->SetBinContent(6,1241);
   hGaus->SetBinContent(7,18848);
   hGaus->SetBinContent(8,7410);
   hGaus->SetBinContent(9,503);
   hGaus->SetBinContent(10,116);
   hGaus->SetBinContent(11,71);
   hGaus->SetBinContent(12,46);
   hGaus->SetBinContent(13,45);
   hGaus->SetBinContent(14,35);
   hGaus->SetBinContent(15,43);
   hGaus->SetBinContent(16,533458);
   hGaus->SetBinError(0,432.0324);
   hGaus->SetBinError(1,13.56466);
   hGaus->SetBinError(2,13.2665);
   hGaus->SetBinError(3,13.63818);
   hGaus->SetBinError(4,14.66288);
   hGaus->SetBinError(5,17.63519);
   hGaus->SetBinError(6,35.22783);
   hGaus->SetBinError(7,137.288);
   hGaus->SetBinError(8,86.08136);
   hGaus->SetBinError(9,22.42766);
   hGaus->SetBinError(10,10.77033);
   hGaus->SetBinError(11,8.42615);
   hGaus->SetBinError(12,6.78233);
   hGaus->SetBinError(13,6.708204);
   hGaus->SetBinError(14,5.91608);
   hGaus->SetBinError(15,6.557439);
   hGaus->SetBinError(16,730.3821);
   hGaus->SetEntries(749540);
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
   TText *text = pt->AddText("Time between Trigger and First S1 in Waveform - nerix_160223_0947");
   pt->Draw();
   TLine *line = new TLine(-1,35,-1,18848);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(4,35,4,18848);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   e82364a96bb547a4a4f7fe4910654075->Modified();
   e82364a96bb547a4a4f7fe4910654075->cd();
   e82364a96bb547a4a4f7fe4910654075->SetSelected(e82364a96bb547a4a4f7fe4910654075);
}
