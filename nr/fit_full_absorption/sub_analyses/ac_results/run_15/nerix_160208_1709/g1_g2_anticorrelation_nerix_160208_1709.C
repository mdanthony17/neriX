{
//=========Macro generated from canvas: e5ec2da759434335b5dadf51e6d377ef/
//=========  (Wed Feb 10 09:17:27 2016) by ROOT version5.34/26
   TCanvas *e5ec2da759434335b5dadf51e6d377ef = new TCanvas("e5ec2da759434335b5dadf51e6d377ef", "",10,45,704,527);
   e5ec2da759434335b5dadf51e6d377ef->SetHighLightColor(2);
   e5ec2da759434335b5dadf51e6d377ef->Range(78208.25,371.6832,175681.3,865.9037);
   e5ec2da759434335b5dadf51e6d377ef->SetFillColor(0);
   e5ec2da759434335b5dadf51e6d377ef->SetBorderMode(0);
   e5ec2da759434335b5dadf51e6d377ef->SetBorderSize(2);
   e5ec2da759434335b5dadf51e6d377ef->SetFrameBorderMode(0);
   e5ec2da759434335b5dadf51e6d377ef->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(5);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillColor(1);
   gre->SetPoint(0,103053.3,707.0321);
   gre->SetPointError(0,572.1204,2.385868);
   gre->SetPoint(1,119621.6,625.0011);
   gre->SetPointError(1,571.6688,2.486726);
   gre->SetPoint(2,131490.7,589.2632);
   gre->SetPointError(2,613.0218,2.39395);
   gre->SetPoint(3,134959.9,528.6891);
   gre->SetPointError(3,665.3396,2.354734);
   gre->SetPoint(4,145819.9,492.2432);
   gre->SetPointError(4,632.0593,2.391748);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,87955.55,165933.9);
   Graph_Graph1->SetMinimum(421.1053);
   Graph_Graph1->SetMaximum(816.4817);
   Graph_Graph1->SetDirectory(0);
   Graph_Graph1->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph1->SetLineColor(ci);
   Graph_Graph1->GetXaxis()->SetTitle("S2 at 163 keV Peak");
   Graph_Graph1->GetXaxis()->SetLabelFont(42);
   Graph_Graph1->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleOffset(1.12);
   Graph_Graph1->GetXaxis()->SetTitleFont(42);
   Graph_Graph1->GetYaxis()->SetTitle("S1 at 163 keV Peak [PE]");
   Graph_Graph1->GetYaxis()->SetLabelFont(42);
   Graph_Graph1->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleFont(42);
   Graph_Graph1->GetZaxis()->SetLabelFont(42);
   Graph_Graph1->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1);
   
   
   TF1 *fG1 = new TF1("fG1","pol1",98084.09,150849);
   fG1->SetFillColor(19);
   fG1->SetFillStyle(0);
   fG1->SetLineColor(2);
   fG1->SetLineWidth(1);
   fG1->SetLineStyle(9);
   fG1->SetChisquare(58.61301);
   fG1->SetNDF(3);
   fG1->GetXaxis()->SetLabelFont(42);
   fG1->GetXaxis()->SetLabelSize(0.035);
   fG1->GetXaxis()->SetTitleSize(0.035);
   fG1->GetXaxis()->SetTitleFont(42);
   fG1->GetYaxis()->SetLabelFont(42);
   fG1->GetYaxis()->SetLabelSize(0.035);
   fG1->GetYaxis()->SetTitleSize(0.035);
   fG1->GetYaxis()->SetTitleFont(42);
   fG1->SetParameter(0,1242.115);
   fG1->SetParError(0,15.39115);
   fG1->SetParLimits(0,0,100000);
   fG1->SetParameter(1,-0.005145572);
   fG1->SetParError(1,0.0001211334);
   fG1->SetParLimits(1,-0.2,0.2);
   gre->GetListOfFunctions()->Add(fG1);
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.2,0.75,0.6,0.85,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   TText *text = pt->AddText("g1 = 0.1038 +/- 0.0047");
   text = pt->AddText("g2 = 20.1776 +/- 1.0340");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.65,0.6,0.75,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);

   ci = TColor::GetColor("#0000ff");
   pt->SetTextColor(ci);
   pt->Draw();
   TMarker *marker = new TMarker(123803.2,608.086,22);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(22);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(148458.9,481.2186,23);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(23);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   marker = new TMarker(97728.39,742.256,33);
   marker->SetMarkerColor(9);
   marker->SetMarkerStyle(33);
   marker->SetMarkerSize(1.5);
   marker->Draw();
   
   TF1 *fDraw = new TF1("fDraw","pol1",87955.55,165933.9);
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
   fDraw->SetParameter(0,1242.115);
   fDraw->SetParError(0,0);
   fDraw->SetParLimits(0,0,0);
   fDraw->SetParameter(1,-0.005145572);
   fDraw->SetParError(1,0);
   fDraw->SetParLimits(1,0,0);
   fDraw->Draw("same");
   e5ec2da759434335b5dadf51e6d377ef->Modified();
   e5ec2da759434335b5dadf51e6d377ef->cd();
   e5ec2da759434335b5dadf51e6d377ef->SetSelected(e5ec2da759434335b5dadf51e6d377ef);
}
