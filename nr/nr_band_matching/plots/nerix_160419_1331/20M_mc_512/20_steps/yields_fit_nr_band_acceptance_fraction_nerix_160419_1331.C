{
//=========Macro generated from canvas: Canvas_ECRpcmZWawTzrCqRR2c7QC/
//=========  (Wed Aug 10 15:23:49 2016) by ROOT version5.34/26
   TCanvas *Canvas_ECRpcmZWawTzrCqRR2c7QC = new TCanvas("Canvas_ECRpcmZWawTzrCqRR2c7QC", "",10,32,704,528);
   Canvas_ECRpcmZWawTzrCqRR2c7QC->SetHighLightColor(2);
   Canvas_ECRpcmZWawTzrCqRR2c7QC->Range(-0.125,-0.03865462,1.125,0.3478916);
   Canvas_ECRpcmZWawTzrCqRR2c7QC->SetFillColor(0);
   Canvas_ECRpcmZWawTzrCqRR2c7QC->SetBorderMode(0);
   Canvas_ECRpcmZWawTzrCqRR2c7QC->SetBorderSize(2);
   Canvas_ECRpcmZWawTzrCqRR2c7QC->SetFrameBorderMode(0);
   Canvas_ECRpcmZWawTzrCqRR2c7QC->SetFrameBorderMode(0);
   
   TH1F *h_acceptance = new TH1F("h_acceptance","Acceptance Fraction of Most Recent Sampler",100,0,1);
   h_acceptance->SetBinContent(1,0.009765625);
   h_acceptance->SetBinContent(11,0.08984375);
   h_acceptance->SetBinContent(21,0.1660156);
   h_acceptance->SetBinContent(31,0.2402344);
   h_acceptance->SetBinContent(41,0.2714844);
   h_acceptance->SetBinContent(51,0.1503906);
   h_acceptance->SetBinContent(61,0.05664062);
   h_acceptance->SetBinContent(71,0.01367188);
   h_acceptance->SetBinContent(81,0.001953125);
   h_acceptance->SetBinError(1,0.00436732);
   h_acceptance->SetBinError(11,0.01324674);
   h_acceptance->SetBinError(21,0.01800692);
   h_acceptance->SetBinError(31,0.0216612);
   h_acceptance->SetBinError(41,0.023027);
   h_acceptance->SetBinError(51,0.0171386);
   h_acceptance->SetBinError(61,0.0105179);
   h_acceptance->SetBinError(71,0.005167483);
   h_acceptance->SetBinError(81,0.001953125);
   h_acceptance->SetEntries(512);
   
   TPaveStats *ptstats = new TPaveStats(0.78,0.775,0.98,0.935,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(1);
   ptstats->SetFillColor(0);
   ptstats->SetTextAlign(12);
   ptstats->SetTextFont(42);
   TText *text = ptstats->AddText("h_acceptance");
   text->SetTextSize(0.0368);
   text = ptstats->AddText("Entries = 512    ");
   text = ptstats->AddText("Mean  = 0.3432");
   text = ptstats->AddText("RMS   = 0.1441");
   ptstats->SetOptStat(1111);
   ptstats->SetOptFit(0);
   ptstats->Draw();
   h_acceptance->GetListOfFunctions()->Add(ptstats);
   ptstats->SetParent(h_acceptance);
   h_acceptance->SetFillStyle(0);
   h_acceptance->SetMarkerStyle(20);
   h_acceptance->GetXaxis()->SetLabelFont(42);
   h_acceptance->GetXaxis()->SetLabelSize(0.035);
   h_acceptance->GetXaxis()->SetTitleSize(0.035);
   h_acceptance->GetXaxis()->SetTitleFont(42);
   h_acceptance->GetYaxis()->SetLabelFont(42);
   h_acceptance->GetYaxis()->SetLabelSize(0.035);
   h_acceptance->GetYaxis()->SetTitleSize(0.035);
   h_acceptance->GetYaxis()->SetTitleFont(42);
   h_acceptance->GetZaxis()->SetLabelFont(42);
   h_acceptance->GetZaxis()->SetLabelSize(0.035);
   h_acceptance->GetZaxis()->SetTitleSize(0.035);
   h_acceptance->GetZaxis()->SetTitleFont(42);
   h_acceptance->Draw("");
   
   TPaveText *pt = new TPaveText(0.15,0.9326,0.85,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("Acceptance Fraction of Most Recent Sampler");
   pt->Draw();
   Canvas_ECRpcmZWawTzrCqRR2c7QC->Modified();
   Canvas_ECRpcmZWawTzrCqRR2c7QC->cd();
   Canvas_ECRpcmZWawTzrCqRR2c7QC->SetSelected(Canvas_ECRpcmZWawTzrCqRR2c7QC);
}
