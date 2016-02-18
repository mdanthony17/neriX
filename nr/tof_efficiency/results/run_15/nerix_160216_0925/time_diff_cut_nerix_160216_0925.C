{
//=========Macro generated from canvas: 0f8050a913a04aeead0f4abe68d21d05/
//=========  (Wed Feb 17 15:30:40 2016) by ROOT version5.34/26
   TCanvas *0f8050a913a04aeead0f4abe68d21d05 = new TCanvas("0f8050a913a04aeead0f4abe68d21d05", "",10,45,704,527);
   0f8050a913a04aeead0f4abe68d21d05->SetHighLightColor(2);
   0f8050a913a04aeead0f4abe68d21d05->Range(-6.875,0.9478538,11.875,5.400693);
   0f8050a913a04aeead0f4abe68d21d05->SetFillColor(0);
   0f8050a913a04aeead0f4abe68d21d05->SetBorderMode(0);
   0f8050a913a04aeead0f4abe68d21d05->SetBorderSize(2);
   0f8050a913a04aeead0f4abe68d21d05->SetLogy();
   0f8050a913a04aeead0f4abe68d21d05->SetFrameBorderMode(0);
   0f8050a913a04aeead0f4abe68d21d05->SetFrameBorderMode(0);
   
   TH1F *hGaus = new TH1F("hGaus","Time between Trigger and First S1 in Waveform - nerix_160216_0925",15,-5,10);
   hGaus->SetBinContent(0,236146);
   hGaus->SetBinContent(1,213);
   hGaus->SetBinContent(2,216);
   hGaus->SetBinContent(3,255);
   hGaus->SetBinContent(4,280);
   hGaus->SetBinContent(5,468);
   hGaus->SetBinContent(6,5988);
   hGaus->SetBinContent(7,47410);
   hGaus->SetBinContent(8,14367);
   hGaus->SetBinContent(9,936);
   hGaus->SetBinContent(10,189);
   hGaus->SetBinContent(11,105);
   hGaus->SetBinContent(12,59);
   hGaus->SetBinContent(13,64);
   hGaus->SetBinContent(14,78);
   hGaus->SetBinContent(15,57);
   hGaus->SetBinContent(16,1130320);
   hGaus->SetBinError(0,485.9486);
   hGaus->SetBinError(1,14.59452);
   hGaus->SetBinError(2,14.69694);
   hGaus->SetBinError(3,15.96872);
   hGaus->SetBinError(4,16.7332);
   hGaus->SetBinError(5,21.63331);
   hGaus->SetBinError(6,77.38217);
   hGaus->SetBinError(7,217.7384);
   hGaus->SetBinError(8,119.8624);
   hGaus->SetBinError(9,30.59412);
   hGaus->SetBinError(10,13.74773);
   hGaus->SetBinError(11,10.24695);
   hGaus->SetBinError(12,7.681146);
   hGaus->SetBinError(13,8);
   hGaus->SetBinError(14,8.831761);
   hGaus->SetBinError(15,7.549834);
   hGaus->SetBinError(16,1063.165);
   hGaus->SetEntries(1437151);
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
   TText *text = pt->AddText("Time between Trigger and First S1 in Waveform - nerix_160216_0925");
   pt->Draw();
   TLine *line = new TLine(0,57,0,47410);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(6,57,6,47410);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   0f8050a913a04aeead0f4abe68d21d05->Modified();
   0f8050a913a04aeead0f4abe68d21d05->cd();
   0f8050a913a04aeead0f4abe68d21d05->SetSelected(0f8050a913a04aeead0f4abe68d21d05);
}
