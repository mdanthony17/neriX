{
//=========Macro generated from canvas: Canvas_Ayagy3aQGfM7ZV9Jw2FPFQ/
//=========  (Wed Aug 10 14:49:44 2016) by ROOT version5.34/26
   TCanvas *Canvas_Ayagy3aQGfM7ZV9Jw2FPFQ = new TCanvas("Canvas_Ayagy3aQGfM7ZV9Jw2FPFQ", "",10,32,704,528);
   Canvas_Ayagy3aQGfM7ZV9Jw2FPFQ->SetHighLightColor(2);
   Canvas_Ayagy3aQGfM7ZV9Jw2FPFQ->Range(-0.125,-0.04025725,1.125,0.3623152);
   Canvas_Ayagy3aQGfM7ZV9Jw2FPFQ->SetFillColor(0);
   Canvas_Ayagy3aQGfM7ZV9Jw2FPFQ->SetBorderMode(0);
   Canvas_Ayagy3aQGfM7ZV9Jw2FPFQ->SetBorderSize(2);
   Canvas_Ayagy3aQGfM7ZV9Jw2FPFQ->SetFrameBorderMode(0);
   Canvas_Ayagy3aQGfM7ZV9Jw2FPFQ->SetFrameBorderMode(0);
   
   TH1F *h_acceptance = new TH1F("h_acceptance","Acceptance Fraction of Most Recent Sampler",100,0,1);
   h_acceptance->SetBinContent(1,0.0078125);
   h_acceptance->SetBinContent(11,0.0546875);
   h_acceptance->SetBinContent(21,0.1347656);
   h_acceptance->SetBinContent(31,0.2148438);
   h_acceptance->SetBinContent(41,0.2832031);
   h_acceptance->SetBinContent(51,0.1914062);
   h_acceptance->SetBinContent(61,0.09375);
   h_acceptance->SetBinContent(71,0.015625);
   h_acceptance->SetBinContent(81,0.00390625);
   h_acceptance->SetBinError(1,0.00390625);
   h_acceptance->SetBinError(11,0.01033497);
   h_acceptance->SetBinError(21,0.01622387);
   h_acceptance->SetBinError(31,0.02048455);
   h_acceptance->SetBinError(41,0.02351874);
   h_acceptance->SetBinError(51,0.01933495);
   h_acceptance->SetBinError(61,0.01353165);
   h_acceptance->SetBinError(71,0.005524272);
   h_acceptance->SetBinError(81,0.002762136);
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
   text = ptstats->AddText("Mean  = 0.3762");
   text = ptstats->AddText("RMS   = 0.1444");
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
   Canvas_Ayagy3aQGfM7ZV9Jw2FPFQ->Modified();
   Canvas_Ayagy3aQGfM7ZV9Jw2FPFQ->cd();
   Canvas_Ayagy3aQGfM7ZV9Jw2FPFQ->SetSelected(Canvas_Ayagy3aQGfM7ZV9Jw2FPFQ);
}
