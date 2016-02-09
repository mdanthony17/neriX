{
//=========Macro generated from canvas: c1/c1
//=========  (Wed Feb  3 15:48:41 2016) by ROOT version5.34/26
   TCanvas *c1 = new TCanvas("c1", "c1",200,45,1000,700);
   c1->SetHighLightColor(2);
   c1->Range(-5.7,-0.125895,44.7,1.133055);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetGridx();
   c1->SetGridy();
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(20);
   grae->SetName("eff_graph");
   grae->SetTitle("S1 Efficiency for TAC - nerix_160111_0935");
   grae->SetFillColor(19);
   grae->SetPoint(0,0.5,0.0248227);
   grae->SetPointError(0,1,1,0.009109796,0.0131143);
   grae->SetPoint(1,2.5,0.1725417);
   grae->SetPointError(1,1,1,0.01655205,0.01783493);
   grae->SetPoint(2,4.5,0.3199195);
   grae->SetPointError(2,1,1,0.02152866,0.02228723);
   grae->SetPoint(3,6.5,0.4530516);
   grae->SetPointError(3,1,1,0.02514543,0.02537624);
   grae->SetPoint(4,8.5,0.5615764);
   grae->SetPointError(4,1,1,0.02598314,0.02566513);
   grae->SetPoint(5,10.5,0.5705329);
   grae->SetPointError(5,1,1,0.0294674,0.0290011);
   grae->SetPoint(6,12.5,0.6261981);
   grae->SetPointError(6,1,1,0.02932311,0.02847134);
   grae->SetPoint(7,14.5,0.6287625);
   grae->SetPointError(7,1,1,0.03001797,0.02910705);
   grae->SetPoint(8,16.5,0.688);
   grae->SetPointError(8,1,1,0.03204323,0.03044062);
   grae->SetPoint(9,18.5,0.7777778);
   grae->SetPointError(9,1,1,0.03054118,0.02798721);
   grae->SetPoint(10,20.5,0.752809);
   grae->SetPointError(10,1,1,0.03659389,0.03351758);
   grae->SetPoint(11,22.5,0.830303);
   grae->SetPointError(11,1,1,0.03439084,0.02998901);
   grae->SetPoint(12,24.5,0.7753623);
   grae->SetPointError(12,1,1,0.04121946,0.03684214);
   grae->SetPoint(13,26.5,0.8695652);
   grae->SetPointError(13,1,1,0.03932075,0.03204537);
   grae->SetPoint(14,28.5,0.8727273);
   grae->SetPointError(14,1,1,0.04010111,0.03240119);
   grae->SetPoint(15,30.5,0.8712871);
   grae->SetPointError(15,1,1,0.0423812,0.03399206);
   grae->SetPoint(16,32.5,0.7972973);
   grae->SetPointError(16,1,1,0.05774735,0.04863307);
   grae->SetPoint(17,34.5,0.8783784);
   grae->SetPointError(17,1,1,0.0505838,0.03865016);
   grae->SetPoint(18,36.5,0.862069);
   grae->SetPointError(18,1,1,0.06096425,0.0462793);
   grae->SetPoint(19,38.5,0.86);
   grae->SetPointError(19,1,1,0.0672437,0.05015192);
   
   TH1F *Graph_eff_graph1 = new TH1F("Graph_eff_graph1","S1 Efficiency for TAC - nerix_160111_0935",100,-4.5,43.5);
   Graph_eff_graph1->SetMinimum(0);
   Graph_eff_graph1->SetMaximum(1.00716);
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
   
   TPaveText *pt = new TPaveText(0.1501406,0.936159,0.8498594,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("S1 Efficiency for TAC - nerix_160111_0935");
   pt->Draw();
   
   TF1 *fEfficiency = new TF1("fEfficiency","1.-exp(-[0]*x)",-0.5,39.5);
   fEfficiency->SetFillColor(19);
   fEfficiency->SetFillStyle(0);
   fEfficiency->SetLineColor(2);
   fEfficiency->SetLineWidth(2);
   fEfficiency->SetChisquare(23.89173);
   fEfficiency->SetNDF(19);
   fEfficiency->GetXaxis()->SetLabelFont(42);
   fEfficiency->GetXaxis()->SetLabelSize(0.035);
   fEfficiency->GetXaxis()->SetTitleSize(0.035);
   fEfficiency->GetXaxis()->SetTitleFont(42);
   fEfficiency->GetYaxis()->SetLabelFont(42);
   fEfficiency->GetYaxis()->SetLabelSize(0.035);
   fEfficiency->GetYaxis()->SetTitleSize(0.035);
   fEfficiency->GetYaxis()->SetTitleFont(42);
   fEfficiency->SetParameter(0,0.07304329);
   fEfficiency->SetParError(0,0.002368047);
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
   grae->SetPoint(0,0.5,0.03586277);
   grae->SetPointError(0,0,0,0.002679295,0.002679295);
   grae->SetPoint(1,2.5,0.1669055);
   grae->SetPointError(1,0,0,0.01157567,0.01157567);
   grae->SetPoint(2,4.5,0.2801373);
   grae->SetPointError(2,0,0,0.0180042,0.0180042);
   grae->SetPoint(3,6.5,0.377979);
   grae->SetPointError(3,0,0,0.0224714,0.0224714);
   grae->SetPoint(4,8.5,0.4625223);
   grae->SetPointError(4,0,0,0.02539166,0.02539166);
   grae->SetPoint(5,10.5,0.5355747);
   grae->SetPointError(5,0,0,0.02710297,0.02710297);
   grae->SetPoint(6,12.5,0.598698);
   grae->SetPointError(6,0,0,0.02788002,0.02788002);
   grae->SetPoint(7,14.5,0.6532419);
   grae->SetPointError(7,0,0,0.02794515,0.02794515);
   grae->SetPoint(8,16.5,0.7003722);
   grae->SetPointError(8,0,0,0.02747753,0.02747753);
   grae->SetPoint(9,18.5,0.7410967);
   grae->SetPointError(9,0,0,0.02662079,0.02662079);
   grae->SetPoint(10,20.5,0.7762861);
   grae->SetPointError(10,0,0,0.02548933,0.02548933);
   grae->SetPoint(11,22.5,0.8066927);
   grae->SetPointError(11,0,0,0.02417367,0.02417367);
   grae->SetPoint(12,24.5,0.8329664);
   grae->SetPointError(12,0,0,0.02274477,0.02274477);
   grae->SetPoint(13,26.5,0.8556691);
   grae->SetPointError(13,0,0,0.02125772,0.02125772);
   grae->SetPoint(14,28.5,0.8752862);
   grae->SetPointError(14,0,0,0.01975473,0.01975473);
   grae->SetPoint(15,30.5,0.8922369);
   grae->SetPointError(15,0,0,0.0182676,0.0182676);
   grae->SetPoint(16,32.5,0.9068838);
   grae->SetPointError(16,0,0,0.01681978,0.01681978);
   grae->SetPoint(17,34.5,0.9195399);
   grae->SetPointError(17,0,0,0.01542807,0.01542807);
   grae->SetPoint(18,36.5,0.9304758);
   grae->SetPointError(18,0,0,0.01410395,0.01410395);
   grae->SetPoint(19,38.5,0.9399253);
   grae->SetPointError(19,0,0,0.01285476,0.01285476);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Graph",100,0,42.3);
   Graph_Graph2->SetMinimum(0);
   Graph_Graph2->SetMaximum(1.04474);
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
   text = pt->AddText("#alpha = 0.0730 #pm 0.0024");
   pt->Draw();
   TLine *line = new TLine(9.489539,0,9.489539,1.00716);

   ci = TColor::GetColor("#ff9933");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
