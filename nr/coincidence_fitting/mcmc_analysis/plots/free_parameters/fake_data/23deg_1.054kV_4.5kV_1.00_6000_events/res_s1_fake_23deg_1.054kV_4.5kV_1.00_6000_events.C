{
//=========Macro generated from canvas: Canvas_uXYvouLMnVGzr6VtYJDadK/
//=========  (Mon Feb 29 14:03:47 2016) by ROOT version5.34/28
   TCanvas *Canvas_uXYvouLMnVGzr6VtYJDadK = new TCanvas("Canvas_uXYvouLMnVGzr6VtYJDadK", "",0,0,700,500);
   Canvas_uXYvouLMnVGzr6VtYJDadK->SetHighLightColor(2);
   Canvas_uXYvouLMnVGzr6VtYJDadK->Range(-0.25,-197.4379,2.25,1776.941);
   Canvas_uXYvouLMnVGzr6VtYJDadK->SetFillColor(0);
   Canvas_uXYvouLMnVGzr6VtYJDadK->SetBorderMode(0);
   Canvas_uXYvouLMnVGzr6VtYJDadK->SetBorderSize(2);
   Canvas_uXYvouLMnVGzr6VtYJDadK->SetFrameBorderMode(0);
   Canvas_uXYvouLMnVGzr6VtYJDadK->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(1,25);
   h_res_s1->SetBinContent(2,20);
   h_res_s1->SetBinContent(3,40);
   h_res_s1->SetBinContent(4,35);
   h_res_s1->SetBinContent(5,47);
   h_res_s1->SetBinContent(6,33);
   h_res_s1->SetBinContent(7,114);
   h_res_s1->SetBinContent(8,209);
   h_res_s1->SetBinContent(9,409);
   h_res_s1->SetBinContent(10,482);
   h_res_s1->SetBinContent(11,752);
   h_res_s1->SetBinContent(12,1080);
   h_res_s1->SetBinContent(13,1451);
   h_res_s1->SetBinContent(14,1466);
   h_res_s1->SetBinContent(15,1397);
   h_res_s1->SetBinContent(16,1081);
   h_res_s1->SetBinContent(17,646);
   h_res_s1->SetBinContent(18,553);
   h_res_s1->SetBinContent(19,267);
   h_res_s1->SetBinContent(20,80);
   h_res_s1->SetBinContent(21,40);
   h_res_s1->SetBinContent(22,8);
   h_res_s1->SetBinContent(23,5);
   h_res_s1->SetBinError(1,5);
   h_res_s1->SetBinError(2,4.472136);
   h_res_s1->SetBinError(3,6.324555);
   h_res_s1->SetBinError(4,5.91608);
   h_res_s1->SetBinError(5,6.855655);
   h_res_s1->SetBinError(6,5.744563);
   h_res_s1->SetBinError(7,10.67708);
   h_res_s1->SetBinError(8,14.45683);
   h_res_s1->SetBinError(9,20.22375);
   h_res_s1->SetBinError(10,21.9545);
   h_res_s1->SetBinError(11,27.42262);
   h_res_s1->SetBinError(12,32.86335);
   h_res_s1->SetBinError(13,38.09199);
   h_res_s1->SetBinError(14,38.28838);
   h_res_s1->SetBinError(15,37.37646);
   h_res_s1->SetBinError(16,32.87856);
   h_res_s1->SetBinError(17,25.41653);
   h_res_s1->SetBinError(18,23.51595);
   h_res_s1->SetBinError(19,16.34013);
   h_res_s1->SetBinError(20,8.944272);
   h_res_s1->SetBinError(21,6.324555);
   h_res_s1->SetBinError(22,2.828427);
   h_res_s1->SetBinError(23,2.236068);
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
   TLine *line = new TLine(0.2056957,0,0.2056957,1466);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2657708,0,0.2657708,1466);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3194605,0,0.3194605,1466);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3,0,0.3,1466);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.27^{-0.06}_{+0.05} ");
   pt->Draw();
   Canvas_uXYvouLMnVGzr6VtYJDadK->Modified();
   Canvas_uXYvouLMnVGzr6VtYJDadK->cd();
   Canvas_uXYvouLMnVGzr6VtYJDadK->SetSelected(Canvas_uXYvouLMnVGzr6VtYJDadK);
}
