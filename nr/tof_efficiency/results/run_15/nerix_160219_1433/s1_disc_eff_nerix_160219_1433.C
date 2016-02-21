{
//=========Macro generated from canvas: c1/c1
//=========  (Fri Feb 19 17:22:56 2016) by ROOT version5.34/26
   TCanvas *c1 = new TCanvas("c1", "c1",713,114,1000,700);
   c1->SetHighLightColor(2);
   c1->Range(-5.7,-0.1336136,44.7,1.202522);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetGridx();
   c1->SetGridy();
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(20);
   grae->SetName("eff_graph");
   grae->SetTitle("S1 Efficiency for TAC - nerix_160219_1433");
   grae->SetFillColor(19);
   grae->SetPoint(0,0.5,0.1005291);
   grae->SetPointError(0,1,1,0.0221513,0.02687393);
   grae->SetPoint(1,2.5,0.3070326);
   grae->SetPointError(1,1,1,0.01959974,0.02029071);
   grae->SetPoint(2,4.5,0.3821782);
   grae->SetPointError(2,1,1,0.022347,0.02283438);
   grae->SetPoint(3,6.5,0.5634921);
   grae->SetPointError(3,1,1,0.02697126,0.02661849);
   grae->SetPoint(4,8.5,0.5967213);
   grae->SetPointError(4,1,1,0.03001311,0.02934325);
   grae->SetPoint(5,10.5,0.7518797);
   grae->SetPointError(5,1,1,0.02932848,0.027305);
   grae->SetPoint(6,12.5,0.7054264);
   grae->SetPointError(6,1,1,0.03111129,0.02941424);
   grae->SetPoint(7,14.5,0.7361702);
   grae->SetPointError(7,1,1,0.03189395,0.02974101);
   grae->SetPoint(8,16.5,0.7150538);
   grae->SetPointError(8,1,1,0.03694103,0.03444935);
   grae->SetPoint(9,18.5,0.7625);
   grae->SetPointError(9,1,1,0.03845475,0.034883);
   grae->SetPoint(10,20.5,0.7898551);
   grae->SetPointError(10,1,1,0.04050232,0.03588422);
   grae->SetPoint(11,22.5,0.8442623);
   grae->SetPointError(11,1,1,0.0399938,0.03367932);
   grae->SetPoint(12,24.5,0.8878505);
   grae->SetPointError(12,1,1,0.03928228,0.03097414);
   grae->SetPoint(13,26.5,0.8761905);
   grae->SetPointError(13,1,1,0.04092011,0.03274484);
   grae->SetPoint(14,28.5,0.7464789);
   grae->SetPointError(14,1,1,0.06218799,0.05437135);
   grae->SetPoint(15,30.5,0.7666667);
   grae->SetPointError(15,1,1,0.06751857,0.05738726);
   grae->SetPoint(16,32.5,0.9538462);
   grae->SetPointError(16,1,1,0.04286342,0.02501417);
   grae->SetPoint(17,34.5,0.7608696);
   grae->SetPointError(17,1,1,0.07954691,0.06644054);
   grae->SetPoint(18,36.5,0.75);
   grae->SetPointError(18,1,1,0.07452238,0.0635115);
   grae->SetPoint(19,38.5,0.8181818);
   grae->SetPointError(19,1,1,0.07736695,0.06034916);
   
   TH1F *Graph_eff_graph1 = new TH1F("Graph_eff_graph1","S1 Efficiency for TAC - nerix_160219_1433",100,-4.5,43.5);
   Graph_eff_graph1->SetMinimum(0);
   Graph_eff_graph1->SetMaximum(1.068909);
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
   TText *text = pt->AddText("S1 Efficiency for TAC - nerix_160219_1433");
   pt->Draw();
   
   TF1 *fEfficiency = new TF1("fEfficiency","1.-exp(-[0]*x)",-0.5,39.5);
   fEfficiency->SetFillColor(19);
   fEfficiency->SetFillStyle(0);
   fEfficiency->SetLineColor(2);
   fEfficiency->SetLineWidth(2);
   fEfficiency->SetChisquare(70.17489);
   fEfficiency->SetNDF(19);
   fEfficiency->GetXaxis()->SetLabelFont(42);
   fEfficiency->GetXaxis()->SetLabelSize(0.035);
   fEfficiency->GetXaxis()->SetTitleSize(0.035);
   fEfficiency->GetXaxis()->SetTitleFont(42);
   fEfficiency->GetYaxis()->SetLabelFont(42);
   fEfficiency->GetYaxis()->SetLabelSize(0.035);
   fEfficiency->GetYaxis()->SetTitleSize(0.035);
   fEfficiency->GetYaxis()->SetTitleFont(42);
   fEfficiency->SetParameter(0,0.08755359);
   fEfficiency->SetParError(0,0.003350967);
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
   grae->SetPoint(0,0.5,0.04283242);
   grae->SetPointError(0,0,0,0.006450846,0.006450846);
   grae->SetPoint(1,2.5,0.1965851);
   grae->SetPointError(1,0,0,0.02707314,0.02707314);
   grae->SetPoint(2,4.5,0.32564);
   grae->SetPointError(2,0,0,0.04090374,0.04090374);
   grae->SetPoint(3,6.5,0.4339644);
   grae->SetPointError(3,0,0,0.04959248,0.04959248);
   grae->SetPoint(4,8.5,0.5248884);
   grae->SetPointError(4,0,0,0.05443438,0.05443438);
   grae->SetPoint(5,10.5,0.601207);
   grae->SetPointError(5,0,0,0.05644111,0.05644111);
   grae->SetPoint(6,12.5,0.6652663);
   grae->SetPointError(6,0,0,0.05639859,0.05639859);
   grae->SetPoint(7,14.5,0.7190355);
   grae->SetPointError(7,0,0,0.05491337,0.05491337);
   grae->SetPoint(8,16.5,0.7641677);
   grae->SetPointError(8,0,0,0.05245006,0.05245006);
   grae->SetPoint(9,18.5,0.8020501);
   grae->SetPointError(9,0,0,0.0493612,0.0493612);
   grae->SetPoint(10,20.5,0.8338474);
   grae->SetPointError(10,0,0,0.04591131,0.04591131);
   grae->SetPoint(11,22.5,0.860537);
   grae->SetPointError(11,0,0,0.0422961,0.0422961);
   grae->SetPoint(12,24.5,0.8829393);
   grae->SetPointError(12,0,0,0.03865768,0.03865768);
   grae->SetPoint(13,26.5,0.9017431);
   grae->SetPointError(13,0,0,0.0350968,0.0350968);
   grae->SetPoint(14,28.5,0.9175264);
   grae->SetPointError(14,0,0,0.03168242,0.03168242);
   grae->SetPoint(15,30.5,0.9307744);
   grae->SetPointError(15,0,0,0.02845937,0.02845937);
   grae->SetPoint(16,32.5,0.9418943);
   grae->SetPointError(16,0,0,0.02545428,0.02545428);
   grae->SetPoint(17,34.5,0.951228);
   grae->SetPointError(17,0,0,0.02268028,0.02268028);
   grae->SetPoint(18,36.5,0.9590624);
   grae->SetPointError(18,0,0,0.02014068,0.02014068);
   grae->SetPoint(19,38.5,0.9656383);
   grae->SetPointError(19,0,0,0.01783174,0.01783174);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Graph",100,0,42.3);
   Graph_Graph2->SetMinimum(0);
   Graph_Graph2->SetMaximum(1.078179);
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
   text = pt->AddText("#alpha = 0.0876 #pm 0.0034");
   pt->Draw();
   TLine *line = new TLine(7.916833,0,7.916833,1.068909);

   ci = TColor::GetColor("#ff9933");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
