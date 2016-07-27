{
//=========Macro generated from canvas: Canvas_NuqYP7RhyDJQGLWJWqHusc/
//=========  (Mon Feb 29 14:03:47 2016) by ROOT version5.34/28
   TCanvas *Canvas_NuqYP7RhyDJQGLWJWqHusc = new TCanvas("Canvas_NuqYP7RhyDJQGLWJWqHusc", "",0,0,700,500);
   Canvas_NuqYP7RhyDJQGLWJWqHusc->SetHighLightColor(2);
   Canvas_NuqYP7RhyDJQGLWJWqHusc->Range(-0.25,-145.9301,2.25,1313.371);
   Canvas_NuqYP7RhyDJQGLWJWqHusc->SetFillColor(0);
   Canvas_NuqYP7RhyDJQGLWJWqHusc->SetBorderMode(0);
   Canvas_NuqYP7RhyDJQGLWJWqHusc->SetBorderSize(2);
   Canvas_NuqYP7RhyDJQGLWJWqHusc->SetFrameBorderMode(0);
   Canvas_NuqYP7RhyDJQGLWJWqHusc->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,160);
   h_res_s2->SetBinContent(2,355);
   h_res_s2->SetBinContent(3,427);
   h_res_s2->SetBinContent(4,609);
   h_res_s2->SetBinContent(5,815);
   h_res_s2->SetBinContent(6,847);
   h_res_s2->SetBinContent(7,921);
   h_res_s2->SetBinContent(8,1027);
   h_res_s2->SetBinContent(9,1079);
   h_res_s2->SetBinContent(10,1041);
   h_res_s2->SetBinContent(11,1029);
   h_res_s2->SetBinContent(12,788);
   h_res_s2->SetBinContent(13,544);
   h_res_s2->SetBinContent(14,328);
   h_res_s2->SetBinContent(15,173);
   h_res_s2->SetBinContent(16,76);
   h_res_s2->SetBinContent(17,16);
   h_res_s2->SetBinContent(19,5);
   h_res_s2->SetBinError(1,12.64911);
   h_res_s2->SetBinError(2,18.84144);
   h_res_s2->SetBinError(3,20.66398);
   h_res_s2->SetBinError(4,24.67793);
   h_res_s2->SetBinError(5,28.5482);
   h_res_s2->SetBinError(6,29.10326);
   h_res_s2->SetBinError(7,30.34798);
   h_res_s2->SetBinError(8,32.04684);
   h_res_s2->SetBinError(9,32.84814);
   h_res_s2->SetBinError(10,32.26453);
   h_res_s2->SetBinError(11,32.07803);
   h_res_s2->SetBinError(12,28.07134);
   h_res_s2->SetBinError(13,23.32381);
   h_res_s2->SetBinError(14,18.11077);
   h_res_s2->SetBinError(15,13.15295);
   h_res_s2->SetBinError(16,8.717798);
   h_res_s2->SetBinError(17,4);
   h_res_s2->SetBinError(19,2.236068);
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
   TLine *line = new TLine(0.0818935,0,0.0818935,1079);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1592016,0,0.1592016,1079);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2276609,0,0.2276609,1079);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,1079);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.16^{-0.08}_{+0.07} ");
   pt->Draw();
   Canvas_NuqYP7RhyDJQGLWJWqHusc->Modified();
   Canvas_NuqYP7RhyDJQGLWJWqHusc->cd();
   Canvas_NuqYP7RhyDJQGLWJWqHusc->SetSelected(Canvas_NuqYP7RhyDJQGLWJWqHusc);
}
