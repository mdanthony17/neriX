{
//=========Macro generated from canvas: 5f20f85d7c8e4846869fce909dcd3b1c/
//=========  (Wed Apr  6 11:35:01 2016) by ROOT version5.34/26
   TCanvas *5f20f85d7c8e4846869fce909dcd3b1c = new TCanvas("5f20f85d7c8e4846869fce909dcd3b1c", "",148,124,704,527);
   5f20f85d7c8e4846869fce909dcd3b1c->SetHighLightColor(2);
   5f20f85d7c8e4846869fce909dcd3b1c->Range(1.416149e+09,-1.098392,1.425076e+09,5.472172);
   5f20f85d7c8e4846869fce909dcd3b1c->SetFillColor(0);
   5f20f85d7c8e4846869fce909dcd3b1c->SetBorderMode(0);
   5f20f85d7c8e4846869fce909dcd3b1c->SetBorderSize(2);
   5f20f85d7c8e4846869fce909dcd3b1c->SetFrameBorderMode(0);
   5f20f85d7c8e4846869fce909dcd3b1c->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(15);
   gre->SetName("Graph0");
   gre->SetTitle("Fermi Dirac Shape in Time (bottom) - run 11");
   gre->SetFillColor(1);
   gre->SetPoint(0,1.420566e+09,2.60071);
   gre->SetPointError(0,0,0.1733508);
   gre->SetPoint(1,1.418225e+09,3.02558);
   gre->SetPointError(1,0,0.2811753);
   gre->SetPoint(2,1.422475e+09,2.88005);
   gre->SetPointError(2,0,0.2097948);
   gre->SetPoint(3,1.418056e+09,2.9089);
   gre->SetPointError(3,0,0.292718);
   gre->SetPoint(4,1.422464e+09,2.99731);
   gre->SetPointError(4,0,0.3027373);
   gre->SetPoint(5,1.417637e+09,3.83249);
   gre->SetPointError(5,0,0.4435689);
   gre->SetPoint(6,1.423588e+09,1.94066);
   gre->SetPointError(6,0,1.9428);
   gre->SetPoint(7,1.420571e+09,2.83941);
   gre->SetPointError(7,0,0.204021);
   gre->SetPoint(8,1.421262e+09,2.83065);
   gre->SetPointError(8,0,0.2516402);
   gre->SetPoint(9,1.42307e+09,2.18689);
   gre->SetPointError(9,0,2.190188);
   gre->SetPoint(10,1.42056e+09,3.20379);
   gre->SetPointError(10,0,0.45779);
   gre->SetPoint(11,1.418831e+09,3.55237);
   gre->SetPointError(11,0,0.338122);
   gre->SetPoint(12,1.42247e+09,2.23724);
   gre->SetPointError(12,0,0.1540973);
   gre->SetPoint(13,1.420562e+09,3.06072);
   gre->SetPointError(13,0,0.2567709);
   gre->SetPoint(14,1.42248e+09,2.64916);
   gre->SetPointError(14,0,0.1142498);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","Fermi Dirac Shape in Time (bottom) - run 11",100,1.417042e+09,1.424183e+09);
   Graph_Graph1->SetMinimum(-0.4413357);
   Graph_Graph1->SetMaximum(4.815116);
   Graph_Graph1->SetDirectory(0);
   Graph_Graph1->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph1->SetLineColor(ci);
   Graph_Graph1->GetXaxis()->SetTitle("Time");
   Graph_Graph1->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph1->GetXaxis()->SetTimeFormat("%m/%d%F1995-01-01 00:00:00s0");
   Graph_Graph1->GetXaxis()->SetLabelFont(42);
   Graph_Graph1->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleFont(42);
   Graph_Graph1->GetYaxis()->SetTitle("FD Shape (bottom) [PE]");
   Graph_Graph1->GetYaxis()->SetRange(1,1);
   Graph_Graph1->GetYaxis()->SetLabelFont(42);
   Graph_Graph1->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleFont(42);
   Graph_Graph1->GetZaxis()->SetLabelFont(42);
   Graph_Graph1->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1);
   
   
   TF1 *fG1 = new TF1("fG1","pol0",1.417042e+09,1.424183e+09);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(29.46581);
   fG1->SetNDF(14);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,2.747163);
   fG1->SetParError(0,0.05933812);
   fG1->SetParLimits(0,0,0);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(15);
   grae->SetName("Graph1");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#0000ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3005);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.420566e+09,2.747163);
   grae->SetPointError(0,0,0,0.1846346,0.1846346);
   grae->SetPoint(1,1.418225e+09,2.747163);
   grae->SetPointError(1,0,0,0.1846346,0.1846346);
   grae->SetPoint(2,1.422475e+09,2.747163);
   grae->SetPointError(2,0,0,0.1846346,0.1846346);
   grae->SetPoint(3,1.418056e+09,2.747163);
   grae->SetPointError(3,0,0,0.1846346,0.1846346);
   grae->SetPoint(4,1.422464e+09,2.747163);
   grae->SetPointError(4,0,0,0.1846346,0.1846346);
   grae->SetPoint(5,1.417637e+09,2.747163);
   grae->SetPointError(5,0,0,0.1846346,0.1846346);
   grae->SetPoint(6,1.423588e+09,2.747163);
   grae->SetPointError(6,0,0,0.1846346,0.1846346);
   grae->SetPoint(7,1.420571e+09,2.747163);
   grae->SetPointError(7,0,0,0.1846346,0.1846346);
   grae->SetPoint(8,1.421262e+09,2.747163);
   grae->SetPointError(8,0,0,0.1846346,0.1846346);
   grae->SetPoint(9,1.42307e+09,2.747163);
   grae->SetPointError(9,0,0,0.1846346,0.1846346);
   grae->SetPoint(10,1.42056e+09,2.747163);
   grae->SetPointError(10,0,0,0.1846346,0.1846346);
   grae->SetPoint(11,1.418831e+09,2.747163);
   grae->SetPointError(11,0,0,0.1846346,0.1846346);
   grae->SetPoint(12,1.42247e+09,2.747163);
   grae->SetPointError(12,0,0,0.1846346,0.1846346);
   grae->SetPoint(13,1.420562e+09,2.747163);
   grae->SetPointError(13,0,0,0.1846346,0.1846346);
   grae->SetPoint(14,1.42248e+09,2.747163);
   grae->SetPointError(14,0,0,0.1846346,0.1846346);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","Graph",100,1.417042e+09,1.424183e+09);
   Graph_Graph1->SetMinimum(2.525602);
   Graph_Graph1->SetMaximum(2.968725);
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
   grae->SetHistogram(Graph_Graph1);
   
   grae->Draw("3 ");
   
   TPaveText *pt = new TPaveText(0.15,0.9374,0.85,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Fermi Dirac Shape in Time (bottom) - run 11");
   pt->Draw();
   5f20f85d7c8e4846869fce909dcd3b1c->Modified();
   5f20f85d7c8e4846869fce909dcd3b1c->cd();
   5f20f85d7c8e4846869fce909dcd3b1c->SetSelected(5f20f85d7c8e4846869fce909dcd3b1c);
}
