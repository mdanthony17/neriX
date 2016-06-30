{
//=========Macro generated from canvas: b46c31c433834290bf3bc37379fc76d7/
//=========  (Thu May 26 12:43:56 2016) by ROOT version5.34/26
   TCanvas *b46c31c433834290bf3bc37379fc76d7 = new TCanvas("b46c31c433834290bf3bc37379fc76d7", "",1996,99,704,527);
   b46c31c433834290bf3bc37379fc76d7->SetHighLightColor(2);
   b46c31c433834290bf3bc37379fc76d7->Range(1.416657e+09,13.375,1.42346e+09,19.625);
   b46c31c433834290bf3bc37379fc76d7->SetFillColor(0);
   b46c31c433834290bf3bc37379fc76d7->SetBorderMode(0);
   b46c31c433834290bf3bc37379fc76d7->SetBorderSize(2);
   b46c31c433834290bf3bc37379fc76d7->SetFrameBorderMode(0);
   b46c31c433834290bf3bc37379fc76d7->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(2);
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
   gre->SetPoint(0,1.42056e+09,17.35);
   gre->SetPointError(0,0,0.61);
   gre->SetPoint(1,1.422464e+09,17.3);
   gre->SetPointError(1,0,0.44);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","Gas Gain Run Average, Method Average - 0.0 mm < R < 18.0 mm",100,1.417337e+09,1.42278e+09);
   Graph_Graph1->SetMinimum(14);
   Graph_Graph1->SetMaximum(19);
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
   grae->SetPoint(0,1.417337e+09,17.31711);
   grae->SetPointError(0,0,0,0.3568531,0.3568531);
   grae->SetPoint(1,1.42278e+09,17.31711);
   grae->SetPointError(1,0,0,0.3568531,0.3568531);
   
   TH1F *Graph_conf_0.345_(0.0, 18.0)1 = new TH1F("Graph_conf_0.345_(0.0, 18.0)1","Graph",100,1.416793e+09,1.423324e+09);
   Graph_conf_0.345_(0.0, 18.0)1->SetMinimum(16.88889);
   Graph_conf_0.345_(0.0, 18.0)1->SetMaximum(17.74534);
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
   
   gre = new TGraphErrors(2);
   gre->SetName("Graph1");
   gre->SetTitle("Gas Gain Run Average, Method Average - 0.0 mm < R < 18.0 mm");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.417637e+09,16.35);
   gre->SetPointError(0,0,0.37);
   gre->SetPoint(1,1.421262e+09,17.08);
   gre->SetPointError(1,0,0.6);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Gas Gain Run Average, Method Average - 0.0 mm < R < 18.0 mm",100,1.417337e+09,1.42278e+09);
   Graph_Graph2->SetMinimum(14);
   Graph_Graph2->SetMaximum(19);
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
   grae->SetPoint(0,1.417337e+09,16.55112);
   grae->SetPointError(0,0,0,0.3149332,0.3149332);
   grae->SetPoint(1,1.42278e+09,16.55112);
   grae->SetPointError(1,0,0,0.3149332,0.3149332);
   
   TH1F *Graph_conf_1.054_(0.0, 18.0)2 = new TH1F("Graph_conf_1.054_(0.0, 18.0)2","Graph",100,1.416793e+09,1.423324e+09);
   Graph_conf_1.054_(0.0, 18.0)2->SetMinimum(16.1732);
   Graph_conf_1.054_(0.0, 18.0)2->SetMaximum(16.92904);
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
   
   gre = new TGraphErrors(2);
   gre->SetName("Graph2");
   gre->SetTitle("Gas Gain Run Average, Method Average - 0.0 mm < R < 18.0 mm");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#00ff00");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#00ff00");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.418831e+09,15.18);
   gre->SetPointError(0,0,0.5);
   gre->SetPoint(1,1.422475e+09,17.55);
   gre->SetPointError(1,0,0.45);
   
   TH1F *Graph_Graph3 = new TH1F("Graph_Graph3","Gas Gain Run Average, Method Average - 0.0 mm < R < 18.0 mm",100,1.417337e+09,1.42278e+09);
   Graph_Graph3->SetMinimum(14);
   Graph_Graph3->SetMaximum(19);
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
   grae->SetPoint(0,1.417337e+09,16.48939);
   grae->SetPointError(0,0,0,0.3344824,0.3344824);
   grae->SetPoint(1,1.42278e+09,16.48939);
   grae->SetPointError(1,0,0,0.3344824,0.3344824);
   
   TH1F *Graph_conf_2.356_(0.0, 18.0)3 = new TH1F("Graph_conf_2.356_(0.0, 18.0)3","Graph",100,1.416793e+09,1.423324e+09);
   Graph_conf_2.356_(0.0, 18.0)3->SetMinimum(16.08801);
   Graph_conf_2.356_(0.0, 18.0)3->SetMaximum(16.89077);
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
   
   gre = new TGraphErrors(2);
   gre->SetName("Graph3");
   gre->SetTitle("Gas Gain Run Average, Method Average - 0.0 mm < R < 18.0 mm");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff0000");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff0000");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.418047e+09,15.9125);
   gre->SetPointError(0,0,0.64);
   gre->SetPoint(1,1.422479e+09,17.2);
   gre->SetPointError(1,0,0.47);
   
   TH1F *Graph_Graph4 = new TH1F("Graph_Graph4","Gas Gain Run Average, Method Average - 0.0 mm < R < 18.0 mm",100,1.417337e+09,1.42278e+09);
   Graph_Graph4->SetMinimum(14);
   Graph_Graph4->SetMaximum(19);
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
   grae->SetPoint(0,1.417337e+09,16.74804);
   grae->SetPointError(0,0,0,0.3788221,0.3788221);
   grae->SetPoint(1,1.42278e+09,16.74804);
   grae->SetPointError(1,0,0,0.3788221,0.3788221);
   
   TH1F *Graph_conf_5.5_(0.0, 18.0)4 = new TH1F("Graph_conf_5.5_(0.0, 18.0)4","Graph",100,1.416793e+09,1.423324e+09);
   Graph_conf_5.5_(0.0, 18.0)4->SetMinimum(16.29345);
   Graph_conf_5.5_(0.0, 18.0)4->SetMaximum(17.20263);
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
   text = pt->AddText("V_{c} = 0.345kV: 17.32 +/- 0.36 PE");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.75,0.5,0.8,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.054kV: 16.55 +/- 0.31 PE");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.7,0.5,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#00ff00");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 2.356kV: 16.49 +/- 0.33 PE");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.65,0.5,0.7,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 5.5kV: 16.75 +/- 0.38 PE");
   pt->Draw();
   b46c31c433834290bf3bc37379fc76d7->Modified();
   b46c31c433834290bf3bc37379fc76d7->cd();
   b46c31c433834290bf3bc37379fc76d7->SetSelected(b46c31c433834290bf3bc37379fc76d7);
}
