{
//=========Macro generated from canvas: 6bf440064d3244f3a523ac7eb644b41f/
//=========  (Wed May 18 14:26:22 2016) by ROOT version5.34/26
   TCanvas *6bf440064d3244f3a523ac7eb644b41f = new TCanvas("6bf440064d3244f3a523ac7eb644b41f", "",10,45,704,527);
   6bf440064d3244f3a523ac7eb644b41f->SetHighLightColor(2);
   6bf440064d3244f3a523ac7eb644b41f->Range(1.40989e+09,12.125,1.415959e+09,20.875);
   6bf440064d3244f3a523ac7eb644b41f->SetFillColor(0);
   6bf440064d3244f3a523ac7eb644b41f->SetBorderMode(0);
   6bf440064d3244f3a523ac7eb644b41f->SetBorderSize(2);
   6bf440064d3244f3a523ac7eb644b41f->SetFrameBorderMode(0);
   6bf440064d3244f3a523ac7eb644b41f->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(19);
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
   gre->SetPoint(0,1.410797e+09,17.99);
   gre->SetPointError(0,0,0.42);
   gre->SetPoint(1,1.410963e+09,17.1);
   gre->SetPointError(1,0,0.41);
   gre->SetPoint(2,1.412004e+09,17.79);
   gre->SetPointError(2,0,0.24);
   gre->SetPoint(3,1.412007e+09,17.5);
   gre->SetPointError(3,0,0.18);
   gre->SetPoint(4,1.412084e+09,16.66);
   gre->SetPointError(4,0,0.42);
   gre->SetPoint(5,1.412087e+09,17.76);
   gre->SetPointError(5,0,0.26);
   gre->SetPoint(6,1.412091e+09,17.66);
   gre->SetPointError(6,0,0.26);
   gre->SetPoint(7,1.412109e+09,17.17);
   gre->SetPointError(7,0,0.32);
   gre->SetPoint(8,1.412354e+09,17.3);
   gre->SetPointError(8,0,0.35);
   gre->SetPoint(9,1.412603e+09,17.16);
   gre->SetPointError(9,0,0.39);
   gre->SetPoint(10,1.413297e+09,16.7);
   gre->SetPointError(10,0,0.3);
   gre->SetPoint(11,1.41331e+09,16.33);
   gre->SetPointError(11,0,0.3);
   gre->SetPoint(12,1.413388e+09,17.07);
   gre->SetPointError(12,0,0.27);
   gre->SetPoint(13,1.413992e+09,15.66);
   gre->SetPointError(13,0,0.42);
   gre->SetPoint(14,1.414595e+09,16.18);
   gre->SetPointError(14,0,0.72);
   gre->SetPoint(15,1.41503e+09,15.59);
   gre->SetPointError(15,0,0.38);
   gre->SetPoint(16,1.415048e+09,0);
   gre->SetPointError(16,0,0);
   gre->SetPoint(17,1.41505e+09,15.65);
   gre->SetPointError(17,0,0.28);
   gre->SetPoint(18,1.415052e+09,15.87);
   gre->SetPointError(18,0,0.28);
   
   TH1F *Graph_Graph5 = new TH1F("Graph_Graph5","Gas Gain Method Comparison - 0.0 mm < R < 18.0 mm",100,1.410497e+09,1.415352e+09);
   Graph_Graph5->SetMinimum(13);
   Graph_Graph5->SetMaximum(20);
   Graph_Graph5->SetDirectory(0);
   Graph_Graph5->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph5->SetLineColor(ci);
   Graph_Graph5->GetXaxis()->SetLabelFont(42);
   Graph_Graph5->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph5->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph5->GetXaxis()->SetTitleFont(42);
   Graph_Graph5->GetYaxis()->SetLabelFont(42);
   Graph_Graph5->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph5->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph5->GetYaxis()->SetTitleFont(42);
   Graph_Graph5->GetZaxis()->SetLabelFont(42);
   Graph_Graph5->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph5->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph5->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph5);
   
   gre->Draw("ap");
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(2);
   grae->SetName("conf_gate_pi_(0.0, 18.0)");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#0000ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3004);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.410497e+09,16.99592);
   grae->SetPointError(0,0,0,0.07150626,0.07150626);
   grae->SetPoint(1,1.415352e+09,16.99592);
   grae->SetPointError(1,0,0,0.07150626,0.07150626);
   
   TH1F *Graph_conf_gate_pi_(0.0, 18.0)5 = new TH1F("Graph_conf_gate_pi_(0.0, 18.0)5","Graph",100,1.410011e+09,1.415837e+09);
   Graph_conf_gate_pi_(0.0, 18.0)5->SetMinimum(16.91011);
   Graph_conf_gate_pi_(0.0, 18.0)5->SetMaximum(17.08172);
   Graph_conf_gate_pi_(0.0, 18.0)5->SetDirectory(0);
   Graph_conf_gate_pi_(0.0, 18.0)5->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_conf_gate_pi_(0.0, 18.0)5->SetLineColor(ci);
   Graph_conf_gate_pi_(0.0, 18.0)5->GetXaxis()->SetLabelFont(42);
   Graph_conf_gate_pi_(0.0, 18.0)5->GetXaxis()->SetLabelSize(0.035);
   Graph_conf_gate_pi_(0.0, 18.0)5->GetXaxis()->SetTitleSize(0.035);
   Graph_conf_gate_pi_(0.0, 18.0)5->GetXaxis()->SetTitleFont(42);
   Graph_conf_gate_pi_(0.0, 18.0)5->GetYaxis()->SetLabelFont(42);
   Graph_conf_gate_pi_(0.0, 18.0)5->GetYaxis()->SetLabelSize(0.035);
   Graph_conf_gate_pi_(0.0, 18.0)5->GetYaxis()->SetTitleSize(0.035);
   Graph_conf_gate_pi_(0.0, 18.0)5->GetYaxis()->SetTitleFont(42);
   Graph_conf_gate_pi_(0.0, 18.0)5->GetZaxis()->SetLabelFont(42);
   Graph_conf_gate_pi_(0.0, 18.0)5->GetZaxis()->SetLabelSize(0.035);
   Graph_conf_gate_pi_(0.0, 18.0)5->GetZaxis()->SetTitleSize(0.035);
   Graph_conf_gate_pi_(0.0, 18.0)5->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_conf_gate_pi_(0.0, 18.0)5);
   
   grae->Draw("3 ");
   
   gre = new TGraphErrors(19);
   gre->SetName("Graph1");
   gre->SetTitle("Gas Gain Method Comparison - 0.0 mm < R < 18.0 mm");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#00ff00");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#00ff00");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.410797e+09,16.49);
   gre->SetPointError(0,0,0.44);
   gre->SetPoint(1,1.410963e+09,16.83);
   gre->SetPointError(1,0,0.36);
   gre->SetPoint(2,1.412004e+09,16.12);
   gre->SetPointError(2,0,0.47);
   gre->SetPoint(3,1.412007e+09,15.23);
   gre->SetPointError(3,0,0.5);
   gre->SetPoint(4,1.412084e+09,16.7);
   gre->SetPointError(4,0,0.38);
   gre->SetPoint(5,1.412087e+09,17.07);
   gre->SetPointError(5,0,0.37);
   gre->SetPoint(6,1.412091e+09,16.61);
   gre->SetPointError(6,0,0.42);
   gre->SetPoint(7,1.412109e+09,16.6);
   gre->SetPointError(7,0,0.29);
   gre->SetPoint(8,1.412354e+09,17.2);
   gre->SetPointError(8,0,0.31);
   gre->SetPoint(9,1.412603e+09,0);
   gre->SetPointError(9,0,0);
   gre->SetPoint(10,1.413297e+09,15.8);
   gre->SetPointError(10,0,0.37);
   gre->SetPoint(11,1.41331e+09,16.08);
   gre->SetPointError(11,0,0.3);
   gre->SetPoint(12,1.413388e+09,15.98);
   gre->SetPointError(12,0,0.27);
   gre->SetPoint(13,1.413992e+09,15.64);
   gre->SetPointError(13,0,0.28);
   gre->SetPoint(14,1.414595e+09,16.17);
   gre->SetPointError(14,0,0.26);
   gre->SetPoint(15,1.41503e+09,15.97);
   gre->SetPointError(15,0,0.25);
   gre->SetPoint(16,1.415048e+09,15.02);
   gre->SetPointError(16,0,0.34);
   gre->SetPoint(17,1.41505e+09,15.03);
   gre->SetPointError(17,0,0.33);
   gre->SetPoint(18,1.415052e+09,14.91);
   gre->SetPointError(18,0,0.28);
   
   TH1F *Graph_Graph6 = new TH1F("Graph_Graph6","Gas Gain Method Comparison - 0.0 mm < R < 18.0 mm",100,1.410497e+09,1.415352e+09);
   Graph_Graph6->SetMinimum(13);
   Graph_Graph6->SetMaximum(20);
   Graph_Graph6->SetDirectory(0);
   Graph_Graph6->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph6->SetLineColor(ci);
   Graph_Graph6->GetXaxis()->SetLabelFont(42);
   Graph_Graph6->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph6->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph6->GetXaxis()->SetTitleFont(42);
   Graph_Graph6->GetYaxis()->SetLabelFont(42);
   Graph_Graph6->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph6->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph6->GetYaxis()->SetTitleFont(42);
   Graph_Graph6->GetZaxis()->SetLabelFont(42);
   Graph_Graph6->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph6->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph6->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph6);
   
   gre->Draw("p ");
   
   grae = new TGraphAsymmErrors(2);
   grae->SetName("conf_misc_e_(0.0, 18.0)");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#00ff00");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3006);

   ci = TColor::GetColor("#00ff00");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.410497e+09,16.04386);
   grae->SetPointError(0,0,0,0.07672518,0.07672518);
   grae->SetPoint(1,1.415352e+09,16.04386);
   grae->SetPointError(1,0,0,0.07672518,0.07672518);
   
   TH1F *Graph_conf_misc_e_(0.0, 18.0)6 = new TH1F("Graph_conf_misc_e_(0.0, 18.0)6","Graph",100,1.410011e+09,1.415837e+09);
   Graph_conf_misc_e_(0.0, 18.0)6->SetMinimum(15.95179);
   Graph_conf_misc_e_(0.0, 18.0)6->SetMaximum(16.13593);
   Graph_conf_misc_e_(0.0, 18.0)6->SetDirectory(0);
   Graph_conf_misc_e_(0.0, 18.0)6->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_conf_misc_e_(0.0, 18.0)6->SetLineColor(ci);
   Graph_conf_misc_e_(0.0, 18.0)6->GetXaxis()->SetLabelFont(42);
   Graph_conf_misc_e_(0.0, 18.0)6->GetXaxis()->SetLabelSize(0.035);
   Graph_conf_misc_e_(0.0, 18.0)6->GetXaxis()->SetTitleSize(0.035);
   Graph_conf_misc_e_(0.0, 18.0)6->GetXaxis()->SetTitleFont(42);
   Graph_conf_misc_e_(0.0, 18.0)6->GetYaxis()->SetLabelFont(42);
   Graph_conf_misc_e_(0.0, 18.0)6->GetYaxis()->SetLabelSize(0.035);
   Graph_conf_misc_e_(0.0, 18.0)6->GetYaxis()->SetTitleSize(0.035);
   Graph_conf_misc_e_(0.0, 18.0)6->GetYaxis()->SetTitleFont(42);
   Graph_conf_misc_e_(0.0, 18.0)6->GetZaxis()->SetLabelFont(42);
   Graph_conf_misc_e_(0.0, 18.0)6->GetZaxis()->SetLabelSize(0.035);
   Graph_conf_misc_e_(0.0, 18.0)6->GetZaxis()->SetTitleSize(0.035);
   Graph_conf_misc_e_(0.0, 18.0)6->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_conf_misc_e_(0.0, 18.0)6);
   
   grae->Draw("3 ");
   
   TPaveText *pt = new TPaveText(0.15,0.8,0.5,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("gate_pi: 17.00 +/- 0.07");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.7,0.5,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#00ff00");
   pt->SetTextColor(ci);
   text = pt->AddText("misc_e: 16.04 +/- 0.08");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.9374,0.85,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("Gas Gain Method Comparison - 0.0 mm < R < 18.0 mm");
   pt->Draw();
   6bf440064d3244f3a523ac7eb644b41f->Modified();
   6bf440064d3244f3a523ac7eb644b41f->cd();
   6bf440064d3244f3a523ac7eb644b41f->SetSelected(6bf440064d3244f3a523ac7eb644b41f);
}
