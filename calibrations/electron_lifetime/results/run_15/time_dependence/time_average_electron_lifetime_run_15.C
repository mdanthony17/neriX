{
//=========Macro generated from canvas: 303edd1105db416f904a6ef4b2179f8b/
//=========  (Mon Apr 11 15:01:56 2016) by ROOT version5.34/26
   TCanvas *303edd1105db416f904a6ef4b2179f8b = new TCanvas("303edd1105db416f904a6ef4b2179f8b", "",10,45,704,527);
   303edd1105db416f904a6ef4b2179f8b->SetHighLightColor(2);
   303edd1105db416f904a6ef4b2179f8b->Range(1.444861e+09,6.572673,1.463471e+09,206.8387);
   303edd1105db416f904a6ef4b2179f8b->SetFillColor(0);
   303edd1105db416f904a6ef4b2179f8b->SetBorderMode(0);
   303edd1105db416f904a6ef4b2179f8b->SetBorderSize(2);
   303edd1105db416f904a6ef4b2179f8b->SetFrameBorderMode(0);
   303edd1105db416f904a6ef4b2179f8b->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(20);
   gre->SetName("Graph0");
   gre->SetTitle("Electron Lifetime in Time");
   gre->SetFillColor(1);
   gre->SetPoint(0,1.447962e+09,60.9014);
   gre->SetPointError(0,0,5.953839);
   gre->SetPoint(1,1.448991e+09,43.2484);
   gre->SetPointError(1,0,3.298061);
   gre->SetPoint(2,1.44916e+09,52.53605);
   gre->SetPointError(2,0,4.902276);
   gre->SetPoint(3,1.449519e+09,52.04944);
   gre->SetPointError(3,0,5.229021);
   gre->SetPoint(4,1.45011e+09,101.7771);
   gre->SetPointError(4,0,29.99492);
   gre->SetPoint(5,1.450804e+09,108.1975);
   gre->SetPointError(5,0,30.03004);
   gre->SetPoint(6,1.45133e+09,108.4038);
   gre->SetPointError(6,0,21.87976);
   gre->SetPoint(7,1.451933e+09,54.8367);
   gre->SetPointError(7,0,8.68671);
   gre->SetPoint(8,1.452531e+09,45.9626);
   gre->SetPointError(8,0,5.168757);
   gre->SetPoint(9,1.453134e+09,74.38369);
   gre->SetPointError(9,0,14.68871);
   gre->SetPoint(10,1.453743e+09,75.93297);
   gre->SetPointError(10,0,14.44905);
   gre->SetPoint(11,1.453925e+09,58.73454);
   gre->SetPointError(11,0,7.879056);
   gre->SetPoint(12,1.4546e+09,84.47184);
   gre->SetPointError(12,0,21.3037);
   gre->SetPoint(13,1.454942e+09,69.49844);
   gre->SetPointError(13,0,13.02376);
   gre->SetPoint(14,1.455646e+09,128.1437);
   gre->SetPointError(14,0,45.31733);
   gre->SetPoint(15,1.45616e+09,45.01283);
   gre->SetPointError(15,0,4.722666);
   gre->SetPoint(16,1.456767e+09,48.36355);
   gre->SetPointError(16,0,5.102563);
   gre->SetPoint(17,1.459335e+09,50.84733);
   gre->SetPointError(17,0,9.147526);
   gre->SetPoint(18,1.459786e+09,53.44842);
   gre->SetPointError(18,0,9.505495);
   gre->SetPoint(19,1.46037e+09,62.86088);
   gre->SetPointError(19,0,12.37848);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","Electron Lifetime in Time",100,1.446722e+09,1.46161e+09);
   Graph_Graph1->SetMinimum(26.59927);
   Graph_Graph1->SetMaximum(186.8121);
   Graph_Graph1->SetDirectory(0);
   Graph_Graph1->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph1->SetLineColor(ci);
   Graph_Graph1->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph1->GetXaxis()->SetTimeFormat("");
   Graph_Graph1->GetXaxis()->SetLabelFont(42);
   Graph_Graph1->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleFont(42);
   Graph_Graph1->GetYaxis()->SetTitle("Electron Lifetime [us]");
   Graph_Graph1->GetYaxis()->SetLabelFont(42);
   Graph_Graph1->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleFont(42);
   Graph_Graph1->GetZaxis()->SetLabelFont(42);
   Graph_Graph1->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1);
   
   
   TF1 *fG1 = new TF1("fG1","pol0",1.446722e+09,1.46161e+09);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(39.6536);
   fG1->SetNDF(19);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,51.19186);
   fG1->SetParError(0,1.580687);
   fG1->SetParLimits(0,0,0);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(20);
   grae->SetName("Graph1");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#0000ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3005);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.447962e+09,51.19186);
   grae->SetPointError(0,0,0,4.779524,4.779524);
   grae->SetPoint(1,1.448991e+09,51.19186);
   grae->SetPointError(1,0,0,4.779524,4.779524);
   grae->SetPoint(2,1.44916e+09,51.19186);
   grae->SetPointError(2,0,0,4.779524,4.779524);
   grae->SetPoint(3,1.449519e+09,51.19186);
   grae->SetPointError(3,0,0,4.779524,4.779524);
   grae->SetPoint(4,1.45011e+09,51.19186);
   grae->SetPointError(4,0,0,4.779524,4.779524);
   grae->SetPoint(5,1.450804e+09,51.19186);
   grae->SetPointError(5,0,0,4.779524,4.779524);
   grae->SetPoint(6,1.45133e+09,51.19186);
   grae->SetPointError(6,0,0,4.779524,4.779524);
   grae->SetPoint(7,1.451933e+09,51.19186);
   grae->SetPointError(7,0,0,4.779524,4.779524);
   grae->SetPoint(8,1.452531e+09,51.19186);
   grae->SetPointError(8,0,0,4.779524,4.779524);
   grae->SetPoint(9,1.453134e+09,51.19186);
   grae->SetPointError(9,0,0,4.779524,4.779524);
   grae->SetPoint(10,1.453743e+09,51.19186);
   grae->SetPointError(10,0,0,4.779524,4.779524);
   grae->SetPoint(11,1.453925e+09,51.19186);
   grae->SetPointError(11,0,0,4.779524,4.779524);
   grae->SetPoint(12,1.4546e+09,51.19186);
   grae->SetPointError(12,0,0,4.779524,4.779524);
   grae->SetPoint(13,1.454942e+09,51.19186);
   grae->SetPointError(13,0,0,4.779524,4.779524);
   grae->SetPoint(14,1.455646e+09,51.19186);
   grae->SetPointError(14,0,0,4.779524,4.779524);
   grae->SetPoint(15,1.45616e+09,51.19186);
   grae->SetPointError(15,0,0,4.779524,4.779524);
   grae->SetPoint(16,1.456767e+09,51.19186);
   grae->SetPointError(16,0,0,4.779524,4.779524);
   grae->SetPoint(17,1.459335e+09,51.19186);
   grae->SetPointError(17,0,0,4.779524,4.779524);
   grae->SetPoint(18,1.459786e+09,51.19186);
   grae->SetPointError(18,0,0,4.779524,4.779524);
   grae->SetPoint(19,1.46037e+09,51.19186);
   grae->SetPointError(19,0,0,4.779524,4.779524);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","Graph",100,1.446722e+09,1.46161e+09);
   Graph_Graph1->SetMinimum(45.45643);
   Graph_Graph1->SetMaximum(56.92729);
   Graph_Graph1->SetDirectory(0);
   Graph_Graph1->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph1->SetLineColor(ci);
   Graph_Graph1->GetXaxis()->SetLabelFont(42);
   Graph_Graph1->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleFont(42);
   Graph_Graph1->GetYaxis()->SetLabelFont(42);
   Graph_Graph1->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleFont(42);
   Graph_Graph1->GetZaxis()->SetLabelFont(42);
   Graph_Graph1->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph1);
   
   grae->Draw("3 ");
   
   TPaveText *pt = new TPaveText(0.1,0.65,0.9,0.9,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("Run Average e^{-} lifetime = (51.19 +/- 1.58) us");
   pt->Draw();
   
   pt = new TPaveText(0.2785714,0.94,0.7214286,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("Electron Lifetime in Time");
   pt->Draw();
   303edd1105db416f904a6ef4b2179f8b->Modified();
   303edd1105db416f904a6ef4b2179f8b->cd();
   303edd1105db416f904a6ef4b2179f8b->SetSelected(303edd1105db416f904a6ef4b2179f8b);
}
