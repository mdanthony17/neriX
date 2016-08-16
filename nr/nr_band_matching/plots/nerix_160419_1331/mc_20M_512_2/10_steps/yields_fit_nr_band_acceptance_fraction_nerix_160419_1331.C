{
//=========Macro generated from canvas: Canvas_XbG23VkkvfqUXWZeomRBpZ/
//=========  (Wed Aug 10 22:40:12 2016) by ROOT version5.34/26
   TCanvas *Canvas_XbG23VkkvfqUXWZeomRBpZ = new TCanvas("Canvas_XbG23VkkvfqUXWZeomRBpZ", "",10,32,704,528);
   Canvas_XbG23VkkvfqUXWZeomRBpZ->SetHighLightColor(2);
   Canvas_XbG23VkkvfqUXWZeomRBpZ->Range(-0.125,-0.03705059,1.125,0.3334553);
   Canvas_XbG23VkkvfqUXWZeomRBpZ->SetFillColor(0);
   Canvas_XbG23VkkvfqUXWZeomRBpZ->SetBorderMode(0);
   Canvas_XbG23VkkvfqUXWZeomRBpZ->SetBorderSize(2);
   Canvas_XbG23VkkvfqUXWZeomRBpZ->SetFrameBorderMode(0);
   Canvas_XbG23VkkvfqUXWZeomRBpZ->SetFrameBorderMode(0);
   
   TH1F *h_acceptance = new TH1F("h_acceptance","Acceptance Fraction of Most Recent Sampler",100,0,1);
   h_acceptance->SetBinContent(1,0.02539062);
   h_acceptance->SetBinContent(11,0.04492188);
   h_acceptance->SetBinContent(21,0.1679688);
   h_acceptance->SetBinContent(31,0.2519531);
   h_acceptance->SetBinContent(41,0.2597656);
   h_acceptance->SetBinContent(51,0.1621094);
   h_acceptance->SetBinContent(61,0.06835938);
   h_acceptance->SetBinContent(71,0.01171875);
   h_acceptance->SetBinContent(81,0.005859375);
   h_acceptance->SetBinContent(91,0.001953125);
   h_acceptance->SetBinError(1,0.007042092);
   h_acceptance->SetBinError(11,0.009366858);
   h_acceptance->SetBinError(21,0.01811254);
   h_acceptance->SetBinError(31,0.02218324);
   h_acceptance->SetBinError(41,0.02252454);
   h_acceptance->SetBinError(51,0.01779382);
   h_acceptance->SetBinError(61,0.01155484);
   h_acceptance->SetBinError(71,0.00478416);
   h_acceptance->SetBinError(81,0.003382912);
   h_acceptance->SetBinError(91,0.001953125);
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
   text = ptstats->AddText("Mean  = 0.3543");
   text = ptstats->AddText("RMS   = 0.1486");
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
   Canvas_XbG23VkkvfqUXWZeomRBpZ->Modified();
   Canvas_XbG23VkkvfqUXWZeomRBpZ->cd();
   Canvas_XbG23VkkvfqUXWZeomRBpZ->SetSelected(Canvas_XbG23VkkvfqUXWZeomRBpZ);
}
