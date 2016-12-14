{
//=========Macro generated from canvas: c6/
//=========  (Fri Mar 11 10:00:04 2016) by ROOT version5.34/26
   TCanvas *c6 = new TCanvas("c6", "",476,180,640,480);
   c6->SetHighLightColor(2);
   c6->Range(-500,-9.170964,4500,82.53867);
   c6->SetFillColor(0);
   c6->SetBorderMode(0);
   c6->SetBorderSize(2);
   c6->SetFrameBorderMode(0);
   c6->SetFrameBorderMode(0);
   
   TH1F *hS2 = new TH1F("hS2","S2 Spectrum - 3000 deg, 1.054 kV",10,0,4000);
   hS2->SetBinContent(1,2);
   hS2->SetBinContent(2,62);
   hS2->SetBinContent(3,35);
   hS2->SetBinContent(4,17);
   hS2->SetBinContent(5,10);
   hS2->SetBinContent(6,3);
   hS2->SetBinContent(7,4);
   hS2->SetBinContent(8,2);
   hS2->SetBinContent(9,1);
   hS2->SetBinContent(10,3);
   hS2->SetBinContent(11,19);
   hS2->SetBinError(1,1.414214);
   hS2->SetBinError(2,7.874008);
   hS2->SetBinError(3,5.91608);
   hS2->SetBinError(4,4.123106);
   hS2->SetBinError(5,3.162278);
   hS2->SetBinError(6,1.732051);
   hS2->SetBinError(7,2);
   hS2->SetBinError(8,1.414214);
   hS2->SetBinError(9,1);
   hS2->SetBinError(10,1.732051);
   hS2->SetBinError(11,4.358899);
   hS2->SetEntries(158);
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
   
   TPaveText *pt = new TPaveText(0.2038365,0.9340729,0.7961635,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("S2 Spectrum - 3000 deg, 1.054 kV");
   pt->Draw();
   c6->Modified();
   c6->cd();
   c6->SetSelected(c6);
}
