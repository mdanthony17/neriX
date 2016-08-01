{
//=========Macro generated from canvas: 8c10d87e40b64ca6997ba58ab0a119b8/
//=========  (Mon Apr 11 09:38:33 2016) by ROOT version5.34/26
   TCanvas *8c10d87e40b64ca6997ba58ab0a119b8 = new TCanvas("8c10d87e40b64ca6997ba58ab0a119b8", "",10,45,704,527);
   8c10d87e40b64ca6997ba58ab0a119b8->SetHighLightColor(2);
   8c10d87e40b64ca6997ba58ab0a119b8->Range(2.125,-40.97305,10.875,368.7574);
   8c10d87e40b64ca6997ba58ab0a119b8->SetFillColor(0);
   8c10d87e40b64ca6997ba58ab0a119b8->SetBorderMode(0);
   8c10d87e40b64ca6997ba58ab0a119b8->SetBorderSize(2);
   8c10d87e40b64ca6997ba58ab0a119b8->SetFrameBorderMode(0);
   8c10d87e40b64ca6997ba58ab0a119b8->SetFrameBorderMode(0);
   
   TH1F *h_charge_yield = new TH1F("h_charge_yield","charge_yield",100,3,10);
   h_charge_yield->SetBinContent(43,295);
   h_charge_yield->SetBinContent(44,25);
   h_charge_yield->SetBinError(43,17.17556);
   h_charge_yield->SetBinError(44,5);
   h_charge_yield->SetEntries(320);
   h_charge_yield->SetStats(0);
   h_charge_yield->SetFillStyle(0);
   h_charge_yield->SetMarkerStyle(20);
   h_charge_yield->SetMarkerSize(0);
   h_charge_yield->GetXaxis()->SetLabelFont(42);
   h_charge_yield->GetXaxis()->SetLabelSize(0.035);
   h_charge_yield->GetXaxis()->SetTitleSize(0.035);
   h_charge_yield->GetXaxis()->SetTitleFont(42);
   h_charge_yield->GetYaxis()->SetLabelFont(42);
   h_charge_yield->GetYaxis()->SetLabelSize(0.035);
   h_charge_yield->GetYaxis()->SetTitleSize(0.035);
   h_charge_yield->GetYaxis()->SetTitleFont(42);
   h_charge_yield->GetZaxis()->SetLabelFont(42);
   h_charge_yield->GetZaxis()->SetLabelSize(0.035);
   h_charge_yield->GetZaxis()->SetTitleSize(0.035);
   h_charge_yield->GetZaxis()->SetTitleFont(42);
   h_charge_yield->Draw("");
   
   TPaveText *pt = new TPaveText(0.3814286,0.9374,0.6185714,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("charge_yield");
   pt->Draw();
   TLine *line = new TLine(5.952073,0,5.952073,295);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(5.977966,0,5.977966,295);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(6.003859,0,6.003859,295);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(6.78,0,6.78,295);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("Q_{y} = 5.98^{-0.03}_{+0.03} #frac{electrons}{keV}");
   pt->Draw();
   8c10d87e40b64ca6997ba58ab0a119b8->Modified();
   8c10d87e40b64ca6997ba58ab0a119b8->cd();
   8c10d87e40b64ca6997ba58ab0a119b8->SetSelected(8c10d87e40b64ca6997ba58ab0a119b8);
}
