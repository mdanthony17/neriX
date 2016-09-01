{
//=========Macro generated from canvas: Canvas_epTZttTxc2UijctBhmFeyh/
//=========  (Mon Feb 29 14:03:51 2016) by ROOT version5.34/28
   TCanvas *Canvas_epTZttTxc2UijctBhmFeyh = new TCanvas("Canvas_epTZttTxc2UijctBhmFeyh", "",0,0,700,500);
   Canvas_epTZttTxc2UijctBhmFeyh->SetHighLightColor(2);
   Canvas_epTZttTxc2UijctBhmFeyh->Range(-0.25,-148.8613,2.25,1339.752);
   Canvas_epTZttTxc2UijctBhmFeyh->SetFillColor(0);
   Canvas_epTZttTxc2UijctBhmFeyh->SetBorderMode(0);
   Canvas_epTZttTxc2UijctBhmFeyh->SetBorderSize(2);
   Canvas_epTZttTxc2UijctBhmFeyh->SetFrameBorderMode(0);
   Canvas_epTZttTxc2UijctBhmFeyh->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(1,125);
   h_res_s1->SetBinContent(2,211);
   h_res_s1->SetBinContent(3,228);
   h_res_s1->SetBinContent(4,274);
   h_res_s1->SetBinContent(5,365);
   h_res_s1->SetBinContent(6,484);
   h_res_s1->SetBinContent(7,600);
   h_res_s1->SetBinContent(8,771);
   h_res_s1->SetBinContent(9,844);
   h_res_s1->SetBinContent(10,1084);
   h_res_s1->SetBinContent(11,1101);
   h_res_s1->SetBinContent(12,1025);
   h_res_s1->SetBinContent(13,965);
   h_res_s1->SetBinContent(14,825);
   h_res_s1->SetBinContent(15,585);
   h_res_s1->SetBinContent(16,310);
   h_res_s1->SetBinContent(17,202);
   h_res_s1->SetBinContent(18,102);
   h_res_s1->SetBinContent(19,64);
   h_res_s1->SetBinContent(20,55);
   h_res_s1->SetBinContent(21,5);
   h_res_s1->SetBinContent(22,10);
   h_res_s1->SetBinContent(23,5);
   h_res_s1->SetBinError(1,11.18034);
   h_res_s1->SetBinError(2,14.52584);
   h_res_s1->SetBinError(3,15.09967);
   h_res_s1->SetBinError(4,16.55295);
   h_res_s1->SetBinError(5,19.10497);
   h_res_s1->SetBinError(6,22);
   h_res_s1->SetBinError(7,24.4949);
   h_res_s1->SetBinError(8,27.76689);
   h_res_s1->SetBinError(9,29.05168);
   h_res_s1->SetBinError(10,32.92416);
   h_res_s1->SetBinError(11,33.18132);
   h_res_s1->SetBinError(12,32.01562);
   h_res_s1->SetBinError(13,31.06445);
   h_res_s1->SetBinError(14,28.72281);
   h_res_s1->SetBinError(15,24.18677);
   h_res_s1->SetBinError(16,17.60682);
   h_res_s1->SetBinError(17,14.21267);
   h_res_s1->SetBinError(18,10.0995);
   h_res_s1->SetBinError(19,8);
   h_res_s1->SetBinError(20,7.416198);
   h_res_s1->SetBinError(21,2.236068);
   h_res_s1->SetBinError(22,3.162278);
   h_res_s1->SetBinError(23,2.236068);
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
   TLine *line = new TLine(0.1175686,0,0.1175686,1101);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2024342,0,0.2024342,1101);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2729658,0,0.2729658,1101);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3,0,0.3,1101);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.20^{-0.08}_{+0.07} ");
   pt->Draw();
   Canvas_epTZttTxc2UijctBhmFeyh->Modified();
   Canvas_epTZttTxc2UijctBhmFeyh->cd();
   Canvas_epTZttTxc2UijctBhmFeyh->SetSelected(Canvas_epTZttTxc2UijctBhmFeyh);
}
