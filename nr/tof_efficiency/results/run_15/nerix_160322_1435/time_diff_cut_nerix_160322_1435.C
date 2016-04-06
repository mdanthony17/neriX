{
//=========Macro generated from canvas: 4c0e402fa65647ec827a964fb0217686/
//=========  (Tue Mar 22 15:57:37 2016) by ROOT version5.34/26
   TCanvas *4c0e402fa65647ec827a964fb0217686 = new TCanvas("4c0e402fa65647ec827a964fb0217686", "",10,45,704,527);
   4c0e402fa65647ec827a964fb0217686->SetHighLightColor(2);
   4c0e402fa65647ec827a964fb0217686->Range(-6.875,1.285295,11.875,4.824772);
   4c0e402fa65647ec827a964fb0217686->SetFillColor(0);
   4c0e402fa65647ec827a964fb0217686->SetBorderMode(0);
   4c0e402fa65647ec827a964fb0217686->SetBorderSize(2);
   4c0e402fa65647ec827a964fb0217686->SetLogy();
   4c0e402fa65647ec827a964fb0217686->SetFrameBorderMode(0);
   4c0e402fa65647ec827a964fb0217686->SetFrameBorderMode(0);
   
   TH1F *hGaus = new TH1F("hGaus","Time between Trigger and First S1 in Waveform - nerix_160322_1435",15,-5,10);
   hGaus->SetBinContent(0,231240);
   hGaus->SetBinContent(1,206);
   hGaus->SetBinContent(2,239);
   hGaus->SetBinContent(3,244);
   hGaus->SetBinContent(4,245);
   hGaus->SetBinContent(5,210);
   hGaus->SetBinContent(6,267);
   hGaus->SetBinContent(7,742);
   hGaus->SetBinContent(8,15481);
   hGaus->SetBinContent(9,11693);
   hGaus->SetBinContent(10,1320);
   hGaus->SetBinContent(11,171);
   hGaus->SetBinContent(12,116);
   hGaus->SetBinContent(13,149);
   hGaus->SetBinContent(14,97);
   hGaus->SetBinContent(15,143);
   hGaus->SetBinContent(16,1674554);
   hGaus->SetBinError(0,480.8742);
   hGaus->SetBinError(1,14.3527);
   hGaus->SetBinError(2,15.45962);
   hGaus->SetBinError(3,15.6205);
   hGaus->SetBinError(4,15.65248);
   hGaus->SetBinError(5,14.49138);
   hGaus->SetBinError(6,16.34013);
   hGaus->SetBinError(7,27.23968);
   hGaus->SetBinError(8,124.4227);
   hGaus->SetBinError(9,108.1342);
   hGaus->SetBinError(10,36.3318);
   hGaus->SetBinError(11,13.0767);
   hGaus->SetBinError(12,10.77033);
   hGaus->SetBinError(13,12.20656);
   hGaus->SetBinError(14,9.848858);
   hGaus->SetBinError(15,11.95826);
   hGaus->SetBinError(16,1294.046);
   hGaus->SetEntries(1937117);
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
   TText *text = pt->AddText("Time between Trigger and First S1 in Waveform - nerix_160322_1435");
   pt->Draw();
   TLine *line = new TLine(0,97,0,15481);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(6,97,6,15481);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   4c0e402fa65647ec827a964fb0217686->Modified();
   4c0e402fa65647ec827a964fb0217686->cd();
   4c0e402fa65647ec827a964fb0217686->SetSelected(4c0e402fa65647ec827a964fb0217686);
}
