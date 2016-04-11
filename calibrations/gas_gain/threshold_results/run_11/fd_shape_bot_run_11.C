{
//=========Macro generated from canvas: 3d19f1ce743a4045b92934518552976f/
//=========  (Wed Apr  6 13:53:13 2016) by ROOT version5.34/26
   TCanvas *3d19f1ce743a4045b92934518552976f = new TCanvas("3d19f1ce743a4045b92934518552976f", "",127,189,704,527);
   3d19f1ce743a4045b92934518552976f->SetHighLightColor(2);
   3d19f1ce743a4045b92934518552976f->Range(1.416149e+09,-1.104165,1.425076e+09,5.507745);
   3d19f1ce743a4045b92934518552976f->SetFillColor(0);
   3d19f1ce743a4045b92934518552976f->SetBorderMode(0);
   3d19f1ce743a4045b92934518552976f->SetBorderSize(2);
   3d19f1ce743a4045b92934518552976f->SetFrameBorderMode(0);
   3d19f1ce743a4045b92934518552976f->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(15);
   gre->SetName("Graph0");
   gre->SetTitle("Fermi Dirac Shape in Time (bottom) - run 11");
   gre->SetFillColor(1);
   gre->SetPoint(0,1.420566e+09,2.77385);
   gre->SetPointError(0,0,0.1593889);
   gre->SetPoint(1,1.418225e+09,3.09315);
   gre->SetPointError(1,0,0.2737088);
   gre->SetPoint(2,1.422475e+09,2.96695);
   gre->SetPointError(2,0,0.2131632);
   gre->SetPoint(3,1.418056e+09,3.11527);
   gre->SetPointError(3,0,0.3200766);
   gre->SetPoint(4,1.422464e+09,3.08686);
   gre->SetPointError(4,0,0.2719506);
   gre->SetPoint(5,1.417637e+09,3.41048);
   gre->SetPointError(5,0,0.3710923);
   gre->SetPoint(6,1.423588e+09,1.90046);
   gre->SetPointError(6,0,1.902128);
   gre->SetPoint(7,1.420571e+09,1.96996);
   gre->SetPointError(7,0,0.1347286);
   gre->SetPoint(8,1.421262e+09,2.90091);
   gre->SetPointError(8,0,0.233329);
   gre->SetPoint(9,1.42307e+09,2.20179);
   gre->SetPointError(9,0,2.20397);
   gre->SetPoint(10,1.42056e+09,2.77918);
   gre->SetPointError(10,0,0.2284743);
   gre->SetPoint(11,1.418831e+09,3.58031);
   gre->SetPointError(11,0,0.3413674);
   gre->SetPoint(12,1.42247e+09,2.39807);
   gre->SetPointError(12,0,0.1560568);
   gre->SetPoint(13,1.420562e+09,2.82458);
   gre->SetPointError(13,0,0.1944283);
   gre->SetPoint(14,1.42248e+09,2.76389);
   gre->SetPointError(14,0,0.195415);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","Fermi Dirac Shape in Time (bottom) - run 11",100,1.417042e+09,1.424183e+09);
   Graph_Graph1->SetMinimum(-0.4429738);
   Graph_Graph1->SetMaximum(4.846554);
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
   fG1->SetChisquare(52.42895);
   fG1->SetNDF(14);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,2.677593);
   fG1->SetParError(0,0.05767318);
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
   grae->SetPoint(0,1.420566e+09,2.677593);
   grae->SetPointError(0,0,0,0.2393754,0.2393754);
   grae->SetPoint(1,1.418225e+09,2.677593);
   grae->SetPointError(1,0,0,0.2393754,0.2393754);
   grae->SetPoint(2,1.422475e+09,2.677593);
   grae->SetPointError(2,0,0,0.2393754,0.2393754);
   grae->SetPoint(3,1.418056e+09,2.677593);
   grae->SetPointError(3,0,0,0.2393754,0.2393754);
   grae->SetPoint(4,1.422464e+09,2.677593);
   grae->SetPointError(4,0,0,0.2393754,0.2393754);
   grae->SetPoint(5,1.417637e+09,2.677593);
   grae->SetPointError(5,0,0,0.2393754,0.2393754);
   grae->SetPoint(6,1.423588e+09,2.677593);
   grae->SetPointError(6,0,0,0.2393754,0.2393754);
   grae->SetPoint(7,1.420571e+09,2.677593);
   grae->SetPointError(7,0,0,0.2393754,0.2393754);
   grae->SetPoint(8,1.421262e+09,2.677593);
   grae->SetPointError(8,0,0,0.2393754,0.2393754);
   grae->SetPoint(9,1.42307e+09,2.677593);
   grae->SetPointError(9,0,0,0.2393754,0.2393754);
   grae->SetPoint(10,1.42056e+09,2.677593);
   grae->SetPointError(10,0,0,0.2393754,0.2393754);
   grae->SetPoint(11,1.418831e+09,2.677593);
   grae->SetPointError(11,0,0,0.2393754,0.2393754);
   grae->SetPoint(12,1.42247e+09,2.677593);
   grae->SetPointError(12,0,0,0.2393754,0.2393754);
   grae->SetPoint(13,1.420562e+09,2.677593);
   grae->SetPointError(13,0,0,0.2393754,0.2393754);
   grae->SetPoint(14,1.42248e+09,2.677593);
   grae->SetPointError(14,0,0,0.2393754,0.2393754);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","Graph",100,1.417042e+09,1.424183e+09);
   Graph_Graph1->SetMinimum(2.390343);
   Graph_Graph1->SetMaximum(2.964844);
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
   3d19f1ce743a4045b92934518552976f->Modified();
   3d19f1ce743a4045b92934518552976f->cd();
   3d19f1ce743a4045b92934518552976f->SetSelected(3d19f1ce743a4045b92934518552976f);
}
