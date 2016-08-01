{
//=========Macro generated from canvas: 112e94a25d414801a0045c9ea26f7653/
//=========  (Mon Apr 11 09:38:33 2016) by ROOT version5.34/26
   TCanvas *112e94a25d414801a0045c9ea26f7653 = new TCanvas("112e94a25d414801a0045c9ea26f7653", "",10,45,704,527);
   112e94a25d414801a0045c9ea26f7653->SetHighLightColor(2);
   112e94a25d414801a0045c9ea26f7653->Range(0.2499999,-42.9984,7.75,386.9856);
   112e94a25d414801a0045c9ea26f7653->SetFillColor(0);
   112e94a25d414801a0045c9ea26f7653->SetBorderMode(0);
   112e94a25d414801a0045c9ea26f7653->SetBorderSize(2);
   112e94a25d414801a0045c9ea26f7653->SetFrameBorderMode(0);
   112e94a25d414801a0045c9ea26f7653->SetFrameBorderMode(0);
   
   TH1F *h_photon_yield = new TH1F("h_photon_yield","photon_yield",100,1,7);
   h_photon_yield->SetBinContent(83,10);
   h_photon_yield->SetBinContent(84,310);
   h_photon_yield->SetBinError(83,3.162278);
   h_photon_yield->SetBinError(84,17.60682);
   h_photon_yield->SetEntries(320);
   h_photon_yield->SetStats(0);
   h_photon_yield->SetFillStyle(0);
   h_photon_yield->SetMarkerStyle(20);
   h_photon_yield->SetMarkerSize(0);
   h_photon_yield->GetXaxis()->SetLabelFont(42);
   h_photon_yield->GetXaxis()->SetLabelSize(0.035);
   h_photon_yield->GetXaxis()->SetTitleSize(0.035);
   h_photon_yield->GetXaxis()->SetTitleFont(42);
   h_photon_yield->GetYaxis()->SetLabelFont(42);
   h_photon_yield->GetYaxis()->SetLabelSize(0.035);
   h_photon_yield->GetYaxis()->SetTitleSize(0.035);
   h_photon_yield->GetYaxis()->SetTitleFont(42);
   h_photon_yield->GetZaxis()->SetLabelFont(42);
   h_photon_yield->GetZaxis()->SetLabelSize(0.035);
   h_photon_yield->GetZaxis()->SetTitleSize(0.035);
   h_photon_yield->GetZaxis()->SetTitleFont(42);
   h_photon_yield->Draw("");
   
   TPaveText *pt = new TPaveText(0.3792857,0.9374,0.6207143,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("photon_yield");
   pt->Draw();
   TLine *line = new TLine(5.987912,0,5.987912,310);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(6.009032,0,6.009032,310);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(6.030152,0,6.030152,310);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   line = new TLine(4.32,0,4.32,310);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->SetLineStyle(7);
   line->Draw();
   
   pt = new TPaveText(0.6,0.55,0.88,0.6,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   text = pt->AddText("P_{y} = 6.01^{-0.02}_{+0.02} #frac{photons}{keV}");
   pt->Draw();
   112e94a25d414801a0045c9ea26f7653->Modified();
   112e94a25d414801a0045c9ea26f7653->cd();
   112e94a25d414801a0045c9ea26f7653->SetSelected(112e94a25d414801a0045c9ea26f7653);
}
