{
//=========Macro generated from canvas: 74e2282b4095425e87c01885c655973e/
//=========  (Mon Apr 11 09:38:34 2016) by ROOT version5.34/26
   TCanvas *74e2282b4095425e87c01885c655973e = new TCanvas("74e2282b4095425e87c01885c655973e", "",10,45,704,527);
   74e2282b4095425e87c01885c655973e->SetHighLightColor(2);
   74e2282b4095425e87c01885c655973e->Range(-0.25,-24.02379,2.25,216.2141);
   74e2282b4095425e87c01885c655973e->SetFillColor(0);
   74e2282b4095425e87c01885c655973e->SetBorderMode(0);
   74e2282b4095425e87c01885c655973e->SetBorderSize(2);
   74e2282b4095425e87c01885c655973e->SetFrameBorderMode(0);
   74e2282b4095425e87c01885c655973e->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(4,5);
   h_res_s2->SetBinContent(5,130);
   h_res_s2->SetBinContent(6,170);
   h_res_s2->SetBinContent(7,15);
   h_res_s2->SetBinError(4,2.236068);
   h_res_s2->SetBinError(5,11.40175);
   h_res_s2->SetBinError(6,13.0384);
   h_res_s2->SetBinError(7,3.872983);
   h_res_s2->SetEntries(320);
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
   TLine *line = new TLine(0.08705846,0,0.08705846,170);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1029412,0,0.1029412,170);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1157788,0,0.1157788,170);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,170);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.10^{-0.02}_{+0.01} ");
   pt->Draw();
   74e2282b4095425e87c01885c655973e->Modified();
   74e2282b4095425e87c01885c655973e->cd();
   74e2282b4095425e87c01885c655973e->SetSelected(74e2282b4095425e87c01885c655973e);
}
