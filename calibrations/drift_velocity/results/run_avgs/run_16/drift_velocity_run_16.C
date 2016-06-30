{
//=========Macro generated from canvas: 33b5c80d377a4df9ac3addbee347fe68/
//=========  (Thu Jun  9 16:30:25 2016) by ROOT version5.34/26
   TCanvas *33b5c80d377a4df9ac3addbee347fe68 = new TCanvas("33b5c80d377a4df9ac3addbee347fe68", "",10,45,1904,727);
   33b5c80d377a4df9ac3addbee347fe68->SetHighLightColor(2);
   33b5c80d377a4df9ac3addbee347fe68->Range(0,0,1,1);
   33b5c80d377a4df9ac3addbee347fe68->SetFillColor(0);
   33b5c80d377a4df9ac3addbee347fe68->SetBorderMode(0);
   33b5c80d377a4df9ac3addbee347fe68->SetBorderSize(2);
   33b5c80d377a4df9ac3addbee347fe68->SetFrameBorderMode(0);
  
// ------------>Primitives in pad: 33b5c80d377a4df9ac3addbee347fe68_1
   TPad *33b5c80d377a4df9ac3addbee347fe68_1 = new TPad("33b5c80d377a4df9ac3addbee347fe68_1", "33b5c80d377a4df9ac3addbee347fe68_1",0.01,0.01,0.49,0.99);
   33b5c80d377a4df9ac3addbee347fe68_1->Draw();
   33b5c80d377a4df9ac3addbee347fe68_1->cd();
   33b5c80d377a4df9ac3addbee347fe68_1->Range(1.458189e+09,1.2625,1.466354e+09,2.6375);
   33b5c80d377a4df9ac3addbee347fe68_1->SetFillColor(0);
   33b5c80d377a4df9ac3addbee347fe68_1->SetBorderMode(0);
   33b5c80d377a4df9ac3addbee347fe68_1->SetBorderSize(2);
   33b5c80d377a4df9ac3addbee347fe68_1->SetFrameBorderMode(0);
   33b5c80d377a4df9ac3addbee347fe68_1->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(18);
   gre->SetName("Graph0");
   gre->SetTitle("Drift Velocity Run Average");
   gre->SetFillColor(1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#ff00ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff00ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459335e+09,1.576376);
   gre->SetPointError(0,0,0.0004740688);
   gre->SetPoint(1,1.459344e+09,1.573425);
   gre->SetPointError(1,0,0.0004298356);
   gre->SetPoint(2,1.459349e+09,1.573405);
   gre->SetPointError(2,0,0.0004151667);
   gre->SetPoint(3,1.459354e+09,1.571794);
   gre->SetPointError(3,0,0.0003521357);
   gre->SetPoint(4,1.459786e+09,1.564733);
   gre->SetPointError(4,0,0.0004486525);
   gre->SetPoint(5,1.460986e+09,1.554735);
   gre->SetPointError(5,0,0.00044953);
   gre->SetPoint(6,1.46099e+09,1.554104);
   gre->SetPointError(6,0,0.0004077394);
   gre->SetPoint(7,1.461261e+09,1.567479);
   gre->SetPointError(7,0,0.0004162596);
   gre->SetPoint(8,1.4616e+09,1.553001);
   gre->SetPointError(8,0,0.0005366422);
   gre->SetPoint(9,1.462196e+09,1.540501);
   gre->SetPointError(9,0,0.0006012155);
   gre->SetPoint(10,1.462201e+09,1.545195);
   gre->SetPointError(10,0,0.0005376539);
   gre->SetPoint(11,1.462455e+09,1.563316);
   gre->SetPointError(11,0,0.0004797261);
   gre->SetPoint(12,1.462805e+09,1.563619);
   gre->SetPointError(12,0,0.000423803);
   gre->SetPoint(13,1.462809e+09,1.562442);
   gre->SetPointError(13,0,0.0002861177);
   gre->SetPoint(14,1.463416e+09,1.558649);
   gre->SetPointError(14,0,0.0003036891);
   gre->SetPoint(15,1.46402e+09,1.548942);
   gre->SetPointError(15,0,0.0004500308);
   gre->SetPoint(16,1.464709e+09,1.548339);
   gre->SetPointError(16,0,0.0008046417);
   gre->SetPoint(17,1.465228e+09,1.549768);
   gre->SetPointError(17,0,0.0008535862);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","Drift Velocity Run Average",100,1.459006e+09,1.465537e+09);
   Graph_Graph1->SetMinimum(1.4);
   Graph_Graph1->SetMaximum(2.5);
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
   Graph_Graph1->GetYaxis()->SetTitle("Drift Velocity [mm/s]");
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
   
   TF1 *f_dv_0.345 = new TF1("f_dv_0.345","pol0",1.459006e+09,1.465537e+09);
   f_dv_0.345->SetFillColor(19);
   f_dv_0.345->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   f_dv_0.345->SetLineColor(ci);
   f_dv_0.345->SetLineWidth(2);
   f_dv_0.345->SetChisquare(8006.915);
   f_dv_0.345->SetNDF(17);
   f_dv_0.345->GetXaxis()->SetLabelFont(42);
   f_dv_0.345->GetXaxis()->SetLabelSize(0.035);
   f_dv_0.345->GetXaxis()->SetTitleSize(0.035);
   f_dv_0.345->GetXaxis()->SetTitleFont(42);
   f_dv_0.345->GetYaxis()->SetLabelFont(42);
   f_dv_0.345->GetYaxis()->SetLabelSize(0.035);
   f_dv_0.345->GetYaxis()->SetTitleSize(0.035);
   f_dv_0.345->GetYaxis()->SetTitleFont(42);
   f_dv_0.345->SetParameter(0,1.561638);
   f_dv_0.345->SetParError(0,0.0001020859);
   f_dv_0.345->SetParLimits(0,0,0);
   f_dv_0.345->Draw("same");
   
   TPaveText *pt = new TPaveText(0.2504167,0.9355248,0.7495833,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Drift Velocity Run Average");
   pt->Draw();
   
   gre = new TGraphErrors(9);
   gre->SetName("Graph1");
   gre->SetTitle("Drift Velocity Run Average");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459788e+09,1.684018);
   gre->SetPointError(0,0,0.0003369208);
   gre->SetPoint(1,1.460991e+09,1.678079);
   gre->SetPointError(1,0,0.0003081591);
   gre->SetPoint(2,1.461602e+09,1.676963);
   gre->SetPointError(2,0,0.0004014101);
   gre->SetPoint(3,1.462203e+09,1.667921);
   gre->SetPointError(3,0,0.000318486);
   gre->SetPoint(4,1.462811e+09,1.678996);
   gre->SetPointError(4,0,0.000277923);
   gre->SetPoint(5,1.463418e+09,1.679682);
   gre->SetPointError(5,0,0.000267729);
   gre->SetPoint(6,1.464022e+09,1.673384);
   gre->SetPointError(6,0,0.0002812802);
   gre->SetPoint(7,1.464712e+09,1.667343);
   gre->SetPointError(7,0,0.000425598);
   gre->SetPoint(8,1.46523e+09,1.667715);
   gre->SetPointError(8,0,0.0004743358);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Drift Velocity Run Average",100,1.459006e+09,1.465537e+09);
   Graph_Graph2->SetMinimum(1.4);
   Graph_Graph2->SetMaximum(2.5);
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
   
   TF1 *f_dv_0.7 = new TF1("f_dv_0.7","pol0",1.459006e+09,1.465537e+09);
   f_dv_0.7->SetFillColor(19);
   f_dv_0.7->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_dv_0.7->SetLineColor(ci);
   f_dv_0.7->SetLineWidth(2);
   f_dv_0.7->SetChisquare(2370.762);
   f_dv_0.7->SetNDF(8);
   f_dv_0.7->GetXaxis()->SetLabelFont(42);
   f_dv_0.7->GetXaxis()->SetLabelSize(0.035);
   f_dv_0.7->GetXaxis()->SetTitleSize(0.035);
   f_dv_0.7->GetXaxis()->SetTitleFont(42);
   f_dv_0.7->GetYaxis()->SetLabelFont(42);
   f_dv_0.7->GetYaxis()->SetLabelSize(0.035);
   f_dv_0.7->GetYaxis()->SetTitleSize(0.035);
   f_dv_0.7->GetYaxis()->SetTitleFont(42);
   f_dv_0.7->SetParameter(0,1.675898);
   f_dv_0.7->SetParError(0,0.0001085247);
   f_dv_0.7->SetParLimits(0,0,0);
   f_dv_0.7->Draw("same");
   
   gre = new TGraphErrors(16);
   gre->SetName("Graph2");
   gre->SetTitle("Drift Velocity Run Average");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#00cc00");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#00cc00");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459306e+09,1.782373);
   gre->SetPointError(0,0,0.0007203141);
   gre->SetPoint(1,1.459782e+09,1.781094);
   gre->SetPointError(1,0,0.0005510131);
   gre->SetPoint(2,1.459789e+09,1.778085);
   gre->SetPointError(2,0,0.0003328475);
   gre->SetPoint(3,1.460052e+09,1.779093);
   gre->SetPointError(3,0,0.000504206);
   gre->SetPoint(4,1.460373e+09,1.78083);
   gre->SetPointError(4,0,0.0005855229);
   gre->SetPoint(5,1.460467e+09,1.777731);
   gre->SetPointError(5,0,0.0005907711);
   gre->SetPoint(6,1.460650e+09,1.778381);
   gre->SetPointError(6,0,0.0004740165);
   gre->SetPoint(7,1.460993e+09,1.773617);
   gre->SetPointError(7,0,0.0002740429);
   gre->SetPoint(8,1.461605e+09,1.77255);
   gre->SetPointError(8,0,0.0003719056);
   gre->SetPoint(9,1.462205e+09,1.768479);
   gre->SetPointError(9,0,0.0002536993);
   gre->SetPoint(10,1.462813e+09,1.771499);
   gre->SetPointError(10,0,0.0002586556);
   gre->SetPoint(11,1.463407e+09,1.775841);
   gre->SetPointError(11,0,0.0004349895);
   gre->SetPoint(12,1.463419e+09,1.772493);
   gre->SetPointError(12,0,0.0002467422);
   gre->SetPoint(13,1.464023e+09,1.770857);
   gre->SetPointError(13,0,0.0002502958);
   gre->SetPoint(14,1.464714e+09,1.766362);
   gre->SetPointError(14,0,0.0003522275);
   gre->SetPoint(15,1.465232e+09,1.76612);
   gre->SetPointError(15,0,0.0004022298);
   
   TH1F *Graph_Graph3 = new TH1F("Graph_Graph3","Drift Velocity Run Average",100,1.459006e+09,1.465537e+09);
   Graph_Graph3->SetMinimum(1.4);
   Graph_Graph3->SetMaximum(2.5);
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
   
   TF1 *f_dv_1.054 = new TF1("f_dv_1.054","pol0",1.459006e+09,1.465537e+09);
   f_dv_1.054->SetFillColor(19);
   f_dv_1.054->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   f_dv_1.054->SetLineColor(ci);
   f_dv_1.054->SetLineWidth(2);
   f_dv_1.054->SetChisquare(2252.11);
   f_dv_1.054->SetNDF(15);
   f_dv_1.054->GetXaxis()->SetLabelFont(42);
   f_dv_1.054->GetXaxis()->SetLabelSize(0.035);
   f_dv_1.054->GetXaxis()->SetTitleSize(0.035);
   f_dv_1.054->GetXaxis()->SetTitleFont(42);
   f_dv_1.054->GetYaxis()->SetLabelFont(42);
   f_dv_1.054->GetYaxis()->SetLabelSize(0.035);
   f_dv_1.054->GetYaxis()->SetTitleSize(0.035);
   f_dv_1.054->GetYaxis()->SetTitleFont(42);
   f_dv_1.054->SetParameter(0,1.772723);
   f_dv_1.054->SetParError(0,8.725173e-05);
   f_dv_1.054->SetParLimits(0,0,0);
   f_dv_1.054->Draw("same");
   
   gre = new TGraphErrors(10);
   gre->SetName("Graph3");
   gre->SetTitle("Drift Velocity Run Average");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff9933");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff9933");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459791e+09,1.873455);
   gre->SetPointError(0,0,0.0003111724);
   gre->SetPoint(1,1.460375e+09,1.874592);
   gre->SetPointError(1,0,0.0005314237);
   gre->SetPoint(2,1.460994e+09,1.868426);
   gre->SetPointError(2,0,0.0002886117);
   gre->SetPoint(3,1.461607e+09,1.868911);
   gre->SetPointError(3,0,0.0003594155);
   gre->SetPoint(4,1.462207e+09,1.86691);
   gre->SetPointError(4,0,0.0002335928);
   gre->SetPoint(5,1.462814e+09,1.86605);
   gre->SetPointError(5,0,0.0002458102);
   gre->SetPoint(6,1.463421e+09,1.866504);
   gre->SetPointError(6,0,0.0002374446);
   gre->SetPoint(7,1.464025e+09,1.868401);
   gre->SetPointError(7,0,0.0002490792);
   gre->SetPoint(8,1.464716e+09,1.865681);
   gre->SetPointError(8,0,0.0003391977);
   gre->SetPoint(9,1.465235e+09,1.867061);
   gre->SetPointError(9,0,0.0004392922);
   
   TH1F *Graph_Graph4 = new TH1F("Graph_Graph4","Drift Velocity Run Average",100,1.459006e+09,1.465537e+09);
   Graph_Graph4->SetMinimum(1.4);
   Graph_Graph4->SetMaximum(2.5);
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
   
   TF1 *f_dv_1.5 = new TF1("f_dv_1.5","pol0",1.459006e+09,1.465537e+09);
   f_dv_1.5->SetFillColor(19);
   f_dv_1.5->SetFillStyle(0);

   ci = TColor::GetColor("#ff9933");
   f_dv_1.5->SetLineColor(ci);
   f_dv_1.5->SetLineWidth(2);
   f_dv_1.5->SetChisquare(647.7762);
   f_dv_1.5->SetNDF(9);
   f_dv_1.5->GetXaxis()->SetLabelFont(42);
   f_dv_1.5->GetXaxis()->SetLabelSize(0.035);
   f_dv_1.5->GetXaxis()->SetTitleSize(0.035);
   f_dv_1.5->GetXaxis()->SetTitleFont(42);
   f_dv_1.5->GetYaxis()->SetLabelFont(42);
   f_dv_1.5->GetYaxis()->SetLabelSize(0.035);
   f_dv_1.5->GetYaxis()->SetTitleSize(0.035);
   f_dv_1.5->GetYaxis()->SetTitleFont(42);
   f_dv_1.5->SetParameter(0,1.867953);
   f_dv_1.5->SetParError(0,9.27596e-05);
   f_dv_1.5->SetParLimits(0,0,0);
   f_dv_1.5->Draw("same");
   
   gre = new TGraphErrors(14);
   gre->SetName("Graph4");
   gre->SetTitle("Drift Velocity Run Average");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff0000");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff0000");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459792e+09,2.007452);
   gre->SetPointError(0,0,0.0003144888);
   gre->SetPoint(1,1.460381e+09,2.008167);
   gre->SetPointError(1,0,0.000586254);
   gre->SetPoint(2,1.460996e+09,2.003328);
   gre->SetPointError(2,0,0.0002896023);
   gre->SetPoint(3,1.46161e+09,2.002882);
   gre->SetPointError(3,0,0.0003349438);
   gre->SetPoint(4,1.461857e+09,2.005821);
   gre->SetPointError(4,0,0.000674012);
   gre->SetPoint(5,1.462209e+09,2.002453);
   gre->SetPointError(5,0,0.000230398);
   gre->SetPoint(6,1.462816e+09,2.000116);
   gre->SetPointError(6,0,0.0002366587);
   gre->SetPoint(7,1.463423e+09,2.000497);
   gre->SetPointError(7,0,0.0002336881);
   gre->SetPoint(8,1.464027e+09,2.003006);
   gre->SetPointError(8,0,0.0002800756);
   gre->SetPoint(9,1.464104e+09,2.007831);
   gre->SetPointError(9,0,0.0006731102);
   gre->SetPoint(10,1.464705e+09,2.009975);
   gre->SetPointError(10,0,0.0005721018);
   gre->SetPoint(11,1.464717e+09,2.003373);
   gre->SetPointError(11,0,0.0004556441);
   gre->SetPoint(12,1.465224e+09,2.011166);
   gre->SetPointError(12,0,0.0007598496);
   gre->SetPoint(13,1.465237e+09,2.005071);
   gre->SetPointError(13,0,0.0004547164);
   
   TH1F *Graph_Graph5 = new TH1F("Graph_Graph5","Drift Velocity Run Average",100,1.459006e+09,1.465537e+09);
   Graph_Graph5->SetMinimum(1.4);
   Graph_Graph5->SetMaximum(2.5);
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
   
   TF1 *f_dv_2.356 = new TF1("f_dv_2.356","pol0",1.459006e+09,1.465537e+09);
   f_dv_2.356->SetFillColor(19);
   f_dv_2.356->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   f_dv_2.356->SetLineColor(ci);
   f_dv_2.356->SetLineWidth(2);
   f_dv_2.356->SetChisquare(899.7846);
   f_dv_2.356->SetNDF(13);
   f_dv_2.356->GetXaxis()->SetLabelFont(42);
   f_dv_2.356->GetXaxis()->SetLabelSize(0.035);
   f_dv_2.356->GetXaxis()->SetTitleSize(0.035);
   f_dv_2.356->GetXaxis()->SetTitleFont(42);
   f_dv_2.356->GetYaxis()->SetLabelFont(42);
   f_dv_2.356->GetYaxis()->SetLabelSize(0.035);
   f_dv_2.356->GetYaxis()->SetTitleSize(0.035);
   f_dv_2.356->GetYaxis()->SetTitleFont(42);
   f_dv_2.356->SetParameter(0,2.003162);
   f_dv_2.356->SetParError(0,9.111298e-05);
   f_dv_2.356->SetParLimits(0,0,0);
   f_dv_2.356->Draw("same");
   
   pt = new TPaveText(0.15,0.8,0.5,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 0.345kV: 1.56 +/- 1.02e-04 mm/s");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.75,0.5,0.8,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 0.7kV: 1.68 +/- 1.09e-04 mm/s");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.7,0.5,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.054kV: 1.77 +/- 8.73e-05 mm/s");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.65,0.5,0.7,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff9933");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.5kV: 1.87 +/- 9.28e-05 mm/s");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.6,0.5,0.65,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 2.356kV: 2.00 +/- 9.11e-05 mm/s");
   pt->Draw();
   33b5c80d377a4df9ac3addbee347fe68_1->Modified();
   33b5c80d377a4df9ac3addbee347fe68->cd();
  
