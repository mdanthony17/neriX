{
	gStyle->SetLabelSize(.03, "XYZ");
	gStyle->SetTitleSize(.035, "XYZ");
	gStyle->SetTitleOffset(1.4, "Y");
	TGaxis::SetMaxDigits(3);

	TCanvas *can = new TCanvas("can", "", 800, 600);
	can->SetFillColor(10);
	TTree *mytree = new TTree();
//	mytree->ReadFile("filling3-20090715-lm.dat", "t/:c1/I:c2/I:c3/I:level/F");
	mytree->ReadFile("090918-root.dat", "t/I:level/F:rms/F");
	cout<<"mytree->GetEntries():" << mytree->GetEntries() << endl;

	Int_t  time,start, stop;
//	Double_t  time,start, stop;
	Float_t  level, rms;

	mytree->SetBranchAddress("level", &level);
	mytree->SetBranchAddress("t", &time);
	mytree->SetBranchAddress("rms", &rms);
	mytree->GetEntry(mytree->GetEntries()-1);
	stop = time;

	mytree->GetEntry(0);
	start = time;



	//Setup range for the histogram
	
//	gPad->SetLogy();

	Int_t nbin_time = 1000;
	Float_t level_max = 250;
	Float_t level_min = 120;
	Int_t nbin_level = 1000;

	Float_t unixtimeoffset = 3600.*5.;

	TH2F *bg = new TH2F("bg", "", nbin_time, start+unixtimeoffset, stop+unixtimeoffset, nbin_level ,level_min, level_max);
	bg->SetStats(0);
	bg->Draw();
	bg->SetTitle("Level Test, 090716");
	bg->GetXaxis()->SetTimeDisplay(1);
//	bg->GetXaxis()->SetTimeFormat("%m-%d-%H");
	bg->GetXaxis()->SetTimeFormat("%H:%M");
	bg->GetXaxis()->SetTitle("Time");
	bg->GetYaxis()->SetTitle("Level");
//	bg->GetYaxis()->SetTitle("rms");



	TGraphErrors *gr = new TGraphErrors();
	for (int i=0;i<mytree->GetEntries();i++)
		{
			mytree->GetEntry(i);
			gr->SetPoint(i,  time+unixtimeoffset , level);
			gr->SetPointError(0., rms);
			
			
		}     
	
	
	gr->Draw("p");
	gr->SetMarkerStyle(7);
	
/*	
	//l_th->DrawLine(valve_closing_time,0,valve_closing_time,1e-1);
	int SetLineColor = 3;
	l_th->SetLineColor(SetLineColor);
	l_th->DrawLine(time1,pressure_min,time1,pressure_max);
	l_th->DrawLine(time2,pressure_min,time2,pressure_max);
	l_th->DrawLine(time3,pressure_min,time3,pressure_max);
	l_th->DrawLine(time4,pressure_min,time4,pressure_max);
	l_th->DrawLine(time5,pressure_min,time5,pressure_max);
//	l_th->DrawLine(time6,pressure_min,time6,pressure_max);

	l_th->SetLineStyle(1);
	l_th->SetLineColor(2);
//	l_th->DrawLine(time5,pressure_min,time5,pressure_max);

	
	
	
	TLatex tex;
	tex.SetTextSize(0.03);

	int SetTextColor = SetLineColor;
	tex.SetTextColor(SetTextColor);
	Char_t soutgassing_rate21[100];
//	sprintf(soutgassing_rate21,"%1.2e mbar*l/(s*cm^2)",outgassing_rate21);
	sprintf(soutgassing_rate21,"(%1.2e)",outgassing_rate21);
	tex.DrawLatex((time1+time2)/2.,pressure_max/1000.,soutgassing_rate21);

//	SetTextColor++;
	tex.SetTextColor(SetTextColor);
	Char_t soutgassing_rate32[100];
//	sprintf(soutgassing_rate32,"%1.2e mbar*l/(s*cm^2)",outgassing_rate32);
	sprintf(soutgassing_rate32,"(%1.2e)",outgassing_rate32);
	tex.DrawLatex((time2+time3)/2.,pressure_max/1000.,soutgassing_rate32);

//	SetTextColor++;
	tex.SetTextColor(SetTextColor);
	Char_t soutgassing_rate43[100];
//	sprintf(soutgassing_rate43,"%1.2e mbar*l/(s*cm^2)",outgassing_rate43);
	sprintf(soutgassing_rate43,"(%1.2e)",outgassing_rate43);
	tex.DrawLatex((time3+time4)/2.,pressure_max/1000.,soutgassing_rate43);

	tex.SetTextColor(SetTextColor);
	Char_t soutgassing_rate54[100];
//	sprintf(soutgassing_rate43,"%1.2e mbar*l/(s*cm^2)",outgassing_rate43);
	sprintf(soutgassing_rate54,"(%1.2e)",outgassing_rate54);
	tex.DrawLatex((time5+time4)/2.,pressure_max/1000.,soutgassing_rate54);

/*	tex.SetTextColor(SetTextColor);
	Char_t soutgassing_rate65[100];
//	sprintf(soutgassing_rate43,"%1.2e mbar*l/(s*cm^2)",outgassing_rate43);
	sprintf(soutgassing_rate65,"(%1.2e)",outgassing_rate65);
	tex.DrawLatex((time6+time5)/2.,pressure_max/1000.,soutgassing_rate65);
*/

/*	SetTextColor++;
	tex.SetTextColor(SetTextColor);
	Char_t soutgassing_rate_fi[100];
	sprintf(soutgassing_rate_fi,"%1.2e mbarl/scm^{2}",outgassing_rate51);
	tex.DrawLatex((time1+time5)/2.,pressure_max/1000.,soutgassing_rate_fi);

	tex.DrawLatex((time1+time5)/2. ,0.04,"Outgassing Rate");

//	tex.SetTextColor(2);
//	tex.DrawLatex(time5 ,0.04,"Pump Stopping Time");
	
	
	//Leak Test results
	//tex.SetTextColor(2);
	//tex.DrawLatex((valve_closing_time+pump_stopping_time)/2,0.03,"1.52E-8 mbarl/scm^{2}");
	//tex.DrawLatex(pump_stopping_time+300,0.03,"2.94E-8 mbarl/scm^{2}");

*/
}
