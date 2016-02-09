{
//=========Macro generated from canvas: c6/
//=========  (Tue Feb  9 11:13:02 2016) by ROOT version5.34/26
   TCanvas *c6 = new TCanvas("c6", "",2524,45,640,480);
   c6->SetHighLightColor(2);
   c6->Range(-500,-37.59416,4500,338.3474);
   c6->SetFillColor(0);
   c6->SetBorderMode(0);
   c6->SetBorderSize(2);
   c6->SetFrameBorderMode(0);
   c6->SetFrameBorderMode(0);
   
   TH1F *hS2 = new TH1F("hS2","S2 Spectrum - 35 deg, 1.054 kV",10,0,4000);
   hS2->SetBinContent(1,4);
   hS2->SetBinContent(2,90);
   hS2->SetBinContent(3,270);
   hS2->SetBinContent(4,148);
   hS2->SetBinContent(5,54);
   hS2->SetBinContent(6,24);
   hS2->SetBinContent(7,12);
   hS2->SetBinContent(8,5);
   hS2->SetBinContent(9,6);
   hS2->SetBinContent(10,6);
   hS2->SetBinContent(11,49);
   hS2->SetBinError(1,2);
   hS2->SetBinError(2,9.486833);
   hS2->SetBinError(3,16.43168);
   hS2->SetBinError(4,12.16553);
   hS2->SetBinError(5,7.348469);
   hS2->SetBinError(6,4.898979);
   hS2->SetBinError(7,3.464102);
   hS2->SetBinError(8,2.236068);
   hS2->SetBinError(9,2.44949);
   hS2->SetBinError(10,2.44949);
   hS2->SetBinError(11,7);
   hS2->SetEntries(668);
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
   TText *text = pt->AddText("S2 Spectrum - 35 deg, 1.054 kV");
   pt->Draw();
   c6->Modified();
   c6->cd();
   c6->SetSelected(c6);
}
