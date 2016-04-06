{
//=========Macro generated from canvas: Canvas_y5fN8nodFVuA4XvzG75pMn/
//=========  (Mon Feb 29 14:03:57 2016) by ROOT version5.34/28
   TCanvas *Canvas_y5fN8nodFVuA4XvzG75pMn = new TCanvas("Canvas_y5fN8nodFVuA4XvzG75pMn", "",0,0,700,500);
   Canvas_y5fN8nodFVuA4XvzG75pMn->SetHighLightColor(2);
   Canvas_y5fN8nodFVuA4XvzG75pMn->Range(-0.25,-147.3957,2.25,1326.562);
   Canvas_y5fN8nodFVuA4XvzG75pMn->SetFillColor(0);
   Canvas_y5fN8nodFVuA4XvzG75pMn->SetBorderMode(0);
   Canvas_y5fN8nodFVuA4XvzG75pMn->SetBorderSize(2);
   Canvas_y5fN8nodFVuA4XvzG75pMn->SetFrameBorderMode(0);
   Canvas_y5fN8nodFVuA4XvzG75pMn->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,353);
   h_res_s2->SetBinContent(2,536);
   h_res_s2->SetBinContent(3,649);
   h_res_s2->SetBinContent(4,787);
   h_res_s2->SetBinContent(5,819);
   h_res_s2->SetBinContent(6,1010);
   h_res_s2->SetBinContent(7,1009);
   h_res_s2->SetBinContent(8,886);
   h_res_s2->SetBinContent(9,1090);
   h_res_s2->SetBinContent(10,995);
   h_res_s2->SetBinContent(11,806);
   h_res_s2->SetBinContent(12,641);
   h_res_s2->SetBinContent(13,363);
   h_res_s2->SetBinContent(14,144);
   h_res_s2->SetBinContent(15,118);
   h_res_s2->SetBinContent(16,19);
   h_res_s2->SetBinContent(17,15);
   h_res_s2->SetBinError(1,18.78829);
   h_res_s2->SetBinError(2,23.15167);
   h_res_s2->SetBinError(3,25.47548);
   h_res_s2->SetBinError(4,28.05352);
   h_res_s2->SetBinError(5,28.61818);
   h_res_s2->SetBinError(6,31.7805);
   h_res_s2->SetBinError(7,31.76476);
   h_res_s2->SetBinError(8,29.76575);
   h_res_s2->SetBinError(9,33.01515);
   h_res_s2->SetBinError(10,31.54362);
   h_res_s2->SetBinError(11,28.39014);
   h_res_s2->SetBinError(12,25.31798);
   h_res_s2->SetBinError(13,19.05256);
   h_res_s2->SetBinError(14,12);
   h_res_s2->SetBinError(15,10.86278);
   h_res_s2->SetBinError(16,4.358899);
   h_res_s2->SetBinError(17,3.872983);
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
   TLine *line = new TLine(0.06229123,0,0.06229123,1090);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1391477,0,0.1391477,1090);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2118571,0,0.2118571,1090);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,1090);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.14^{-0.08}_{+0.07} ");
   pt->Draw();
   Canvas_y5fN8nodFVuA4XvzG75pMn->Modified();
   Canvas_y5fN8nodFVuA4XvzG75pMn->cd();
   Canvas_y5fN8nodFVuA4XvzG75pMn->SetSelected(Canvas_y5fN8nodFVuA4XvzG75pMn);
}
