{
//=========Macro generated from canvas: Canvas_uEF7G6n2VEBnQ6JDKjA8Ge/
//=========  (Tue Aug  2 07:14:28 2016) by ROOT version5.34/30
   TCanvas *Canvas_uEF7G6n2VEBnQ6JDKjA8Ge = new TCanvas("Canvas_uEF7G6n2VEBnQ6JDKjA8Ge", "",10,32,704,528);
   Canvas_uEF7G6n2VEBnQ6JDKjA8Ge->SetHighLightColor(2);
   Canvas_uEF7G6n2VEBnQ6JDKjA8Ge->Range(-0.125,-0.06273306,1.125,0.5645975);
   Canvas_uEF7G6n2VEBnQ6JDKjA8Ge->SetFillColor(0);
   Canvas_uEF7G6n2VEBnQ6JDKjA8Ge->SetBorderMode(0);
   Canvas_uEF7G6n2VEBnQ6JDKjA8Ge->SetBorderSize(2);
   Canvas_uEF7G6n2VEBnQ6JDKjA8Ge->SetFrameBorderMode(0);
   Canvas_uEF7G6n2VEBnQ6JDKjA8Ge->SetFrameBorderMode(0);
   
   TH1F *h_acceptance = new TH1F("h_acceptance","Acceptance Fraction of Most Recent Sampler",100,0,1);
   h_acceptance->SetBinContent(1,0.2443848);
   h_acceptance->SetBinContent(2,0.4672852);
   h_acceptance->SetBinContent(3,0.2504883);
   h_acceptance->SetBinContent(4,0.03417969);
   h_acceptance->SetBinContent(5,0.003173828);
   h_acceptance->SetBinContent(6,0.0002441406);
   h_acceptance->SetBinContent(11,0.0002441406);
   h_acceptance->SetBinError(1,0.007724264);
   h_acceptance->SetBinError(2,0.01068098);
   h_acceptance->SetBinError(3,0.007820126);
   h_acceptance->SetBinError(4,0.002888711);
   h_acceptance->SetBinError(5,0.0008802615);
   h_acceptance->SetBinError(6,0.0002441406);
   h_acceptance->SetBinError(11,0.0002441406);
   h_acceptance->SetEntries(4096);
   
   TPaveStats *ptstats = new TPaveStats(0.78,0.775,0.98,0.935,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(1);
   ptstats->SetFillColor(0);
   ptstats->SetTextAlign(12);
   ptstats->SetTextFont(42);
   TText *text = ptstats->AddText("h_acceptance");
   text->SetTextSize(0.0368);
   text = ptstats->AddText("Entries = 4096   ");
   text = ptstats->AddText("Mean  = 0.01571");
   text = ptstats->AddText("RMS   = 0.007853");
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
   Canvas_uEF7G6n2VEBnQ6JDKjA8Ge->Modified();
   Canvas_uEF7G6n2VEBnQ6JDKjA8Ge->cd();
   Canvas_uEF7G6n2VEBnQ6JDKjA8Ge->SetSelected(Canvas_uEF7G6n2VEBnQ6JDKjA8Ge);
}
