{
//=========Macro generated from canvas: c6fda1299e54491abe90276898ea80f4/
//=========  (Wed Feb 10 09:24:11 2016) by ROOT version5.34/26
   TCanvas *c6fda1299e54491abe90276898ea80f4 = new TCanvas("c6fda1299e54491abe90276898ea80f4", "",10,45,704,527);
   c6fda1299e54491abe90276898ea80f4->SetHighLightColor(2);
   c6fda1299e54491abe90276898ea80f4->Range(283369.7,806.9241,622205,2151.351);
   c6fda1299e54491abe90276898ea80f4->SetFillColor(0);
   c6fda1299e54491abe90276898ea80f4->SetBorderMode(0);
   c6fda1299e54491abe90276898ea80f4->SetBorderSize(2);
   c6fda1299e54491abe90276898ea80f4->SetFrameBorderMode(0);
   c6fda1299e54491abe90276898ea80f4->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,385602.3,1687.847);
   gre->SetPointError(0,1842.755,8.313293);
   gre->SetPoint(1,432582.3,1482.053);
   gre->SetPointError(1,1655.132,7.528135);
   gre->SetPoint(2,449478.2,1395.603);
   gre->SetPointError(2,1628.976,6.928048);
   gre->SetPoint(3,471515.8,1305.406);
   gre->SetPointError(3,1691,6.927704);
   gre->SetPoint(4,492365.2,1229.505);
   gre->SetPointError(4,1443.737,6.117162);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,317253.2,588321.5);
   Graph_Graph1->SetMinimum(941.3668);
   Graph_Graph1->SetMaximum(2016.908);
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
   
   
   TF1 *fG1 = new TF1("fG1","pol1",372754.6,504813.8);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(2.7387);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,3348.075);
   fG1->SetParError(0,56.75997);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.004319484);
   fG1->SetParError(1,0.0001250988);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.0898 +/- 0.0042");
   text = pt->AddText("g2 = 20.7808 +/- 1.1466");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(451683.1,1405.148,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(534837.7,1045.963,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(352503.5,1833.553,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",317253.2,588321.5);
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
   fDraw->SetParameter(0,3348.075);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.004319484);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   c6fda1299e54491abe90276898ea80f4->Modified();
   c6fda1299e54491abe90276898ea80f4->cd();
   c6fda1299e54491abe90276898ea80f4->SetSelected(c6fda1299e54491abe90276898ea80f4);
}
