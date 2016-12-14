{
//=========Macro generated from canvas: c6/
//=========  (Tue Mar  1 09:54:25 2016) by ROOT version5.34/26
   TCanvas *c6 = new TCanvas("c6", "",2458,49,640,480);
   c6->SetHighLightColor(2);
   c6->Range(-500,-69.63323,4500,626.699);
   c6->SetFillColor(0);
   c6->SetBorderMode(0);
   c6->SetBorderSize(2);
   c6->SetFrameBorderMode(0);
   c6->SetFrameBorderMode(0);
   
   TH1F *hS2 = new TH1F("hS2","S2 Spectrum - 62 deg, 2.356 kV",10,0,4000);
   hS2->SetBinContent(1,19);
   hS2->SetBinContent(2,145);
   hS2->SetBinContent(3,260);
   hS2->SetBinContent(4,450);
   hS2->SetBinContent(5,508);
   hS2->SetBinContent(6,416);
   hS2->SetBinContent(7,195);
   hS2->SetBinContent(8,87);
   hS2->SetBinContent(9,51);
   hS2->SetBinContent(10,51);
   hS2->SetBinContent(11,236);
   hS2->SetBinError(1,4.358899);
   hS2->SetBinError(2,12.04159);
   hS2->SetBinError(3,16.12452);
   hS2->SetBinError(4,21.2132);
   hS2->SetBinError(5,22.53886);
   hS2->SetBinError(6,20.39608);
   hS2->SetBinError(7,13.96424);
   hS2->SetBinError(8,9.327379);
   hS2->SetBinError(9,7.141428);
   hS2->SetBinError(10,7.141428);
   hS2->SetBinError(11,15.36229);
   hS2->SetEntries(2418);
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
   TText *text = pt->AddText("S2 Spectrum - 62 deg, 2.356 kV");
   pt->Draw();
   c6->Modified();
   c6->cd();
   c6->SetSelected(c6);
}
