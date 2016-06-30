{
//=========Macro generated from canvas: 3ded5c1066b44382b614e6f87af4cb43/
//=========  (Tue May 24 15:31:00 2016) by ROOT version5.34/26
   TCanvas *3ded5c1066b44382b614e6f87af4cb43 = new TCanvas("3ded5c1066b44382b614e6f87af4cb43", "",10,45,704,527);
   3ded5c1066b44382b614e6f87af4cb43->SetHighLightColor(2);
   3ded5c1066b44382b614e6f87af4cb43->Range(1.416657e+09,-0.625,1.42346e+09,5.625);
   3ded5c1066b44382b614e6f87af4cb43->SetFillColor(0);
   3ded5c1066b44382b614e6f87af4cb43->SetBorderMode(0);
   3ded5c1066b44382b614e6f87af4cb43->SetBorderSize(2);
   3ded5c1066b44382b614e6f87af4cb43->SetFrameBorderMode(0);
   3ded5c1066b44382b614e6f87af4cb43->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(6);
   gre->SetName("Graph0");
   gre->SetTitle("fd_shape_tot Weighted Average - run 11");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.417637e+09,2.193842);
   gre->SetPointError(0,0,0.2337784);
   gre->SetPoint(1,1.418225e+09,1.91842);
   gre->SetPointError(1,0,0.1515803);
   gre->SetPoint(2,1.418831e+09,1.884894);
   gre->SetPointError(2,0,0.100525);
   gre->SetPoint(3,1.420566e+09,1.298826);
   gre->SetPointError(3,0,0.2006121);
   gre->SetPoint(4,1.420571e+09,1.51832);
   gre->SetPointError(4,0,0.1792201);
   gre->SetPoint(5,1.42247e+09,1.552988);
   gre->SetPointError(5,0,0.1044239);
   
   TH1F *Graph_Graph3 = new TH1F("Graph_Graph3","fd_shape_tot Weighted Average - run 11",100,1.417337e+09,1.42278e+09);
   Graph_Graph3->SetMinimum(0);
   Graph_Graph3->SetMaximum(5);
   Graph_Graph3->SetDirectory(0);
   Graph_Graph3->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
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
   
   gre->Draw("ap");
   
   TF1 *f_fd_shape_tot_0 = new TF1("f_fd_shape_tot_0","pol0",1.417337e+09,1.419272e+09);
   f_fd_shape_tot_0->SetFillColor(19);
   f_fd_shape_tot_0->SetFillStyle(0);
   f_fd_shape_tot_0->SetLineColor(2);
   f_fd_shape_tot_0->SetLineWidth(2);
   f_fd_shape_tot_0->SetChisquare(1.480786);
   f_fd_shape_tot_0->SetNDF(2);
   f_fd_shape_tot_0->GetXaxis()->SetLabelFont(42);
   f_fd_shape_tot_0->GetXaxis()->SetLabelSize(0.035);
   f_fd_shape_tot_0->GetXaxis()->SetTitleSize(0.035);
   f_fd_shape_tot_0->GetXaxis()->SetTitleFont(42);
   f_fd_shape_tot_0->GetYaxis()->SetLabelFont(42);
   f_fd_shape_tot_0->GetYaxis()->SetLabelSize(0.035);
   f_fd_shape_tot_0->GetYaxis()->SetTitleSize(0.035);
   f_fd_shape_tot_0->GetYaxis()->SetTitleFont(42);
   f_fd_shape_tot_0->SetParameter(0,1.92913);
   f_fd_shape_tot_0->SetParError(0,0.07886537);
   f_fd_shape_tot_0->SetParLimits(0,0,0);
   f_fd_shape_tot_0->Draw("same");
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(6);
   grae->SetName("Graph1");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#0000ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3005);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.417637e+09,1.92913);
   grae->SetPointError(0,0,0,0.08900447,0.08900447);
   grae->SetPoint(1,1.418225e+09,1.92913);
   grae->SetPointError(1,0,0,0.08900447,0.08900447);
   grae->SetPoint(2,1.418831e+09,1.92913);
   grae->SetPointError(2,0,0,0.08900447,0.08900447);
   grae->SetPoint(3,1.420566e+09,1.92913);
   grae->SetPointError(3,0,0,0.08900447,0.08900447);
   grae->SetPoint(4,1.420571e+09,1.92913);
   grae->SetPointError(4,0,0,0.08900447,0.08900447);
   grae->SetPoint(5,1.42247e+09,1.92913);
   grae->SetPointError(5,0,0,0.08900447,0.08900447);
   
   TH1F *Graph_Graph5 = new TH1F("Graph_Graph5","Graph",100,1.417154e+09,1.422953e+09);
   Graph_Graph5->SetMinimum(1.822324);
   Graph_Graph5->SetMaximum(2.035935);
   Graph_Graph5->SetDirectory(0);
   Graph_Graph5->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph5->SetLineColor(ci);
   Graph_Graph5->GetXaxis()->SetLabelFont(42);
   Graph_Graph5->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph5->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph5->GetXaxis()->SetTitleFont(42);
   Graph_Graph5->GetYaxis()->SetLabelFont(42);
   Graph_Graph5->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph5->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph5->GetYaxis()->SetTitleFont(42);
   Graph_Graph5->GetZaxis()->SetLabelFont(42);
   Graph_Graph5->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph5->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph5->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph5);
   
   grae->Draw("3 ");
   
   TPaveText *pt = new TPaveText(0.1514286,0.9374,0.8485714,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("fd_shape_tot Weighted Average - run 11");
   pt->Draw();
   
   TF1 *f_fd_shape_tot_1 = new TF1("f_fd_shape_tot_1","pol0",1.419272e+09,1.42278e+09);
   f_fd_shape_tot_1->SetFillColor(19);
   f_fd_shape_tot_1->SetFillStyle(0);
   f_fd_shape_tot_1->SetLineColor(2);
   f_fd_shape_tot_1->SetLineWidth(2);
   f_fd_shape_tot_1->SetChisquare(1.272296);
   f_fd_shape_tot_1->SetNDF(2);
   f_fd_shape_tot_1->GetXaxis()->SetLabelFont(42);
   f_fd_shape_tot_1->GetXaxis()->SetLabelSize(0.035);
   f_fd_shape_tot_1->GetXaxis()->SetTitleSize(0.035);
   f_fd_shape_tot_1->GetXaxis()->SetTitleFont(42);
   f_fd_shape_tot_1->GetYaxis()->SetLabelFont(42);
   f_fd_shape_tot_1->GetYaxis()->SetLabelSize(0.035);
   f_fd_shape_tot_1->GetYaxis()->SetTitleSize(0.035);
   f_fd_shape_tot_1->GetYaxis()->SetTitleFont(42);
   f_fd_shape_tot_1->SetParameter(0,1.502918);
   f_fd_shape_tot_1->SetParError(0,0.08228637);
   f_fd_shape_tot_1->SetParLimits(0,0,0);
   f_fd_shape_tot_1->Draw("same");
   
   grae = new TGraphAsymmErrors(6);
   grae->SetName("Graph2");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#0000ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3005);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.417637e+09,1.502918);
   grae->SetPointError(0,0,0,0.0860798,0.0860798);
   grae->SetPoint(1,1.418225e+09,1.502918);
   grae->SetPointError(1,0,0,0.0860798,0.0860798);
   grae->SetPoint(2,1.418831e+09,1.502918);
   grae->SetPointError(2,0,0,0.0860798,0.0860798);
   grae->SetPoint(3,1.420566e+09,1.502918);
   grae->SetPointError(3,0,0,0.0860798,0.0860798);
   grae->SetPoint(4,1.420571e+09,1.502918);
   grae->SetPointError(4,0,0,0.0860798,0.0860798);
   grae->SetPoint(5,1.42247e+09,1.502918);
   grae->SetPointError(5,0,0,0.0860798,0.0860798);
   
   TH1F *Graph_Graph6 = new TH1F("Graph_Graph6","Graph",100,1.417154e+09,1.422953e+09);
   Graph_Graph6->SetMinimum(1.399623);
   Graph_Graph6->SetMaximum(1.606214);
   Graph_Graph6->SetDirectory(0);
   Graph_Graph6->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph6->SetLineColor(ci);
   Graph_Graph6->GetXaxis()->SetLabelFont(42);
   Graph_Graph6->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph6->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph6->GetXaxis()->SetTitleFont(42);
   Graph_Graph6->GetYaxis()->SetLabelFont(42);
   Graph_Graph6->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph6->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph6->GetYaxis()->SetTitleFont(42);
   Graph_Graph6->GetZaxis()->SetLabelFont(42);
   Graph_Graph6->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph6->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph6->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph6);
   
   grae->Draw("3 ");
   3ded5c1066b44382b614e6f87af4cb43->Modified();
   3ded5c1066b44382b614e6f87af4cb43->cd();
   3ded5c1066b44382b614e6f87af4cb43->SetSelected(3ded5c1066b44382b614e6f87af4cb43);
}
