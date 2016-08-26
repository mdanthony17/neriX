{
//=========Macro generated from canvas: Canvas_vUfzhv9H5NqE8WMvQgQTG4/
//=========  (Wed Aug 17 12:21:49 2016) by ROOT version5.34/26
   TCanvas *Canvas_vUfzhv9H5NqE8WMvQgQTG4 = new TCanvas("Canvas_vUfzhv9H5NqE8WMvQgQTG4", "",10,32,704,528);
   Canvas_vUfzhv9H5NqE8WMvQgQTG4->SetHighLightColor(2);
   Canvas_vUfzhv9H5NqE8WMvQgQTG4->Range(-0.125,-0.05038269,1.125,0.4534442);
   Canvas_vUfzhv9H5NqE8WMvQgQTG4->SetFillColor(0);
   Canvas_vUfzhv9H5NqE8WMvQgQTG4->SetBorderMode(0);
   Canvas_vUfzhv9H5NqE8WMvQgQTG4->SetBorderSize(2);
   Canvas_vUfzhv9H5NqE8WMvQgQTG4->SetFrameBorderMode(0);
   Canvas_vUfzhv9H5NqE8WMvQgQTG4->SetFrameBorderMode(0);
   
   TH1F *h_acceptance = new TH1F("h_acceptance","Acceptance Fraction of Most Recent Sampler",100,0,1);
   h_acceptance->SetBinContent(1,0.2896282);
   h_acceptance->SetBinContent(21,0.3649707);
   h_acceptance->SetBinContent(41,0.2172211);
   h_acceptance->SetBinContent(61,0.1086106);
   h_acceptance->SetBinContent(81,0.01956947);
   h_acceptance->SetBinContent(101,0.001956947);
   h_acceptance->SetBinError(1,0.0168343);
   h_acceptance->SetBinError(21,0.01889746);
   h_acceptance->SetBinError(41,0.01457893);
   h_acceptance->SetBinError(61,0.01030886);
   h_acceptance->SetBinError(81,0.004375867);
   h_acceptance->SetBinError(101,0.001383771);
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
   text = ptstats->AddText("Mean  = 0.2422");
   text = ptstats->AddText("RMS   =   0.21");
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
   Canvas_vUfzhv9H5NqE8WMvQgQTG4->Modified();
   Canvas_vUfzhv9H5NqE8WMvQgQTG4->cd();
   Canvas_vUfzhv9H5NqE8WMvQgQTG4->SetSelected(Canvas_vUfzhv9H5NqE8WMvQgQTG4);
}
