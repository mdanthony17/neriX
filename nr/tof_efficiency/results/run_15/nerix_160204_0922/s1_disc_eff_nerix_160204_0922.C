{
//=========Macro generated from canvas: c1/c1
//=========  (Wed Feb 10 09:37:20 2016) by ROOT version5.34/26
   TCanvas *c1 = new TCanvas("c1", "c1",200,45,1000,700);
   c1->SetHighLightColor(2);
   c1->Range(-5.7,-0.132358,44.7,1.191222);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetGridx();
   c1->SetGridy();
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(20);
   grae->SetName("eff_graph");
   grae->SetTitle("S1 Efficiency for TAC - nerix_160204_0922");
   grae->SetFillColor(19);
   grae->SetPoint(0,0.5,0.02409639);
   grae->SetPointError(0,1,1,0.01149722,0.01864232);
   grae->SetPoint(1,2.5,0.2335766);
   grae->SetPointError(1,1,1,0.01648006,0.01729234);
   grae->SetPoint(2,4.5,0.3889764);
   grae->SetPointError(2,1,1,0.01993622,0.02029978);
   grae->SetPoint(3,6.5,0.527027);
   grae->SetPointError(3,1,1,0.02139317,0.0212982);
   grae->SetPoint(4,8.5,0.6536885);
   grae->SetPointError(4,1,1,0.0228696,0.02221061);
   grae->SetPoint(5,10.5,0.7261905);
   grae->SetPointError(5,1,1,0.02349159,0.02235724);
   grae->SetPoint(6,12.5,0.7590361);
   grae->SetPointError(6,1,1,0.02577473,0.02411725);
   grae->SetPoint(7,14.5,0.7790262);
   grae->SetPointError(7,1,1,0.02834328,0.0261043);
   grae->SetPoint(8,16.5,0.8090909);
   grae->SetPointError(8,1,1,0.03024479,0.02720255);
   grae->SetPoint(9,18.5,0.8235294);
   grae->SetPointError(9,1,1,0.03420292,0.03002944);
   grae->SetPoint(10,20.5,0.8639053);
   grae->SetPointError(10,1,1,0.03168171,0.02691007);
   grae->SetPoint(11,22.5,0.8507463);
   grae->SetPointError(11,1,1,0.03736361,0.03152226);
   grae->SetPoint(12,24.5,0.875);
   grae->SetPointError(12,1,1,0.04335218,0.03439678);
   grae->SetPoint(13,26.5,0.8584906);
   grae->SetPointError(13,1,1,0.04230309,0.03464441);
   grae->SetPoint(14,28.5,0.875);
   grae->SetPointError(14,1,1,0.04574343,0.03592267);
   grae->SetPoint(15,30.5,0.8666667);
   grae->SetPointError(15,1,1,0.05142489,0.04008986);
   grae->SetPoint(16,32.5,0.9041096);
   grae->SetPointError(16,1,1,0.04782581,0.03468189);
   grae->SetPoint(17,34.5,0.8732394);
   grae->SetPointError(17,1,1,0.05250067,0.04023113);
   grae->SetPoint(18,36.5,0.8909091);
   grae->SetPointError(18,1,1,0.05947333,0.04240908);
   grae->SetPoint(19,38.5,0.9310345);
   grae->SetPointError(19,1,1,0.05117748,0.03271432);
   
   TH1F *Graph_eff_graph1 = new TH1F("Graph_eff_graph1","S1 Efficiency for TAC - nerix_160204_0922",100,-4.5,43.5);
   Graph_eff_graph1->SetMinimum(0);
   Graph_eff_graph1->SetMaximum(1.058864);
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
   TText *text = pt->AddText("S1 Efficiency for TAC - nerix_160204_0922");
   pt->Draw();
   
   TF1 *fEfficiency = new TF1("fEfficiency","1.-exp(-[0]*x)",-0.5,39.5);
   fEfficiency->SetFillColor(19);
   fEfficiency->SetFillStyle(0);
   fEfficiency->SetLineColor(2);
   fEfficiency->SetLineWidth(2);
   fEfficiency->SetChisquare(37.42753);
   fEfficiency->SetNDF(19);
   fEfficiency->GetXaxis()->SetLabelFont(42);
   fEfficiency->GetXaxis()->SetLabelSize(0.035);
   fEfficiency->GetXaxis()->SetTitleSize(0.035);
   fEfficiency->GetXaxis()->SetTitleFont(42);
   fEfficiency->GetYaxis()->SetLabelFont(42);
   fEfficiency->GetYaxis()->SetLabelSize(0.035);
   fEfficiency->GetYaxis()->SetTitleSize(0.035);
   fEfficiency->GetYaxis()->SetTitleFont(42);
   fEfficiency->SetParameter(0,0.09709712);
   fEfficiency->SetParError(0,0.003673184);
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
   grae->SetPoint(0,0.5,0.04738892);
   grae->SetPointError(0,0,0,0.005139507,0.005139507);
   grae->SetPoint(1,2.5,0.2155267);
   grae->SetPointError(1,0,0,0.02116187,0.02116187);
   grae->SetPoint(2,4.5,0.3539879);
   grae->SetPointError(2,0,0,0.03136816,0.03136816);
   grae->SetPoint(3,6.5,0.4680104);
   grae->SetPointError(3,0,0,0.03731233,0.03731233);
   grae->SetPoint(4,8.5,0.5619076);
   grae->SetPointError(4,0,0,0.04018098,0.04018098);
   grae->SetPoint(5,10.5,0.6392319);
   grae->SetPointError(5,0,0,0.04087459,0.04087459);
   grae->SetPoint(6,12.5,0.7029082);
   grae->SetPointError(6,0,0,0.04007159,0.04007159);
   grae->SetPoint(7,14.5,0.7553455);
   grae->SetPointError(7,0,0,0.03827869,0.03827869);
   grae->SetPoint(8,16.5,0.7985275);
   grae->SetPointError(8,0,0,0.03587034,0.03587034);
   grae->SetPoint(9,18.5,0.8340878);
   grae->SetPointError(9,0,0,0.03311966,0.03311966);
   grae->SetPoint(10,20.5,0.8633717);
   grae->SetPointError(10,0,0,0.03022251,0.03022251);
   grae->SetPoint(11,22.5,0.8874869);
   grae->SetPointError(11,0,0,0.02731629,0.02731629);
   grae->SetPoint(12,24.5,0.9073457);
   grae->SetPointError(12,0,0,0.02449445,0.02449445);
   grae->SetPoint(13,26.5,0.9236993);
   grae->SetPointError(13,0,0,0.02181775,0.02181775);
   grae->SetPoint(14,28.5,0.9371666);
   grae->SetPointError(14,0,0,0.01932287,0.01932287);
   grae->SetPoint(15,30.5,0.9482568);
   grae->SetPointError(15,0,0,0.01702899,0.01702899);
   grae->SetPoint(16,32.5,0.9573896);
   grae->SetPointError(16,0,0,0.01494291,0.01494291);
   grae->SetPoint(17,34.5,0.9649104);
   grae->SetPointError(17,0,0,0.01306271,0.01306271);
   grae->SetPoint(18,36.5,0.9711038);
   grae->SetPointError(18,0,0,0.01138072,0.01138072);
   grae->SetPoint(19,38.5,0.976204);
   grae->SetPointError(19,0,0,0.00988553,0.00988553);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Graph",100,0,42.3);
   Graph_Graph2->SetMinimum(0);
   Graph_Graph2->SetMaximum(1.080474);
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
   text = pt->AddText("#alpha = 0.0971 #pm 0.0037");
   pt->Draw();
   TLine *line = new TLine(7.1387,0,7.1387,1.058864);

   ci = TColor::GetColor("#ff9933");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
