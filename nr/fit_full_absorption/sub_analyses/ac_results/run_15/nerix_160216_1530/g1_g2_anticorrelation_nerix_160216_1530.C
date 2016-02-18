{
//=========Macro generated from canvas: 678ade5a91ab450ca9ffa2b4807927e8/
//=========  (Wed Feb 17 15:21:22 2016) by ROOT version5.34/26
   TCanvas *678ade5a91ab450ca9ffa2b4807927e8 = new TCanvas("678ade5a91ab450ca9ffa2b4807927e8", "",10,45,704,527);
   678ade5a91ab450ca9ffa2b4807927e8->SetHighLightColor(2);
   678ade5a91ab450ca9ffa2b4807927e8->Range(285849.3,787.7649,627649.6,2100.27);
   678ade5a91ab450ca9ffa2b4807927e8->SetFillColor(0);
   678ade5a91ab450ca9ffa2b4807927e8->SetBorderMode(0);
   678ade5a91ab450ca9ffa2b4807927e8->SetBorderSize(2);
   678ade5a91ab450ca9ffa2b4807927e8->SetFrameBorderMode(0);
   678ade5a91ab450ca9ffa2b4807927e8->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,385787.8,1671.168);
   gre->SetPointError(0,2271.502,10.37047);
   gre->SetPoint(1,425639,1476.794);
   gre->SetPointError(1,1999.151,8.418945);
   gre->SetPoint(2,451817.6,1368.788);
   gre->SetPointError(2,2033.377,8.976083);
   gre->SetPoint(3,468964.2,1309.427);
   gre->SetPointError(3,1771.365,7.390211);
   gre->SetPoint(4,493971,1211.44);
   gre->SetPointError(4,1786.604,7.317758);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,320029.3,593469.6);
   Graph_Graph1->SetMinimum(919.0154);
   Graph_Graph1->SetMaximum(1969.019);
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
   
   
   TF1 *fG1 = new TF1("fG1","pol1",372292.2,506981.7);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(3.663774);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,3268.58);
   fG1->SetParError(0,66.68671);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.004180345);
   fG1->SetParError(1,0.0001469501);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0876 +/- 0.0042");
   text = pt->AddText("g2 = 20.9627 +/- 1.2525");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(455635.5,1371.785,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(539517.8,1021.128,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(355588.1,1790.018,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",320029.3,593469.6);
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
   fDraw->SetParameter(0,3268.58);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.004180345);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   678ade5a91ab450ca9ffa2b4807927e8->Modified();
   678ade5a91ab450ca9ffa2b4807927e8->cd();
   678ade5a91ab450ca9ffa2b4807927e8->SetSelected(678ade5a91ab450ca9ffa2b4807927e8);
}
