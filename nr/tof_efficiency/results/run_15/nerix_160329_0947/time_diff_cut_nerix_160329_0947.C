{
//=========Macro generated from canvas: 96940c7078654883b9042a30960d727e/
//=========  (Tue Mar 29 11:07:14 2016) by ROOT version5.34/26
   TCanvas *96940c7078654883b9042a30960d727e = new TCanvas("96940c7078654883b9042a30960d727e", "",10,45,704,527);
   96940c7078654883b9042a30960d727e->SetHighLightColor(2);
   96940c7078654883b9042a30960d727e->Range(-6.875,1.513242,11.875,4.791651);
   96940c7078654883b9042a30960d727e->SetFillColor(0);
   96940c7078654883b9042a30960d727e->SetBorderMode(0);
   96940c7078654883b9042a30960d727e->SetBorderSize(2);
   96940c7078654883b9042a30960d727e->SetLogy();
   96940c7078654883b9042a30960d727e->SetFrameBorderMode(0);
   96940c7078654883b9042a30960d727e->SetFrameBorderMode(0);
   
   TH1F *hGaus = new TH1F("hGaus","Time between Trigger and First S1 in Waveform - nerix_160329_0947",15,-5,10);
   hGaus->SetBinContent(0,248045);
   hGaus->SetBinContent(1,252);
   hGaus->SetBinContent(2,299);
   hGaus->SetBinContent(3,222);
   hGaus->SetBinContent(4,279);
   hGaus->SetBinContent(5,263);
   hGaus->SetBinContent(6,335);
   hGaus->SetBinContent(7,830);
   hGaus->SetBinContent(8,15232);
   hGaus->SetBinContent(9,12927);
   hGaus->SetBinContent(10,2083);
   hGaus->SetBinContent(11,221);
   hGaus->SetBinContent(12,175);
   hGaus->SetBinContent(13,168);
   hGaus->SetBinContent(14,151);
   hGaus->SetBinContent(15,154);
   hGaus->SetBinContent(16,1733541);
   hGaus->SetBinError(0,498.0412);
   hGaus->SetBinError(1,15.87451);
   hGaus->SetBinError(2,17.29162);
   hGaus->SetBinError(3,14.89966);
   hGaus->SetBinError(4,16.70329);
   hGaus->SetBinError(5,16.21727);
   hGaus->SetBinError(6,18.30301);
   hGaus->SetBinError(7,28.80972);
   hGaus->SetBinError(8,123.418);
   hGaus->SetBinError(9,113.697);
   hGaus->SetBinError(10,45.63989);
   hGaus->SetBinError(11,14.86607);
   hGaus->SetBinError(12,13.22876);
   hGaus->SetBinError(13,12.96148);
   hGaus->SetBinError(14,12.28821);
   hGaus->SetBinError(15,12.40967);
   hGaus->SetBinError(16,1316.64);
   hGaus->SetEntries(2015177);
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
   TText *text = pt->AddText("Time between Trigger and First S1 in Waveform - nerix_160329_0947");
   pt->Draw();
   TLine *line = new TLine(0,151,0,15232);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(6,151,6,15232);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   96940c7078654883b9042a30960d727e->Modified();
   96940c7078654883b9042a30960d727e->cd();
   96940c7078654883b9042a30960d727e->SetSelected(96940c7078654883b9042a30960d727e);
}
