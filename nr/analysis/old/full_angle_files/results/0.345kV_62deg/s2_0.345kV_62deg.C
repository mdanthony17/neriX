{
//=========Macro generated from canvas: c6/
//=========  (Thu Feb 18 10:06:11 2016) by ROOT version5.34/26
   TCanvas *c6 = new TCanvas("c6", "",2474,196,640,480);
   c6->SetHighLightColor(2);
   c6->Range(-500,-72.18179,4500,649.6361);
   c6->SetFillColor(0);
   c6->SetBorderMode(0);
   c6->SetBorderSize(2);
   c6->SetFrameBorderMode(0);
   c6->SetFrameBorderMode(0);
   
   TH1F *hS2 = new TH1F("hS2","S2 Spectrum - 62 deg, 0.345 kV",10,0,4000);
   hS2->SetBinContent(1,12);
   hS2->SetBinContent(2,111);
   hS2->SetBinContent(3,240);
   hS2->SetBinContent(4,431);
   hS2->SetBinContent(5,527);
   hS2->SetBinContent(6,324);
   hS2->SetBinContent(7,142);
   hS2->SetBinContent(8,71);
   hS2->SetBinContent(9,62);
   hS2->SetBinContent(10,52);
   hS2->SetBinContent(11,248);
   hS2->SetBinError(1,3.464102);
   hS2->SetBinError(2,10.53565);
   hS2->SetBinError(3,15.49193);
   hS2->SetBinError(4,20.76054);
   hS2->SetBinError(5,22.95648);
   hS2->SetBinError(6,18);
   hS2->SetBinError(7,11.91638);
   hS2->SetBinError(8,8.42615);
   hS2->SetBinError(9,7.874008);
   hS2->SetBinError(10,7.211103);
   hS2->SetBinError(11,15.74802);
   hS2->SetEntries(2220);
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
   TText *text = pt->AddText("S2 Spectrum - 62 deg, 0.345 kV");
   pt->Draw();
   c6->Modified();
   c6->cd();
   c6->SetSelected(c6);
}
