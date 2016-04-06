{
//=========Macro generated from canvas: c6/
//=========  (Tue Mar  1 09:49:34 2016) by ROOT version5.34/26
   TCanvas *c6 = new TCanvas("c6", "",2519,47,640,480);
   c6->SetHighLightColor(2);
   c6->Range(-500,-54.4523,4500,490.0706);
   c6->SetFillColor(0);
   c6->SetBorderMode(0);
   c6->SetBorderSize(2);
   c6->SetFrameBorderMode(0);
   c6->SetFrameBorderMode(0);
   
   TH1F *hS2 = new TH1F("hS2","S2 Spectrum - 35 deg, 2.356 kV",10,0,4000);
   hS2->SetBinContent(1,7);
   hS2->SetBinContent(2,183);
   hS2->SetBinContent(3,395);
   hS2->SetBinContent(4,166);
   hS2->SetBinContent(5,66);
   hS2->SetBinContent(6,36);
   hS2->SetBinContent(7,13);
   hS2->SetBinContent(8,6);
   hS2->SetBinContent(9,8);
   hS2->SetBinContent(10,6);
   hS2->SetBinContent(11,48);
   hS2->SetBinError(1,2.645751);
   hS2->SetBinError(2,13.52775);
   hS2->SetBinError(3,19.87461);
   hS2->SetBinError(4,12.8841);
   hS2->SetBinError(5,8.124038);
   hS2->SetBinError(6,6);
   hS2->SetBinError(7,3.605551);
   hS2->SetBinError(8,2.44949);
   hS2->SetBinError(9,2.828427);
   hS2->SetBinError(10,2.44949);
   hS2->SetBinError(11,6.928203);
   hS2->SetEntries(934);
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
   TText *text = pt->AddText("S2 Spectrum - 35 deg, 2.356 kV");
   pt->Draw();
   c6->Modified();
   c6->cd();
   c6->SetSelected(c6);
}
