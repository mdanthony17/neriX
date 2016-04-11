{
//=========Macro generated from canvas: b0e2c07472a347979b2db4da016c5d1f/
//=========  (Wed Apr  6 13:53:13 2016) by ROOT version5.34/26
   TCanvas *b0e2c07472a347979b2db4da016c5d1f = new TCanvas("b0e2c07472a347979b2db4da016c5d1f", "",112,366,704,527);
   b0e2c07472a347979b2db4da016c5d1f->SetHighLightColor(2);
   b0e2c07472a347979b2db4da016c5d1f->Range(1.416149e+09,-4.568301,1.425076e+09,22.82554);
   b0e2c07472a347979b2db4da016c5d1f->SetFillColor(0);
   b0e2c07472a347979b2db4da016c5d1f->SetBorderMode(0);
   b0e2c07472a347979b2db4da016c5d1f->SetBorderSize(2);
   b0e2c07472a347979b2db4da016c5d1f->SetFrameBorderMode(0);
   b0e2c07472a347979b2db4da016c5d1f->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(15);
   gre->SetName("Graph0");
   gre->SetTitle("Fermi Dirac Center in Time (bottom) - run 11");
   gre->SetFillColor(1);
   gre->SetPoint(0,1.420566e+09,10.66392);
   gre->SetPointError(0,0,0.4998656);
   gre->SetPoint(1,1.418225e+09,8.6805);
   gre->SetPointError(1,0,0.7050135);
   gre->SetPoint(2,1.422475e+09,11.48406);
   gre->SetPointError(2,0,0.560566);
   gre->SetPoint(3,1.418056e+09,8.2875);
   gre->SetPointError(3,0,0.8138866);
   gre->SetPoint(4,1.422464e+09,9.70842);
   gre->SetPointError(4,0,0.6325334);
   gre->SetPoint(5,1.417637e+09,7.81226);
   gre->SetPointError(5,0,0.7324576);
   gre->SetPoint(6,1.423588e+09,8.34105);
   gre->SetPointError(6,0,8.343432);
   gre->SetPoint(7,1.420571e+09,7.37973);
   gre->SetPointError(7,0,0.3971277);
   gre->SetPoint(8,1.421262e+09,9.92142);
   gre->SetPointError(8,0,0.6022552);
   gre->SetPoint(9,1.42307e+09,9.12862);
   gre->SetPointError(9,0,9.131281);
   gre->SetPoint(10,1.42056e+09,10.09137);
   gre->SetPointError(10,0,0.594442);
   gre->SetPoint(11,1.418831e+09,8.98434);
   gre->SetPointError(11,0,0.6798726);
   gre->SetPoint(12,1.42247e+09,9.41027);
   gre->SetPointError(12,0,0.5144887);
   gre->SetPoint(13,1.420562e+09,9.2733);
   gre->SetPointError(13,0,0.5184307);
   gre->SetPoint(14,1.42248e+09,11.29369);
   gre->SetPointError(14,0,0.554165);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Fermi Dirac Center in Time (bottom) - run 11",100,1.417042e+09,1.424183e+09);
   Graph_Graph2->SetMinimum(-1.828917);
   Graph_Graph2->SetMaximum(20.08616);
   Graph_Graph2->SetDirectory(0);
   Graph_Graph2->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph2->SetLineColor(ci);
   Graph_Graph2->GetXaxis()->SetTitle("Time");
   Graph_Graph2->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph2->GetXaxis()->SetTimeFormat("%m/%d%F1995-01-01 00:00:00s0");
   Graph_Graph2->GetXaxis()->SetLabelFont(42);
   Graph_Graph2->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph2->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph2->GetXaxis()->SetTitleFont(42);
   Graph_Graph2->GetYaxis()->SetTitle("FD Center (bottom) [PE]");
   Graph_Graph2->GetYaxis()->SetLabelFont(42);
   Graph_Graph2->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph2->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph2->GetYaxis()->SetTitleFont(42);
   Graph_Graph2->GetZaxis()->SetLabelFont(42);
   Graph_Graph2->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph2->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph2->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph2);
   
   
   TF1 *fG2 = new TF1("fG2","pol0",1.417042e+09,1.424183e+09);
   fG2->SetFillColor(19);
   fG2->SetFillStyle(0);
   fG2->SetLineColor(2);
   fG2->SetLineWidth(1);
   fG2->SetLineStyle(9);
   fG2->SetChisquare(68.093);
   fG2->SetNDF(14);
   fG2->GetXaxis()->SetLabelFont(42);
   fG2->GetXaxis()->SetLabelSize(0.035);
   fG2->GetXaxis()->SetTitleSize(0.035);
   fG2->GetXaxis()->SetTitleFont(42);
   fG2->GetYaxis()->SetLabelFont(42);
   fG2->GetYaxis()->SetLabelSize(0.035);
   fG2->GetYaxis()->SetTitleSize(0.035);
   fG2->GetYaxis()->SetTitleFont(42);
   fG2->SetParameter(0,9.444443);
   fG2->SetParError(0,0.1582293);
   fG2->SetParLimits(0,0,0);
   gre->GetListOfFunctions()->Add(fG2);
   gre->Draw("ap");
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(15);
   grae->SetName("Graph1");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#0000ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3005);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.420566e+09,9.444443);
   grae->SetPointError(0,0,0,0.748442,0.748442);
   grae->SetPoint(1,1.418225e+09,9.444443);
   grae->SetPointError(1,0,0,0.748442,0.748442);
   grae->SetPoint(2,1.422475e+09,9.444443);
   grae->SetPointError(2,0,0,0.748442,0.748442);
   grae->SetPoint(3,1.418056e+09,9.444443);
   grae->SetPointError(3,0,0,0.748442,0.748442);
   grae->SetPoint(4,1.422464e+09,9.444443);
   grae->SetPointError(4,0,0,0.748442,0.748442);
   grae->SetPoint(5,1.417637e+09,9.444443);
   grae->SetPointError(5,0,0,0.748442,0.748442);
   grae->SetPoint(6,1.423588e+09,9.444443);
   grae->SetPointError(6,0,0,0.748442,0.748442);
   grae->SetPoint(7,1.420571e+09,9.444443);
   grae->SetPointError(7,0,0,0.748442,0.748442);
   grae->SetPoint(8,1.421262e+09,9.444443);
   grae->SetPointError(8,0,0,0.748442,0.748442);
   grae->SetPoint(9,1.42307e+09,9.444443);
   grae->SetPointError(9,0,0,0.748442,0.748442);
   grae->SetPoint(10,1.42056e+09,9.444443);
   grae->SetPointError(10,0,0,0.748442,0.748442);
   grae->SetPoint(11,1.418831e+09,9.444443);
   grae->SetPointError(11,0,0,0.748442,0.748442);
   grae->SetPoint(12,1.42247e+09,9.444443);
   grae->SetPointError(12,0,0,0.748442,0.748442);
   grae->SetPoint(13,1.420562e+09,9.444443);
   grae->SetPointError(13,0,0,0.748442,0.748442);
   grae->SetPoint(14,1.42248e+09,9.444443);
   grae->SetPointError(14,0,0,0.748442,0.748442);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Graph",100,1.417042e+09,1.424183e+09);
   Graph_Graph2->SetMinimum(8.546312);
   Graph_Graph2->SetMaximum(10.34257);
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
   grae->SetHistogram(Graph_Graph2);
   
   grae->Draw("3 ");
   
   TPaveText *pt = new TPaveText(0.15,0.9374,0.85,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Fermi Dirac Center in Time (bottom) - run 11");
   pt->Draw();
   b0e2c07472a347979b2db4da016c5d1f->Modified();
   b0e2c07472a347979b2db4da016c5d1f->cd();
   b0e2c07472a347979b2db4da016c5d1f->SetSelected(b0e2c07472a347979b2db4da016c5d1f);
}
