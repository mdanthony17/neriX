{
//=========Macro generated from canvas: c1/c1
//=========  (Tue Feb 16 14:15:00 2016) by ROOT version5.34/26
   TCanvas *c1 = new TCanvas("c1", "c1",200,45,1000,700);
   c1->SetHighLightColor(2);
   c1->Range(-5.7,-0.1339597,44.7,1.205637);
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
   grae->SetPoint(1,2.5,0.1320182);
   grae->SetPointError(1,1,1,0.01335189,0.01453188);
   grae->SetPoint(2,4.5,0.2703252);
   grae->SetPointError(2,1,1,0.02053036,0.02151015);
   grae->SetPoint(3,6.5,0.469697);
   grae->SetPointError(3,1,1,0.02622732,0.02638782);
   grae->SetPoint(4,8.5,0.574344);
   grae->SetPointError(4,1,1,0.02834062,0.02788429);
   grae->SetPoint(5,10.5,0.6933798);
   grae->SetPointError(5,1,1,0.02962675,0.02819603);
   grae->SetPoint(6,12.5,0.7056738);
   grae->SetPointError(6,1,1,0.02963916,0.02808864);
   grae->SetPoint(7,14.5,0.7857143);
   grae->SetPointError(7,1,1,0.03210915,0.0291681);
   grae->SetPoint(8,16.5,0.8176471);
   grae->SetPointError(8,1,1,0.03453652,0.0304437);
   grae->SetPoint(9,18.5,0.8193548);
   grae->SetPointError(9,1,1,0.03631745,0.03178531);
   grae->SetPoint(10,20.5,0.8346457);
   grae->SetPointError(10,1,1,0.03974365,0.03387423);
   grae->SetPoint(11,22.5,0.8305085);
   grae->SetPointError(11,1,1,0.04179341,0.03553888);
   grae->SetPoint(12,24.5,0.877551);
   grae->SetPointError(12,1,1,0.04255139,0.0337195);
   grae->SetPoint(13,26.5,0.9066667);
   grae->SetPointError(13,1,1,0.04665001,0.03377537);
   grae->SetPoint(14,28.5,0.9454545);
   grae->SetPointError(14,1,1,0.05022318,0.02953925);
   grae->SetPoint(15,30.5,0.9032258);
   grae->SetPointError(15,1,1,0.05332885,0.03771076);
   grae->SetPoint(16,32.5,0.8965517);
   grae->SetPointError(16,1,1,0.05667643,0.0402596);
   grae->SetPoint(17,34.5,0.9);
   grae->SetPointError(17,1,1,0.07203184,0.04723751);
   grae->SetPoint(18,36.5,0.8823529);
   grae->SetPointError(18,1,1,0.08329135,0.05543815);
   grae->SetPoint(19,38.5,0.893617);
   grae->SetPointError(19,1,1,0.06556213,0.04517251);
   
   TH1F *Graph_eff_graph1 = new TH1F("Graph_eff_graph1","S1 Efficiency for TAC - nerix_160209_0955",100,-4.5,43.5);
   Graph_eff_graph1->SetMinimum(0);
   Graph_eff_graph1->SetMaximum(1.071678);
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
   fEfficiency->SetChisquare(19.10135);
   fEfficiency->SetNDF(19);
   fEfficiency->GetXaxis()->SetLabelFont(42);
   fEfficiency->GetXaxis()->SetLabelSize(0.035);
   fEfficiency->GetXaxis()->SetTitleSize(0.035);
   fEfficiency->GetXaxis()->SetTitleFont(42);
   fEfficiency->GetYaxis()->SetLabelFont(42);
   fEfficiency->GetYaxis()->SetLabelSize(0.035);
   fEfficiency->GetYaxis()->SetTitleSize(0.035);
   fEfficiency->GetYaxis()->SetTitleFont(42);
   fEfficiency->SetParameter(0,0.09240275);
   fEfficiency->SetParError(0,0.003618947);
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
   grae->SetPoint(0,0.5,0.04515034);
   grae->SetPointError(0,0,0,0.003625909,0.003625909);
   grae->SetPoint(1,2.5,0.206266);
   grae->SetPointError(1,0,0,0.01507047,0.01507047);
   grae->SetPoint(2,4.5,0.3401959);
   grae->SetPointError(2,0,0,0.02254963,0.02254963);
   grae->SetPoint(3,6.5,0.4515274);
   grae->SetPointError(3,0,0,0.02707573,0.02707573);
   grae->SetPoint(4,8.5,0.5440733);
   grae->SetPointError(4,0,0,0.02943241,0.02943241);
   grae->SetPoint(5,10.5,0.6210037);
   grae->SetPointError(5,0,0,0.0302229,0.0302229);
   grae->SetPoint(6,12.5,0.6849533);
   grae->SetPointError(6,0,0,0.02990865,0.02990865);
   grae->SetPoint(7,14.5,0.7381124);
   grae->SetPointError(7,0,0,0.02883997,0.02883997);
   grae->SetPoint(8,16.5,0.7823017);
   grae->SetPointError(8,0,0,0.0272804,0.0272804);
   grae->SetPoint(9,18.5,0.8190349);
   grae->SetPointError(9,0,0,0.02542603,0.02542603);
   grae->SetPoint(10,20.5,0.8495699);
   grae->SetPointError(10,0,0,0.02342074,0.02342074);
   grae->SetPoint(11,22.5,0.8749525);
   grae->SetPointError(11,0,0,0.02136827,0.02136827);
   grae->SetPoint(12,24.5,0.8960523);
   grae->SetPointError(12,0,0,0.01934162,0.01934162);
   grae->SetPoint(13,26.5,0.9135918);
   grae->SetPointError(13,0,0,0.01739052,0.01739052);
   grae->SetPoint(14,28.5,0.9281718);
   grae->SetPointError(14,0,0,0.01554718,0.01554718);
   grae->SetPoint(15,30.5,0.9402916);
   grae->SetPointError(15,0,0,0.01383078,0.01383078);
   grae->SetPoint(16,32.5,0.9503665);
   grae->SetPointError(16,0,0,0.01225096,0.01225096);
   grae->SetPoint(17,34.5,0.9587414);
   grae->SetPointError(17,0,0,0.0108105,0.0108105);
   grae->SetPoint(18,36.5,0.9657031);
   grae->SetPointError(18,0,0,0.009507353,0.009507353);
   grae->SetPoint(19,38.5,0.9714901);
   grae->SetPointError(19,0,0,0.008336188,0.008336188);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Graph",100,0,42.3);
   Graph_Graph2->SetMinimum(0);
   Graph_Graph2->SetMaximum(1.073657);
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
   text = pt->AddText("#alpha = 0.0924 #pm 0.0036");
   pt->Draw();
   TLine *line = new TLine(7.50137,0,7.50137,1.071678);

   ci = TColor::GetColor("#ff9933");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
