{
//=========Macro generated from canvas: 2b79dd64af1b416e870c7192792da0bb/
//=========  (Tue Feb 16 14:15:02 2016) by ROOT version5.34/26
   TCanvas *2b79dd64af1b416e870c7192792da0bb = new TCanvas("2b79dd64af1b416e870c7192792da0bb", "",10,45,704,527);
   2b79dd64af1b416e870c7192792da0bb->SetHighLightColor(2);
   2b79dd64af1b416e870c7192792da0bb->Range(-6.875,0.7409701,11.875,5.350097);
   2b79dd64af1b416e870c7192792da0bb->SetFillColor(0);
   2b79dd64af1b416e870c7192792da0bb->SetBorderMode(0);
   2b79dd64af1b416e870c7192792da0bb->SetBorderSize(2);
   2b79dd64af1b416e870c7192792da0bb->SetLogy();
   2b79dd64af1b416e870c7192792da0bb->SetFrameBorderMode(0);
   2b79dd64af1b416e870c7192792da0bb->SetFrameBorderMode(0);
   
   TH1F *hGaus = new TH1F("hGaus","Time between Trigger and First S1 in Waveform - nerix_160209_0955",15,-5,10);
   hGaus->SetBinContent(0,95399);
   hGaus->SetBinContent(1,95);
   hGaus->SetBinContent(2,109);
   hGaus->SetBinContent(3,127);
   hGaus->SetBinContent(4,162);
   hGaus->SetBinContent(5,273);
   hGaus->SetBinContent(6,2712);
   hGaus->SetBinContent(7,40690);
   hGaus->SetBinContent(8,15508);
   hGaus->SetBinContent(9,757);
   hGaus->SetBinContent(10,138);
   hGaus->SetBinContent(11,80);
   hGaus->SetBinContent(12,59);
   hGaus->SetBinContent(13,49);
   hGaus->SetBinContent(14,38);
   hGaus->SetBinContent(15,38);
   hGaus->SetBinContent(16,884409);
   hGaus->SetBinError(0,308.8673);
   hGaus->SetBinError(1,9.746794);
   hGaus->SetBinError(2,10.44031);
   hGaus->SetBinError(3,11.26943);
   hGaus->SetBinError(4,12.72792);
   hGaus->SetBinError(5,16.52271);
   hGaus->SetBinError(6,52.07687);
   hGaus->SetBinError(7,201.7176);
   hGaus->SetBinError(8,124.5311);
   hGaus->SetBinError(9,27.51363);
   hGaus->SetBinError(10,11.74734);
   hGaus->SetBinError(11,8.944272);
   hGaus->SetBinError(12,7.681146);
   hGaus->SetBinError(13,7);
   hGaus->SetBinError(14,6.164414);
   hGaus->SetBinError(15,6.164414);
   hGaus->SetBinError(16,940.4302);
   hGaus->SetEntries(1040643);
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
   TText *text = pt->AddText("Time between Trigger and First S1 in Waveform - nerix_160209_0955");
   pt->Draw();
   TLine *line = new TLine(0,38,0,40690);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(6,38,6,40690);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   2b79dd64af1b416e870c7192792da0bb->Modified();
   2b79dd64af1b416e870c7192792da0bb->cd();
   2b79dd64af1b416e870c7192792da0bb->SetSelected(2b79dd64af1b416e870c7192792da0bb);
}
