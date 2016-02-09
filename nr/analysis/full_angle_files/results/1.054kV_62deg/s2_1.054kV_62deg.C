{
//=========Macro generated from canvas: c6/
//=========  (Tue Feb  9 11:15:34 2016) by ROOT version5.34/26
   TCanvas *c6 = new TCanvas("c6", "",2574,55,640,480);
   c6->SetHighLightColor(2);
   c6->Range(-500,-51.89504,4500,467.0553);
   c6->SetFillColor(0);
   c6->SetBorderMode(0);
   c6->SetBorderSize(2);
   c6->SetFrameBorderMode(0);
   c6->SetFrameBorderMode(0);
   
   TH1F *hS2 = new TH1F("hS2","S2 Spectrum - 62 deg, 1.054 kV",10,0,4000);
   hS2->SetBinContent(1,10);
   hS2->SetBinContent(2,78);
   hS2->SetBinContent(3,193);
   hS2->SetBinContent(4,277);
   hS2->SetBinContent(5,376);
   hS2->SetBinContent(6,327);
   hS2->SetBinContent(7,186);
   hS2->SetBinContent(8,77);
   hS2->SetBinContent(9,48);
   hS2->SetBinContent(10,47);
   hS2->SetBinContent(11,276);
   hS2->SetBinError(1,3.162278);
   hS2->SetBinError(2,8.831761);
   hS2->SetBinError(3,13.89244);
   hS2->SetBinError(4,16.64332);
   hS2->SetBinError(5,19.39072);
   hS2->SetBinError(6,18.08314);
   hS2->SetBinError(7,13.63818);
   hS2->SetBinError(8,8.774964);
   hS2->SetBinError(9,6.928203);
   hS2->SetBinError(10,6.855655);
   hS2->SetBinError(11,16.61325);
   hS2->SetEntries(1895);
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
   TText *text = pt->AddText("S2 Spectrum - 62 deg, 1.054 kV");
   pt->Draw();
   c6->Modified();
   c6->cd();
   c6->SetSelected(c6);
}
