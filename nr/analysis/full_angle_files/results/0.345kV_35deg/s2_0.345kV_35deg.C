{
//=========Macro generated from canvas: c6/
//=========  (Thu Feb 18 12:22:57 2016) by ROOT version5.34/26
   TCanvas *c6 = new TCanvas("c6", "",2240,45,640,480);
   c6->SetHighLightColor(2);
   c6->Range(-500,-39.0814,4500,351.7326);
   c6->SetFillColor(0);
   c6->SetBorderMode(0);
   c6->SetBorderSize(2);
   c6->SetFrameBorderMode(0);
   c6->SetFrameBorderMode(0);
   
   TH1F *hS2 = new TH1F("hS2","S2 Spectrum - 35 deg, 0.345 kV",10,0,4000);
   hS2->SetBinContent(1,7);
   hS2->SetBinContent(2,149);
   hS2->SetBinContent(3,281);
   hS2->SetBinContent(4,146);
   hS2->SetBinContent(5,55);
   hS2->SetBinContent(6,21);
   hS2->SetBinContent(7,8);
   hS2->SetBinContent(8,9);
   hS2->SetBinContent(9,9);
   hS2->SetBinContent(10,7);
   hS2->SetBinContent(11,47);
   hS2->SetBinError(1,2.645751);
   hS2->SetBinError(2,12.20656);
   hS2->SetBinError(3,16.76305);
   hS2->SetBinError(4,12.08305);
   hS2->SetBinError(5,7.416198);
   hS2->SetBinError(6,4.582576);
   hS2->SetBinError(7,2.828427);
   hS2->SetBinError(8,3);
   hS2->SetBinError(9,3);
   hS2->SetBinError(10,2.645751);
   hS2->SetBinError(11,6.855655);
   hS2->SetEntries(739);
   hS2->SetStats(0);
   hS2->SetFillStyle(0);
   hS2->SetMarkerStyle(20);
   hS2->SetMarkerSize(0);
   hS2->GetXaxis()->SetTitle("cpS2sTotBottom[0] [PE]");
   hS2->GetXaxis()->SetLabelFont(42);
   hS2->GetXaxis()->SetLabelSize(0.035);
   hS2->GetXaxis()->SetTitleSize(0.035);
   hS2->GetXaxis()->SetTitleFont(42);
   hS2->GetYaxis()->SetTitle("Counts");
   hS2->GetYaxis()->SetLabelFont(42);
   hS2->GetYaxis()->SetLabelSize(0.035);
   hS2->GetYaxis()->SetTitleSize(0.035);
   hS2->GetYaxis()->SetTitleFont(42);
   hS2->GetZaxis()->SetLabelFont(42);
   hS2->GetZaxis()->SetLabelSize(0.035);
   hS2->GetZaxis()->SetTitleSize(0.035);
   hS2->GetZaxis()->SetTitleFont(42);
   hS2->Draw("e");
   
   TPaveText *pt = new TPaveText(0.2242767,0.9340729,0.7757233,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("S2 Spectrum - 35 deg, 0.345 kV");
   pt->Draw();
   c6->Modified();
   c6->cd();
   c6->SetSelected(c6);
}
