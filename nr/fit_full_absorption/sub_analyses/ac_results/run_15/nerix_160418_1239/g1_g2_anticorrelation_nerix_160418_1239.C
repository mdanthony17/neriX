{
//=========Macro generated from canvas: 2513eb4269664ffb993cdb4d3823d9d1/
//=========  (Tue Apr 19 11:43:59 2016) by ROOT version5.34/26
   TCanvas *2513eb4269664ffb993cdb4d3823d9d1 = new TCanvas("2513eb4269664ffb993cdb4d3823d9d1", "",10,45,704,527);
   2513eb4269664ffb993cdb4d3823d9d1->SetHighLightColor(2);
   2513eb4269664ffb993cdb4d3823d9d1->Range(181268.8,783.671,402853.3,2197.115);
   2513eb4269664ffb993cdb4d3823d9d1->SetFillColor(0);
   2513eb4269664ffb993cdb4d3823d9d1->SetBorderMode(0);
   2513eb4269664ffb993cdb4d3823d9d1->SetBorderSize(2);
   2513eb4269664ffb993cdb4d3823d9d1->SetFrameBorderMode(0);
   2513eb4269664ffb993cdb4d3823d9d1->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,272522.2,1529.781);
   gre->SetPointError(0,1487.403,8.361411);
   gre->SetPoint(1,298533.9,1359.381);
   gre->SetPointError(1,1808.173,9.428078);
   gre->SetPoint(2,316380.6,1263.3);
   gre->SetPointError(2,1286.594,7.199374);
   gre->SetPoint(3,323924.8,1192.77);
   gre->SetPointError(3,1291.087,7.026941);
   gre->SetPoint(4,338168,1079.537);
   gre->SetPointError(4,1095.399,5.346606);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,203427.2,380694.8);
   Graph_Graph1->SetMinimum(925.0155);
   Graph_Graph1->SetMaximum(2055.771);
   Graph_Graph1->SetDirectory(0);
   Graph_Graph1->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph1->SetLineColor(ci);
   Graph_Graph1->GetXaxis()->SetTitle("S2 at 511 keV Peak");
   Graph_Graph1->GetXaxis()->SetLabelFont(42);
   Graph_Graph1->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleOffset(1.12);
   Graph_Graph1->GetXaxis()->SetTitleFont(42);
   Graph_Graph1->GetYaxis()->SetTitle("S1 at 511 keV Peak [PE]");
   Graph_Graph1->GetYaxis()->SetLabelFont(42);
   Graph_Graph1->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleFont(42);
   Graph_Graph1->GetZaxis()->SetLabelFont(42);
   Graph_Graph1->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1);
   
   
   TF1 *fG1 = new TF1("fG1","pol1",264212,346086.2);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(6.509743);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,3412.588);
   fG1->SetParError(0,72.96604);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.006866216);
   fG1->SetParError(1,0.0002301999);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0915 +/- 0.0045");
   text = pt->AddText("g2 = 13.3250 +/- 0.7882");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(289625.6,1432.224,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(342945.5,1066.117,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(226030.3,1868.883,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",203427.2,380694.8);
   fDraw->SetFillColor(19);
   fDraw->SetFillStyle(0);
   fDraw->SetLineColor(2);
   fDraw->SetLineWidth(1);
   fDraw->SetLineStyle(9);
   fDraw->GetXaxis()->SetLabelFont(42);
   fDraw->GetXaxis()->SetLabelSize(0.035);
   fDraw->GetXaxis()->SetTitleSize(0.035);
   fDraw->GetXaxis()->SetTitleFont(42);
   fDraw->GetYaxis()->SetLabelFont(42);
   fDraw->GetYaxis()->SetLabelSize(0.035);
   fDraw->GetYaxis()->SetTitleSize(0.035);
   fDraw->GetYaxis()->SetTitleFont(42);
   fDraw->SetParameter(0,3412.588);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.006866216);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   2513eb4269664ffb993cdb4d3823d9d1->Modified();
   2513eb4269664ffb993cdb4d3823d9d1->cd();
   2513eb4269664ffb993cdb4d3823d9d1->SetSelected(2513eb4269664ffb993cdb4d3823d9d1);
}
