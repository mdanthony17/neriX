{
//=========Macro generated from canvas: Canvas_SjmuUD3uH9rFYSf4mrPhyb/
//=========  (Mon Feb 29 14:04:36 2016) by ROOT version5.34/28
   TCanvas *Canvas_SjmuUD3uH9rFYSf4mrPhyb = new TCanvas("Canvas_SjmuUD3uH9rFYSf4mrPhyb", "",0,0,700,500);
   Canvas_SjmuUD3uH9rFYSf4mrPhyb->SetHighLightColor(2);
   Canvas_SjmuUD3uH9rFYSf4mrPhyb->Range(-0.25,-166.7062,2.25,1500.356);
   Canvas_SjmuUD3uH9rFYSf4mrPhyb->SetFillColor(0);
   Canvas_SjmuUD3uH9rFYSf4mrPhyb->SetBorderMode(0);
   Canvas_SjmuUD3uH9rFYSf4mrPhyb->SetBorderSize(2);
   Canvas_SjmuUD3uH9rFYSf4mrPhyb->SetFrameBorderMode(0);
   Canvas_SjmuUD3uH9rFYSf4mrPhyb->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(1,50);
   h_res_s1->SetBinContent(2,61);
   h_res_s1->SetBinContent(3,70);
   h_res_s1->SetBinContent(4,110);
   h_res_s1->SetBinContent(5,70);
   h_res_s1->SetBinContent(6,184);
   h_res_s1->SetBinContent(7,161);
   h_res_s1->SetBinContent(8,303);
   h_res_s1->SetBinContent(9,316);
   h_res_s1->SetBinContent(10,448);
   h_res_s1->SetBinContent(11,605);
   h_res_s1->SetBinContent(12,809);
   h_res_s1->SetBinContent(13,994);
   h_res_s1->SetBinContent(14,1132);
   h_res_s1->SetBinContent(15,1235);
   h_res_s1->SetBinContent(16,1071);
   h_res_s1->SetBinContent(17,851);
   h_res_s1->SetBinContent(18,672);
   h_res_s1->SetBinContent(19,490);
   h_res_s1->SetBinContent(20,298);
   h_res_s1->SetBinContent(21,145);
   h_res_s1->SetBinContent(22,75);
   h_res_s1->SetBinContent(23,55);
   h_res_s1->SetBinContent(24,15);
   h_res_s1->SetBinContent(25,5);
   h_res_s1->SetBinContent(26,10);
   h_res_s1->SetBinContent(27,5);
   h_res_s1->SetBinError(1,7.071068);
   h_res_s1->SetBinError(2,7.81025);
   h_res_s1->SetBinError(3,8.3666);
   h_res_s1->SetBinError(4,10.48809);
   h_res_s1->SetBinError(5,8.3666);
   h_res_s1->SetBinError(6,13.56466);
   h_res_s1->SetBinError(7,12.68858);
   h_res_s1->SetBinError(8,17.4069);
   h_res_s1->SetBinError(9,17.77639);
   h_res_s1->SetBinError(10,21.16601);
   h_res_s1->SetBinError(11,24.59675);
   h_res_s1->SetBinError(12,28.44293);
   h_res_s1->SetBinError(13,31.52777);
   h_res_s1->SetBinError(14,33.64521);
   h_res_s1->SetBinError(15,35.14257);
   h_res_s1->SetBinError(16,32.72614);
   h_res_s1->SetBinError(17,29.1719);
   h_res_s1->SetBinError(18,25.92296);
   h_res_s1->SetBinError(19,22.13594);
   h_res_s1->SetBinError(20,17.26268);
   h_res_s1->SetBinError(21,12.04159);
   h_res_s1->SetBinError(22,8.660254);
   h_res_s1->SetBinError(23,7.416198);
   h_res_s1->SetBinError(24,3.872983);
   h_res_s1->SetBinError(25,2.236068);
   h_res_s1->SetBinError(26,3.162278);
   h_res_s1->SetBinError(27,2.236068);
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
   TLine *line = new TLine(0.1935339,0,0.1935339,1235);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2765901,0,0.2765901,1235);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3442214,0,0.3442214,1235);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3,0,0.3,1235);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.28^{-0.08}_{+0.07} ");
   pt->Draw();
   Canvas_SjmuUD3uH9rFYSf4mrPhyb->Modified();
   Canvas_SjmuUD3uH9rFYSf4mrPhyb->cd();
   Canvas_SjmuUD3uH9rFYSf4mrPhyb->SetSelected(Canvas_SjmuUD3uH9rFYSf4mrPhyb);
}
