{
//=========Macro generated from canvas: Canvas_Z8CndW8DtotuywVeYhSSLf/
//=========  (Mon Feb 29 14:03:24 2016) by ROOT version5.34/28
   TCanvas *Canvas_Z8CndW8DtotuywVeYhSSLf = new TCanvas("Canvas_Z8CndW8DtotuywVeYhSSLf", "",0,0,700,500);
   Canvas_Z8CndW8DtotuywVeYhSSLf->SetHighLightColor(2);
   Canvas_Z8CndW8DtotuywVeYhSSLf->Range(-0.25,-180.5465,2.25,1624.918);
   Canvas_Z8CndW8DtotuywVeYhSSLf->SetFillColor(0);
   Canvas_Z8CndW8DtotuywVeYhSSLf->SetBorderMode(0);
   Canvas_Z8CndW8DtotuywVeYhSSLf->SetBorderSize(2);
   Canvas_Z8CndW8DtotuywVeYhSSLf->SetFrameBorderMode(0);
   Canvas_Z8CndW8DtotuywVeYhSSLf->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,592);
   h_res_s2->SetBinContent(2,813);
   h_res_s2->SetBinContent(3,979);
   h_res_s2->SetBinContent(4,1192);
   h_res_s2->SetBinContent(5,1323);
   h_res_s2->SetBinContent(6,1339);
   h_res_s2->SetBinContent(7,1253);
   h_res_s2->SetBinContent(8,1082);
   h_res_s2->SetBinContent(9,704);
   h_res_s2->SetBinContent(10,510);
   h_res_s2->SetBinContent(11,288);
   h_res_s2->SetBinContent(12,115);
   h_res_s2->SetBinContent(13,30);
   h_res_s2->SetBinContent(14,20);
   h_res_s2->SetBinError(1,24.33105);
   h_res_s2->SetBinError(2,28.51315);
   h_res_s2->SetBinError(3,31.28898);
   h_res_s2->SetBinError(4,34.52535);
   h_res_s2->SetBinError(5,36.37307);
   h_res_s2->SetBinError(6,36.59235);
   h_res_s2->SetBinError(7,35.39774);
   h_res_s2->SetBinError(8,32.89377);
   h_res_s2->SetBinError(9,26.533);
   h_res_s2->SetBinError(10,22.58318);
   h_res_s2->SetBinError(11,16.97056);
   h_res_s2->SetBinError(12,10.72381);
   h_res_s2->SetBinError(13,5.477226);
   h_res_s2->SetBinError(14,4.472136);
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
   TLine *line = new TLine(0.04455894,0,0.04455894,1339);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.103301,0,0.103301,1339);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1611034,0,0.1611034,1339);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,1339);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.10^{-0.06}_{+0.06} ");
   pt->Draw();
   Canvas_Z8CndW8DtotuywVeYhSSLf->Modified();
   Canvas_Z8CndW8DtotuywVeYhSSLf->cd();
   Canvas_Z8CndW8DtotuywVeYhSSLf->SetSelected(Canvas_Z8CndW8DtotuywVeYhSSLf);
}
