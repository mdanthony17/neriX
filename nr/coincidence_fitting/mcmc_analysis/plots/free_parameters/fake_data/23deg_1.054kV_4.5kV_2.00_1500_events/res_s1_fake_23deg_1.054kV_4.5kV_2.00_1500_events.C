{
//=========Macro generated from canvas: Canvas_VWeqBVT3Ks6FtmrPsdZPtK/
//=========  (Mon Feb 29 14:04:38 2016) by ROOT version5.34/28
   TCanvas *Canvas_VWeqBVT3Ks6FtmrPsdZPtK = new TCanvas("Canvas_VWeqBVT3Ks6FtmrPsdZPtK", "",0,0,700,500);
   Canvas_VWeqBVT3Ks6FtmrPsdZPtK->SetHighLightColor(2);
   Canvas_VWeqBVT3Ks6FtmrPsdZPtK->Range(-0.25,-122.0625,2.25,1098.563);
   Canvas_VWeqBVT3Ks6FtmrPsdZPtK->SetFillColor(0);
   Canvas_VWeqBVT3Ks6FtmrPsdZPtK->SetBorderMode(0);
   Canvas_VWeqBVT3Ks6FtmrPsdZPtK->SetBorderSize(2);
   Canvas_VWeqBVT3Ks6FtmrPsdZPtK->SetFrameBorderMode(0);
   Canvas_VWeqBVT3Ks6FtmrPsdZPtK->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(1,177);
   h_res_s1->SetBinContent(2,163);
   h_res_s1->SetBinContent(3,218);
   h_res_s1->SetBinContent(4,267);
   h_res_s1->SetBinContent(5,367);
   h_res_s1->SetBinContent(6,377);
   h_res_s1->SetBinContent(7,525);
   h_res_s1->SetBinContent(8,532);
   h_res_s1->SetBinContent(9,671);
   h_res_s1->SetBinContent(10,766);
   h_res_s1->SetBinContent(11,900);
   h_res_s1->SetBinContent(12,882);
   h_res_s1->SetBinContent(13,891);
   h_res_s1->SetBinContent(14,779);
   h_res_s1->SetBinContent(15,807);
   h_res_s1->SetBinContent(16,567);
   h_res_s1->SetBinContent(17,422);
   h_res_s1->SetBinContent(18,342);
   h_res_s1->SetBinContent(19,262);
   h_res_s1->SetBinContent(20,123);
   h_res_s1->SetBinContent(21,97);
   h_res_s1->SetBinContent(22,23);
   h_res_s1->SetBinContent(23,53);
   h_res_s1->SetBinContent(24,15);
   h_res_s1->SetBinContent(25,9);
   h_res_s1->SetBinContent(26,5);
   h_res_s1->SetBinError(1,13.30413);
   h_res_s1->SetBinError(2,12.76715);
   h_res_s1->SetBinError(3,14.76482);
   h_res_s1->SetBinError(4,16.34013);
   h_res_s1->SetBinError(5,19.15724);
   h_res_s1->SetBinError(6,19.41649);
   h_res_s1->SetBinError(7,22.91288);
   h_res_s1->SetBinError(8,23.06513);
   h_res_s1->SetBinError(9,25.90367);
   h_res_s1->SetBinError(10,27.67671);
   h_res_s1->SetBinError(11,30);
   h_res_s1->SetBinError(12,29.69848);
   h_res_s1->SetBinError(13,29.84962);
   h_res_s1->SetBinError(14,27.91057);
   h_res_s1->SetBinError(15,28.40775);
   h_res_s1->SetBinError(16,23.81176);
   h_res_s1->SetBinError(17,20.54264);
   h_res_s1->SetBinError(18,18.49324);
   h_res_s1->SetBinError(19,16.18641);
   h_res_s1->SetBinError(20,11.09054);
   h_res_s1->SetBinError(21,9.848858);
   h_res_s1->SetBinError(22,4.795832);
   h_res_s1->SetBinError(23,7.28011);
   h_res_s1->SetBinError(24,3.872983);
   h_res_s1->SetBinError(25,3);
   h_res_s1->SetBinError(26,2.236068);
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
   TLine *line = new TLine(0.1222537,0,0.1222537,900);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2235601,0,0.2235601,900);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3102236,0,0.3102236,900);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3,0,0.3,900);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.22^{-0.10}_{+0.09} ");
   pt->Draw();
   Canvas_VWeqBVT3Ks6FtmrPsdZPtK->Modified();
   Canvas_VWeqBVT3Ks6FtmrPsdZPtK->cd();
   Canvas_VWeqBVT3Ks6FtmrPsdZPtK->SetSelected(Canvas_VWeqBVT3Ks6FtmrPsdZPtK);
}
