{
//=========Macro generated from canvas: f46a8ed78d6c4eb481ce382e78a91041/
//=========  (Wed Mar 30 09:56:27 2016) by ROOT version5.34/26
   TCanvas *f46a8ed78d6c4eb481ce382e78a91041 = new TCanvas("f46a8ed78d6c4eb481ce382e78a91041", "",10,45,704,527);
   f46a8ed78d6c4eb481ce382e78a91041->SetHighLightColor(2);
   f46a8ed78d6c4eb481ce382e78a91041->Range(265549,913.5169,580317.1,2529.22);
   f46a8ed78d6c4eb481ce382e78a91041->SetFillColor(0);
   f46a8ed78d6c4eb481ce382e78a91041->SetBorderMode(0);
   f46a8ed78d6c4eb481ce382e78a91041->SetBorderSize(2);
   f46a8ed78d6c4eb481ce382e78a91041->SetFrameBorderMode(0);
   f46a8ed78d6c4eb481ce382e78a91041->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,376382.2,1878.758);
   gre->SetPointError(0,1432.23,8.532482);
   gre->SetPoint(1,412867.2,1681.016);
   gre->SetPointError(1,1467.414,8.683345);
   gre->SetPoint(2,429425.3,1578.796);
   gre->SetPointError(2,1316.292,7.013639);
   gre->SetPoint(3,444268.9,1485.881);
   gre->SetPointError(3,1161.245,6.308498);
   gre->SetPoint(4,465026.9,1380.648);
   gre->SetPointError(4,958.9969,5.043553);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,297025.8,548840.3);
   Graph_Graph1->SetMinimum(1075.087);
   Graph_Graph1->SetMaximum(2367.65);
   Graph_Graph1->SetDirectory(0);
   Graph_Graph1->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph1->SetLineColor(ci);
   Graph_Graph1->GetXaxis()->SetTitle("S2 at 662 keV Peak");
   Graph_Graph1->GetXaxis()->SetLabelFont(42);
   Graph_Graph1->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleOffset(1.12);
   Graph_Graph1->GetXaxis()->SetTitleFont(42);
   Graph_Graph1->GetYaxis()->SetTitle("S1 at 662 keV Peak [PE]");
   Graph_Graph1->GetYaxis()->SetLabelFont(42);
   Graph_Graph1->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleFont(42);
   Graph_Graph1->GetZaxis()->SetLabelFont(42);
   Graph_Graph1->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1);
   
   
   TF1 *fG1 = new TF1("fG1","pol1",365846.4,475089.5);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(1.652881);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,4014.316);
   fG1->SetParError(0,66.78451);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.005670957);
   fG1->SetParError(1,0.0001530179);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0831 +/- 0.0039");
   text = pt->AddText("g2 = 14.6493 +/- 0.7920");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(423217.5,1623.993,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(498945.7,1194.541,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(330028.7,2152.409,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",297025.8,548840.3);
   fDraw->SetFillColor(19);
   fDraw->SetFillStyle(0);
   fDraw->SetLineColor(2);
   fDraw->SetLineWidth(1);
   fDraw->SetLineStyle(9);
   fDraw->GetXaxis()->SetLabelFont(42);
   fDraw->GetXaxis()->SetLabelSize(0.035);
   fDraw->GetXaxis()->SetTitleSize(0.035);
   fDraw->GetXaxis()->SetTitleFont(42);
   fDraw->GetYaxis()->SetLabelFont(42);
   fDraw->GetYaxis()->SetLabelSize(0.035);
   fDraw->GetYaxis()->SetTitleSize(0.035);
   fDraw->GetYaxis()->SetTitleFont(42);
   fDraw->SetParameter(0,4014.316);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.005670957);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   f46a8ed78d6c4eb481ce382e78a91041->Modified();
   f46a8ed78d6c4eb481ce382e78a91041->cd();
   f46a8ed78d6c4eb481ce382e78a91041->SetSelected(f46a8ed78d6c4eb481ce382e78a91041);
}
