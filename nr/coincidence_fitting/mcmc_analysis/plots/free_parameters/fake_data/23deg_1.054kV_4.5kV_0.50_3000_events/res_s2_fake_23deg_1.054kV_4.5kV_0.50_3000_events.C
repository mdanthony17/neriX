{
//=========Macro generated from canvas: Canvas_wSnkH2iYJgAAjLsLxL5qHf/
//=========  (Mon Feb 29 14:03:27 2016) by ROOT version5.34/28
   TCanvas *Canvas_wSnkH2iYJgAAjLsLxL5qHf = new TCanvas("Canvas_wSnkH2iYJgAAjLsLxL5qHf", "",0,0,700,500);
   Canvas_wSnkH2iYJgAAjLsLxL5qHf->SetHighLightColor(2);
   Canvas_wSnkH2iYJgAAjLsLxL5qHf->Range(-0.25,-164.0437,2.25,1476.393);
   Canvas_wSnkH2iYJgAAjLsLxL5qHf->SetFillColor(0);
   Canvas_wSnkH2iYJgAAjLsLxL5qHf->SetBorderMode(0);
   Canvas_wSnkH2iYJgAAjLsLxL5qHf->SetBorderSize(2);
   Canvas_wSnkH2iYJgAAjLsLxL5qHf->SetFrameBorderMode(0);
   Canvas_wSnkH2iYJgAAjLsLxL5qHf->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,541);
   h_res_s2->SetBinContent(2,703);
   h_res_s2->SetBinContent(3,887);
   h_res_s2->SetBinContent(4,912);
   h_res_s2->SetBinContent(5,1113);
   h_res_s2->SetBinContent(6,1077);
   h_res_s2->SetBinContent(7,1215);
   h_res_s2->SetBinContent(8,1076);
   h_res_s2->SetBinContent(9,897);
   h_res_s2->SetBinContent(10,664);
   h_res_s2->SetBinContent(11,487);
   h_res_s2->SetBinContent(12,374);
   h_res_s2->SetBinContent(13,194);
   h_res_s2->SetBinContent(14,65);
   h_res_s2->SetBinContent(15,25);
   h_res_s2->SetBinContent(16,10);
   h_res_s2->SetBinError(1,23.25941);
   h_res_s2->SetBinError(2,26.51415);
   h_res_s2->SetBinError(3,29.78255);
   h_res_s2->SetBinError(4,30.19934);
   h_res_s2->SetBinError(5,33.36165);
   h_res_s2->SetBinError(6,32.81768);
   h_res_s2->SetBinError(7,34.85685);
   h_res_s2->SetBinError(8,32.80244);
   h_res_s2->SetBinError(9,29.94996);
   h_res_s2->SetBinError(10,25.7682);
   h_res_s2->SetBinError(11,22.06808);
   h_res_s2->SetBinError(12,19.33908);
   h_res_s2->SetBinError(13,13.92839);
   h_res_s2->SetBinError(14,8.062258);
   h_res_s2->SetBinError(15,5);
   h_res_s2->SetBinError(16,3.162278);
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
   TLine *line = new TLine(0.04866201,0,0.04866201,1215);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1179016,0,0.1179016,1215);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1857482,0,0.1857482,1215);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,1215);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.12^{-0.07}_{+0.07} ");
   pt->Draw();
   Canvas_wSnkH2iYJgAAjLsLxL5qHf->Modified();
   Canvas_wSnkH2iYJgAAjLsLxL5qHf->cd();
   Canvas_wSnkH2iYJgAAjLsLxL5qHf->SetSelected(Canvas_wSnkH2iYJgAAjLsLxL5qHf);
}
