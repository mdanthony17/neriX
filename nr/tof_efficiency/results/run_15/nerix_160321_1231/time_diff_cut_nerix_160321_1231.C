{
//=========Macro generated from canvas: c02cbba331f9443c8cad01e7fda3dcd5/
//=========  (Mon Mar 21 14:46:31 2016) by ROOT version5.34/26
   TCanvas *c02cbba331f9443c8cad01e7fda3dcd5 = new TCanvas("c02cbba331f9443c8cad01e7fda3dcd5", "",380,145,704,527);
   c02cbba331f9443c8cad01e7fda3dcd5->SetHighLightColor(2);
   c02cbba331f9443c8cad01e7fda3dcd5->Range(-6.875,1.369037,11.875,4.967771);
   c02cbba331f9443c8cad01e7fda3dcd5->SetFillColor(0);
   c02cbba331f9443c8cad01e7fda3dcd5->SetBorderMode(0);
   c02cbba331f9443c8cad01e7fda3dcd5->SetBorderSize(2);
   c02cbba331f9443c8cad01e7fda3dcd5->SetLogy();
   c02cbba331f9443c8cad01e7fda3dcd5->SetFrameBorderMode(0);
   c02cbba331f9443c8cad01e7fda3dcd5->SetFrameBorderMode(0);
   
   TH1F *hGaus = new TH1F("hGaus","Time between Trigger and First S1 in Waveform - nerix_160321_1231",15,-5,10);
   hGaus->SetBinContent(0,229423);
   hGaus->SetBinContent(1,224);
   hGaus->SetBinContent(2,254);
   hGaus->SetBinContent(3,207);
   hGaus->SetBinContent(4,248);
   hGaus->SetBinContent(5,283);
   hGaus->SetBinContent(6,337);
   hGaus->SetBinContent(7,3297);
   hGaus->SetBinContent(8,21251);
   hGaus->SetBinContent(9,5267);
   hGaus->SetBinContent(10,740);
   hGaus->SetBinContent(11,159);
   hGaus->SetBinContent(12,149);
   hGaus->SetBinContent(13,140);
   hGaus->SetBinContent(14,143);
   hGaus->SetBinContent(15,118);
   hGaus->SetBinContent(16,1842074);
   hGaus->SetBinError(0,478.9812);
   hGaus->SetBinError(1,14.96663);
   hGaus->SetBinError(2,15.93738);
   hGaus->SetBinError(3,14.38749);
   hGaus->SetBinError(4,15.74802);
   hGaus->SetBinError(5,16.8226);
   hGaus->SetBinError(6,18.35756);
   hGaus->SetBinError(7,57.41951);
   hGaus->SetBinError(8,145.7772);
   hGaus->SetBinError(9,72.5741);
   hGaus->SetBinError(10,27.20294);
   hGaus->SetBinError(11,12.60952);
   hGaus->SetBinError(12,12.20656);
   hGaus->SetBinError(13,11.83216);
   hGaus->SetBinError(14,11.95826);
   hGaus->SetBinError(15,10.86278);
   hGaus->SetBinError(16,1357.23);
   hGaus->SetEntries(2104314);
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
   TText *text = pt->AddText("Time between Trigger and First S1 in Waveform - nerix_160321_1231");
   pt->Draw();
   TLine *line = new TLine(0,118,0,21251);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(6,118,6,21251);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   c02cbba331f9443c8cad01e7fda3dcd5->Modified();
   c02cbba331f9443c8cad01e7fda3dcd5->cd();
   c02cbba331f9443c8cad01e7fda3dcd5->SetSelected(c02cbba331f9443c8cad01e7fda3dcd5);
}
