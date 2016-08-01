{
//=========Macro generated from canvas: Canvas_QG6zT4JKLhaTk5NEhEczQj/
//=========  (Mon Feb 29 14:02:18 2016) by ROOT version5.34/28
   TCanvas *Canvas_QG6zT4JKLhaTk5NEhEczQj = new TCanvas("Canvas_QG6zT4JKLhaTk5NEhEczQj", "",0,0,700,500);
   Canvas_QG6zT4JKLhaTk5NEhEczQj->SetHighLightColor(2);
   Canvas_QG6zT4JKLhaTk5NEhEczQj->Range(-0.25,-188.2619,2.25,1694.357);
   Canvas_QG6zT4JKLhaTk5NEhEczQj->SetFillColor(0);
   Canvas_QG6zT4JKLhaTk5NEhEczQj->SetBorderMode(0);
   Canvas_QG6zT4JKLhaTk5NEhEczQj->SetBorderSize(2);
   Canvas_QG6zT4JKLhaTk5NEhEczQj->SetFrameBorderMode(0);
   Canvas_QG6zT4JKLhaTk5NEhEczQj->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,564);
   h_res_s2->SetBinContent(2,790);
   h_res_s2->SetBinContent(3,928);
   h_res_s2->SetBinContent(4,1023);
   h_res_s2->SetBinContent(5,1317);
   h_res_s2->SetBinContent(6,1397);
   h_res_s2->SetBinContent(7,1358);
   h_res_s2->SetBinContent(8,1164);
   h_res_s2->SetBinContent(9,771);
   h_res_s2->SetBinContent(10,499);
   h_res_s2->SetBinContent(11,246);
   h_res_s2->SetBinContent(12,135);
   h_res_s2->SetBinContent(13,33);
   h_res_s2->SetBinContent(14,15);
   h_res_s2->SetBinError(1,23.74868);
   h_res_s2->SetBinError(2,28.10694);
   h_res_s2->SetBinError(3,30.46309);
   h_res_s2->SetBinError(4,31.98437);
   h_res_s2->SetBinError(5,36.29049);
   h_res_s2->SetBinError(6,37.37646);
   h_res_s2->SetBinError(7,36.85105);
   h_res_s2->SetBinError(8,34.11744);
   h_res_s2->SetBinError(9,27.76689);
   h_res_s2->SetBinError(10,22.33831);
   h_res_s2->SetBinError(11,15.68439);
   h_res_s2->SetBinError(12,11.61895);
   h_res_s2->SetBinError(13,5.744563);
   h_res_s2->SetBinError(14,3.872983);
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
   TLine *line = new TLine(0.04590862,0,0.04590862,1397);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1071296,0,0.1071296,1397);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1618376,0,0.1618376,1397);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,1397);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.11^{-0.06}_{+0.05} ");
   pt->Draw();
   Canvas_QG6zT4JKLhaTk5NEhEczQj->Modified();
   Canvas_QG6zT4JKLhaTk5NEhEczQj->cd();
   Canvas_QG6zT4JKLhaTk5NEhEczQj->SetSelected(Canvas_QG6zT4JKLhaTk5NEhEczQj);
}
