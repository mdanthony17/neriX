{
//=========Macro generated from canvas: cacbc44bee1244d5b40259efa7cf5288/
//=========  (Tue Feb 23 12:06:33 2016) by ROOT version5.34/26
   TCanvas *cacbc44bee1244d5b40259efa7cf5288 = new TCanvas("cacbc44bee1244d5b40259efa7cf5288", "",10,45,704,527);
   cacbc44bee1244d5b40259efa7cf5288->SetHighLightColor(2);
   cacbc44bee1244d5b40259efa7cf5288->Range(-6.875,0.9881721,11.875,5.19892);
   cacbc44bee1244d5b40259efa7cf5288->SetFillColor(0);
   cacbc44bee1244d5b40259efa7cf5288->SetBorderMode(0);
   cacbc44bee1244d5b40259efa7cf5288->SetBorderSize(2);
   cacbc44bee1244d5b40259efa7cf5288->SetLogy();
   cacbc44bee1244d5b40259efa7cf5288->SetFrameBorderMode(0);
   cacbc44bee1244d5b40259efa7cf5288->SetFrameBorderMode(0);
   
   TH1F *hGaus = new TH1F("hGaus","Time between Trigger and First S1 in Waveform - nerix_160223_0947",15,-5,10);
   hGaus->SetBinContent(0,345595);
   hGaus->SetBinContent(1,283);
   hGaus->SetBinContent(2,273);
   hGaus->SetBinContent(3,297);
   hGaus->SetBinContent(4,331);
   hGaus->SetBinContent(5,489);
   hGaus->SetBinContent(6,2406);
   hGaus->SetBinContent(7,31467);
   hGaus->SetBinContent(8,13576);
   hGaus->SetBinContent(9,713);
   hGaus->SetBinContent(10,157);
   hGaus->SetBinContent(11,99);
   hGaus->SetBinContent(12,78);
   hGaus->SetBinContent(13,62);
   hGaus->SetBinContent(14,59);
   hGaus->SetBinContent(15,64);
   hGaus->SetBinContent(16,889983);
   hGaus->SetBinError(0,587.8733);
   hGaus->SetBinError(1,16.8226);
   hGaus->SetBinError(2,16.52271);
   hGaus->SetBinError(3,17.23369);
   hGaus->SetBinError(4,18.19341);
   hGaus->SetBinError(5,22.11334);
   hGaus->SetBinError(6,49.05099);
   hGaus->SetBinError(7,177.3894);
   hGaus->SetBinError(8,116.5161);
   hGaus->SetBinError(9,26.70206);
   hGaus->SetBinError(10,12.52996);
   hGaus->SetBinError(11,9.949874);
   hGaus->SetBinError(12,8.831761);
   hGaus->SetBinError(13,7.874008);
   hGaus->SetBinError(14,7.681146);
   hGaus->SetBinError(15,8);
   hGaus->SetBinError(16,943.3891);
   hGaus->SetEntries(1285932);
   hGaus->SetStats(0);
   hGaus->SetFillStyle(0);
   hGaus->SetMarkerStyle(20);
   hGaus->SetMarkerSize(0);
   hGaus->GetXaxis()->SetTitle("TrigLeftEdge[] - S1sPeak[S1Order[0]]");
   hGaus->GetXaxis()->SetLabelFont(42);
   hGaus->GetXaxis()->SetLabelSize(0.035);
   hGaus->GetXaxis()->SetTitleSize(0.035);
   hGaus->GetXaxis()->SetTitleFont(42);
   hGaus->GetYaxis()->SetTitle("Counts");
   hGaus->GetYaxis()->SetLabelFont(42);
   hGaus->GetYaxis()->SetLabelSize(0.035);
   hGaus->GetYaxis()->SetTitleSize(0.035);
   hGaus->GetYaxis()->SetTitleFont(42);
   hGaus->GetZaxis()->SetLabelFont(42);
   hGaus->GetZaxis()->SetLabelSize(0.035);
   hGaus->GetZaxis()->SetTitleSize(0.035);
   hGaus->GetZaxis()->SetTitleFont(42);
   hGaus->Draw("");
   
   TPaveText *pt = new TPaveText(0.15,0.9374,0.85,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Time between Trigger and First S1 in Waveform - nerix_160223_0947");
   pt->Draw();
   TLine *line = new TLine(0,59,0,31467);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(6,59,6,31467);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   cacbc44bee1244d5b40259efa7cf5288->Modified();
   cacbc44bee1244d5b40259efa7cf5288->cd();
   cacbc44bee1244d5b40259efa7cf5288->SetSelected(cacbc44bee1244d5b40259efa7cf5288);
}
