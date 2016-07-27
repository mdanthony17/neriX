{
//=========Macro generated from canvas: Canvas_67kCokMwaKLtfZ8LWYCHPT/
//=========  (Mon Feb 29 14:03:46 2016) by ROOT version5.34/28
   TCanvas *Canvas_67kCokMwaKLtfZ8LWYCHPT = new TCanvas("Canvas_67kCokMwaKLtfZ8LWYCHPT", "",0,0,700,500);
   Canvas_67kCokMwaKLtfZ8LWYCHPT->SetHighLightColor(2);
   Canvas_67kCokMwaKLtfZ8LWYCHPT->Range(-0.25,-145.1306,2.25,1306.175);
   Canvas_67kCokMwaKLtfZ8LWYCHPT->SetFillColor(0);
   Canvas_67kCokMwaKLtfZ8LWYCHPT->SetBorderMode(0);
   Canvas_67kCokMwaKLtfZ8LWYCHPT->SetBorderSize(2);
   Canvas_67kCokMwaKLtfZ8LWYCHPT->SetFrameBorderMode(0);
   Canvas_67kCokMwaKLtfZ8LWYCHPT->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,888);
   h_res_s2->SetBinContent(2,1073);
   h_res_s2->SetBinContent(3,927);
   h_res_s2->SetBinContent(4,963);
   h_res_s2->SetBinContent(5,866);
   h_res_s2->SetBinContent(6,990);
   h_res_s2->SetBinContent(7,841);
   h_res_s2->SetBinContent(8,776);
   h_res_s2->SetBinContent(9,652);
   h_res_s2->SetBinContent(10,601);
   h_res_s2->SetBinContent(11,543);
   h_res_s2->SetBinContent(12,298);
   h_res_s2->SetBinContent(13,236);
   h_res_s2->SetBinContent(14,197);
   h_res_s2->SetBinContent(15,145);
   h_res_s2->SetBinContent(16,52);
   h_res_s2->SetBinContent(17,50);
   h_res_s2->SetBinContent(18,56);
   h_res_s2->SetBinContent(19,23);
   h_res_s2->SetBinContent(20,15);
   h_res_s2->SetBinContent(21,20);
   h_res_s2->SetBinContent(22,7);
   h_res_s2->SetBinContent(23,2);
   h_res_s2->SetBinContent(24,4);
   h_res_s2->SetBinContent(27,5);
   h_res_s2->SetBinContent(39,5);
   h_res_s2->SetBinContent(46,5);
   h_res_s2->SetBinError(1,29.79933);
   h_res_s2->SetBinError(2,32.75668);
   h_res_s2->SetBinError(3,30.44667);
   h_res_s2->SetBinError(4,31.03224);
   h_res_s2->SetBinError(5,29.42788);
   h_res_s2->SetBinError(6,31.46427);
   h_res_s2->SetBinError(7,29);
   h_res_s2->SetBinError(8,27.85678);
   h_res_s2->SetBinError(9,25.53429);
   h_res_s2->SetBinError(10,24.5153);
   h_res_s2->SetBinError(11,23.30236);
   h_res_s2->SetBinError(12,17.26268);
   h_res_s2->SetBinError(13,15.36229);
   h_res_s2->SetBinError(14,14.03567);
   h_res_s2->SetBinError(15,12.04159);
   h_res_s2->SetBinError(16,7.211103);
   h_res_s2->SetBinError(17,7.071068);
   h_res_s2->SetBinError(18,7.483315);
   h_res_s2->SetBinError(19,4.795832);
   h_res_s2->SetBinError(20,3.872983);
   h_res_s2->SetBinError(21,4.472136);
   h_res_s2->SetBinError(22,2.645751);
   h_res_s2->SetBinError(23,1.414214);
   h_res_s2->SetBinError(24,2);
   h_res_s2->SetBinError(27,2.236068);
   h_res_s2->SetBinError(39,2.236068);
   h_res_s2->SetBinError(46,2.236068);
   h_res_s2->SetEntries(10240);
   h_res_s2->SetStats(0);
   h_res_s2->SetFillColor(1);
   h_res_s2->SetFillStyle(0);
   h_res_s2->SetMarkerStyle(20);
   h_res_s2->SetMarkerSize(0);
   h_res_s2->GetXaxis()->SetLabelFont(42);
   h_res_s2->GetXaxis()->SetLabelSize(0.035);
   h_res_s2->GetXaxis()->SetTitleSize(0.035);
   h_res_s2->GetXaxis()->SetTitleFont(42);
   h_res_s2->GetYaxis()->SetLabelFont(42);
   h_res_s2->GetYaxis()->SetLabelSize(0.035);
   h_res_s2->GetYaxis()->SetTitleSize(0.035);
   h_res_s2->GetYaxis()->SetTitleFont(42);
   h_res_s2->GetZaxis()->SetLabelFont(42);
   h_res_s2->GetZaxis()->SetLabelSize(0.035);
   h_res_s2->GetZaxis()->SetTitleSize(0.035);
   h_res_s2->GetZaxis()->SetTitleFont(42);
   h_res_s2->Draw("");
   
   TPaveText *pt = new TPaveText(0.4314286,0.935,0.5685714,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("res_s2");
   pt->Draw();
   TLine *line = new TLine(0.03379609,0,0.03379609,1073);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1081414,0,0.1081414,1073);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2012832,0,0.2012832,1073);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,1073);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.11^{-0.07}_{+0.09} ");
   pt->Draw();
   Canvas_67kCokMwaKLtfZ8LWYCHPT->Modified();
   Canvas_67kCokMwaKLtfZ8LWYCHPT->cd();
   Canvas_67kCokMwaKLtfZ8LWYCHPT->SetSelected(Canvas_67kCokMwaKLtfZ8LWYCHPT);
}
