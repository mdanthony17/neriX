{
//=========Macro generated from canvas: ec0442f67e654a19b132c28badc71d98/
//=========  (Sat Feb 13 16:46:34 2016) by ROOT version5.34/26
   TCanvas *ec0442f67e654a19b132c28badc71d98 = new TCanvas("ec0442f67e654a19b132c28badc71d98", "",233,230,704,527);
   ec0442f67e654a19b132c28badc71d98->SetHighLightColor(2);
   ec0442f67e654a19b132c28badc71d98->Range(-0.25,-208.0732,2.25,1872.658);
   ec0442f67e654a19b132c28badc71d98->SetFillColor(0);
   ec0442f67e654a19b132c28badc71d98->SetBorderMode(0);
   ec0442f67e654a19b132c28badc71d98->SetBorderSize(2);
   ec0442f67e654a19b132c28badc71d98->SetFrameBorderMode(0);
   ec0442f67e654a19b132c28badc71d98->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(1,10);
   h_res_s1->SetBinContent(3,13);
   h_res_s1->SetBinContent(4,13);
   h_res_s1->SetBinContent(5,7);
   h_res_s1->SetBinContent(6,20);
   h_res_s1->SetBinContent(7,32);
   h_res_s1->SetBinContent(8,46);
   h_res_s1->SetBinContent(9,120);
   h_res_s1->SetBinContent(10,187);
   h_res_s1->SetBinContent(11,376);
   h_res_s1->SetBinContent(12,692);
   h_res_s1->SetBinContent(13,942);
   h_res_s1->SetBinContent(14,1346);
   h_res_s1->SetBinContent(15,1546);
   h_res_s1->SetBinContent(16,1344);
   h_res_s1->SetBinContent(17,1410);
   h_res_s1->SetBinContent(18,1032);
   h_res_s1->SetBinContent(19,608);
   h_res_s1->SetBinContent(20,268);
   h_res_s1->SetBinContent(21,145);
   h_res_s1->SetBinContent(22,62);
   h_res_s1->SetBinContent(23,6);
   h_res_s1->SetBinContent(24,15);
   h_res_s1->SetBinError(1,3.162278);
   h_res_s1->SetBinError(3,3.605551);
   h_res_s1->SetBinError(4,3.605551);
   h_res_s1->SetBinError(5,2.645751);
   h_res_s1->SetBinError(6,4.472136);
   h_res_s1->SetBinError(7,5.656854);
   h_res_s1->SetBinError(8,6.78233);
   h_res_s1->SetBinError(9,10.95445);
   h_res_s1->SetBinError(10,13.67479);
   h_res_s1->SetBinError(11,19.39072);
   h_res_s1->SetBinError(12,26.30589);
   h_res_s1->SetBinError(13,30.69202);
   h_res_s1->SetBinError(14,36.68787);
   h_res_s1->SetBinError(15,39.31921);
   h_res_s1->SetBinError(16,36.66061);
   h_res_s1->SetBinError(17,37.54997);
   h_res_s1->SetBinError(18,32.12476);
   h_res_s1->SetBinError(19,24.65766);
   h_res_s1->SetBinError(20,16.37071);
   h_res_s1->SetBinError(21,12.04159);
   h_res_s1->SetBinError(22,7.874008);
   h_res_s1->SetBinError(23,2.44949);
   h_res_s1->SetBinError(24,3.872983);
   h_res_s1->SetEntries(10240);
   h_res_s1->SetStats(0);
   h_res_s1->SetFillStyle(0);
   h_res_s1->SetMarkerStyle(20);
   h_res_s1->SetMarkerSize(0);
   h_res_s1->GetXaxis()->SetLabelFont(42);
   h_res_s1->GetXaxis()->SetLabelSize(0.035);
   h_res_s1->GetXaxis()->SetTitleSize(0.035);
   h_res_s1->GetXaxis()->SetTitleFont(42);
   h_res_s1->GetYaxis()->SetLabelFont(42);
   h_res_s1->GetYaxis()->SetLabelSize(0.035);
   h_res_s1->GetYaxis()->SetTitleSize(0.035);
   h_res_s1->GetYaxis()->SetTitleFont(42);
   h_res_s1->GetZaxis()->SetLabelFont(42);
   h_res_s1->GetZaxis()->SetLabelSize(0.035);
   h_res_s1->GetZaxis()->SetTitleSize(0.035);
   h_res_s1->GetZaxis()->SetTitleFont(42);
   h_res_s1->Draw("");
   
   TPaveText *pt = new TPaveText(0.4335714,0.94,0.5664286,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("res_s1");
   pt->Draw();
   TLine *line = new TLine(0.2423813,0,0.2423813,1546);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2970246,0,0.2970246,1546);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3498419,0,0.3498419,1546);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.30^{-0.05}_{+0.05} ");
   pt->Draw();
   ec0442f67e654a19b132c28badc71d98->Modified();
   ec0442f67e654a19b132c28badc71d98->cd();
   ec0442f67e654a19b132c28badc71d98->SetSelected(ec0442f67e654a19b132c28badc71d98);
}
