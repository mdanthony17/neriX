{
//=========Macro generated from canvas: Canvas_ZSLRXf7VkbigSYJNSBqL7J/
//=========  (Mon Feb 29 14:03:01 2016) by ROOT version5.34/28
   TCanvas *Canvas_ZSLRXf7VkbigSYJNSBqL7J = new TCanvas("Canvas_ZSLRXf7VkbigSYJNSBqL7J", "",0,0,700,500);
   Canvas_ZSLRXf7VkbigSYJNSBqL7J->SetHighLightColor(2);
   Canvas_ZSLRXf7VkbigSYJNSBqL7J->Range(-0.25,-244.8727,2.25,2203.854);
   Canvas_ZSLRXf7VkbigSYJNSBqL7J->SetFillColor(0);
   Canvas_ZSLRXf7VkbigSYJNSBqL7J->SetBorderMode(0);
   Canvas_ZSLRXf7VkbigSYJNSBqL7J->SetBorderSize(2);
   Canvas_ZSLRXf7VkbigSYJNSBqL7J->SetFrameBorderMode(0);
   Canvas_ZSLRXf7VkbigSYJNSBqL7J->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,1823);
   h_res_s2->SetBinContent(2,1804);
   h_res_s2->SetBinContent(3,1639);
   h_res_s2->SetBinContent(4,1360);
   h_res_s2->SetBinContent(5,1046);
   h_res_s2->SetBinContent(6,863);
   h_res_s2->SetBinContent(7,569);
   h_res_s2->SetBinContent(8,456);
   h_res_s2->SetBinContent(9,271);
   h_res_s2->SetBinContent(10,124);
   h_res_s2->SetBinContent(11,97);
   h_res_s2->SetBinContent(12,79);
   h_res_s2->SetBinContent(13,46);
   h_res_s2->SetBinContent(14,7);
   h_res_s2->SetBinContent(15,32);
   h_res_s2->SetBinContent(16,5);
   h_res_s2->SetBinContent(17,12);
   h_res_s2->SetBinContent(18,6);
   h_res_s2->SetBinContent(19,1);
   h_res_s2->SetBinError(1,42.6966);
   h_res_s2->SetBinError(2,42.47352);
   h_res_s2->SetBinError(3,40.48456);
   h_res_s2->SetBinError(4,36.87818);
   h_res_s2->SetBinError(5,32.34192);
   h_res_s2->SetBinError(6,29.37686);
   h_res_s2->SetBinError(7,23.85372);
   h_res_s2->SetBinError(8,21.35416);
   h_res_s2->SetBinError(9,16.46208);
   h_res_s2->SetBinError(10,11.13553);
   h_res_s2->SetBinError(11,9.848858);
   h_res_s2->SetBinError(12,8.888194);
   h_res_s2->SetBinError(13,6.78233);
   h_res_s2->SetBinError(14,2.645751);
   h_res_s2->SetBinError(15,5.656854);
   h_res_s2->SetBinError(16,2.236068);
   h_res_s2->SetBinError(17,3.464102);
   h_res_s2->SetBinError(18,2.44949);
   h_res_s2->SetBinError(19,1);
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
   TLine *line = new TLine(0.01786242,0,0.01786242,1823);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05821843,0,0.05821843,1823);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1227009,0,0.1227009,1823);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,1823);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.06^{-0.04}_{+0.06} ");
   pt->Draw();
   Canvas_ZSLRXf7VkbigSYJNSBqL7J->Modified();
   Canvas_ZSLRXf7VkbigSYJNSBqL7J->cd();
   Canvas_ZSLRXf7VkbigSYJNSBqL7J->SetSelected(Canvas_ZSLRXf7VkbigSYJNSBqL7J);
}
