{
//=========Macro generated from canvas: Canvas_H9KWSRYMZ6wxznKYaAyunG/
//=========  (Mon Feb 29 14:04:10 2016) by ROOT version5.34/28
   TCanvas *Canvas_H9KWSRYMZ6wxznKYaAyunG = new TCanvas("Canvas_H9KWSRYMZ6wxznKYaAyunG", "",0,0,700,500);
   Canvas_H9KWSRYMZ6wxznKYaAyunG->SetHighLightColor(2);
   Canvas_H9KWSRYMZ6wxznKYaAyunG->Range(-0.25,-186.7988,2.25,1681.189);
   Canvas_H9KWSRYMZ6wxznKYaAyunG->SetFillColor(0);
   Canvas_H9KWSRYMZ6wxznKYaAyunG->SetBorderMode(0);
   Canvas_H9KWSRYMZ6wxznKYaAyunG->SetBorderSize(2);
   Canvas_H9KWSRYMZ6wxznKYaAyunG->SetFrameBorderMode(0);
   Canvas_H9KWSRYMZ6wxznKYaAyunG->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,119);
   h_res_s2->SetBinContent(2,169);
   h_res_s2->SetBinContent(3,225);
   h_res_s2->SetBinContent(4,337);
   h_res_s2->SetBinContent(5,402);
   h_res_s2->SetBinContent(6,483);
   h_res_s2->SetBinContent(7,558);
   h_res_s2->SetBinContent(8,917);
   h_res_s2->SetBinContent(9,1108);
   h_res_s2->SetBinContent(10,1084);
   h_res_s2->SetBinContent(11,1386);
   h_res_s2->SetBinContent(12,1138);
   h_res_s2->SetBinContent(13,1062);
   h_res_s2->SetBinContent(14,680);
   h_res_s2->SetBinContent(15,341);
   h_res_s2->SetBinContent(16,171);
   h_res_s2->SetBinContent(17,45);
   h_res_s2->SetBinContent(18,5);
   h_res_s2->SetBinContent(19,10);
   h_res_s2->SetBinError(1,10.90871);
   h_res_s2->SetBinError(2,13);
   h_res_s2->SetBinError(3,15);
   h_res_s2->SetBinError(4,18.35756);
   h_res_s2->SetBinError(5,20.04994);
   h_res_s2->SetBinError(6,21.97726);
   h_res_s2->SetBinError(7,23.62202);
   h_res_s2->SetBinError(8,30.28201);
   h_res_s2->SetBinError(9,33.28663);
   h_res_s2->SetBinError(10,32.92416);
   h_res_s2->SetBinError(11,37.22902);
   h_res_s2->SetBinError(12,33.73426);
   h_res_s2->SetBinError(13,32.58834);
   h_res_s2->SetBinError(14,26.07681);
   h_res_s2->SetBinError(15,18.46619);
   h_res_s2->SetBinError(16,13.0767);
   h_res_s2->SetBinError(17,6.708204);
   h_res_s2->SetBinError(18,2.236068);
   h_res_s2->SetBinError(19,3.162278);
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
   TLine *line = new TLine(0.115576,0,0.115576,1386);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.194797,0,0.194797,1386);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.252916,0,0.252916,1386);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,1386);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.19^{-0.08}_{+0.06} ");
   pt->Draw();
   Canvas_H9KWSRYMZ6wxznKYaAyunG->Modified();
   Canvas_H9KWSRYMZ6wxznKYaAyunG->cd();
   Canvas_H9KWSRYMZ6wxznKYaAyunG->SetSelected(Canvas_H9KWSRYMZ6wxznKYaAyunG);
}
