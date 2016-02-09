{
//=========Macro generated from canvas: c1/c1
//=========  (Wed Feb  3 15:51:29 2016) by ROOT version5.34/26
   TCanvas *c1 = new TCanvas("c1", "c1",200,45,1000,700);
   c1->SetHighLightColor(2);
   c1->Range(-5.7,-0.1352215,44.7,1.216994);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetGridx();
   c1->SetGridy();
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(20);
   grae->SetName("eff_graph");
   grae->SetTitle("S1 Efficiency for TAC - nerix_160127_1300");
   grae->SetFillColor(19);
   grae->SetPoint(0,0.5,0.02808989);
   grae->SetPointError(0,1,1,0.01208054,0.0185555);
   grae->SetPoint(1,2.5,0.1744);
   grae->SetPointError(1,1,1,0.01542272,0.0165185);
   grae->SetPoint(2,4.5,0.3118908);
   grae->SetPointError(2,1,1,0.02102496,0.02179235);
   grae->SetPoint(3,6.5,0.4444444);
   grae->SetPointError(3,1,1,0.02365234,0.02389569);
   grae->SetPoint(4,8.5,0.5679612);
   grae->SetPointError(4,1,1,0.02575888,0.0254131);
   grae->SetPoint(5,10.5,0.6873065);
   grae->SetPointError(5,1,1,0.02791589,0.02668888);
   grae->SetPoint(6,12.5,0.7666667);
   grae->SetPointError(6,1,1,0.02860604,0.02649352);
   grae->SetPoint(7,14.5,0.78327);
   grae->SetPointError(7,1,1,0.02842152,0.02611167);
   grae->SetPoint(8,16.5,0.784689);
   grae->SetPointError(8,1,1,0.03223849,0.02929386);
   grae->SetPoint(9,18.5,0.8368421);
   grae->SetPointError(9,1,1,0.03133269,0.02745183);
   grae->SetPoint(10,20.5,0.8560606);
   grae->SetPointError(10,1,1,0.03728983,0.0312558);
   grae->SetPoint(11,22.5,0.8263889);
   grae->SetPointError(11,1,1,0.03746016,0.03245069);
   grae->SetPoint(12,24.5,0.9056604);
   grae->SetPointError(12,1,1,0.037526,0.02865718);
   grae->SetPoint(13,26.5,0.9135802);
   grae->SetPointError(13,1,1,0.04344359,0.03131921);
   grae->SetPoint(14,28.5,0.9594595);
   grae->SetPointError(14,1,1,0.03786625,0.02198324);
   grae->SetPoint(15,30.5,0.9032258);
   grae->SetPointError(15,1,1,0.05332885,0.03771076);
   grae->SetPoint(16,32.5,0.9148936);
   grae->SetPointError(16,1,1,0.06219152,0.04028366);
   grae->SetPoint(17,34.5,0.86);
   grae->SetPointError(17,1,1,0.0672437,0.05015192);
   grae->SetPoint(18,36.5,0.9574468);
   grae->SetPointError(18,1,1,0.05338482,0.02743771);
   grae->SetPoint(19,38.5,0.9117647);
   grae->SetPointError(19,1,1,0.07844157,0.04763239);
   
   TH1F *Graph_eff_graph1 = new TH1F("Graph_eff_graph1","S1 Efficiency for TAC - nerix_160127_1300",100,-4.5,43.5);
   Graph_eff_graph1->SetMinimum(0);
   Graph_eff_graph1->SetMaximum(1.081772);
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
   TText *text = pt->AddText("S1 Efficiency for TAC - nerix_160127_1300");
   pt->Draw();
   
   TF1 *fEfficiency = new TF1("fEfficiency","1.-exp(-[0]*x)",-0.5,39.5);
   fEfficiency->SetFillColor(19);
   fEfficiency->SetFillStyle(0);
   fEfficiency->SetLineColor(2);
   fEfficiency->SetLineWidth(2);
   fEfficiency->SetChisquare(17.68366);
   fEfficiency->SetNDF(19);
   fEfficiency->GetXaxis()->SetLabelFont(42);
   fEfficiency->GetXaxis()->SetLabelSize(0.035);
   fEfficiency->GetXaxis()->SetTitleSize(0.035);
   fEfficiency->GetXaxis()->SetTitleFont(42);
   fEfficiency->GetYaxis()->SetLabelFont(42);
   fEfficiency->GetYaxis()->SetLabelSize(0.035);
   fEfficiency->GetYaxis()->SetTitleSize(0.035);
   fEfficiency->GetYaxis()->SetTitleFont(42);
   fEfficiency->SetParameter(0,0.09562369);
   fEfficiency->SetParError(0,0.003565391);
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
   grae->SetPoint(0,0.5,0.04668686);
   grae->SetPointError(0,0,0,0.003431598,0.003431598);
   grae->SetPoint(1,2.5,0.2126317);
   grae->SetPointError(1,0,0,0.01417127,0.01417127);
   grae->SetPoint(2,4.5,0.3496903);
   grae->SetPointError(2,0,0,0.02106801,0.02106801);
   grae->SetPoint(3,6.5,0.4628909);
   grae->SetPointError(3,0,0,0.0251343,0.0251343);
   grae->SetPoint(4,8.5,0.5563864);
   grae->SetPointError(4,0,0,0.02714655,0.02714655);
   grae->SetPoint(5,10.5,0.633607);
   grae->SetPointError(5,0,0,0.02769665,0.02769665);
   grae->SetPoint(6,12.5,0.6973857);
   grae->SetPointError(6,0,0,0.02723268,0.02723268);
   grae->SetPoint(7,14.5,0.7500623);
   grae->SetPointError(7,0,0,0.026091,0.026091);
   grae->SetPoint(8,16.5,0.7935694);
   grae->SetPointError(8,0,0,0.02452161,0.02452161);
   grae->SetPoint(9,18.5,0.8295031);
   grae->SetPointError(9,0,0,0.02270801,0.02270801);
   grae->SetPoint(10,20.5,0.8591818);
   grae->SetPointError(10,0,0,0.02078277,0.02078277);
   grae->SetPoint(11,22.5,0.8836943);
   grae->SetPointError(11,0,0,0.01883972,0.01883972);
   grae->SetPoint(12,24.5,0.9039398);
   grae->SetPointError(12,0,0,0.01694339,0.01694339);
   grae->SetPoint(13,26.5,0.9206612);
   grae->SetPointError(13,0,0,0.01513639,0.01513639);
   grae->SetPoint(14,28.5,0.9344718);
   grae->SetPointError(14,0,0,0.01344509,0.01344509);
   grae->SetPoint(15,30.5,0.9458784);
   grae->SetPointError(15,0,0,0.01188396,0.01188396);
   grae->SetPoint(16,32.5,0.9552995);
   grae->SetPointError(16,0,0,0.01045892,0.01045892);
   grae->SetPoint(17,34.5,0.9630806);
   grae->SetPointError(17,0,0,0.009169907,0.009169907);
   grae->SetPoint(18,36.5,0.9695072);
   grae->SetPointError(18,0,0,0.00801274,0.00801274);
   grae->SetPoint(19,38.5,0.9748151);
   grae->SetPointError(19,0,0,0.006980575,0.006980575);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Graph",100,0,42.3);
   Graph_Graph2->SetMinimum(0);
   Graph_Graph2->SetMaximum(1.07565);
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
   text = pt->AddText("#alpha = 0.0956 #pm 0.0036");
   pt->Draw();
   TLine *line = new TLine(7.248697,0,7.248697,1.081772);

   ci = TColor::GetColor("#ff9933");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
