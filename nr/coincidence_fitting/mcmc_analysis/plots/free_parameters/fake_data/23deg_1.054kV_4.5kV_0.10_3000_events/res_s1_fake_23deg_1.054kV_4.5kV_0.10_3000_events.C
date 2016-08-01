{
//=========Macro generated from canvas: Canvas_Xn8rJYNbpG6kL4ExD25DuZ/
//=========  (Mon Feb 29 14:02:52 2016) by ROOT version5.34/28
   TCanvas *Canvas_Xn8rJYNbpG6kL4ExD25DuZ = new TCanvas("Canvas_Xn8rJYNbpG6kL4ExD25DuZ", "",0,0,700,500);
   Canvas_Xn8rJYNbpG6kL4ExD25DuZ->SetHighLightColor(2);
   Canvas_Xn8rJYNbpG6kL4ExD25DuZ->Range(-0.25,-151.1261,2.25,1360.134);
   Canvas_Xn8rJYNbpG6kL4ExD25DuZ->SetFillColor(0);
   Canvas_Xn8rJYNbpG6kL4ExD25DuZ->SetBorderMode(0);
   Canvas_Xn8rJYNbpG6kL4ExD25DuZ->SetBorderSize(2);
   Canvas_Xn8rJYNbpG6kL4ExD25DuZ->SetFrameBorderMode(0);
   Canvas_Xn8rJYNbpG6kL4ExD25DuZ->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(1,85);
   h_res_s1->SetBinContent(2,88);
   h_res_s1->SetBinContent(3,106);
   h_res_s1->SetBinContent(4,135);
   h_res_s1->SetBinContent(5,202);
   h_res_s1->SetBinContent(6,234);
   h_res_s1->SetBinContent(7,308);
   h_res_s1->SetBinContent(8,525);
   h_res_s1->SetBinContent(9,562);
   h_res_s1->SetBinContent(10,777);
   h_res_s1->SetBinContent(11,953);
   h_res_s1->SetBinContent(12,981);
   h_res_s1->SetBinContent(13,1083);
   h_res_s1->SetBinContent(14,1118);
   h_res_s1->SetBinContent(15,910);
   h_res_s1->SetBinContent(16,750);
   h_res_s1->SetBinContent(17,553);
   h_res_s1->SetBinContent(18,378);
   h_res_s1->SetBinContent(19,204);
   h_res_s1->SetBinContent(20,110);
   h_res_s1->SetBinContent(21,73);
   h_res_s1->SetBinContent(22,43);
   h_res_s1->SetBinContent(23,32);
   h_res_s1->SetBinContent(24,15);
   h_res_s1->SetBinContent(25,5);
   h_res_s1->SetBinContent(26,9);
   h_res_s1->SetBinContent(27,1);
   h_res_s1->SetBinError(1,9.219544);
   h_res_s1->SetBinError(2,9.380832);
   h_res_s1->SetBinError(3,10.29563);
   h_res_s1->SetBinError(4,11.61895);
   h_res_s1->SetBinError(5,14.21267);
   h_res_s1->SetBinError(6,15.29706);
   h_res_s1->SetBinError(7,17.54993);
   h_res_s1->SetBinError(8,22.91288);
   h_res_s1->SetBinError(9,23.70654);
   h_res_s1->SetBinError(10,27.87472);
   h_res_s1->SetBinError(11,30.8707);
   h_res_s1->SetBinError(12,31.32092);
   h_res_s1->SetBinError(13,32.90897);
   h_res_s1->SetBinError(14,33.43651);
   h_res_s1->SetBinError(15,30.16621);
   h_res_s1->SetBinError(16,27.38613);
   h_res_s1->SetBinError(17,23.51595);
   h_res_s1->SetBinError(18,19.44222);
   h_res_s1->SetBinError(19,14.28286);
   h_res_s1->SetBinError(20,10.48809);
   h_res_s1->SetBinError(21,8.544004);
   h_res_s1->SetBinError(22,6.557439);
   h_res_s1->SetBinError(23,5.656854);
   h_res_s1->SetBinError(24,3.872983);
   h_res_s1->SetBinError(25,2.236068);
   h_res_s1->SetBinError(26,3);
   h_res_s1->SetBinError(27,1);
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
   TLine *line = new TLine(0.1579109,0,0.1579109,1118);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.2430286,0,0.2430286,1118);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3145291,0,0.3145291,1118);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3,0,0.3,1118);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.24^{-0.09}_{+0.07} ");
   pt->Draw();
   Canvas_Xn8rJYNbpG6kL4ExD25DuZ->Modified();
   Canvas_Xn8rJYNbpG6kL4ExD25DuZ->cd();
   Canvas_Xn8rJYNbpG6kL4ExD25DuZ->SetSelected(Canvas_Xn8rJYNbpG6kL4ExD25DuZ);
}
