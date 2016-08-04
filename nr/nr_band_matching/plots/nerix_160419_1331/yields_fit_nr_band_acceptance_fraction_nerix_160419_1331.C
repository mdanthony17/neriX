{
//=========Macro generated from canvas: Canvas_cZZYx7V8R2jbifSP9tqCGX/
//=========  (Tue Aug  2 22:05:50 2016) by ROOT version5.34/30
   TCanvas *Canvas_cZZYx7V8R2jbifSP9tqCGX = new TCanvas("Canvas_cZZYx7V8R2jbifSP9tqCGX", "",10,32,704,528);
   Canvas_cZZYx7V8R2jbifSP9tqCGX->SetHighLightColor(2);
   Canvas_cZZYx7V8R2jbifSP9tqCGX->Range(-0.125,-0.06205319,1.125,0.5584787);
   Canvas_cZZYx7V8R2jbifSP9tqCGX->SetFillColor(0);
   Canvas_cZZYx7V8R2jbifSP9tqCGX->SetBorderMode(0);
   Canvas_cZZYx7V8R2jbifSP9tqCGX->SetBorderSize(2);
   Canvas_cZZYx7V8R2jbifSP9tqCGX->SetFrameBorderMode(0);
   Canvas_cZZYx7V8R2jbifSP9tqCGX->SetFrameBorderMode(0);
   
   TH1F *h_acceptance = new TH1F("h_acceptance","Acceptance Fraction of Most Recent Sampler",100,0,1);
   h_acceptance->SetBinContent(1,0.3046875);
   h_acceptance->SetBinContent(2,0.4433594);
   h_acceptance->SetBinContent(3,0.1972656);
   h_acceptance->SetBinContent(4,0.046875);
   h_acceptance->SetBinContent(5,0.0078125);
   h_acceptance->SetBinError(1,0.02439452);
   h_acceptance->SetBinError(2,0.0294268);
   h_acceptance->SetBinError(3,0.01962866);
   h_acceptance->SetBinError(4,0.009568319);
   h_acceptance->SetBinError(5,0.00390625);
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
   text = ptstats->AddText("Mean  = 0.01489");
   text = ptstats->AddText("RMS   = 0.008117");
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
   Canvas_cZZYx7V8R2jbifSP9tqCGX->Modified();
   Canvas_cZZYx7V8R2jbifSP9tqCGX->cd();
   Canvas_cZZYx7V8R2jbifSP9tqCGX->SetSelected(Canvas_cZZYx7V8R2jbifSP9tqCGX);
}
