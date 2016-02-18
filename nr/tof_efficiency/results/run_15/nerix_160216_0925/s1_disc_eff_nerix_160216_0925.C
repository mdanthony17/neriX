{
//=========Macro generated from canvas: c1/c1
//=========  (Wed Feb 17 15:30:39 2016) by ROOT version5.34/26
   TCanvas *c1 = new TCanvas("c1", "c1",200,45,1000,700);
   c1->SetHighLightColor(2);
   c1->Range(-5.7,-0.1314552,44.7,1.183097);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetGridx();
   c1->SetGridy();
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(20);
   grae->SetName("eff_graph");
   grae->SetTitle("S1 Efficiency for TAC - nerix_160216_0925");
   grae->SetFillColor(19);
   grae->SetPoint(0,0.5,0.05882353);
   grae->SetPointError(0,1,1,0.01898125,0.02570898);
   grae->SetPoint(1,2.5,0.266559);
   grae->SetPointError(1,1,1,0.01817276,0.01896051);
   grae->SetPoint(2,4.5,0.4348624);
   grae->SetPointError(2,1,1,0.02200706,0.02225614);
   grae->SetPoint(3,6.5,0.5731463);
   grae->SetPointError(3,1,1,0.0232734,0.02296735);
   grae->SetPoint(4,8.5,0.6781003);
   grae->SetPointError(4,1,1,0.02578076,0.02479088);
   grae->SetPoint(5,10.5,0.7257143);
   grae->SetPointError(5,1,1,0.02592636,0.02456173);
   grae->SetPoint(6,12.5,0.770073);
   grae->SetPointError(6,1,1,0.02825887,0.02615083);
   grae->SetPoint(7,14.5,0.8273092);
   grae->SetPointError(7,1,1,0.02735004,0.02450778);
   grae->SetPoint(8,16.5,0.722488);
   grae->SetPointError(8,1,1,0.03443491,0.03214833);
   grae->SetPoint(9,18.5,0.7764706);
   grae->SetPointError(9,1,1,0.03657853,0.03303982);
   grae->SetPoint(10,20.5,0.8409091);
   grae->SetPointError(10,1,1,0.03841697,0.03266532);
   grae->SetPoint(11,22.5,0.8543689);
   grae->SetPointError(11,1,1,0.0433985,0.03560654);
   grae->SetPoint(12,24.5,0.8247423);
   grae->SetPointError(12,1,1,0.04736578,0.03982728);
   grae->SetPoint(13,26.5,0.875);
   grae->SetPointError(13,1,1,0.04335218,0.03439678);
   grae->SetPoint(14,28.5,0.8837209);
   grae->SetPointError(14,1,1,0.04546334,0.03512073);
   grae->SetPoint(15,30.5,0.9295775);
   grae->SetPointError(15,1,1,0.04483222,0.03008213);
   grae->SetPoint(16,32.5,0.8867925);
   grae->SetPointError(16,1,1,0.06149454,0.04397407);
   grae->SetPoint(17,34.5,0.875);
   grae->SetPointError(17,1,1,0.06082416,0.04492713);
   grae->SetPoint(18,36.5,0.8333333);
   grae->SetPointError(18,1,1,0.06254653,0.04965859);
   grae->SetPoint(19,38.5,0.8723404);
   grae->SetPointError(19,1,1,0.0684614,0.04944672);
   
   TH1F *Graph_eff_graph1 = new TH1F("Graph_eff_graph1","S1 Efficiency for TAC - nerix_160216_0925",100,-4.5,43.5);
   Graph_eff_graph1->SetMinimum(0);
   Graph_eff_graph1->SetMaximum(1.051641);
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
   TText *text = pt->AddText("S1 Efficiency for TAC - nerix_160216_0925");
   pt->Draw();
   
   TF1 *fEfficiency = new TF1("fEfficiency","1.-exp(-[0]*x)",-0.5,39.5);
   fEfficiency->SetFillColor(19);
   fEfficiency->SetFillStyle(0);
   fEfficiency->SetLineColor(2);
   fEfficiency->SetLineWidth(2);
   fEfficiency->SetChisquare(56.4232);
   fEfficiency->SetNDF(19);
   fEfficiency->GetXaxis()->SetLabelFont(42);
   fEfficiency->GetXaxis()->SetLabelSize(0.035);
   fEfficiency->GetXaxis()->SetTitleSize(0.035);
   fEfficiency->GetXaxis()->SetTitleFont(42);
   fEfficiency->GetYaxis()->SetLabelFont(42);
   fEfficiency->GetYaxis()->SetLabelSize(0.035);
   fEfficiency->GetYaxis()->SetTitleSize(0.035);
   fEfficiency->GetYaxis()->SetTitleFont(42);
   fEfficiency->SetParameter(0,0.09648071);
   fEfficiency->SetParError(0,0.003712318);
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
   grae->SetPoint(0,0.5,0.04709528);
   grae->SetPointError(0,0,0,0.006379567,0.006379567);
   grae->SetPoint(1,2.5,0.2143169);
   grae->SetPointError(1,0,0,0.0263002,0.0263002);
   grae->SetPoint(2,4.5,0.3521935);
   grae->SetPointError(2,0,0,0.03903279,0.03903279);
   grae->SetPoint(3,6.5,0.4658746);
   grae->SetPointError(3,0,0,0.04648666,0.04648666);
   grae->SetPoint(4,8.5,0.5596063);
   grae->SetPointError(4,0,0,0.0501224,0.0501224);
   grae->SetPoint(5,10.5,0.6368893);
   grae->SetPointError(5,0,0,0.05105051,0.05105051);
   grae->SetPoint(6,12.5,0.7006102);
   grae->SetPointError(6,0,0,0.05010935,0.05010935);
   grae->SetPoint(7,14.5,0.753149);
   grae->SetPointError(7,0,0,0.04792638,0.04792638);
   grae->SetPoint(8,16.5,0.796468);
   grae->SetPointError(8,0,0,0.04496644,0.04496644);
   grae->SetPoint(9,18.5,0.832185);
   grae->SetPointError(9,0,0,0.04156945,0.04156945);
   grae->SetPoint(10,20.5,0.8616342);
   grae->SetPointError(10,0,0,0.03797994,0.03797994);
   grae->SetPoint(11,22.5,0.8859155);
   grae->SetPointError(11,0,0,0.03437011,0.03437011);
   grae->SetPoint(12,24.5,0.9059358);
   grae->SetPointError(12,0,0,0.03085761,0.03085761);
   grae->SetPoint(13,26.5,0.9224427);
   grae->SetPointError(13,0,0,0.02751947,0.02751947);
   grae->SetPoint(14,28.5,0.936053);
   grae->SetPointError(14,0,0,0.02440265,0.02440265);
   grae->SetPoint(15,30.5,0.9472748);
   grae->SetPointError(15,0,0,0.02153227,0.02153227);
   grae->SetPoint(16,32.5,0.9565274);
   grae->SetPointError(16,0,0,0.01891783,0.01891783);
   grae->SetPoint(17,34.5,0.9641562);
   grae->SetPointError(17,0,0,0.01655789,0.01655789);
   grae->SetPoint(18,36.5,0.9704463);
   grae->SetPointError(18,0,0,0.01444364,0.01444364);
   grae->SetPoint(19,38.5,0.9756325);
   grae->SetPointError(19,0,0,0.01256153,0.01256153);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Graph",100,0,42.3);
   Graph_Graph2->SetMinimum(0);
   Graph_Graph2->SetMaximum(1.082942);
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
   text = pt->AddText("#alpha = 0.0965 #pm 0.0037");
   pt->Draw();
   TLine *line = new TLine(7.184309,0,7.184309,1.051641);

   ci = TColor::GetColor("#ff9933");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
