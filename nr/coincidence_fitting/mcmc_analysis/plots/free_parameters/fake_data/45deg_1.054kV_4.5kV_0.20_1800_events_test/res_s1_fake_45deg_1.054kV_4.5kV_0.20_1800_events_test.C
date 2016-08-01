{
//=========Macro generated from canvas: 0f59b7bd201c4b7f90b4f6c83814deda/
//=========  (Mon Apr 11 09:38:34 2016) by ROOT version5.34/26
   TCanvas *0f59b7bd201c4b7f90b4f6c83814deda = new TCanvas("0f59b7bd201c4b7f90b4f6c83814deda", "",10,45,704,527);
   0f59b7bd201c4b7f90b4f6c83814deda->SetHighLightColor(2);
   0f59b7bd201c4b7f90b4f6c83814deda->Range(-0.25,-42.9984,2.25,386.9856);
   0f59b7bd201c4b7f90b4f6c83814deda->SetFillColor(0);
   0f59b7bd201c4b7f90b4f6c83814deda->SetBorderMode(0);
   0f59b7bd201c4b7f90b4f6c83814deda->SetBorderSize(2);
   0f59b7bd201c4b7f90b4f6c83814deda->SetFrameBorderMode(0);
   0f59b7bd201c4b7f90b4f6c83814deda->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(12,5);
   h_res_s1->SetBinContent(13,310);
   h_res_s1->SetBinContent(14,5);
   h_res_s1->SetBinError(12,2.236068);
   h_res_s1->SetBinError(13,17.60682);
   h_res_s1->SetBinError(14,2.236068);
   h_res_s1->SetEntries(320);
   h_res_s1->SetStats(0);
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
   
   TPaveText *pt = new TPaveText(0.4335714,0.94,0.5664286,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("res_s1");
   pt->Draw();
   TLine *line = new TLine(0.24296,0,0.24296,310);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.25,0,0.25,310);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.25704,0,0.25704,310);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3,0,0.3,310);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.25^{-0.01}_{+0.01} ");
   pt->Draw();
   0f59b7bd201c4b7f90b4f6c83814deda->Modified();
   0f59b7bd201c4b7f90b4f6c83814deda->cd();
   0f59b7bd201c4b7f90b4f6c83814deda->SetSelected(0f59b7bd201c4b7f90b4f6c83814deda);
}
