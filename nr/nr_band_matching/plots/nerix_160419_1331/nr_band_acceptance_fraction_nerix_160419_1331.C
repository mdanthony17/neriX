{
//=========Macro generated from canvas: 828ff75bb15e4f2b88f9309ac64dc0b5/
//=========  (Tue Jul 12 17:10:28 2016) by ROOT version5.34/26
   TCanvas *828ff75bb15e4f2b88f9309ac64dc0b5 = new TCanvas("828ff75bb15e4f2b88f9309ac64dc0b5", "",10,45,704,527);
   828ff75bb15e4f2b88f9309ac64dc0b5->SetHighLightColor(2);
   828ff75bb15e4f2b88f9309ac64dc0b5->Range(-0.125,-0.1090063,1.125,0.9810568);
   828ff75bb15e4f2b88f9309ac64dc0b5->SetFillColor(0);
   828ff75bb15e4f2b88f9309ac64dc0b5->SetBorderMode(0);
   828ff75bb15e4f2b88f9309ac64dc0b5->SetBorderSize(2);
   828ff75bb15e4f2b88f9309ac64dc0b5->SetFrameBorderMode(0);
   828ff75bb15e4f2b88f9309ac64dc0b5->SetFrameBorderMode(0);
   
   TH1F *h_acceptance = new TH1F("h_acceptance","Acceptance Fraction of Most Recent Sampler",100,0,1);
   h_acceptance->SetBinContent(1,0.8164062);
   h_acceptance->SetBinContent(11,0.09643555);
   h_acceptance->SetBinContent(21,0.05786133);
   h_acceptance->SetBinContent(31,0.0222168);
   h_acceptance->SetBinContent(41,0.005371094);
   h_acceptance->SetBinContent(51,0.001708984);
   h_acceptance->SetBinError(1,0.014118);
   h_acceptance->SetBinError(11,0.004852199);
   h_acceptance->SetBinError(21,0.003758497);
   h_acceptance->SetBinError(31,0.002328953);
   h_acceptance->SetBinError(41,0.001145121);
   h_acceptance->SetBinError(51,0.0006459354);
   h_acceptance->SetEntries(4096);
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
   828ff75bb15e4f2b88f9309ac64dc0b5->Modified();
   828ff75bb15e4f2b88f9309ac64dc0b5->cd();
   828ff75bb15e4f2b88f9309ac64dc0b5->SetSelected(828ff75bb15e4f2b88f9309ac64dc0b5);
}
