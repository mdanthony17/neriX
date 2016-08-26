{
//=========Macro generated from canvas: Canvas_qwe7DiGvEAwy8kooXSk7D4/
//=========  (Thu Aug 18 12:32:11 2016) by ROOT version5.34/26
   TCanvas *Canvas_qwe7DiGvEAwy8kooXSk7D4 = new TCanvas("Canvas_qwe7DiGvEAwy8kooXSk7D4", "",10,32,704,528);
   Canvas_qwe7DiGvEAwy8kooXSk7D4->SetHighLightColor(2);
   Canvas_qwe7DiGvEAwy8kooXSk7D4->Range(-0.125,-0.04080406,1.125,0.3672365);
   Canvas_qwe7DiGvEAwy8kooXSk7D4->SetFillColor(0);
   Canvas_qwe7DiGvEAwy8kooXSk7D4->SetBorderMode(0);
   Canvas_qwe7DiGvEAwy8kooXSk7D4->SetBorderSize(2);
   Canvas_qwe7DiGvEAwy8kooXSk7D4->SetFrameBorderMode(0);
   Canvas_qwe7DiGvEAwy8kooXSk7D4->SetFrameBorderMode(0);
   
   TH1F *h_acceptance = new TH1F("h_acceptance","Acceptance Fraction of Most Recent Sampler",100,0,1);
   h_acceptance->SetBinContent(1,0.1796875);
   h_acceptance->SetBinContent(11,0.2939453);
   h_acceptance->SetBinContent(21,0.2910156);
   h_acceptance->SetBinContent(31,0.1582031);
   h_acceptance->SetBinContent(41,0.0546875);
   h_acceptance->SetBinContent(51,0.015625);
   h_acceptance->SetBinContent(61,0.006835938);
   h_acceptance->SetBinError(1,0.01324674);
   h_acceptance->SetBinError(11,0.01694273);
   h_acceptance->SetBinError(21,0.01685808);
   h_acceptance->SetBinError(31,0.01242961);
   h_acceptance->SetBinError(41,0.007307925);
   h_acceptance->SetBinError(51,0.00390625);
   h_acceptance->SetBinError(61,0.002583742);
   h_acceptance->SetEntries(1024);
   
   TPaveStats *ptstats = new TPaveStats(0.78,0.775,0.98,0.935,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(1);
   ptstats->SetFillColor(0);
   ptstats->SetTextAlign(12);
   ptstats->SetTextFont(42);
   TText *text = ptstats->AddText("h_acceptance");
   text->SetTextSize(0.0368);
   text = ptstats->AddText("Entries = 1024   ");
   text = ptstats->AddText("Mean  = 0.1688");
   text = ptstats->AddText("RMS   = 0.1242");
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
   Canvas_qwe7DiGvEAwy8kooXSk7D4->Modified();
   Canvas_qwe7DiGvEAwy8kooXSk7D4->cd();
   Canvas_qwe7DiGvEAwy8kooXSk7D4->SetSelected(Canvas_qwe7DiGvEAwy8kooXSk7D4);
}
