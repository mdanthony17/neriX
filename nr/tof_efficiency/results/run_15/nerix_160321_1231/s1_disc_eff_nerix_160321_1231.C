{
//=========Macro generated from canvas: c1/c1
//=========  (Mon Mar 21 14:46:31 2016) by ROOT version5.34/26
   TCanvas *c1 = new TCanvas("c1", "c1",592,49,1000,700);
   c1->SetHighLightColor(2);
   c1->Range(-5.7,-0.1155889,44.7,1.218306);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetGridx();
   c1->SetGridy();
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(20);
   grae->SetName("eff_graph");
   grae->SetTitle("S1 Efficiency for TAC - nerix_160321_1231");
   grae->SetFillColor(19);
   grae->SetPoint(0,0.5,0.1200658);
   grae->SetPointError(0,1,1,0.01333877,0.01466525);
   grae->SetPoint(1,2.5,0.3645606);
   grae->SetPointError(1,1,1,0.01322776,0.0134299);
   grae->SetPoint(2,4.5,0.3084112);
   grae->SetPointError(2,1,1,0.01868141,0.01930316);
   grae->SetPoint(3,6.5,0.4460641);
   grae->SetPointError(3,1,1,0.02808905,0.02842002);
   grae->SetPoint(4,8.5,0.6335079);
   grae->SetPointError(4,1,1,0.03813135,0.0366334);
   grae->SetPoint(5,10.5,0.7483871);
   grae->SetPointError(5,1,1,0.03971473,0.03622753);
   grae->SetPoint(6,12.5,0.8282828);
   grae->SetPointError(6,1,1,0.04653629,0.03907038);
   grae->SetPoint(7,14.5,0.8817204);
   grae->SetPointError(7,1,1,0.04351277,0.0340545);
   grae->SetPoint(8,16.5,0.9459459);
   grae->SetPointError(8,1,1,0.04067846,0.02569146);
   grae->SetPoint(9,18.5,0.9733333);
   grae->SetPointError(9,1,1,0.03409125,0.0172056);
   grae->SetPoint(10,20.5,0.902439);
   grae->SetPointError(10,1,1,0.07044154,0.04610076);
   grae->SetPoint(11,22.5,0.9767442);
   grae->SetPointError(11,1,1,0.05145338,0.01924634);
   grae->SetPoint(12,24.5,0.9090909);
   grae->SetPointError(12,1,1,0.08059045,0.04906319);
   grae->SetPoint(13,26.5,0.9411765);
   grae->SetPointError(13,1,1,0.0723694,0.0379027);
   grae->SetPoint(14,28.5,0.9210526);
   grae->SetPointError(14,1,1,0.0708737,0.04265626);
   grae->SetPoint(15,30.5,0.9047619);
   grae->SetPointError(15,1,1,0.1120626,0.06127008);
   grae->SetPoint(16,32.5,0.9615385);
   grae->SetPointError(16,1,1,0.08295595,0.03183919);
   grae->SetPoint(17,34.5,0.96);
   grae->SetPointError(17,1,1,0.08605119,0.03311367);
   grae->SetPoint(18,36.5,0.9090909);
   grae->SetPointError(18,1,1,0.107544,0.05849615);
   grae->SetPoint(19,38.5,0.8947368);
   grae->SetPointError(19,1,1,0.1223217,0.06768965);
   
   TH1F *Graph_eff_graph1 = new TH1F("Graph_eff_graph1","S1 Efficiency for TAC - nerix_160321_1231",100,-4.5,43.5);
   Graph_eff_graph1->SetMinimum(0.01780067);
   Graph_eff_graph1->SetMaximum(1.084917);
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
   TText *text = pt->AddText("S1 Efficiency for TAC - nerix_160321_1231");
   pt->Draw();
   
   TF1 *fEfficiency = new TF1("fEfficiency","1.-exp(-[0]*x)",-0.5,39.5);
   fEfficiency->SetFillColor(19);
   fEfficiency->SetFillStyle(0);
   fEfficiency->SetLineColor(2);
   fEfficiency->SetLineWidth(2);
   fEfficiency->SetChisquare(25.17291);
   fEfficiency->SetNDF(19);
   fEfficiency->GetXaxis()->SetLabelFont(42);
   fEfficiency->GetXaxis()->SetLabelSize(0.035);
   fEfficiency->GetXaxis()->SetTitleSize(0.035);
   fEfficiency->GetXaxis()->SetTitleFont(42);
   fEfficiency->GetYaxis()->SetLabelFont(42);
   fEfficiency->GetYaxis()->SetLabelSize(0.035);
   fEfficiency->GetYaxis()->SetTitleSize(0.035);
   fEfficiency->GetYaxis()->SetTitleFont(42);
   fEfficiency->SetParameter(0,0.1272889);
   fEfficiency->SetParError(0,0.007014981);
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
   grae->SetPoint(0,0.5,0.06166145);
   grae->SetPointError(0,0,0,0.007929042,0.007929042);
   grae->SetPoint(1,2.5,0.272559);
   grae->SetPointError(1,0,0,0.0307347,0.0307347);
   grae->SetPoint(2,4.5,0.4360559);
   grae->SetPointError(2,0,0,0.04288839,0.04288839);
   grae->SetPoint(3,6.5,0.562806);
   grae->SetPointError(3,0,0,0.04802626,0.04802626);
   grae->SetPoint(4,8.5,0.6610681);
   grae->SetPointError(4,0,0,0.04868807,0.04868807);
   grae->SetPoint(5,10.5,0.7372451);
   grae->SetPointError(5,0,0,0.04662633,0.04662633);
   grae->SetPoint(6,12.5,0.7963009);
   grae->SetPointError(6,0,0,0.04303186,0.04303186);
   grae->SetPoint(7,14.5,0.8420836);
   grae->SetPointError(7,0,0,0.03869781,0.03869781);
   grae->SetPoint(8,16.5,0.8775763);
   grae->SetPointError(8,0,0,0.0341382,0.0341382);
   grae->SetPoint(9,18.5,0.9050918);
   grae->SetPointError(9,0,0,0.02967335,0.02967335);
   grae->SetPoint(10,20.5,0.926423);
   grae->SetPointError(10,0,0,0.025491,0.025491);
   grae->SetPoint(11,22.5,0.9429599);
   grae->SetPointError(11,0,0,0.02168971,0.02168971);
   grae->SetPoint(12,24.5,0.95578);
   grae->SetPointError(12,0,0,0.01830946,0.01830946);
   grae->SetPoint(13,26.5,0.9657187);
   grae->SetPointError(13,0,0,0.01535301,0.01535301);
   grae->SetPoint(14,28.5,0.9734237);
   grae->SetPointError(14,0,0,0.01280062,0.01280062);
   grae->SetPoint(15,30.5,0.9793969);
   grae->SetPointError(15,0,0,0.01061999,0.01061999);
   grae->SetPoint(16,32.5,0.9840276);
   grae->SetPointError(16,0,0,0.008772953,0.008772953);
   grae->SetPoint(17,34.5,0.9876175);
   grae->SetPointError(17,0,0,0.007219711,0.007219711);
   grae->SetPoint(18,36.5,0.9904005);
   grae->SetPointError(18,0,0,0.005921501,0.005921501);
   grae->SetPoint(19,38.5,0.9925581);
   grae->SetPointError(19,0,0,0.004842146,0.004842146);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Graph",100,0,42.3);
   Graph_Graph2->SetMinimum(0);
   Graph_Graph2->SetMaximum(1.091767);
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
   text = pt->AddText("#alpha = 0.1273 #pm 0.0070");
   pt->Draw();
   TLine *line = new TLine(5.445463,0.01780067,5.445463,1.084917);

   ci = TColor::GetColor("#ff9933");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
