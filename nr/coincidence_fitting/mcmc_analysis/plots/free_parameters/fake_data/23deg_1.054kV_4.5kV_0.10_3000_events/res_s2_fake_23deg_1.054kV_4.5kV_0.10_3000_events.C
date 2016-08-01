{
//=========Macro generated from canvas: Canvas_EkDKxHYxiqhNxFGJ6MzZ9c/
//=========  (Mon Feb 29 14:02:52 2016) by ROOT version5.34/28
   TCanvas *Canvas_EkDKxHYxiqhNxFGJ6MzZ9c = new TCanvas("Canvas_EkDKxHYxiqhNxFGJ6MzZ9c", "",0,0,700,500);
   Canvas_EkDKxHYxiqhNxFGJ6MzZ9c->SetHighLightColor(2);
   Canvas_EkDKxHYxiqhNxFGJ6MzZ9c->Range(-0.25,-201.6924,2.25,1815.232);
   Canvas_EkDKxHYxiqhNxFGJ6MzZ9c->SetFillColor(0);
   Canvas_EkDKxHYxiqhNxFGJ6MzZ9c->SetBorderMode(0);
   Canvas_EkDKxHYxiqhNxFGJ6MzZ9c->SetBorderSize(2);
   Canvas_EkDKxHYxiqhNxFGJ6MzZ9c->SetFrameBorderMode(0);
   Canvas_EkDKxHYxiqhNxFGJ6MzZ9c->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,1135);
   h_res_s2->SetBinContent(2,1320);
   h_res_s2->SetBinContent(3,1327);
   h_res_s2->SetBinContent(4,1498);
   h_res_s2->SetBinContent(5,1350);
   h_res_s2->SetBinContent(6,1114);
   h_res_s2->SetBinContent(7,936);
   h_res_s2->SetBinContent(8,694);
   h_res_s2->SetBinContent(9,468);
   h_res_s2->SetBinContent(10,225);
   h_res_s2->SetBinContent(11,130);
   h_res_s2->SetBinContent(12,26);
   h_res_s2->SetBinContent(13,12);
   h_res_s2->SetBinContent(14,5);
   h_res_s2->SetBinError(1,33.68976);
   h_res_s2->SetBinError(2,36.3318);
   h_res_s2->SetBinError(3,36.42801);
   h_res_s2->SetBinError(4,38.704);
   h_res_s2->SetBinError(5,36.74235);
   h_res_s2->SetBinError(6,33.37664);
   h_res_s2->SetBinError(7,30.59412);
   h_res_s2->SetBinError(8,26.34388);
   h_res_s2->SetBinError(9,21.63331);
   h_res_s2->SetBinError(10,15);
   h_res_s2->SetBinError(11,11.40175);
   h_res_s2->SetBinError(12,5.09902);
   h_res_s2->SetBinError(13,3.464102);
   h_res_s2->SetBinError(14,2.236068);
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
   TLine *line = new TLine(0.02747212,0,0.02747212,1498);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.07786382,0,0.07786382,1498);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1385436,0,0.1385436,1498);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,1498);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.08^{-0.05}_{+0.06} ");
   pt->Draw();
   Canvas_EkDKxHYxiqhNxFGJ6MzZ9c->Modified();
   Canvas_EkDKxHYxiqhNxFGJ6MzZ9c->cd();
   Canvas_EkDKxHYxiqhNxFGJ6MzZ9c->SetSelected(Canvas_EkDKxHYxiqhNxFGJ6MzZ9c);
}
