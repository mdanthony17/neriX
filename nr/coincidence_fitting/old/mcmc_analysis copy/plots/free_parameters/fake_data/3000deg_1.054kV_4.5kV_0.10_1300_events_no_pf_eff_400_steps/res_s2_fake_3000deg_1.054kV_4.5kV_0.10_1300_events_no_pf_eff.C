{
//=========Macro generated from canvas: ea96eef8d9d944a1acfea872ea0118aa/
//=========  (Thu Apr 21 16:56:54 2016) by ROOT version5.34/26
   TCanvas *ea96eef8d9d944a1acfea872ea0118aa = new TCanvas("ea96eef8d9d944a1acfea872ea0118aa", "",10,45,704,527);
   ea96eef8d9d944a1acfea872ea0118aa->SetHighLightColor(2);
   ea96eef8d9d944a1acfea872ea0118aa->Range(-0.25,-2469.689,2.25,22227.2);
   ea96eef8d9d944a1acfea872ea0118aa->SetFillColor(0);
   ea96eef8d9d944a1acfea872ea0118aa->SetBorderMode(0);
   ea96eef8d9d944a1acfea872ea0118aa->SetBorderSize(2);
   ea96eef8d9d944a1acfea872ea0118aa->SetFrameBorderMode(0);
   ea96eef8d9d944a1acfea872ea0118aa->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,17313);
   h_res_s2->SetBinContent(2,18680);
   h_res_s2->SetBinContent(3,17308);
   h_res_s2->SetBinContent(4,16597);
   h_res_s2->SetBinContent(5,12059);
   h_res_s2->SetBinContent(6,9966);
   h_res_s2->SetBinContent(7,5875);
   h_res_s2->SetBinContent(8,3000);
   h_res_s2->SetBinContent(9,1232);
   h_res_s2->SetBinContent(10,341);
   h_res_s2->SetBinContent(11,28);
   h_res_s2->SetBinContent(12,1);
   h_res_s2->SetBinError(1,131.5789);
   h_res_s2->SetBinError(2,136.6748);
   h_res_s2->SetBinError(3,131.5599);
   h_res_s2->SetBinError(4,128.8293);
   h_res_s2->SetBinError(5,109.8135);
   h_res_s2->SetBinError(6,99.82986);
   h_res_s2->SetBinError(7,76.64855);
   h_res_s2->SetBinError(8,54.77226);
   h_res_s2->SetBinError(9,35.09986);
   h_res_s2->SetBinError(10,18.46619);
   h_res_s2->SetBinError(11,5.291503);
   h_res_s2->SetBinError(12,1);
   h_res_s2->SetEntries(102400);
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
   TLine *line = new TLine(0.01880853,0,0.01880853,18680);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05757222,0,0.05757222,18680);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1083512,0,0.1083512,18680);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,18680);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.06^{-0.04}_{+0.05} ");
   pt->Draw();
   ea96eef8d9d944a1acfea872ea0118aa->Modified();
   ea96eef8d9d944a1acfea872ea0118aa->cd();
   ea96eef8d9d944a1acfea872ea0118aa->SetSelected(ea96eef8d9d944a1acfea872ea0118aa);
}
