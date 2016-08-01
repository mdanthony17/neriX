{
//=========Macro generated from canvas: Canvas_gLHfkfEfZLo7kPCWs5fMuZ/
//=========  (Mon Feb 29 14:03:34 2016) by ROOT version5.34/28
   TCanvas *Canvas_gLHfkfEfZLo7kPCWs5fMuZ = new TCanvas("Canvas_gLHfkfEfZLo7kPCWs5fMuZ", "",0,0,700,500);
   Canvas_gLHfkfEfZLo7kPCWs5fMuZ->SetHighLightColor(2);
   Canvas_gLHfkfEfZLo7kPCWs5fMuZ->Range(-0.25,-128.4661,2.25,1156.195);
   Canvas_gLHfkfEfZLo7kPCWs5fMuZ->SetFillColor(0);
   Canvas_gLHfkfEfZLo7kPCWs5fMuZ->SetBorderMode(0);
   Canvas_gLHfkfEfZLo7kPCWs5fMuZ->SetBorderSize(2);
   Canvas_gLHfkfEfZLo7kPCWs5fMuZ->SetFrameBorderMode(0);
   Canvas_gLHfkfEfZLo7kPCWs5fMuZ->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,713);
   h_res_s2->SetBinContent(2,918);
   h_res_s2->SetBinContent(3,886);
   h_res_s2->SetBinContent(4,878);
   h_res_s2->SetBinContent(5,931);
   h_res_s2->SetBinContent(6,948);
   h_res_s2->SetBinContent(7,813);
   h_res_s2->SetBinContent(8,816);
   h_res_s2->SetBinContent(9,720);
   h_res_s2->SetBinContent(10,562);
   h_res_s2->SetBinContent(11,621);
   h_res_s2->SetBinContent(12,422);
   h_res_s2->SetBinContent(13,310);
   h_res_s2->SetBinContent(14,223);
   h_res_s2->SetBinContent(15,199);
   h_res_s2->SetBinContent(16,116);
   h_res_s2->SetBinContent(17,91);
   h_res_s2->SetBinContent(18,10);
   h_res_s2->SetBinContent(19,39);
   h_res_s2->SetBinContent(20,15);
   h_res_s2->SetBinContent(21,5);
   h_res_s2->SetBinContent(22,1);
   h_res_s2->SetBinContent(25,3);
   h_res_s2->SetBinError(1,26.70206);
   h_res_s2->SetBinError(2,30.29851);
   h_res_s2->SetBinError(3,29.76575);
   h_res_s2->SetBinError(4,29.63106);
   h_res_s2->SetBinError(5,30.51229);
   h_res_s2->SetBinError(6,30.78961);
   h_res_s2->SetBinError(7,28.51315);
   h_res_s2->SetBinError(8,28.56571);
   h_res_s2->SetBinError(9,26.83282);
   h_res_s2->SetBinError(10,23.70654);
   h_res_s2->SetBinError(11,24.91987);
   h_res_s2->SetBinError(12,20.54264);
   h_res_s2->SetBinError(13,17.60682);
   h_res_s2->SetBinError(14,14.93318);
   h_res_s2->SetBinError(15,14.10674);
   h_res_s2->SetBinError(16,10.77033);
   h_res_s2->SetBinError(17,9.539392);
   h_res_s2->SetBinError(18,3.162278);
   h_res_s2->SetBinError(19,6.244998);
   h_res_s2->SetBinError(20,3.872983);
   h_res_s2->SetBinError(21,2.236068);
   h_res_s2->SetBinError(22,1);
   h_res_s2->SetBinError(25,1.732051);
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
   TLine *line = new TLine(0.03993813,0,0.03993813,948);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1167511,0,0.1167511,948);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2137469,0,0.2137469,948);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,948);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.12^{-0.08}_{+0.10} ");
   pt->Draw();
   Canvas_gLHfkfEfZLo7kPCWs5fMuZ->Modified();
   Canvas_gLHfkfEfZLo7kPCWs5fMuZ->cd();
   Canvas_gLHfkfEfZLo7kPCWs5fMuZ->SetSelected(Canvas_gLHfkfEfZLo7kPCWs5fMuZ);
}
