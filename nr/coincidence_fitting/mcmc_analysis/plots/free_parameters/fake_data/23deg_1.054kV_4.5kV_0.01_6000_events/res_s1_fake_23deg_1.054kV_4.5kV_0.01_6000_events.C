{
//=========Macro generated from canvas: Canvas_EuRWML69pDfcNRn2drjPQD/
//=========  (Mon Feb 29 14:02:18 2016) by ROOT version5.34/28
   TCanvas *Canvas_EuRWML69pDfcNRn2drjPQD = new TCanvas("Canvas_EuRWML69pDfcNRn2drjPQD", "",0,0,700,500);
   Canvas_EuRWML69pDfcNRn2drjPQD->SetHighLightColor(2);
   Canvas_EuRWML69pDfcNRn2drjPQD->Range(-0.25,-173.2281,2.25,1559.053);
   Canvas_EuRWML69pDfcNRn2drjPQD->SetFillColor(0);
   Canvas_EuRWML69pDfcNRn2drjPQD->SetBorderMode(0);
   Canvas_EuRWML69pDfcNRn2drjPQD->SetBorderSize(2);
   Canvas_EuRWML69pDfcNRn2drjPQD->SetFrameBorderMode(0);
   Canvas_EuRWML69pDfcNRn2drjPQD->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(1,16);
   h_res_s1->SetBinContent(2,5);
   h_res_s1->SetBinContent(3,10);
   h_res_s1->SetBinContent(4,39);
   h_res_s1->SetBinContent(5,10);
   h_res_s1->SetBinContent(6,15);
   h_res_s1->SetBinContent(7,65);
   h_res_s1->SetBinContent(8,119);
   h_res_s1->SetBinContent(9,218);
   h_res_s1->SetBinContent(10,324);
   h_res_s1->SetBinContent(11,512);
   h_res_s1->SetBinContent(12,737);
   h_res_s1->SetBinContent(13,964);
   h_res_s1->SetBinContent(14,1135);
   h_res_s1->SetBinContent(15,1284);
   h_res_s1->SetBinContent(16,1280);
   h_res_s1->SetBinContent(17,1061);
   h_res_s1->SetBinContent(18,904);
   h_res_s1->SetBinContent(19,670);
   h_res_s1->SetBinContent(20,381);
   h_res_s1->SetBinContent(21,251);
   h_res_s1->SetBinContent(22,112);
   h_res_s1->SetBinContent(23,50);
   h_res_s1->SetBinContent(24,65);
   h_res_s1->SetBinContent(25,3);
   h_res_s1->SetBinContent(26,5);
   h_res_s1->SetBinContent(28,5);
   h_res_s1->SetBinError(1,4);
   h_res_s1->SetBinError(2,2.236068);
   h_res_s1->SetBinError(3,3.162278);
   h_res_s1->SetBinError(4,6.244998);
   h_res_s1->SetBinError(5,3.162278);
   h_res_s1->SetBinError(6,3.872983);
   h_res_s1->SetBinError(7,8.062258);
   h_res_s1->SetBinError(8,10.90871);
   h_res_s1->SetBinError(9,14.76482);
   h_res_s1->SetBinError(10,18);
   h_res_s1->SetBinError(11,22.62742);
   h_res_s1->SetBinError(12,27.14774);
   h_res_s1->SetBinError(13,31.04835);
   h_res_s1->SetBinError(14,33.68976);
   h_res_s1->SetBinError(15,35.83295);
   h_res_s1->SetBinError(16,35.77709);
   h_res_s1->SetBinError(17,32.57299);
   h_res_s1->SetBinError(18,30.06659);
   h_res_s1->SetBinError(19,25.88436);
   h_res_s1->SetBinError(20,19.51922);
   h_res_s1->SetBinError(21,15.84298);
   h_res_s1->SetBinError(22,10.58301);
   h_res_s1->SetBinError(23,7.071068);
   h_res_s1->SetBinError(24,8.062258);
   h_res_s1->SetBinError(25,1.732051);
   h_res_s1->SetBinError(26,2.236068);
   h_res_s1->SetBinError(28,2.236068);
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
   TLine *line = new TLine(0.2280098,0,0.2280098,1284);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2948131,0,0.2948131,1284);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3580938,0,0.3580938,1284);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3,0,0.3,1284);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.29^{-0.07}_{+0.06} ");
   pt->Draw();
   Canvas_EuRWML69pDfcNRn2drjPQD->Modified();
   Canvas_EuRWML69pDfcNRn2drjPQD->cd();
   Canvas_EuRWML69pDfcNRn2drjPQD->SetSelected(Canvas_EuRWML69pDfcNRn2drjPQD);
}
