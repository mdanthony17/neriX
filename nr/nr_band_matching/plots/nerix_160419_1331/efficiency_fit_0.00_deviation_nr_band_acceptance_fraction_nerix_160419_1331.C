{
//=========Macro generated from canvas: d19643885f56451f844548513de1499b/
//=========  (Wed Jul 20 11:58:35 2016) by ROOT version5.34/26
   TCanvas *d19643885f56451f844548513de1499b = new TCanvas("d19643885f56451f844548513de1499b", "",10,45,704,527);
   d19643885f56451f844548513de1499b->SetHighLightColor(2);
   d19643885f56451f844548513de1499b->Range(-0.125,-0.05957066,1.125,0.5361359);
   d19643885f56451f844548513de1499b->SetFillColor(0);
   d19643885f56451f844548513de1499b->SetBorderMode(0);
   d19643885f56451f844548513de1499b->SetBorderSize(2);
   d19643885f56451f844548513de1499b->SetFrameBorderMode(0);
   d19643885f56451f844548513de1499b->SetFrameBorderMode(0);
   
   TH1F *h_acceptance = new TH1F("h_acceptance","Acceptance Fraction of Most Recent Sampler",100,0,1);
   h_acceptance->SetBinContent(1,0.137931);
   h_acceptance->SetBinContent(21,0.1724138);
   h_acceptance->SetBinContent(41,0.3448276);
   h_acceptance->SetBinContent(61,0.2413793);
   h_acceptance->SetBinContent(81,0.1034483);
   h_acceptance->SetBinContent(101,0.03448276);
   h_acceptance->SetBinError(1,0.06896552);
   h_acceptance->SetBinError(21,0.07710579);
   h_acceptance->SetBinError(41,0.1090441);
   h_acceptance->SetBinError(61,0.0912328);
   h_acceptance->SetBinError(81,0.05972589);
   h_acceptance->SetBinError(101,0.03448276);
   h_acceptance->SetEntries(30);
   h_acceptance->SetStats(0);
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
   
   TPaveText *pt = new TPaveText(0.15,0.9374,0.85,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Acceptance Fraction of Most Recent Sampler");
   pt->Draw();
   d19643885f56451f844548513de1499b->Modified();
   d19643885f56451f844548513de1499b->cd();
   d19643885f56451f844548513de1499b->SetSelected(d19643885f56451f844548513de1499b);
}
