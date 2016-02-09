{
//=========Macro generated from canvas: c1/c1
//=========  (Wed Feb  3 15:46:16 2016) by ROOT version5.34/26
   TCanvas *c1 = new TCanvas("c1", "c1",200,45,1000,700);
   c1->SetHighLightColor(2);
   c1->Range(-5.7,-0.1235689,44.7,1.11212);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetGridx();
   c1->SetGridy();
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(20);
   grae->SetName("eff_graph");
   grae->SetTitle("S1 Efficiency for TAC - nerix_160105_0939");
   grae->SetFillColor(19);
   grae->SetPoint(0,0.5,0.01809955);
   grae->SetPointError(0,1,1,0.008642503,0.01407946);
   grae->SetPoint(1,2.5,0.1105528);
   grae->SetPointError(1,1,1,0.01591648,0.01802675);
   grae->SetPoint(2,4.5,0.1859649);
   grae->SetPointError(2,1,1,0.02358887,0.02595523);
   grae->SetPoint(3,6.5,0.3132075);
   grae->SetPointError(3,1,1,0.02957268,0.03107222);
   grae->SetPoint(4,8.5,0.4570312);
   grae->SetPointError(4,1,1,0.03284134,0.03319721);
   grae->SetPoint(5,10.5,0.476378);
   grae->SetPointError(5,1,1,0.03313822,0.03333541);
   grae->SetPoint(6,12.5,0.5728643);
   grae->SetPointError(6,1,1,0.03786972,0.03708745);
   grae->SetPoint(7,14.5,0.6363636);
   grae->SetPointError(7,1,1,0.04124335,0.03946363);
   grae->SetPoint(8,16.5,0.6865672);
   grae->SetPointError(8,1,1,0.04515774,0.04212848);
   grae->SetPoint(9,18.5,0.7310345);
   grae->SetPointError(9,1,1,0.04187861,0.03840891);
   grae->SetPoint(10,20.5,0.7009346);
   grae->SetPointError(10,1,1,0.05077281,0.04664566);
   grae->SetPoint(11,22.5,0.7333333);
   grae->SetPointError(11,1,1,0.05015523,0.04525088);
   grae->SetPoint(12,24.5,0.7764706);
   grae->SetPointError(12,1,1,0.05445604,0.04716085);
   grae->SetPoint(13,26.5,0.7931034);
   grae->SetPointError(13,1,1,0.06715432,0.05554554);
   grae->SetPoint(14,28.5,0.8333333);
   grae->SetPointError(14,1,1,0.05898296,0.04733417);
   grae->SetPoint(15,30.5,0.8421053);
   grae->SetPointError(15,1,1,0.06372643,0.04971307);
   grae->SetPoint(16,32.5,0.8490566);
   grae->SetPointError(16,1,1,0.06598017,0.05048607);
   grae->SetPoint(17,34.5,0.7727273);
   grae->SetPointError(17,1,1,0.0809453,0.06654532);
   grae->SetPoint(18,36.5,0.7954545);
   grae->SetPointError(18,1,1,0.07929081,0.06359974);
   grae->SetPoint(19,38.5,0.7837838);
   grae->SetPointError(19,1,1,0.08919379,0.07113464);
   
   TH1F *Graph_eff_graph1 = new TH1F("Graph_eff_graph1","S1 Efficiency for TAC - nerix_160105_0939",100,-4.5,43.5);
   Graph_eff_graph1->SetMinimum(0);
   Graph_eff_graph1->SetMaximum(0.9885512);
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
   TText *text = pt->AddText("S1 Efficiency for TAC - nerix_160105_0939");
   pt->Draw();
   
   TF1 *fEfficiency = new TF1("fEfficiency","1.-exp(-[0]*x)",-0.5,39.5);
   fEfficiency->SetFillColor(19);
   fEfficiency->SetFillStyle(0);
   fEfficiency->SetLineColor(2);
   fEfficiency->SetLineWidth(2);
   fEfficiency->SetChisquare(14.52467);
   fEfficiency->SetNDF(19);
   fEfficiency->GetXaxis()->SetLabelFont(42);
   fEfficiency->GetXaxis()->SetLabelSize(0.035);
   fEfficiency->GetXaxis()->SetTitleSize(0.035);
   fEfficiency->GetXaxis()->SetTitleFont(42);
   fEfficiency->GetYaxis()->SetLabelFont(42);
   fEfficiency->GetYaxis()->SetLabelSize(0.035);
   fEfficiency->GetYaxis()->SetTitleSize(0.035);
   fEfficiency->GetYaxis()->SetTitleFont(42);
   fEfficiency->SetParameter(0,0.06194156);
   fEfficiency->SetParError(0,0.002439179);
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
   grae->SetPoint(0,0.5,0.0304961);
   grae->SetPointError(0,0,0,0.002163785,0.002163785);
   grae->SetPoint(1,2.5,0.1434597);
   grae->SetPointError(1,0,0,0.009558336,0.009558336);
   grae->SetPoint(2,4.5,0.2432611);
   grae->SetPointError(2,0,0,0.01520033,0.01520033);
   grae->SetPoint(3,6.5,0.331434);
   grae->SetPointError(3,0,0,0.01939778,0.01939778);
   grae->SetPoint(4,8.5,0.4093332);
   grae->SetPointError(4,0,0,0.02241073,0.02241073);
   grae->SetPoint(5,10.5,0.4781559);
   grae->SetPointError(5,0,0,0.02445821,0.02445821);
   grae->SetPoint(6,12.5,0.5389595);
   grae->SetPointError(6,0,0,0.0257243,0.0257243);
   grae->SetPoint(7,14.5,0.5926785);
   grae->SetPointError(7,0,0,0.0263633,0.0263633);
   grae->SetPoint(8,16.5,0.6401384);
   grae->SetPointError(8,0,0,0.02650416,0.02650416);
   grae->SetPoint(9,18.5,0.6820683);
   grae->SetPointError(9,0,0,0.02625427,0.02625427);
   grae->SetPoint(10,20.5,0.7191128);
   grae->SetPointError(10,0,0,0.0257028,0.0257028);
   grae->SetPoint(11,22.5,0.7518408);
   grae->SetPointError(11,0,0,0.0249234,0.0249234);
   grae->SetPoint(12,24.5,0.7807556);
   grae->SetPointError(12,0,0,0.02397668,0.02397668);
   grae->SetPoint(13,26.5,0.8063013);
   grae->SetPointError(13,0,0,0.02291222,0.02291222);
   grae->SetPoint(14,28.5,0.8288704);
   grae->SetPointError(14,0,0,0.0217703,0.0217703);
   grae->SetPoint(15,30.5,0.8488099);
   grae->SetPointError(15,0,0,0.02058342,0.02058342);
   grae->SetPoint(16,32.5,0.8664261);
   grae->SetPointError(16,0,0,0.01937757,0.01937757);
   grae->SetPoint(17,34.5,0.8819897);
   grae->SetPointError(17,0,0,0.01817328,0.01817328);
   grae->SetPoint(18,36.5,0.8957399);
   grae->SetPointError(18,0,0,0.01698656,0.01698656);
   grae->SetPoint(19,38.5,0.907888);
   grae->SetPointError(19,0,0,0.01582966,0.01582966);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Graph",100,0,42.3);
   Graph_Graph2->SetMinimum(0);
   Graph_Graph2->SetMaximum(1.013256);
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
   text = pt->AddText("#alpha = 0.0619 #pm 0.0024");
   pt->Draw();
   TLine *line = new TLine(11.19034,0,11.19034,0.9885512);

   ci = TColor::GetColor("#ff9933");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
