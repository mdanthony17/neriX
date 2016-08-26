{
//=========Macro generated from canvas: Canvas_hFHqwT3GZ5h282WpRMkAVB/
//=========  (Wed Aug 17 14:22:21 2016) by ROOT version5.34/26
   TCanvas *Canvas_hFHqwT3GZ5h282WpRMkAVB = new TCanvas("Canvas_hFHqwT3GZ5h282WpRMkAVB", "",10,32,704,528);
   Canvas_hFHqwT3GZ5h282WpRMkAVB->SetHighLightColor(2);
   Canvas_hFHqwT3GZ5h282WpRMkAVB->Range(-0.125,-0.0347311,1.125,0.3125799);
   Canvas_hFHqwT3GZ5h282WpRMkAVB->SetFillColor(0);
   Canvas_hFHqwT3GZ5h282WpRMkAVB->SetBorderMode(0);
   Canvas_hFHqwT3GZ5h282WpRMkAVB->SetBorderSize(2);
   Canvas_hFHqwT3GZ5h282WpRMkAVB->SetFrameBorderMode(0);
   Canvas_hFHqwT3GZ5h282WpRMkAVB->SetFrameBorderMode(0);
   
   TH1F *h_acceptance = new TH1F("h_acceptance","Acceptance Fraction of Most Recent Sampler",100,0,1);
   h_acceptance->SetBinContent(1,0.1298828);
   h_acceptance->SetBinContent(11,0.1806641);
   h_acceptance->SetBinContent(21,0.2490234);
   h_acceptance->SetBinContent(31,0.2373047);
   h_acceptance->SetBinContent(41,0.1191406);
   h_acceptance->SetBinContent(51,0.05957031);
   h_acceptance->SetBinContent(61,0.015625);
   h_acceptance->SetBinContent(71,0.008789062);
   h_acceptance->SetBinError(1,0.01126227);
   h_acceptance->SetBinError(11,0.01328269);
   h_acceptance->SetBinError(21,0.01559445);
   h_acceptance->SetBinError(31,0.0152231);
   h_acceptance->SetBinError(41,0.01078649);
   h_acceptance->SetBinError(51,0.007627197);
   h_acceptance->SetBinError(61,0.00390625);
   h_acceptance->SetBinError(71,0.002929688);
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
   text = ptstats->AddText("Mean  =  0.232");
   text = ptstats->AddText("RMS   = 0.1522");
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
   Canvas_hFHqwT3GZ5h282WpRMkAVB->Modified();
   Canvas_hFHqwT3GZ5h282WpRMkAVB->cd();
   Canvas_hFHqwT3GZ5h282WpRMkAVB->SetSelected(Canvas_hFHqwT3GZ5h282WpRMkAVB);
}
