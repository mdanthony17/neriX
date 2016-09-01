{
//=========Macro generated from canvas: 701526e2e49d4ac28488fe3f1ca2b2c2/
//=========  (Tue Feb 23 11:19:36 2016) by ROOT version5.34/26
   TCanvas *701526e2e49d4ac28488fe3f1ca2b2c2 = new TCanvas("701526e2e49d4ac28488fe3f1ca2b2c2", "",286,369,704,527);
   701526e2e49d4ac28488fe3f1ca2b2c2->SetHighLightColor(2);
   701526e2e49d4ac28488fe3f1ca2b2c2->Range(-0.25,-408.8854,2.25,3679.969);
   701526e2e49d4ac28488fe3f1ca2b2c2->SetFillColor(0);
   701526e2e49d4ac28488fe3f1ca2b2c2->SetBorderMode(0);
   701526e2e49d4ac28488fe3f1ca2b2c2->SetBorderSize(2);
   701526e2e49d4ac28488fe3f1ca2b2c2->SetFrameBorderMode(0);
   701526e2e49d4ac28488fe3f1ca2b2c2->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,2970);
   h_res_s2->SetBinContent(2,3060);
   h_res_s2->SetBinContent(3,2265);
   h_res_s2->SetBinContent(4,1435);
   h_res_s2->SetBinContent(5,429);
   h_res_s2->SetBinContent(6,71);
   h_res_s2->SetBinContent(7,10);
   h_res_s2->SetBinError(1,54.49771);
   h_res_s2->SetBinError(2,55.31727);
   h_res_s2->SetBinError(3,47.59202);
   h_res_s2->SetBinError(4,37.88139);
   h_res_s2->SetBinError(5,20.71232);
   h_res_s2->SetBinError(6,8.42615);
   h_res_s2->SetBinError(7,3.162278);
   h_res_s2->SetEntries(10240);
   h_res_s2->SetStats(0);
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
   
   TPaveText *pt = new TPaveText(0.4335714,0.94,0.5664286,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("res_s2");
   pt->Draw();
   TLine *line = new TLine(0.01096404,0,0.01096404,3060);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.03405229,0,0.03405229,3060);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.06441589,0,0.06441589,3060);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.03^{-0.02}_{+0.03} ");
   pt->Draw();
   701526e2e49d4ac28488fe3f1ca2b2c2->Modified();
   701526e2e49d4ac28488fe3f1ca2b2c2->cd();
   701526e2e49d4ac28488fe3f1ca2b2c2->SetSelected(701526e2e49d4ac28488fe3f1ca2b2c2);
}
