{
//=========Macro generated from canvas: 6bd300d886d64556bf6a98e35799e214/
//=========  (Wed May 18 15:27:49 2016) by ROOT version5.34/26
   TCanvas *6bd300d886d64556bf6a98e35799e214 = new TCanvas("6bd300d886d64556bf6a98e35799e214", "",10,45,704,527);
   6bd300d886d64556bf6a98e35799e214->SetHighLightColor(2);
   6bd300d886d64556bf6a98e35799e214->Range(1.416657e+09,12.125,1.42346e+09,20.875);
   6bd300d886d64556bf6a98e35799e214->SetFillColor(0);
   6bd300d886d64556bf6a98e35799e214->SetBorderMode(0);
   6bd300d886d64556bf6a98e35799e214->SetBorderSize(2);
   6bd300d886d64556bf6a98e35799e214->SetFrameBorderMode(0);
   6bd300d886d64556bf6a98e35799e214->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(13);
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
   gre->SetPoint(0,1.417637e+09,15.4);
   gre->SetPointError(0,0,0.64);
   gre->SetPoint(1,1.418056e+09,14.79);
   gre->SetPointError(1,0,1.1);
   gre->SetPoint(2,1.418225e+09,14.56);
   gre->SetPointError(2,0,0.84);
   gre->SetPoint(3,1.418831e+09,13.71);
   gre->SetPointError(3,0,0.62);
   gre->SetPoint(4,1.42056e+09,13.41);
   gre->SetPointError(4,0,0.83);
   gre->SetPoint(5,1.420562e+09,15.41);
   gre->SetPointError(5,0,0.7);
   gre->SetPoint(6,1.420566e+09,14.57);
   gre->SetPointError(6,0,0.75);
   gre->SetPoint(7,1.420571e+09,12.56);
   gre->SetPointError(7,0,0.81);
   gre->SetPoint(8,1.421262e+09,13.68);
   gre->SetPointError(8,0,0.91);
   gre->SetPoint(9,1.422464e+09,15.84);
   gre->SetPointError(9,0,0.74);
   gre->SetPoint(10,1.42247e+09,0);
   gre->SetPointError(10,0,0);
   gre->SetPoint(11,1.422475e+09,14.74);
   gre->SetPointError(11,0,0.78);
   gre->SetPoint(12,1.42248e+09,14.07);
   gre->SetPointError(12,0,1.02);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","Gas Gain Method Comparison - 0.0 mm < R < 13.0 mm",100,1.417337e+09,1.42278e+09);
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
   grae->SetPoint(0,1.417337e+09,14.4669);
   grae->SetPointError(0,0,0,0.2251919,0.2251919);
   grae->SetPoint(1,1.42278e+09,14.4669);
   grae->SetPointError(1,0,0,0.2251919,0.2251919);
   
   TH1F *Graph_conf_gate_pi_(0.0, 13.0)1 = new TH1F("Graph_conf_gate_pi_(0.0, 13.0)1","Graph",100,1.416793e+09,1.423324e+09);
   Graph_conf_gate_pi_(0.0, 13.0)1->SetMinimum(14.19667);
   Graph_conf_gate_pi_(0.0, 13.0)1->SetMaximum(14.73713);
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
   
   gre = new TGraphErrors(13);
   gre->SetName("Graph1");
   gre->SetTitle("Gas Gain Method Comparison - 0.0 mm < R < 13.0 mm");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#00ff00");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#00ff00");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.417637e+09,12.74);
   gre->SetPointError(0,0,0.49);
   gre->SetPoint(1,1.418056e+09,12);
   gre->SetPointError(1,0,1.09);
   gre->SetPoint(2,1.418225e+09,13.39);
   gre->SetPointError(2,0,0.5);
   gre->SetPoint(3,1.418831e+09,12.24);
   gre->SetPointError(3,0,0.47);
   gre->SetPoint(4,1.42056e+09,12);
   gre->SetPointError(4,0,0.48);
   gre->SetPoint(5,1.420562e+09,13.71);
   gre->SetPointError(5,0,0.91);
   gre->SetPoint(6,1.420566e+09,12.43);
   gre->SetPointError(6,0,0.68);
   gre->SetPoint(7,1.420571e+09,0);
   gre->SetPointError(7,0,0);
   gre->SetPoint(8,1.421262e+09,0);
   gre->SetPointError(8,0,0);
   gre->SetPoint(9,1.422464e+09,12.21);
   gre->SetPointError(9,0,0.38);
   gre->SetPoint(10,1.42247e+09,12);
   gre->SetPointError(10,0,0.33);
   gre->SetPoint(11,1.422475e+09,12.17);
   gre->SetPointError(11,0,0.44);
   gre->SetPoint(12,1.42248e+09,0);
   gre->SetPointError(12,0,0);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Gas Gain Method Comparison - 0.0 mm < R < 13.0 mm",100,1.417337e+09,1.42278e+09);
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
   grae->SetPoint(0,1.417337e+09,12.35411);
   grae->SetPointError(0,0,0,0.1533618,0.1533618);
   grae->SetPoint(1,1.42278e+09,12.35411);
   grae->SetPointError(1,0,0,0.1533618,0.1533618);
   
   TH1F *Graph_conf_misc_e_(0.0, 13.0)2 = new TH1F("Graph_conf_misc_e_(0.0, 13.0)2","Graph",100,1.416793e+09,1.423324e+09);
   Graph_conf_misc_e_(0.0, 13.0)2->SetMinimum(12.17008);
   Graph_conf_misc_e_(0.0, 13.0)2->SetMaximum(12.53814);
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
   TText *text = pt->AddText("gate_pi: 14.47 +/- 0.23");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.7,0.5,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#00ff00");
   pt->SetTextColor(ci);
   text = pt->AddText("misc_e: 12.35 +/- 0.15");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.9374,0.85,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("Gas Gain Method Comparison - 0.0 mm < R < 13.0 mm");
   pt->Draw();
   6bd300d886d64556bf6a98e35799e214->Modified();
   6bd300d886d64556bf6a98e35799e214->cd();
   6bd300d886d64556bf6a98e35799e214->SetSelected(6bd300d886d64556bf6a98e35799e214);
}
