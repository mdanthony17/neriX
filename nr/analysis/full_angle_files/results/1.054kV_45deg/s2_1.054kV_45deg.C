{
//=========Macro generated from canvas: c6/
//=========  (Thu Feb 11 17:44:05 2016) by ROOT version5.34/26
   TCanvas *c6 = new TCanvas("c6", "",2586,45,640,480);
   c6->SetHighLightColor(2);
   c6->Range(-500,-36.91785,4500,332.2606);
   c6->SetFillColor(0);
   c6->SetBorderMode(0);
   c6->SetBorderSize(2);
   c6->SetFrameBorderMode(0);
   c6->SetFrameBorderMode(0);
   
   TH1F *hS2 = new TH1F("hS2","S2 Spectrum - 45 deg, 1.054 kV",10,0,4000);
   hS2->SetBinContent(1,6);
   hS2->SetBinContent(2,35);
   hS2->SetBinContent(3,129);
   hS2->SetBinContent(4,265);
   hS2->SetBinContent(5,140);
   hS2->SetBinContent(6,49);
   hS2->SetBinContent(7,15);
   hS2->SetBinContent(8,10);
   hS2->SetBinContent(9,5);
   hS2->SetBinContent(10,4);
   hS2->SetBinContent(11,39);
   hS2->SetBinError(1,2.44949);
   hS2->SetBinError(2,5.91608);
   hS2->SetBinError(3,11.35782);
   hS2->SetBinError(4,16.27882);
   hS2->SetBinError(5,11.83216);
   hS2->SetBinError(6,7);
   hS2->SetBinError(7,3.872983);
   hS2->SetBinError(8,3.162278);
   hS2->SetBinError(9,2.236068);
   hS2->SetBinError(10,2);
   hS2->SetBinError(11,6.244998);
   hS2->SetEntries(697);
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
   TText *text = pt->AddText("S2 Spectrum - 45 deg, 1.054 kV");
   pt->Draw();
   c6->Modified();
   c6->cd();
   c6->SetSelected(c6);
}
