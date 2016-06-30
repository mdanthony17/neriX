{
//=========Macro generated from canvas: ecddf8070f0e484eb6e3923c032d6dea/
//=========  (Wed May 25 11:46:27 2016) by ROOT version5.34/26
   TCanvas *ecddf8070f0e484eb6e3923c032d6dea = new TCanvas("ecddf8070f0e484eb6e3923c032d6dea", "",10,45,704,527);
   ecddf8070f0e484eb6e3923c032d6dea->SetHighLightColor(2);
   ecddf8070f0e484eb6e3923c032d6dea->Range(1.416657e+09,13.85,1.42346e+09,18.83);
   ecddf8070f0e484eb6e3923c032d6dea->SetFillColor(0);
   ecddf8070f0e484eb6e3923c032d6dea->SetBorderMode(0);
   ecddf8070f0e484eb6e3923c032d6dea->SetBorderSize(2);
   ecddf8070f0e484eb6e3923c032d6dea->SetFrameBorderMode(0);
   ecddf8070f0e484eb6e3923c032d6dea->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(10);
   gre->SetName("Graph0");
   gre->SetTitle("Gas Gain Method Comparison - 0.0 mm < R < 18.0 mm");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.417637e+09,16.35);
   gre->SetPointError(0,0,0.37);
   gre->SetPoint(1,1.418056e+09,15.91);
   gre->SetPointError(1,0,0.64);
   gre->SetPoint(2,1.418225e+09,15.93);
   gre->SetPointError(2,0,0.51);
   gre->SetPoint(3,1.418831e+09,15.18);
   gre->SetPointError(3,0,0.5);
   gre->SetPoint(4,1.42056e+09,17.35);
   gre->SetPointError(4,0,0.61);
   gre->SetPoint(5,1.421262e+09,17.08);
   gre->SetPointError(5,0,0.6);
   gre->SetPoint(6,1.422464e+09,17.3);
   gre->SetPointError(6,0,0.44);
   gre->SetPoint(7,1.42247e+09,17.05);
   gre->SetPointError(7,0,0.46);
   gre->SetPoint(8,1.422475e+09,17.55);
   gre->SetPointError(8,0,0.45);
   gre->SetPoint(9,1.42248e+09,17.2);
   gre->SetPointError(9,0,0.47);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","Gas Gain Method Comparison - 0.0 mm < R < 18.0 mm",100,1.417337e+09,1.42278e+09);
   Graph_Graph1->SetMinimum(14.348);
   Graph_Graph1->SetMaximum(18.332);
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
   grae->SetName("conf_gate_pi_(0.0, 18.0)");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#0000ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3004);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.417337e+09,16.71157);
   grae->SetPointError(0,0,0,0.1535515,0.1535515);
   grae->SetPoint(1,1.42278e+09,16.71157);
   grae->SetPointError(1,0,0,0.1535515,0.1535515);
   
   TH1F *Graph_conf_gate_pi_(0.0, 18.0)1 = new TH1F("Graph_conf_gate_pi_(0.0, 18.0)1","Graph",100,1.416793e+09,1.423324e+09);
   Graph_conf_gate_pi_(0.0, 18.0)1->SetMinimum(16.52731);
   Graph_conf_gate_pi_(0.0, 18.0)1->SetMaximum(16.89584);
   Graph_conf_gate_pi_(0.0, 18.0)1->SetDirectory(0);
   Graph_conf_gate_pi_(0.0, 18.0)1->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_conf_gate_pi_(0.0, 18.0)1->SetLineColor(ci);
   Graph_conf_gate_pi_(0.0, 18.0)1->GetXaxis()->SetLabelFont(42);
   Graph_conf_gate_pi_(0.0, 18.0)1->GetXaxis()->SetLabelSize(0.035);
   Graph_conf_gate_pi_(0.0, 18.0)1->GetXaxis()->SetTitleSize(0.035);
   Graph_conf_gate_pi_(0.0, 18.0)1->GetXaxis()->SetTitleFont(42);
   Graph_conf_gate_pi_(0.0, 18.0)1->GetYaxis()->SetLabelFont(42);
   Graph_conf_gate_pi_(0.0, 18.0)1->GetYaxis()->SetLabelSize(0.035);
   Graph_conf_gate_pi_(0.0, 18.0)1->GetYaxis()->SetTitleSize(0.035);
   Graph_conf_gate_pi_(0.0, 18.0)1->GetYaxis()->SetTitleFont(42);
   Graph_conf_gate_pi_(0.0, 18.0)1->GetZaxis()->SetLabelFont(42);
   Graph_conf_gate_pi_(0.0, 18.0)1->GetZaxis()->SetLabelSize(0.035);
   Graph_conf_gate_pi_(0.0, 18.0)1->GetZaxis()->SetTitleSize(0.035);
   Graph_conf_gate_pi_(0.0, 18.0)1->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_conf_gate_pi_(0.0, 18.0)1);
   
   grae->Draw("3 ");
   
   gre = new TGraphErrors(10);
   gre->SetName("Graph1");
   gre->SetTitle("Gas Gain Method Comparison - 0.0 mm < R < 18.0 mm");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#00ff00");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#00ff00");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.417637e+09,15.89);
   gre->SetPointError(0,0,0.34);
   gre->SetPoint(1,1.418056e+09,16.24);
   gre->SetPointError(1,0,0.59);
   gre->SetPoint(2,1.418225e+09,16.22);
   gre->SetPointError(2,0,0.46);
   gre->SetPoint(3,1.418831e+09,14.94);
   gre->SetPointError(3,0,0.46);
   gre->SetPoint(4,1.42056e+09,16.5);
   gre->SetPointError(4,0,0.57);
   gre->SetPoint(5,1.421262e+09,16.4);
   gre->SetPointError(5,0,0.55);
   gre->SetPoint(6,1.422464e+09,16.78);
   gre->SetPointError(6,0,0.41);
   gre->SetPoint(7,1.42247e+09,16.7);
   gre->SetPointError(7,0,0.41);
   gre->SetPoint(8,1.422475e+09,16.33);
   gre->SetPointError(8,0,0.41);
   gre->SetPoint(9,1.42248e+09,16.2);
   gre->SetPointError(9,0,0.4);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Gas Gain Method Comparison - 0.0 mm < R < 18.0 mm",100,1.417337e+09,1.42278e+09);
   Graph_Graph2->SetMinimum(14.209);
   Graph_Graph2->SetMaximum(17.461);
   Graph_Graph2->SetDirectory(0);
   Graph_Graph2->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph2->SetLineColor(ci);
   Graph_Graph2->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph2->GetXaxis()->SetTimeFormat("%m-%d%F1995-01-01 00:00:00s0");
   Graph_Graph2->GetXaxis()->SetLabelFont(42);
   Graph_Graph2->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph2->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph2->GetXaxis()->SetTitleFont(42);
   Graph_Graph2->GetYaxis()->SetTitle("Gas gain [PE per extracted electron]");
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
   grae->SetName("conf_misc_e_(0.0, 18.0)");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#00ff00");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3006);

   ci = TColor::GetColor("#00ff00");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.417337e+09,16.20691);
   grae->SetPointError(0,0,0,0.1394072,0.1394072);
   grae->SetPoint(1,1.42278e+09,16.20691);
   grae->SetPointError(1,0,0,0.1394072,0.1394072);
   
   TH1F *Graph_conf_misc_e_(0.0, 18.0)2 = new TH1F("Graph_conf_misc_e_(0.0, 18.0)2","Graph",100,1.416793e+09,1.423324e+09);
   Graph_conf_misc_e_(0.0, 18.0)2->SetMinimum(16.03963);
   Graph_conf_misc_e_(0.0, 18.0)2->SetMaximum(16.3742);
   Graph_conf_misc_e_(0.0, 18.0)2->SetDirectory(0);
   Graph_conf_misc_e_(0.0, 18.0)2->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_conf_misc_e_(0.0, 18.0)2->SetLineColor(ci);
   Graph_conf_misc_e_(0.0, 18.0)2->GetXaxis()->SetLabelFont(42);
   Graph_conf_misc_e_(0.0, 18.0)2->GetXaxis()->SetLabelSize(0.035);
   Graph_conf_misc_e_(0.0, 18.0)2->GetXaxis()->SetTitleSize(0.035);
   Graph_conf_misc_e_(0.0, 18.0)2->GetXaxis()->SetTitleFont(42);
   Graph_conf_misc_e_(0.0, 18.0)2->GetYaxis()->SetLabelFont(42);
   Graph_conf_misc_e_(0.0, 18.0)2->GetYaxis()->SetLabelSize(0.035);
   Graph_conf_misc_e_(0.0, 18.0)2->GetYaxis()->SetTitleSize(0.035);
   Graph_conf_misc_e_(0.0, 18.0)2->GetYaxis()->SetTitleFont(42);
   Graph_conf_misc_e_(0.0, 18.0)2->GetZaxis()->SetLabelFont(42);
   Graph_conf_misc_e_(0.0, 18.0)2->GetZaxis()->SetLabelSize(0.035);
   Graph_conf_misc_e_(0.0, 18.0)2->GetZaxis()->SetTitleSize(0.035);
   Graph_conf_misc_e_(0.0, 18.0)2->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_conf_misc_e_(0.0, 18.0)2);
   
   grae->Draw("3 ");
   
   TPaveText *pt = new TPaveText(0.15,0.8,0.5,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("gate_pi: 16.71 +/- 0.15 PE");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.7,0.5,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#00ff00");
   pt->SetTextColor(ci);
   text = pt->AddText("misc_e: 16.21 +/- 0.14 PE");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.9374,0.85,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("Gas Gain Method Comparison - 0.0 mm < R < 18.0 mm");
   pt->Draw();
   ecddf8070f0e484eb6e3923c032d6dea->Modified();
   ecddf8070f0e484eb6e3923c032d6dea->cd();
   ecddf8070f0e484eb6e3923c032d6dea->SetSelected(ecddf8070f0e484eb6e3923c032d6dea);
}
