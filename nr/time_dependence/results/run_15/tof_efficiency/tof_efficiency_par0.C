{
//=========Macro generated from canvas: 74c514f6a1a24d3da1ab39f344092da0/
//=========  (Thu Feb 11 17:11:27 2016) by ROOT version5.34/26
   TCanvas *74c514f6a1a24d3da1ab39f344092da0 = new TCanvas("74c514f6a1a24d3da1ab39f344092da0", "",10,45,704,527);
   74c514f6a1a24d3da1ab39f344092da0->SetHighLightColor(2);
   74c514f6a1a24d3da1ab39f344092da0->Range(1.451357e+09,0.0491854,1.455243e+09,0.1110873);
   74c514f6a1a24d3da1ab39f344092da0->SetFillColor(0);
   74c514f6a1a24d3da1ab39f344092da0->SetBorderMode(0);
   74c514f6a1a24d3da1ab39f344092da0->SetBorderSize(2);
   74c514f6a1a24d3da1ab39f344092da0->SetFrameBorderMode(0);
   74c514f6a1a24d3da1ab39f344092da0->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(6);
   gre->SetName("Graph0");
   gre->SetTitle("tof_efficiency, par0 in Time");
   gre->SetFillColor(1);
   gre->SetPoint(0,1.452005e+09,0.06194156);
   gre->SetPointError(0,0,0.002439179);
   gre->SetPoint(1,1.452523e+09,0.07304329);
   gre->SetPointError(1,0,0.002368047);
   gre->SetPoint(2,1.453127e+09,0.07638309);
   gre->SetPointError(2,0,0.002576566);
   gre->SetPoint(3,1.453735e+09,0.07364992);
   gre->SetPointError(3,0,0.002025262);
   gre->SetPoint(4,1.453918e+09,0.09562369);
   gre->SetPointError(4,0,0.003565391);
   gre->SetPoint(5,1.454596e+09,0.09709712);
   gre->SetPointError(5,0,0.003673184);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","tof_efficiency, par0 in Time",100,1.451746e+09,1.454855e+09);
   Graph_Graph1->SetMinimum(0.05537558);
   Graph_Graph1->SetMaximum(0.1048971);
   Graph_Graph1->SetDirectory(0);
   Graph_Graph1->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph1->SetLineColor(ci);
   Graph_Graph1->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph1->GetXaxis()->SetTimeFormat("");
   Graph_Graph1->GetXaxis()->SetLabelFont(42);
   Graph_Graph1->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleFont(42);
   Graph_Graph1->GetYaxis()->SetTitle("tof_efficiency, par0 [1/PE]");
   Graph_Graph1->GetYaxis()->SetLabelFont(42);
   Graph_Graph1->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleFont(42);
   Graph_Graph1->GetZaxis()->SetLabelFont(42);
   Graph_Graph1->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1);
   
   
   TF1 *f_par0 = new TF1("f_par0","pol0",1.451746e+09,1.454855e+09);
   f_par0->SetFillColor(19);
   f_par0->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_par0->SetLineColor(ci);
   f_par0->SetLineWidth(2);
   f_par0->SetChisquare(99.32889);
   f_par0->SetNDF(5);
   f_par0->GetXaxis()->SetLabelFont(42);
   f_par0->GetXaxis()->SetLabelSize(0.035);
   f_par0->GetXaxis()->SetTitleSize(0.035);
   f_par0->GetXaxis()->SetTitleFont(42);
   f_par0->GetYaxis()->SetLabelFont(42);
   f_par0->GetYaxis()->SetLabelSize(0.035);
   f_par0->GetYaxis()->SetTitleSize(0.035);
   f_par0->GetYaxis()->SetTitleFont(42);
   f_par0->SetParameter(0,0.07566643);
   f_par0->SetParError(0,0.001057847);
   f_par0->SetParLimits(0,0,0);
   gre->GetListOfFunctions()->Add(f_par0);
   gre->Draw("ap");
   
   TF1 *f_par0 = new TF1("f_par0","pol0",0,1);
   f_par0->SetFillColor(19);
   f_par0->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_par0->SetLineColor(ci);
   f_par0->SetLineWidth(2);
   f_par0->SetChisquare(99.32889);
   f_par0->SetNDF(5);
   f_par0->GetXaxis()->SetLabelFont(42);
   f_par0->GetXaxis()->SetLabelSize(0.035);
   f_par0->GetXaxis()->SetTitleSize(0.035);
   f_par0->GetXaxis()->SetTitleFont(42);
   f_par0->GetYaxis()->SetLabelFont(42);
   f_par0->GetYaxis()->SetLabelSize(0.035);
   f_par0->GetYaxis()->SetTitleSize(0.035);
   f_par0->GetYaxis()->SetTitleFont(42);
   f_par0->SetParameter(0,0.07566643);
   f_par0->SetParError(0,0.001057847);
   f_par0->SetParLimits(0,0,0);
   f_par0->Draw("same");
   TLine *line = new TLine(1.451746e+09,0.06239115,1.454855e+09,0.06239115);

   ci = TColor::GetColor("#0000cc");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(1.451746e+09,0.08894172,1.454855e+09,0.08894172);

   ci = TColor::GetColor("#0000cc");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   TPaveText *pt = new TPaveText(0.2614286,0.9374,0.3542857,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("pol0");
   pt->Draw();
   74c514f6a1a24d3da1ab39f344092da0->Modified();
   74c514f6a1a24d3da1ab39f344092da0->cd();
   74c514f6a1a24d3da1ab39f344092da0->SetSelected(74c514f6a1a24d3da1ab39f344092da0);
}
