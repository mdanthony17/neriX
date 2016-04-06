{
//=========Macro generated from canvas: 36013e79711044b6b14fa20959e6feae/
//=========  (Mon Mar 21 14:48:07 2016) by ROOT version5.34/26
   TCanvas *36013e79711044b6b14fa20959e6feae = new TCanvas("36013e79711044b6b14fa20959e6feae", "",10,45,704,527);
   36013e79711044b6b14fa20959e6feae->SetHighLightColor(2);
   36013e79711044b6b14fa20959e6feae->Range(-6.875,1.067579,11.875,5.345421);
   36013e79711044b6b14fa20959e6feae->SetFillColor(0);
   36013e79711044b6b14fa20959e6feae->SetBorderMode(0);
   36013e79711044b6b14fa20959e6feae->SetBorderSize(2);
   36013e79711044b6b14fa20959e6feae->SetLogy();
   36013e79711044b6b14fa20959e6feae->SetFrameBorderMode(0);
   36013e79711044b6b14fa20959e6feae->SetFrameBorderMode(0);
   
   TH1F *hGaus = new TH1F("hGaus","Time between Trigger and First S1 in Waveform - nerix_160229_1015",15,-5,10);
   hGaus->SetBinContent(0,625075);
   hGaus->SetBinContent(1,476);
   hGaus->SetBinContent(2,459);
   hGaus->SetBinContent(3,466);
   hGaus->SetBinContent(4,517);
   hGaus->SetBinContent(5,620);
   hGaus->SetBinContent(6,830);
   hGaus->SetBinContent(7,6125);
   hGaus->SetBinContent(8,43452);
   hGaus->SetBinContent(9,9543);
   hGaus->SetBinContent(10,731);
   hGaus->SetBinContent(11,229);
   hGaus->SetBinContent(12,112);
   hGaus->SetBinContent(13,106);
   hGaus->SetBinContent(14,71);
   hGaus->SetBinContent(15,91);
   hGaus->SetBinContent(16,1375138);
   hGaus->SetBinError(0,790.6168);
   hGaus->SetBinError(1,21.81742);
   hGaus->SetBinError(2,21.42429);
   hGaus->SetBinError(3,21.58703);
   hGaus->SetBinError(4,22.73763);
   hGaus->SetBinError(5,24.8998);
   hGaus->SetBinError(6,28.80972);
   hGaus->SetBinError(7,78.26238);
   hGaus->SetBinError(8,208.4514);
   hGaus->SetBinError(9,97.68828);
   hGaus->SetBinError(10,27.03701);
   hGaus->SetBinError(11,15.13275);
   hGaus->SetBinError(12,10.58301);
   hGaus->SetBinError(13,10.29563);
   hGaus->SetBinError(14,8.42615);
   hGaus->SetBinError(15,9.539392);
   hGaus->SetBinError(16,1172.663);
   hGaus->SetEntries(2064041);
   hGaus->SetStats(0);
   hGaus->SetFillStyle(0);
   hGaus->SetMarkerStyle(20);
   hGaus->SetMarkerSize(0);
   hGaus->GetXaxis()->SetTitle("TrigLeftEdge[] - S1sPeak[S1Order[0]]");
   hGaus->GetXaxis()->SetLabelFont(42);
   hGaus->GetXaxis()->SetLabelSize(0.035);
   hGaus->GetXaxis()->SetTitleSize(0.035);
   hGaus->GetXaxis()->SetTitleFont(42);
   hGaus->GetYaxis()->SetTitle("Counts");
   hGaus->GetYaxis()->SetLabelFont(42);
   hGaus->GetYaxis()->SetLabelSize(0.035);
   hGaus->GetYaxis()->SetTitleSize(0.035);
   hGaus->GetYaxis()->SetTitleFont(42);
   hGaus->GetZaxis()->SetLabelFont(42);
   hGaus->GetZaxis()->SetLabelSize(0.035);
   hGaus->GetZaxis()->SetTitleSize(0.035);
   hGaus->GetZaxis()->SetTitleFont(42);
   hGaus->Draw("");
   
   TPaveText *pt = new TPaveText(0.15,0.9374,0.85,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Time between Trigger and First S1 in Waveform - nerix_160229_1015");
   pt->Draw();
   TLine *line = new TLine(0,71,0,43452);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(6,71,6,43452);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   36013e79711044b6b14fa20959e6feae->Modified();
   36013e79711044b6b14fa20959e6feae->cd();
   36013e79711044b6b14fa20959e6feae->SetSelected(36013e79711044b6b14fa20959e6feae);
}
