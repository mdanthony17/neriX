{
//=========Macro generated from canvas: 05222b00488e4e58b2cb750f332b958c/
//=========  (Thu May 26 12:46:15 2016) by ROOT version5.34/26
   TCanvas *05222b00488e4e58b2cb750f332b958c = new TCanvas("05222b00488e4e58b2cb750f332b958c", "",10,45,704,527);
   05222b00488e4e58b2cb750f332b958c->SetHighLightColor(2);
   05222b00488e4e58b2cb750f332b958c->Range(1.40989e+09,13.6875,1.415959e+09,21.8125);
   05222b00488e4e58b2cb750f332b958c->SetFillColor(0);
   05222b00488e4e58b2cb750f332b958c->SetBorderMode(0);
   05222b00488e4e58b2cb750f332b958c->SetBorderSize(2);
   05222b00488e4e58b2cb750f332b958c->SetFrameBorderMode(0);
   05222b00488e4e58b2cb750f332b958c->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(3);
   gre->SetName("Graph0");
   gre->SetTitle("Gas Gain Run Average, Method Average - 0.0 mm < R < 18.0 mm");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#ff00ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff00ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.412084e+09,16.58);
   gre->SetPointError(0,0,0.45);
   gre->SetPoint(1,1.413992e+09,16.02);
   gre->SetPointError(1,0,0.26);
   gre->SetPoint(2,1.415052e+09,15.93);
   gre->SetPointError(2,0,0.28);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","Gas Gain Run Average, Method Average - 0.0 mm < R < 18.0 mm",100,1.410497e+09,1.415352e+09);
   Graph_Graph1->SetMinimum(14.5);
   Graph_Graph1->SetMaximum(21);
   Graph_Graph1->SetDirectory(0);
   Graph_Graph1->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph1->SetLineColor(ci);
   Graph_Graph1->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph1->GetXaxis()->SetTimeFormat("%m-%d%F1995-01-01 00:00:00s0");
   Graph_Graph1->GetXaxis()->SetLabelFont(42);
   Graph_Graph1->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleFont(42);
   Graph_Graph1->GetYaxis()->SetTitle("Gas gain [PE per extracted electron]");
   Graph_Graph1->GetYaxis()->SetLabelFont(42);
   Graph_Graph1->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleFont(42);
   Graph_Graph1->GetZaxis()->SetLabelFont(42);
   Graph_Graph1->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1);
   
   gre->Draw("ap");
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(2);
   grae->SetName("conf_0.345_(0.0, 18.0)");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#ff00ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3004);

   ci = TColor::GetColor("#ff00ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.410497e+09,16.06979);
   grae->SetPointError(0,0,0,0.1754487,0.1754487);
   grae->SetPoint(1,1.415352e+09,16.06979);
   grae->SetPointError(1,0,0,0.1754487,0.1754487);
   
   TH1F *Graph_conf_0.345_(0.0, 18.0)1 = new TH1F("Graph_conf_0.345_(0.0, 18.0)1","Graph",100,1.410011e+09,1.415837e+09);
   Graph_conf_0.345_(0.0, 18.0)1->SetMinimum(15.85925);
   Graph_conf_0.345_(0.0, 18.0)1->SetMaximum(16.28033);
   Graph_conf_0.345_(0.0, 18.0)1->SetDirectory(0);
   Graph_conf_0.345_(0.0, 18.0)1->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_conf_0.345_(0.0, 18.0)1->SetLineColor(ci);
   Graph_conf_0.345_(0.0, 18.0)1->GetXaxis()->SetLabelFont(42);
   Graph_conf_0.345_(0.0, 18.0)1->GetXaxis()->SetLabelSize(0.035);
   Graph_conf_0.345_(0.0, 18.0)1->GetXaxis()->SetTitleSize(0.035);
   Graph_conf_0.345_(0.0, 18.0)1->GetXaxis()->SetTitleFont(42);
   Graph_conf_0.345_(0.0, 18.0)1->GetYaxis()->SetLabelFont(42);
   Graph_conf_0.345_(0.0, 18.0)1->GetYaxis()->SetLabelSize(0.035);
   Graph_conf_0.345_(0.0, 18.0)1->GetYaxis()->SetTitleSize(0.035);
   Graph_conf_0.345_(0.0, 18.0)1->GetYaxis()->SetTitleFont(42);
   Graph_conf_0.345_(0.0, 18.0)1->GetZaxis()->SetLabelFont(42);
   Graph_conf_0.345_(0.0, 18.0)1->GetZaxis()->SetLabelSize(0.035);
   Graph_conf_0.345_(0.0, 18.0)1->GetZaxis()->SetTitleSize(0.035);
   Graph_conf_0.345_(0.0, 18.0)1->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_conf_0.345_(0.0, 18.0)1);
   
   grae->Draw("3 ");
   
   TPaveText *pt = new TPaveText(0.15,0.9374,0.85,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Gas Gain Run Average, Method Average - 0.0 mm < R < 18.0 mm");
   pt->Draw();
   
   gre = new TGraphErrors(4);
   gre->SetName("Graph1");
   gre->SetTitle("Gas Gain Run Average, Method Average - 0.0 mm < R < 18.0 mm");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.412091e+09,17.95);
   gre->SetPointError(0,0,0.24);
   gre->SetPoint(1,1.412603e+09,17.13);
   gre->SetPointError(1,0,0.39);
   gre->SetPoint(2,1.41331e+09,16.43);
   gre->SetPointError(2,0,0.27);
   gre->SetPoint(3,1.41505e+09,16.03);
   gre->SetPointError(3,0,0.26);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Gas Gain Run Average, Method Average - 0.0 mm < R < 18.0 mm",100,1.410497e+09,1.415352e+09);
   Graph_Graph2->SetMinimum(14.5);
   Graph_Graph2->SetMaximum(21);
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
   gre->SetHistogram(Graph_Graph2);
   
   gre->Draw("p ");
   
   grae = new TGraphAsymmErrors(2);
   grae->SetName("conf_1.054_(0.0, 18.0)");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#0000ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3005);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.410497e+09,16.90809);
   grae->SetPointError(0,0,0,0.1380841,0.1380841);
   grae->SetPoint(1,1.415352e+09,16.90809);
   grae->SetPointError(1,0,0,0.1380841,0.1380841);
   
   TH1F *Graph_conf_1.054_(0.0, 18.0)2 = new TH1F("Graph_conf_1.054_(0.0, 18.0)2","Graph",100,1.410011e+09,1.415837e+09);
   Graph_conf_1.054_(0.0, 18.0)2->SetMinimum(16.74239);
   Graph_conf_1.054_(0.0, 18.0)2->SetMaximum(17.07379);
   Graph_conf_1.054_(0.0, 18.0)2->SetDirectory(0);
   Graph_conf_1.054_(0.0, 18.0)2->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_conf_1.054_(0.0, 18.0)2->SetLineColor(ci);
   Graph_conf_1.054_(0.0, 18.0)2->GetXaxis()->SetLabelFont(42);
   Graph_conf_1.054_(0.0, 18.0)2->GetXaxis()->SetLabelSize(0.035);
   Graph_conf_1.054_(0.0, 18.0)2->GetXaxis()->SetTitleSize(0.035);
   Graph_conf_1.054_(0.0, 18.0)2->GetXaxis()->SetTitleFont(42);
   Graph_conf_1.054_(0.0, 18.0)2->GetYaxis()->SetLabelFont(42);
   Graph_conf_1.054_(0.0, 18.0)2->GetYaxis()->SetLabelSize(0.035);
   Graph_conf_1.054_(0.0, 18.0)2->GetYaxis()->SetTitleSize(0.035);
   Graph_conf_1.054_(0.0, 18.0)2->GetYaxis()->SetTitleFont(42);
   Graph_conf_1.054_(0.0, 18.0)2->GetZaxis()->SetLabelFont(42);
   Graph_conf_1.054_(0.0, 18.0)2->GetZaxis()->SetLabelSize(0.035);
   Graph_conf_1.054_(0.0, 18.0)2->GetZaxis()->SetTitleSize(0.035);
   Graph_conf_1.054_(0.0, 18.0)2->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_conf_1.054_(0.0, 18.0)2);
   
   grae->Draw("3 ");
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph2");
   gre->SetTitle("Gas Gain Run Average, Method Average - 0.0 mm < R < 18.0 mm");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#00ff00");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#00ff00");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.410797e+09,17.94);
   gre->SetPointError(0,0,0.42);
   gre->SetPoint(1,1.410963e+09,17.13);
   gre->SetPointError(1,0,0.42);
   gre->SetPoint(2,1.412109e+09,17.34);
   gre->SetPointError(2,0,0.3);
   gre->SetPoint(3,1.412354e+09,17.22);
   gre->SetPointError(3,0,0.37);
   gre->SetPoint(4,1.413297e+09,16.74);
   gre->SetPointError(4,0,0.27);
   gre->SetPoint(5,1.413388e+09,16.99);
   gre->SetPointError(5,0,0.29);
   gre->SetPoint(6,1.415048e+09,15.45);
   gre->SetPointError(6,0,0.29);
   
   TH1F *Graph_Graph3 = new TH1F("Graph_Graph3","Gas Gain Run Average, Method Average - 0.0 mm < R < 18.0 mm",100,1.410497e+09,1.415352e+09);
   Graph_Graph3->SetMinimum(14.5);
   Graph_Graph3->SetMaximum(21);
   Graph_Graph3->SetDirectory(0);
   Graph_Graph3->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph3->SetLineColor(ci);
   Graph_Graph3->GetXaxis()->SetLabelFont(42);
   Graph_Graph3->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph3->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph3->GetXaxis()->SetTitleFont(42);
   Graph_Graph3->GetYaxis()->SetLabelFont(42);
   Graph_Graph3->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph3->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph3->GetYaxis()->SetTitleFont(42);
   Graph_Graph3->GetZaxis()->SetLabelFont(42);
   Graph_Graph3->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph3->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph3->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph3);
   
   gre->Draw("p ");
   
   grae = new TGraphAsymmErrors(2);
   grae->SetName("conf_2.356_(0.0, 18.0)");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#00ff00");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3006);

   ci = TColor::GetColor("#00ff00");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.410497e+09,16.84141);
   grae->SetPointError(0,0,0,0.1219401,0.1219401);
   grae->SetPoint(1,1.415352e+09,16.84141);
   grae->SetPointError(1,0,0,0.1219401,0.1219401);
   
   TH1F *Graph_conf_2.356_(0.0, 18.0)3 = new TH1F("Graph_conf_2.356_(0.0, 18.0)3","Graph",100,1.410011e+09,1.415837e+09);
   Graph_conf_2.356_(0.0, 18.0)3->SetMinimum(16.69508);
   Graph_conf_2.356_(0.0, 18.0)3->SetMaximum(16.98774);
   Graph_conf_2.356_(0.0, 18.0)3->SetDirectory(0);
   Graph_conf_2.356_(0.0, 18.0)3->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_conf_2.356_(0.0, 18.0)3->SetLineColor(ci);
   Graph_conf_2.356_(0.0, 18.0)3->GetXaxis()->SetLabelFont(42);
   Graph_conf_2.356_(0.0, 18.0)3->GetXaxis()->SetLabelSize(0.035);
   Graph_conf_2.356_(0.0, 18.0)3->GetXaxis()->SetTitleSize(0.035);
   Graph_conf_2.356_(0.0, 18.0)3->GetXaxis()->SetTitleFont(42);
   Graph_conf_2.356_(0.0, 18.0)3->GetYaxis()->SetLabelFont(42);
   Graph_conf_2.356_(0.0, 18.0)3->GetYaxis()->SetLabelSize(0.035);
   Graph_conf_2.356_(0.0, 18.0)3->GetYaxis()->SetTitleSize(0.035);
   Graph_conf_2.356_(0.0, 18.0)3->GetYaxis()->SetTitleFont(42);
   Graph_conf_2.356_(0.0, 18.0)3->GetZaxis()->SetLabelFont(42);
   Graph_conf_2.356_(0.0, 18.0)3->GetZaxis()->SetLabelSize(0.035);
   Graph_conf_2.356_(0.0, 18.0)3->GetZaxis()->SetTitleSize(0.035);
   Graph_conf_2.356_(0.0, 18.0)3->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_conf_2.356_(0.0, 18.0)3);
   
   grae->Draw("3 ");
   
   gre = new TGraphErrors(3);
   gre->SetName("Graph3");
   gre->SetTitle("Gas Gain Run Average, Method Average - 0.0 mm < R < 18.0 mm");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff0000");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff0000");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.412087e+09,17.82);
   gre->SetPointError(0,0,0.35);
   gre->SetPoint(1,1.414595e+09,16.45);
   gre->SetPointError(1,0,0.34);
   gre->SetPoint(2,1.41503e+09,15.71);
   gre->SetPointError(2,0,0.32);
   
   TH1F *Graph_Graph4 = new TH1F("Graph_Graph4","Gas Gain Run Average, Method Average - 0.0 mm < R < 18.0 mm",100,1.410497e+09,1.415352e+09);
   Graph_Graph4->SetMinimum(14.5);
   Graph_Graph4->SetMaximum(21);
   Graph_Graph4->SetDirectory(0);
   Graph_Graph4->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph4->SetLineColor(ci);
   Graph_Graph4->GetXaxis()->SetLabelFont(42);
   Graph_Graph4->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph4->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph4->GetXaxis()->SetTitleFont(42);
   Graph_Graph4->GetYaxis()->SetLabelFont(42);
   Graph_Graph4->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph4->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph4->GetYaxis()->SetTitleFont(42);
   Graph_Graph4->GetZaxis()->SetLabelFont(42);
   Graph_Graph4->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph4->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph4->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph4);
   
   gre->Draw("p ");
   
   grae = new TGraphAsymmErrors(2);
   grae->SetName("conf_5.5_(0.0, 18.0)");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#ff0000");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3007);

   ci = TColor::GetColor("#ff0000");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.410497e+09,16.59888);
   grae->SetPointError(0,0,0,0.1939668,0.1939668);
   grae->SetPoint(1,1.415352e+09,16.59888);
   grae->SetPointError(1,0,0,0.1939668,0.1939668);
   
   TH1F *Graph_conf_5.5_(0.0, 18.0)4 = new TH1F("Graph_conf_5.5_(0.0, 18.0)4","Graph",100,1.410011e+09,1.415837e+09);
   Graph_conf_5.5_(0.0, 18.0)4->SetMinimum(16.36612);
   Graph_conf_5.5_(0.0, 18.0)4->SetMaximum(16.83164);
   Graph_conf_5.5_(0.0, 18.0)4->SetDirectory(0);
   Graph_conf_5.5_(0.0, 18.0)4->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_conf_5.5_(0.0, 18.0)4->SetLineColor(ci);
   Graph_conf_5.5_(0.0, 18.0)4->GetXaxis()->SetLabelFont(42);
   Graph_conf_5.5_(0.0, 18.0)4->GetXaxis()->SetLabelSize(0.035);
   Graph_conf_5.5_(0.0, 18.0)4->GetXaxis()->SetTitleSize(0.035);
   Graph_conf_5.5_(0.0, 18.0)4->GetXaxis()->SetTitleFont(42);
   Graph_conf_5.5_(0.0, 18.0)4->GetYaxis()->SetLabelFont(42);
   Graph_conf_5.5_(0.0, 18.0)4->GetYaxis()->SetLabelSize(0.035);
   Graph_conf_5.5_(0.0, 18.0)4->GetYaxis()->SetTitleSize(0.035);
   Graph_conf_5.5_(0.0, 18.0)4->GetYaxis()->SetTitleFont(42);
   Graph_conf_5.5_(0.0, 18.0)4->GetZaxis()->SetLabelFont(42);
   Graph_conf_5.5_(0.0, 18.0)4->GetZaxis()->SetLabelSize(0.035);
   Graph_conf_5.5_(0.0, 18.0)4->GetZaxis()->SetTitleSize(0.035);
   Graph_conf_5.5_(0.0, 18.0)4->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_conf_5.5_(0.0, 18.0)4);
   
   grae->Draw("3 ");
   
   pt = new TPaveText(0.15,0.8,0.5,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 0.345kV: 16.07 +/- 0.18 PE");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.75,0.5,0.8,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.054kV: 16.91 +/- 0.14 PE");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.7,0.5,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#00ff00");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 2.356kV: 16.84 +/- 0.12 PE");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.65,0.5,0.7,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 5.5kV: 16.60 +/- 0.19 PE");
   pt->Draw();
   05222b00488e4e58b2cb750f332b958c->Modified();
   05222b00488e4e58b2cb750f332b958c->cd();
   05222b00488e4e58b2cb750f332b958c->SetSelected(05222b00488e4e58b2cb750f332b958c);
}
