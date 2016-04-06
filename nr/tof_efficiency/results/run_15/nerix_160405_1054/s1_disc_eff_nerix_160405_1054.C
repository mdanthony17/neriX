{
//=========Macro generated from canvas: c1/c1
//=========  (Tue Apr  5 15:49:49 2016) by ROOT version5.34/26
   TCanvas *c1 = new TCanvas("c1", "c1",200,45,1000,700);
   c1->SetHighLightColor(2);
   c1->Range(-5.7,-0.02193894,44.7,1.204388);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetGridx();
   c1->SetGridy();
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(20);
   grae->SetName("eff_graph");
   grae->SetTitle("S1 Efficiency for TAC - nerix_160405_1054");
   grae->SetFillColor(19);
   grae->SetPoint(0,0.5,0.197724);
   grae->SetPointError(0,1,1,0.01527518,0.01617509);
   grae->SetPoint(1,2.5,0.3963682);
   grae->SetPointError(1,1,1,0.01248269,0.01261574);
   grae->SetPoint(2,4.5,0.3781676);
   grae->SetPointError(2,1,1,0.02211492,0.02261092);
   grae->SetPoint(3,6.5,0.5932836);
   grae->SetPointError(3,1,1,0.03217892,0.03144123);
   grae->SetPoint(4,8.5,0.6690141);
   grae->SetPointError(4,1,1,0.04414827,0.04156724);
   grae->SetPoint(5,10.5,0.87);
   grae->SetPointError(5,1,1,0.04276269,0.0343188);
   grae->SetPoint(6,12.5,0.8987342);
   grae->SetPointError(6,1,1,0.04614386,0.03427203);
   grae->SetPoint(7,14.5,0.92);
   grae->SetPointError(7,1,1,0.04472676,0.03127412);
   grae->SetPoint(8,16.5,0.9104478);
   grae->SetPointError(8,1,1,0.04965836,0.03494485);
   grae->SetPoint(9,18.5,0.7755102);
   grae->SetPointError(9,1,1,0.07564768,0.06265937);
   grae->SetPoint(10,20.5,0.8125);
   grae->SetPointError(10,1,1,0.07377968,0.05857158);
   grae->SetPoint(11,22.5,0.925);
   grae->SetPointError(11,1,1,0.06760868,0.04053863);
   grae->SetPoint(12,24.5,0.8787879);
   grae->SetPointError(12,1,1,0.08551339,0.05708965);
   grae->SetPoint(13,26.5,0.8928571);
   grae->SetPointError(13,1,1,0.09335264,0.05773358);
   grae->SetPoint(14,28.5,0.9375);
   grae->SetPointError(14,1,1,0.07655147,0.04026534);
   grae->SetPoint(15,30.5,0.9285714);
   grae->SetPointError(15,1,1,0.08654378,0.046);
   grae->SetPoint(16,32.5,0.952381);
   grae->SetPointError(16,1,1,0.1011341,0.03942642);
   grae->SetPoint(17,34.5,0.9230769);
   grae->SetPointError(17,1,1,0.1554154,0.06372223);
   grae->SetPoint(18,36.5,0.9444444);
   grae->SetPointError(18,1,1,0.1164146,0.04600403);
   grae->SetPoint(19,38.5,1);
   grae->SetPointError(19,1,1,0.1541097,0);
   
   TH1F *Graph_eff_graph1 = new TH1F("Graph_eff_graph1","S1 Efficiency for TAC - nerix_160405_1054",100,-4.5,43.5);
   Graph_eff_graph1->SetMinimum(0.1006937);
   Graph_eff_graph1->SetMaximum(1.081755);
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
   TText *text = pt->AddText("S1 Efficiency for TAC - nerix_160405_1054");
   pt->Draw();
   
   TF1 *fEfficiency = new TF1("fEfficiency","1.-exp(-[0]*x)",-0.5,39.5);
   fEfficiency->SetFillColor(19);
   fEfficiency->SetFillStyle(0);
   fEfficiency->SetLineColor(2);
   fEfficiency->SetLineWidth(2);
   fEfficiency->SetChisquare(31.4312);
   fEfficiency->SetNDF(19);
   fEfficiency->GetXaxis()->SetLabelFont(42);
   fEfficiency->GetXaxis()->SetLabelSize(0.035);
   fEfficiency->GetXaxis()->SetTitleSize(0.035);
   fEfficiency->GetXaxis()->SetTitleFont(42);
   fEfficiency->GetYaxis()->SetLabelFont(42);
   fEfficiency->GetYaxis()->SetLabelSize(0.035);
   fEfficiency->GetYaxis()->SetTitleSize(0.035);
   fEfficiency->GetYaxis()->SetTitleFont(42);
   fEfficiency->SetParameter(0,0.136568);
   fEfficiency->SetParError(0,0.009214321);
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
   grae->SetPoint(0,0.5,0.06600484);
   grae->SetPointError(0,0,0,0.01158395,0.01158395);
   grae->SetPoint(1,2.5,0.2892397);
   grae->SetPointError(1,0,0,0.04407632,0.04407632);
   grae->SetPoint(2,4.5,0.4591191);
   grae->SetPointError(2,0,0,0.06037489,0.06037489);
   grae->SetPoint(3,6.5,0.5883955);
   grae->SetPointError(3,0,0,0.06636448,0.06636448);
   grae->SetPoint(4,8.5,0.6867734);
   grae->SetPointError(4,0,0,0.06604193,0.06604193);
   grae->SetPoint(5,10.5,0.7616379);
   grae->SetPointError(5,0,0,0.06208242,0.06208242);
   grae->SetPoint(6,12.5,0.8186091);
   grae->SetPointError(6,0,0,0.05624292,0.05624292);
   grae->SetPoint(7,14.5,0.8619635);
   grae->SetPointError(7,0,0,0.0496483,0.0496483);
   grae->SetPoint(8,16.5,0.8949556);
   grae->SetPointError(8,0,0,0.04299311,0.04299311);
   grae->SetPoint(9,18.5,0.9200624);
   grae->SetPointError(9,0,0,0.03668302,0.03668302);
   grae->SetPoint(10,20.5,0.9391683);
   grae->SetPointError(10,0,0,0.03093326,0.03093326);
   grae->SetPoint(11,22.5,0.9537078);
   grae->SetPointError(11,0,0,0.02583645,0.02583645);
   grae->SetPoint(12,24.5,0.9647721);
   grae->SetPointError(12,0,0,0.02140893,0.02140893);
   grae->SetPoint(13,26.5,0.973192);
   grae->SetPointError(13,0,0,0.01762192,0.01762192);
   grae->SetPoint(14,28.5,0.9795994);
   grae->SetPointError(14,0,0,0.01442218,0.01442218);
   grae->SetPoint(15,30.5,0.9844753);
   grae->SetPointError(15,0,0,0.0117453,0.0117453);
   grae->SetPoint(16,32.5,0.9881859);
   grae->SetPointError(16,0,0,0.009524152,0.009524152);
   grae->SetPoint(17,34.5,0.9910096);
   grae->SetPointError(17,0,0,0.007693794,0.007693794);
   grae->SetPoint(18,36.5,0.9931584);
   grae->SetPointError(18,0,0,0.006194308,0.006194308);
   grae->SetPoint(19,38.5,0.9947936);
   grae->SetPointError(19,0,0,0.004972092,0.004972092);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Graph",100,0,42.3);
   Graph_Graph2->SetMinimum(0);
   Graph_Graph2->SetMaximum(1.0943);
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
   text = pt->AddText("#alpha = 0.1366 #pm 0.0092");
   pt->Draw();
   TLine *line = new TLine(5.075471,0.1006937,5.075471,1.081755);

   ci = TColor::GetColor("#ff9933");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
