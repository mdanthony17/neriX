{
//=========Macro generated from canvas: 6ea6e121a0c7445d80d8d5b520cc66ff/
//=========  (Tue May 24 15:30:57 2016) by ROOT version5.34/26
   TCanvas *6ea6e121a0c7445d80d8d5b520cc66ff = new TCanvas("6ea6e121a0c7445d80d8d5b520cc66ff", "",10,45,704,527);
   6ea6e121a0c7445d80d8d5b520cc66ff->SetHighLightColor(2);
   6ea6e121a0c7445d80d8d5b520cc66ff->Range(1.416657e+09,-0.5,1.42346e+09,4.5);
   6ea6e121a0c7445d80d8d5b520cc66ff->SetFillColor(0);
   6ea6e121a0c7445d80d8d5b520cc66ff->SetBorderMode(0);
   6ea6e121a0c7445d80d8d5b520cc66ff->SetBorderSize(2);
   6ea6e121a0c7445d80d8d5b520cc66ff->SetFrameBorderMode(0);
   6ea6e121a0c7445d80d8d5b520cc66ff->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(6);
   gre->SetName("Graph0");
   gre->SetTitle("fd_shape_top Weighted Average - run 11");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.417637e+09,1.563686);
   gre->SetPointError(0,0,0.2319241);
   gre->SetPoint(1,1.418225e+09,0.8523064);
   gre->SetPointError(1,0,0.09444671);
   gre->SetPoint(2,1.418831e+09,0.9354301);
   gre->SetPointError(2,0,0.09907288);
   gre->SetPoint(3,1.420566e+09,0.5368619);
   gre->SetPointError(3,0,0.1540944);
   gre->SetPoint(4,1.420571e+09,0.3787481);
   gre->SetPointError(4,0,0.06743326);
   gre->SetPoint(5,1.42247e+09,0.5795813);
   gre->SetPointError(5,0,0.06826613);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","fd_shape_top Weighted Average - run 11",100,1.417337e+09,1.42278e+09);
   Graph_Graph1->SetMinimum(0);
   Graph_Graph1->SetMaximum(4);
   Graph_Graph1->SetDirectory(0);
   Graph_Graph1->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
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
   gre->SetHistogram(Graph_Graph1);
   
   gre->Draw("ap");
   
   TF1 *f_fd_shape_top_0 = new TF1("f_fd_shape_top_0","pol0",1.417337e+09,1.419272e+09);
   f_fd_shape_top_0->SetFillColor(19);
   f_fd_shape_top_0->SetFillStyle(0);
   f_fd_shape_top_0->SetLineColor(2);
   f_fd_shape_top_0->SetLineWidth(2);
   f_fd_shape_top_0->SetChisquare(8.088687);
   f_fd_shape_top_0->SetNDF(2);
   f_fd_shape_top_0->GetXaxis()->SetLabelFont(42);
   f_fd_shape_top_0->GetXaxis()->SetLabelSize(0.035);
   f_fd_shape_top_0->GetXaxis()->SetTitleSize(0.035);
   f_fd_shape_top_0->GetXaxis()->SetTitleFont(42);
   f_fd_shape_top_0->GetYaxis()->SetLabelFont(42);
   f_fd_shape_top_0->GetYaxis()->SetLabelSize(0.035);
   f_fd_shape_top_0->GetYaxis()->SetTitleSize(0.035);
   f_fd_shape_top_0->GetYaxis()->SetTitleFont(42);
   f_fd_shape_top_0->SetParameter(0,0.9455835);
   f_fd_shape_top_0->SetParError(0,0.06557174);
   f_fd_shape_top_0->SetParLimits(0,0,0);
   f_fd_shape_top_0->Draw("same");
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(6);
   grae->SetName("Graph1");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#0000ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3005);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.417637e+09,0.9455835);
   grae->SetPointError(0,0,0,0.1729557,0.1729557);
   grae->SetPoint(1,1.418225e+09,0.9455835);
   grae->SetPointError(1,0,0,0.1729557,0.1729557);
   grae->SetPoint(2,1.418831e+09,0.9455835);
   grae->SetPointError(2,0,0,0.1729557,0.1729557);
   grae->SetPoint(3,1.420566e+09,0.9455835);
   grae->SetPointError(3,0,0,0.1729557,0.1729557);
   grae->SetPoint(4,1.420571e+09,0.9455835);
   grae->SetPointError(4,0,0,0.1729557,0.1729557);
   grae->SetPoint(5,1.42247e+09,0.9455835);
   grae->SetPointError(5,0,0,0.1729557,0.1729557);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","Graph",100,1.417154e+09,1.422953e+09);
   Graph_Graph1->SetMinimum(0.7380366);
   Graph_Graph1->SetMaximum(1.15313);
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
   TText *text = pt->AddText("fd_shape_top Weighted Average - run 11");
   pt->Draw();
   
   TF1 *f_fd_shape_top_1 = new TF1("f_fd_shape_top_1","pol0",1.419272e+09,1.42278e+09);
   f_fd_shape_top_1->SetFillColor(19);
   f_fd_shape_top_1->SetFillStyle(0);
   f_fd_shape_top_1->SetLineColor(2);
   f_fd_shape_top_1->SetLineWidth(2);
   f_fd_shape_top_1->SetChisquare(4.513881);
   f_fd_shape_top_1->SetNDF(2);
   f_fd_shape_top_1->GetXaxis()->SetLabelFont(42);
   f_fd_shape_top_1->GetXaxis()->SetLabelSize(0.035);
   f_fd_shape_top_1->GetXaxis()->SetTitleSize(0.035);
   f_fd_shape_top_1->GetXaxis()->SetTitleFont(42);
   f_fd_shape_top_1->GetYaxis()->SetLabelFont(42);
   f_fd_shape_top_1->GetYaxis()->SetLabelSize(0.035);
   f_fd_shape_top_1->GetYaxis()->SetTitleSize(0.035);
   f_fd_shape_top_1->GetYaxis()->SetTitleFont(42);
   f_fd_shape_top_1->SetParameter(0,0.4831392);
   f_fd_shape_top_1->SetParError(0,0.0458057);
   f_fd_shape_top_1->SetParLimits(0,0,0);
   f_fd_shape_top_1->Draw("same");
   
   grae = new TGraphAsymmErrors(6);
   grae->SetName("Graph2");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#0000ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3005);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,1.417637e+09,0.4831392);
   grae->SetPointError(0,0,0,0.09025554,0.09025554);
   grae->SetPoint(1,1.418225e+09,0.4831392);
   grae->SetPointError(1,0,0,0.09025554,0.09025554);
   grae->SetPoint(2,1.418831e+09,0.4831392);
   grae->SetPointError(2,0,0,0.09025554,0.09025554);
   grae->SetPoint(3,1.420566e+09,0.4831392);
   grae->SetPointError(3,0,0,0.09025554,0.09025554);
   grae->SetPoint(4,1.420571e+09,0.4831392);
   grae->SetPointError(4,0,0,0.09025554,0.09025554);
   grae->SetPoint(5,1.42247e+09,0.4831392);
   grae->SetPointError(5,0,0,0.09025554,0.09025554);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Graph",100,1.417154e+09,1.422953e+09);
   Graph_Graph2->SetMinimum(0.3748326);
   Graph_Graph2->SetMaximum(0.5914459);
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
   6ea6e121a0c7445d80d8d5b520cc66ff->Modified();
   6ea6e121a0c7445d80d8d5b520cc66ff->cd();
   6ea6e121a0c7445d80d8d5b520cc66ff->SetSelected(6ea6e121a0c7445d80d8d5b520cc66ff);
}
