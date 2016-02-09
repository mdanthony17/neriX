{
//=========Macro generated from canvas: c1/c1
//=========  (Fri Jan 29 15:20:43 2016) by ROOT version5.34/26
   TCanvas *c1 = new TCanvas("c1", "c1",200,45,1000,700);
   c1->SetHighLightColor(2);
   c1->Range(-5.7,-0.1322251,44.7,1.190026);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetGridx();
   c1->SetGridy();
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(20);
   grae->SetName("eff_graph");
   grae->SetTitle("S1 Efficiency for TAC - nerix_160125_1023");
   grae->SetFillColor(19);
   grae->SetPoint(0,0.5,0.0001069061);
   grae->SetPointError(0,1,1,3.699386e-05,5.271577e-05);
   grae->SetPoint(1,2.5,0.1501458);
   grae->SetPointError(1,1,1,0.01382589,0.01489945);
   grae->SetPoint(2,4.5,0.3131673);
   grae->SetPointError(2,1,1,0.02008882,0.02078318);
   grae->SetPoint(3,6.5,0.4246032);
   grae->SetPointError(3,1,1,0.02282952,0.02314181);
   grae->SetPoint(4,8.5,0.4671053);
   grae->SetPointError(4,1,1,0.02435702,0.02450785);
   grae->SetPoint(5,10.5,0.5859729);
   grae->SetPointError(5,1,1,0.02473465,0.02432742);
   grae->SetPoint(6,12.5,0.5906863);
   grae->SetPointError(6,1,1,0.02576953,0.02530333);
   grae->SetPoint(7,14.5,0.6204986);
   grae->SetPointError(7,1,1,0.02723866,0.0265361);
   grae->SetPoint(8,16.5,0.7);
   grae->SetPointError(8,1,1,0.02828197,0.02691445);
   grae->SetPoint(9,18.5,0.7929936);
   grae->SetPointError(9,1,1,0.02542398,0.02343175);
   grae->SetPoint(10,20.5,0.7844828);
   grae->SetPointError(10,1,1,0.03042172,0.02778065);
   grae->SetPoint(11,22.5,0.8256228);
   grae->SetPointError(11,1,1,0.02563441,0.02314013);
   grae->SetPoint(12,24.5,0.8302752);
   grae->SetPointError(12,1,1,0.02932576,0.02603185);
   grae->SetPoint(13,26.5,0.8617021);
   grae->SetPointError(13,1,1,0.02992335,0.02568325);
   grae->SetPoint(14,28.5,0.8502674);
   grae->SetPointError(14,1,1,0.03078385,0.02666771);
   grae->SetPoint(15,30.5,0.8932584);
   grae->SetPointError(15,1,1,0.02840923,0.02347198);
   grae->SetPoint(16,32.5,0.916129);
   grae->SetPointError(16,1,1,0.02856217,0.02244066);
   grae->SetPoint(17,34.5,0.8853503);
   grae->SetPointError(17,1,1,0.03133719,0.02583597);
   grae->SetPoint(18,36.5,0.9396552);
   grae->SetPointError(18,1,1,0.0309908,0.02198827);
   grae->SetPoint(19,38.5,0.8703704);
   grae->SetPointError(19,1,1,0.04077071,0.03297738);
   
   TH1F *Graph_eff_graph1 = new TH1F("Graph_eff_graph1","S1 Efficiency for TAC - nerix_160125_1023",100,-4.5,43.5);
   Graph_eff_graph1->SetMinimum(0);
   Graph_eff_graph1->SetMaximum(1.057801);
   Graph_eff_graph1->SetDirectory(0);
   Graph_eff_graph1->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_eff_graph1->SetLineColor(ci);
   Graph_eff_graph1->GetXaxis()->SetTitle(" cpS1sTotBottom[S1Order[0]] [PE]");
   Graph_eff_graph1->GetXaxis()->SetRange(9,92);
   Graph_eff_graph1->GetXaxis()->SetLabelFont(42);
   Graph_eff_graph1->GetXaxis()->SetLabelSize(0.035);
   Graph_eff_graph1->GetXaxis()->SetTitleSize(0.035);
   Graph_eff_graph1->GetXaxis()->SetTitleFont(42);
   Graph_eff_graph1->GetYaxis()->SetTitle(" Percentage Causing Discriminator Pulse for TAC");
   Graph_eff_graph1->GetYaxis()->SetLabelFont(42);
   Graph_eff_graph1->GetYaxis()->SetLabelSize(0.035);
   Graph_eff_graph1->GetYaxis()->SetTitleSize(0.035);
   Graph_eff_graph1->GetYaxis()->SetTitleFont(42);
   Graph_eff_graph1->GetZaxis()->SetLabelFont(42);
   Graph_eff_graph1->GetZaxis()->SetLabelSize(0.035);
   Graph_eff_graph1->GetZaxis()->SetTitleSize(0.035);
   Graph_eff_graph1->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_eff_graph1);
   
   grae->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.15,0.936159,0.85,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("S1 Efficiency for TAC - nerix_160125_1023");
   pt->Draw();
   
   TF1 *fEfficiency = new TF1("fEfficiency","1.-exp(-[0]*x)",-0.5,39.5);
   fEfficiency->SetFillColor(19);
   fEfficiency->SetFillStyle(0);
   fEfficiency->SetLineColor(2);
   fEfficiency->SetLineWidth(2);
   fEfficiency->SetChisquare(13.92706);
   fEfficiency->SetNDF(19);
   fEfficiency->GetXaxis()->SetLabelFont(42);
   fEfficiency->GetXaxis()->SetLabelSize(0.035);
   fEfficiency->GetXaxis()->SetTitleSize(0.035);
   fEfficiency->GetXaxis()->SetTitleFont(42);
   fEfficiency->GetYaxis()->SetLabelFont(42);
   fEfficiency->GetYaxis()->SetLabelSize(0.035);
   fEfficiency->GetYaxis()->SetTitleSize(0.035);
   fEfficiency->GetYaxis()->SetTitleFont(42);
   fEfficiency->SetParameter(0,0.07364992);
   fEfficiency->SetParError(0,0.002025262);
   fEfficiency->SetParLimits(0,0.01,50);
   fEfficiency->Draw("same");
   
   grae = new TGraphAsymmErrors(20);
   grae->SetName("Graph0");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#0000ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3005);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,0.5,0.03615517);
   grae->SetPointError(0,0,0,0.001748983,0.001748983);
   grae->SetPoint(1,2.5,0.168168);
   grae->SetPointError(1,0,0,0.007547169,0.007547169);
   grae->SetPoint(2,4.5,0.2820998);
   grae->SetPointError(2,0,0,0.01172425,0.01172425);
   grae->SetPoint(3,6.5,0.3804268);
   grae->SetPointError(3,0,0,0.01461552,0.01461552);
   grae->SetPoint(4,8.5,0.4652866);
   grae->SetPointError(4,0,0,0.01649485,0.01649485);
   grae->SetPoint(5,10.5,0.5385235);
   grae->SetPointError(5,0,0,0.0175852,0.0175852);
   grae->SetPoint(6,12.5,0.6017296);
   grae->SetPointError(6,0,0,0.01806744,0.01806744);
   grae->SetPoint(7,14.5,0.6562786);
   grae->SetPointError(7,0,0,0.01808769,0.01808769);
   grae->SetPoint(8,16.5,0.7033563);
   grae->SetPointError(8,0,0,0.01776346,0.01776346);
   grae->SetPoint(9,18.5,0.7439861);
   grae->SetPointError(9,0,0,0.01718873,0.01718873);
   grae->SetPoint(10,20.5,0.779051);
   grae->SetPointError(10,0,0,0.0164382,0.0164382);
   grae->SetPoint(11,22.5,0.8093132);
   grae->SetPointError(11,0,0,0.01557082,0.01557082);
   grae->SetPoint(12,24.5,0.8354306);
   grae->SetPointError(12,0,0,0.01463267,0.01463267);
   grae->SetPoint(13,26.5,0.8579708);
   grae->SetPointError(13,0,0,0.01365941,0.01365941);
   grae->SetPoint(14,28.5,0.8774238);
   grae->SetPointError(14,0,0,0.01267825,0.01267825);
   grae->SetPoint(15,30.5,0.8942125);
   grae->SetPointError(15,0,0,0.01170962,0.01170962);
   grae->SetPoint(16,32.5,0.9087017);
   grae->SetPointError(16,0,0,0.01076849,0.01076849);
   grae->SetPoint(17,34.5,0.9212063);
   grae->SetPointError(17,0,0,0.009865499,0.009865499);
   grae->SetPoint(18,36.5,0.9319983);
   grae->SetPointError(18,0,0,0.009007853,0.009007853);
   grae->SetPoint(19,38.5,0.9413121);
   grae->SetPointError(19,0,0,0.008200072,0.008200072);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Graph",100,0,42.3);
   Graph_Graph2->SetMinimum(0);
   Graph_Graph2->SetMaximum(1.041023);
   Graph_Graph2->SetDirectory(0);
   Graph_Graph2->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph2->SetLineColor(ci);
   Graph_Graph2->GetXaxis()->SetLabelFont(42);
   Graph_Graph2->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph2->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph2->GetXaxis()->SetTitleFont(42);
   Graph_Graph2->GetYaxis()->SetLabelFont(42);
   Graph_Graph2->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph2->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph2->GetYaxis()->SetTitleFont(42);
   Graph_Graph2->GetZaxis()->SetLabelFont(42);
   Graph_Graph2->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph2->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph2->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph2);
   
   grae->Draw("3 ");
   
   pt = new TPaveText(0.5,0.35,0.8,0.45,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("#epsilon_{trig} = 1 - e^{-#alphax}");
   pt->Draw();
   
   pt = new TPaveText(0.5,0.25,0.8,0.35,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("#alpha = 0.0736 #pm 0.0020");
   pt->Draw();
   TLine *line = new TLine(9.411377,0,9.411377,1.057801);

   ci = TColor::GetColor("#ff9933");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
