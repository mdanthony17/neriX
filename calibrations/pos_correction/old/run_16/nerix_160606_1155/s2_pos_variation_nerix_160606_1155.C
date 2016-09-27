{
//=========Macro generated from canvas: 64b2efc9fff0444db5888d08a28935a3/
//=========  (Mon Jun 13 10:58:57 2016) by ROOT version5.34/26
   TCanvas *64b2efc9fff0444db5888d08a28935a3 = new TCanvas("64b2efc9fff0444db5888d08a28935a3", "",10,45,1804,727);
   64b2efc9fff0444db5888d08a28935a3->SetHighLightColor(2);
   64b2efc9fff0444db5888d08a28935a3->Range(0,0,1,1);
   64b2efc9fff0444db5888d08a28935a3->SetFillColor(0);
   64b2efc9fff0444db5888d08a28935a3->SetBorderMode(0);
   64b2efc9fff0444db5888d08a28935a3->SetBorderSize(2);
   64b2efc9fff0444db5888d08a28935a3->SetFrameBorderMode(0);
  
// ------------>Primitives in pad: 64b2efc9fff0444db5888d08a28935a3_1
   TPad *64b2efc9fff0444db5888d08a28935a3_1 = new TPad("64b2efc9fff0444db5888d08a28935a3_1", "64b2efc9fff0444db5888d08a28935a3_1",0.01,0.01,0.49,0.99);
   64b2efc9fff0444db5888d08a28935a3_1->Draw();
   64b2efc9fff0444db5888d08a28935a3_1->cd();
   64b2efc9fff0444db5888d08a28935a3_1->Range(-2.2,0.125,19.8,1.875);
   64b2efc9fff0444db5888d08a28935a3_1->SetFillColor(0);
   64b2efc9fff0444db5888d08a28935a3_1->SetBorderMode(0);
   64b2efc9fff0444db5888d08a28935a3_1->SetBorderSize(2);
   64b2efc9fff0444db5888d08a28935a3_1->SetFrameBorderMode(0);
   64b2efc9fff0444db5888d08a28935a3_1->SetFrameBorderMode(0);
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(16);
   grae->SetName("Graph0");
   grae->SetTitle("S2 vs. dt (adjusted for gate time)");
   grae->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#ff00ff");
   grae->SetLineColor(ci);
   grae->SetMarkerSize(0);
   grae->SetPoint(0,0.5,0.965);
   grae->SetPointError(0,0.5,0.5,0.07,0.07);
   grae->SetPoint(1,1.5,0.965);
   grae->SetPointError(1,0.5,0.5,0.1092,0.07);
   grae->SetPoint(2,2.5,0.965);
   grae->SetPointError(2,0.5,0.5,0.07,0.07);
   grae->SetPoint(3,3.5,0.895);
   grae->SetPointError(3,0.5,0.5,0.07,0.07);
   grae->SetPoint(4,4.5,0.895);
   grae->SetPointError(4,0.5,0.5,0.07,0.07);
   grae->SetPoint(5,5.5,0.825);
   grae->SetPointError(5,0.5,0.5,0.07,0.07);
   grae->SetPoint(6,6.5,0.825);
   grae->SetPointError(6,0.5,0.5,0.07,0.07);
   grae->SetPoint(7,7.5,0.755);
   grae->SetPointError(7,0.5,0.5,0.07,0.14);
   grae->SetPoint(8,8.5,0.755);
   grae->SetPointError(8,0.5,0.5,0,0.14);
   grae->SetPoint(9,9.5,0.755);
   grae->SetPointError(9,0.5,0.5,0.07,0.07);
   grae->SetPoint(10,10.5,0.755);
   grae->SetPointError(10,0.5,0.5,0.14,0.07);
   grae->SetPoint(11,11.5,0.685);
   grae->SetPointError(11,0.5,0.5,0.14,0.14);
   grae->SetPoint(12,12.5,0.685);
   grae->SetPointError(12,0.5,0.5,0.07,0.07);
   grae->SetPoint(13,13.5,0.615);
   grae->SetPointError(13,0.5,0.5,0.14,0.14);
   grae->SetPoint(14,14.5,0.545);
   grae->SetPointError(14,0.5,0.5,0.07,0.14);
   grae->SetPoint(15,15.5,0.545);
   grae->SetPointError(15,0.5,0.5,0.21,0.182);
   
   TH1F *Graph_Graph11 = new TH1F("Graph_Graph11","S2 vs. dt (adjusted for gate time)",100,0,17.6);
   Graph_Graph11->SetMinimum(0.3);
   Graph_Graph11->SetMaximum(1.7);
   Graph_Graph11->SetDirectory(0);
   Graph_Graph11->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph11->SetLineColor(ci);
   Graph_Graph11->GetXaxis()->SetTitle("dt [us after gate]");
   Graph_Graph11->GetXaxis()->SetLabelFont(42);
   Graph_Graph11->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph11->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph11->GetXaxis()->SetTitleFont(42);
   Graph_Graph11->GetYaxis()->SetTitle("S2 Normalized to Max");
   Graph_Graph11->GetYaxis()->SetLabelFont(42);
   Graph_Graph11->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph11->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph11->GetYaxis()->SetTitleFont(42);
   Graph_Graph11->GetZaxis()->SetLabelFont(42);
   Graph_Graph11->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph11->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph11->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph11);
   
   grae->Draw("ap");
   
   TF1 *f_s2_dt_0.345000 = new TF1("f_s2_dt_0.345000","[0]*exp(-x/[1])",0,11);
   f_s2_dt_0.345000->SetFillColor(19);
   f_s2_dt_0.345000->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   f_s2_dt_0.345000->SetLineColor(ci);
   f_s2_dt_0.345000->SetLineWidth(2);
   f_s2_dt_0.345000->GetXaxis()->SetLabelFont(42);
   f_s2_dt_0.345000->GetXaxis()->SetLabelSize(0.035);
   f_s2_dt_0.345000->GetXaxis()->SetTitleSize(0.035);
   f_s2_dt_0.345000->GetXaxis()->SetTitleFont(42);
   f_s2_dt_0.345000->GetYaxis()->SetLabelFont(42);
   f_s2_dt_0.345000->GetYaxis()->SetLabelSize(0.035);
   f_s2_dt_0.345000->GetYaxis()->SetTitleSize(0.035);
   f_s2_dt_0.345000->GetYaxis()->SetTitleFont(42);
   f_s2_dt_0.345000->SetParameter(0,1);
   f_s2_dt_0.345000->SetParError(0,0);
   f_s2_dt_0.345000->SetParLimits(0,0,0);
   f_s2_dt_0.345000->SetParameter(1,33.53546);
   f_s2_dt_0.345000->SetParError(1,0);
   f_s2_dt_0.345000->SetParLimits(1,0,0);
   f_s2_dt_0.345000->Draw("same");
   
   TPaveText *pt = new TPaveText(0.15,0.85,0.7,0.9,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("V_{c} = 0.345kV: electron lifetime = 3.35e+01 +/- 6.72e+00 us");
   pt->Draw();
   
   pt = new TPaveText(0.1775,0.9355248,0.8225,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("S2 vs. dt (adjusted for gate time)");
   pt->Draw();
   
   grae = new TGraphAsymmErrors(15);
   grae->SetName("Graph1");
   grae->SetTitle("S2 vs. dt (adjusted for gate time)");
   grae->SetFillColor(1);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetMarkerSize(0);
   grae->SetPoint(0,0.5,0.965);
   grae->SetPointError(0,0.5,0.5,0,0.07);
   grae->SetPoint(1,1.5,0.965);
   grae->SetPointError(1,0.5,0.5,0.07,0.07);
   grae->SetPoint(2,2.5,0.965);
   grae->SetPointError(2,0.5,0.5,0.07,0.07);
   grae->SetPoint(3,3.5,0.965);
   grae->SetPointError(3,0.5,0.5,0.07,0.07);
   grae->SetPoint(4,4.5,0.895);
   grae->SetPointError(4,0.5,0.5,0.07,0.07);
   grae->SetPoint(5,5.5,0.895);
   grae->SetPointError(5,0.5,0.5,0.07,0.07);
   grae->SetPoint(6,6.5,0.895);
   grae->SetPointError(6,0.5,0.5,0.07,0.07);
   grae->SetPoint(7,7.5,0.895);
   grae->SetPointError(7,0.5,0.5,0.07,0.07);
   grae->SetPoint(8,8.5,0.86);
   grae->SetPointError(8,0.5,0.5,0.105,0.035);
   grae->SetPoint(9,9.5,0.825);
   grae->SetPointError(9,0.5,0.5,0,0.07);
   grae->SetPoint(10,10.5,0.825);
   grae->SetPointError(10,0.5,0.5,0.07,0.07);
   grae->SetPoint(11,11.5,0.825);
   grae->SetPointError(11,0.5,0.5,0.14,0.07);
   grae->SetPoint(12,12.5,0.79);
   grae->SetPointError(12,0.5,0.5,0.105,0.105);
   grae->SetPoint(13,13.5,0.755);
   grae->SetPointError(13,0.5,0.5,0.28,0.0784);
   grae->SetPoint(14,14.5,0.72);
   grae->SetPointError(14,0.5,0.5,0.329,0.175);
   
   TH1F *Graph_Graph12 = new TH1F("Graph_Graph12","S2 vs. dt (adjusted for gate time)",100,0,16.5);
   Graph_Graph12->SetMinimum(0.3);
   Graph_Graph12->SetMaximum(1.7);
   Graph_Graph12->SetDirectory(0);
   Graph_Graph12->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph12->SetLineColor(ci);
   Graph_Graph12->GetXaxis()->SetTitle("dt [us after gate]");
   Graph_Graph12->GetXaxis()->SetLabelFont(42);
   Graph_Graph12->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph12->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph12->GetXaxis()->SetTitleFont(42);
   Graph_Graph12->GetYaxis()->SetTitle("S2 Normalized to Max");
   Graph_Graph12->GetYaxis()->SetLabelFont(42);
   Graph_Graph12->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph12->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph12->GetYaxis()->SetTitleFont(42);
   Graph_Graph12->GetZaxis()->SetLabelFont(42);
   Graph_Graph12->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph12->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph12->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph12);
   
   grae->Draw(" p");
   
   TF1 *f_s2_dt_0.700000 = new TF1("f_s2_dt_0.700000","[0]*exp(-x/[1])",0,11);
   f_s2_dt_0.700000->SetFillColor(19);
   f_s2_dt_0.700000->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_s2_dt_0.700000->SetLineColor(ci);
   f_s2_dt_0.700000->SetLineWidth(2);
   f_s2_dt_0.700000->GetXaxis()->SetLabelFont(42);
   f_s2_dt_0.700000->GetXaxis()->SetLabelSize(0.035);
   f_s2_dt_0.700000->GetXaxis()->SetTitleSize(0.035);
   f_s2_dt_0.700000->GetXaxis()->SetTitleFont(42);
   f_s2_dt_0.700000->GetYaxis()->SetLabelFont(42);
   f_s2_dt_0.700000->GetYaxis()->SetLabelSize(0.035);
   f_s2_dt_0.700000->GetYaxis()->SetTitleSize(0.035);
   f_s2_dt_0.700000->GetYaxis()->SetTitleFont(42);
   f_s2_dt_0.700000->SetParameter(0,1);
   f_s2_dt_0.700000->SetParError(0,0);
   f_s2_dt_0.700000->SetParLimits(0,0,0);
   f_s2_dt_0.700000->SetParameter(1,59.33436);
   f_s2_dt_0.700000->SetParError(1,0);
   f_s2_dt_0.700000->SetParLimits(1,0,0);
   f_s2_dt_0.700000->Draw("same");
   
   pt = new TPaveText(0.15,0.8,0.7,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 0.7kV: electron lifetime = 5.93e+01 +/- 2.15e+01 us");
   pt->Draw();
   
   grae = new TGraphAsymmErrors(14);
   grae->SetName("Graph2");
   grae->SetTitle("S2 vs. dt (adjusted for gate time)");
   grae->SetFillColor(1);

   ci = TColor::GetColor("#00cc00");
   grae->SetLineColor(ci);
   grae->SetMarkerSize(0);
   grae->SetPoint(0,0.5,1.035);
   grae->SetPointError(0,0.5,0.5,0.126,0);
   grae->SetPoint(1,1.5,0.965);
   grae->SetPointError(1,0.5,0.5,0.07,0.07);
   grae->SetPoint(2,2.5,0.965);
   grae->SetPointError(2,0.5,0.5,0.07,0.07);
   grae->SetPoint(3,3.5,0.895);
   grae->SetPointError(3,0.5,0.5,0.0644,0.07);
   grae->SetPoint(4,4.5,0.895);
   grae->SetPointError(4,0.5,0.5,0,0.07);
   grae->SetPoint(5,5.5,0.895);
   grae->SetPointError(5,0.5,0.5,0.07,0.07);
   grae->SetPoint(6,6.5,0.895);
   grae->SetPointError(6,0.5,0.5,0.07,0.07);
   grae->SetPoint(7,7.5,0.895);
   grae->SetPointError(7,0.5,0.5,0.07,0.07);
   grae->SetPoint(8,8.5,0.825);
   grae->SetPointError(8,0.5,0.5,0.07,0.07);
   grae->SetPoint(9,9.5,0.825);
   grae->SetPointError(9,0.5,0.5,0.07,0.07);
   grae->SetPoint(10,10.5,0.825);
   grae->SetPointError(10,0.5,0.5,0.07,0.07);
   grae->SetPoint(11,11.5,0.825);
   grae->SetPointError(11,0.5,0.5,0.14,0.07);
   grae->SetPoint(12,12.5,0.825);
   grae->SetPointError(12,0.5,0.5,0.1568,0.07);
   grae->SetPoint(13,13.5,0.685);
   grae->SetPointError(13,0.5,0.5,0.252,0.21);
   
   TH1F *Graph_Graph13 = new TH1F("Graph_Graph13","S2 vs. dt (adjusted for gate time)",100,0,15.4);
   Graph_Graph13->SetMinimum(0.3);
   Graph_Graph13->SetMaximum(1.7);
   Graph_Graph13->SetDirectory(0);
   Graph_Graph13->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph13->SetLineColor(ci);
   Graph_Graph13->GetXaxis()->SetTitle("dt [us after gate]");
   Graph_Graph13->GetXaxis()->SetLabelFont(42);
   Graph_Graph13->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph13->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph13->GetXaxis()->SetTitleFont(42);
   Graph_Graph13->GetYaxis()->SetTitle("S2 Normalized to Max");
   Graph_Graph13->GetYaxis()->SetLabelFont(42);
   Graph_Graph13->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph13->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph13->GetYaxis()->SetTitleFont(42);
   Graph_Graph13->GetZaxis()->SetLabelFont(42);
   Graph_Graph13->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph13->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph13->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph13);
   
   grae->Draw(" p");
   
   TF1 *f_s2_dt_1.054000 = new TF1("f_s2_dt_1.054000","[0]*exp(-x/[1])",0,11);
   f_s2_dt_1.054000->SetFillColor(19);
   f_s2_dt_1.054000->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   f_s2_dt_1.054000->SetLineColor(ci);
   f_s2_dt_1.054000->SetLineWidth(2);
   f_s2_dt_1.054000->GetXaxis()->SetLabelFont(42);
   f_s2_dt_1.054000->GetXaxis()->SetLabelSize(0.035);
   f_s2_dt_1.054000->GetXaxis()->SetTitleSize(0.035);
   f_s2_dt_1.054000->GetXaxis()->SetTitleFont(42);
   f_s2_dt_1.054000->GetYaxis()->SetLabelFont(42);
   f_s2_dt_1.054000->GetYaxis()->SetLabelSize(0.035);
   f_s2_dt_1.054000->GetYaxis()->SetTitleSize(0.035);
   f_s2_dt_1.054000->GetYaxis()->SetTitleFont(42);
   f_s2_dt_1.054000->SetParameter(0,1);
   f_s2_dt_1.054000->SetParError(0,0);
   f_s2_dt_1.054000->SetParLimits(0,0,0);
   f_s2_dt_1.054000->SetParameter(1,52.19538);
   f_s2_dt_1.054000->SetParError(1,0);
   f_s2_dt_1.054000->SetParLimits(1,0,0);
   f_s2_dt_1.054000->Draw("same");
   
   pt = new TPaveText(0.15,0.75,0.7,0.8,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.054kV: electron lifetime = 5.22e+01 +/- 1.42e+01 us");
   pt->Draw();
   
   grae = new TGraphAsymmErrors(13);
   grae->SetName("Graph3");
   grae->SetTitle("S2 vs. dt (adjusted for gate time)");
   grae->SetFillColor(1);

   ci = TColor::GetColor("#ff9933");
   grae->SetLineColor(ci);
   grae->SetMarkerSize(0);
   grae->SetPoint(0,0.5,1.035);
   grae->SetPointError(0,0.5,0.5,0.14,0);
   grae->SetPoint(1,1.5,0.965);
   grae->SetPointError(1,0.5,0.5,0.07,0.07);
   grae->SetPoint(2,2.5,0.965);
   grae->SetPointError(2,0.5,0.5,0.07,0.07);
   grae->SetPoint(3,3.5,0.965);
   grae->SetPointError(3,0.5,0.5,0.07,0.07);
   grae->SetPoint(4,4.5,0.965);
   grae->SetPointError(4,0.5,0.5,0.07,0);
   grae->SetPoint(5,5.5,0.895);
   grae->SetPointError(5,0.5,0.5,0,0.07);
   grae->SetPoint(6,6.5,0.895);
   grae->SetPointError(6,0.5,0.5,0.07,0.07);
   grae->SetPoint(7,7.5,0.895);
   grae->SetPointError(7,0.5,0.5,0.07,0.07);
   grae->SetPoint(8,8.5,0.895);
   grae->SetPointError(8,0.5,0.5,0.07,0.07);
   grae->SetPoint(9,9.5,0.895);
   grae->SetPointError(9,0.5,0.5,0.07,0.07);
   grae->SetPoint(10,10.5,0.825);
   grae->SetPointError(10,0.5,0.5,0.07,0.1232);
   grae->SetPoint(11,11.5,0.895);
   grae->SetPointError(11,0.5,0.5,0.14,0.0672);
   grae->SetPoint(12,12.5,0.825);
   grae->SetPointError(12,0.5,0.5,0.3108,0.07);
   
   TH1F *Graph_Graph14 = new TH1F("Graph_Graph14","S2 vs. dt (adjusted for gate time)",100,0,14.3);
   Graph_Graph14->SetMinimum(0.3);
   Graph_Graph14->SetMaximum(1.7);
   Graph_Graph14->SetDirectory(0);
   Graph_Graph14->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph14->SetLineColor(ci);
   Graph_Graph14->GetXaxis()->SetTitle("dt [us after gate]");
   Graph_Graph14->GetXaxis()->SetLabelFont(42);
   Graph_Graph14->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph14->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph14->GetXaxis()->SetTitleFont(42);
   Graph_Graph14->GetYaxis()->SetTitle("S2 Normalized to Max");
   Graph_Graph14->GetYaxis()->SetLabelFont(42);
   Graph_Graph14->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph14->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph14->GetYaxis()->SetTitleFont(42);
   Graph_Graph14->GetZaxis()->SetLabelFont(42);
   Graph_Graph14->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph14->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph14->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph14);
   
   grae->Draw(" p");
   
   TF1 *f_s2_dt_1.500000 = new TF1("f_s2_dt_1.500000","[0]*exp(-x/[1])",0,11);
   f_s2_dt_1.500000->SetFillColor(19);
   f_s2_dt_1.500000->SetFillStyle(0);

   ci = TColor::GetColor("#ff9933");
   f_s2_dt_1.500000->SetLineColor(ci);
   f_s2_dt_1.500000->SetLineWidth(2);
   f_s2_dt_1.500000->GetXaxis()->SetLabelFont(42);
   f_s2_dt_1.500000->GetXaxis()->SetLabelSize(0.035);
   f_s2_dt_1.500000->GetXaxis()->SetTitleSize(0.035);
   f_s2_dt_1.500000->GetXaxis()->SetTitleFont(42);
   f_s2_dt_1.500000->GetYaxis()->SetLabelFont(42);
   f_s2_dt_1.500000->GetYaxis()->SetLabelSize(0.035);
   f_s2_dt_1.500000->GetYaxis()->SetTitleSize(0.035);
   f_s2_dt_1.500000->GetYaxis()->SetTitleFont(42);
   f_s2_dt_1.500000->SetParameter(0,1);
   f_s2_dt_1.500000->SetParError(0,0);
   f_s2_dt_1.500000->SetParLimits(0,0,0);
   f_s2_dt_1.500000->SetParameter(1,66.29066);
   f_s2_dt_1.500000->SetParError(1,0);
   f_s2_dt_1.500000->SetParLimits(1,0,0);
   f_s2_dt_1.500000->Draw("same");
   
   pt = new TPaveText(0.15,0.7,0.7,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff9933");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.5kV: electron lifetime = 6.63e+01 +/- 3.06e+01 us");
   pt->Draw();
   
   grae = new TGraphAsymmErrors(12);
   grae->SetName("Graph4");
   grae->SetTitle("S2 vs. dt (adjusted for gate time)");
   grae->SetFillColor(1);

   ci = TColor::GetColor("#ff0000");
   grae->SetLineColor(ci);
   grae->SetMarkerSize(0);
   grae->SetPoint(0,0.5,0.965);
   grae->SetPointError(0,0.5,0.5,0.0532,0.07);
   grae->SetPoint(1,1.5,0.965);
   grae->SetPointError(1,0.5,0.5,0.07,0.07);
   grae->SetPoint(2,2.5,0.965);
   grae->SetPointError(2,0.5,0.5,0.07,0.07);
   grae->SetPoint(3,3.5,0.965);
   grae->SetPointError(3,0.5,0.5,0.07,0);
   grae->SetPoint(4,4.5,0.965);
   grae->SetPointError(4,0.5,0.5,0.07,0);
   grae->SetPoint(5,5.5,0.965);
   grae->SetPointError(5,0.5,0.5,0.07,0);
   grae->SetPoint(6,6.5,0.895);
   grae->SetPointError(6,0.5,0.5,0,0.07);
   grae->SetPoint(7,7.5,0.895);
   grae->SetPointError(7,0.5,0.5,0.07,0.07);
   grae->SetPoint(8,8.5,0.895);
   grae->SetPointError(8,0.5,0.5,0,0.07);
   grae->SetPoint(9,9.5,0.895);
   grae->SetPointError(9,0.5,0.5,0.1008,0.07);
   grae->SetPoint(10,10.5,0.895);
   grae->SetPointError(10,0.5,0.5,0.2044,0);
   grae->SetPoint(11,11.5,0.825);
   grae->SetPointError(11,0.5,0.5,0.2128,0.07);
   
   TH1F *Graph_Graph15 = new TH1F("Graph_Graph15","S2 vs. dt (adjusted for gate time)",100,0,13.2);
   Graph_Graph15->SetMinimum(0.3);
   Graph_Graph15->SetMaximum(1.7);
   Graph_Graph15->SetDirectory(0);
   Graph_Graph15->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph15->SetLineColor(ci);
   Graph_Graph15->GetXaxis()->SetTitle("dt [us after gate]");
   Graph_Graph15->GetXaxis()->SetLabelFont(42);
   Graph_Graph15->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph15->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph15->GetXaxis()->SetTitleFont(42);
   Graph_Graph15->GetYaxis()->SetTitle("S2 Normalized to Max");
   Graph_Graph15->GetYaxis()->SetLabelFont(42);
   Graph_Graph15->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph15->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph15->GetYaxis()->SetTitleFont(42);
   Graph_Graph15->GetZaxis()->SetLabelFont(42);
   Graph_Graph15->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph15->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph15->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph15);
   
   grae->Draw(" p");
   
   TF1 *f_s2_dt_2.356000 = new TF1("f_s2_dt_2.356000","[0]*exp(-x/[1])",0,11);
   f_s2_dt_2.356000->SetFillColor(19);
   f_s2_dt_2.356000->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   f_s2_dt_2.356000->SetLineColor(ci);
   f_s2_dt_2.356000->SetLineWidth(2);
   f_s2_dt_2.356000->GetXaxis()->SetLabelFont(42);
   f_s2_dt_2.356000->GetXaxis()->SetLabelSize(0.035);
   f_s2_dt_2.356000->GetXaxis()->SetTitleSize(0.035);
   f_s2_dt_2.356000->GetXaxis()->SetTitleFont(42);
   f_s2_dt_2.356000->GetYaxis()->SetLabelFont(42);
   f_s2_dt_2.356000->GetYaxis()->SetLabelSize(0.035);
   f_s2_dt_2.356000->GetYaxis()->SetTitleSize(0.035);
   f_s2_dt_2.356000->GetYaxis()->SetTitleFont(42);
   f_s2_dt_2.356000->SetParameter(0,1);
   f_s2_dt_2.356000->SetParError(0,0);
   f_s2_dt_2.356000->SetParLimits(0,0,0);
   f_s2_dt_2.356000->SetParameter(1,75.74131);
   f_s2_dt_2.356000->SetParError(1,0);
   f_s2_dt_2.356000->SetParLimits(1,0,0);
   f_s2_dt_2.356000->Draw("same");
   
   pt = new TPaveText(0.15,0.65,0.7,0.7,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 2.356kV: electron lifetime = 7.57e+01 +/- 3.21e+01 us");
   pt->Draw();
   64b2efc9fff0444db5888d08a28935a3_1->Modified();
   64b2efc9fff0444db5888d08a28935a3->cd();
  
// ------------>Primitives in pad: 64b2efc9fff0444db5888d08a28935a3_2
   64b2efc9fff0444db5888d08a28935a3_2 = new TPad("64b2efc9fff0444db5888d08a28935a3_2", "64b2efc9fff0444db5888d08a28935a3_2",0.51,0.01,0.99,0.99);
   64b2efc9fff0444db5888d08a28935a3_2->Draw();
   64b2efc9fff0444db5888d08a28935a3_2->cd();
   64b2efc9fff0444db5888d08a28935a3_2->Range(-0.11,0.48125,0.99,2.16875);
   64b2efc9fff0444db5888d08a28935a3_2->SetFillColor(0);
   64b2efc9fff0444db5888d08a28935a3_2->SetBorderMode(0);
   64b2efc9fff0444db5888d08a28935a3_2->SetBorderSize(2);
   64b2efc9fff0444db5888d08a28935a3_2->SetFrameBorderMode(0);
   64b2efc9fff0444db5888d08a28935a3_2->SetFrameBorderMode(0);
   
   grae = new TGraphAsymmErrors(8);
   grae->SetName("Graph0");
   grae->SetTitle("Relative S2 Signal vs R^{2}");
   grae->SetFillColor(1);

   ci = TColor::GetColor("#ff00ff");
   grae->SetLineColor(ci);
   grae->SetMarkerSize(0);
   grae->SetPoint(0,0.05,0.903125);
   grae->SetPointError(0,0.05,0.05,0.135,0.16875);
   grae->SetPoint(1,0.15,0.903125);
   grae->SetPointError(1,0.05,0.05,0.10125,0.135);
   grae->SetPoint(2,0.25,1.004375);
   grae->SetPointError(2,0.05,0.05,0.135,0.16875);
   grae->SetPoint(3,0.35,1.038125);
   grae->SetPointError(3,0.05,0.05,0.135,0.135);
   grae->SetPoint(4,0.45,1.038125);
   grae->SetPointError(4,0.05,0.05,0.2025,0.10125);
   grae->SetPoint(5,0.55,1.038125);
   grae->SetPointError(5,0.05,0.05,0.16875,0.135);
   grae->SetPoint(6,0.65,1.004375);
   grae->SetPointError(6,0.05,0.05,0.16065,0.135);
   grae->SetPoint(7,0.75,1.105625);
   grae->SetPointError(7,0.05,0.05,0.13905,0.0675);
   
   TH1F *Graph_Graph16 = new TH1F("Graph_Graph16","Relative S2 Signal vs R^{2}",100,0,0.88);
   Graph_Graph16->SetMinimum(0.65);
   Graph_Graph16->SetMaximum(2);
   Graph_Graph16->SetDirectory(0);
   Graph_Graph16->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph16->SetLineColor(ci);
   Graph_Graph16->GetXaxis()->SetTitle("R^{2}");
   Graph_Graph16->GetXaxis()->SetLabelFont(42);
   Graph_Graph16->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph16->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph16->GetXaxis()->SetTitleFont(42);
   Graph_Graph16->GetYaxis()->SetTitle("Relative S2 Signal");
   Graph_Graph16->GetYaxis()->SetLabelFont(42);
   Graph_Graph16->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph16->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph16->GetYaxis()->SetTitleFont(42);
   Graph_Graph16->GetZaxis()->SetLabelFont(42);
   Graph_Graph16->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph16->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph16->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph16);
   
   grae->Draw("ap");
   
   TF1 *f_s2_r_0.345000 = new TF1("f_s2_r_0.345000","pol1",0,1);
   f_s2_r_0.345000->SetFillColor(19);
   f_s2_r_0.345000->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   f_s2_r_0.345000->SetLineColor(ci);
   f_s2_r_0.345000->SetLineWidth(2);
   f_s2_r_0.345000->SetChisquare(0.5094182);
   f_s2_r_0.345000->SetNDF(6);
   f_s2_r_0.345000->GetXaxis()->SetLabelFont(42);
   f_s2_r_0.345000->GetXaxis()->SetLabelSize(0.035);
   f_s2_r_0.345000->GetXaxis()->SetTitleSize(0.035);
   f_s2_r_0.345000->GetXaxis()->SetTitleFont(42);
   f_s2_r_0.345000->GetYaxis()->SetLabelFont(42);
   f_s2_r_0.345000->GetYaxis()->SetLabelSize(0.035);
   f_s2_r_0.345000->GetYaxis()->SetTitleSize(0.035);
   f_s2_r_0.345000->GetYaxis()->SetTitleFont(42);
   f_s2_r_0.345000->SetParameter(0,0.9099699);
   f_s2_r_0.345000->SetParError(0,0.1053664);
   f_s2_r_0.345000->SetParLimits(0,0,0);
   f_s2_r_0.345000->SetParameter(1,0.2337504);
   f_s2_r_0.345000->SetParError(1,0.2241751);
   f_s2_r_0.345000->SetParLimits(1,0,0);
   f_s2_r_0.345000->Draw("same");
   
   pt = new TPaveText(0.15,0.85,0.7,0.9,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 0.345kV: intercept = 9.10e-01 +/- 1.05e-01, slope = 2.34e-01 +/- 2.24e-01");
   pt->Draw();
   
   pt = new TPaveText(0.2602546,0.9250292,0.7397454,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("Relative S2 Signal vs R^{2}");
   pt->Draw();
   
   grae = new TGraphAsymmErrors(8);
   grae->SetName("Graph1");
   grae->SetTitle("Relative S2 Signal vs R^{2}");
   grae->SetFillColor(1);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetMarkerSize(0);
   grae->SetPoint(0,0.05,0.936875);
   grae->SetPointError(0,0.05,0.05,0.0675,0.10125);
   grae->SetPoint(1,0.15,0.970625);
   grae->SetPointError(1,0.05,0.05,0.13095,0.0675);
   grae->SetPoint(2,0.25,1.004375);
   grae->SetPointError(2,0.05,0.05,0.10125,0.10125);
   grae->SetPoint(3,0.35,1.004375);
   grae->SetPointError(3,0.05,0.05,0.10125,0.10125);
   grae->SetPoint(4,0.45,1.004375);
   grae->SetPointError(4,0.05,0.05,0.10125,0.10125);
   grae->SetPoint(5,0.55,1.004375);
   grae->SetPointError(5,0.05,0.05,0.10125,0.10125);
   grae->SetPoint(6,0.65,1.004375);
   grae->SetPointError(6,0.05,0.05,0.0972,0.10125);
   grae->SetPoint(7,0.75,1.038125);
   grae->SetPointError(7,0.05,0.05,0.10125,0.135);
   
   TH1F *Graph_Graph17 = new TH1F("Graph_Graph17","Relative S2 Signal vs R^{2}",100,0,0.88);
   Graph_Graph17->SetMinimum(0.65);
   Graph_Graph17->SetMaximum(2);
   Graph_Graph17->SetDirectory(0);
   Graph_Graph17->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph17->SetLineColor(ci);
   Graph_Graph17->GetXaxis()->SetTitle("R^{2}");
   Graph_Graph17->GetXaxis()->SetLabelFont(42);
   Graph_Graph17->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph17->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph17->GetXaxis()->SetTitleFont(42);
   Graph_Graph17->GetYaxis()->SetTitle("Relative S2 Signal");
   Graph_Graph17->GetYaxis()->SetLabelFont(42);
   Graph_Graph17->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph17->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph17->GetYaxis()->SetTitleFont(42);
   Graph_Graph17->GetZaxis()->SetLabelFont(42);
   Graph_Graph17->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph17->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph17->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph17);
   
   grae->Draw(" p");
   
   TF1 *f_s2_r_0.700000 = new TF1("f_s2_r_0.700000","pol1",0,1);
   f_s2_r_0.700000->SetFillColor(19);
   f_s2_r_0.700000->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_s2_r_0.700000->SetLineColor(ci);
   f_s2_r_0.700000->SetLineWidth(2);
   f_s2_r_0.700000->SetChisquare(0.1635552);
   f_s2_r_0.700000->SetNDF(6);
   f_s2_r_0.700000->GetXaxis()->SetLabelFont(42);
   f_s2_r_0.700000->GetXaxis()->SetLabelSize(0.035);
   f_s2_r_0.700000->GetXaxis()->SetTitleSize(0.035);
   f_s2_r_0.700000->GetXaxis()->SetTitleFont(42);
   f_s2_r_0.700000->GetYaxis()->SetLabelFont(42);
   f_s2_r_0.700000->GetYaxis()->SetLabelSize(0.035);
   f_s2_r_0.700000->GetYaxis()->SetTitleSize(0.035);
   f_s2_r_0.700000->GetYaxis()->SetTitleFont(42);
   f_s2_r_0.700000->SetParameter(0,0.9539752);
   f_s2_r_0.700000->SetParError(0,0.0762663);
   f_s2_r_0.700000->SetParLimits(0,0,0);
   f_s2_r_0.700000->SetParameter(1,0.1047888);
   f_s2_r_0.700000->SetParError(1,0.1616894);
   f_s2_r_0.700000->SetParLimits(1,0,0);
   f_s2_r_0.700000->Draw("same");
   
   pt = new TPaveText(0.15,0.8,0.7,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 0.7kV: intercept = 9.54e-01 +/- 7.63e-02, slope = 1.05e-01 +/- 1.62e-01");
   pt->Draw();
   
   grae = new TGraphAsymmErrors(8);
   grae->SetName("Graph2");
   grae->SetTitle("Relative S2 Signal vs R^{2}");
   grae->SetFillColor(1);

   ci = TColor::GetColor("#00cc00");
   grae->SetLineColor(ci);
   grae->SetMarkerSize(0);
   grae->SetPoint(0,0.05,0.970625);
   grae->SetPointError(0,0.05,0.05,0.10125,0.0675);
   grae->SetPoint(1,0.15,0.970625);
   grae->SetPointError(1,0.05,0.05,0.0675,0.0675);
   grae->SetPoint(2,0.25,1.004375);
   grae->SetPointError(2,0.05,0.05,0.10125,0.0675);
   grae->SetPoint(3,0.35,1.004375);
   grae->SetPointError(3,0.05,0.05,0.10125,0.0675);
   grae->SetPoint(4,0.45,1.004375);
   grae->SetPointError(4,0.05,0.05,0.10125,0.10125);
   grae->SetPoint(5,0.55,1.004375);
   grae->SetPointError(5,0.05,0.05,0.0675,0.10125);
   grae->SetPoint(6,0.65,1.038125);
   grae->SetPointError(6,0.05,0.05,0.10125,0.0675);
   grae->SetPoint(7,0.75,1.038125);
   grae->SetPointError(7,0.05,0.05,0.1863,0.0594);
   
   TH1F *Graph_Graph18 = new TH1F("Graph_Graph18","Relative S2 Signal vs R^{2}",100,0,0.88);
   Graph_Graph18->SetMinimum(0.65);
   Graph_Graph18->SetMaximum(2);
   Graph_Graph18->SetDirectory(0);
   Graph_Graph18->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph18->SetLineColor(ci);
   Graph_Graph18->GetXaxis()->SetTitle("R^{2}");
   Graph_Graph18->GetXaxis()->SetLabelFont(42);
   Graph_Graph18->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph18->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph18->GetXaxis()->SetTitleFont(42);
   Graph_Graph18->GetYaxis()->SetTitle("Relative S2 Signal");
   Graph_Graph18->GetYaxis()->SetLabelFont(42);
   Graph_Graph18->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph18->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph18->GetYaxis()->SetTitleFont(42);
   Graph_Graph18->GetZaxis()->SetLabelFont(42);
   Graph_Graph18->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph18->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph18->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph18);
   
   grae->Draw(" p");
   
   TF1 *f_s2_r_1.054000 = new TF1("f_s2_r_1.054000","pol1",0,1);
   f_s2_r_1.054000->SetFillColor(19);
   f_s2_r_1.054000->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   f_s2_r_1.054000->SetLineColor(ci);
   f_s2_r_1.054000->SetLineWidth(2);
   f_s2_r_1.054000->SetChisquare(0.07178236);
   f_s2_r_1.054000->SetNDF(6);
   f_s2_r_1.054000->GetXaxis()->SetLabelFont(42);
   f_s2_r_1.054000->GetXaxis()->SetLabelSize(0.035);
   f_s2_r_1.054000->GetXaxis()->SetTitleSize(0.035);
   f_s2_r_1.054000->GetXaxis()->SetTitleFont(42);
   f_s2_r_1.054000->GetYaxis()->SetLabelFont(42);
   f_s2_r_1.054000->GetYaxis()->SetLabelSize(0.035);
   f_s2_r_1.054000->GetYaxis()->SetTitleSize(0.035);
   f_s2_r_1.054000->GetYaxis()->SetTitleFont(42);
   f_s2_r_1.054000->SetParameter(0,0.9626866);
   f_s2_r_1.054000->SetParError(0,0.05987682);
   f_s2_r_1.054000->SetParLimits(0,0,0);
   f_s2_r_1.054000->SetParameter(1,0.1011569);
   f_s2_r_1.054000->SetParError(1,0.1195884);
   f_s2_r_1.054000->SetParLimits(1,0,0);
   f_s2_r_1.054000->Draw("same");
   
   pt = new TPaveText(0.15,0.75,0.7,0.8,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.054kV: intercept = 9.63e-01 +/- 5.99e-02, slope = 1.01e-01 +/- 1.20e-01");
   pt->Draw();
   
   grae = new TGraphAsymmErrors(8);
   grae->SetName("Graph3");
   grae->SetTitle("Relative S2 Signal vs R^{2}");
   grae->SetFillColor(1);

   ci = TColor::GetColor("#ff9933");
   grae->SetLineColor(ci);
   grae->SetMarkerSize(0);
   grae->SetPoint(0,0.05,0.970625);
   grae->SetPointError(0,0.05,0.05,0.0675,0.10125);
   grae->SetPoint(1,0.15,0.970625);
   grae->SetPointError(1,0.05,0.05,0.0675,0.0675);
   grae->SetPoint(2,0.25,1.004375);
   grae->SetPointError(2,0.05,0.05,0.10125,0.0675);
   grae->SetPoint(3,0.35,1.004375);
   grae->SetPointError(3,0.05,0.05,0.0675,0.0675);
   grae->SetPoint(4,0.45,1.004375);
   grae->SetPointError(4,0.05,0.05,0.0675,0.0675);
   grae->SetPoint(5,0.55,1.004375);
   grae->SetPointError(5,0.05,0.05,0.0675,0.0675);
   grae->SetPoint(6,0.65,1.004375);
   grae->SetPointError(6,0.05,0.05,0.0675,0.10125);
   grae->SetPoint(7,0.75,1.038125);
   grae->SetPointError(7,0.05,0.05,0.03375,0.0378);
   
   TH1F *Graph_Graph19 = new TH1F("Graph_Graph19","Relative S2 Signal vs R^{2}",100,0,0.88);
   Graph_Graph19->SetMinimum(0.65);
   Graph_Graph19->SetMaximum(2);
   Graph_Graph19->SetDirectory(0);
   Graph_Graph19->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph19->SetLineColor(ci);
   Graph_Graph19->GetXaxis()->SetTitle("R^{2}");
   Graph_Graph19->GetXaxis()->SetLabelFont(42);
   Graph_Graph19->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph19->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph19->GetXaxis()->SetTitleFont(42);
   Graph_Graph19->GetYaxis()->SetTitle("Relative S2 Signal");
   Graph_Graph19->GetYaxis()->SetLabelFont(42);
   Graph_Graph19->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph19->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph19->GetYaxis()->SetTitleFont(42);
   Graph_Graph19->GetZaxis()->SetLabelFont(42);
   Graph_Graph19->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph19->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph19->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph19);
   
   grae->Draw(" p");
   
   TF1 *f_s2_r_1.500000 = new TF1("f_s2_r_1.500000","pol1",0,1);
   f_s2_r_1.500000->SetFillColor(19);
   f_s2_r_1.500000->SetFillStyle(0);

   ci = TColor::GetColor("#ff9933");
   f_s2_r_1.500000->SetLineColor(ci);
   f_s2_r_1.500000->SetLineWidth(2);
   f_s2_r_1.500000->SetChisquare(0.1347655);
   f_s2_r_1.500000->SetNDF(6);
   f_s2_r_1.500000->GetXaxis()->SetLabelFont(42);
   f_s2_r_1.500000->GetXaxis()->SetLabelSize(0.035);
   f_s2_r_1.500000->GetXaxis()->SetTitleSize(0.035);
   f_s2_r_1.500000->GetXaxis()->SetTitleFont(42);
   f_s2_r_1.500000->GetYaxis()->SetLabelFont(42);
   f_s2_r_1.500000->GetYaxis()->SetLabelSize(0.035);
   f_s2_r_1.500000->GetYaxis()->SetTitleSize(0.035);
   f_s2_r_1.500000->GetYaxis()->SetTitleFont(42);
   f_s2_r_1.500000->SetParameter(0,0.9637728);
   f_s2_r_1.500000->SetParError(0,0.0474744);
   f_s2_r_1.500000->SetParLimits(0,0,0);
   f_s2_r_1.500000->SetParameter(1,0.09473156);
   f_s2_r_1.500000->SetParError(1,0.08466019);
   f_s2_r_1.500000->SetParLimits(1,0,0);
   f_s2_r_1.500000->Draw("same");
   
   pt = new TPaveText(0.15,0.7,0.7,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff9933");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.5kV: intercept = 9.64e-01 +/- 4.75e-02, slope = 9.47e-02 +/- 8.47e-02");
   pt->Draw();
   
   grae = new TGraphAsymmErrors(8);
   grae->SetName("Graph4");
   grae->SetTitle("Relative S2 Signal vs R^{2}");
   grae->SetFillColor(1);

   ci = TColor::GetColor("#ff0000");
   grae->SetLineColor(ci);
   grae->SetMarkerSize(0);
   grae->SetPoint(0,0.05,0.970625);
   grae->SetPointError(0,0.05,0.05,0.03375,0.0675);
   grae->SetPoint(1,0.15,1.004375);
   grae->SetPointError(1,0.05,0.05,0.0675,0.0675);
   grae->SetPoint(2,0.25,1.004375);
   grae->SetPointError(2,0.05,0.05,0.0675,0.0675);
   grae->SetPoint(3,0.35,1.004375);
   grae->SetPointError(3,0.05,0.05,0.0675,0.0675);
   grae->SetPoint(4,0.45,1.004375);
   grae->SetPointError(4,0.05,0.05,0.0675,0.0675);
   grae->SetPoint(5,0.55,1.004375);
   grae->SetPointError(5,0.05,0.05,0.0675,0.0675);
   grae->SetPoint(6,0.65,1.004375);
   grae->SetPointError(6,0.05,0.05,0.0675,0.0675);
   grae->SetPoint(7,0.75,1.004375);
   grae->SetPointError(7,0.05,0.05,0.03645,0.03375);
   
   TH1F *Graph_Graph20 = new TH1F("Graph_Graph20","Relative S2 Signal vs R^{2}",100,0,0.88);
   Graph_Graph20->SetMinimum(0.65);
   Graph_Graph20->SetMaximum(2);
   Graph_Graph20->SetDirectory(0);
   Graph_Graph20->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph20->SetLineColor(ci);
   Graph_Graph20->GetXaxis()->SetTitle("R^{2}");
   Graph_Graph20->GetXaxis()->SetLabelFont(42);
   Graph_Graph20->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph20->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph20->GetXaxis()->SetTitleFont(42);
   Graph_Graph20->GetYaxis()->SetTitle("Relative S2 Signal");
   Graph_Graph20->GetYaxis()->SetLabelFont(42);
   Graph_Graph20->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph20->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph20->GetYaxis()->SetTitleFont(42);
   Graph_Graph20->GetZaxis()->SetLabelFont(42);
   Graph_Graph20->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph20->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph20->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph20);
   
   grae->Draw(" p");
   
   TF1 *f_s2_r_2.356000 = new TF1("f_s2_r_2.356000","pol1",0,1);
   f_s2_r_2.356000->SetFillColor(19);
   f_s2_r_2.356000->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   f_s2_r_2.356000->SetLineColor(ci);
   f_s2_r_2.356000->SetLineWidth(2);
   f_s2_r_2.356000->SetChisquare(0.1550479);
   f_s2_r_2.356000->SetNDF(6);
   f_s2_r_2.356000->GetXaxis()->SetLabelFont(42);
   f_s2_r_2.356000->GetXaxis()->SetLabelSize(0.035);
   f_s2_r_2.356000->GetXaxis()->SetTitleSize(0.035);
   f_s2_r_2.356000->GetXaxis()->SetTitleFont(42);
   f_s2_r_2.356000->GetYaxis()->SetLabelFont(42);
   f_s2_r_2.356000->GetYaxis()->SetLabelSize(0.035);
   f_s2_r_2.356000->GetYaxis()->SetTitleSize(0.035);
   f_s2_r_2.356000->GetYaxis()->SetTitleFont(42);
   f_s2_r_2.356000->SetParameter(0,0.990454);
   f_s2_r_2.356000->SetParError(0,0.04520707);
   f_s2_r_2.356000->SetParLimits(0,0,0);
   f_s2_r_2.356000->SetParameter(1,0.0218807);
   f_s2_r_2.356000->SetParError(1,0.08149117);
   f_s2_r_2.356000->SetParLimits(1,0,0);
   f_s2_r_2.356000->Draw("same");
   
   pt = new TPaveText(0.15,0.65,0.7,0.7,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 2.356kV: intercept = 9.90e-01 +/- 4.52e-02, slope = 2.19e-02 +/- 8.15e-02");
   pt->Draw();
   64b2efc9fff0444db5888d08a28935a3_2->Modified();
   64b2efc9fff0444db5888d08a28935a3->cd();
   64b2efc9fff0444db5888d08a28935a3->Modified();
   64b2efc9fff0444db5888d08a28935a3->cd();
   64b2efc9fff0444db5888d08a28935a3->SetSelected(64b2efc9fff0444db5888d08a28935a3);
}
