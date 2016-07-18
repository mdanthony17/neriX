{
//=========Macro generated from canvas: 96991507f8b64955bdf70f68a09b0cb9/
//=========  (Fri Jul  8 12:37:54 2016) by ROOT version5.34/26
   TCanvas *96991507f8b64955bdf70f68a09b0cb9 = new TCanvas("96991507f8b64955bdf70f68a09b0cb9", "",10,45,604,527);
   96991507f8b64955bdf70f68a09b0cb9->SetHighLightColor(2);
   96991507f8b64955bdf70f68a09b0cb9->Range(-165,0.2474347,1557,0.9052564);
   96991507f8b64955bdf70f68a09b0cb9->SetFillColor(0);
   96991507f8b64955bdf70f68a09b0cb9->SetBorderMode(0);
   96991507f8b64955bdf70f68a09b0cb9->SetBorderSize(2);
   96991507f8b64955bdf70f68a09b0cb9->SetFrameBorderMode(0);
   96991507f8b64955bdf70f68a09b0cb9->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(4);
   gre->SetName("Graph0");
   gre->SetTitle("HPGe Width");
   gre->SetFillColor(1);
   gre->SetMarkerStyle(32);
   gre->SetPoint(0,122,0.362906);
   gre->SetPointError(0,0,0.005834357);
   gre->SetPoint(1,137,0.3904898);
   gre->SetPointError(1,0,0.007196893);
   gre->SetPoint(2,662,0.5572195);
   gre->SetPointError(2,0,0.02006651);
   gre->SetPoint(3,1270,0.7615827);
   gre->SetPointError(3,0,0.03403675);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","HPGe Width",100,7.2,1384.8);
   Graph_Graph2->SetMinimum(0.3132169);
   Graph_Graph2->SetMaximum(0.8394742);
   Graph_Graph2->SetDirectory(0);
   Graph_Graph2->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph2->SetLineColor(ci);
   Graph_Graph2->GetXaxis()->SetTitle("Energy [keV]");
   Graph_Graph2->GetXaxis()->SetLabelFont(42);
   Graph_Graph2->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph2->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph2->GetXaxis()->SetTitleFont(42);
   Graph_Graph2->GetYaxis()->SetTitle("Width [keV]");
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
   
   TF1 *f_width = new TF1("f_width","[0]+[1]*(x)^0.5",0,1500);
   f_width->SetFillColor(19);
   f_width->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_width->SetLineColor(ci);
   f_width->SetLineWidth(2);
   f_width->SetLineStyle(2);
   f_width->SetChisquare(6.463651);
   f_width->SetNDF(2);
   f_width->GetXaxis()->SetLabelFont(42);
   f_width->GetXaxis()->SetLabelSize(0.035);
   f_width->GetXaxis()->SetTitleSize(0.035);
   f_width->GetXaxis()->SetTitleFont(42);
   f_width->GetYaxis()->SetLabelFont(42);
   f_width->GetYaxis()->SetLabelSize(0.035);
   f_width->GetYaxis()->SetTitleSize(0.035);
   f_width->GetYaxis()->SetTitleFont(42);
   f_width->SetParameter(0,0.2094132);
   f_width->SetParError(0,0.01332714);
   f_width->SetParLimits(0,0,0);
   f_width->SetParameter(1,0.01448656);
   f_width->SetParError(1,0.001015048);
   f_width->SetParLimits(1,0,0);
   f_width->Draw("same");
   
   TPaveText *pt = new TPaveText(0.2,0.65,0.55,0.8,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   TText *text = pt->AddText("#sigma = #alpha + #beta#sqrt{E}");
   text = pt->AddText("#alpha = 2.09e-01 #pm 1.33e-02 keV");
   text = pt->AddText("#beta = 1.45e-02 #pm 1.02e-03 keV^{#frac{1}{2}}");
   pt->Draw();
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(4);
   grae->SetName("Graph1");
   grae->SetTitle("Graph");

   ci = TColor::GetColor("#0000ff");
   grae->SetFillColor(ci);
   grae->SetFillStyle(3005);

   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetPoint(0,122,0.3694224);
   grae->SetPointError(0,0,0,0.01083254,0.01083254);
   grae->SetPoint(1,137,0.378974);
   grae->SetPointError(1,0,0,0.0104695,0.0104695);
   grae->SetPoint(2,662,0.582143);
   grae->SetPointError(2,0,0,0.03353719,0.03353719);
   grae->SetPoint(3,1270,0.7256715);
   grae->SetPointError(3,0,0,0.05656938,0.05656938);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","Graph",100,7.2,1384.8);
   Graph_Graph1->SetMinimum(0.3162248);
   Graph_Graph1->SetMaximum(0.8246059);
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
   
   pt = new TPaveText(0.37,0.94,0.63,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("HPGe Width");
   pt->Draw();
   96991507f8b64955bdf70f68a09b0cb9->Modified();
   96991507f8b64955bdf70f68a09b0cb9->cd();
   96991507f8b64955bdf70f68a09b0cb9->SetSelected(96991507f8b64955bdf70f68a09b0cb9);
}
