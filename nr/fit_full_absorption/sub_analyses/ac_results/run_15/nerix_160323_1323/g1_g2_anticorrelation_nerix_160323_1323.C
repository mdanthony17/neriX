{
//=========Macro generated from canvas: 6cc2ad9e79cf44e38bf302c544b1f659/
//=========  (Tue Mar 29 11:27:55 2016) by ROOT version5.34/26
   TCanvas *6cc2ad9e79cf44e38bf302c544b1f659 = new TCanvas("6cc2ad9e79cf44e38bf302c544b1f659", "",10,45,704,527);
   6cc2ad9e79cf44e38bf302c544b1f659->SetHighLightColor(2);
   6cc2ad9e79cf44e38bf302c544b1f659->Range(159162.7,648.9839,543173.9,2571.387);
   6cc2ad9e79cf44e38bf302c544b1f659->SetFillColor(0);
   6cc2ad9e79cf44e38bf302c544b1f659->SetBorderMode(0);
   6cc2ad9e79cf44e38bf302c544b1f659->SetBorderSize(2);
   6cc2ad9e79cf44e38bf302c544b1f659->SetFrameBorderMode(0);
   6cc2ad9e79cf44e38bf302c544b1f659->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,233860.3,2085.358);
   gre->SetPointError(0,1505.847,11.14261);
   gre->SetPoint(1,277000.5,1783.072);
   gre->SetPointError(1,1156.108,7.989606);
   gre->SetPoint(2,305096.2,1628.833);
   gre->SetPointError(2,1014.317,7.599294);
   gre->SetPoint(3,323540.4,1557.787);
   gre->SetPointError(3,1067.356,6.76923);
   gre->SetPoint(4,359873.3,1439.045);
   gre->SetPointError(4,871.833,6.146325);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,197563.9,504772.8);
   Graph_Graph1->SetMinimum(841.2242);
   Graph_Graph1->SetMaximum(2379.147);
   Graph_Graph1->SetDirectory(0);
   Graph_Graph1->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph1->SetLineColor(ci);
   Graph_Graph1->GetXaxis()->SetTitle("S2 at 662 keV Peak");
   Graph_Graph1->GetXaxis()->SetLabelFont(42);
   Graph_Graph1->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleOffset(1.12);
   Graph_Graph1->GetXaxis()->SetTitleFont(42);
   Graph_Graph1->GetYaxis()->SetTitle("S1 at 662 keV Peak [PE]");
   Graph_Graph1->GetYaxis()->SetLabelFont(42);
   Graph_Graph1->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleFont(42);
   Graph_Graph1->GetZaxis()->SetLabelFont(42);
   Graph_Graph1->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1);
   
   
   TF1 *fG1 = new TF1("fG1","pol1",219515.4,373584.2);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(82.52301);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,3141.084);
   fG1->SetParError(0,33.38431);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.004824746);
   fG1->SetParError(1,0.0001054557);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0650 +/- 0.0029");
   text = pt->AddText("g2 = 13.4731 +/- 0.6748");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(389236.5,1270.727,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(458884.3,934.6935,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(303530,1684.197,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",197563.9,504772.8);
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
   fDraw->SetParameter(0,3141.084);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.004824746);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   6cc2ad9e79cf44e38bf302c544b1f659->Modified();
   6cc2ad9e79cf44e38bf302c544b1f659->cd();
   6cc2ad9e79cf44e38bf302c544b1f659->SetSelected(6cc2ad9e79cf44e38bf302c544b1f659);
}
