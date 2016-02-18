{
//=========Macro generated from canvas: c6/
//=========  (Tue Feb 16 16:57:00 2016) by ROOT version5.34/26
   TCanvas *c6 = new TCanvas("c6", "",2240,45,640,480);
   c6->SetHighLightColor(2);
   c6->Range(-500,-20.06476,4500,180.5828);
   c6->SetFillColor(0);
   c6->SetBorderMode(0);
   c6->SetBorderSize(2);
   c6->SetFrameBorderMode(0);
   c6->SetFrameBorderMode(0);
   
   TH1F *hS2 = new TH1F("hS2","S2 Spectrum - 30 deg, 1.054 kV",10,0,4000);
   hS2->SetBinContent(1,7);
   hS2->SetBinContent(2,141);
   hS2->SetBinContent(3,141);
   hS2->SetBinContent(4,89);
   hS2->SetBinContent(5,58);
   hS2->SetBinContent(6,35);
   hS2->SetBinContent(7,15);
   hS2->SetBinContent(8,12);
   hS2->SetBinContent(9,16);
   hS2->SetBinContent(10,14);
   hS2->SetBinContent(11,72);
   hS2->SetBinError(1,2.645751);
   hS2->SetBinError(2,11.87434);
   hS2->SetBinError(3,11.87434);
   hS2->SetBinError(4,9.433981);
   hS2->SetBinError(5,7.615773);
   hS2->SetBinError(6,5.91608);
   hS2->SetBinError(7,3.872983);
   hS2->SetBinError(8,3.464102);
   hS2->SetBinError(9,4);
   hS2->SetBinError(10,3.741657);
   hS2->SetBinError(11,8.485281);
   hS2->SetEntries(600);
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
   TText *text = pt->AddText("S2 Spectrum - 30 deg, 1.054 kV");
   pt->Draw();
   c6->Modified();
   c6->cd();
   c6->SetSelected(c6);
}
