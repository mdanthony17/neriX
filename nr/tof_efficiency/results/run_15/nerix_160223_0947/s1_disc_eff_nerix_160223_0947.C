{
//=========Macro generated from canvas: c1/c1
//=========  (Thu Mar  3 14:57:38 2016) by ROOT version5.34/26
   TCanvas *c1 = new TCanvas("c1", "c1",200,45,1000,700);
   c1->SetHighLightColor(2);
   c1->Range(-5.7,-0.1286882,44.7,1.158193);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetGridx();
   c1->SetGridy();
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(20);
   grae->SetName("eff_graph");
   grae->SetTitle("S1 Efficiency for TAC - nerix_160223_0947");
   grae->SetFillColor(19);
   grae->SetPoint(0,0.5,0.09433962);
   grae->SetPointError(0,1,1,0.02865718,0.037526);
   grae->SetPoint(1,2.5,0.2894118);
   grae->SetPointError(1,1,1,0.02262771,0.02367019);
   grae->SetPoint(2,4.5,0.4310345);
   grae->SetPointError(2,1,1,0.02773305,0.02815011);
   grae->SetPoint(3,6.5,0.6006826);
   grae->SetPointError(3,1,1,0.03062844,0.02990176);
   grae->SetPoint(4,8.5,0.6492537);
   grae->SetPointError(4,1,1,0.03154876,0.03036642);
   grae->SetPoint(5,10.5,0.6477733);
   grae->SetPointError(5,1,1,0.03298787,0.03171489);
   grae->SetPoint(6,12.5,0.7051282);
   grae->SetPointError(6,1,1,0.03281788,0.03094403);
   grae->SetPoint(7,14.5,0.7251185);
   grae->SetPointError(7,1,1,0.03417852,0.03188706);
   grae->SetPoint(8,16.5,0.7239583);
   grae->SetPointError(8,1,1,0.0360388,0.03352592);
   grae->SetPoint(9,18.5,0.7241379);
   grae->SetPointError(9,1,1,0.04211598,0.03875213);
   grae->SetPoint(10,20.5,0.8455285);
   grae->SetPointError(10,1,1,0.03970624,0.0334203);
   grae->SetPoint(11,22.5,0.8);
   grae->SetPointError(11,1,1,0.04440256,0.03860982);
   grae->SetPoint(12,24.5,0.8214286);
   grae->SetPointError(12,1,1,0.05186786,0.04319354);
   grae->SetPoint(13,26.5,0.75);
   grae->SetPointError(13,1,1,0.05473591,0.04840217);
   grae->SetPoint(14,28.5,0.9047619);
   grae->SetPointError(14,1,1,0.05255233,0.03712313);
   grae->SetPoint(15,30.5,0.8157895);
   grae->SetPointError(15,1,1,0.05551623,0.04605996);
   grae->SetPoint(16,32.5,0.7818182);
   grae->SetPointError(16,1,1,0.07014047,0.05836569);
   grae->SetPoint(17,34.5,0.8809524);
   grae->SetPointError(17,1,1,0.07251484,0.05044131);
   grae->SetPoint(18,36.5,0.8888889);
   grae->SetPointError(18,1,1,0.06817902,0.04714237);
   grae->SetPoint(19,38.5,0.8571429);
   grae->SetPointError(19,1,1,0.08509498,0.06028007);
   
   TH1F *Graph_eff_graph1 = new TH1F("Graph_eff_graph1","S1 Efficiency for TAC - nerix_160223_0947",100,-4.5,43.5);
   Graph_eff_graph1->SetMinimum(0);
   Graph_eff_graph1->SetMaximum(1.029505);
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
   TText *text = pt->AddText("S1 Efficiency for TAC - nerix_160223_0947");
   pt->Draw();
   
   TF1 *fEfficiency = new TF1("fEfficiency","[1]*(1.-exp(-[0]*x))",-0.5,39.5);
   fEfficiency->SetFillColor(19);
   fEfficiency->SetFillStyle(0);
   fEfficiency->SetLineColor(2);
   fEfficiency->SetLineWidth(2);
   fEfficiency->SetChisquare(14.93348);
   fEfficiency->SetNDF(18);
   fEfficiency->GetXaxis()->SetLabelFont(42);
   fEfficiency->GetXaxis()->SetLabelSize(0.035);
   fEfficiency->GetXaxis()->SetTitleSize(0.035);
   fEfficiency->GetXaxis()->SetTitleFont(42);
   fEfficiency->GetYaxis()->SetLabelFont(42);
   fEfficiency->GetYaxis()->SetLabelSize(0.035);
   fEfficiency->GetYaxis()->SetTitleSize(0.035);
   fEfficiency->GetYaxis()->SetTitleFont(42);
   fEfficiency->SetParameter(0,0.1578326);
   fEfficiency->SetParError(0,0.01711022);
   fEfficiency->SetParLimits(0,0.01,50);
   fEfficiency->SetParameter(1,0.8252628);
   fEfficiency->SetParError(1,0.0202801);
   fEfficiency->SetParLimits(1,0,0);
   fEfficiency->Draw("same");
   
   grae = new TGraphAsymmErrors(20);
   grae->SetName("Graph0");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#0000ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3005);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,0.5,0.06262319);
   grae->SetPointError(0,0,0,0.01024746,0.01024746);
   grae->SetPoint(1,2.5,0.269067);
   grae->SetPointError(1,0,0,0.03605492,0.03605492);
   grae->SetPoint(2,4.5,0.4196272);
   grae->SetPointError(2,0,0,0.04529275,0.04529275);
   grae->SetPoint(3,6.5,0.5294313);
   grae->SetPointError(3,0,0,0.0452247,0.0452247);
   grae->SetPoint(4,8.5,0.6095119);
   grae->SetPointError(4,0,0,0.04049374,0.04049374);
   grae->SetPoint(5,10.5,0.6679149);
   grae->SetPointError(5,0,0,0.03407558,0.03407558);
   grae->SetPoint(6,12.5,0.7105084);
   grae->SetPointError(6,0,0,0.02794269,0.02794269);
   grae->SetPoint(7,14.5,0.741572);
   grae->SetPointError(7,0,0,0.02346947,0.02346947);
   grae->SetPoint(8,16.5,0.7642268);
   grae->SetPointError(8,0,0,0.02143962,0.02143962);
   grae->SetPoint(9,18.5,0.780749);
   grae->SetPointError(9,0,0,0.02171964,0.02171964);
   grae->SetPoint(10,20.5,0.7927988);
   grae->SetPointError(10,0,0,0.02343207,0.02343207);
   grae->SetPoint(11,22.5,0.8015867);
   grae->SetPointError(11,0,0,0.02569666,0.02569666);
   grae->SetPoint(12,24.5,0.8079957);
   grae->SetPointError(12,0,0,0.02798593,0.02798593);
   grae->SetPoint(13,26.5,0.8126699);
   grae->SetPointError(13,0,0,0.03005974,0.03005974);
   grae->SetPoint(14,28.5,0.8160787);
   grae->SetPointError(14,0,0,0.03183615,0.03183615);
   grae->SetPoint(15,30.5,0.8185648);
   grae->SetPointError(15,0,0,0.03330886,0.03330886);
   grae->SetPoint(16,32.5,0.8203779);
   grae->SetPointError(16,0,0,0.03450435,0.03450435);
   grae->SetPoint(17,34.5,0.8217003);
   grae->SetPointError(17,0,0,0.03546069,0.03546069);
   grae->SetPoint(18,36.5,0.8226647);
   grae->SetPointError(18,0,0,0.03621755,0.03621755);
   grae->SetPoint(19,38.5,0.823368);
   grae->SetPointError(19,0,0,0.03681162,0.03681162);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Graph",100,0,42.3);
   Graph_Graph2->SetMinimum(0);
   Graph_Graph2->SetMaximum(0.94096);
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
   text = pt->AddText("#alpha = 0.1578 #pm 0.0171");
   pt->Draw();
   TLine *line = new TLine(4.391661,0,4.391661,1.029505);

   ci = TColor::GetColor("#ff9933");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
