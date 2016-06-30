{
//=========Macro generated from canvas: 641f7fc554e44d26b9912578b73fb047/
//=========  (Wed May 18 14:26:21 2016) by ROOT version5.34/26
   TCanvas *641f7fc554e44d26b9912578b73fb047 = new TCanvas("641f7fc554e44d26b9912578b73fb047", "",10,45,704,527);
   641f7fc554e44d26b9912578b73fb047->SetHighLightColor(2);
   641f7fc554e44d26b9912578b73fb047->Range(1.40989e+09,12.125,1.415959e+09,20.875);
   641f7fc554e44d26b9912578b73fb047->SetFillColor(0);
   641f7fc554e44d26b9912578b73fb047->SetBorderMode(0);
   641f7fc554e44d26b9912578b73fb047->SetBorderSize(2);
   641f7fc554e44d26b9912578b73fb047->SetFrameBorderMode(0);
   641f7fc554e44d26b9912578b73fb047->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(19);
   gre->SetName("Graph0");
   gre->SetTitle("Gas Gain Method Comparison - 0.0 mm < R < 13.0 mm");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.410797e+09,18.09);
   gre->SetPointError(0,0,0.4);
   gre->SetPoint(1,1.410963e+09,17.21);
   gre->SetPointError(1,0,0.48);
   gre->SetPoint(2,1.412004e+09,17.75);
   gre->SetPointError(2,0,0.29);
   gre->SetPoint(3,1.412007e+09,17.39);
   gre->SetPointError(3,0,0.21);
   gre->SetPoint(4,1.412084e+09,17.01);
   gre->SetPointError(4,0,0.34);
   gre->SetPoint(5,1.412087e+09,17.86);
   gre->SetPointError(5,0,0.37);
   gre->SetPoint(6,1.412091e+09,17.57);
   gre->SetPointError(6,0,0.34);
   gre->SetPoint(7,1.412109e+09,16.65);
   gre->SetPointError(7,0,0.44);
   gre->SetPoint(8,1.412354e+09,17.26);
   gre->SetPointError(8,0,0.38);
   gre->SetPoint(9,1.412603e+09,17.8);
   gre->SetPointError(9,0,0.37);
   gre->SetPoint(10,1.413297e+09,0);
   gre->SetPointError(10,0,0);
   gre->SetPoint(11,1.41331e+09,16.39);
   gre->SetPointError(11,0,0.35);
   gre->SetPoint(12,1.413388e+09,17.26);
   gre->SetPointError(12,0,0.28);
   gre->SetPoint(13,1.413992e+09,0);
   gre->SetPointError(13,0,0);
   gre->SetPoint(14,1.414595e+09,16.27);
   gre->SetPointError(14,0,0.43);
   gre->SetPoint(15,1.41503e+09,15.49);
   gre->SetPointError(15,0,0.49);
   gre->SetPoint(16,1.415048e+09,0);
   gre->SetPointError(16,0,0);
   gre->SetPoint(17,1.41505e+09,15.76);
   gre->SetPointError(17,0,0.32);
   gre->SetPoint(18,1.415052e+09,15.95);
   gre->SetPointError(18,0,0.3);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","Gas Gain Method Comparison - 0.0 mm < R < 13.0 mm",100,1.410497e+09,1.415352e+09);
   Graph_Graph1->SetMinimum(13);
   Graph_Graph1->SetMaximum(20);
   Graph_Graph1->SetDirectory(0);
   Graph_Graph1->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph1->SetLineColor(ci);
   Graph_Graph1->GetXaxis()->SetLabelFont(42);
   Graph_Graph1->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleFont(42);
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
   grae->SetName("conf_gate_pi_(0.0, 13.0)");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#0000ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3004);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.410497e+09,17.04942);
   grae->SetPointError(0,0,0,0.08435719,0.08435719);
   grae->SetPoint(1,1.415352e+09,17.04942);
   grae->SetPointError(1,0,0,0.08435719,0.08435719);
   
   TH1F *Graph_conf_gate_pi_(0.0, 13.0)1 = new TH1F("Graph_conf_gate_pi_(0.0, 13.0)1","Graph",100,1.410011e+09,1.415837e+09);
   Graph_conf_gate_pi_(0.0, 13.0)1->SetMinimum(16.94819);
   Graph_conf_gate_pi_(0.0, 13.0)1->SetMaximum(17.15065);
   Graph_conf_gate_pi_(0.0, 13.0)1->SetDirectory(0);
   Graph_conf_gate_pi_(0.0, 13.0)1->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_conf_gate_pi_(0.0, 13.0)1->SetLineColor(ci);
   Graph_conf_gate_pi_(0.0, 13.0)1->GetXaxis()->SetLabelFont(42);
   Graph_conf_gate_pi_(0.0, 13.0)1->GetXaxis()->SetLabelSize(0.035);
   Graph_conf_gate_pi_(0.0, 13.0)1->GetXaxis()->SetTitleSize(0.035);
   Graph_conf_gate_pi_(0.0, 13.0)1->GetXaxis()->SetTitleFont(42);
   Graph_conf_gate_pi_(0.0, 13.0)1->GetYaxis()->SetLabelFont(42);
   Graph_conf_gate_pi_(0.0, 13.0)1->GetYaxis()->SetLabelSize(0.035);
   Graph_conf_gate_pi_(0.0, 13.0)1->GetYaxis()->SetTitleSize(0.035);
   Graph_conf_gate_pi_(0.0, 13.0)1->GetYaxis()->SetTitleFont(42);
   Graph_conf_gate_pi_(0.0, 13.0)1->GetZaxis()->SetLabelFont(42);
   Graph_conf_gate_pi_(0.0, 13.0)1->GetZaxis()->SetLabelSize(0.035);
   Graph_conf_gate_pi_(0.0, 13.0)1->GetZaxis()->SetTitleSize(0.035);
   Graph_conf_gate_pi_(0.0, 13.0)1->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_conf_gate_pi_(0.0, 13.0)1);
   
   grae->Draw("3 ");
   
   gre = new TGraphErrors(19);
   gre->SetName("Graph1");
   gre->SetTitle("Gas Gain Method Comparison - 0.0 mm < R < 13.0 mm");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#00ff00");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#00ff00");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.410797e+09,16.33);
   gre->SetPointError(0,0,0.47);
   gre->SetPoint(1,1.410963e+09,0);
   gre->SetPointError(1,0,0);
   gre->SetPoint(2,1.412004e+09,16.44);
   gre->SetPointError(2,0,0.58);
   gre->SetPoint(3,1.412007e+09,15.14);
   gre->SetPointError(3,0,0.63);
   gre->SetPoint(4,1.412084e+09,17.06);
   gre->SetPointError(4,0,0.45);
   gre->SetPoint(5,1.412087e+09,17.15);
   gre->SetPointError(5,0,0.36);
   gre->SetPoint(6,1.412091e+09,16.68);
   gre->SetPointError(6,0,0.34);
   gre->SetPoint(7,1.412109e+09,16.49);
   gre->SetPointError(7,0,0.34);
   gre->SetPoint(8,1.412354e+09,17.24);
   gre->SetPointError(8,0,0.37);
   gre->SetPoint(9,1.412603e+09,16.37);
   gre->SetPointError(9,0,0.4);
   gre->SetPoint(10,1.413297e+09,0);
   gre->SetPointError(10,0,0);
   gre->SetPoint(11,1.41331e+09,15.76);
   gre->SetPointError(11,0,0.34);
   gre->SetPoint(12,1.413388e+09,0);
   gre->SetPointError(12,0,0);
   gre->SetPoint(13,1.413992e+09,15.75);
   gre->SetPointError(13,0,0.31);
   gre->SetPoint(14,1.414595e+09,16.34);
   gre->SetPointError(14,0,0.3);
   gre->SetPoint(15,1.41503e+09,16.11);
   gre->SetPointError(15,0,0.28);
   gre->SetPoint(16,1.415048e+09,14.82);
   gre->SetPointError(16,0,0.42);
   gre->SetPoint(17,1.41505e+09,14.77);
   gre->SetPointError(17,0,0.39);
   gre->SetPoint(18,1.415052e+09,14.6);
   gre->SetPointError(18,0,0.3);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Gas Gain Method Comparison - 0.0 mm < R < 13.0 mm",100,1.410497e+09,1.415352e+09);
   Graph_Graph2->SetMinimum(13);
   Graph_Graph2->SetMaximum(20);
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
   grae->SetName("conf_misc_e_(0.0, 13.0)");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#00ff00");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3006);

   ci = TColor::GetColor("#00ff00");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.410497e+09,16.05347);
   grae->SetPointError(0,0,0,0.09136086,0.09136086);
   grae->SetPoint(1,1.415352e+09,16.05347);
   grae->SetPointError(1,0,0,0.09136086,0.09136086);
   
   TH1F *Graph_conf_misc_e_(0.0, 13.0)2 = new TH1F("Graph_conf_misc_e_(0.0, 13.0)2","Graph",100,1.410011e+09,1.415837e+09);
   Graph_conf_misc_e_(0.0, 13.0)2->SetMinimum(15.94384);
   Graph_conf_misc_e_(0.0, 13.0)2->SetMaximum(16.1631);
   Graph_conf_misc_e_(0.0, 13.0)2->SetDirectory(0);
   Graph_conf_misc_e_(0.0, 13.0)2->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_conf_misc_e_(0.0, 13.0)2->SetLineColor(ci);
   Graph_conf_misc_e_(0.0, 13.0)2->GetXaxis()->SetLabelFont(42);
   Graph_conf_misc_e_(0.0, 13.0)2->GetXaxis()->SetLabelSize(0.035);
   Graph_conf_misc_e_(0.0, 13.0)2->GetXaxis()->SetTitleSize(0.035);
   Graph_conf_misc_e_(0.0, 13.0)2->GetXaxis()->SetTitleFont(42);
   Graph_conf_misc_e_(0.0, 13.0)2->GetYaxis()->SetLabelFont(42);
   Graph_conf_misc_e_(0.0, 13.0)2->GetYaxis()->SetLabelSize(0.035);
   Graph_conf_misc_e_(0.0, 13.0)2->GetYaxis()->SetTitleSize(0.035);
   Graph_conf_misc_e_(0.0, 13.0)2->GetYaxis()->SetTitleFont(42);
   Graph_conf_misc_e_(0.0, 13.0)2->GetZaxis()->SetLabelFont(42);
   Graph_conf_misc_e_(0.0, 13.0)2->GetZaxis()->SetLabelSize(0.035);
   Graph_conf_misc_e_(0.0, 13.0)2->GetZaxis()->SetTitleSize(0.035);
   Graph_conf_misc_e_(0.0, 13.0)2->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_conf_misc_e_(0.0, 13.0)2);
   
   grae->Draw("3 ");
   
   TPaveText *pt = new TPaveText(0.15,0.8,0.5,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("gate_pi: 17.05 +/- 0.08");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.7,0.5,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#00ff00");
   pt->SetTextColor(ci);
   text = pt->AddText("misc_e: 16.05 +/- 0.09");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.9374,0.85,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("Gas Gain Method Comparison - 0.0 mm < R < 13.0 mm");
   pt->Draw();
   641f7fc554e44d26b9912578b73fb047->Modified();
   641f7fc554e44d26b9912578b73fb047->cd();
   641f7fc554e44d26b9912578b73fb047->SetSelected(641f7fc554e44d26b9912578b73fb047);
}
