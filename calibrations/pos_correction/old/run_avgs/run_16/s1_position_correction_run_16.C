{
//=========Macro generated from canvas: 515419f19f2e4f069146204745b98d47/
//=========  (Tue Jun 14 16:20:21 2016) by ROOT version5.34/26
   TCanvas *515419f19f2e4f069146204745b98d47 = new TCanvas("515419f19f2e4f069146204745b98d47", "",10,45,1604,1035);
   515419f19f2e4f069146204745b98d47->SetHighLightColor(2);
   515419f19f2e4f069146204745b98d47->Range(0,0,1,1);
   515419f19f2e4f069146204745b98d47->SetFillColor(0);
   515419f19f2e4f069146204745b98d47->SetBorderMode(0);
   515419f19f2e4f069146204745b98d47->SetBorderSize(2);
   515419f19f2e4f069146204745b98d47->SetFrameBorderMode(0);
  
// ------------>Primitives in pad: 515419f19f2e4f069146204745b98d47_1
   TPad *515419f19f2e4f069146204745b98d47_1 = new TPad("515419f19f2e4f069146204745b98d47_1", "515419f19f2e4f069146204745b98d47_1",0.01,0.51,0.49,0.99);
   515419f19f2e4f069146204745b98d47_1->Draw();
   515419f19f2e4f069146204745b98d47_1->cd();
   515419f19f2e4f069146204745b98d47_1->Range(1.458957e+09,0.65,1.464245e+09,1.15);
   515419f19f2e4f069146204745b98d47_1->SetFillColor(0);
   515419f19f2e4f069146204745b98d47_1->SetBorderMode(0);
   515419f19f2e4f069146204745b98d47_1->SetBorderSize(2);
   515419f19f2e4f069146204745b98d47_1->SetFrameBorderMode(0);
   515419f19f2e4f069146204745b98d47_1->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(7);
   gre->SetName("Graph0");
   gre->SetTitle("s1_z_intercept");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#ff00ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff00ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,0.8426013);
   gre->SetPointError(0,0,0.04308127);
   gre->SetPoint(1,1.46037e+09,0.8400895);
   gre->SetPointError(1,0,0.04368344);
   gre->SetPoint(2,1.46099e+09,0.8423295);
   gre->SetPointError(2,0,0.03680111);
   gre->SetPoint(3,1.4616e+09,0.8392132);
   gre->SetPointError(3,0,0.04430622);
   gre->SetPoint(4,1.462201e+09,0.8022139);
   gre->SetPointError(4,0,0.03732823);
   gre->SetPoint(5,1.462809e+09,0.8562663);
   gre->SetPointError(5,0,0.04119692);
   gre->SetPoint(6,1.463416e+09,0.8529864);
   gre->SetPointError(6,0,0.03601138);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","s1_z_intercept",100,1.459486e+09,1.463716e+09);
   Graph_Graph1->SetMinimum(0.7);
   Graph_Graph1->SetMaximum(1.1);
   Graph_Graph1->SetDirectory(0);
   Graph_Graph1->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph1->SetLineColor(ci);
   Graph_Graph1->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph1->GetXaxis()->SetTimeFormat("%m-%d%F1995-01-01 00:00:00s0");
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
   
   TF1 *f_dv_0.345 = new TF1("f_dv_0.345","pol0",1.459486e+09,1.463716e+09);
   f_dv_0.345->SetFillColor(19);
   f_dv_0.345->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   f_dv_0.345->SetLineColor(ci);
   f_dv_0.345->SetLineWidth(2);
   f_dv_0.345->SetChisquare(1.313458);
   f_dv_0.345->SetNDF(6);
   f_dv_0.345->GetXaxis()->SetLabelFont(42);
   f_dv_0.345->GetXaxis()->SetLabelSize(0.035);
   f_dv_0.345->GetXaxis()->SetTitleSize(0.035);
   f_dv_0.345->GetXaxis()->SetTitleFont(42);
   f_dv_0.345->GetYaxis()->SetLabelFont(42);
   f_dv_0.345->GetYaxis()->SetLabelSize(0.035);
   f_dv_0.345->GetYaxis()->SetTitleSize(0.035);
   f_dv_0.345->GetYaxis()->SetTitleFont(42);
   f_dv_0.345->SetParameter(0,0.8389172);
   f_dv_0.345->SetParError(0,0.01509509);
   f_dv_0.345->SetParLimits(0,0,0);
   f_dv_0.345->Draw("same");
   
   TPaveText *pt = new TPaveText(0.15,0.8,0.5,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("V_{c} = 0.345kV: 8.39e-01 +/- 1.51e-02 mm/s");
   pt->Draw();
   
   pt = new TPaveText(0.3819271,0.9379564,0.6180729,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("s1_z_intercept");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph1");
   gre->SetTitle("s1_z_intercept");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,0.8761739);
   gre->SetPointError(0,0,0.0373272);
   gre->SetPoint(1,1.46037e+09,0.8308474);
   gre->SetPointError(1,0,0.03882618);
   gre->SetPoint(2,1.46099e+09,0.8887168);
   gre->SetPointError(2,0,0.04387525);
   gre->SetPoint(3,1.4616e+09,0.8522199);
   gre->SetPointError(3,0,0.04036729);
   gre->SetPoint(4,1.462201e+09,0.8330345);
   gre->SetPointError(4,0,0.03693614);
   gre->SetPoint(5,1.462809e+09,0.8767182);
   gre->SetPointError(5,0,0.04584022);
   gre->SetPoint(6,1.463416e+09,0.8640263);
   gre->SetPointError(6,0,0.04492762);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","s1_z_intercept",100,1.459486e+09,1.463716e+09);
   Graph_Graph2->SetMinimum(0.7);
   Graph_Graph2->SetMaximum(1.1);
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
   gre->SetHistogram(Graph_Graph2);
   
   gre->Draw("p ");
   
   TF1 *f_dv_0.7 = new TF1("f_dv_0.7","pol0",1.459486e+09,1.463716e+09);
   f_dv_0.7->SetFillColor(19);
   f_dv_0.7->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_dv_0.7->SetLineColor(ci);
   f_dv_0.7->SetLineWidth(2);
   f_dv_0.7->SetChisquare(1.878089);
   f_dv_0.7->SetNDF(6);
   f_dv_0.7->GetXaxis()->SetLabelFont(42);
   f_dv_0.7->GetXaxis()->SetLabelSize(0.035);
   f_dv_0.7->GetXaxis()->SetTitleSize(0.035);
   f_dv_0.7->GetXaxis()->SetTitleFont(42);
   f_dv_0.7->GetYaxis()->SetLabelFont(42);
   f_dv_0.7->GetYaxis()->SetLabelSize(0.035);
   f_dv_0.7->GetYaxis()->SetTitleSize(0.035);
   f_dv_0.7->GetYaxis()->SetTitleFont(42);
   f_dv_0.7->SetParameter(0,0.8582453);
   f_dv_0.7->SetParError(0,0.01539643);
   f_dv_0.7->SetParLimits(0,0,0);
   f_dv_0.7->Draw("same");
   
   pt = new TPaveText(0.15,0.75,0.5,0.8,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 0.7kV: 8.58e-01 +/- 1.54e-02 mm/s");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph2");
   gre->SetTitle("s1_z_intercept");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#00cc00");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#00cc00");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,0.9260755);
   gre->SetPointError(0,0,0.04310605);
   gre->SetPoint(1,1.46037e+09,0.8694217);
   gre->SetPointError(1,0,0.04217586);
   gre->SetPoint(2,1.46099e+09,0.8848405);
   gre->SetPointError(2,0,0.043077);
   gre->SetPoint(3,1.4616e+09,0.8580409);
   gre->SetPointError(3,0,0.03829671);
   gre->SetPoint(4,1.462201e+09,0.8658334);
   gre->SetPointError(4,0,0.03908604);
   gre->SetPoint(5,1.462809e+09,0.8774521);
   gre->SetPointError(5,0,0.03651311);
   gre->SetPoint(6,1.463416e+09,0.8939203);
   gre->SetPointError(6,0,0.0371927);
   
   TH1F *Graph_Graph3 = new TH1F("Graph_Graph3","s1_z_intercept",100,1.459486e+09,1.463716e+09);
   Graph_Graph3->SetMinimum(0.7);
   Graph_Graph3->SetMaximum(1.1);
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
   gre->SetHistogram(Graph_Graph3);
   
   gre->Draw("p ");
   
   TF1 *f_dv_1.054 = new TF1("f_dv_1.054","pol0",1.459486e+09,1.463716e+09);
   f_dv_1.054->SetFillColor(19);
   f_dv_1.054->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   f_dv_1.054->SetLineColor(ci);
   f_dv_1.054->SetLineWidth(2);
   f_dv_1.054->SetChisquare(1.816682);
   f_dv_1.054->SetNDF(6);
   f_dv_1.054->GetXaxis()->SetLabelFont(42);
   f_dv_1.054->GetXaxis()->SetLabelSize(0.035);
   f_dv_1.054->GetXaxis()->SetTitleSize(0.035);
   f_dv_1.054->GetXaxis()->SetTitleFont(42);
   f_dv_1.054->GetYaxis()->SetLabelFont(42);
   f_dv_1.054->GetYaxis()->SetLabelSize(0.035);
   f_dv_1.054->GetYaxis()->SetTitleSize(0.035);
   f_dv_1.054->GetYaxis()->SetTitleFont(42);
   f_dv_1.054->SetParameter(0,0.881206);
   f_dv_1.054->SetParError(0,0.01499293);
   f_dv_1.054->SetParLimits(0,0,0);
   f_dv_1.054->Draw("same");
   
   pt = new TPaveText(0.15,0.7,0.5,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.054kV: 8.81e-01 +/- 1.50e-02 mm/s");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph3");
   gre->SetTitle("s1_z_intercept");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff9933");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff9933");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,0.8947566);
   gre->SetPointError(0,0,0.03910993);
   gre->SetPoint(1,1.46037e+09,0.8919258);
   gre->SetPointError(1,0,0.04225525);
   gre->SetPoint(2,1.46099e+09,0.8727288);
   gre->SetPointError(2,0,0.04465149);
   gre->SetPoint(3,1.4616e+09,0.8886229);
   gre->SetPointError(3,0,0.04055998);
   gre->SetPoint(4,1.462201e+09,0.8764414);
   gre->SetPointError(4,0,0.04008758);
   gre->SetPoint(5,1.462809e+09,0.8944186);
   gre->SetPointError(5,0,0.03572729);
   gre->SetPoint(6,1.463416e+09,0.8844715);
   gre->SetPointError(6,0,0.04441986);
   
   TH1F *Graph_Graph4 = new TH1F("Graph_Graph4","s1_z_intercept",100,1.459486e+09,1.463716e+09);
   Graph_Graph4->SetMinimum(0.7);
   Graph_Graph4->SetMaximum(1.1);
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
   gre->SetHistogram(Graph_Graph4);
   
   gre->Draw("p ");
   
   TF1 *f_dv_1.5 = new TF1("f_dv_1.5","pol0",1.459486e+09,1.463716e+09);
   f_dv_1.5->SetFillColor(19);
   f_dv_1.5->SetFillStyle(0);

   ci = TColor::GetColor("#ff9933");
   f_dv_1.5->SetLineColor(ci);
   f_dv_1.5->SetLineWidth(2);
   f_dv_1.5->SetChisquare(0.2723732);
   f_dv_1.5->SetNDF(6);
   f_dv_1.5->GetXaxis()->SetLabelFont(42);
   f_dv_1.5->GetXaxis()->SetLabelSize(0.035);
   f_dv_1.5->GetXaxis()->SetTitleSize(0.035);
   f_dv_1.5->GetXaxis()->SetTitleFont(42);
   f_dv_1.5->GetYaxis()->SetLabelFont(42);
   f_dv_1.5->GetYaxis()->SetLabelSize(0.035);
   f_dv_1.5->GetYaxis()->SetTitleSize(0.035);
   f_dv_1.5->GetYaxis()->SetTitleFont(42);
   f_dv_1.5->SetParameter(0,0.8869099);
   f_dv_1.5->SetParError(0,0.01536503);
   f_dv_1.5->SetParLimits(0,0,0);
   f_dv_1.5->Draw("same");
   
   pt = new TPaveText(0.15,0.65,0.5,0.7,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff9933");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.5kV: 8.87e-01 +/- 1.54e-02 mm/s");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph4");
   gre->SetTitle("s1_z_intercept");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff0000");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff0000");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,0.8886511);
   gre->SetPointError(0,0,0.0395873);
   gre->SetPoint(1,1.46037e+09,0.8762229);
   gre->SetPointError(1,0,0.04150852);
   gre->SetPoint(2,1.46099e+09,0.8871528);
   gre->SetPointError(2,0,0.0402097);
   gre->SetPoint(3,1.4616e+09,0.8910852);
   gre->SetPointError(3,0,0.04466951);
   gre->SetPoint(4,1.462201e+09,0.8983722);
   gre->SetPointError(4,0,0.03922656);
   gre->SetPoint(5,1.462809e+09,0.9181055);
   gre->SetPointError(5,0,0.03579448);
   gre->SetPoint(6,1.463416e+09,0.9338072);
   gre->SetPointError(6,0,0.03683922);
   
   TH1F *Graph_Graph5 = new TH1F("Graph_Graph5","s1_z_intercept",100,1.459486e+09,1.463716e+09);
   Graph_Graph5->SetMinimum(0.7);
   Graph_Graph5->SetMaximum(1.1);
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
   gre->SetHistogram(Graph_Graph5);
   
   gre->Draw("p ");
   
   TF1 *f_dv_2.356 = new TF1("f_dv_2.356","pol0",1.459486e+09,1.463716e+09);
   f_dv_2.356->SetFillColor(19);
   f_dv_2.356->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   f_dv_2.356->SetLineColor(ci);
   f_dv_2.356->SetLineWidth(2);
   f_dv_2.356->SetChisquare(1.647415);
   f_dv_2.356->SetNDF(6);
   f_dv_2.356->GetXaxis()->SetLabelFont(42);
   f_dv_2.356->GetXaxis()->SetLabelSize(0.035);
   f_dv_2.356->GetXaxis()->SetTitleSize(0.035);
   f_dv_2.356->GetXaxis()->SetTitleFont(42);
   f_dv_2.356->GetYaxis()->SetLabelFont(42);
   f_dv_2.356->GetYaxis()->SetLabelSize(0.035);
   f_dv_2.356->GetYaxis()->SetTitleSize(0.035);
   f_dv_2.356->GetYaxis()->SetTitleFont(42);
   f_dv_2.356->SetParameter(0,0.9010051);
   f_dv_2.356->SetParError(0,0.01489821);
   f_dv_2.356->SetParLimits(0,0,0);
   f_dv_2.356->Draw("same");
   
   pt = new TPaveText(0.15,0.6,0.5,0.65,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 2.356kV: 9.01e-01 +/- 1.49e-02 mm/s");
   pt->Draw();
   515419f19f2e4f069146204745b98d47_1->Modified();
   515419f19f2e4f069146204745b98d47->cd();
  
// ------------>Primitives in pad: 515419f19f2e4f069146204745b98d47_2
   515419f19f2e4f069146204745b98d47_2 = new TPad("515419f19f2e4f069146204745b98d47_2", "515419f19f2e4f069146204745b98d47_2",0.51,0.51,0.99,0.99);
   515419f19f2e4f069146204745b98d47_2->Draw();
   515419f19f2e4f069146204745b98d47_2->cd();
   515419f19f2e4f069146204745b98d47_2->Range(1.458957e+09,-0.035,1.464245e+09,0.015);
   515419f19f2e4f069146204745b98d47_2->SetFillColor(0);
   515419f19f2e4f069146204745b98d47_2->SetBorderMode(0);
   515419f19f2e4f069146204745b98d47_2->SetBorderSize(2);
   515419f19f2e4f069146204745b98d47_2->SetFrameBorderMode(0);
   515419f19f2e4f069146204745b98d47_2->SetFrameBorderMode(0);
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph0");
   gre->SetTitle("s1_z_slope");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff00ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff00ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,-0.01232603);
   gre->SetPointError(0,0,0.003311364);
   gre->SetPoint(1,1.46037e+09,-0.01136165);
   gre->SetPointError(1,0,0.003095846);
   gre->SetPoint(2,1.46099e+09,-0.01237626);
   gre->SetPointError(2,0,0.002843912);
   gre->SetPoint(3,1.4616e+09,-0.01307055);
   gre->SetPointError(3,0,0.002959206);
   gre->SetPoint(4,1.462201e+09,-0.01588953);
   gre->SetPointError(4,0,0.002913762);
   gre->SetPoint(5,1.462809e+09,-0.01086521);
   gre->SetPointError(5,0,0.003051438);
   gre->SetPoint(6,1.463416e+09,-0.01158775);
   gre->SetPointError(6,0,0.002622065);
   
   TH1F *Graph_Graph6 = new TH1F("Graph_Graph6","s1_z_slope",100,1.459486e+09,1.463716e+09);
   Graph_Graph6->SetMinimum(-0.03);
   Graph_Graph6->SetMaximum(0.01);
   Graph_Graph6->SetDirectory(0);
   Graph_Graph6->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph6->SetLineColor(ci);
   Graph_Graph6->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph6->GetXaxis()->SetTimeFormat("%m-%d%F1995-01-01 00:00:00s0");
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
   gre->SetHistogram(Graph_Graph6);
   
   gre->Draw("ap");
   
   TF1 *f_dv_0.345 = new TF1("f_dv_0.345","pol0",1.459486e+09,1.463716e+09);
   f_dv_0.345->SetFillColor(19);
   f_dv_0.345->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   f_dv_0.345->SetLineColor(ci);
   f_dv_0.345->SetLineWidth(2);
   f_dv_0.345->SetChisquare(1.938281);
   f_dv_0.345->SetNDF(6);
   f_dv_0.345->GetXaxis()->SetLabelFont(42);
   f_dv_0.345->GetXaxis()->SetLabelSize(0.035);
   f_dv_0.345->GetXaxis()->SetTitleSize(0.035);
   f_dv_0.345->GetXaxis()->SetTitleFont(42);
   f_dv_0.345->GetYaxis()->SetLabelFont(42);
   f_dv_0.345->GetYaxis()->SetLabelSize(0.035);
   f_dv_0.345->GetYaxis()->SetTitleSize(0.035);
   f_dv_0.345->GetYaxis()->SetTitleFont(42);
   f_dv_0.345->SetParameter(0,-0.01250771);
   f_dv_0.345->SetParError(0,0.001115245);
   f_dv_0.345->SetParLimits(0,0,0);
   f_dv_0.345->Draw("same");
   
   pt = new TPaveText(0.15,0.8,0.5,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 0.345kV: -1.25e-02 +/- 1.12e-03 mm/s");
   pt->Draw();
   
   pt = new TPaveText(0.4066667,0.9379564,0.5933333,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("s1_z_slope");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph1");
   gre->SetTitle("s1_z_slope");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,-0.009252117);
   gre->SetPointError(0,0,0.002446111);
   gre->SetPoint(1,1.46037e+09,-0.01259002);
   gre->SetPointError(1,0,0.00279609);
   gre->SetPoint(2,1.46099e+09,-0.008453963);
   gre->SetPointError(2,0,0.002995586);
   gre->SetPoint(3,1.4616e+09,-0.01111554);
   gre->SetPointError(3,0,0.002937279);
   gre->SetPoint(4,1.462201e+09,-0.01288467);
   gre->SetPointError(4,0,0.00268411);
   gre->SetPoint(5,1.462809e+09,-0.009893152);
   gre->SetPointError(5,0,0.003522783);
   gre->SetPoint(6,1.463416e+09,-0.01037161);
   gre->SetPointError(6,0,0.003380633);
   
   TH1F *Graph_Graph7 = new TH1F("Graph_Graph7","s1_z_slope",100,1.459486e+09,1.463716e+09);
   Graph_Graph7->SetMinimum(-0.03);
   Graph_Graph7->SetMaximum(0.01);
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
   gre->SetHistogram(Graph_Graph7);
   
   gre->Draw("p ");
   
   TF1 *f_dv_0.7 = new TF1("f_dv_0.7","pol0",1.459486e+09,1.463716e+09);
   f_dv_0.7->SetFillColor(19);
   f_dv_0.7->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_dv_0.7->SetLineColor(ci);
   f_dv_0.7->SetLineWidth(2);
   f_dv_0.7->SetChisquare(2.113767);
   f_dv_0.7->SetNDF(6);
   f_dv_0.7->GetXaxis()->SetLabelFont(42);
   f_dv_0.7->GetXaxis()->SetLabelSize(0.035);
   f_dv_0.7->GetXaxis()->SetTitleSize(0.035);
   f_dv_0.7->GetXaxis()->SetTitleFont(42);
   f_dv_0.7->GetYaxis()->SetLabelFont(42);
   f_dv_0.7->GetYaxis()->SetLabelSize(0.035);
   f_dv_0.7->GetYaxis()->SetTitleSize(0.035);
   f_dv_0.7->GetYaxis()->SetTitleFont(42);
   f_dv_0.7->SetParameter(0,-0.01070859);
   f_dv_0.7->SetParError(0,0.00109813);
   f_dv_0.7->SetParLimits(0,0,0);
   f_dv_0.7->Draw("same");
   
   pt = new TPaveText(0.15,0.75,0.5,0.8,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 0.7kV: -1.07e-02 +/- 1.10e-03 mm/s");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph2");
   gre->SetTitle("s1_z_slope");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#00cc00");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#00cc00");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,-0.005239605);
   gre->SetPointError(0,0,0.002582981);
   gre->SetPoint(1,1.46037e+09,-0.01043422);
   gre->SetPointError(1,0,0.003064094);
   gre->SetPoint(2,1.46099e+09,-0.009120016);
   gre->SetPointError(2,0,0.003299401);
   gre->SetPoint(3,1.4616e+09,-0.01032341);
   gre->SetPointError(3,0,0.002930368);
   gre->SetPoint(4,1.462201e+09,-0.01055668);
   gre->SetPointError(4,0,0.002741245);
   gre->SetPoint(5,1.462809e+09,-0.009101395);
   gre->SetPointError(5,0,0.002596375);
   gre->SetPoint(6,1.463416e+09,-0.008771297);
   gre->SetPointError(6,0,0.002811691);
   
   TH1F *Graph_Graph8 = new TH1F("Graph_Graph8","s1_z_slope",100,1.459486e+09,1.463716e+09);
   Graph_Graph8->SetMinimum(-0.03);
   Graph_Graph8->SetMaximum(0.01);
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
   gre->SetHistogram(Graph_Graph8);
   
   gre->Draw("p ");
   
   TF1 *f_dv_1.054 = new TF1("f_dv_1.054","pol0",1.459486e+09,1.463716e+09);
   f_dv_1.054->SetFillColor(19);
   f_dv_1.054->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   f_dv_1.054->SetLineColor(ci);
   f_dv_1.054->SetLineWidth(2);
   f_dv_1.054->SetChisquare(2.871304);
   f_dv_1.054->SetNDF(6);
   f_dv_1.054->GetXaxis()->SetLabelFont(42);
   f_dv_1.054->GetXaxis()->SetLabelSize(0.035);
   f_dv_1.054->GetXaxis()->SetTitleSize(0.035);
   f_dv_1.054->GetXaxis()->SetTitleFont(42);
   f_dv_1.054->GetYaxis()->SetLabelFont(42);
   f_dv_1.054->GetYaxis()->SetLabelSize(0.035);
   f_dv_1.054->GetYaxis()->SetTitleSize(0.035);
   f_dv_1.054->GetYaxis()->SetTitleFont(42);
   f_dv_1.054->SetParameter(0,-0.008939892);
   f_dv_1.054->SetParError(0,0.0010705);
   f_dv_1.054->SetParLimits(0,0,0);
   f_dv_1.054->Draw("same");
   
   pt = new TPaveText(0.15,0.7,0.5,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.054kV: -8.94e-03 +/- 1.07e-03 mm/s");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph3");
   gre->SetTitle("s1_z_slope");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff9933");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff9933");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,-0.008428157);
   gre->SetPointError(0,0,0.002927346);
   gre->SetPoint(1,1.46037e+09,-0.008716816);
   gre->SetPointError(1,0,0.002982795);
   gre->SetPoint(2,1.46099e+09,-0.00975068);
   gre->SetPointError(2,0,0.003198306);
   gre->SetPoint(3,1.4616e+09,-0.009375177);
   gre->SetPointError(3,0,0.002857223);
   gre->SetPoint(4,1.462201e+09,-0.009578982);
   gre->SetPointError(4,0,0.002761796);
   gre->SetPoint(5,1.462809e+09,-0.008337481);
   gre->SetPointError(5,0,0.002539713);
   gre->SetPoint(6,1.463416e+09,-0.009100709);
   gre->SetPointError(6,0,0.002975348);
   
   TH1F *Graph_Graph9 = new TH1F("Graph_Graph9","s1_z_slope",100,1.459486e+09,1.463716e+09);
   Graph_Graph9->SetMinimum(-0.03);
   Graph_Graph9->SetMaximum(0.01);
   Graph_Graph9->SetDirectory(0);
   Graph_Graph9->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph9->SetLineColor(ci);
   Graph_Graph9->GetXaxis()->SetLabelFont(42);
   Graph_Graph9->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph9->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph9->GetXaxis()->SetTitleFont(42);
   Graph_Graph9->GetYaxis()->SetLabelFont(42);
   Graph_Graph9->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph9->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph9->GetYaxis()->SetTitleFont(42);
   Graph_Graph9->GetZaxis()->SetLabelFont(42);
   Graph_Graph9->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph9->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph9->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph9);
   
   gre->Draw("p ");
   
   TF1 *f_dv_1.5 = new TF1("f_dv_1.5","pol0",1.459486e+09,1.463716e+09);
   f_dv_1.5->SetFillColor(19);
   f_dv_1.5->SetFillStyle(0);

   ci = TColor::GetColor("#ff9933");
   f_dv_1.5->SetLineColor(ci);
   f_dv_1.5->SetLineWidth(2);
   f_dv_1.5->SetChisquare(0.2326152);
   f_dv_1.5->SetNDF(6);
   f_dv_1.5->GetXaxis()->SetLabelFont(42);
   f_dv_1.5->GetXaxis()->SetLabelSize(0.035);
   f_dv_1.5->GetXaxis()->SetTitleSize(0.035);
   f_dv_1.5->GetXaxis()->SetTitleFont(42);
   f_dv_1.5->GetYaxis()->SetLabelFont(42);
   f_dv_1.5->GetYaxis()->SetLabelSize(0.035);
   f_dv_1.5->GetYaxis()->SetTitleSize(0.035);
   f_dv_1.5->GetYaxis()->SetTitleFont(42);
   f_dv_1.5->SetParameter(0,-0.009006308);
   f_dv_1.5->SetParError(0,0.001085647);
   f_dv_1.5->SetParLimits(0,0,0);
   f_dv_1.5->Draw("same");
   
   pt = new TPaveText(0.15,0.65,0.5,0.7,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff9933");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.5kV: -9.01e-03 +/- 1.09e-03 mm/s");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph4");
   gre->SetTitle("s1_z_slope");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff0000");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff0000");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,-0.008821753);
   gre->SetPointError(0,0,0.002822346);
   gre->SetPoint(1,1.46037e+09,-0.01104029);
   gre->SetPointError(1,0,0.002932093);
   gre->SetPoint(2,1.46099e+09,-0.008756007);
   gre->SetPointError(2,0,0.003092529);
   gre->SetPoint(3,1.4616e+09,-0.008524885);
   gre->SetPointError(3,0,0.002996721);
   gre->SetPoint(4,1.462201e+09,-0.008086122);
   gre->SetPointError(4,0,0.002580505);
   gre->SetPoint(5,1.462809e+09,-0.004586414);
   gre->SetPointError(5,0,0.001743586);
   gre->SetPoint(6,1.463416e+09,-0.004523261);
   gre->SetPointError(6,0,0.001920113);
   
   TH1F *Graph_Graph10 = new TH1F("Graph_Graph10","s1_z_slope",100,1.459486e+09,1.463716e+09);
   Graph_Graph10->SetMinimum(-0.03);
   Graph_Graph10->SetMaximum(0.01);
   Graph_Graph10->SetDirectory(0);
   Graph_Graph10->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph10->SetLineColor(ci);
   Graph_Graph10->GetXaxis()->SetLabelFont(42);
   Graph_Graph10->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph10->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph10->GetXaxis()->SetTitleFont(42);
   Graph_Graph10->GetYaxis()->SetLabelFont(42);
   Graph_Graph10->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph10->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph10->GetYaxis()->SetTitleFont(42);
   Graph_Graph10->GetZaxis()->SetLabelFont(42);
   Graph_Graph10->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph10->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph10->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph10);
   
   gre->Draw("p ");
   
   TF1 *f_dv_2.356 = new TF1("f_dv_2.356","pol0",1.459486e+09,1.463716e+09);
   f_dv_2.356->SetFillColor(19);
   f_dv_2.356->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   f_dv_2.356->SetLineColor(ci);
   f_dv_2.356->SetLineWidth(2);
   f_dv_2.356->SetChisquare(6.601962);
   f_dv_2.356->SetNDF(6);
   f_dv_2.356->GetXaxis()->SetLabelFont(42);
   f_dv_2.356->GetXaxis()->SetLabelSize(0.035);
   f_dv_2.356->GetXaxis()->SetTitleSize(0.035);
   f_dv_2.356->GetXaxis()->SetTitleFont(42);
   f_dv_2.356->GetYaxis()->SetLabelFont(42);
   f_dv_2.356->GetYaxis()->SetLabelSize(0.035);
   f_dv_2.356->GetYaxis()->SetTitleSize(0.035);
   f_dv_2.356->GetYaxis()->SetTitleFont(42);
   f_dv_2.356->SetParameter(0,-0.006792635);
   f_dv_2.356->SetParError(0,0.0009097987);
   f_dv_2.356->SetParLimits(0,0,0);
   f_dv_2.356->Draw("same");
   
   pt = new TPaveText(0.15,0.6,0.5,0.65,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 2.356kV: -6.79e-03 +/- 9.10e-04 mm/s");
   pt->Draw();
   515419f19f2e4f069146204745b98d47_2->Modified();
   515419f19f2e4f069146204745b98d47->cd();
  
// ------------>Primitives in pad: 515419f19f2e4f069146204745b98d47_3
   515419f19f2e4f069146204745b98d47_3 = new TPad("515419f19f2e4f069146204745b98d47_3", "515419f19f2e4f069146204745b98d47_3",0.01,0.01,0.49,0.49);
   515419f19f2e4f069146204745b98d47_3->Draw();
   515419f19f2e4f069146204745b98d47_3->cd();
   515419f19f2e4f069146204745b98d47_3->Range(1.458957e+09,0.65,1.464245e+09,1.15);
   515419f19f2e4f069146204745b98d47_3->SetFillColor(0);
   515419f19f2e4f069146204745b98d47_3->SetBorderMode(0);
   515419f19f2e4f069146204745b98d47_3->SetBorderSize(2);
   515419f19f2e4f069146204745b98d47_3->SetFrameBorderMode(0);
   515419f19f2e4f069146204745b98d47_3->SetFrameBorderMode(0);
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph0");
   gre->SetTitle("s1_r_intercept");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff00ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff00ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,0.8426013);
   gre->SetPointError(0,0,0.04308127);
   gre->SetPoint(1,1.46037e+09,0.8400895);
   gre->SetPointError(1,0,0.04368344);
   gre->SetPoint(2,1.46099e+09,0.8423295);
   gre->SetPointError(2,0,0.03680111);
   gre->SetPoint(3,1.4616e+09,0.8392132);
   gre->SetPointError(3,0,0.04430622);
   gre->SetPoint(4,1.462201e+09,0.8022139);
   gre->SetPointError(4,0,0.03732823);
   gre->SetPoint(5,1.462809e+09,0.8562663);
   gre->SetPointError(5,0,0.04119692);
   gre->SetPoint(6,1.463416e+09,0.8529864);
   gre->SetPointError(6,0,0.03601138);
   
   TH1F *Graph_Graph11 = new TH1F("Graph_Graph11","s1_r_intercept",100,1.459486e+09,1.463716e+09);
   Graph_Graph11->SetMinimum(0.7);
   Graph_Graph11->SetMaximum(1.1);
   Graph_Graph11->SetDirectory(0);
   Graph_Graph11->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph11->SetLineColor(ci);
   Graph_Graph11->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph11->GetXaxis()->SetTimeFormat("%m-%d%F1995-01-01 00:00:00s0");
   Graph_Graph11->GetXaxis()->SetLabelFont(42);
   Graph_Graph11->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph11->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph11->GetXaxis()->SetTitleFont(42);
   Graph_Graph11->GetYaxis()->SetLabelFont(42);
   Graph_Graph11->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph11->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph11->GetYaxis()->SetTitleFont(42);
   Graph_Graph11->GetZaxis()->SetLabelFont(42);
   Graph_Graph11->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph11->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph11->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph11);
   
   gre->Draw("ap");
   
   TF1 *f_dv_0.345 = new TF1("f_dv_0.345","pol0",1.459486e+09,1.463716e+09);
   f_dv_0.345->SetFillColor(19);
   f_dv_0.345->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   f_dv_0.345->SetLineColor(ci);
   f_dv_0.345->SetLineWidth(2);
   f_dv_0.345->SetChisquare(1.313458);
   f_dv_0.345->SetNDF(6);
   f_dv_0.345->GetXaxis()->SetLabelFont(42);
   f_dv_0.345->GetXaxis()->SetLabelSize(0.035);
   f_dv_0.345->GetXaxis()->SetTitleSize(0.035);
   f_dv_0.345->GetXaxis()->SetTitleFont(42);
   f_dv_0.345->GetYaxis()->SetLabelFont(42);
   f_dv_0.345->GetYaxis()->SetLabelSize(0.035);
   f_dv_0.345->GetYaxis()->SetTitleSize(0.035);
   f_dv_0.345->GetYaxis()->SetTitleFont(42);
   f_dv_0.345->SetParameter(0,0.8389172);
   f_dv_0.345->SetParError(0,0.01509509);
   f_dv_0.345->SetParLimits(0,0,0);
   f_dv_0.345->Draw("same");
   
   pt = new TPaveText(0.15,0.8,0.5,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 0.345kV: 8.39e-01 +/- 1.51e-02 mm/s");
   pt->Draw();
   
   pt = new TPaveText(0.3832292,0.9379564,0.6167708,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("s1_r_intercept");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph1");
   gre->SetTitle("s1_r_intercept");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,0.8761739);
   gre->SetPointError(0,0,0.0373272);
   gre->SetPoint(1,1.46037e+09,0.8308474);
   gre->SetPointError(1,0,0.03882618);
   gre->SetPoint(2,1.46099e+09,0.8887168);
   gre->SetPointError(2,0,0.04387525);
   gre->SetPoint(3,1.4616e+09,0.8522199);
   gre->SetPointError(3,0,0.04036729);
   gre->SetPoint(4,1.462201e+09,0.8330345);
   gre->SetPointError(4,0,0.03693614);
   gre->SetPoint(5,1.462809e+09,0.8767182);
   gre->SetPointError(5,0,0.04584022);
   gre->SetPoint(6,1.463416e+09,0.8640263);
   gre->SetPointError(6,0,0.04492762);
   
   TH1F *Graph_Graph12 = new TH1F("Graph_Graph12","s1_r_intercept",100,1.459486e+09,1.463716e+09);
   Graph_Graph12->SetMinimum(0.7);
   Graph_Graph12->SetMaximum(1.1);
   Graph_Graph12->SetDirectory(0);
   Graph_Graph12->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph12->SetLineColor(ci);
   Graph_Graph12->GetXaxis()->SetLabelFont(42);
   Graph_Graph12->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph12->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph12->GetXaxis()->SetTitleFont(42);
   Graph_Graph12->GetYaxis()->SetLabelFont(42);
   Graph_Graph12->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph12->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph12->GetYaxis()->SetTitleFont(42);
   Graph_Graph12->GetZaxis()->SetLabelFont(42);
   Graph_Graph12->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph12->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph12->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph12);
   
   gre->Draw("p ");
   
   TF1 *f_dv_0.7 = new TF1("f_dv_0.7","pol0",1.459486e+09,1.463716e+09);
   f_dv_0.7->SetFillColor(19);
   f_dv_0.7->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_dv_0.7->SetLineColor(ci);
   f_dv_0.7->SetLineWidth(2);
   f_dv_0.7->SetChisquare(1.878089);
   f_dv_0.7->SetNDF(6);
   f_dv_0.7->GetXaxis()->SetLabelFont(42);
   f_dv_0.7->GetXaxis()->SetLabelSize(0.035);
   f_dv_0.7->GetXaxis()->SetTitleSize(0.035);
   f_dv_0.7->GetXaxis()->SetTitleFont(42);
   f_dv_0.7->GetYaxis()->SetLabelFont(42);
   f_dv_0.7->GetYaxis()->SetLabelSize(0.035);
   f_dv_0.7->GetYaxis()->SetTitleSize(0.035);
   f_dv_0.7->GetYaxis()->SetTitleFont(42);
   f_dv_0.7->SetParameter(0,0.8582453);
   f_dv_0.7->SetParError(0,0.01539643);
   f_dv_0.7->SetParLimits(0,0,0);
   f_dv_0.7->Draw("same");
   
   pt = new TPaveText(0.15,0.75,0.5,0.8,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 0.7kV: 8.58e-01 +/- 1.54e-02 mm/s");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph2");
   gre->SetTitle("s1_r_intercept");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#00cc00");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#00cc00");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,0.9260755);
   gre->SetPointError(0,0,0.04310605);
   gre->SetPoint(1,1.46037e+09,0.8694217);
   gre->SetPointError(1,0,0.04217586);
   gre->SetPoint(2,1.46099e+09,0.8848405);
   gre->SetPointError(2,0,0.043077);
   gre->SetPoint(3,1.4616e+09,0.8580409);
   gre->SetPointError(3,0,0.03829671);
   gre->SetPoint(4,1.462201e+09,0.8658334);
   gre->SetPointError(4,0,0.03908604);
   gre->SetPoint(5,1.462809e+09,0.8774521);
   gre->SetPointError(5,0,0.03651311);
   gre->SetPoint(6,1.463416e+09,0.8939203);
   gre->SetPointError(6,0,0.0371927);
   
   TH1F *Graph_Graph13 = new TH1F("Graph_Graph13","s1_r_intercept",100,1.459486e+09,1.463716e+09);
   Graph_Graph13->SetMinimum(0.7);
   Graph_Graph13->SetMaximum(1.1);
   Graph_Graph13->SetDirectory(0);
   Graph_Graph13->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph13->SetLineColor(ci);
   Graph_Graph13->GetXaxis()->SetLabelFont(42);
   Graph_Graph13->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph13->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph13->GetXaxis()->SetTitleFont(42);
   Graph_Graph13->GetYaxis()->SetLabelFont(42);
   Graph_Graph13->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph13->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph13->GetYaxis()->SetTitleFont(42);
   Graph_Graph13->GetZaxis()->SetLabelFont(42);
   Graph_Graph13->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph13->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph13->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph13);
   
   gre->Draw("p ");
   
   TF1 *f_dv_1.054 = new TF1("f_dv_1.054","pol0",1.459486e+09,1.463716e+09);
   f_dv_1.054->SetFillColor(19);
   f_dv_1.054->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   f_dv_1.054->SetLineColor(ci);
   f_dv_1.054->SetLineWidth(2);
   f_dv_1.054->SetChisquare(1.816682);
   f_dv_1.054->SetNDF(6);
   f_dv_1.054->GetXaxis()->SetLabelFont(42);
   f_dv_1.054->GetXaxis()->SetLabelSize(0.035);
   f_dv_1.054->GetXaxis()->SetTitleSize(0.035);
   f_dv_1.054->GetXaxis()->SetTitleFont(42);
   f_dv_1.054->GetYaxis()->SetLabelFont(42);
   f_dv_1.054->GetYaxis()->SetLabelSize(0.035);
   f_dv_1.054->GetYaxis()->SetTitleSize(0.035);
   f_dv_1.054->GetYaxis()->SetTitleFont(42);
   f_dv_1.054->SetParameter(0,0.881206);
   f_dv_1.054->SetParError(0,0.01499293);
   f_dv_1.054->SetParLimits(0,0,0);
   f_dv_1.054->Draw("same");
   
   pt = new TPaveText(0.15,0.7,0.5,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.054kV: 8.81e-01 +/- 1.50e-02 mm/s");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph3");
   gre->SetTitle("s1_r_intercept");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff9933");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff9933");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,0.8947566);
   gre->SetPointError(0,0,0.03910993);
   gre->SetPoint(1,1.46037e+09,0.8919258);
   gre->SetPointError(1,0,0.04225525);
   gre->SetPoint(2,1.46099e+09,0.8727288);
   gre->SetPointError(2,0,0.04465149);
   gre->SetPoint(3,1.4616e+09,0.8886229);
   gre->SetPointError(3,0,0.04055998);
   gre->SetPoint(4,1.462201e+09,0.8764414);
   gre->SetPointError(4,0,0.04008758);
   gre->SetPoint(5,1.462809e+09,0.8944186);
   gre->SetPointError(5,0,0.03572729);
   gre->SetPoint(6,1.463416e+09,0.8844715);
   gre->SetPointError(6,0,0.04441986);
   
   TH1F *Graph_Graph14 = new TH1F("Graph_Graph14","s1_r_intercept",100,1.459486e+09,1.463716e+09);
   Graph_Graph14->SetMinimum(0.7);
   Graph_Graph14->SetMaximum(1.1);
   Graph_Graph14->SetDirectory(0);
   Graph_Graph14->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph14->SetLineColor(ci);
   Graph_Graph14->GetXaxis()->SetLabelFont(42);
   Graph_Graph14->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph14->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph14->GetXaxis()->SetTitleFont(42);
   Graph_Graph14->GetYaxis()->SetLabelFont(42);
   Graph_Graph14->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph14->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph14->GetYaxis()->SetTitleFont(42);
   Graph_Graph14->GetZaxis()->SetLabelFont(42);
   Graph_Graph14->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph14->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph14->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph14);
   
   gre->Draw("p ");
   
   TF1 *f_dv_1.5 = new TF1("f_dv_1.5","pol0",1.459486e+09,1.463716e+09);
   f_dv_1.5->SetFillColor(19);
   f_dv_1.5->SetFillStyle(0);

   ci = TColor::GetColor("#ff9933");
   f_dv_1.5->SetLineColor(ci);
   f_dv_1.5->SetLineWidth(2);
   f_dv_1.5->SetChisquare(0.2723732);
   f_dv_1.5->SetNDF(6);
   f_dv_1.5->GetXaxis()->SetLabelFont(42);
   f_dv_1.5->GetXaxis()->SetLabelSize(0.035);
   f_dv_1.5->GetXaxis()->SetTitleSize(0.035);
   f_dv_1.5->GetXaxis()->SetTitleFont(42);
   f_dv_1.5->GetYaxis()->SetLabelFont(42);
   f_dv_1.5->GetYaxis()->SetLabelSize(0.035);
   f_dv_1.5->GetYaxis()->SetTitleSize(0.035);
   f_dv_1.5->GetYaxis()->SetTitleFont(42);
   f_dv_1.5->SetParameter(0,0.8869099);
   f_dv_1.5->SetParError(0,0.01536503);
   f_dv_1.5->SetParLimits(0,0,0);
   f_dv_1.5->Draw("same");
   
   pt = new TPaveText(0.15,0.65,0.5,0.7,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff9933");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.5kV: 8.87e-01 +/- 1.54e-02 mm/s");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph4");
   gre->SetTitle("s1_r_intercept");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff0000");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff0000");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,0.8886511);
   gre->SetPointError(0,0,0.0395873);
   gre->SetPoint(1,1.46037e+09,0.8762229);
   gre->SetPointError(1,0,0.04150852);
   gre->SetPoint(2,1.46099e+09,0.8871528);
   gre->SetPointError(2,0,0.0402097);
   gre->SetPoint(3,1.4616e+09,0.8910852);
   gre->SetPointError(3,0,0.04466951);
   gre->SetPoint(4,1.462201e+09,0.8983722);
   gre->SetPointError(4,0,0.03922656);
   gre->SetPoint(5,1.462809e+09,0.9181055);
   gre->SetPointError(5,0,0.03579448);
   gre->SetPoint(6,1.463416e+09,0.9338072);
   gre->SetPointError(6,0,0.03683922);
   
   TH1F *Graph_Graph15 = new TH1F("Graph_Graph15","s1_r_intercept",100,1.459486e+09,1.463716e+09);
   Graph_Graph15->SetMinimum(0.7);
   Graph_Graph15->SetMaximum(1.1);
   Graph_Graph15->SetDirectory(0);
   Graph_Graph15->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph15->SetLineColor(ci);
   Graph_Graph15->GetXaxis()->SetLabelFont(42);
   Graph_Graph15->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph15->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph15->GetXaxis()->SetTitleFont(42);
   Graph_Graph15->GetYaxis()->SetLabelFont(42);
   Graph_Graph15->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph15->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph15->GetYaxis()->SetTitleFont(42);
   Graph_Graph15->GetZaxis()->SetLabelFont(42);
   Graph_Graph15->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph15->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph15->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph15);
   
   gre->Draw("p ");
   
   TF1 *f_dv_2.356 = new TF1("f_dv_2.356","pol0",1.459486e+09,1.463716e+09);
   f_dv_2.356->SetFillColor(19);
   f_dv_2.356->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   f_dv_2.356->SetLineColor(ci);
   f_dv_2.356->SetLineWidth(2);
   f_dv_2.356->SetChisquare(1.647415);
   f_dv_2.356->SetNDF(6);
   f_dv_2.356->GetXaxis()->SetLabelFont(42);
   f_dv_2.356->GetXaxis()->SetLabelSize(0.035);
   f_dv_2.356->GetXaxis()->SetTitleSize(0.035);
   f_dv_2.356->GetXaxis()->SetTitleFont(42);
   f_dv_2.356->GetYaxis()->SetLabelFont(42);
   f_dv_2.356->GetYaxis()->SetLabelSize(0.035);
   f_dv_2.356->GetYaxis()->SetTitleSize(0.035);
   f_dv_2.356->GetYaxis()->SetTitleFont(42);
   f_dv_2.356->SetParameter(0,0.9010051);
   f_dv_2.356->SetParError(0,0.01489821);
   f_dv_2.356->SetParLimits(0,0,0);
   f_dv_2.356->Draw("same");
   
   pt = new TPaveText(0.15,0.6,0.5,0.65,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 2.356kV: 9.01e-01 +/- 1.49e-02 mm/s");
   pt->Draw();
   515419f19f2e4f069146204745b98d47_3->Modified();
   515419f19f2e4f069146204745b98d47->cd();
  
// ------------>Primitives in pad: 515419f19f2e4f069146204745b98d47_4
   515419f19f2e4f069146204745b98d47_4 = new TPad("515419f19f2e4f069146204745b98d47_4", "515419f19f2e4f069146204745b98d47_4",0.51,0.01,0.99,0.49);
   515419f19f2e4f069146204745b98d47_4->Draw();
   515419f19f2e4f069146204745b98d47_4->cd();
   515419f19f2e4f069146204745b98d47_4->Range(1.458957e+09,-0.035,1.464245e+09,0.015);
   515419f19f2e4f069146204745b98d47_4->SetFillColor(0);
   515419f19f2e4f069146204745b98d47_4->SetBorderMode(0);
   515419f19f2e4f069146204745b98d47_4->SetBorderSize(2);
   515419f19f2e4f069146204745b98d47_4->SetFrameBorderMode(0);
   515419f19f2e4f069146204745b98d47_4->SetFrameBorderMode(0);
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph0");
   gre->SetTitle("s1_r_slope");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff00ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff00ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,-0.01232603);
   gre->SetPointError(0,0,0.003311364);
   gre->SetPoint(1,1.46037e+09,-0.01136165);
   gre->SetPointError(1,0,0.003095846);
   gre->SetPoint(2,1.46099e+09,-0.01237626);
   gre->SetPointError(2,0,0.002843912);
   gre->SetPoint(3,1.4616e+09,-0.01307055);
   gre->SetPointError(3,0,0.002959206);
   gre->SetPoint(4,1.462201e+09,-0.01588953);
   gre->SetPointError(4,0,0.002913762);
   gre->SetPoint(5,1.462809e+09,-0.01086521);
   gre->SetPointError(5,0,0.003051438);
   gre->SetPoint(6,1.463416e+09,-0.01158775);
   gre->SetPointError(6,0,0.002622065);
   
   TH1F *Graph_Graph16 = new TH1F("Graph_Graph16","s1_r_slope",100,1.459486e+09,1.463716e+09);
   Graph_Graph16->SetMinimum(-0.03);
   Graph_Graph16->SetMaximum(0.01);
   Graph_Graph16->SetDirectory(0);
   Graph_Graph16->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph16->SetLineColor(ci);
   Graph_Graph16->GetXaxis()->SetTimeDisplay(1);
   Graph_Graph16->GetXaxis()->SetTimeFormat("%m-%d%F1995-01-01 00:00:00s0");
   Graph_Graph16->GetXaxis()->SetLabelFont(42);
   Graph_Graph16->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph16->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph16->GetXaxis()->SetTitleFont(42);
   Graph_Graph16->GetYaxis()->SetLabelFont(42);
   Graph_Graph16->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph16->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph16->GetYaxis()->SetTitleFont(42);
   Graph_Graph16->GetZaxis()->SetLabelFont(42);
   Graph_Graph16->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph16->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph16->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph16);
   
   gre->Draw("ap");
   
   TF1 *f_dv_0.345 = new TF1("f_dv_0.345","pol0",1.459486e+09,1.463716e+09);
   f_dv_0.345->SetFillColor(19);
   f_dv_0.345->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   f_dv_0.345->SetLineColor(ci);
   f_dv_0.345->SetLineWidth(2);
   f_dv_0.345->SetChisquare(1.938281);
   f_dv_0.345->SetNDF(6);
   f_dv_0.345->GetXaxis()->SetLabelFont(42);
   f_dv_0.345->GetXaxis()->SetLabelSize(0.035);
   f_dv_0.345->GetXaxis()->SetTitleSize(0.035);
   f_dv_0.345->GetXaxis()->SetTitleFont(42);
   f_dv_0.345->GetYaxis()->SetLabelFont(42);
   f_dv_0.345->GetYaxis()->SetLabelSize(0.035);
   f_dv_0.345->GetYaxis()->SetTitleSize(0.035);
   f_dv_0.345->GetYaxis()->SetTitleFont(42);
   f_dv_0.345->SetParameter(0,-0.01250771);
   f_dv_0.345->SetParError(0,0.001115245);
   f_dv_0.345->SetParLimits(0,0,0);
   f_dv_0.345->Draw("same");
   
   pt = new TPaveText(0.15,0.8,0.5,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 0.345kV: -1.25e-02 +/- 1.12e-03 mm/s");
   pt->Draw();
   
   pt = new TPaveText(0.4086198,0.9379564,0.5913802,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("s1_r_slope");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph1");
   gre->SetTitle("s1_r_slope");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,-0.009252117);
   gre->SetPointError(0,0,0.002446111);
   gre->SetPoint(1,1.46037e+09,-0.01259002);
   gre->SetPointError(1,0,0.00279609);
   gre->SetPoint(2,1.46099e+09,-0.008453963);
   gre->SetPointError(2,0,0.002995586);
   gre->SetPoint(3,1.4616e+09,-0.01111554);
   gre->SetPointError(3,0,0.002937279);
   gre->SetPoint(4,1.462201e+09,-0.01288467);
   gre->SetPointError(4,0,0.00268411);
   gre->SetPoint(5,1.462809e+09,-0.009893152);
   gre->SetPointError(5,0,0.003522783);
   gre->SetPoint(6,1.463416e+09,-0.01037161);
   gre->SetPointError(6,0,0.003380633);
   
   TH1F *Graph_Graph17 = new TH1F("Graph_Graph17","s1_r_slope",100,1.459486e+09,1.463716e+09);
   Graph_Graph17->SetMinimum(-0.03);
   Graph_Graph17->SetMaximum(0.01);
   Graph_Graph17->SetDirectory(0);
   Graph_Graph17->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph17->SetLineColor(ci);
   Graph_Graph17->GetXaxis()->SetLabelFont(42);
   Graph_Graph17->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph17->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph17->GetXaxis()->SetTitleFont(42);
   Graph_Graph17->GetYaxis()->SetLabelFont(42);
   Graph_Graph17->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph17->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph17->GetYaxis()->SetTitleFont(42);
   Graph_Graph17->GetZaxis()->SetLabelFont(42);
   Graph_Graph17->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph17->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph17->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph17);
   
   gre->Draw("p ");
   
   TF1 *f_dv_0.7 = new TF1("f_dv_0.7","pol0",1.459486e+09,1.463716e+09);
   f_dv_0.7->SetFillColor(19);
   f_dv_0.7->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_dv_0.7->SetLineColor(ci);
   f_dv_0.7->SetLineWidth(2);
   f_dv_0.7->SetChisquare(2.113767);
   f_dv_0.7->SetNDF(6);
   f_dv_0.7->GetXaxis()->SetLabelFont(42);
   f_dv_0.7->GetXaxis()->SetLabelSize(0.035);
   f_dv_0.7->GetXaxis()->SetTitleSize(0.035);
   f_dv_0.7->GetXaxis()->SetTitleFont(42);
   f_dv_0.7->GetYaxis()->SetLabelFont(42);
   f_dv_0.7->GetYaxis()->SetLabelSize(0.035);
   f_dv_0.7->GetYaxis()->SetTitleSize(0.035);
   f_dv_0.7->GetYaxis()->SetTitleFont(42);
   f_dv_0.7->SetParameter(0,-0.01070859);
   f_dv_0.7->SetParError(0,0.00109813);
   f_dv_0.7->SetParLimits(0,0,0);
   f_dv_0.7->Draw("same");
   
   pt = new TPaveText(0.15,0.75,0.5,0.8,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 0.7kV: -1.07e-02 +/- 1.10e-03 mm/s");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph2");
   gre->SetTitle("s1_r_slope");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#00cc00");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#00cc00");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,-0.005239605);
   gre->SetPointError(0,0,0.002582981);
   gre->SetPoint(1,1.46037e+09,-0.01043422);
   gre->SetPointError(1,0,0.003064094);
   gre->SetPoint(2,1.46099e+09,-0.009120016);
   gre->SetPointError(2,0,0.003299401);
   gre->SetPoint(3,1.4616e+09,-0.01032341);
   gre->SetPointError(3,0,0.002930368);
   gre->SetPoint(4,1.462201e+09,-0.01055668);
   gre->SetPointError(4,0,0.002741245);
   gre->SetPoint(5,1.462809e+09,-0.009101395);
   gre->SetPointError(5,0,0.002596375);
   gre->SetPoint(6,1.463416e+09,-0.008771297);
   gre->SetPointError(6,0,0.002811691);
   
   TH1F *Graph_Graph18 = new TH1F("Graph_Graph18","s1_r_slope",100,1.459486e+09,1.463716e+09);
   Graph_Graph18->SetMinimum(-0.03);
   Graph_Graph18->SetMaximum(0.01);
   Graph_Graph18->SetDirectory(0);
   Graph_Graph18->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph18->SetLineColor(ci);
   Graph_Graph18->GetXaxis()->SetLabelFont(42);
   Graph_Graph18->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph18->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph18->GetXaxis()->SetTitleFont(42);
   Graph_Graph18->GetYaxis()->SetLabelFont(42);
   Graph_Graph18->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph18->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph18->GetYaxis()->SetTitleFont(42);
   Graph_Graph18->GetZaxis()->SetLabelFont(42);
   Graph_Graph18->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph18->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph18->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph18);
   
   gre->Draw("p ");
   
   TF1 *f_dv_1.054 = new TF1("f_dv_1.054","pol0",1.459486e+09,1.463716e+09);
   f_dv_1.054->SetFillColor(19);
   f_dv_1.054->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   f_dv_1.054->SetLineColor(ci);
   f_dv_1.054->SetLineWidth(2);
   f_dv_1.054->SetChisquare(2.871304);
   f_dv_1.054->SetNDF(6);
   f_dv_1.054->GetXaxis()->SetLabelFont(42);
   f_dv_1.054->GetXaxis()->SetLabelSize(0.035);
   f_dv_1.054->GetXaxis()->SetTitleSize(0.035);
   f_dv_1.054->GetXaxis()->SetTitleFont(42);
   f_dv_1.054->GetYaxis()->SetLabelFont(42);
   f_dv_1.054->GetYaxis()->SetLabelSize(0.035);
   f_dv_1.054->GetYaxis()->SetTitleSize(0.035);
   f_dv_1.054->GetYaxis()->SetTitleFont(42);
   f_dv_1.054->SetParameter(0,-0.008939892);
   f_dv_1.054->SetParError(0,0.0010705);
   f_dv_1.054->SetParLimits(0,0,0);
   f_dv_1.054->Draw("same");
   
   pt = new TPaveText(0.15,0.7,0.5,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.054kV: -8.94e-03 +/- 1.07e-03 mm/s");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph3");
   gre->SetTitle("s1_r_slope");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff9933");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff9933");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,-0.008428157);
   gre->SetPointError(0,0,0.002927346);
   gre->SetPoint(1,1.46037e+09,-0.008716816);
   gre->SetPointError(1,0,0.002982795);
   gre->SetPoint(2,1.46099e+09,-0.00975068);
   gre->SetPointError(2,0,0.003198306);
   gre->SetPoint(3,1.4616e+09,-0.009375177);
   gre->SetPointError(3,0,0.002857223);
   gre->SetPoint(4,1.462201e+09,-0.009578982);
   gre->SetPointError(4,0,0.002761796);
   gre->SetPoint(5,1.462809e+09,-0.008337481);
   gre->SetPointError(5,0,0.002539713);
   gre->SetPoint(6,1.463416e+09,-0.009100709);
   gre->SetPointError(6,0,0.002975348);
   
   TH1F *Graph_Graph19 = new TH1F("Graph_Graph19","s1_r_slope",100,1.459486e+09,1.463716e+09);
   Graph_Graph19->SetMinimum(-0.03);
   Graph_Graph19->SetMaximum(0.01);
   Graph_Graph19->SetDirectory(0);
   Graph_Graph19->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph19->SetLineColor(ci);
   Graph_Graph19->GetXaxis()->SetLabelFont(42);
   Graph_Graph19->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph19->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph19->GetXaxis()->SetTitleFont(42);
   Graph_Graph19->GetYaxis()->SetLabelFont(42);
   Graph_Graph19->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph19->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph19->GetYaxis()->SetTitleFont(42);
   Graph_Graph19->GetZaxis()->SetLabelFont(42);
   Graph_Graph19->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph19->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph19->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph19);
   
   gre->Draw("p ");
   
   TF1 *f_dv_1.5 = new TF1("f_dv_1.5","pol0",1.459486e+09,1.463716e+09);
   f_dv_1.5->SetFillColor(19);
   f_dv_1.5->SetFillStyle(0);

   ci = TColor::GetColor("#ff9933");
   f_dv_1.5->SetLineColor(ci);
   f_dv_1.5->SetLineWidth(2);
   f_dv_1.5->SetChisquare(0.2326152);
   f_dv_1.5->SetNDF(6);
   f_dv_1.5->GetXaxis()->SetLabelFont(42);
   f_dv_1.5->GetXaxis()->SetLabelSize(0.035);
   f_dv_1.5->GetXaxis()->SetTitleSize(0.035);
   f_dv_1.5->GetXaxis()->SetTitleFont(42);
   f_dv_1.5->GetYaxis()->SetLabelFont(42);
   f_dv_1.5->GetYaxis()->SetLabelSize(0.035);
   f_dv_1.5->GetYaxis()->SetTitleSize(0.035);
   f_dv_1.5->GetYaxis()->SetTitleFont(42);
   f_dv_1.5->SetParameter(0,-0.009006308);
   f_dv_1.5->SetParError(0,0.001085647);
   f_dv_1.5->SetParLimits(0,0,0);
   f_dv_1.5->Draw("same");
   
   pt = new TPaveText(0.15,0.65,0.5,0.7,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff9933");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.5kV: -9.01e-03 +/- 1.09e-03 mm/s");
   pt->Draw();
   
   gre = new TGraphErrors(7);
   gre->SetName("Graph4");
   gre->SetTitle("s1_r_slope");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff0000");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff0000");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459786e+09,-0.008821753);
   gre->SetPointError(0,0,0.002822346);
   gre->SetPoint(1,1.46037e+09,-0.01104029);
   gre->SetPointError(1,0,0.002932093);
   gre->SetPoint(2,1.46099e+09,-0.008756007);
   gre->SetPointError(2,0,0.003092529);
   gre->SetPoint(3,1.4616e+09,-0.008524885);
   gre->SetPointError(3,0,0.002996721);
   gre->SetPoint(4,1.462201e+09,-0.008086122);
   gre->SetPointError(4,0,0.002580505);
   gre->SetPoint(5,1.462809e+09,-0.004586414);
   gre->SetPointError(5,0,0.001743586);
   gre->SetPoint(6,1.463416e+09,-0.004523261);
   gre->SetPointError(6,0,0.001920113);
   
   TH1F *Graph_Graph20 = new TH1F("Graph_Graph20","s1_r_slope",100,1.459486e+09,1.463716e+09);
   Graph_Graph20->SetMinimum(-0.03);
   Graph_Graph20->SetMaximum(0.01);
   Graph_Graph20->SetDirectory(0);
   Graph_Graph20->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph20->SetLineColor(ci);
   Graph_Graph20->GetXaxis()->SetLabelFont(42);
   Graph_Graph20->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph20->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph20->GetXaxis()->SetTitleFont(42);
   Graph_Graph20->GetYaxis()->SetLabelFont(42);
   Graph_Graph20->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph20->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph20->GetYaxis()->SetTitleFont(42);
   Graph_Graph20->GetZaxis()->SetLabelFont(42);
   Graph_Graph20->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph20->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph20->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph20);
   
   gre->Draw("p ");
   
   TF1 *f_dv_2.356 = new TF1("f_dv_2.356","pol0",1.459486e+09,1.463716e+09);
   f_dv_2.356->SetFillColor(19);
   f_dv_2.356->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   f_dv_2.356->SetLineColor(ci);
   f_dv_2.356->SetLineWidth(2);
   f_dv_2.356->SetChisquare(6.601962);
   f_dv_2.356->SetNDF(6);
   f_dv_2.356->GetXaxis()->SetLabelFont(42);
   f_dv_2.356->GetXaxis()->SetLabelSize(0.035);
   f_dv_2.356->GetXaxis()->SetTitleSize(0.035);
   f_dv_2.356->GetXaxis()->SetTitleFont(42);
   f_dv_2.356->GetYaxis()->SetLabelFont(42);
   f_dv_2.356->GetYaxis()->SetLabelSize(0.035);
   f_dv_2.356->GetYaxis()->SetTitleSize(0.035);
   f_dv_2.356->GetYaxis()->SetTitleFont(42);
   f_dv_2.356->SetParameter(0,-0.006792635);
   f_dv_2.356->SetParError(0,0.0009097987);
   f_dv_2.356->SetParLimits(0,0,0);
   f_dv_2.356->Draw("same");
   
   pt = new TPaveText(0.15,0.6,0.5,0.65,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 2.356kV: -6.79e-03 +/- 9.10e-04 mm/s");
   pt->Draw();
   515419f19f2e4f069146204745b98d47_4->Modified();
   515419f19f2e4f069146204745b98d47->cd();
   515419f19f2e4f069146204745b98d47->Modified();
   515419f19f2e4f069146204745b98d47->cd();
   515419f19f2e4f069146204745b98d47->SetSelected(515419f19f2e4f069146204745b98d47);
}
