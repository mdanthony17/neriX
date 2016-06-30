{
//=========Macro generated from canvas: 1aac9783c15d46698196aa0fa165b578/
//=========  (Wed May 18 15:27:49 2016) by ROOT version5.34/26
   TCanvas *1aac9783c15d46698196aa0fa165b578 = new TCanvas("1aac9783c15d46698196aa0fa165b578", "",10,45,704,527);
   1aac9783c15d46698196aa0fa165b578->SetHighLightColor(2);
   1aac9783c15d46698196aa0fa165b578->Range(1.416657e+09,12.125,1.42346e+09,20.875);
   1aac9783c15d46698196aa0fa165b578->SetFillColor(0);
   1aac9783c15d46698196aa0fa165b578->SetBorderMode(0);
   1aac9783c15d46698196aa0fa165b578->SetBorderSize(2);
   1aac9783c15d46698196aa0fa165b578->SetFrameBorderMode(0);
   1aac9783c15d46698196aa0fa165b578->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(13);
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
   gre->SetPoint(0,1.417637e+09,14.1);
   gre->SetPointError(0,0,0.66);
   gre->SetPoint(1,1.418056e+09,14.08);
   gre->SetPointError(1,0,1.16);
   gre->SetPoint(2,1.418225e+09,14.23);
   gre->SetPointError(2,0,0.81);
   gre->SetPoint(3,1.418831e+09,0);
   gre->SetPointError(3,0,0);
   gre->SetPoint(4,1.42056e+09,14.86);
   gre->SetPointError(4,0,0.88);
   gre->SetPoint(5,1.420562e+09,14.7);
   gre->SetPointError(5,0,0.69);
   gre->SetPoint(6,1.420566e+09,14.16);
   gre->SetPointError(6,0,0.84);
   gre->SetPoint(7,1.420571e+09,15.97);
   gre->SetPointError(7,0,0.71);
   gre->SetPoint(8,1.421262e+09,14);
   gre->SetPointError(8,0,0.73);
   gre->SetPoint(9,1.422464e+09,0);
   gre->SetPointError(9,0,0);
   gre->SetPoint(10,1.42247e+09,14.3);
   gre->SetPointError(10,0,0.7);
   gre->SetPoint(11,1.422475e+09,14.18);
   gre->SetPointError(11,0,1.02);
   gre->SetPoint(12,1.42248e+09,15.29);
   gre->SetPointError(12,0,0.7);
   
   TH1F *Graph_Graph3 = new TH1F("Graph_Graph3","Gas Gain Method Comparison - 13.0 mm < R < 21.0 mm",100,1.417337e+09,1.42278e+09);
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
   grae->SetPoint(0,1.417337e+09,14.58949);
   grae->SetPointError(0,0,0,0.233922,0.233922);
   grae->SetPoint(1,1.42278e+09,14.58949);
   grae->SetPointError(1,0,0,0.233922,0.233922);
   
   TH1F *Graph_conf_gate_pi_(13.0, 21.0)3 = new TH1F("Graph_conf_gate_pi_(13.0, 21.0)3","Graph",100,1.416793e+09,1.423324e+09);
   Graph_conf_gate_pi_(13.0, 21.0)3->SetMinimum(14.30879);
   Graph_conf_gate_pi_(13.0, 21.0)3->SetMaximum(14.8702);
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
   
   gre = new TGraphErrors(13);
   gre->SetName("Graph1");
   gre->SetTitle("Gas Gain Method Comparison - 13.0 mm < R < 21.0 mm");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#00ff00");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#00ff00");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.417637e+09,12);
   gre->SetPointError(0,0,0.31);
   gre->SetPoint(1,1.418056e+09,12.7);
   gre->SetPointError(1,0,0.56);
   gre->SetPoint(2,1.418225e+09,12);
   gre->SetPointError(2,0,0.42);
   gre->SetPoint(3,1.418831e+09,0);
   gre->SetPointError(3,0,0);
   gre->SetPoint(4,1.42056e+09,0);
   gre->SetPointError(4,0,0);
   gre->SetPoint(5,1.420562e+09,0);
   gre->SetPointError(5,0,0);
   gre->SetPoint(6,1.420566e+09,12.23);
   gre->SetPointError(6,0,0.74);
   gre->SetPoint(7,1.420571e+09,12);
   gre->SetPointError(7,0,0.8);
   gre->SetPoint(8,1.421262e+09,12);
   gre->SetPointError(8,0,0.42);
   gre->SetPoint(9,1.422464e+09,12.09);
   gre->SetPointError(9,0,0.33);
   gre->SetPoint(10,1.42247e+09,12);
   gre->SetPointError(10,0,0.31);
   gre->SetPoint(11,1.422475e+09,0);
   gre->SetPointError(11,0,0);
   gre->SetPoint(12,1.42248e+09,12.27);
   gre->SetPointError(12,0,0.42);
   
   TH1F *Graph_Graph4 = new TH1F("Graph_Graph4","Gas Gain Method Comparison - 13.0 mm < R < 21.0 mm",100,1.417337e+09,1.42278e+09);
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
   grae->SetPoint(0,1.417337e+09,12.09349);
   grae->SetPointError(0,0,0,0.1366168,0.1366168);
   grae->SetPoint(1,1.42278e+09,12.09349);
   grae->SetPointError(1,0,0,0.1366168,0.1366168);
   
   TH1F *Graph_conf_misc_e_(13.0, 21.0)4 = new TH1F("Graph_conf_misc_e_(13.0, 21.0)4","Graph",100,1.416793e+09,1.423324e+09);
   Graph_conf_misc_e_(13.0, 21.0)4->SetMinimum(11.92955);
   Graph_conf_misc_e_(13.0, 21.0)4->SetMaximum(12.25743);
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
   TText *text = pt->AddText("gate_pi: 14.59 +/- 0.23");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.7,0.5,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#00ff00");
   pt->SetTextColor(ci);
   text = pt->AddText("misc_e: 12.09 +/- 0.14");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.9374,0.85,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("Gas Gain Method Comparison - 13.0 mm < R < 21.0 mm");
   pt->Draw();
   1aac9783c15d46698196aa0fa165b578->Modified();
   1aac9783c15d46698196aa0fa165b578->cd();
   1aac9783c15d46698196aa0fa165b578->SetSelected(1aac9783c15d46698196aa0fa165b578);
}
