{
//=========Macro generated from canvas: c1/c1
//=========  (Wed Feb  3 15:50:19 2016) by ROOT version5.34/26
   TCanvas *c1 = new TCanvas("c1", "c1",200,45,1000,700);
   c1->SetHighLightColor(2);
   c1->Range(-5.7,-0.1270879,44.7,1.143791);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetGridx();
   c1->SetGridy();
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(20);
   grae->SetName("eff_graph");
   grae->SetTitle("S1 Efficiency for TAC - nerix_160118_0922");
   grae->SetFillColor(19);
   grae->SetPoint(0,0.5,0.01838235);
   grae->SetPointError(0,1,1,0.007917618,0.01224403);
   grae->SetPoint(1,2.5,0.204947);
   grae->SetPointError(1,1,1,0.0172915,0.0183872);
   grae->SetPoint(2,4.5,0.3869565);
   grae->SetPointError(2,1,1,0.0235126,0.02402692);
   grae->SetPoint(3,6.5,0.4712644);
   grae->SetPointError(3,1,1,0.02498371,0.02512198);
   grae->SetPoint(4,8.5,0.5656566);
   grae->SetPointError(4,1,1,0.02631062,0.02596277);
   grae->SetPoint(5,10.5,0.5730028);
   grae->SetPointError(5,1,1,0.02751171,0.02708888);
   grae->SetPoint(6,12.5,0.632107);
   grae->SetPointError(6,1,1,0.02997779,0.02904311);
   grae->SetPoint(7,14.5,0.6751825);
   grae->SetPointError(7,1,1,0.03074167,0.02938361);
   grae->SetPoint(8,16.5,0.7328244);
   grae->SetPointError(8,1,1,0.03014356,0.02824675);
   grae->SetPoint(9,18.5,0.761194);
   grae->SetPointError(9,1,1,0.03389382,0.03108873);
   grae->SetPoint(10,20.5,0.7625);
   grae->SetPointError(10,1,1,0.03845475,0.034883);
   grae->SetPoint(11,22.5,0.8194444);
   grae->SetPointError(11,1,1,0.03788805,0.03299259);
   grae->SetPoint(12,24.5,0.8294574);
   grae->SetPointError(12,1,1,0.03973381,0.03405659);
   grae->SetPoint(13,26.5,0.8796296);
   grae->SetPointError(13,1,1,0.03988796,0.03186769);
   grae->SetPoint(14,28.5,0.8712871);
   grae->SetPointError(14,1,1,0.0423812,0.03399206);
   grae->SetPoint(15,30.5,0.8902439);
   grae->SetPointError(15,1,1,0.04608897,0.03498292);
   grae->SetPoint(16,32.5,0.7948718);
   grae->SetPointError(16,1,1,0.05612728,0.04757952);
   grae->SetPoint(17,34.5,0.8181818);
   grae->SetPointError(17,1,1,0.05490074,0.04549724);
   grae->SetPoint(18,36.5,0.8360656);
   grae->SetPointError(18,1,1,0.06166147,0.04888156);
   grae->SetPoint(19,38.5,0.8666667);
   grae->SetPointError(19,1,1,0.07114163,0.05158603);
   
   TH1F *Graph_eff_graph1 = new TH1F("Graph_eff_graph1","S1 Efficiency for TAC - nerix_160118_0922",100,-4.5,43.5);
   Graph_eff_graph1->SetMinimum(0);
   Graph_eff_graph1->SetMaximum(1.016703);
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
   TText *text = pt->AddText("S1 Efficiency for TAC - nerix_160118_0922");
   pt->Draw();
   
   TF1 *fEfficiency = new TF1("fEfficiency","1.-exp(-[0]*x)",-0.5,39.5);
   fEfficiency->SetFillColor(19);
   fEfficiency->SetFillStyle(0);
   fEfficiency->SetLineColor(2);
   fEfficiency->SetLineWidth(2);
   fEfficiency->SetChisquare(29.37949);
   fEfficiency->SetNDF(19);
   fEfficiency->GetXaxis()->SetLabelFont(42);
   fEfficiency->GetXaxis()->SetLabelSize(0.035);
   fEfficiency->GetXaxis()->SetTitleSize(0.035);
   fEfficiency->GetXaxis()->SetTitleFont(42);
   fEfficiency->GetYaxis()->SetLabelFont(42);
   fEfficiency->GetYaxis()->SetLabelSize(0.035);
   fEfficiency->GetYaxis()->SetTitleSize(0.035);
   fEfficiency->GetYaxis()->SetTitleFont(42);
   fEfficiency->SetParameter(0,0.07638309);
   fEfficiency->SetParError(0,0.002576566);
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
   grae->SetPoint(0,0.5,0.03747144);
   grae->SetPointError(0,0,0,0.003227339,0.003227339);
   grae->SetPoint(1,2.5,0.1738325);
   grae->SetPointError(1,0,0,0.01385062,0.01385062);
   grae->SetPoint(2,4.5,0.2908753);
   grae->SetPointError(2,0,0,0.02139913,0.02139913);
   grae->SetPoint(3,6.5,0.3913367);
   grae->SetPointError(3,0,0,0.02653087,0.02653087);
   grae->SetPoint(4,8.5,0.4775658);
   grae->SetPointError(4,0,0,0.0297791,0.0297791);
   grae->SetPoint(5,10.5,0.5515789);
   grae->SetPointError(5,0,0,0.0315745,0.0315745);
   grae->SetPoint(6,12.5,0.6151065);
   grae->SetPointError(6,0,0,0.03226351,0.03226351);
   grae->SetPoint(7,14.5,0.6696342);
   grae->SetPointError(7,0,0,0.03212359,0.03212359);
   grae->SetPoint(8,16.5,0.716437);
   grae->SetPointError(8,0,0,0.03137578,0.03137578);
   grae->SetPoint(9,18.5,0.7566093);
   grae->SetPointError(9,0,0,0.03019512,0.03019512);
   grae->SetPoint(10,20.5,0.7910903);
   grae->SetPointError(10,0,0,0.02871927,0.02871927);
   grae->SetPoint(11,22.5,0.8206865);
   grae->SetPointError(11,0,0,0.02705556,0.02705556);
   grae->SetPoint(12,24.5,0.8460898);
   grae->SetPointError(12,0,0,0.02528684,0.02528684);
   grae->SetPoint(13,26.5,0.8678942);
   grae->SetPointError(13,0,0,0.02347626,0.02347626);
   grae->SetPoint(14,28.5,0.8866096);
   grae->SetPointError(14,0,0,0.02167117,0.02167117);
   grae->SetPoint(15,30.5,0.9026735);
   grae->SetPointError(15,0,0,0.01990636,0.01990636);
   grae->SetPoint(16,32.5,0.9164617);
   grae->SetPointError(16,0,0,0.01820664,0.01820664);
   grae->SetPoint(17,34.5,0.9282966);
   grae->SetPointError(17,0,0,0.016589,0.016589);
   grae->SetPoint(18,36.5,0.9384547);
   grae->SetPointError(18,0,0,0.01506428,0.01506428);
   grae->SetPoint(19,38.5,0.9471738);
   grae->SetPointError(19,0,0,0.01363863,0.01363863);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Graph",100,0,42.3);
   Graph_Graph2->SetMinimum(0);
   Graph_Graph2->SetMaximum(1.053469);
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
   text = pt->AddText("#alpha = 0.0764 #pm 0.0026");
   pt->Draw();
   TLine *line = new TLine(9.074616,0,9.074616,1.016703);

   ci = TColor::GetColor("#ff9933");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
