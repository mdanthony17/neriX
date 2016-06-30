{
//=========Macro generated from canvas: 39e1c4a034934368b20f56b140d4dc05/
//=========  (Wed May 18 14:26:21 2016) by ROOT version5.34/26
   TCanvas *39e1c4a034934368b20f56b140d4dc05 = new TCanvas("39e1c4a034934368b20f56b140d4dc05", "",10,45,704,527);
   39e1c4a034934368b20f56b140d4dc05->SetHighLightColor(2);
   39e1c4a034934368b20f56b140d4dc05->Range(1.40989e+09,12.125,1.415959e+09,20.875);
   39e1c4a034934368b20f56b140d4dc05->SetFillColor(0);
   39e1c4a034934368b20f56b140d4dc05->SetBorderMode(0);
   39e1c4a034934368b20f56b140d4dc05->SetBorderSize(2);
   39e1c4a034934368b20f56b140d4dc05->SetFrameBorderMode(0);
   39e1c4a034934368b20f56b140d4dc05->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(19);
   gre->SetName("Graph0");
   gre->SetTitle("Gas Gain Method Comparison - 13.0 mm < R < 21.0 mm");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.410797e+09,18.13);
   gre->SetPointError(0,0,0.43);
   gre->SetPoint(1,1.410963e+09,17.2);
   gre->SetPointError(1,0,0.41);
   gre->SetPoint(2,1.412004e+09,0);
   gre->SetPointError(2,0,0);
   gre->SetPoint(3,1.412007e+09,17.58);
   gre->SetPointError(3,0,0.2);
   gre->SetPoint(4,1.412084e+09,17.53);
   gre->SetPointError(4,0,0.34);
   gre->SetPoint(5,1.412087e+09,17.75);
   gre->SetPointError(5,0,0.33);
   gre->SetPoint(6,1.412091e+09,17.8);
   gre->SetPointError(6,0,0.26);
   gre->SetPoint(7,1.412109e+09,17.36);
   gre->SetPointError(7,0,0.35);
   gre->SetPoint(8,1.412354e+09,17.8);
   gre->SetPointError(8,0,0.3);
   gre->SetPoint(9,1.412603e+09,16.84);
   gre->SetPointError(9,0,0.46);
   gre->SetPoint(10,1.413297e+09,16.69);
   gre->SetPointError(10,0,0.39);
   gre->SetPoint(11,1.41331e+09,16.45);
   gre->SetPointError(11,0,0.33);
   gre->SetPoint(12,1.413388e+09,16.97);
   gre->SetPointError(12,0,0.32);
   gre->SetPoint(13,1.413992e+09,14.82);
   gre->SetPointError(13,0,0.63);
   gre->SetPoint(14,1.414595e+09,15.88);
   gre->SetPointError(14,0,0.56);
   gre->SetPoint(15,1.41503e+09,16.11);
   gre->SetPointError(15,0,0.42);
   gre->SetPoint(16,1.415048e+09,15.3);
   gre->SetPointError(16,0,0.33);
   gre->SetPoint(17,1.41505e+09,0);
   gre->SetPointError(17,0,0);
   gre->SetPoint(18,1.415052e+09,15.73);
   gre->SetPointError(18,0,0.32);
   
   TH1F *Graph_Graph3 = new TH1F("Graph_Graph3","Gas Gain Method Comparison - 13.0 mm < R < 21.0 mm",100,1.410497e+09,1.415352e+09);
   Graph_Graph3->SetMinimum(13);
   Graph_Graph3->SetMaximum(20);
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
   
   gre->Draw("ap");
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(2);
   grae->SetName("conf_gate_pi_(13.0, 21.0)");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#0000ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3004);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.410497e+09,17.04485);
   grae->SetPointError(0,0,0,0.08196937,0.08196937);
   grae->SetPoint(1,1.415352e+09,17.04485);
   grae->SetPointError(1,0,0,0.08196937,0.08196937);
   
   TH1F *Graph_conf_gate_pi_(13.0, 21.0)3 = new TH1F("Graph_conf_gate_pi_(13.0, 21.0)3","Graph",100,1.410011e+09,1.415837e+09);
   Graph_conf_gate_pi_(13.0, 21.0)3->SetMinimum(16.94649);
   Graph_conf_gate_pi_(13.0, 21.0)3->SetMaximum(17.14321);
   Graph_conf_gate_pi_(13.0, 21.0)3->SetDirectory(0);
   Graph_conf_gate_pi_(13.0, 21.0)3->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_conf_gate_pi_(13.0, 21.0)3->SetLineColor(ci);
   Graph_conf_gate_pi_(13.0, 21.0)3->GetXaxis()->SetLabelFont(42);
   Graph_conf_gate_pi_(13.0, 21.0)3->GetXaxis()->SetLabelSize(0.035);
   Graph_conf_gate_pi_(13.0, 21.0)3->GetXaxis()->SetTitleSize(0.035);
   Graph_conf_gate_pi_(13.0, 21.0)3->GetXaxis()->SetTitleFont(42);
   Graph_conf_gate_pi_(13.0, 21.0)3->GetYaxis()->SetLabelFont(42);
   Graph_conf_gate_pi_(13.0, 21.0)3->GetYaxis()->SetLabelSize(0.035);
   Graph_conf_gate_pi_(13.0, 21.0)3->GetYaxis()->SetTitleSize(0.035);
   Graph_conf_gate_pi_(13.0, 21.0)3->GetYaxis()->SetTitleFont(42);
   Graph_conf_gate_pi_(13.0, 21.0)3->GetZaxis()->SetLabelFont(42);
   Graph_conf_gate_pi_(13.0, 21.0)3->GetZaxis()->SetLabelSize(0.035);
   Graph_conf_gate_pi_(13.0, 21.0)3->GetZaxis()->SetTitleSize(0.035);
   Graph_conf_gate_pi_(13.0, 21.0)3->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_conf_gate_pi_(13.0, 21.0)3);
   
   grae->Draw("3 ");
   
   gre = new TGraphErrors(19);
   gre->SetName("Graph1");
   gre->SetTitle("Gas Gain Method Comparison - 13.0 mm < R < 21.0 mm");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#00ff00");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#00ff00");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.410797e+09,17);
   gre->SetPointError(0,0,0.46);
   gre->SetPoint(1,1.410963e+09,16.6);
   gre->SetPointError(1,0,0.39);
   gre->SetPoint(2,1.412004e+09,15.59);
   gre->SetPointError(2,0,0.62);
   gre->SetPoint(3,1.412007e+09,15.4);
   gre->SetPointError(3,0,0.58);
   gre->SetPoint(4,1.412084e+09,15.97);
   gre->SetPointError(4,0,0.63);
   gre->SetPoint(5,1.412087e+09,17.02);
   gre->SetPointError(5,0,0.34);
   gre->SetPoint(6,1.412091e+09,16.29);
   gre->SetPointError(6,0,0.53);
   gre->SetPoint(7,1.412109e+09,0);
   gre->SetPointError(7,0,0);
   gre->SetPoint(8,1.412354e+09,16.67);
   gre->SetPointError(8,0,0.41);
   gre->SetPoint(9,1.412603e+09,0);
   gre->SetPointError(9,0,0);
   gre->SetPoint(10,1.413297e+09,15.96);
   gre->SetPointError(10,0,0.35);
   gre->SetPoint(11,1.41331e+09,16.51);
   gre->SetPointError(11,0,0.35);
   gre->SetPoint(12,1.413388e+09,15.98);
   gre->SetPointError(12,0,0.42);
   gre->SetPoint(13,1.413992e+09,15.65);
   gre->SetPointError(13,0,0.35);
   gre->SetPoint(14,1.414595e+09,16.21);
   gre->SetPointError(14,0,0.26);
   gre->SetPoint(15,1.41503e+09,15.72);
   gre->SetPointError(15,0,0.28);
   gre->SetPoint(16,1.415048e+09,15.34);
   gre->SetPointError(16,0,0.35);
   gre->SetPoint(17,1.41505e+09,15.49);
   gre->SetPointError(17,0,0.36);
   gre->SetPoint(18,1.415052e+09,15.86);
   gre->SetPointError(18,0,0.3);
   
   TH1F *Graph_Graph4 = new TH1F("Graph_Graph4","Gas Gain Method Comparison - 13.0 mm < R < 21.0 mm",100,1.410497e+09,1.415352e+09);
   Graph_Graph4->SetMinimum(13);
   Graph_Graph4->SetMaximum(20);
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
   grae->SetName("conf_misc_e_(13.0, 21.0)");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#00ff00");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3006);

   ci = TColor::GetColor("#00ff00");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.410497e+09,16.07451);
   grae->SetPointError(0,0,0,0.09044288,0.09044288);
   grae->SetPoint(1,1.415352e+09,16.07451);
   grae->SetPointError(1,0,0,0.09044288,0.09044288);
   
   TH1F *Graph_conf_misc_e_(13.0, 21.0)4 = new TH1F("Graph_conf_misc_e_(13.0, 21.0)4","Graph",100,1.410011e+09,1.415837e+09);
   Graph_conf_misc_e_(13.0, 21.0)4->SetMinimum(15.96598);
   Graph_conf_misc_e_(13.0, 21.0)4->SetMaximum(16.18305);
   Graph_conf_misc_e_(13.0, 21.0)4->SetDirectory(0);
   Graph_conf_misc_e_(13.0, 21.0)4->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_conf_misc_e_(13.0, 21.0)4->SetLineColor(ci);
   Graph_conf_misc_e_(13.0, 21.0)4->GetXaxis()->SetLabelFont(42);
   Graph_conf_misc_e_(13.0, 21.0)4->GetXaxis()->SetLabelSize(0.035);
   Graph_conf_misc_e_(13.0, 21.0)4->GetXaxis()->SetTitleSize(0.035);
   Graph_conf_misc_e_(13.0, 21.0)4->GetXaxis()->SetTitleFont(42);
   Graph_conf_misc_e_(13.0, 21.0)4->GetYaxis()->SetLabelFont(42);
   Graph_conf_misc_e_(13.0, 21.0)4->GetYaxis()->SetLabelSize(0.035);
   Graph_conf_misc_e_(13.0, 21.0)4->GetYaxis()->SetTitleSize(0.035);
   Graph_conf_misc_e_(13.0, 21.0)4->GetYaxis()->SetTitleFont(42);
   Graph_conf_misc_e_(13.0, 21.0)4->GetZaxis()->SetLabelFont(42);
   Graph_conf_misc_e_(13.0, 21.0)4->GetZaxis()->SetLabelSize(0.035);
   Graph_conf_misc_e_(13.0, 21.0)4->GetZaxis()->SetTitleSize(0.035);
   Graph_conf_misc_e_(13.0, 21.0)4->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_conf_misc_e_(13.0, 21.0)4);
   
   grae->Draw("3 ");
   
   TPaveText *pt = new TPaveText(0.15,0.8,0.5,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("gate_pi: 17.04 +/- 0.08");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.7,0.5,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#00ff00");
   pt->SetTextColor(ci);
   text = pt->AddText("misc_e: 16.07 +/- 0.09");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.9374,0.85,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("Gas Gain Method Comparison - 13.0 mm < R < 21.0 mm");
   pt->Draw();
   39e1c4a034934368b20f56b140d4dc05->Modified();
   39e1c4a034934368b20f56b140d4dc05->cd();
   39e1c4a034934368b20f56b140d4dc05->SetSelected(39e1c4a034934368b20f56b140d4dc05);
}
