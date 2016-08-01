{
//=========Macro generated from canvas: Canvas_5kLCozgYrbbPbaQMT9C6wn/
//=========  (Mon Feb 29 14:02:12 2016) by ROOT version5.34/28
   TCanvas *Canvas_5kLCozgYrbbPbaQMT9C6wn = new TCanvas("Canvas_5kLCozgYrbbPbaQMT9C6wn", "",0,0,700,500);
   Canvas_5kLCozgYrbbPbaQMT9C6wn->SetHighLightColor(2);
   Canvas_5kLCozgYrbbPbaQMT9C6wn->Range(-0.25,-155.788,2.25,1402.092);
   Canvas_5kLCozgYrbbPbaQMT9C6wn->SetFillColor(0);
   Canvas_5kLCozgYrbbPbaQMT9C6wn->SetBorderMode(0);
   Canvas_5kLCozgYrbbPbaQMT9C6wn->SetBorderSize(2);
   Canvas_5kLCozgYrbbPbaQMT9C6wn->SetFrameBorderMode(0);
   Canvas_5kLCozgYrbbPbaQMT9C6wn->SetFrameBorderMode(0);
   
   TH1F *h_res_s2 = new TH1F("h_res_s2","res_s2",100,0,2);
   h_res_s2->SetBinContent(1,657);
   h_res_s2->SetBinContent(2,979);
   h_res_s2->SetBinContent(3,995);
   h_res_s2->SetBinContent(4,1028);
   h_res_s2->SetBinContent(5,1153);
   h_res_s2->SetBinContent(6,1112);
   h_res_s2->SetBinContent(7,1025);
   h_res_s2->SetBinContent(8,937);
   h_res_s2->SetBinContent(9,849);
   h_res_s2->SetBinContent(10,589);
   h_res_s2->SetBinContent(11,427);
   h_res_s2->SetBinContent(12,213);
   h_res_s2->SetBinContent(13,179);
   h_res_s2->SetBinContent(14,67);
   h_res_s2->SetBinContent(15,20);
   h_res_s2->SetBinContent(16,6);
   h_res_s2->SetBinContent(17,4);
   h_res_s2->SetBinError(1,25.63201);
   h_res_s2->SetBinError(2,31.28898);
   h_res_s2->SetBinError(3,31.54362);
   h_res_s2->SetBinError(4,32.06244);
   h_res_s2->SetBinError(5,33.95585);
   h_res_s2->SetBinError(6,33.34666);
   h_res_s2->SetBinError(7,32.01562);
   h_res_s2->SetBinError(8,30.61046);
   h_res_s2->SetBinError(9,29.1376);
   h_res_s2->SetBinError(10,24.26932);
   h_res_s2->SetBinError(11,20.66398);
   h_res_s2->SetBinError(12,14.59452);
   h_res_s2->SetBinError(13,13.37909);
   h_res_s2->SetBinError(14,8.185353);
   h_res_s2->SetBinError(15,4.472136);
   h_res_s2->SetBinError(16,2.44949);
   h_res_s2->SetBinError(17,2);
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
   TLine *line = new TLine(0.03983984,0,0.03983984,1153);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1055396,0,0.1055396,1153);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1770987,0,0.1770987,1153);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.05,0,0.05,1153);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S2} = 0.11^{-0.07}_{+0.07} ");
   pt->Draw();
   Canvas_5kLCozgYrbbPbaQMT9C6wn->Modified();
   Canvas_5kLCozgYrbbPbaQMT9C6wn->cd();
   Canvas_5kLCozgYrbbPbaQMT9C6wn->SetSelected(Canvas_5kLCozgYrbbPbaQMT9C6wn);
}
