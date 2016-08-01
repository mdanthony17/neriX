{
//=========Macro generated from canvas: ee738a2729114ed1bc51ea750bf24319/
//=========  (Fri Feb 12 14:32:44 2016) by ROOT version5.34/26
   TCanvas *ee738a2729114ed1bc51ea750bf24319 = new TCanvas("ee738a2729114ed1bc51ea750bf24319", "",75,77,704,527);
   ee738a2729114ed1bc51ea750bf24319->SetHighLightColor(2);
   ee738a2729114ed1bc51ea750bf24319->Range(-0.25,-291.5899,2.25,2624.309);
   ee738a2729114ed1bc51ea750bf24319->SetFillColor(0);
   ee738a2729114ed1bc51ea750bf24319->SetBorderMode(0);
   ee738a2729114ed1bc51ea750bf24319->SetBorderSize(2);
   ee738a2729114ed1bc51ea750bf24319->SetFrameBorderMode(0);
   ee738a2729114ed1bc51ea750bf24319->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,67);
   h_res_s2->SetBinContent(2,113);
   h_res_s2->SetBinContent(3,188);
   h_res_s2->SetBinContent(4,389);
   h_res_s2->SetBinContent(5,794);
   h_res_s2->SetBinContent(6,1290);
   h_res_s2->SetBinContent(7,1772);
   h_res_s2->SetBinContent(8,2175);
   h_res_s2->SetBinContent(9,1679);
   h_res_s2->SetBinContent(10,1132);
   h_res_s2->SetBinContent(11,467);
   h_res_s2->SetBinContent(12,137);
   h_res_s2->SetBinContent(13,22);
   h_res_s2->SetBinContent(14,10);
   h_res_s2->SetBinContent(17,5);
   h_res_s2->SetBinError(1,8.185353);
   h_res_s2->SetBinError(2,10.63015);
   h_res_s2->SetBinError(3,13.71131);
   h_res_s2->SetBinError(4,19.72308);
   h_res_s2->SetBinError(5,28.17801);
   h_res_s2->SetBinError(6,35.91657);
   h_res_s2->SetBinError(7,42.09513);
   h_res_s2->SetBinError(8,46.6369);
   h_res_s2->SetBinError(9,40.9756);
   h_res_s2->SetBinError(10,33.64521);
   h_res_s2->SetBinError(11,21.61018);
   h_res_s2->SetBinError(12,11.7047);
   h_res_s2->SetBinError(13,4.690416);
   h_res_s2->SetBinError(14,3.162278);
   h_res_s2->SetBinError(17,2.236068);
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
   TLine *line = new TLine(0.1011963,0,0.1011963,2175);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1446621,0,0.1446621,2175);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.182559,0,0.182559,2175);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1,0,0.1,2175);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.14^{-0.04}_{+0.04} ");
   pt->Draw();
   ee738a2729114ed1bc51ea750bf24319->Modified();
   ee738a2729114ed1bc51ea750bf24319->cd();
   ee738a2729114ed1bc51ea750bf24319->SetSelected(ee738a2729114ed1bc51ea750bf24319);
}
