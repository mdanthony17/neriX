{
//=========Macro generated from canvas: Canvas_yibVuSCgbEKyhZeyy5T7ae/
//=========  (Mon Feb 29 14:03:24 2016) by ROOT version5.34/28
   TCanvas *Canvas_yibVuSCgbEKyhZeyy5T7ae = new TCanvas("Canvas_yibVuSCgbEKyhZeyy5T7ae", "",0,0,700,500);
   Canvas_yibVuSCgbEKyhZeyy5T7ae->SetHighLightColor(2);
   Canvas_yibVuSCgbEKyhZeyy5T7ae->Range(-0.25,-197.9697,2.25,1781.727);
   Canvas_yibVuSCgbEKyhZeyy5T7ae->SetFillColor(0);
   Canvas_yibVuSCgbEKyhZeyy5T7ae->SetBorderMode(0);
   Canvas_yibVuSCgbEKyhZeyy5T7ae->SetBorderSize(2);
   Canvas_yibVuSCgbEKyhZeyy5T7ae->SetFrameBorderMode(0);
   Canvas_yibVuSCgbEKyhZeyy5T7ae->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(1,8);
   h_res_s1->SetBinContent(2,2);
   h_res_s1->SetBinContent(3,5);
   h_res_s1->SetBinContent(5,5);
   h_res_s1->SetBinContent(6,20);
   h_res_s1->SetBinContent(7,16);
   h_res_s1->SetBinContent(8,79);
   h_res_s1->SetBinContent(9,135);
   h_res_s1->SetBinContent(10,278);
   h_res_s1->SetBinContent(11,377);
   h_res_s1->SetBinContent(12,727);
   h_res_s1->SetBinContent(13,947);
   h_res_s1->SetBinContent(14,1303);
   h_res_s1->SetBinContent(15,1470);
   h_res_s1->SetBinContent(16,1442);
   h_res_s1->SetBinContent(17,1141);
   h_res_s1->SetBinContent(18,879);
   h_res_s1->SetBinContent(19,581);
   h_res_s1->SetBinContent(20,399);
   h_res_s1->SetBinContent(21,163);
   h_res_s1->SetBinContent(22,123);
   h_res_s1->SetBinContent(23,90);
   h_res_s1->SetBinContent(24,25);
   h_res_s1->SetBinContent(25,10);
   h_res_s1->SetBinContent(26,5);
   h_res_s1->SetBinContent(28,5);
   h_res_s1->SetBinContent(29,5);
   h_res_s1->SetBinError(1,2.828427);
   h_res_s1->SetBinError(2,1.414214);
   h_res_s1->SetBinError(3,2.236068);
   h_res_s1->SetBinError(5,2.236068);
   h_res_s1->SetBinError(6,4.472136);
   h_res_s1->SetBinError(7,4);
   h_res_s1->SetBinError(8,8.888194);
   h_res_s1->SetBinError(9,11.61895);
   h_res_s1->SetBinError(10,16.67333);
   h_res_s1->SetBinError(11,19.41649);
   h_res_s1->SetBinError(12,26.96294);
   h_res_s1->SetBinError(13,30.77337);
   h_res_s1->SetBinError(14,36.09709);
   h_res_s1->SetBinError(15,38.34058);
   h_res_s1->SetBinError(16,37.97368);
   h_res_s1->SetBinError(17,33.77869);
   h_res_s1->SetBinError(18,29.64793);
   h_res_s1->SetBinError(19,24.10394);
   h_res_s1->SetBinError(20,19.97498);
   h_res_s1->SetBinError(21,12.76715);
   h_res_s1->SetBinError(22,11.09054);
   h_res_s1->SetBinError(23,9.486833);
   h_res_s1->SetBinError(24,5);
   h_res_s1->SetBinError(25,3.162278);
   h_res_s1->SetBinError(26,2.236068);
   h_res_s1->SetBinError(28,2.236068);
   h_res_s1->SetBinError(29,2.236068);
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
   TLine *line = new TLine(0.2393442,0,0.2393442,1470);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2965714,0,0.2965714,1470);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3549452,0,0.3549452,1470);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3,0,0.3,1470);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.30^{-0.06}_{+0.06} ");
   pt->Draw();
   Canvas_yibVuSCgbEKyhZeyy5T7ae->Modified();
   Canvas_yibVuSCgbEKyhZeyy5T7ae->cd();
   Canvas_yibVuSCgbEKyhZeyy5T7ae->SetSelected(Canvas_yibVuSCgbEKyhZeyy5T7ae);
}
