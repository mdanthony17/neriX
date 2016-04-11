{
//=========Macro generated from canvas: 89f6ce536b944fa59da717a1317511f0/
//=========  (Wed Apr  6 13:53:14 2016) by ROOT version5.34/26
   TCanvas *89f6ce536b944fa59da717a1317511f0 = new TCanvas("89f6ce536b944fa59da717a1317511f0", "",882,97,704,527);
   89f6ce536b944fa59da717a1317511f0->SetHighLightColor(2);
   89f6ce536b944fa59da717a1317511f0->Range(1.416149e+09,-0.9807896,1.425076e+09,4.901336);
   89f6ce536b944fa59da717a1317511f0->SetFillColor(0);
   89f6ce536b944fa59da717a1317511f0->SetBorderMode(0);
   89f6ce536b944fa59da717a1317511f0->SetBorderSize(2);
   89f6ce536b944fa59da717a1317511f0->SetFrameBorderMode(0);
   89f6ce536b944fa59da717a1317511f0->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(15);
   gre->SetName("Graph0");
   gre->SetTitle("Fermi Dirac Center in Time (top) - run 11");
   gre->SetFillColor(1);
   gre->SetPoint(0,1.420566e+09,1.00494);
   gre->SetPointError(0,0,0.03422534);
   gre->SetPoint(1,1.418225e+09,2.7703);
   gre->SetPointError(1,0,0.4946714);
   gre->SetPoint(2,1.422475e+09,1.50668);
   gre->SetPointError(2,0,0.1616796);
   gre->SetPoint(3,1.418056e+09,3.14524);
   gre->SetPointError(3,0,0.5781985);
   gre->SetPoint(4,1.422464e+09,1.07881);
   gre->SetPointError(4,0,0.02746631);
   gre->SetPoint(5,1.417637e+09,3.35344);
   gre->SetPointError(5,0,0.5675414);
   gre->SetPoint(6,1.423588e+09,1.05286);
   gre->SetPointError(6,0,1.053295);
   gre->SetPoint(7,1.420571e+09,1.55043);
   gre->SetPointError(7,0,0.1161971);
   gre->SetPoint(8,1.421262e+09,1.08313);
   gre->SetPointError(8,0,0.589378);
   gre->SetPoint(9,1.42307e+09,1.00213);
   gre->SetPointError(9,0,1.002142);
   gre->SetPoint(10,1.42056e+09,0);
   gre->SetPointError(10,0,0);
   gre->SetPoint(11,1.418831e+09,2.70828);
   gre->SetPointError(11,0,0.3858467);
   gre->SetPoint(12,1.42247e+09,1.17579);
   gre->SetPointError(12,0,1.083175);
   gre->SetPoint(13,1.420562e+09,1.00014);
   gre->SetPointError(13,0,0.03421992);
   gre->SetPoint(14,1.42248e+09,1.457);
   gre->SetPointError(14,0,0.1708637);
   
   TH1F *Graph_Graph4 = new TH1F("Graph_Graph4","Fermi Dirac Center in Time (top) - run 11",100,1.417042e+09,1.424183e+09);
   Graph_Graph4->SetMinimum(-0.392577);
   Graph_Graph4->SetMaximum(4.313123);
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
   fG4->SetChisquare(96.31717);
   fG4->SetNDF(13);
   fG4->GetXaxis()->SetLabelFont(42);
   fG4->GetXaxis()->SetLabelSize(0.035);
   fG4->GetXaxis()->SetTitleSize(0.035);
   fG4->GetXaxis()->SetTitleFont(42);
   fG4->GetYaxis()->SetLabelFont(42);
   fG4->GetYaxis()->SetLabelSize(0.035);
   fG4->GetYaxis()->SetTitleSize(0.035);
   fG4->GetYaxis()->SetTitleFont(42);
   fG4->SetParameter(0,1.067913);
   fG4->SetParError(0,0.01767124);
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
   grae->SetPoint(0,1.420566e+09,1.067913);
   grae->SetPointError(0,0,0,0.1039143,0.1039143);
   grae->SetPoint(1,1.418225e+09,1.067913);
   grae->SetPointError(1,0,0,0.1039143,0.1039143);
   grae->SetPoint(2,1.422475e+09,1.067913);
   grae->SetPointError(2,0,0,0.1039143,0.1039143);
   grae->SetPoint(3,1.418056e+09,1.067913);
   grae->SetPointError(3,0,0,0.1039143,0.1039143);
   grae->SetPoint(4,1.422464e+09,1.067913);
   grae->SetPointError(4,0,0,0.1039143,0.1039143);
   grae->SetPoint(5,1.417637e+09,1.067913);
   grae->SetPointError(5,0,0,0.1039143,0.1039143);
   grae->SetPoint(6,1.423588e+09,1.067913);
   grae->SetPointError(6,0,0,0.1039143,0.1039143);
   grae->SetPoint(7,1.420571e+09,1.067913);
   grae->SetPointError(7,0,0,0.1039143,0.1039143);
   grae->SetPoint(8,1.421262e+09,1.067913);
   grae->SetPointError(8,0,0,0.1039143,0.1039143);
   grae->SetPoint(9,1.42307e+09,1.067913);
   grae->SetPointError(9,0,0,0.1039143,0.1039143);
   grae->SetPoint(10,1.42056e+09,1.067913);
   grae->SetPointError(10,0,0,0.1039143,0.1039143);
   grae->SetPoint(11,1.418831e+09,1.067913);
   grae->SetPointError(11,0,0,0.1039143,0.1039143);
   grae->SetPoint(12,1.42247e+09,1.067913);
   grae->SetPointError(12,0,0,0.1039143,0.1039143);
   grae->SetPoint(13,1.420562e+09,1.067913);
   grae->SetPointError(13,0,0,0.1039143,0.1039143);
   grae->SetPoint(14,1.42248e+09,1.067913);
   grae->SetPointError(14,0,0,0.1039143,0.1039143);
   
   TH1F *Graph_Graph4 = new TH1F("Graph_Graph4","Graph",100,1.417042e+09,1.424183e+09);
   Graph_Graph4->SetMinimum(0.9432158);
   Graph_Graph4->SetMaximum(1.19261);
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
   89f6ce536b944fa59da717a1317511f0->Modified();
   89f6ce536b944fa59da717a1317511f0->cd();
   89f6ce536b944fa59da717a1317511f0->SetSelected(89f6ce536b944fa59da717a1317511f0);
}
