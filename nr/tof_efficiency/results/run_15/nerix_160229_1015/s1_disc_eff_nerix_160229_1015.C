{
//=========Macro generated from canvas: c1/c1
//=========  (Mon Mar 21 14:48:07 2016) by ROOT version5.34/26
   TCanvas *c1 = new TCanvas("c1", "c1",200,45,1000,700);
   c1->SetHighLightColor(2);
   c1->Range(-5.7,-0.08917592,44.7,1.182848);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetGridx();
   c1->SetGridy();
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(20);
   grae->SetName("eff_graph");
   grae->SetTitle("S1 Efficiency for TAC - nerix_160229_1015");
   grae->SetFillColor(19);
   grae->SetPoint(0,0.5,0.1578947);
   grae->SetPointError(0,1,1,0.0350666,0.04180057);
   grae->SetPoint(1,2.5,0.4025974);
   grae->SetPointError(1,1,1,0.02038991,0.02071881);
   grae->SetPoint(2,4.5,0.5641953);
   grae->SetPointError(2,1,1,0.02209077,0.02184892);
   grae->SetPoint(3,6.5,0.6589958);
   grae->SetPointError(3,1,1,0.02305344,0.022357);
   grae->SetPoint(4,8.5,0.7344754);
   grae->SetPointError(4,1,1,0.02201362,0.02095823);
   grae->SetPoint(5,10.5,0.7034121);
   grae->SetPointError(5,1,1,0.02524526,0.02411944);
   grae->SetPoint(6,12.5,0.756447);
   grae->SetPointError(6,1,1,0.02515853,0.02360005);
   grae->SetPoint(7,14.5,0.8315789);
   grae->SetPointError(7,1,1,0.02515016,0.02264459);
   grae->SetPoint(8,16.5,0.8106996);
   grae->SetPointError(8,1,1,0.02852898,0.02576951);
   grae->SetPoint(9,18.5,0.7900552);
   grae->SetPointError(9,1,1,0.0347057,0.03122044);
   grae->SetPoint(10,20.5,0.8235294);
   grae->SetPointError(10,1,1,0.03420292,0.03002944);
   grae->SetPoint(11,22.5,0.7610063);
   grae->SetPointError(11,1,1,0.03865234,0.03507838);
   grae->SetPoint(12,24.5,0.8794326);
   grae->SetPointError(12,1,1,0.03395589,0.02790572);
   grae->SetPoint(13,26.5,0.8899083);
   grae->SetPointError(13,1,1,0.03862221,0.03042354);
   grae->SetPoint(14,28.5,0.8854167);
   grae->SetPointError(14,1,1,0.0422747,0.03302381);
   grae->SetPoint(15,30.5,0.8409091);
   grae->SetPointError(15,1,1,0.04891206,0.04010189);
   grae->SetPoint(16,32.5,0.9444444);
   grae->SetPointError(16,1,1,0.04174976,0.02639991);
   grae->SetPoint(17,34.5,0.8518519);
   grae->SetPointError(17,1,1,0.06491349,0.04958485);
   grae->SetPoint(18,36.5,0.7605634);
   grae->SetPointError(18,1,1,0.06145457,0.05317211);
   grae->SetPoint(19,38.5,0.9047619);
   grae->SetPointError(19,1,1,0.06891925,0.0450174);
   
   TH1F *Graph_eff_graph1 = new TH1F("Graph_eff_graph1","S1 Efficiency for TAC - nerix_160229_1015",100,-4.5,43.5);
   Graph_eff_graph1->SetMinimum(0.03802652);
   Graph_eff_graph1->SetMaximum(1.055646);
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
   TText *text = pt->AddText("S1 Efficiency for TAC - nerix_160229_1015");
   pt->Draw();
   
   TF1 *fEfficiency = new TF1("fEfficiency","1.-exp(-[0]*x)",-0.5,39.5);
   fEfficiency->SetFillColor(19);
   fEfficiency->SetFillStyle(0);
   fEfficiency->SetLineColor(2);
   fEfficiency->SetLineWidth(2);
   fEfficiency->SetChisquare(98.72967);
   fEfficiency->SetNDF(19);
   fEfficiency->GetXaxis()->SetLabelFont(42);
   fEfficiency->GetXaxis()->SetLabelSize(0.035);
   fEfficiency->GetXaxis()->SetTitleSize(0.035);
   fEfficiency->GetXaxis()->SetTitleFont(42);
   fEfficiency->GetYaxis()->SetLabelFont(42);
   fEfficiency->GetYaxis()->SetLabelSize(0.035);
   fEfficiency->GetYaxis()->SetTitleSize(0.035);
   fEfficiency->GetYaxis()->SetTitleFont(42);
   fEfficiency->SetParameter(0,0.1017139);
   fEfficiency->SetParError(0,0.003761325);
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
   grae->SetPoint(0,0.5,0.04958538);
   grae->SetPointError(0,0,0,0.008527965,0.008527965);
   grae->SetPoint(1,2.5,0.224529);
   grae->SetPointError(1,0,0,0.03479108,0.03479108);
   grae->SetPoint(2,4.5,0.3672706);
   grae->SetPointError(2,0,0,0.0510967,0.0510967);
   grae->SetPoint(3,6.5,0.4837377);
   grae->SetPointError(3,0,0,0.06022074,0.06022074);
   grae->SetPoint(4,8.5,0.5787666);
   grae->SetPointError(4,0,0,0.06425458,0.06425458);
   grae->SetPoint(5,10.5,0.6563033);
   grae->SetPointError(5,0,0,0.06476299,0.06476299);
   grae->SetPoint(6,12.5,0.7195679);
   grae->SetPointError(6,0,0,0.06290715,0.06290715);
   grae->SetPoint(7,14.5,0.7711873);
   grae->SetPointError(7,0,0,0.05954022,0.05954022);
   grae->SetPoint(8,16.5,0.8133051);
   grae->SetPointError(8,0,0,0.05528137,0.05528137);
   grae->SetPoint(9,18.5,0.8476701);
   grae->SetPointError(9,0,0,0.05057304,0.05057304);
   grae->SetPoint(10,20.5,0.8757097);
   grae->SetPointError(10,0,0,0.04572499,0.04572499);
   grae->SetPoint(11,22.5,0.8985879);
   grae->SetPointError(11,0,0,0.04094819,0.04094819);
   grae->SetPoint(12,24.5,0.9172549);
   grae->SetPointError(12,0,0,0.03638067,0.03638067);
   grae->SetPoint(13,26.5,0.9324859);
   grae->SetPointError(13,0,0,0.03210723,0.03210723);
   grae->SetPoint(14,28.5,0.9449133);
   grae->SetPointError(14,0,0,0.02817438,0.02817438);
   grae->SetPoint(15,30.5,0.9550531);
   grae->SetPointError(15,0,0,0.02460151,0.02460151);
   grae->SetPoint(16,32.5,0.9633265);
   grae->SetPointError(16,0,0,0.02138936,0.02138936);
   grae->SetPoint(17,34.5,0.970077);
   grae->SetPointError(17,0,0,0.01852618,0.01852618);
   grae->SetPoint(18,36.5,0.975585);
   grae->SetPointError(18,0,0,0.01599234,0.01599234);
   grae->SetPoint(19,38.5,0.9800791);
   grae->SetPointError(19,0,0,0.01376361,0.01376361);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Graph",100,0,42.3);
   Graph_Graph2->SetMinimum(0);
   Graph_Graph2->SetMaximum(1.089121);
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
   text = pt->AddText("#alpha = 0.1017 #pm 0.0038");
   pt->Draw();
   TLine *line = new TLine(6.814675,0.03802652,6.814675,1.055646);

   ci = TColor::GetColor("#ff9933");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
