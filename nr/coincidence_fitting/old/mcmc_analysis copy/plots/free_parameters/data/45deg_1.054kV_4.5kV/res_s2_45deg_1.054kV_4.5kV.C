{
//=========Macro generated from canvas: cb8719fa959247189dab3c8f11ccaeee/
//=========  (Sat Feb 13 16:46:33 2016) by ROOT version5.34/26
   TCanvas *cb8719fa959247189dab3c8f11ccaeee = new TCanvas("cb8719fa959247189dab3c8f11ccaeee", "",106,290,704,527);
   cb8719fa959247189dab3c8f11ccaeee->SetHighLightColor(2);
   cb8719fa959247189dab3c8f11ccaeee->Range(-0.25,-553.5395,2.25,4981.855);
   cb8719fa959247189dab3c8f11ccaeee->SetFillColor(0);
   cb8719fa959247189dab3c8f11ccaeee->SetBorderMode(0);
   cb8719fa959247189dab3c8f11ccaeee->SetBorderSize(2);
   cb8719fa959247189dab3c8f11ccaeee->SetFrameBorderMode(0);
   cb8719fa959247189dab3c8f11ccaeee->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,4153);
   h_res_s2->SetBinContent(2,3426);
   h_res_s2->SetBinContent(3,1966);
   h_res_s2->SetBinContent(4,582);
   h_res_s2->SetBinContent(5,98);
   h_res_s2->SetBinContent(6,15);
   h_res_s2->SetBinError(1,64.44377);
   h_res_s2->SetBinError(2,58.53204);
   h_res_s2->SetBinError(3,44.3396);
   h_res_s2->SetBinError(4,24.12468);
   h_res_s2->SetBinError(5,9.899495);
   h_res_s2->SetBinError(6,3.872983);
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
   TLine *line = new TLine(0.007840886,0,0.007840886,4153);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.02564507,0,0.02564507,4153);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05050702,0,0.05050702,4153);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.03^{-0.02}_{+0.02} ");
   pt->Draw();
   cb8719fa959247189dab3c8f11ccaeee->Modified();
   cb8719fa959247189dab3c8f11ccaeee->cd();
   cb8719fa959247189dab3c8f11ccaeee->SetSelected(cb8719fa959247189dab3c8f11ccaeee);
}
