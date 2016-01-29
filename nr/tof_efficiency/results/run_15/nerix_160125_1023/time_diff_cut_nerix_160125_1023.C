{
//=========Macro generated from canvas: ea526072495641a59dc99343b2300ea5/
//=========  (Fri Jan 29 12:57:56 2016) by ROOT version5.34/26
   TCanvas *ea526072495641a59dc99343b2300ea5 = new TCanvas("ea526072495641a59dc99343b2300ea5", "",10,45,704,527);
   ea526072495641a59dc99343b2300ea5->SetHighLightColor(2);
   ea526072495641a59dc99343b2300ea5->Range(-6.875,1.186341,11.875,5.319905);
   ea526072495641a59dc99343b2300ea5->SetFillColor(0);
   ea526072495641a59dc99343b2300ea5->SetBorderMode(0);
   ea526072495641a59dc99343b2300ea5->SetBorderSize(2);
   ea526072495641a59dc99343b2300ea5->SetLogy();
   ea526072495641a59dc99343b2300ea5->SetFrameBorderMode(0);
   ea526072495641a59dc99343b2300ea5->SetFrameBorderMode(0);
   
   TH1F *hGaus = new TH1F("hGaus","Time between Trigger and First S1 in Waveform - nerix_160125_1023",15,-5,10);
   hGaus->SetBinContent(0,112506);
   hGaus->SetBinContent(1,127);
   hGaus->SetBinContent(2,113);
   hGaus->SetBinContent(3,122);
   hGaus->SetBinContent(4,193);
   hGaus->SetBinContent(5,330);
   hGaus->SetBinContent(6,3281);
   hGaus->SetBinContent(7,42354);
   hGaus->SetBinContent(8,17317);
   hGaus->SetBinContent(9,2067);
   hGaus->SetBinContent(10,645);
   hGaus->SetBinContent(11,275);
   hGaus->SetBinContent(12,142);
   hGaus->SetBinContent(13,95);
   hGaus->SetBinContent(14,100);
   hGaus->SetBinContent(15,89);
   hGaus->SetBinContent(16,1173979);
   hGaus->SetBinError(0,335.4191);
   hGaus->SetBinError(1,11.26943);
   hGaus->SetBinError(2,10.63015);
   hGaus->SetBinError(3,11.04536);
   hGaus->SetBinError(4,13.89244);
   hGaus->SetBinError(5,18.1659);
   hGaus->SetBinError(6,57.28001);
   hGaus->SetBinError(7,205.8009);
   hGaus->SetBinError(8,131.5941);
   hGaus->SetBinError(9,45.46427);
   hGaus->SetBinError(10,25.39685);
   hGaus->SetBinError(11,16.58312);
   hGaus->SetBinError(12,11.91638);
   hGaus->SetBinError(13,9.746794);
   hGaus->SetBinError(14,10);
   hGaus->SetBinError(15,9.433981);
   hGaus->SetBinError(16,1083.503);
   hGaus->SetEntries(1353735);
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
   TText *text = pt->AddText("Time between Trigger and First S1 in Waveform - nerix_160125_1023");
   pt->Draw();
   TLine *line = new TLine(0,89,0,42354);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(6,89,6,42354);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   ea526072495641a59dc99343b2300ea5->Modified();
   ea526072495641a59dc99343b2300ea5->cd();
   ea526072495641a59dc99343b2300ea5->SetSelected(ea526072495641a59dc99343b2300ea5);
}
