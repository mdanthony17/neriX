{
//=========Macro generated from canvas: 55ddf31f63a645ad924ed97883406bd0/
//=========  (Mon Feb 15 09:37:35 2016) by ROOT version5.34/26
   TCanvas *55ddf31f63a645ad924ed97883406bd0 = new TCanvas("55ddf31f63a645ad924ed97883406bd0", "",7,126,704,527);
   55ddf31f63a645ad924ed97883406bd0->SetHighLightColor(2);
   55ddf31f63a645ad924ed97883406bd0->Range(-0.25,-215.3829,2.25,1938.446);
   55ddf31f63a645ad924ed97883406bd0->SetFillColor(0);
   55ddf31f63a645ad924ed97883406bd0->SetBorderMode(0);
   55ddf31f63a645ad924ed97883406bd0->SetBorderSize(2);
   55ddf31f63a645ad924ed97883406bd0->SetFrameBorderMode(0);
   55ddf31f63a645ad924ed97883406bd0->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,452);
   h_res_s2->SetBinContent(2,478);
   h_res_s2->SetBinContent(3,750);
   h_res_s2->SetBinContent(4,897);
   h_res_s2->SetBinContent(5,1257);
   h_res_s2->SetBinContent(6,1350);
   h_res_s2->SetBinContent(7,1559);
   h_res_s2->SetBinContent(8,1601);
   h_res_s2->SetBinContent(9,953);
   h_res_s2->SetBinContent(10,620);
   h_res_s2->SetBinContent(11,273);
   h_res_s2->SetBinContent(12,42);
   h_res_s2->SetBinContent(13,7);
   h_res_s2->SetBinContent(14,1);
   h_res_s2->SetBinError(1,21.26029);
   h_res_s2->SetBinError(2,21.86321);
   h_res_s2->SetBinError(3,27.38613);
   h_res_s2->SetBinError(4,29.94996);
   h_res_s2->SetBinError(5,35.4542);
   h_res_s2->SetBinError(6,36.74235);
   h_res_s2->SetBinError(7,39.48417);
   h_res_s2->SetBinError(8,40.0125);
   h_res_s2->SetBinError(9,30.8707);
   h_res_s2->SetBinError(10,24.8998);
   h_res_s2->SetBinError(11,16.52271);
   h_res_s2->SetBinError(12,6.480741);
   h_res_s2->SetBinError(13,2.645751);
   h_res_s2->SetBinError(14,1);
   h_res_s2->SetEntries(10240);
   h_res_s2->SetStats(0);
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
   
   TPaveText *pt = new TPaveText(0.4335714,0.94,0.5664286,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("res_s2");
   pt->Draw();
   TLine *line = new TLine(0.0586176,0,0.0586176,1601);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1190519,0,0.1190519,1601);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.165621,0,0.165621,1601);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.12^{-0.06}_{+0.05} ");
   pt->Draw();
   55ddf31f63a645ad924ed97883406bd0->Modified();
   55ddf31f63a645ad924ed97883406bd0->cd();
   55ddf31f63a645ad924ed97883406bd0->SetSelected(55ddf31f63a645ad924ed97883406bd0);
}
