{
//=========Macro generated from canvas: 57c6846531ea4d438eee4aea0c628611/
//=========  (Tue May 24 15:30:59 2016) by ROOT version5.34/26
   TCanvas *57c6846531ea4d438eee4aea0c628611 = new TCanvas("57c6846531ea4d438eee4aea0c628611", "",10,45,704,527);
   57c6846531ea4d438eee4aea0c628611->SetHighLightColor(2);
   57c6846531ea4d438eee4aea0c628611->Range(1.416657e+09,6.875,1.42346e+09,18.125);
   57c6846531ea4d438eee4aea0c628611->SetFillColor(0);
   57c6846531ea4d438eee4aea0c628611->SetBorderMode(0);
   57c6846531ea4d438eee4aea0c628611->SetBorderSize(2);
   57c6846531ea4d438eee4aea0c628611->SetFrameBorderMode(0);
   57c6846531ea4d438eee4aea0c628611->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(6);
   gre->SetName("Graph0");
   gre->SetTitle("fd_center_tot Weighted Average - run 11");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.417637e+09,14.94257);
   gre->SetPointError(0,0,0.3762386);
   gre->SetPoint(1,1.418225e+09,15.04759);
   gre->SetPointError(1,0,0.2605997);
   gre->SetPoint(2,1.418831e+09,16.14405);
   gre->SetPointError(2,0,0.2099596);
   gre->SetPoint(3,1.420566e+09,11.77808);
   gre->SetPointError(3,0,0.3041102);
   gre->SetPoint(4,1.420571e+09,11.94299);
   gre->SetPointError(4,0,0.3190541);
   gre->SetPoint(5,1.42247e+09,12.39021);
   gre->SetPointError(5,0,0.1915586);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","fd_center_tot Weighted Average - run 11",100,1.417337e+09,1.42278e+09);
   Graph_Graph2->SetMinimum(8);
   Graph_Graph2->SetMaximum(17);
   Graph_Graph2->SetDirectory(0);
   Graph_Graph2->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
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
   
   gre->Draw("ap");
   
   TF1 *f_fd_center_tot_0 = new TF1("f_fd_center_tot_0","pol0",1.417337e+09,1.419272e+09);
   f_fd_center_tot_0->SetFillColor(19);
   f_fd_center_tot_0->SetFillStyle(0);
   f_fd_center_tot_0->SetLineColor(2);
   f_fd_center_tot_0->SetLineWidth(2);
   f_fd_center_tot_0->SetChisquare(14.2568);
   f_fd_center_tot_0->SetNDF(2);
   f_fd_center_tot_0->GetXaxis()->SetLabelFont(42);
   f_fd_center_tot_0->GetXaxis()->SetLabelSize(0.035);
   f_fd_center_tot_0->GetXaxis()->SetTitleSize(0.035);
   f_fd_center_tot_0->GetXaxis()->SetTitleFont(42);
   f_fd_center_tot_0->GetYaxis()->SetLabelFont(42);
   f_fd_center_tot_0->GetYaxis()->SetLabelSize(0.035);
   f_fd_center_tot_0->GetYaxis()->SetTitleSize(0.035);
   f_fd_center_tot_0->GetYaxis()->SetTitleFont(42);
   f_fd_center_tot_0->SetParameter(0,15.59017);
   f_fd_center_tot_0->SetParError(0,0.1499506);
   f_fd_center_tot_0->SetParLimits(0,0,0);
   f_fd_center_tot_0->Draw("same");
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(6);
   grae->SetName("Graph1");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#0000ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3005);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.417637e+09,15.59017);
   grae->SetPointError(0,0,0,0.5250958,0.5250958);
   grae->SetPoint(1,1.418225e+09,15.59017);
   grae->SetPointError(1,0,0,0.5250958,0.5250958);
   grae->SetPoint(2,1.418831e+09,15.59017);
   grae->SetPointError(2,0,0,0.5250958,0.5250958);
   grae->SetPoint(3,1.420566e+09,15.59017);
   grae->SetPointError(3,0,0,0.5250958,0.5250958);
   grae->SetPoint(4,1.420571e+09,15.59017);
   grae->SetPointError(4,0,0,0.5250958,0.5250958);
   grae->SetPoint(5,1.42247e+09,15.59017);
   grae->SetPointError(5,0,0,0.5250958,0.5250958);
   
   TH1F *Graph_Graph3 = new TH1F("Graph_Graph3","Graph",100,1.417154e+09,1.422953e+09);
   Graph_Graph3->SetMinimum(14.96006);
   Graph_Graph3->SetMaximum(16.22029);
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
   
   TPaveText *pt = new TPaveText(0.15,0.9374,0.85,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("fd_center_tot Weighted Average - run 11");
   pt->Draw();
   
   TF1 *f_fd_center_tot_1 = new TF1("f_fd_center_tot_1","pol0",1.419272e+09,1.42278e+09);
   f_fd_center_tot_1->SetFillColor(19);
   f_fd_center_tot_1->SetFillStyle(0);
   f_fd_center_tot_1->SetLineColor(2);
   f_fd_center_tot_1->SetLineWidth(2);
   f_fd_center_tot_1->SetChisquare(3.484057);
   f_fd_center_tot_1->SetNDF(2);
   f_fd_center_tot_1->GetXaxis()->SetLabelFont(42);
   f_fd_center_tot_1->GetXaxis()->SetLabelSize(0.035);
   f_fd_center_tot_1->GetXaxis()->SetTitleSize(0.035);
   f_fd_center_tot_1->GetXaxis()->SetTitleFont(42);
   f_fd_center_tot_1->GetYaxis()->SetLabelFont(42);
   f_fd_center_tot_1->GetYaxis()->SetLabelSize(0.035);
   f_fd_center_tot_1->GetYaxis()->SetTitleSize(0.035);
   f_fd_center_tot_1->GetYaxis()->SetTitleFont(42);
   f_fd_center_tot_1->SetParameter(0,12.16025);
   f_fd_center_tot_1->SetParError(0,0.1445058);
   f_fd_center_tot_1->SetParLimits(0,0,0);
   f_fd_center_tot_1->Draw("same");
   
   grae = new TGraphAsymmErrors(6);
   grae->SetName("Graph2");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#0000ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3005);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.417637e+09,12.16025);
   grae->SetPointError(0,0,0,0.2501539,0.2501539);
   grae->SetPoint(1,1.418225e+09,12.16025);
   grae->SetPointError(1,0,0,0.2501539,0.2501539);
   grae->SetPoint(2,1.418831e+09,12.16025);
   grae->SetPointError(2,0,0,0.2501539,0.2501539);
   grae->SetPoint(3,1.420566e+09,12.16025);
   grae->SetPointError(3,0,0,0.2501539,0.2501539);
   grae->SetPoint(4,1.420571e+09,12.16025);
   grae->SetPointError(4,0,0,0.2501539,0.2501539);
   grae->SetPoint(5,1.42247e+09,12.16025);
   grae->SetPointError(5,0,0,0.2501539,0.2501539);
   
   TH1F *Graph_Graph4 = new TH1F("Graph_Graph4","Graph",100,1.417154e+09,1.422953e+09);
   Graph_Graph4->SetMinimum(11.86007);
   Graph_Graph4->SetMaximum(12.46044);
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
   57c6846531ea4d438eee4aea0c628611->Modified();
   57c6846531ea4d438eee4aea0c628611->cd();
   57c6846531ea4d438eee4aea0c628611->SetSelected(57c6846531ea4d438eee4aea0c628611);
}
