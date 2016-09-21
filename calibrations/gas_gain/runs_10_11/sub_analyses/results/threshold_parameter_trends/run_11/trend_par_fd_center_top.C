{
//=========Macro generated from canvas: 6891e028adab4538884044d1f2027f48/
//=========  (Tue May 24 15:31:01 2016) by ROOT version5.34/26
   TCanvas *6891e028adab4538884044d1f2027f48 = new TCanvas("6891e028adab4538884044d1f2027f48", "",10,45,704,527);
   6891e028adab4538884044d1f2027f48->SetHighLightColor(2);
   6891e028adab4538884044d1f2027f48->Range(1.416657e+09,-0.7500001,1.42346e+09,6.75);
   6891e028adab4538884044d1f2027f48->SetFillColor(0);
   6891e028adab4538884044d1f2027f48->SetBorderMode(0);
   6891e028adab4538884044d1f2027f48->SetBorderSize(2);
   6891e028adab4538884044d1f2027f48->SetFrameBorderMode(0);
   6891e028adab4538884044d1f2027f48->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(6);
   gre->SetName("Graph0");
   gre->SetTitle("fd_center_top Weighted Average - run 11");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.417637e+09,2.298088);
   gre->SetPointError(0,0,0.2069068);
   gre->SetPoint(1,1.418225e+09,1.807982);
   gre->SetPointError(1,0,0.1149953);
   gre->SetPoint(2,1.418831e+09,1.773463);
   gre->SetPointError(2,0,0.1063703);
   gre->SetPoint(3,1.420566e+09,1.153429);
   gre->SetPointError(3,0,0.08328516);
   gre->SetPoint(4,1.420571e+09,1.148887);
   gre->SetPointError(4,0,0.05260417);
   gre->SetPoint(5,1.42247e+09,1.411541);
   gre->SetPointError(5,0,0.06585026);
   
   TH1F *Graph_Graph4 = new TH1F("Graph_Graph4","fd_center_top Weighted Average - run 11",100,1.417337e+09,1.42278e+09);
   Graph_Graph4->SetMinimum(0);
   Graph_Graph4->SetMaximum(6);
   Graph_Graph4->SetDirectory(0);
   Graph_Graph4->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
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
   
   gre->Draw("ap");
   
   TF1 *f_fd_center_top_0 = new TF1("f_fd_center_top_0","pol0",1.417337e+09,1.419272e+09);
   f_fd_center_top_0->SetFillColor(19);
   f_fd_center_top_0->SetFillStyle(0);
   f_fd_center_top_0->SetLineColor(2);
   f_fd_center_top_0->SetLineWidth(2);
   f_fd_center_top_0->SetChisquare(5.339805);
   f_fd_center_top_0->SetNDF(2);
   f_fd_center_top_0->GetXaxis()->SetLabelFont(42);
   f_fd_center_top_0->GetXaxis()->SetLabelSize(0.035);
   f_fd_center_top_0->GetXaxis()->SetTitleSize(0.035);
   f_fd_center_top_0->GetXaxis()->SetTitleFont(42);
   f_fd_center_top_0->GetYaxis()->SetLabelFont(42);
   f_fd_center_top_0->GetYaxis()->SetLabelSize(0.035);
   f_fd_center_top_0->GetYaxis()->SetTitleSize(0.035);
   f_fd_center_top_0->GetYaxis()->SetTitleFont(42);
   f_fd_center_top_0->SetParameter(0,1.852802);
   f_fd_center_top_0->SetParError(0,0.07305688);
   f_fd_center_top_0->SetParLimits(0,0,0);
   f_fd_center_top_0->Draw("same");
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(6);
   grae->SetName("Graph1");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#0000ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3005);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.417637e+09,1.852802);
   grae->SetPointError(0,0,0,0.156568,0.156568);
   grae->SetPoint(1,1.418225e+09,1.852802);
   grae->SetPointError(1,0,0,0.156568,0.156568);
   grae->SetPoint(2,1.418831e+09,1.852802);
   grae->SetPointError(2,0,0,0.156568,0.156568);
   grae->SetPoint(3,1.420566e+09,1.852802);
   grae->SetPointError(3,0,0,0.156568,0.156568);
   grae->SetPoint(4,1.420571e+09,1.852802);
   grae->SetPointError(4,0,0,0.156568,0.156568);
   grae->SetPoint(5,1.42247e+09,1.852802);
   grae->SetPointError(5,0,0,0.156568,0.156568);
   
   TH1F *Graph_Graph7 = new TH1F("Graph_Graph7","Graph",100,1.417154e+09,1.422953e+09);
   Graph_Graph7->SetMinimum(1.66492);
   Graph_Graph7->SetMaximum(2.040684);
   Graph_Graph7->SetDirectory(0);
   Graph_Graph7->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph7->SetLineColor(ci);
   Graph_Graph7->GetXaxis()->SetLabelFont(42);
   Graph_Graph7->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph7->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph7->GetXaxis()->SetTitleFont(42);
   Graph_Graph7->GetYaxis()->SetLabelFont(42);
   Graph_Graph7->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph7->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph7->GetYaxis()->SetTitleFont(42);
   Graph_Graph7->GetZaxis()->SetLabelFont(42);
   Graph_Graph7->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph7->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph7->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph7);
   
   grae->Draw("3 ");
   
   TPaveText *pt = new TPaveText(0.15,0.9374,0.85,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("fd_center_top Weighted Average - run 11");
   pt->Draw();
   
   TF1 *f_fd_center_top_1 = new TF1("f_fd_center_top_1","pol0",1.419272e+09,1.42278e+09);
   f_fd_center_top_1->SetFillColor(19);
   f_fd_center_top_1->SetFillStyle(0);
   f_fd_center_top_1->SetLineColor(2);
   f_fd_center_top_1->SetLineWidth(2);
   f_fd_center_top_1->SetChisquare(10.82011);
   f_fd_center_top_1->SetNDF(2);
   f_fd_center_top_1->GetXaxis()->SetLabelFont(42);
   f_fd_center_top_1->GetXaxis()->SetLabelSize(0.035);
   f_fd_center_top_1->GetXaxis()->SetTitleSize(0.035);
   f_fd_center_top_1->GetXaxis()->SetTitleFont(42);
   f_fd_center_top_1->GetYaxis()->SetLabelFont(42);
   f_fd_center_top_1->GetYaxis()->SetLabelSize(0.035);
   f_fd_center_top_1->GetYaxis()->SetTitleSize(0.035);
   f_fd_center_top_1->GetYaxis()->SetTitleFont(42);
   f_fd_center_top_1->SetParameter(0,1.232058);
   f_fd_center_top_1->SetParError(0,0.03685657);
   f_fd_center_top_1->SetParLimits(0,0,0);
   f_fd_center_top_1->Draw("same");
   
   grae = new TGraphAsymmErrors(6);
   grae->SetName("Graph2");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#0000ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3005);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.417637e+09,1.232058);
   grae->SetPointError(0,0,0,0.1124372,0.1124372);
   grae->SetPoint(1,1.418225e+09,1.232058);
   grae->SetPointError(1,0,0,0.1124372,0.1124372);
   grae->SetPoint(2,1.418831e+09,1.232058);
   grae->SetPointError(2,0,0,0.1124372,0.1124372);
   grae->SetPoint(3,1.420566e+09,1.232058);
   grae->SetPointError(3,0,0,0.1124372,0.1124372);
   grae->SetPoint(4,1.420571e+09,1.232058);
   grae->SetPointError(4,0,0,0.1124372,0.1124372);
   grae->SetPoint(5,1.42247e+09,1.232058);
   grae->SetPointError(5,0,0,0.1124372,0.1124372);
   
   TH1F *Graph_Graph8 = new TH1F("Graph_Graph8","Graph",100,1.417154e+09,1.422953e+09);
   Graph_Graph8->SetMinimum(1.097133);
   Graph_Graph8->SetMaximum(1.366982);
   Graph_Graph8->SetDirectory(0);
   Graph_Graph8->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph8->SetLineColor(ci);
   Graph_Graph8->GetXaxis()->SetLabelFont(42);
   Graph_Graph8->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph8->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph8->GetXaxis()->SetTitleFont(42);
   Graph_Graph8->GetYaxis()->SetLabelFont(42);
   Graph_Graph8->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph8->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph8->GetYaxis()->SetTitleFont(42);
   Graph_Graph8->GetZaxis()->SetLabelFont(42);
   Graph_Graph8->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph8->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph8->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph8);
   
   grae->Draw("3 ");
   6891e028adab4538884044d1f2027f48->Modified();
   6891e028adab4538884044d1f2027f48->cd();
   6891e028adab4538884044d1f2027f48->SetSelected(6891e028adab4538884044d1f2027f48);
}
