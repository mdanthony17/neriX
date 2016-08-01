{
//=========Macro generated from canvas: Canvas_2NBZ7NbXaMYZtTNFLM95ZR/
//=========  (Mon Feb 29 14:03:50 2016) by ROOT version5.34/28
   TCanvas *Canvas_2NBZ7NbXaMYZtTNFLM95ZR = new TCanvas("Canvas_2NBZ7NbXaMYZtTNFLM95ZR", "",0,0,700,500);
   Canvas_2NBZ7NbXaMYZtTNFLM95ZR->SetHighLightColor(2);
   Canvas_2NBZ7NbXaMYZtTNFLM95ZR->Range(-0.25,-142.0655,2.25,1278.589);
   Canvas_2NBZ7NbXaMYZtTNFLM95ZR->SetFillColor(0);
   Canvas_2NBZ7NbXaMYZtTNFLM95ZR->SetBorderMode(0);
   Canvas_2NBZ7NbXaMYZtTNFLM95ZR->SetBorderSize(2);
   Canvas_2NBZ7NbXaMYZtTNFLM95ZR->SetFrameBorderMode(0);
   Canvas_2NBZ7NbXaMYZtTNFLM95ZR->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,777);
   h_res_s2->SetBinContent(2,908);
   h_res_s2->SetBinContent(3,1043);
   h_res_s2->SetBinContent(4,1050);
   h_res_s2->SetBinContent(5,887);
   h_res_s2->SetBinContent(6,1027);
   h_res_s2->SetBinContent(7,849);
   h_res_s2->SetBinContent(8,680);
   h_res_s2->SetBinContent(9,713);
   h_res_s2->SetBinContent(10,521);
   h_res_s2->SetBinContent(11,423);
   h_res_s2->SetBinContent(12,342);
   h_res_s2->SetBinContent(13,391);
   h_res_s2->SetBinContent(14,249);
   h_res_s2->SetBinContent(15,143);
   h_res_s2->SetBinContent(16,127);
   h_res_s2->SetBinContent(17,53);
   h_res_s2->SetBinContent(18,21);
   h_res_s2->SetBinContent(19,21);
   h_res_s2->SetBinContent(20,5);
   h_res_s2->SetBinContent(22,10);
   h_res_s2->SetBinError(1,27.87472);
   h_res_s2->SetBinError(2,30.13304);
   h_res_s2->SetBinError(3,32.29551);
   h_res_s2->SetBinError(4,32.4037);
   h_res_s2->SetBinError(5,29.78255);
   h_res_s2->SetBinError(6,32.04684);
   h_res_s2->SetBinError(7,29.1376);
   h_res_s2->SetBinError(8,26.07681);
   h_res_s2->SetBinError(9,26.70206);
   h_res_s2->SetBinError(10,22.82542);
   h_res_s2->SetBinError(11,20.56696);
   h_res_s2->SetBinError(12,18.49324);
   h_res_s2->SetBinError(13,19.77372);
   h_res_s2->SetBinError(14,15.77973);
   h_res_s2->SetBinError(15,11.95826);
   h_res_s2->SetBinError(16,11.26943);
   h_res_s2->SetBinError(17,7.28011);
   h_res_s2->SetBinError(18,4.582576);
   h_res_s2->SetBinError(19,4.582576);
   h_res_s2->SetBinError(20,2.236068);
   h_res_s2->SetBinError(22,3.162278);
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
   TLine *line = new TLine(0.03874802,0,0.03874802,1050);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1088608,0,0.1088608,1050);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2074156,0,0.2074156,1050);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,1050);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.11^{-0.07}_{+0.10} ");
   pt->Draw();
   Canvas_2NBZ7NbXaMYZtTNFLM95ZR->Modified();
   Canvas_2NBZ7NbXaMYZtTNFLM95ZR->cd();
   Canvas_2NBZ7NbXaMYZtTNFLM95ZR->SetSelected(Canvas_2NBZ7NbXaMYZtTNFLM95ZR);
}
