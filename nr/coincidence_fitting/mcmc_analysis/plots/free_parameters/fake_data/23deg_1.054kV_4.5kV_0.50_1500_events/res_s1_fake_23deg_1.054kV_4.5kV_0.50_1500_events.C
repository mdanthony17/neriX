{
//=========Macro generated from canvas: Canvas_5D3swenE2nNypfkRp7eZUa/
//=========  (Mon Feb 29 14:03:35 2016) by ROOT version5.34/28
   TCanvas *Canvas_5D3swenE2nNypfkRp7eZUa = new TCanvas("Canvas_5D3swenE2nNypfkRp7eZUa", "",0,0,700,500);
   Canvas_5D3swenE2nNypfkRp7eZUa->SetHighLightColor(2);
   Canvas_5D3swenE2nNypfkRp7eZUa->Range(-0.25,-126.1985,2.25,1135.786);
   Canvas_5D3swenE2nNypfkRp7eZUa->SetFillColor(0);
   Canvas_5D3swenE2nNypfkRp7eZUa->SetBorderMode(0);
   Canvas_5D3swenE2nNypfkRp7eZUa->SetBorderSize(2);
   Canvas_5D3swenE2nNypfkRp7eZUa->SetFrameBorderMode(0);
   Canvas_5D3swenE2nNypfkRp7eZUa->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(1,140);
   h_res_s1->SetBinContent(2,109);
   h_res_s1->SetBinContent(3,130);
   h_res_s1->SetBinContent(4,231);
   h_res_s1->SetBinContent(5,251);
   h_res_s1->SetBinContent(6,265);
   h_res_s1->SetBinContent(7,289);
   h_res_s1->SetBinContent(8,544);
   h_res_s1->SetBinContent(9,485);
   h_res_s1->SetBinContent(10,605);
   h_res_s1->SetBinContent(11,832);
   h_res_s1->SetBinContent(12,914);
   h_res_s1->SetBinContent(13,931);
   h_res_s1->SetBinContent(14,919);
   h_res_s1->SetBinContent(15,872);
   h_res_s1->SetBinContent(16,847);
   h_res_s1->SetBinContent(17,557);
   h_res_s1->SetBinContent(18,406);
   h_res_s1->SetBinContent(19,327);
   h_res_s1->SetBinContent(20,240);
   h_res_s1->SetBinContent(21,151);
   h_res_s1->SetBinContent(22,102);
   h_res_s1->SetBinContent(23,30);
   h_res_s1->SetBinContent(24,17);
   h_res_s1->SetBinContent(25,22);
   h_res_s1->SetBinContent(27,24);
   h_res_s1->SetBinError(1,11.83216);
   h_res_s1->SetBinError(2,10.44031);
   h_res_s1->SetBinError(3,11.40175);
   h_res_s1->SetBinError(4,15.19868);
   h_res_s1->SetBinError(5,15.84298);
   h_res_s1->SetBinError(6,16.27882);
   h_res_s1->SetBinError(7,17);
   h_res_s1->SetBinError(8,23.32381);
   h_res_s1->SetBinError(9,22.02272);
   h_res_s1->SetBinError(10,24.59675);
   h_res_s1->SetBinError(11,28.84441);
   h_res_s1->SetBinError(12,30.23243);
   h_res_s1->SetBinError(13,30.51229);
   h_res_s1->SetBinError(14,30.31501);
   h_res_s1->SetBinError(15,29.52965);
   h_res_s1->SetBinError(16,29.10326);
   h_res_s1->SetBinError(17,23.60085);
   h_res_s1->SetBinError(18,20.14944);
   h_res_s1->SetBinError(19,18.08314);
   h_res_s1->SetBinError(20,15.49193);
   h_res_s1->SetBinError(21,12.28821);
   h_res_s1->SetBinError(22,10.0995);
   h_res_s1->SetBinError(23,5.477226);
   h_res_s1->SetBinError(24,4.123106);
   h_res_s1->SetBinError(25,4.690416);
   h_res_s1->SetBinError(27,4.898979);
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
   TLine *line = new TLine(0.1478368,0,0.1478368,931);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2469817,0,0.2469817,931);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3288991,0,0.3288991,931);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3,0,0.3,931);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.25^{-0.10}_{+0.08} ");
   pt->Draw();
   Canvas_5D3swenE2nNypfkRp7eZUa->Modified();
   Canvas_5D3swenE2nNypfkRp7eZUa->cd();
   Canvas_5D3swenE2nNypfkRp7eZUa->SetSelected(Canvas_5D3swenE2nNypfkRp7eZUa);
}
