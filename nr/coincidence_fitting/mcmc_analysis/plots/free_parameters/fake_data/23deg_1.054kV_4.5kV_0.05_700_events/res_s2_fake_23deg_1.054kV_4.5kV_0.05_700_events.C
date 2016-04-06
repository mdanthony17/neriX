{
//=========Macro generated from canvas: Canvas_3XhWmuaiNNZQeRQDz2atXC/
//=========  (Mon Feb 29 14:02:41 2016) by ROOT version5.34/28
   TCanvas *Canvas_3XhWmuaiNNZQeRQDz2atXC = new TCanvas("Canvas_3XhWmuaiNNZQeRQDz2atXC", "",0,0,700,500);
   Canvas_3XhWmuaiNNZQeRQDz2atXC->SetHighLightColor(2);
   Canvas_3XhWmuaiNNZQeRQDz2atXC->Range(-0.25,-169.3684,2.25,1524.316);
   Canvas_3XhWmuaiNNZQeRQDz2atXC->SetFillColor(0);
   Canvas_3XhWmuaiNNZQeRQDz2atXC->SetBorderMode(0);
   Canvas_3XhWmuaiNNZQeRQDz2atXC->SetBorderSize(2);
   Canvas_3XhWmuaiNNZQeRQDz2atXC->SetFrameBorderMode(0);
   Canvas_3XhWmuaiNNZQeRQDz2atXC->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,1255);
   h_res_s2->SetBinContent(2,1238);
   h_res_s2->SetBinContent(3,1193);
   h_res_s2->SetBinContent(4,1092);
   h_res_s2->SetBinContent(5,1131);
   h_res_s2->SetBinContent(6,1029);
   h_res_s2->SetBinContent(7,762);
   h_res_s2->SetBinContent(8,748);
   h_res_s2->SetBinContent(9,519);
   h_res_s2->SetBinContent(10,350);
   h_res_s2->SetBinContent(11,354);
   h_res_s2->SetBinContent(12,232);
   h_res_s2->SetBinContent(13,126);
   h_res_s2->SetBinContent(14,63);
   h_res_s2->SetBinContent(15,42);
   h_res_s2->SetBinContent(16,43);
   h_res_s2->SetBinContent(17,24);
   h_res_s2->SetBinContent(18,19);
   h_res_s2->SetBinContent(19,9);
   h_res_s2->SetBinContent(20,6);
   h_res_s2->SetBinContent(24,1);
   h_res_s2->SetBinContent(25,4);
   h_res_s2->SetBinError(1,35.42598);
   h_res_s2->SetBinError(2,35.18522);
   h_res_s2->SetBinError(3,34.53983);
   h_res_s2->SetBinError(4,33.04542);
   h_res_s2->SetBinError(5,33.63034);
   h_res_s2->SetBinError(6,32.07803);
   h_res_s2->SetBinError(7,27.60435);
   h_res_s2->SetBinError(8,27.34959);
   h_res_s2->SetBinError(9,22.78157);
   h_res_s2->SetBinError(10,18.70829);
   h_res_s2->SetBinError(11,18.81489);
   h_res_s2->SetBinError(12,15.23155);
   h_res_s2->SetBinError(13,11.22497);
   h_res_s2->SetBinError(14,7.937254);
   h_res_s2->SetBinError(15,6.480741);
   h_res_s2->SetBinError(16,6.557439);
   h_res_s2->SetBinError(17,4.898979);
   h_res_s2->SetBinError(18,4.358899);
   h_res_s2->SetBinError(19,3);
   h_res_s2->SetBinError(20,2.44949);
   h_res_s2->SetBinError(24,1);
   h_res_s2->SetBinError(25,2);
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
   TLine *line = new TLine(0.02602843,0,0.02602843,1255);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.08604775,0,0.08604775,1255);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1663137,0,0.1663137,1255);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,1255);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.09^{-0.06}_{+0.08} ");
   pt->Draw();
   Canvas_3XhWmuaiNNZQeRQDz2atXC->Modified();
   Canvas_3XhWmuaiNNZQeRQDz2atXC->cd();
   Canvas_3XhWmuaiNNZQeRQDz2atXC->SetSelected(Canvas_3XhWmuaiNNZQeRQDz2atXC);
}