// ------------>Primitives in pad: 33b5c80d377a4df9ac3addbee347fe68_2
   33b5c80d377a4df9ac3addbee347fe68_2 = new TPad("33b5c80d377a4df9ac3addbee347fe68_2", "33b5c80d377a4df9ac3addbee347fe68_2",0.51,0.01,0.99,0.99);
   33b5c80d377a4df9ac3addbee347fe68_2->Draw();
   33b5c80d377a4df9ac3addbee347fe68_2->cd();
   33b5c80d377a4df9ac3addbee347fe68_2->Range(1.458189e+09,1.25,1.466354e+09,1.75);
   33b5c80d377a4df9ac3addbee347fe68_2->SetFillColor(0);
   33b5c80d377a4df9ac3addbee347fe68_2->SetBorderMode(0);
   33b5c80d377a4df9ac3addbee347fe68_2->SetBorderSize(2);
   33b5c80d377a4df9ac3addbee347fe68_2->SetFrameBorderMode(0);
   33b5c80d377a4df9ac3addbee347fe68_2->SetFrameBorderMode(0);
   
   gre = new TGraphErrors(18);
   gre->SetName("Graph0");
   gre->SetTitle("Gate Time Mean Run Average");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff00ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff00ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459335e+09,1.478404);
   gre->SetPointError(0,0,0.001224169);
   gre->SetPoint(1,1.459344e+09,1.475153);
   gre->SetPointError(1,0,0.001325326);
   gre->SetPoint(2,1.459349e+09,1.482612);
   gre->SetPointError(2,0,0.001209569);
   gre->SetPoint(3,1.459354e+09,1.481729);
   gre->SetPointError(3,0,0.001129811);
   gre->SetPoint(4,1.459786e+09,1.465744);
   gre->SetPointError(4,0,0.001631097);
   gre->SetPoint(5,1.460986e+09,1.487903);
   gre->SetPointError(5,0,0.003486566);
   gre->SetPoint(6,1.46099e+09,1.436795);
   gre->SetPointError(6,0,0.001709263);
   gre->SetPoint(7,1.461261e+09,1.478725);
   gre->SetPointError(7,0,0.003524128);
   gre->SetPoint(8,1.4616e+09,1.404467);
   gre->SetPointError(8,0,0.002418358);
   gre->SetPoint(9,1.462196e+09,1.481029);
   gre->SetPointError(9,0,0.002901203);
   gre->SetPoint(10,1.462201e+09,1.430008);
   gre->SetPointError(10,0,0.001495207);
   gre->SetPoint(11,1.462455e+09,1.460882);
   gre->SetPointError(11,0,0.004216893);
   gre->SetPoint(12,1.462805e+09,1.477612);
   gre->SetPointError(12,0,0.003766102);
   gre->SetPoint(13,1.462809e+09,1.435082);
   gre->SetPointError(13,0,0.001358147);
   gre->SetPoint(14,1.463416e+09,1.432665);
   gre->SetPointError(14,0,0.001324102);
   gre->SetPoint(15,1.46402e+09,1.442719);
   gre->SetPointError(15,0,0.001503675);
   gre->SetPoint(16,1.464709e+09,1.427433);
   gre->SetPointError(16,0,0.002048351);
   gre->SetPoint(17,1.465228e+09,1.418864);
   gre->SetPointError(17,0,0.002642909);
   
   TH1F *Graph_Graph6 = new TH1F("Graph_Graph6","Gate Time Mean Run Average",100,1.459006e+09,1.465537e+09);
   Graph_Graph6->SetMinimum(1.3);
   Graph_Graph6->SetMaximum(1.7);
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
   Graph_Graph6->GetYaxis()->SetTitle("Gate Time [us after main S2]");
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
   
   TF1 *f_gtm_0.345 = new TF1("f_gtm_0.345","pol0",1.459006e+09,1.465537e+09);
   f_gtm_0.345->SetFillColor(19);
   f_gtm_0.345->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   f_gtm_0.345->SetLineColor(ci);
   f_gtm_0.345->SetLineWidth(2);
   f_gtm_0.345->SetChisquare(3707.729);
   f_gtm_0.345->SetNDF(17);
   f_gtm_0.345->GetXaxis()->SetLabelFont(42);
   f_gtm_0.345->GetXaxis()->SetLabelSize(0.035);
   f_gtm_0.345->GetXaxis()->SetTitleSize(0.035);
   f_gtm_0.345->GetXaxis()->SetTitleFont(42);
   f_gtm_0.345->GetYaxis()->SetLabelFont(42);
   f_gtm_0.345->GetYaxis()->SetLabelSize(0.035);
   f_gtm_0.345->GetYaxis()->SetTitleSize(0.035);
   f_gtm_0.345->GetYaxis()->SetTitleFont(42);
   f_gtm_0.345->SetParameter(0,1.457163);
   f_gtm_0.345->SetParError(0,0.0003957622);
   f_gtm_0.345->SetParLimits(0,0,0);
   f_gtm_0.345->Draw("same");
   
   pt = new TPaveText(0.2213596,0.9355248,0.7786404,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("Gate Time Mean Run Average");
   pt->Draw();
   
   gre = new TGraphErrors(9);
   gre->SetName("Graph1");
   gre->SetTitle("Gate Time Mean Run Average");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#0000ff");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459788e+09,1.464428);
   gre->SetPointError(0,0,0.001650896);
   gre->SetPoint(1,1.460991e+09,1.433591);
   gre->SetPointError(1,0,0.001590985);
   gre->SetPoint(2,1.461602e+09,1.408899);
   gre->SetPointError(2,0,0.002154659);
   gre->SetPoint(3,1.462203e+09,1.426172);
   gre->SetPointError(3,0,0.001258088);
   gre->SetPoint(4,1.462811e+09,1.432808);
   gre->SetPointError(4,0,0.001342512);
   gre->SetPoint(5,1.463418e+09,1.43162);
   gre->SetPointError(5,0,0.001292135);
   gre->SetPoint(6,1.464022e+09,1.436823);
   gre->SetPointError(6,0,0.00133139);
   gre->SetPoint(7,1.464712e+09,1.425008);
   gre->SetPointError(7,0,0.002205955);
   gre->SetPoint(8,1.46523e+09,1.424467);
   gre->SetPointError(8,0,0.002569428);
   
   TH1F *Graph_Graph7 = new TH1F("Graph_Graph7","Gate Time Mean Run Average",100,1.459006e+09,1.465537e+09);
   Graph_Graph7->SetMinimum(1.3);
   Graph_Graph7->SetMaximum(1.7);
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
   
   TF1 *f_gtm_0.7 = new TF1("f_gtm_0.7","pol0",1.459006e+09,1.465537e+09);
   f_gtm_0.7->SetFillColor(19);
   f_gtm_0.7->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   f_gtm_0.7->SetLineColor(ci);
   f_gtm_0.7->SetLineWidth(2);
   f_gtm_0.7->SetChisquare(550.5888);
   f_gtm_0.7->SetNDF(8);
   f_gtm_0.7->GetXaxis()->SetLabelFont(42);
   f_gtm_0.7->GetXaxis()->SetLabelSize(0.035);
   f_gtm_0.7->GetXaxis()->SetTitleSize(0.035);
   f_gtm_0.7->GetXaxis()->SetTitleFont(42);
   f_gtm_0.7->GetYaxis()->SetLabelFont(42);
   f_gtm_0.7->GetYaxis()->SetLabelSize(0.035);
   f_gtm_0.7->GetYaxis()->SetTitleSize(0.035);
   f_gtm_0.7->GetYaxis()->SetTitleFont(42);
   f_gtm_0.7->SetParameter(0,1.433149);
   f_gtm_0.7->SetParError(0,0.0005209988);
   f_gtm_0.7->SetParLimits(0,0,0);
   f_gtm_0.7->Draw("same");
   
   gre = new TGraphErrors(16);
   gre->SetName("Graph2");
   gre->SetTitle("Gate Time Mean Run Average");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#00cc00");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#00cc00");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459306e+09,1.517486);
   gre->SetPointError(0,0,0.004576158);
   gre->SetPoint(1,1.459782e+09,1.499898);
   gre->SetPointError(1,0,0.003751748);
   gre->SetPoint(2,1.459789e+09,1.461689);
   gre->SetPointError(2,0,0.001767761);
   gre->SetPoint(3,1.460052e+09,1.497115);
   gre->SetPointError(3,0,0.003466949);
   gre->SetPoint(4,1.460373e+09,1.430792);
   gre->SetPointError(4,0,0.003950242);
   gre->SetPoint(5,1.460467e+09,1.471097);
   gre->SetPointError(5,0,0.004013435);
   gre->SetPoint(6,1.460650e+09,1.470355);
   gre->SetPointError(6,0,0.003291676);
   gre->SetPoint(7,1.460993e+09,1.432308);
   gre->SetPointError(7,0,0.001491098);
   gre->SetPoint(8,1.461605e+09,1.406474);
   gre->SetPointError(8,0,0.002096447);
   gre->SetPoint(9,1.462205e+09,1.426588);
   gre->SetPointError(9,0,0.001308589);
   gre->SetPoint(10,1.462813e+09,1.430941);
   gre->SetPointError(10,0,0.001323853);
   gre->SetPoint(11,1.463407e+09,1.471454);
   gre->SetPointError(11,0,0.003076788);
   gre->SetPoint(12,1.463419e+09,1.431514);
   gre->SetPointError(12,0,0.001269779);
   gre->SetPoint(13,1.464023e+09,1.435433);
   gre->SetPointError(13,0,0.001232007);
   gre->SetPoint(14,1.464714e+09,1.426228);
   gre->SetPointError(14,0,0.002041211);
   gre->SetPoint(15,1.465232e+09,1.415446);
   gre->SetPointError(15,0,0.002487936);
   
   TH1F *Graph_Graph8 = new TH1F("Graph_Graph8","Gate Time Mean Run Average",100,1.459006e+09,1.465537e+09);
   Graph_Graph8->SetMinimum(1.3);
   Graph_Graph8->SetMaximum(1.7);
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
   
   TF1 *f_gtm_1.054 = new TF1("f_gtm_1.054","pol0",1.459006e+09,1.465537e+09);
   f_gtm_1.054->SetFillColor(19);
   f_gtm_1.054->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   f_gtm_1.054->SetLineColor(ci);
   f_gtm_1.054->SetLineWidth(2);
   f_gtm_1.054->SetChisquare(1818.127);
   f_gtm_1.054->SetNDF(15);
   f_gtm_1.054->GetXaxis()->SetLabelFont(42);
   f_gtm_1.054->GetXaxis()->SetLabelSize(0.035);
   f_gtm_1.054->GetXaxis()->SetTitleSize(0.035);
   f_gtm_1.054->GetXaxis()->SetTitleFont(42);
   f_gtm_1.054->GetYaxis()->SetLabelFont(42);
   f_gtm_1.054->GetYaxis()->SetLabelSize(0.035);
   f_gtm_1.054->GetYaxis()->SetTitleSize(0.035);
   f_gtm_1.054->GetYaxis()->SetTitleFont(42);
   f_gtm_1.054->SetParameter(0,1.437127);
   f_gtm_1.054->SetParError(0,0.0004789927);
   f_gtm_1.054->SetParLimits(0,0,0);
   f_gtm_1.054->Draw("same");
   
   gre = new TGraphErrors(10);
   gre->SetName("Graph3");
   gre->SetTitle("Gate Time Mean Run Average");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff9933");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff9933");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459791e+09,1.464756);
   gre->SetPointError(0,0,0.001572714);
   gre->SetPoint(1,1.460375e+09,1.430526);
   gre->SetPointError(1,0,0.00322815);
   gre->SetPoint(2,1.460994e+09,1.432236);
   gre->SetPointError(2,0,0.001578586);
   gre->SetPoint(3,1.461607e+09,1.41053);
   gre->SetPointError(3,0,0.001965264);
   gre->SetPoint(4,1.462207e+09,1.42596);
   gre->SetPointError(4,0,0.001228657);
   gre->SetPoint(5,1.462814e+09,1.429563);
   gre->SetPointError(5,0,0.001188603);
   gre->SetPoint(6,1.463421e+09,1.426477);
   gre->SetPointError(6,0,0.001209596);
   gre->SetPoint(7,1.464025e+09,1.437725);
   gre->SetPointError(7,0,0.001363834);
   gre->SetPoint(8,1.464716e+09,1.421499);
   gre->SetPointError(8,0,0.001997007);
   gre->SetPoint(9,1.465235e+09,1.421186);
   gre->SetPointError(9,0,0.002712048);
   
   TH1F *Graph_Graph9 = new TH1F("Graph_Graph9","Gate Time Mean Run Average",100,1.459006e+09,1.465537e+09);
   Graph_Graph9->SetMinimum(1.3);
   Graph_Graph9->SetMaximum(1.7);
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
   
   TF1 *f_gtm_1.5 = new TF1("f_gtm_1.5","pol0",1.459006e+09,1.465537e+09);
   f_gtm_1.5->SetFillColor(19);
   f_gtm_1.5->SetFillStyle(0);

   ci = TColor::GetColor("#ff9933");
   f_gtm_1.5->SetLineColor(ci);
   f_gtm_1.5->SetLineWidth(2);
   f_gtm_1.5->SetChisquare(661.7337);
   f_gtm_1.5->SetNDF(9);
   f_gtm_1.5->GetXaxis()->SetLabelFont(42);
   f_gtm_1.5->GetXaxis()->SetLabelSize(0.035);
   f_gtm_1.5->GetXaxis()->SetTitleSize(0.035);
   f_gtm_1.5->GetXaxis()->SetTitleFont(42);
   f_gtm_1.5->GetYaxis()->SetLabelFont(42);
   f_gtm_1.5->GetYaxis()->SetLabelSize(0.035);
   f_gtm_1.5->GetYaxis()->SetTitleSize(0.035);
   f_gtm_1.5->GetYaxis()->SetTitleFont(42);
   f_gtm_1.5->SetParameter(0,1.431302);
   f_gtm_1.5->SetParError(0,0.0004915609);
   f_gtm_1.5->SetParLimits(0,0,0);
   f_gtm_1.5->Draw("same");
   
   gre = new TGraphErrors(14);
   gre->SetName("Graph4");
   gre->SetTitle("Gate Time Mean Run Average");
   gre->SetFillColor(1);

   ci = TColor::GetColor("#ff0000");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#ff0000");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,1.459792e+09,1.464005);
   gre->SetPointError(0,0,0.001524507);
   gre->SetPoint(1,1.460381e+09,1.428432);
   gre->SetPointError(1,0,0.003202974);
   gre->SetPoint(2,1.460996e+09,1.431256);
   gre->SetPointError(2,0,0.001431152);
   gre->SetPoint(3,1.46161e+09,1.409089);
   gre->SetPointError(3,0,0.001638906);
   gre->SetPoint(4,1.461857e+09,1.43487);
   gre->SetPointError(4,0,0.003815584);
   gre->SetPoint(5,1.462209e+09,1.421946);
   gre->SetPointError(5,0,0.001150213);
   gre->SetPoint(6,1.462816e+09,1.427845);
   gre->SetPointError(6,0,0.001141922);
   gre->SetPoint(7,1.463423e+09,1.424555);
   gre->SetPointError(7,0,0.001138278);
   gre->SetPoint(8,1.464027e+09,1.437478);
   gre->SetPointError(8,0,0.001411418);
   gre->SetPoint(9,1.464104e+09,1.479059);
   gre->SetPointError(9,0,0.003826004);
   gre->SetPoint(10,1.464705e+09,1.465581);
   gre->SetPointError(10,0,0.00321927);
   gre->SetPoint(11,1.464717e+09,1.417616);
   gre->SetPointError(11,0,0.002535822);
   gre->SetPoint(12,1.465224e+09,1.465349);
   gre->SetPointError(12,0,0.004301592);
   gre->SetPoint(13,1.465237e+09,1.418444);
   gre->SetPointError(13,0,0.002535777);
   
   TH1F *Graph_Graph10 = new TH1F("Graph_Graph10","Gate Time Mean Run Average",100,1.459006e+09,1.465537e+09);
   Graph_Graph10->SetMinimum(1.3);
   Graph_Graph10->SetMaximum(1.7);
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
   
   TF1 *f_gtm_2.356 = new TF1("f_gtm_2.356","pol0",1.459006e+09,1.465537e+09);
   f_gtm_2.356->SetFillColor(19);
   f_gtm_2.356->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   f_gtm_2.356->SetLineColor(ci);
   f_gtm_2.356->SetLineWidth(2);
   f_gtm_2.356->SetChisquare(1161.159);
   f_gtm_2.356->SetNDF(13);
   f_gtm_2.356->GetXaxis()->SetLabelFont(42);
   f_gtm_2.356->GetXaxis()->SetLabelSize(0.035);
   f_gtm_2.356->GetXaxis()->SetTitleSize(0.035);
   f_gtm_2.356->GetXaxis()->SetTitleFont(42);
   f_gtm_2.356->GetYaxis()->SetLabelFont(42);
   f_gtm_2.356->GetYaxis()->SetLabelSize(0.035);
   f_gtm_2.356->GetYaxis()->SetTitleSize(0.035);
   f_gtm_2.356->GetYaxis()->SetTitleFont(42);
   f_gtm_2.356->SetParameter(0,1.430925);
   f_gtm_2.356->SetParError(0,0.0004572284);
   f_gtm_2.356->SetParLimits(0,0,0);
   f_gtm_2.356->Draw("same");
   
   pt = new TPaveText(0.15,0.8,0.5,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff00ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 0.345kV: 1.46 +/- 3.96e-04 us");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.75,0.5,0.8,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 0.7kV: 1.43 +/- 5.21e-04 us");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.7,0.5,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.054kV: 1.44 +/- 4.79e-04 us");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.65,0.5,0.7,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff9933");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 1.5kV: 1.43 +/- 4.92e-04 us");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.6,0.5,0.65,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#ff0000");
   pt->SetTextColor(ci);
   text = pt->AddText("V_{c} = 2.356kV: 1.43 +/- 4.57e-04 us");
   pt->Draw();
   33b5c80d377a4df9ac3addbee347fe68_2->Modified();
   33b5c80d377a4df9ac3addbee347fe68->cd();
   33b5c80d377a4df9ac3addbee347fe68->Modified();
   33b5c80d377a4df9ac3addbee347fe68->cd();
   33b5c80d377a4df9ac3addbee347fe68->SetSelected(33b5c80d377a4df9ac3addbee347fe68);
}
