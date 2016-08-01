{
//=========Macro generated from canvas: e8f3e18ccc17496e8ad57c1b8732f21f/
//=========  (Thu Apr 21 16:08:26 2016) by ROOT version5.34/26
   TCanvas *e8f3e18ccc17496e8ad57c1b8732f21f = new TCanvas("e8f3e18ccc17496e8ad57c1b8732f21f", "",10,45,704,527);
   e8f3e18ccc17496e8ad57c1b8732f21f->SetHighLightColor(2);
   e8f3e18ccc17496e8ad57c1b8732f21f->Range(-0.0525,-2438.864,0.4725,21949.77);
   e8f3e18ccc17496e8ad57c1b8732f21f->SetFillColor(0);
   e8f3e18ccc17496e8ad57c1b8732f21f->SetBorderMode(0);
   e8f3e18ccc17496e8ad57c1b8732f21f->SetBorderSize(2);
   e8f3e18ccc17496e8ad57c1b8732f21f->SetFrameBorderMode(0);
   e8f3e18ccc17496e8ad57c1b8732f21f->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,18446);
   h_res_s2->SetBinContent(2,16628);
   h_res_s2->SetBinContent(3,15227);
   h_res_s2->SetBinContent(4,12640);
   h_res_s2->SetBinContent(5,12989);
   h_res_s2->SetBinContent(6,10171);
   h_res_s2->SetBinContent(7,8035);
   h_res_s2->SetBinContent(8,4810);
   h_res_s2->SetBinContent(9,2592);
   h_res_s2->SetBinContent(10,654);
   h_res_s2->SetBinContent(11,186);
   h_res_s2->SetBinContent(12,22);
   h_res_s2->SetBinError(1,135.8161);
   h_res_s2->SetBinError(2,128.9496);
   h_res_s2->SetBinError(3,123.3977);
   h_res_s2->SetBinError(4,112.4278);
   h_res_s2->SetBinError(5,113.9693);
   h_res_s2->SetBinError(6,100.8514);
   h_res_s2->SetBinError(7,89.63816);
   h_res_s2->SetBinError(8,69.35416);
   h_res_s2->SetBinError(9,50.91169);
   h_res_s2->SetBinError(10,25.57342);
   h_res_s2->SetBinError(11,13.63818);
   h_res_s2->SetBinError(12,4.690416);
   h_res_s2->SetEntries(102400);
   h_res_s2->SetStats(0);
   h_res_s2->SetFillStyle(0);
   h_res_s2->SetMarkerStyle(20);
   h_res_s2->SetMarkerSize(0);
   h_res_s2->GetXaxis()->SetRange(1,21);
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
   TLine *line = new TLine(0.01765326,0,0.01765326,18446);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.06142247,0,0.06142247,18446);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1200433,0,0.1200433,18446);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,18446);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.06^{-0.04}_{+0.06} ");
   pt->Draw();
   e8f3e18ccc17496e8ad57c1b8732f21f->Modified();
   e8f3e18ccc17496e8ad57c1b8732f21f->cd();
   e8f3e18ccc17496e8ad57c1b8732f21f->SetSelected(e8f3e18ccc17496e8ad57c1b8732f21f);
}
