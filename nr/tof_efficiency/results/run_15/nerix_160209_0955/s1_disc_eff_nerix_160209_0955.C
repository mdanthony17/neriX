{
//=========Macro generated from canvas: c1/c1
//=========  (Mon Mar 21 14:49:25 2016) by ROOT version5.34/26
   TCanvas *c1 = new TCanvas("c1", "c1",200,45,1000,700);
   c1->SetHighLightColor(2);
   c1->Range(-5.7,-0.1373981,44.7,1.236583);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetGridx();
   c1->SetGridy();
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(20);
   grae->SetName("eff_graph");
   grae->SetTitle("S1 Efficiency for TAC - nerix_160209_0955");
   grae->SetFillColor(19);
   grae->SetPoint(0,0.5,0.01785714);
   grae->SetPointError(0,1,1,0.009703041,0.01706491);
   grae->SetPoint(1,2.5,0.1229135);
   grae->SetPointError(1,1,1,0.01294094,0.01415218);
   grae->SetPoint(2,4.5,0.2865854);
   grae->SetPointError(2,1,1,0.02092691,0.02183656);
   grae->SetPoint(3,6.5,0.489899);
   grae->SetPointError(3,1,1,0.02632176,0.02637526);
   grae->SetPoint(4,8.5,0.6005831);
   grae->SetPointError(4,1,1,0.02817035,0.02755266);
   grae->SetPoint(5,10.5,0.6968641);
   grae->SetPointError(5,1,1,0.02955298,0.0280962);
   grae->SetPoint(6,12.5,0.7375887);
   grae->SetPointError(6,1,1,0.02882504,0.02702996);
   grae->SetPoint(7,14.5,0.847619);
   grae->SetPointError(7,1,1,0.02895558,0.0253379);
   grae->SetPoint(8,16.5,0.8647059);
   grae->SetPointError(8,1,1,0.03151341,0.02675951);
   grae->SetPoint(9,18.5,0.8258065);
   grae->SetPointError(9,1,1,0.03593265,0.03130267);
   grae->SetPoint(10,20.5,0.8661417);
   grae->SetPointError(10,1,1,0.03733146,0.03084742);
   grae->SetPoint(11,22.5,0.8898305);
   grae->SetPointError(11,1,1,0.03679091,0.02925459);
   grae->SetPoint(12,24.5,0.9081633);
   grae->SetPointError(12,1,1,0.03911912,0.02940066);
   grae->SetPoint(13,26.5,0.9333333);
   grae->SetPointError(13,1,1,0.0425828,0.02849518);
   grae->SetPoint(14,28.5,0.9636364);
   grae->SetPointError(14,1,1,0.04595725,0.0234528);
   grae->SetPoint(15,30.5,0.9354839);
   grae->SetPointError(15,1,1,0.0480773,0.03062178);
   grae->SetPoint(16,32.5,1);
   grae->SetPointError(16,1,1,0.03124327,0);
   grae->SetPoint(17,34.5,0.975);
   grae->SetPointError(17,1,1,0.05515157,0.02069047);
   grae->SetPoint(18,36.5,0.9411765);
   grae->SetPointError(18,1,1,0.0723694,0.0379027);
   grae->SetPoint(19,38.5,0.9787234);
   grae->SetPointError(19,1,1,0.04723001,0.01760773);
   
   TH1F *Graph_eff_graph1 = new TH1F("Graph_eff_graph1","S1 Efficiency for TAC - nerix_160209_0955",100,-4.5,43.5);
   Graph_eff_graph1->SetMinimum(0);
   Graph_eff_graph1->SetMaximum(1.099185);
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
   TText *text = pt->AddText("S1 Efficiency for TAC - nerix_160209_0955");
   pt->Draw();
   
   TF1 *fEfficiency = new TF1("fEfficiency","1.-exp(-[0]*x)",-0.5,39.5);
   fEfficiency->SetFillColor(19);
   fEfficiency->SetFillStyle(0);
   fEfficiency->SetLineColor(2);
   fEfficiency->SetLineWidth(2);
   fEfficiency->SetChisquare(12.86499);
   fEfficiency->SetNDF(19);
   fEfficiency->GetXaxis()->SetLabelFont(42);
   fEfficiency->GetXaxis()->SetLabelSize(0.035);
   fEfficiency->GetXaxis()->SetTitleSize(0.035);
   fEfficiency->GetXaxis()->SetTitleFont(42);
   fEfficiency->GetYaxis()->SetLabelFont(42);
   fEfficiency->GetYaxis()->SetLabelSize(0.035);
   fEfficiency->GetYaxis()->SetTitleSize(0.035);
   fEfficiency->GetYaxis()->SetTitleFont(42);
   fEfficiency->SetParameter(0,0.1056824);
   fEfficiency->SetParError(0,0.004068933);
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
   grae->SetPoint(0,0.5,0.05146939);
   grae->SetPointError(0,0,0,0.003323565,0.003323565);
   grae->SetPoint(1,2.5,0.2321847);
   grae->SetPointError(1,0,0,0.01345178,0.01345178);
   grae->SetPoint(2,4.5,0.3784699);
   grae->SetPointError(2,0,0,0.01960007,0.01960007);
   grae->SetPoint(3,6.5,0.4968846);
   grae->SetPointError(3,0,0,0.02291732,0.02291732);
   grae->SetPoint(4,8.5,0.5927388);
   grae->SetPointError(4,0,0,0.02425911,0.02425911);
   grae->SetPoint(5,10.5,0.6703308);
   grae->SetPointError(5,0,0,0.02425775,0.02425775);
   grae->SetPoint(6,12.5,0.7331398);
   grae->SetPointError(6,0,0,0.02337635,0.02337635);
   grae->SetPoint(7,14.5,0.7839823);
   grae->SetPointError(7,0,0,0.02195028,0.02195028);
   grae->SetPoint(8,16.5,0.8251383);
   grae->SetPointError(8,0,0,0.02021908,0.02021908);
   grae->SetPoint(9,18.5,0.8584532);
   grae->SetPointError(9,0,0,0.01835078,0.01835078);
   grae->SetPoint(10,20.5,0.8854209);
   grae->SetPointError(10,0,0,0.01646047,0.01646047);
   grae->SetPoint(11,22.5,0.9072506);
   grae->SetPointError(11,0,0,0.01462434,0.01462434);
   grae->SetPoint(12,24.5,0.9249214);
   grae->SetPointError(12,0,0,0.01289036,0.01289036);
   grae->SetPoint(13,26.5,0.9392254);
   grae->SetPointError(13,0,0,0.01128627,0.01128627);
   grae->SetPoint(14,28.5,0.9508043);
   grae->SetPointError(14,0,0,0.009825503,0.009825503);
   grae->SetPoint(15,30.5,0.9601771);
   grae->SetPointError(15,0,0,0.008511678,0.008511678);
   grae->SetPoint(16,32.5,0.9677643);
   grae->SetPointError(16,0,0,0.007341827,0.007341827);
   grae->SetPoint(17,34.5,0.9739059);
   grae->SetPointError(17,0,0,0.006308779,0.006308779);
   grae->SetPoint(18,36.5,0.9788773);
   grae->SetPointError(18,0,0,0.00540287,0.00540287);
   grae->SetPoint(19,38.5,0.9829016);
   grae->SetPointError(19,0,0,0.004613152,0.004613152);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Graph",100,0,42.3);
   Graph_Graph2->SetMinimum(0);
   Graph_Graph2->SetMaximum(1.081452);
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
   text = pt->AddText("#alpha = 0.1057 #pm 0.0041");
   pt->Draw();
   TLine *line = new TLine(6.558774,0,6.558774,1.099185);

   ci = TColor::GetColor("#ff9933");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
