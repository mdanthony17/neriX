{
//=========Macro generated from canvas: Canvas_iRTTKcq5L3Gaxe6zEw63TA/
//=========  (Mon Feb 29 14:03:23 2016) by ROOT version5.34/28
   TCanvas *Canvas_iRTTKcq5L3Gaxe6zEw63TA = new TCanvas("Canvas_iRTTKcq5L3Gaxe6zEw63TA", "",0,0,700,500);
   Canvas_iRTTKcq5L3Gaxe6zEw63TA->SetHighLightColor(2);
   Canvas_iRTTKcq5L3Gaxe6zEw63TA->Range(-0.25,-203.2877,2.25,1829.589);
   Canvas_iRTTKcq5L3Gaxe6zEw63TA->SetFillColor(0);
   Canvas_iRTTKcq5L3Gaxe6zEw63TA->SetBorderMode(0);
   Canvas_iRTTKcq5L3Gaxe6zEw63TA->SetBorderSize(2);
   Canvas_iRTTKcq5L3Gaxe6zEw63TA->SetFrameBorderMode(0);
   Canvas_iRTTKcq5L3Gaxe6zEw63TA->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(1,15);
   h_res_s1->SetBinContent(2,20);
   h_res_s1->SetBinContent(3,25);
   h_res_s1->SetBinContent(4,15);
   h_res_s1->SetBinContent(5,35);
   h_res_s1->SetBinContent(6,52);
   h_res_s1->SetBinContent(7,65);
   h_res_s1->SetBinContent(8,176);
   h_res_s1->SetBinContent(9,174);
   h_res_s1->SetBinContent(10,400);
   h_res_s1->SetBinContent(11,610);
   h_res_s1->SetBinContent(12,830);
   h_res_s1->SetBinContent(13,1181);
   h_res_s1->SetBinContent(14,1376);
   h_res_s1->SetBinContent(15,1510);
   h_res_s1->SetBinContent(16,1272);
   h_res_s1->SetBinContent(17,992);
   h_res_s1->SetBinContent(18,715);
   h_res_s1->SetBinContent(19,446);
   h_res_s1->SetBinContent(20,182);
   h_res_s1->SetBinContent(21,79);
   h_res_s1->SetBinContent(22,40);
   h_res_s1->SetBinContent(23,20);
   h_res_s1->SetBinContent(24,10);
   h_res_s1->SetBinError(1,3.872983);
   h_res_s1->SetBinError(2,4.472136);
   h_res_s1->SetBinError(3,5);
   h_res_s1->SetBinError(4,3.872983);
   h_res_s1->SetBinError(5,5.91608);
   h_res_s1->SetBinError(6,7.211103);
   h_res_s1->SetBinError(7,8.062258);
   h_res_s1->SetBinError(8,13.2665);
   h_res_s1->SetBinError(9,13.19091);
   h_res_s1->SetBinError(10,20);
   h_res_s1->SetBinError(11,24.69818);
   h_res_s1->SetBinError(12,28.80972);
   h_res_s1->SetBinError(13,34.36568);
   h_res_s1->SetBinError(14,37.09447);
   h_res_s1->SetBinError(15,38.85872);
   h_res_s1->SetBinError(16,35.66511);
   h_res_s1->SetBinError(17,31.49603);
   h_res_s1->SetBinError(18,26.73948);
   h_res_s1->SetBinError(19,21.11871);
   h_res_s1->SetBinError(20,13.49074);
   h_res_s1->SetBinError(21,8.888194);
   h_res_s1->SetBinError(22,6.324555);
   h_res_s1->SetBinError(23,4.472136);
   h_res_s1->SetBinError(24,3.162278);
   h_res_s1->SetEntries(10240);
   h_res_s1->SetStats(0);
   h_res_s1->SetFillColor(1);
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
   
   TPaveText *pt = new TPaveText(0.4342857,0.935,0.5657143,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("res_s1");
   pt->Draw();
   TLine *line = new TLine(0.2209918,0,0.2209918,1510);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2819338,0,0.2819338,1510);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3372548,0,0.3372548,1510);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3,0,0.3,1510);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.28^{-0.06}_{+0.06} ");
   pt->Draw();
   Canvas_iRTTKcq5L3Gaxe6zEw63TA->Modified();
   Canvas_iRTTKcq5L3Gaxe6zEw63TA->cd();
   Canvas_iRTTKcq5L3Gaxe6zEw63TA->SetSelected(Canvas_iRTTKcq5L3Gaxe6zEw63TA);
}
