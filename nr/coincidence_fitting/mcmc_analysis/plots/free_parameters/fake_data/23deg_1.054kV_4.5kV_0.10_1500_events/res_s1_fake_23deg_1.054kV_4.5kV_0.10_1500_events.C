{
//=========Macro generated from canvas: Canvas_zQVrjhe9xR4a8JDgMwpBwd/
//=========  (Mon Feb 29 14:03:01 2016) by ROOT version5.34/28
   TCanvas *Canvas_zQVrjhe9xR4a8JDgMwpBwd = new TCanvas("Canvas_zQVrjhe9xR4a8JDgMwpBwd", "",0,0,700,500);
   Canvas_zQVrjhe9xR4a8JDgMwpBwd->SetHighLightColor(2);
   Canvas_zQVrjhe9xR4a8JDgMwpBwd->Range(-0.25,-109.7808,2.25,988.0274);
   Canvas_zQVrjhe9xR4a8JDgMwpBwd->SetFillColor(0);
   Canvas_zQVrjhe9xR4a8JDgMwpBwd->SetBorderMode(0);
   Canvas_zQVrjhe9xR4a8JDgMwpBwd->SetBorderSize(2);
   Canvas_zQVrjhe9xR4a8JDgMwpBwd->SetFrameBorderMode(0);
   Canvas_zQVrjhe9xR4a8JDgMwpBwd->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(1,382);
   h_res_s1->SetBinContent(2,465);
   h_res_s1->SetBinContent(3,465);
   h_res_s1->SetBinContent(4,504);
   h_res_s1->SetBinContent(5,588);
   h_res_s1->SetBinContent(6,528);
   h_res_s1->SetBinContent(7,565);
   h_res_s1->SetBinContent(8,755);
   h_res_s1->SetBinContent(9,741);
   h_res_s1->SetBinContent(10,807);
   h_res_s1->SetBinContent(11,808);
   h_res_s1->SetBinContent(12,689);
   h_res_s1->SetBinContent(13,594);
   h_res_s1->SetBinContent(14,571);
   h_res_s1->SetBinContent(15,531);
   h_res_s1->SetBinContent(16,382);
   h_res_s1->SetBinContent(17,316);
   h_res_s1->SetBinContent(18,209);
   h_res_s1->SetBinContent(19,103);
   h_res_s1->SetBinContent(20,120);
   h_res_s1->SetBinContent(21,39);
   h_res_s1->SetBinContent(22,40);
   h_res_s1->SetBinContent(23,22);
   h_res_s1->SetBinContent(24,6);
   h_res_s1->SetBinContent(25,7);
   h_res_s1->SetBinContent(27,3);
   h_res_s1->SetBinError(1,19.54482);
   h_res_s1->SetBinError(2,21.56386);
   h_res_s1->SetBinError(3,21.56386);
   h_res_s1->SetBinError(4,22.44994);
   h_res_s1->SetBinError(5,24.24871);
   h_res_s1->SetBinError(6,22.97825);
   h_res_s1->SetBinError(7,23.76973);
   h_res_s1->SetBinError(8,27.47726);
   h_res_s1->SetBinError(9,27.22132);
   h_res_s1->SetBinError(10,28.40775);
   h_res_s1->SetBinError(11,28.42534);
   h_res_s1->SetBinError(12,26.24881);
   h_res_s1->SetBinError(13,24.37212);
   h_res_s1->SetBinError(14,23.89561);
   h_res_s1->SetBinError(15,23.04344);
   h_res_s1->SetBinError(16,19.54482);
   h_res_s1->SetBinError(17,17.77639);
   h_res_s1->SetBinError(18,14.45683);
   h_res_s1->SetBinError(19,10.14889);
   h_res_s1->SetBinError(20,10.95445);
   h_res_s1->SetBinError(21,6.244998);
   h_res_s1->SetBinError(22,6.324555);
   h_res_s1->SetBinError(23,4.690416);
   h_res_s1->SetBinError(24,2.44949);
   h_res_s1->SetBinError(25,2.645751);
   h_res_s1->SetBinError(27,1.732051);
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
   TLine *line = new TLine(0.07254603,0,0.07254603,808);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1831475,0,0.1831475,808);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2856437,0,0.2856437,808);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3,0,0.3,808);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.18^{-0.11}_{+0.10} ");
   pt->Draw();
   Canvas_zQVrjhe9xR4a8JDgMwpBwd->Modified();
   Canvas_zQVrjhe9xR4a8JDgMwpBwd->cd();
   Canvas_zQVrjhe9xR4a8JDgMwpBwd->SetSelected(Canvas_zQVrjhe9xR4a8JDgMwpBwd);
}
