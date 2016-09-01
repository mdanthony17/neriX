{
//=========Macro generated from canvas: 472928087c9347828686972f33385969/
//=========  (Tue Feb 23 07:33:20 2016) by ROOT version5.34/26
   TCanvas *472928087c9347828686972f33385969 = new TCanvas("472928087c9347828686972f33385969", "",558,45,704,527);
   472928087c9347828686972f33385969->SetHighLightColor(2);
   472928087c9347828686972f33385969->Range(-0.25,-320.2366,2.25,2882.129);
   472928087c9347828686972f33385969->SetFillColor(0);
   472928087c9347828686972f33385969->SetBorderMode(0);
   472928087c9347828686972f33385969->SetBorderSize(2);
   472928087c9347828686972f33385969->SetFrameBorderMode(0);
   472928087c9347828686972f33385969->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(8,5);
   h_res_s1->SetBinContent(9,30);
   h_res_s1->SetBinContent(10,95);
   h_res_s1->SetBinContent(11,269);
   h_res_s1->SetBinContent(12,593);
   h_res_s1->SetBinContent(13,1262);
   h_res_s1->SetBinContent(14,2174);
   h_res_s1->SetBinContent(15,2391);
   h_res_s1->SetBinContent(16,1831);
   h_res_s1->SetBinContent(17,954);
   h_res_s1->SetBinContent(18,431);
   h_res_s1->SetBinContent(19,180);
   h_res_s1->SetBinContent(20,20);
   h_res_s1->SetBinContent(21,5);
   h_res_s1->SetBinError(8,2.236068);
   h_res_s1->SetBinError(9,5.477226);
   h_res_s1->SetBinError(10,9.746794);
   h_res_s1->SetBinError(11,16.40122);
   h_res_s1->SetBinError(12,24.35159);
   h_res_s1->SetBinError(13,35.52464);
   h_res_s1->SetBinError(14,46.62617);
   h_res_s1->SetBinError(15,48.89785);
   h_res_s1->SetBinError(16,42.79019);
   h_res_s1->SetBinError(17,30.88689);
   h_res_s1->SetBinError(18,20.76054);
   h_res_s1->SetBinError(19,13.41641);
   h_res_s1->SetBinError(20,4.472136);
   h_res_s1->SetBinError(21,2.236068);
   h_res_s1->SetEntries(10240);
   h_res_s1->SetStats(0);
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
   
   TPaveText *pt = new TPaveText(0.4335714,0.94,0.5664286,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("res_s1");
   pt->Draw();
   TLine *line = new TLine(0.2500818,0,0.2500818,2391);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2857884,0,0.2857884,2391);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3195832,0,0.3195832,2391);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3,0,0.3,2391);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.29^{-0.04}_{+0.03} ");
   pt->Draw();
   472928087c9347828686972f33385969->Modified();
   472928087c9347828686972f33385969->cd();
   472928087c9347828686972f33385969->SetSelected(472928087c9347828686972f33385969);
}
