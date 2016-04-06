{
//=========Macro generated from canvas: c1/c1
//=========  (Tue Mar 29 11:07:13 2016) by ROOT version5.34/26
   TCanvas *c1 = new TCanvas("c1", "c1",200,45,1000,700);
   c1->SetHighLightColor(2);
   c1->Range(-5.700001,-0.1064226,44.7,1.221285);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetGridx();
   c1->SetGridy();
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(20);
   grae->SetName("eff_graph");
   grae->SetTitle("S1 Efficiency for TAC - nerix_160329_0947");
   grae->SetFillColor(19);
   grae->SetPoint(0,0.5,0.1292639);
   grae->SetPointError(0,1,1,0.01440195,0.0158157);
   grae->SetPoint(1,2.5,0.3695073);
   grae->SetPointError(1,1,1,0.01269378,0.01287211);
   grae->SetPoint(2,4.5,0.3472222);
   grae->SetPointError(2,1,1,0.01921417,0.01970469);
   grae->SetPoint(3,6.5,0.5608466);
   grae->SetPointError(3,1,1,0.0269809,0.02664284);
   grae->SetPoint(4,8.5,0.7456897);
   grae->SetPointError(4,1,1,0.03182296,0.02955146);
   grae->SetPoint(5,10.5,0.8723404);
   grae->SetPointError(5,1,1,0.03456468,0.02864402);
   grae->SetPoint(6,12.5,0.8939394);
   grae->SetPointError(6,1,1,0.03395357,0.02717489);
   grae->SetPoint(7,14.5,0.9480519);
   grae->SetPointError(7,1,1,0.03917053,0.0246973);
   grae->SetPoint(8,16.5,0.9431818);
   grae->SetPointError(8,1,1,0.03660873,0.0243244);
   grae->SetPoint(9,18.5,0.9393939);
   grae->SetPointError(9,1,1,0.04533002,0.02878078);
   grae->SetPoint(10,20.5,0.9577465);
   grae->SetPointError(10,1,1,0.03939751,0.02290851);
   grae->SetPoint(11,22.5,0.8793103);
   grae->SetPointError(11,1,1,0.05894438,0.04341883);
   grae->SetPoint(12,24.5,0.96);
   grae->SetPointError(12,1,1,0.05033473,0.02579419);
   grae->SetPoint(13,26.5,0.9148936);
   grae->SetPointError(13,1,1,0.06219152,0.04028366);
   grae->SetPoint(14,28.5,1);
   grae->SetPointError(14,1,1,0.05270781,0);
   grae->SetPoint(15,30.5,0.9411765);
   grae->SetPointError(15,1,1,0.0723694,0.0379027);
   grae->SetPoint(16,32.5,0.96875);
   grae->SetPointError(16,1,1,0.06822251,0.02586599);
   grae->SetPoint(17,34.5,0.8695652);
   grae->SetPointError(17,1,1,0.1108137,0.07012284);
   grae->SetPoint(18,36.5,0.9310345);
   grae->SetPointError(18,1,1,0.08381046,0.04441848);
   grae->SetPoint(19,38.5,0.952381);
   grae->SetPointError(19,1,1,0.1011341,0.03942642);
   
   TH1F *Graph_eff_graph1 = new TH1F("Graph_eff_graph1","S1 Efficiency for TAC - nerix_160329_0947",100,-4.5,43.5);
   Graph_eff_graph1->SetMinimum(0.02634816);
   Graph_eff_graph1->SetMaximum(1.088514);
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
   TText *text = pt->AddText("S1 Efficiency for TAC - nerix_160329_0947");
   pt->Draw();
   
   TF1 *fEfficiency = new TF1("fEfficiency","1.-exp(-[0]*x)",-0.5,39.5);
   fEfficiency->SetFillColor(19);
   fEfficiency->SetFillStyle(0);
   fEfficiency->SetLineColor(2);
   fEfficiency->SetLineWidth(2);
   fEfficiency->SetChisquare(27.60801);
   fEfficiency->SetNDF(19);
   fEfficiency->GetXaxis()->SetLabelFont(42);
   fEfficiency->GetXaxis()->SetLabelSize(0.035);
   fEfficiency->GetXaxis()->SetTitleSize(0.035);
   fEfficiency->GetXaxis()->SetTitleFont(42);
   fEfficiency->GetYaxis()->SetLabelFont(42);
   fEfficiency->GetYaxis()->SetLabelSize(0.035);
   fEfficiency->GetYaxis()->SetTitleSize(0.035);
   fEfficiency->GetYaxis()->SetTitleFont(42);
   fEfficiency->SetParameter(0,0.1545141);
   fEfficiency->SetParError(0,0.008915759);
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
   grae->SetPoint(0,0.5,0.07434814);
   grae->SetPointError(0,0,0,0.01041098,0.01041098);
   grae->SetPoint(1,2.5,0.3204234);
   grae->SetPointError(1,0,0,0.03821664,0.03821664);
   grae->SetPoint(2,4.5,0.5010821);
   grae->SetPointError(2,0,0,0.05050283,0.05050283);
   grae->SetPoint(3,6.5,0.6337144);
   grae->SetPointError(3,0,0,0.0535559,0.0535559);
   grae->SetPoint(4,8.5,0.7310877);
   grae->SetPointError(4,0,0,0.05141663,0.05141663);
   grae->SetPoint(5,10.5,0.8025754);
   grae->SetPointError(5,0,0,0.04662992,0.04662992);
   grae->SetPoint(6,12.5,0.8550587);
   grae->SetPointError(6,0,0,0.04075455,0.04075455);
   grae->SetPoint(7,14.5,0.8935899);
   grae->SetPointError(7,0,0,0.03470762,0.03470762);
   grae->SetPoint(8,16.5,0.921878);
   grae->SetPointError(8,0,0,0.02899556,0.02899556);
   grae->SetPoint(9,18.5,0.942646);
   grae->SetPointError(9,0,0,0.02386767,0.02386767);
   grae->SetPoint(10,20.5,0.957893);
   grae->SetPointError(10,0,0,0.01941703,0.01941703);
   grae->SetPoint(11,22.5,0.9690867);
   grae->SetPointError(11,0,0,0.01564596,0.01564596);
   grae->SetPoint(12,24.5,0.9773047);
   grae->SetPointError(12,0,0,0.01250767,0.01250767);
   grae->SetPoint(13,26.5,0.983338);
   grae->SetPointError(13,0,0,0.009932236,0.009932236);
   grae->SetPoint(14,28.5,0.9877675);
   grae->SetPointError(14,0,0,0.007842178,0.007842178);
   grae->SetPoint(15,30.5,0.9910193);
   grae->SetPointError(15,0,0,0.006161442,0.006161442);
   grae->SetPoint(16,32.5,0.9934068);
   grae->SetPointError(16,0,0,0.004820107,0.004820107);
   grae->SetPoint(17,34.5,0.9951595);
   grae->SetPointError(17,0,0,0.003756498,0.003756498);
   grae->SetPoint(18,36.5,0.9964463);
   grae->SetPointError(18,0,0,0.002917747,0.002917747);
   grae->SetPoint(19,38.5,0.997391);
   grae->SetPointError(19,0,0,0.002259468,0.002259468);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Graph",100,0,42.3);
   Graph_Graph2->SetMinimum(0);
   Graph_Graph2->SetMaximum(1.093222);
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
   text = pt->AddText("#alpha = 0.1545 #pm 0.0089");
   pt->Draw();
   TLine *line = new TLine(4.485979,0.02634816,4.485979,1.088514);

   ci = TColor::GetColor("#ff9933");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
