{
//=========Macro generated from canvas: Canvas_owGkGy7XuBSEa5v7PQ5Y9k/
//=========  (Mon Feb 29 14:02:33 2016) by ROOT version5.34/28
   TCanvas *Canvas_owGkGy7XuBSEa5v7PQ5Y9k = new TCanvas("Canvas_owGkGy7XuBSEa5v7PQ5Y9k", "",0,0,700,500);
   Canvas_owGkGy7XuBSEa5v7PQ5Y9k->SetHighLightColor(2);
   Canvas_owGkGy7XuBSEa5v7PQ5Y9k->Range(-0.25,-140.1995,2.25,1261.796);
   Canvas_owGkGy7XuBSEa5v7PQ5Y9k->SetFillColor(0);
   Canvas_owGkGy7XuBSEa5v7PQ5Y9k->SetBorderMode(0);
   Canvas_owGkGy7XuBSEa5v7PQ5Y9k->SetBorderSize(2);
   Canvas_owGkGy7XuBSEa5v7PQ5Y9k->SetFrameBorderMode(0);
   Canvas_owGkGy7XuBSEa5v7PQ5Y9k->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,1036);
   h_res_s2->SetBinContent(2,854);
   h_res_s2->SetBinContent(3,871);
   h_res_s2->SetBinContent(4,944);
   h_res_s2->SetBinContent(5,993);
   h_res_s2->SetBinContent(6,894);
   h_res_s2->SetBinContent(7,866);
   h_res_s2->SetBinContent(8,785);
   h_res_s2->SetBinContent(9,762);
   h_res_s2->SetBinContent(10,667);
   h_res_s2->SetBinContent(11,494);
   h_res_s2->SetBinContent(12,336);
   h_res_s2->SetBinContent(13,307);
   h_res_s2->SetBinContent(14,208);
   h_res_s2->SetBinContent(15,116);
   h_res_s2->SetBinContent(16,44);
   h_res_s2->SetBinContent(17,39);
   h_res_s2->SetBinContent(18,13);
   h_res_s2->SetBinContent(19,10);
   h_res_s2->SetBinContent(21,1);
   h_res_s2->SetBinError(1,32.18695);
   h_res_s2->SetBinError(2,29.22328);
   h_res_s2->SetBinError(3,29.51271);
   h_res_s2->SetBinError(4,30.72458);
   h_res_s2->SetBinError(5,31.5119);
   h_res_s2->SetBinError(6,29.89983);
   h_res_s2->SetBinError(7,29.42788);
   h_res_s2->SetBinError(8,28.01785);
   h_res_s2->SetBinError(9,27.60435);
   h_res_s2->SetBinError(10,25.82634);
   h_res_s2->SetBinError(11,22.22611);
   h_res_s2->SetBinError(12,18.3303);
   h_res_s2->SetBinError(13,17.52142);
   h_res_s2->SetBinError(14,14.42221);
   h_res_s2->SetBinError(15,10.77033);
   h_res_s2->SetBinError(16,6.63325);
   h_res_s2->SetBinError(17,6.244998);
   h_res_s2->SetBinError(18,3.605551);
   h_res_s2->SetBinError(19,3.162278);
   h_res_s2->SetBinError(21,1);
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
   TLine *line = new TLine(0.03386792,0,0.03386792,1036);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1094407,0,0.1094407,1036);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1981961,0,0.1981961,1036);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,1036);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.11^{-0.08}_{+0.09} ");
   pt->Draw();
   Canvas_owGkGy7XuBSEa5v7PQ5Y9k->Modified();
   Canvas_owGkGy7XuBSEa5v7PQ5Y9k->cd();
   Canvas_owGkGy7XuBSEa5v7PQ5Y9k->SetSelected(Canvas_owGkGy7XuBSEa5v7PQ5Y9k);
}
