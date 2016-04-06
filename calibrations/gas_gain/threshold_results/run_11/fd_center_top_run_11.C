{
//=========Macro generated from canvas: b3b1cb262643412abd334fabc6729d4f/
//=========  (Wed Apr  6 11:35:03 2016) by ROOT version5.34/26
   TCanvas *b3b1cb262643412abd334fabc6729d4f = new TCanvas("b3b1cb262643412abd334fabc6729d4f", "",927,47,704,527);
   b3b1cb262643412abd334fabc6729d4f->SetHighLightColor(2);
   b3b1cb262643412abd334fabc6729d4f->Range(1.416149e+09,-1.498227,1.425076e+09,7.479067);
   b3b1cb262643412abd334fabc6729d4f->SetFillColor(0);
   b3b1cb262643412abd334fabc6729d4f->SetBorderMode(0);
   b3b1cb262643412abd334fabc6729d4f->SetBorderSize(2);
   b3b1cb262643412abd334fabc6729d4f->SetFrameBorderMode(0);
   b3b1cb262643412abd334fabc6729d4f->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(15);
   gre->SetName("Graph0");
   gre->SetTitle("Fermi Dirac Center in Time (top) - run 11");
   gre->SetFillColor(1);
   gre->SetPoint(0,1.420566e+09,2.98549);
   gre->SetPointError(0,0,0.1721928);
   gre->SetPoint(1,1.418225e+09,3.00385);
   gre->SetPointError(1,0,0.08524018);
   gre->SetPoint(2,1.422475e+09,3);
   gre->SetPointError(2,0,0.07309818);
   gre->SetPoint(3,1.418056e+09,3.00663);
   gre->SetPointError(3,0,0.1083071);
   gre->SetPoint(4,1.422464e+09,2.9722);
   gre->SetPointError(4,0,0.07273275);
   gre->SetPoint(5,1.417637e+09,3.83559);
   gre->SetPointError(5,0,0.4020874);
   gre->SetPoint(6,1.423588e+09,2.98382);
   gre->SetPointError(6,0,2.98384);
   gre->SetPoint(7,1.420571e+09,3.0074);
   gre->SetPointError(7,0,0.1037217);
   gre->SetPoint(8,1.421262e+09,2.99984);
   gre->SetPointError(8,0,0.1005607);
   gre->SetPoint(9,1.42307e+09,2.99042);
   gre->SetPointError(9,0,2.992431);
   gre->SetPoint(10,1.42056e+09,2.9995);
   gre->SetPointError(10,0,0.1031735);
   gre->SetPoint(11,1.418831e+09,3.05995);
   gre->SetPointError(11,0,0.9062622);
   gre->SetPoint(12,1.42247e+09,0);
   gre->SetPointError(12,0,0);
   gre->SetPoint(13,1.420562e+09,2.99935);
   gre->SetPointError(13,0,0.1028212);
   gre->SetPoint(14,1.42248e+09,3.00189);
   gre->SetPointError(14,0,0.2224319);
   
   TH1F *Graph_Graph4 = new TH1F("Graph_Graph4","Fermi Dirac Center in Time (top) - run 11",100,1.417042e+09,1.424183e+09);
   Graph_Graph4->SetMinimum(-0.6004977);
   Graph_Graph4->SetMaximum(6.581338);
   Graph_Graph4->SetDirectory(0);
   Graph_Graph4->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph4->SetLineColor(ci);
   Graph_Graph4->GetXaxis()->SetTitle("Time");
   Graph_Graph4->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph4->GetXaxis()->SetTimeFormat("%m/%d%F1995-01-01 00:00:00s0");
   Graph_Graph4->GetXaxis()->SetLabelFont(42);
   Graph_Graph4->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph4->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph4->GetXaxis()->SetTitleFont(42);
   Graph_Graph4->GetYaxis()->SetTitle("FD Center (top) [PE]");
   Graph_Graph4->GetYaxis()->SetLabelFont(42);
   Graph_Graph4->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph4->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph4->GetYaxis()->SetTitleFont(42);
   Graph_Graph4->GetZaxis()->SetLabelFont(42);
   Graph_Graph4->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph4->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph4->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph4);
   
   
   TF1 *fG4 = new TF1("fG4","pol0",1.417042e+09,1.424183e+09);
   fG4->SetFillColor(19);
   fG4->SetFillStyle(0);
   fG4->SetLineColor(2);
   fG4->SetLineWidth(1);
   fG4->SetLineStyle(9);
   fG4->SetChisquare(4.485889);
   fG4->SetNDF(13);
   fG4->GetXaxis()->SetLabelFont(42);
   fG4->GetXaxis()->SetLabelSize(0.035);
   fG4->GetXaxis()->SetTitleSize(0.035);
   fG4->GetXaxis()->SetTitleFont(42);
   fG4->GetYaxis()->SetLabelFont(42);
   fG4->GetYaxis()->SetLabelSize(0.035);
   fG4->GetYaxis()->SetTitleSize(0.035);
   fG4->GetYaxis()->SetTitleFont(42);
   fG4->SetParameter(0,3.001144);
   fG4->SetParError(0,0.03099384);
   fG4->SetParLimits(0,0,0);
   gre->GetListOfFunctions()->Add(fG4);
   gre->Draw("ap");
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(15);
   grae->SetName("Graph1");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#0000ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3005);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.420566e+09,3.001144);
   grae->SetPointError(0,0,0,0.03933289,0.03933289);
   grae->SetPoint(1,1.418225e+09,3.001144);
   grae->SetPointError(1,0,0,0.03933289,0.03933289);
   grae->SetPoint(2,1.422475e+09,3.001144);
   grae->SetPointError(2,0,0,0.03933289,0.03933289);
   grae->SetPoint(3,1.418056e+09,3.001144);
   grae->SetPointError(3,0,0,0.03933289,0.03933289);
   grae->SetPoint(4,1.422464e+09,3.001144);
   grae->SetPointError(4,0,0,0.03933289,0.03933289);
   grae->SetPoint(5,1.417637e+09,3.001144);
   grae->SetPointError(5,0,0,0.03933289,0.03933289);
   grae->SetPoint(6,1.423588e+09,3.001144);
   grae->SetPointError(6,0,0,0.03933289,0.03933289);
   grae->SetPoint(7,1.420571e+09,3.001144);
   grae->SetPointError(7,0,0,0.03933289,0.03933289);
   grae->SetPoint(8,1.421262e+09,3.001144);
   grae->SetPointError(8,0,0,0.03933289,0.03933289);
   grae->SetPoint(9,1.42307e+09,3.001144);
   grae->SetPointError(9,0,0,0.03933289,0.03933289);
   grae->SetPoint(10,1.42056e+09,3.001144);
   grae->SetPointError(10,0,0,0.03933289,0.03933289);
   grae->SetPoint(11,1.418831e+09,3.001144);
   grae->SetPointError(11,0,0,0.03933289,0.03933289);
   grae->SetPoint(12,1.42247e+09,3.001144);
   grae->SetPointError(12,0,0,0.03933289,0.03933289);
   grae->SetPoint(13,1.420562e+09,3.001144);
   grae->SetPointError(13,0,0,0.03933289,0.03933289);
   grae->SetPoint(14,1.42248e+09,3.001144);
   grae->SetPointError(14,0,0,0.03933289,0.03933289);
   
   TH1F *Graph_Graph4 = new TH1F("Graph_Graph4","Graph",100,1.417042e+09,1.424183e+09);
   Graph_Graph4->SetMinimum(2.953944);
   Graph_Graph4->SetMaximum(3.048343);
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
   grae->SetHistogram(Graph_Graph4);
   
   grae->Draw("3 ");
   
   TPaveText *pt = new TPaveText(0.15,0.9374,0.85,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Fermi Dirac Center in Time (top) - run 11");
   pt->Draw();
   b3b1cb262643412abd334fabc6729d4f->Modified();
   b3b1cb262643412abd334fabc6729d4f->cd();
   b3b1cb262643412abd334fabc6729d4f->SetSelected(b3b1cb262643412abd334fabc6729d4f);
}
