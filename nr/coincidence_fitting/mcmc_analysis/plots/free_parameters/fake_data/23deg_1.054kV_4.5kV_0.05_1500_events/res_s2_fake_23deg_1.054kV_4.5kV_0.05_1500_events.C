{
//=========Macro generated from canvas: Canvas_We3MuW8G9CwKAU7LToevuR/
//=========  (Mon Feb 29 14:02:28 2016) by ROOT version5.34/28
   TCanvas *Canvas_We3MuW8G9CwKAU7LToevuR = new TCanvas("Canvas_We3MuW8G9CwKAU7LToevuR", "",0,0,700,500);
   Canvas_We3MuW8G9CwKAU7LToevuR->SetHighLightColor(2);
   Canvas_We3MuW8G9CwKAU7LToevuR->Range(-0.25,-186.6658,2.25,1679.992);
   Canvas_We3MuW8G9CwKAU7LToevuR->SetFillColor(0);
   Canvas_We3MuW8G9CwKAU7LToevuR->SetBorderMode(0);
   Canvas_We3MuW8G9CwKAU7LToevuR->SetBorderSize(2);
   Canvas_We3MuW8G9CwKAU7LToevuR->SetFrameBorderMode(0);
   Canvas_We3MuW8G9CwKAU7LToevuR->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,1115);
   h_res_s2->SetBinContent(2,1221);
   h_res_s2->SetBinContent(3,1385);
   h_res_s2->SetBinContent(4,1292);
   h_res_s2->SetBinContent(5,1092);
   h_res_s2->SetBinContent(6,1026);
   h_res_s2->SetBinContent(7,892);
   h_res_s2->SetBinContent(8,629);
   h_res_s2->SetBinContent(9,614);
   h_res_s2->SetBinContent(10,400);
   h_res_s2->SetBinContent(11,298);
   h_res_s2->SetBinContent(12,152);
   h_res_s2->SetBinContent(13,81);
   h_res_s2->SetBinContent(14,30);
   h_res_s2->SetBinContent(15,8);
   h_res_s2->SetBinContent(16,5);
   h_res_s2->SetBinError(1,33.39162);
   h_res_s2->SetBinError(2,34.94281);
   h_res_s2->SetBinError(3,37.21559);
   h_res_s2->SetBinError(4,35.9444);
   h_res_s2->SetBinError(5,33.04542);
   h_res_s2->SetBinError(6,32.03123);
   h_res_s2->SetBinError(7,29.86637);
   h_res_s2->SetBinError(8,25.07987);
   h_res_s2->SetBinError(9,24.77902);
   h_res_s2->SetBinError(10,20);
   h_res_s2->SetBinError(11,17.26268);
   h_res_s2->SetBinError(12,12.32883);
   h_res_s2->SetBinError(13,9);
   h_res_s2->SetBinError(14,5.477226);
   h_res_s2->SetBinError(15,2.828427);
   h_res_s2->SetBinError(16,2.236068);
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
   TLine *line = new TLine(0.02840557,0,0.02840557,1385);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.08195971,0,0.08195971,1385);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1587231,0,0.1587231,1385);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,1385);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.08^{-0.05}_{+0.08} ");
   pt->Draw();
   Canvas_We3MuW8G9CwKAU7LToevuR->Modified();
   Canvas_We3MuW8G9CwKAU7LToevuR->cd();
   Canvas_We3MuW8G9CwKAU7LToevuR->SetSelected(Canvas_We3MuW8G9CwKAU7LToevuR);
}
