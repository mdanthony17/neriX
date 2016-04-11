{
//=========Macro generated from canvas: 56de3534a5b744229803a4f95ae6a8a2/
//=========  (Wed Apr  6 13:53:14 2016) by ROOT version5.34/26
   TCanvas *56de3534a5b744229803a4f95ae6a8a2 = new TCanvas("56de3534a5b744229803a4f95ae6a8a2", "",291,92,704,527);
   56de3534a5b744229803a4f95ae6a8a2->SetHighLightColor(2);
   56de3534a5b744229803a4f95ae6a8a2->Range(1.416149e+09,-4.59311,1.425076e+09,13.33051);
   56de3534a5b744229803a4f95ae6a8a2->SetFillColor(0);
   56de3534a5b744229803a4f95ae6a8a2->SetBorderMode(0);
   56de3534a5b744229803a4f95ae6a8a2->SetBorderSize(2);
   56de3534a5b744229803a4f95ae6a8a2->SetFrameBorderMode(0);
   56de3534a5b744229803a4f95ae6a8a2->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(15);
   gre->SetName("Graph0");
   gre->SetTitle("Fermi Dirac Shape in Time (top) - run 11");
   gre->SetFillColor(1);
   gre->SetPoint(0,1.420566e+09,0.00773);
   gre->SetPointError(0,0,0.003073847);
   gre->SetPoint(1,1.418225e+09,1.59548);
   gre->SetPointError(1,0,0.3832196);
   gre->SetPoint(2,1.422475e+09,0.6384);
   gre->SetPointError(2,0,0.1406493);
   gre->SetPoint(3,1.418056e+09,1.99472);
   gre->SetPointError(3,0,0.4292793);
   gre->SetPoint(4,1.422464e+09,0.1124);
   gre->SetPointError(4,0,0.0280986);
   gre->SetPoint(5,1.417637e+09,2.20632);
   gre->SetPointError(5,0,0.4043888);
   gre->SetPoint(6,1.423588e+09,0.09227);
   gre->SetPointError(6,0,0.1030606);
   gre->SetPoint(7,1.420571e+09,0.69196);
   gre->SetPointError(7,0,0.09003676);
   gre->SetPoint(8,1.421262e+09,0.14002);
   gre->SetPointError(8,0,1.377152);
   gre->SetPoint(9,1.42307e+09,0.00373);
   gre->SetPointError(9,0,0.009364881);
   gre->SetPoint(10,1.42056e+09,9.99966);
   gre->SetPointError(10,0,0.3435829);
   gre->SetPoint(11,1.418831e+09,1.20717);
   gre->SetPointError(11,0,0.2949672);
   gre->SetPoint(12,1.42247e+09,0.26138);
   gre->SetPointError(12,0,1.867219);
   gre->SetPoint(13,1.420562e+09,0.00019);
   gre->SetPointError(13,0,8.176038e-05);
   gre->SetPoint(14,1.42248e+09,0.6199);
   gre->SetPointError(14,0,0.1655305);
   
   TH1F *Graph_Graph3 = new TH1F("Graph_Graph3","Fermi Dirac Shape in Time (top) - run 11",100,1.417042e+09,1.424183e+09);
   Graph_Graph3->SetMinimum(-2.800747);
   Graph_Graph3->SetMaximum(11.53815);
   Graph_Graph3->SetDirectory(0);
   Graph_Graph3->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph3->SetLineColor(ci);
   Graph_Graph3->GetXaxis()->SetTitle("Time");
   Graph_Graph3->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph3->GetXaxis()->SetTimeFormat("%m/%d%F1995-01-01 00:00:00s0");
   Graph_Graph3->GetXaxis()->SetLabelFont(42);
   Graph_Graph3->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph3->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph3->GetXaxis()->SetTitleFont(42);
   Graph_Graph3->GetYaxis()->SetTitle("FD Shape (top) [PE]");
   Graph_Graph3->GetYaxis()->SetLabelFont(42);
   Graph_Graph3->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph3->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph3->GetYaxis()->SetTitleFont(42);
   Graph_Graph3->GetZaxis()->SetLabelFont(42);
   Graph_Graph3->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph3->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph3->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph3);
   
   
   TF1 *fG3 = new TF1("fG3","pol0",1.417042e+09,1.424183e+09);
   fG3->SetFillColor(19);
   fG3->SetFillStyle(0);
   fG3->SetLineColor(2);
   fG3->SetLineWidth(1);
   fG3->SetLineStyle(9);
   fG3->SetChisquare(1049);
   fG3->SetNDF(14);
   fG3->GetXaxis()->SetLabelFont(42);
   fG3->GetXaxis()->SetLabelSize(0.035);
   fG3->GetXaxis()->SetTitleSize(0.035);
   fG3->GetXaxis()->SetTitleFont(42);
   fG3->GetYaxis()->SetLabelFont(42);
   fG3->GetYaxis()->SetLabelSize(0.035);
   fG3->GetYaxis()->SetTitleSize(0.035);
   fG3->GetYaxis()->SetTitleFont(42);
   fG3->SetParameter(0,0.000198438);
   fG3->SetParError(0,8.172792e-05);
   fG3->SetParLimits(0,0,0);
   gre->GetListOfFunctions()->Add(fG3);
   gre->Draw("ap");
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(15);
   grae->SetName("Graph1");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#0000ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3005);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.420566e+09,0.000198438);
   grae->SetPointError(0,0,0,0.001517324,0.001517324);
   grae->SetPoint(1,1.418225e+09,0.000198438);
   grae->SetPointError(1,0,0,0.001517324,0.001517324);
   grae->SetPoint(2,1.422475e+09,0.000198438);
   grae->SetPointError(2,0,0,0.001517324,0.001517324);
   grae->SetPoint(3,1.418056e+09,0.000198438);
   grae->SetPointError(3,0,0,0.001517324,0.001517324);
   grae->SetPoint(4,1.422464e+09,0.000198438);
   grae->SetPointError(4,0,0,0.001517324,0.001517324);
   grae->SetPoint(5,1.417637e+09,0.000198438);
   grae->SetPointError(5,0,0,0.001517324,0.001517324);
   grae->SetPoint(6,1.423588e+09,0.000198438);
   grae->SetPointError(6,0,0,0.001517324,0.001517324);
   grae->SetPoint(7,1.420571e+09,0.000198438);
   grae->SetPointError(7,0,0,0.001517324,0.001517324);
   grae->SetPoint(8,1.421262e+09,0.000198438);
   grae->SetPointError(8,0,0,0.001517324,0.001517324);
   grae->SetPoint(9,1.42307e+09,0.000198438);
   grae->SetPointError(9,0,0,0.001517324,0.001517324);
   grae->SetPoint(10,1.42056e+09,0.000198438);
   grae->SetPointError(10,0,0,0.001517324,0.001517324);
   grae->SetPoint(11,1.418831e+09,0.000198438);
   grae->SetPointError(11,0,0,0.001517324,0.001517324);
   grae->SetPoint(12,1.42247e+09,0.000198438);
   grae->SetPointError(12,0,0,0.001517324,0.001517324);
   grae->SetPoint(13,1.420562e+09,0.000198438);
   grae->SetPointError(13,0,0,0.001517324,0.001517324);
   grae->SetPoint(14,1.42248e+09,0.000198438);
   grae->SetPointError(14,0,0,0.001517324,0.001517324);
   
   TH1F *Graph_Graph3 = new TH1F("Graph_Graph3","Graph",100,1.417042e+09,1.424183e+09);
   Graph_Graph3->SetMinimum(-0.00162235);
   Graph_Graph3->SetMaximum(0.002019226);
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
   grae->SetHistogram(Graph_Graph3);
   
   grae->Draw("3 ");
   
   TPaveText *pt = new TPaveText(0.1535714,0.9374,0.8464286,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Fermi Dirac Shape in Time (top) - run 11");
   pt->Draw();
   56de3534a5b744229803a4f95ae6a8a2->Modified();
   56de3534a5b744229803a4f95ae6a8a2->cd();
   56de3534a5b744229803a4f95ae6a8a2->SetSelected(56de3534a5b744229803a4f95ae6a8a2);
}
