{
//=========Macro generated from canvas: aed1403bd4904787aaaf90d1f0e248e5/
//=========  (Wed Apr  6 11:35:02 2016) by ROOT version5.34/26
   TCanvas *aed1403bd4904787aaaf90d1f0e248e5 = new TCanvas("aed1403bd4904787aaaf90d1f0e248e5", "",514,463,704,527);
   aed1403bd4904787aaaf90d1f0e248e5->SetHighLightColor(2);
   aed1403bd4904787aaaf90d1f0e248e5->Range(1.416149e+09,-4.514629,1.425076e+09,22.54743);
   aed1403bd4904787aaaf90d1f0e248e5->SetFillColor(0);
   aed1403bd4904787aaaf90d1f0e248e5->SetBorderMode(0);
   aed1403bd4904787aaaf90d1f0e248e5->SetBorderSize(2);
   aed1403bd4904787aaaf90d1f0e248e5->SetFrameBorderMode(0);
   aed1403bd4904787aaaf90d1f0e248e5->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(15);
   gre->SetName("Graph0");
   gre->SetTitle("Fermi Dirac Center in Time (bottom) - run 11");
   gre->SetFillColor(1);
   gre->SetPoint(0,1.420566e+09,9.48363);
   gre->SetPointError(0,0,0.5406627);
   gre->SetPoint(1,1.418225e+09,8.30513);
   gre->SetPointError(1,0,0.7459151);
   gre->SetPoint(2,1.422475e+09,11.54383);
   gre->SetPointError(2,0,0.4442506);
   gre->SetPoint(3,1.418056e+09,7.91733);
   gre->SetPointError(3,0,0.7377041);
   gre->SetPoint(4,1.422464e+09,9.88191);
   gre->SetPointError(4,0,0.6737802);
   gre->SetPoint(5,1.417637e+09,8.07086);
   gre->SetPointError(5,0,0.8939349);
   gre->SetPoint(6,1.423588e+09,8.06953);
   gre->SetPointError(6,0,8.07278);
   gre->SetPoint(7,1.420571e+09,10.02852);
   gre->SetPointError(7,0,0.6638752);
   gre->SetPoint(8,1.421262e+09,10.14813);
   gre->SetPointError(8,0,0.6622253);
   gre->SetPoint(9,1.42307e+09,9.0164);
   gre->SetPointError(9,0,9.020686);
   gre->SetPoint(10,1.42056e+09,8.26793);
   gre->SetPointError(10,0,0.8471974);
   gre->SetPoint(11,1.418831e+09,9.18149);
   gre->SetPointError(11,0,0.7176019);
   gre->SetPoint(12,1.42247e+09,9.83917);
   gre->SetPointError(12,0,0.4321148);
   gre->SetPoint(13,1.420562e+09,9.57325);
   gre->SetPointError(13,0,0.5684887);
   gre->SetPoint(14,1.42248e+09,11.33478);
   gre->SetPointError(14,0,0.3115504);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Fermi Dirac Center in Time (bottom) - run 11",100,1.417042e+09,1.424183e+09);
   Graph_Graph2->SetMinimum(-1.808423);
   Graph_Graph2->SetMaximum(19.84122);
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
   fG2->SetChisquare(55.00304);
   fG2->SetNDF(14);
   fG2->GetXaxis()->SetLabelFont(42);
   fG2->GetXaxis()->SetLabelSize(0.035);
   fG2->GetXaxis()->SetTitleSize(0.035);
   fG2->GetXaxis()->SetTitleFont(42);
   fG2->GetYaxis()->SetLabelFont(42);
   fG2->GetYaxis()->SetLabelSize(0.035);
   fG2->GetYaxis()->SetTitleSize(0.035);
   fG2->GetYaxis()->SetTitleFont(42);
   fG2->SetParameter(0,10.10503);
   fG2->SetParError(0,0.1538622);
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
   grae->SetPoint(0,1.420566e+09,10.10503);
   grae->SetPointError(0,0,0,0.6541018,0.6541018);
   grae->SetPoint(1,1.418225e+09,10.10503);
   grae->SetPointError(1,0,0,0.6541018,0.6541018);
   grae->SetPoint(2,1.422475e+09,10.10503);
   grae->SetPointError(2,0,0,0.6541018,0.6541018);
   grae->SetPoint(3,1.418056e+09,10.10503);
   grae->SetPointError(3,0,0,0.6541018,0.6541018);
   grae->SetPoint(4,1.422464e+09,10.10503);
   grae->SetPointError(4,0,0,0.6541018,0.6541018);
   grae->SetPoint(5,1.417637e+09,10.10503);
   grae->SetPointError(5,0,0,0.6541018,0.6541018);
   grae->SetPoint(6,1.423588e+09,10.10503);
   grae->SetPointError(6,0,0,0.6541018,0.6541018);
   grae->SetPoint(7,1.420571e+09,10.10503);
   grae->SetPointError(7,0,0,0.6541018,0.6541018);
   grae->SetPoint(8,1.421262e+09,10.10503);
   grae->SetPointError(8,0,0,0.6541018,0.6541018);
   grae->SetPoint(9,1.42307e+09,10.10503);
   grae->SetPointError(9,0,0,0.6541018,0.6541018);
   grae->SetPoint(10,1.42056e+09,10.10503);
   grae->SetPointError(10,0,0,0.6541018,0.6541018);
   grae->SetPoint(11,1.418831e+09,10.10503);
   grae->SetPointError(11,0,0,0.6541018,0.6541018);
   grae->SetPoint(12,1.42247e+09,10.10503);
   grae->SetPointError(12,0,0,0.6541018,0.6541018);
   grae->SetPoint(13,1.420562e+09,10.10503);
   grae->SetPointError(13,0,0,0.6541018,0.6541018);
   grae->SetPoint(14,1.42248e+09,10.10503);
   grae->SetPointError(14,0,0,0.6541018,0.6541018);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Graph",100,1.417042e+09,1.424183e+09);
   Graph_Graph2->SetMinimum(9.320105);
   Graph_Graph2->SetMaximum(10.88995);
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
   aed1403bd4904787aaaf90d1f0e248e5->Modified();
   aed1403bd4904787aaaf90d1f0e248e5->cd();
   aed1403bd4904787aaaf90d1f0e248e5->SetSelected(aed1403bd4904787aaaf90d1f0e248e5);
}
