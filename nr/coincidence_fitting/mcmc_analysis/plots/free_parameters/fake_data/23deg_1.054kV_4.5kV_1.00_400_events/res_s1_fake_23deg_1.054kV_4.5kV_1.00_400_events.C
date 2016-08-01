{
//=========Macro generated from canvas: Canvas_VqE9m7DnGrbLHZs7kX4DGo/
//=========  (Mon Feb 29 14:03:31 2016) by ROOT version5.34/28
   TCanvas *Canvas_VqE9m7DnGrbLHZs7kX4DGo = new TCanvas("Canvas_VqE9m7DnGrbLHZs7kX4DGo", "",0,0,700,500);
   Canvas_VqE9m7DnGrbLHZs7kX4DGo->SetHighLightColor(2);
   Canvas_VqE9m7DnGrbLHZs7kX4DGo->Range(-0.25,-216.9776,2.25,1952.798);
   Canvas_VqE9m7DnGrbLHZs7kX4DGo->SetFillColor(0);
   Canvas_VqE9m7DnGrbLHZs7kX4DGo->SetBorderMode(0);
   Canvas_VqE9m7DnGrbLHZs7kX4DGo->SetBorderSize(2);
   Canvas_VqE9m7DnGrbLHZs7kX4DGo->SetFrameBorderMode(0);
   Canvas_VqE9m7DnGrbLHZs7kX4DGo->SetFrameBorderMode(0);
   
   TH1F *h_res_s1 = new TH1F("h_res_s1","res_s1",100,0,2);
   h_res_s1->SetBinContent(1,1613);
   h_res_s1->SetBinContent(2,1436);
   h_res_s1->SetBinContent(3,1399);
   h_res_s1->SetBinContent(4,1242);
   h_res_s1->SetBinContent(5,900);
   h_res_s1->SetBinContent(6,837);
   h_res_s1->SetBinContent(7,707);
   h_res_s1->SetBinContent(8,532);
   h_res_s1->SetBinContent(9,445);
   h_res_s1->SetBinContent(10,341);
   h_res_s1->SetBinContent(11,247);
   h_res_s1->SetBinContent(12,162);
   h_res_s1->SetBinContent(13,113);
   h_res_s1->SetBinContent(14,115);
   h_res_s1->SetBinContent(15,43);
   h_res_s1->SetBinContent(16,21);
   h_res_s1->SetBinContent(17,30);
   h_res_s1->SetBinContent(18,10);
   h_res_s1->SetBinContent(19,17);
   h_res_s1->SetBinContent(20,12);
   h_res_s1->SetBinContent(21,5);
   h_res_s1->SetBinContent(22,8);
   h_res_s1->SetBinContent(24,5);
   h_res_s1->SetBinError(1,40.16217);
   h_res_s1->SetBinError(2,37.89459);
   h_res_s1->SetBinError(3,37.40321);
   h_res_s1->SetBinError(4,35.24202);
   h_res_s1->SetBinError(5,30);
   h_res_s1->SetBinError(6,28.93095);
   h_res_s1->SetBinError(7,26.58947);
   h_res_s1->SetBinError(8,23.06513);
   h_res_s1->SetBinError(9,21.09502);
   h_res_s1->SetBinError(10,18.46619);
   h_res_s1->SetBinError(11,15.71623);
   h_res_s1->SetBinError(12,12.72792);
   h_res_s1->SetBinError(13,10.63015);
   h_res_s1->SetBinError(14,10.72381);
   h_res_s1->SetBinError(15,6.557439);
   h_res_s1->SetBinError(16,4.582576);
   h_res_s1->SetBinError(17,5.477226);
   h_res_s1->SetBinError(18,3.162278);
   h_res_s1->SetBinError(19,4.123106);
   h_res_s1->SetBinError(20,3.464102);
   h_res_s1->SetBinError(21,2.236068);
   h_res_s1->SetBinError(22,2.828427);
   h_res_s1->SetBinError(24,2.236068);
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
   TLine *line = new TLine(0.02021114,0,0.02021114,1613);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.07082126,0,0.07082126,1613);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.1579639,0,0.1579639,1613);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(0.3,0,0.3,1613);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("R_{S1} = 0.07^{-0.05}_{+0.09} ");
   pt->Draw();
   Canvas_VqE9m7DnGrbLHZs7kX4DGo->Modified();
   Canvas_VqE9m7DnGrbLHZs7kX4DGo->cd();
   Canvas_VqE9m7DnGrbLHZs7kX4DGo->SetSelected(Canvas_VqE9m7DnGrbLHZs7kX4DGo);
}
