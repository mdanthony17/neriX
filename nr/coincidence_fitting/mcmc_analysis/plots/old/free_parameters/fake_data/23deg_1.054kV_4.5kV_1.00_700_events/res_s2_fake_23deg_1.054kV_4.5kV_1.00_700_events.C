{
//=========Macro generated from canvas: Canvas_vQUPDwomZ5BtbozoqrXSRJ/
//=========  (Mon Feb 29 14:03:26 2016) by ROOT version5.34/28
   TCanvas *Canvas_vQUPDwomZ5BtbozoqrXSRJ = new TCanvas("Canvas_vQUPDwomZ5BtbozoqrXSRJ", "",0,0,700,500);
   Canvas_vQUPDwomZ5BtbozoqrXSRJ->SetHighLightColor(2);
   Canvas_vQUPDwomZ5BtbozoqrXSRJ->Range(-0.25,-125.9317,2.25,1133.385);
   Canvas_vQUPDwomZ5BtbozoqrXSRJ->SetFillColor(0);
   Canvas_vQUPDwomZ5BtbozoqrXSRJ->SetBorderMode(0);
   Canvas_vQUPDwomZ5BtbozoqrXSRJ->SetBorderSize(2);
   Canvas_vQUPDwomZ5BtbozoqrXSRJ->SetFrameBorderMode(0);
   Canvas_vQUPDwomZ5BtbozoqrXSRJ->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,929);
   h_res_s2->SetBinContent(2,872);
   h_res_s2->SetBinContent(3,811);
   h_res_s2->SetBinContent(4,812);
   h_res_s2->SetBinContent(5,911);
   h_res_s2->SetBinContent(6,827);
   h_res_s2->SetBinContent(7,849);
   h_res_s2->SetBinContent(8,706);
   h_res_s2->SetBinContent(9,709);
   h_res_s2->SetBinContent(10,622);
   h_res_s2->SetBinContent(11,518);
   h_res_s2->SetBinContent(12,481);
   h_res_s2->SetBinContent(13,368);
   h_res_s2->SetBinContent(14,262);
   h_res_s2->SetBinContent(15,186);
   h_res_s2->SetBinContent(16,138);
   h_res_s2->SetBinContent(17,75);
   h_res_s2->SetBinContent(18,59);
   h_res_s2->SetBinContent(19,26);
   h_res_s2->SetBinContent(20,15);
   h_res_s2->SetBinContent(21,18);
   h_res_s2->SetBinContent(22,9);
   h_res_s2->SetBinContent(23,12);
   h_res_s2->SetBinContent(60,5);
   h_res_s2->SetBinContent(84,10);
   h_res_s2->SetBinContent(94,5);
   h_res_s2->SetBinContent(96,5);
   h_res_s2->SetBinError(1,30.4795);
   h_res_s2->SetBinError(2,29.52965);
   h_res_s2->SetBinError(3,28.47806);
   h_res_s2->SetBinError(4,28.49561);
   h_res_s2->SetBinError(5,30.18278);
   h_res_s2->SetBinError(6,28.75761);
   h_res_s2->SetBinError(7,29.1376);
   h_res_s2->SetBinError(8,26.57066);
   h_res_s2->SetBinError(9,26.62705);
   h_res_s2->SetBinError(10,24.93993);
   h_res_s2->SetBinError(11,22.75961);
   h_res_s2->SetBinError(12,21.93171);
   h_res_s2->SetBinError(13,19.18333);
   h_res_s2->SetBinError(14,16.18641);
   h_res_s2->SetBinError(15,13.63818);
   h_res_s2->SetBinError(16,11.74734);
   h_res_s2->SetBinError(17,8.660254);
   h_res_s2->SetBinError(18,7.681146);
   h_res_s2->SetBinError(19,5.09902);
   h_res_s2->SetBinError(20,3.872983);
   h_res_s2->SetBinError(21,4.242641);
   h_res_s2->SetBinError(22,3);
   h_res_s2->SetBinError(23,3.464102);
   h_res_s2->SetBinError(60,2.236068);
   h_res_s2->SetBinError(84,3.162278);
   h_res_s2->SetBinError(94,2.236068);
   h_res_s2->SetBinError(96,2.236068);
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
   TLine *line = new TLine(0.03603578,0,0.03603578,929);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1189843,0,0.1189843,929);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.221906,0,0.221906,929);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,929);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.12^{-0.08}_{+0.10} ");
   pt->Draw();
   Canvas_vQUPDwomZ5BtbozoqrXSRJ->Modified();
   Canvas_vQUPDwomZ5BtbozoqrXSRJ->cd();
   Canvas_vQUPDwomZ5BtbozoqrXSRJ->SetSelected(Canvas_vQUPDwomZ5BtbozoqrXSRJ);
}
