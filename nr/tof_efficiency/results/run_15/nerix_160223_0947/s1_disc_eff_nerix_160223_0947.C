{
//=========Macro generated from canvas: c1/c1
//=========  (Tue Feb 23 12:06:31 2016) by ROOT version5.34/26
   TCanvas *c1 = new TCanvas("c1", "c1",200,45,1000,700);
   c1->SetHighLightColor(2);
   c1->Range(-5.7,-0.1309919,44.7,1.178927);
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
   grae->SetPoint(0,0.5,0.09090909);
   grae->SetPointError(0,1,1,0.02427501,0.03079876);
   grae->SetPoint(1,2.5,0.3140794);
   grae->SetPointError(1,1,1,0.02025418,0.02095531);
   grae->SetPoint(2,4.5,0.4398977);
   grae->SetPointError(2,1,1,0.02618513,0.0265077);
   grae->SetPoint(3,6.5,0.5806452);
   grae->SetPointError(3,1,1,0.02986406,0.02931495);
   grae->SetPoint(4,8.5,0.6631579);
   grae->SetPointError(4,1,1,0.03030612,0.02909209);
   grae->SetPoint(5,10.5,0.688716);
   grae->SetPointError(5,1,1,0.03155197,0.02998827);
   grae->SetPoint(6,12.5,0.6947791);
   grae->SetPointError(6,1,1,0.03196517,0.03029726);
   grae->SetPoint(7,14.5,0.7293578);
   grae->SetPointError(7,1,1,0.0334462,0.03118817);
   grae->SetPoint(8,16.5,0.6796117);
   grae->SetPointError(8,1,1,0.03578947,0.03392124);
   grae->SetPoint(9,18.5,0.7579618);
   grae->SetPointError(9,1,1,0.03905327,0.03547557);
   grae->SetPoint(10,20.5,0.8181818);
   grae->SetPointError(10,1,1,0.0399273,0.03458855);
   grae->SetPoint(11,22.5,0.8145161);
   grae->SetPointError(11,1,1,0.041638,0.0360086);
   grae->SetPoint(12,24.5,0.7789474);
   grae->SetPointError(12,1,1,0.05089775,0.04434455);
   grae->SetPoint(13,26.5,0.71875);
   grae->SetPointError(13,1,1,0.05335858,0.04831845);
   grae->SetPoint(14,28.5,0.880597);
   grae->SetPointError(14,1,1,0.05360147,0.04023966);
   grae->SetPoint(15,30.5,0.8414634);
   grae->SetPointError(15,1,1,0.05100102,0.04149147);
   grae->SetPoint(16,32.5,0.75);
   grae->SetPointError(16,1,1,0.06590414,0.05705688);
   grae->SetPoint(17,34.5,0.84);
   grae->SetPointError(17,1,1,0.0693955,0.05339668);
   grae->SetPoint(18,36.5,0.9215686);
   grae->SetPointError(18,1,1,0.05768084,0.03715752);
   grae->SetPoint(19,38.5,0.8780488);
   grae->SetPointError(19,1,1,0.07408333,0.05164584);
   
   TH1F *Graph_eff_graph1 = new TH1F("Graph_eff_graph1","S1 Efficiency for TAC - nerix_160223_0947",100,-4.5,43.5);
   Graph_eff_graph1->SetMinimum(0);
   Graph_eff_graph1->SetMaximum(1.047935);
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
   
   TF1 *fEfficiency = new TF1("fEfficiency","1.-exp(-[0]*x)",-0.5,39.5);
   fEfficiency->SetFillColor(19);
   fEfficiency->SetFillStyle(0);
   fEfficiency->SetLineColor(2);
   fEfficiency->SetLineWidth(2);
   fEfficiency->SetChisquare(73.12542);
   fEfficiency->SetNDF(19);
   fEfficiency->GetXaxis()->SetLabelFont(42);
   fEfficiency->GetXaxis()->SetLabelSize(0.035);
   fEfficiency->GetXaxis()->SetTitleSize(0.035);
   fEfficiency->GetXaxis()->SetTitleFont(42);
   fEfficiency->GetYaxis()->SetLabelFont(42);
   fEfficiency->GetYaxis()->SetLabelSize(0.035);
   fEfficiency->GetYaxis()->SetTitleSize(0.035);
   fEfficiency->GetYaxis()->SetTitleFont(42);
   fEfficiency->SetParameter(0,0.08316024);
   fEfficiency->SetParError(0,0.00314195);
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
   grae->SetPoint(0,0.5,0.04072753);
   grae->SetPointError(0,0,0,0.006187898,0.006187898);
   grae->SetPoint(1,2.5,0.1877122);
   grae->SetPointError(1,0,0,0.02619878,0.02619878);
   grae->SetPoint(2,4.5,0.3121752);
   grae->SetPointError(2,0,0,0.03993204,0.03993204);
   grae->SetPoint(3,6.5,0.4175673);
   grae->SetPointError(3,0,0,0.04884164,0.04884164);
   grae->SetPoint(4,8.5,0.5068106);
   grae->SetPointError(4,0,0,0.05408337,0.05408337);
   grae->SetPoint(5,10.5,0.5823796);
   grae->SetPointError(5,0,0,0.05657207,0.05657207);
   grae->SetPoint(6,12.5,0.6463696);
   grae->SetPointError(6,0,0,0.05702834,0.05702834);
   grae->SetPoint(7,14.5,0.7005547);
   grae->SetPointError(7,0,0,0.05601659,0.05601659);
   grae->SetPoint(8,16.5,0.7464373);
   grae->SetPointError(8,0,0,0.05397598,0.05397598);
   grae->SetPoint(9,18.5,0.7852895);
   grae->SetPointError(9,0,0,0.05124556,0.05124556);
   grae->SetPoint(10,20.5,0.8181885);
   grae->SetPointError(10,0,0,0.04808463,0.04808463);
   grae->SetPoint(11,22.5,0.8460466);
   grae->SetPointError(11,0,0,0.04468923,0.04468923);
   grae->SetPoint(12,24.5,0.8696362);
   grae->SetPointError(12,0,0,0.04120542,0.04120542);
   grae->SetPoint(13,26.5,0.8896112);
   grae->SetPointError(13,0,0,0.03774002,0.03774002);
   grae->SetPoint(14,28.5,0.9065256);
   grae->SetPointError(14,0,0,0.03436917,0.03436917);
   grae->SetPoint(15,30.5,0.9208482);
   grae->SetPointError(15,0,0,0.03114526,0.03114526);
   grae->SetPoint(16,32.5,0.9329762);
   grae->SetPointError(16,0,0,0.0281024,0.0281024);
   grae->SetPoint(17,34.5,0.9432459);
   grae->SetPointError(17,0,0,0.0252608,0.0252608);
   grae->SetPoint(18,36.5,0.9519421);
   grae->SetPointError(18,0,0,0.02263022,0.02263022);
   grae->SetPoint(19,38.5,0.9593058);
   grae->SetPointError(19,0,0,0.02021271,0.02021271);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Graph",100,0,42.3);
   Graph_Graph2->SetMinimum(0);
   Graph_Graph2->SetMaximum(1.074016);
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
   text = pt->AddText("#alpha = 0.0832 #pm 0.0031");
   pt->Draw();
   TLine *line = new TLine(8.335079,0,8.335079,1.047935);

   ci = TColor::GetColor("#ff9933");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
