
//#define DISCRIM
//#define FIT
#include "nerix.hh"

int modPoisRnd ( double poisMean, double preFactor );
int modBinom ( int nTot, double prob, double preFactor );
int BinomFluct ( int N0, double prob );
double VonNeumann ( double xMin, double xMax, double yMin, double yMax );
double ZeplinBackground ( );

double GetSpikeCount(int Nph, double s1area); void LoadSpikeProb(); void LoadOverlapBiasCorrection();
double SpikeCorrection(int s1_spikes, double s1area);

using namespace std;
TRandom3 r; int i; double masterX[10000000], masterY[10000000], leakage[101], total[101], sigma[101], center[101], bandFitparams[4], s1cPhd[101];
long phArray, e_Array; double ke, ef; double recombProby;

double spike_prob[100][14][6];
double spike_corrections[80][4];

int main ( int argc, char** argv ) {
	
	if ( argc < 7 ) {
		cout << "This program takes 6 inputs. If you want to use defaults (for field means position dependence, for position random), use -1 as the value." << endl << endl;
		cout << "evt_TotNum type_particle E_min[keV] E_max[keV] field_drift[V/cm] evt_Zpos[mm]" << endl;
		cout << "Under LUX Run3 conditions, g1_gas=0.0946 -> SE = 24.66 phd/e- mean and fano_s2=1.4 -> 5.95 phd/e- width." << endl;
		return -1;
	} //int xxx = 0;
	
#ifndef pheNOTphd
	spEff *= ( 1. + P_dphe );
#endif
	
	unsigned long int numEvts = atoi(argv[1]);
	string pt = argv[2];
	double eMin = atof(argv[3]);
	double eMax = atof(argv[4]);
	double df = atof(argv[5]);
	double dz = atof(argv[6]); double driftTimeStyle;
	if ( dz == -1. ) driftTimeStyle = -1;
	else
	driftTimeStyle = (liquidBorder-dz)/eDriftSpeed;
	
	if ( pt == "-1" )pt = "NR";
	if ( eMin == -1. ) eMin = 0.;
	if ( eMax == -1. ) eMax = 100.;
	
	//for ( int ij = 0; ij < 100000; ij++ ) cout << ZeplinBackground() << endl;
	
#ifdef FIT
	FILE * ifp; double binSlope, binIncpt; int minFit, maxFit;
	double s1Bin[101], muJV[101], muErr[101], logSig[101], sigErr[101];
	if ( pt == "DD" || pt == "NR" ) {
#ifdef DISCRIM
		binSlope = 1.0; binIncpt = 0.5; minFit = 2; maxFit = 99;
#else
		binSlope = 1.0; binIncpt = 0.0; minFit = 2; maxFit = 40;
#endif
		ifp = fopen("ddnrBand_noSpike.txt", "r");
		for ( i = 0; i < 100; i++ )
		fscanf(ifp, "%lf %lf %lf %lf %lf", &s1Bin[i], &muJV[i], &muErr[i], &logSig[i], &sigErr[i]);
		if ( pt == "DD" && vuv1_1to1_2 == 0.915 ) {
			g1 = 0.115; ee = 0.489; //def 115, 48(4)
			g1_gas*= (23.77/24.66); //ff = 2.2; s2_rat = 0.432;
			dt_min = 80.; dt_max = 130.; e_life = 650.;
		}
	}
	else {
		binSlope = 1.0; binIncpt = 0.5; minFit = 2; maxFit = 99;
		ifp = fopen("tritiumBand_noSpike.Dec", "r");
		for ( i = 0; i < 100; i++ ) {
			fscanf(ifp, "%lf %lf %lf %lf %lf", &s1Bin[i], &muJV[i], &muErr[i], &logSig[i], &sigErr[i]);
			//muJV[i] *= 1.013;
		}
		if ( pt == "CH3T" && vuv1_1to1_2 == 0.915 ) {
			g1_gas *=(23.77/24.66); //ff = 2.0; s2_rat = 0.436;
			g1 = 0.111; ee = 0.501; //def 115, 509 from spectral fit
		}
	}
	fclose(ifp);
#endif
	
	double W = (21.717-2.7759*Xe_rho)*1e-3; int Z = 54; double molarMass = 131.293;
	
	double Fex =1.;
	double Fi = 1.;
	double Fr = 1.;
	
	double kappa = 0.1735; double L,epsilon,LindkFactor;
TRY_AGAIN:
	for ( unsigned long int j = 0; j < numEvts; j++ ) {
		
		ke=(eMin+(eMax-eMin)*r.Rndm())/1.; //if(pt=="NR")ke=pow(r.Exp(1.1562565039),1.8672392867); if(pt=="ER")ke=ZeplinBackground();
		if ( pt == "CH3T" ) {
			double QQ = 18.5898;
			double m_e = 511.;
			double pi = 3.14159265358979312;
			double aa = 1./137.;
			double ZZ = 2.;
			double xmax = QQ;
			double ymax = 1.1e7;
			double xTry = xmax*r.Rndm();
			double yTry = ymax*r.Rndm();
			double B = sqrt(xTry*xTry + 2*xTry*m_e) / (xTry + m_e);
			double x = (2*pi*ZZ*aa)*(xTry + m_e)/sqrt(xTry*xTry + 2*xTry*m_e);
			while( yTry > (sqrt(2*xTry*m_e) *
						   (xTry + m_e) *
						   (QQ-xTry) * (QQ-xTry) *
						   x*(1./(1-exp(-x)))*(1.002037-0.001427*(B))) ){
				xTry = xmax*r.Rndm();
				yTry = ymax*r.Rndm();
				B = sqrt(xTry*xTry + 2*xTry*m_e) / (xTry + m_e);
				x = (2*pi*ZZ*aa)*(xTry + m_e)/sqrt(xTry*xTry + 2*xTry*m_e);
			}
			ke = xTry;
		}
		if ( pt == "DD" )
		ke = VonNeumann(eMin,eMax,0.,5.5);
		//cout << ke << endl;
		if ( pt == "NR" || pt == "DD" ) {
			
			epsilon = 11.5*ke*pow(double(Z),(-7./3.));
			double lambda = 3.*pow(epsilon,0.15)+0.7*pow(epsilon,0.6)+epsilon;
			double isotope = r.Rndm()*100.; int A;
			if ( isotope > 0.000 && isotope <= 0.090 )
			A = 124;
			else if ( isotope > 0.090 && isotope <= 0.180 )
			A = 126;
			else if ( isotope > 0.180 && isotope <= 2.100 )
			A = 128;
			else if ( isotope > 2.100 && isotope <= 28.54 )
			A = 129;
			else if ( isotope > 28.54 && isotope <= 32.62 )
			A = 130;
			else if ( isotope > 32.62 && isotope <= 53.80 )
			A = 131;
			else if ( isotope > 53.80 && isotope <= 80.69 )
			A = 132;
			else if ( isotope > 80.69 && isotope <= 91.13 )
			A = 134;
			else
			A = 136;
			LindkFactor = kappa*sqrt((double)A/molarMass);
			L = (LindkFactor*lambda)/(1.+LindkFactor*lambda);// - 1e-9 / epsilon;
			/*epsilon = 1.05e-3*ke; double eps_z = 1.068*epsilon;
			 double sn = log(1.+1.1383*eps_z)/(2.*(eps_z+0.01321*pow(eps_z,0.21226)+0.19593*pow(eps_z,0.5)));
			 double se = 0.166*sqrt(epsilon);
			 L = 2.44*(1.-exp(-ke/0.423))*(se/(se+sn));*/
			if(L>1.)L=1.;if(L<0.)L=0.;
			
		}
		
		double dt;
		if ( driftTimeStyle == -1 ) dt = dt_min+(dt_max-dt_min)*r.Rndm();
		else dt = driftTimeStyle;
		if ( df < 0. )
		ef=efpolA+efpolB*dt+efpolC*pow(dt,2.)+efpolD*pow(dt,3.)+efpolE*pow(dt,4.)+efpolF*pow(dt,5.);
		else
		ef = df; //no fringing
		//if(ef < 1.5) ef = 1.5;
		//ef=(3.9730-0.0096627*dt+6.2156e-6*pow(dt,2.))*df; //if(j<10) cout << ef << "\t" << df << endl;
		
		double NexONi = 0.482;//0.60*(1.-exp(-ke/1.41)); if(NexONi<0.)NexONi=0.;
		if ( pt == "ER" || pt == "CH3T" ) { NexONi=(0.059709+0.048577*Xe_rho)*(1.-exp(-1.5*(ke-0.19))); if(ke<-1.3)NexONi=0.06; if(NexONi<0.)NexONi=0.; }
		double alf = 1./(1.+NexONi); double alf_special = 1.059709+0.048577*Xe_rho;
		double TIB = 0.10077*pow(ef,-0.078314);//0.072704*pow(ef,-0.065569);//0.070242*pow(ef,-0.066973);
		if ( pt == "ER" || pt == "CH3T" ) { L = 1.;
			double TIB_lo = 0.045*pow(ef,-0.13)*(1.-exp(-0.035*pow(ke,4.)-0.13));
			double m1 = 0.0024389+((0.50042-0.0024389)/(1.+pow(ef/11.747,0.64723)));
			double m2 = 2.6927000+((160.500-2.6927000)/(1.+pow(ef/28.518,0.95537)));
			double m3 = 398.08000+((23223.0-398.08000)/(1.+pow(ef/35.008,1.06550)));
			double m4 = 54083.000+((3.2519e6-54083.00)/(1.+pow(ef/37.286,1.11110)));
			double m5 = 2.35600e6+((1.3762e8-2.3560e6)/(1.+pow(ef/36.118,1.11000)));
			double TIB_hi = m1/ke + m2/pow(ke,2.) - m3/pow(ke,3.) + m4/pow(ke,4.) - m5/pow(ke,5.);
			double TIB100 = m1/100. + m2/pow(100.,2.) - m3/pow(100.,3.) + m4/pow(100.,4.) - m5/pow(100.,5.);
			if ( ke <= 18.6 && ke>=1.3) {
				double eCurv = 45.836*pow(ef,-0.203);
				TIB = TIB100+((TIB_lo*1.1-TIB100)/(1.+pow(ke/eCurv,2.0)));
			}
			else if ( ke < 122. && ke > 18.6 ) {
				double x = log10(ef);
				double pol0 = 57.92384-8.097075*x+12.31535*pow(x,2.)-15.4544*pow(x,3.)+8.911858*pow(x,4.)-2.675756*pow(x,5.)+0.3966776*pow(x,6.)-0.02281601*pow(x,7.);
				double pol1 = 0.1338665-0.413536*x+1.836502*pow(x,2.)-3.052137*pow(x,3.)+2.780416*pow(x,4.)-1.511113*pow(x,5.)+0.4935669*pow(x,6.)-0.09448295*pow(x,7.)+0.009755602*pow(x,8.)-0.000419436*pow(x,9.);
				double pol2=-0.0007282148+0.003062095*x-0.01255166*pow(x,2.)+0.02011632*pow(x,3.)-0.0177259*pow(x,4.)+0.009324331*pow(x,5.)-0.00296341*pow(x,6.)+0.0005555244*pow(x,7.)-5.647981e-5*pow(x,8.)+2.401166e-6*pow(x,9.);
				
				double photonYield = pol0 + pol1 * ke + pol2 * pow(ke,2.);
				double tempNumIons = BinomFluct(int(floor(ke/W+0.5)),alf);
				double tempNumExcs = int(floor(ke/W+0.5)) - tempNumIons;
				recombProby = ( photonYield*ke - tempNumExcs ) / tempNumIons;
			}
			else if ( ke < 1.3 ) {
				double x = log10(ef);
				double pol0 = 77.111 + ( 76.329 - 77.111 ) / ( 1. + pow((x/3.3739),2.6411) );
				double pol1 = 21.698 + ( 14.215 - 21.698 ) / ( 1. + pow((x/4.7893),1.9549) );
				double pol2 = 15.821 + (-.55771 - 15.821 ) / ( 1. + pow((x/10.739),1.1941) );
				double electronYield = pol0 - pol1 * ke + pol2 * pow(ke,2.);
				if ( electronYield > 73. ) electronYield = 73.; alf = 1./1.019;
				double tempNumQuanta = int(floor(ke/W+0.5)); double tempNumIons = tempNumQuanta*alf;
				recombProby = 1. - ( ( electronYield * ke ) / tempNumIons );
			}
			else
			TIB = TIB_hi;
		}
		TIB *= pow((Xe_rho/2.888),0.3); //density dependence
		
		int Nex, Ni, Nq;
		if ( pt == "NR" || pt == "DD" ) {
			//if(ke<5.){Fex=0.001;Fi=0.001;} else {Fex=1.;Fi=1.;}
			Ni = modPoisRnd((ke/W)*L*alf,Fi);
			Nex= modPoisRnd((ke/W)*L*NexONi*alf,Fex);
			Nq = Nex + Ni;
			//if(Nq>int(floor(ke/W+0.5)))
			//Nq=int(floor(ke/W+0.5));
		}
		else {
			Nq = int(floor(ke/W+0.5));//BinomFluct(r.Poisson(ke/(L*W)),L);
			if ( Nq < 0 ) Nq = 0;
			Ni = BinomFluct(Nq,alf); Nex=Nq-Ni; if(Nex<0)Nex=0;
		}
		
		double R = 1.-log(1.+(double(Ni)/4.)*TIB)/((double(Ni)/4.)*TIB);
		if ( pt == "ER" || pt == "CH3T" ) {
			//if(ke<1.0)R=0.;
			if((ke < 122. && ke > 18.6) || ke < 1.3)R=recombProby;
		}
		if(R>1.)R=1.;if(R<0.)R=0.;if(Ni<=0.)R=0.;
		
		double Ni_eff = double(Ni); if( Ni_eff > 3e2 ) Ni_eff=3e2;
		Fr = 0.0075*Ni_eff; if ( Fr < 0. ) Fr = 0.; int Ne; //if(ke<2.)Fr=0.;
	    if ( pt == "ER" || pt == "CH3T" ) {
			Ni_eff = double(Ni);
			if ( Ni_eff > 3e4 ) Ni_eff = 3e4;
			Fr = 0.0075*Ni_eff; if(ke<0.)Fr=0.; if(ke>(12259.*pow(ef,-1.218)) && ke<100.)Fr=sqrt(3.)*0.0075*Ni_eff;
			if ( Fr <= 0. )
			Ne = int(floor(double(Ni)*(1.-R)+0.5));
			else {
				double man = 0.85607 + 0.032441 * log10(ef);
				double pwr = .023162 - .0053890 * log10(ef);
				double kludge = man*pow(ke,pwr);
				if ( kludge < 1.0 )
					kludge = 1.0;
				Ne = modPoisRnd(kludge*double(Ni)*(1.-R),Fr); //Ne=BinomFluct(Ni,(1.-R));
			}//correction factor discovered by Bhawna
		 }
		else
		Ne = modPoisRnd(double(Ni)*(1.-R),Fr);
		if ( Ne > Ni ) Ne = Ni;
		int Nph = Nq - Ne;
		if(Ne<0)Ne=0; if(Nph<0)Nph=0; if(Nph>Nq)Nph=Nq; if(Nph<Nex)Nph=Nex;
		if(!R || Fr<=-1.) {Nph=Nex; Ne=Ni;}
		if ( pt == "NR" || pt == "DD" ) {
			int Nph_qb = BinomFluct(Nph,1./(1.+13.2*0.166*pow(epsilon,0.5)));
			Ne += int(floor(0.111*(double(Nph)-double(Nph_qb))+0.5));
			Nph = Nph_qb;
		}
		//if ( 1 ) {
		//Ne  = modPoisRnd(-17.204+22.464*pow(ke,0.5774),Fr);
		//Nph = modPoisRnd(3.2645*pow(ke,1.3795),Fr);
		//}
		
		double dz = liquidBorder - eDriftSpeed * dt;
		double dz_center = liquidBorder - eDriftSpeed * dtCntr;
		double posDep =s1polA+s1polB*dz+s1polC*pow(dz,2.)+s1polD*pow(dz,3.)+s1polE*pow(dz,4.);
		posDep/=s1polA+s1polB*dz_center+s1polC*pow(dz_center,2.)+s1polD*pow(dz_center,3.)+s1polE*pow(dz_center,4.);
		
		int Npe = BinomFluct(Nph,g1*posDep); double S1;
		if ( nFold ) {
			S1 = 0.; int nHits = 0; double stRatio, tau;
			if ( pt == "DD" || pt == "NR" ) stRatio = 1.6;
			else stRatio = 0.6;
			for ( i = 0; i < Npe; i++ ) {
				double phe1 = r.Gaus(1.,sp);
				if ( phe1 < 0.0 || r.Rndm() > spEff_direct ) phe1 = 0.;
				double phe2 = 0.;
				if ( r.Rndm() < P_dphe ) {
					phe2 = r.Gaus(1.,sp);
					if ( phe2 < 0.0 || r.Rndm() > spEff_direct ) phe2 = 0.;
				}
				if ( r.Rndm() < (stRatio/(1.+stRatio)) ) tau = 3.1;
				else tau = 24.;
				if ( (phe1+phe2)*vuv1_1to1_2 > spEff && r.Exp(tau) < coinWind ) { // spEff has units of phd, but before correcting from VUV v1.1 to v1.2
					S1 += (phe1+phe2); nHits++;
				}
			}
#ifndef pheNOTphd
			S1 /= ( 1. + P_dphe ); //S1 = GetSpikeCount ( nHits, S1 );
#endif
			if ( nHits >= nFold && r.Rndm() < 1.-pow((double)numberPMTs,(1.-(double)nHits)) ) ;
			else S1 = 0.;
		}
		else {
			Npe += BinomFluct(Npe,P_dphe); S1 = r.Gaus(Npe,sp*sqrt(Npe))/(1.+P_dphe);
			if ( r.Rndm() > 0.5*erf((S1/posDep-erf_mu)/(erf_sg*sqrt(2.)))+0.5 )
			S1 = 0.;
		}
		double S1c = S1 / posDep;
		
		if ( ee == -1. ) {
			af /= 1e3; //kV per cm conversion
			double LiqExtField = af / (1.96/1.00126);
			double ExtractEffA = -0.0012052 + //Aprile 2004 IEEE No. 5
			0.1638*af-0.0063782*pow(af,2.);
			double ExtractEffB = 1. - 1.3558*exp(-0.074312* //Gushchin
												 pow(LiqExtField,2.4259));
			ee = 1. - 1.2958*exp(-0.071543*pow(LiqExtField,2.2469)); //PIXeY, LIDINE
			//if ( af >= 12. ) ee = 1.00;
			if ( af <= 0.0 || LiqExtField < 0. ) ee=0.00;
			if ( ee < 0 ) ee = 0;
			if ( ee > 1 ) ee = 1;
			af *= 1e3;
		}
		
		double gasDensity = ( gasRho / molarMass ) * AVO;
		double nphPerElec = (elGain*(af/gasDensity)*1e17-elThld)*gasDensity*1e-17*gasGap; //if(j<10) cout << nphPerElec << endl;
		int Nee = BinomFluct(Ne,ee*exp(-dt/e_life)); //Nee=1;
		int ElecLumYld = int(floor(r.Gaus(nphPerElec*Nee,ff*sqrt(nphPerElec*Nee))+0.5)); //g2=nphPerElec*g1_gas*ee
		Npe = BinomFluct(ElecLumYld,g1_gas); Npe += BinomFluct(Npe,P_dphe);
		double S2 = r.Gaus(Npe,sp*sqrt(Npe))*vuv1_1to1_2; // the "raw" S2 is before correcting from VUV gains v1.1 to v1.2
#ifndef pheNOTphd
		S2 /= ( 1. + P_dphe );
#endif
		double S2v = S2/vuv1_1to1_2;
		double S2b= r.Gaus(s2_rat*S2v,sqrt(s2_rat*S2v*(1.-s2_rat)));
		double S2bc = S2b / exp(-dt/e_life);
		double S2c = S2v/exp(-dt/e_life);
		
#ifndef FIT
		if ( !j ) fprintf(stdout,"E [keV]\t\tNph\tNe-\tS1 [phd]\tS1c [phd]\tS2 [phd]\tS2c [phd]\n");
		//if ( S1c > 0. && S1c <= 50. && S2 > s2_thr ) xxx++;
		fprintf(stdout,"%f\t%d\t%i\t%f\t%f\t%f\t%f\n",ke,Nph,Ne,S1,S1c,S2v,S2c);
		phArray += Nph; e_Array += Ne;
#else
		if(S1c>0 && S2>s2_thr)
		{masterX[j]=S1c;masterY[j]=log10(S2c/S1c);}
		else {masterX[j]=0.;masterY[j]=-1.;}
#endif
	}
	
#ifdef FIT
	TH1F *HistogramArray = new TH1F[101];
	for(int hists=0; hists<101; hists++) {
		TString HistName; HistName.Form("%i",hists);
		HistogramArray[hists].SetName(HistName.Data());
		HistogramArray[hists].SetBins(80,0.6,6.);
	}
	
	for(unsigned long count=0; count<numEvts; count++) {
		std::ifstream infile("discrim.txt"); double NRbandCentroid[101]; i = 0;
		while ( infile >> s1cPhd[i] >> NRbandCentroid[i] ) { i++; }
		infile.close(); remove("discrim.txt");
		for ( i = 0; i < 101; i++ ) {
			if(masterX[count]>(binSlope*double(i)+binIncpt-binSlope/2.) && masterX[count]<=(binSlope*double(i)+binIncpt+binSlope/2.)) {
				HistogramArray[i].Fill(masterY[count]);
				if(masterX[count] > s1_min && masterX[count] < s1_max && masterY[count] ) {
					total[i]++; //double NRbandSlope[2], NRbandCentroid, x1;
					/*x1 = double(i)+0.5;
					 NRbandSlope[0] = nrTest[i-1] - nrTest[i-2];
					 NRbandSlope[1] = nrTest[i-0] - nrTest[i-1];
					 if ( masterX[count] < x1 )
					 NRbandCentroid = NRbandSlope[0]*(masterX[count]-x1)+nrTest[i-1];
					 else
					 NRbandCentroid = NRbandSlope[1]*(masterX[count]-x1)+nrTest[i-1];*/
					if(masterY[count] < NRbandCentroid[i]) leakage[i]++;
				}
			}
		}
	} double SumSqDiff = 0.0;
	cout << "S1c\tband_mu\tband_sigma" << endl;
	for(int hists=0; hists<101; hists++) {
		HistogramArray[hists].Draw();
		//Int_t max_bin = HistogramArray[hists].GetMaximumBin();
		//Double_t max = HistogramArray[hists].GetBinCenter(max_bin)+0.2;
		//Double_t edge =HistogramArray[hists].GetBinCenter(max_bin)-0.2;
		TF1 *f = new TF1("band","gaus");
		HistogramArray[hists].Fit(f,"Q");
		sigma[hists] = f->GetParameter("Sigma");
		center[hists]= f->GetParameter("Mean");
		cout << binSlope*double(hists)+binIncpt << "\t" << center[hists] << "\t" << sigma[hists] << endl;
#ifdef DISCRIM
		if ( pt == "DD" || pt == "NR" ) {
			std::ofstream ofs;
			ofs.open ("discrim.txt", std::ofstream::out | std::ofstream::app);
			ofs << binSlope*double(hists)+binIncpt << "\t" << center[hists] << endl;
			ofs.close(); //save for ER usage later
		}
#endif
		if ( hists >= minFit && hists <= maxFit ) {
			if ( pt == "CH3T" || pt == "ER" ) {
				SumSqDiff += pow((center[hists]-muJV[hists])/muErr[hists],2.);
			}
			else {
				SumSqDiff += pow((center[hists]-muJV[hists])/muErr[hists],2.);
			}
		}
		delete f; s1cPhd[hists]=binSlope*(double)hists+binIncpt;
	}
	cout << "chi^2/dof = " << "\t" << SumSqDiff / ((maxFit-minFit+1.)-0.-2.-1.) << endl;
	
	TGraph *gr1 = new TGraph(101, s1cPhd, center);
	TF1 *fitf = new TF1("edep","[0]/(x+[1])+[2]*x+[3]",s1_min,s1_max);
	gr1->Fit(fitf,"rq","",s1_min,s1_max);
	bandFitparams[0] = fitf->GetParameter(0); bandFitparams[1] = fitf->GetParameter(1); bandFitparams[2] = fitf->GetParameter(2); bandFitparams[3] = fitf->GetParameter(3);
	fprintf(stderr,"Band Fit Parameters: %f %f %f %f\n",bandFitparams[0],bandFitparams[1],bandFitparams[2],bandFitparams[3]);
	delete gr1; delete fitf; //clear the memory
	
#ifdef DISCRIM
	double sum1 = 0.0, sum2 = 0.0; i = 0;
	while ( s1cPhd[i] < s1_max ) {
		if(s1cPhd[i] > s1_min) sum1 += leakage[i]; sum2 += total[i]; cerr << s1cPhd[i] << "\t" << leakage[i] << "\t" << total[i] << "\t" << leakage[i]/total[i] << endl; i++;
	}
	cerr << "DISCRIM or ACCEPTN = " << (1. - sum1 / sum2)*100. << "%" << endl;
#endif
	
	//ee += 0.001; goto TRY_AGAIN;
#else
	//cout << xxx << endl;
	//cout << pt << "\t" << ef << "\t" << ke << "\t" << (double(phArray)/1000000.)/ke << "\t" << (double(e_Array)/1000000.)/ke << endl;
	return 1;
#endif
	
}

int modPoisRnd(double poisMean, double preFactor) {
	
	int randomNumber;
	
	if ( preFactor >= 1. ) {
		poisMean = r.Gaus(poisMean,sqrt((preFactor-1.)*poisMean));
		if ( poisMean < 0. ) poisMean = 0.;
		randomNumber = r.Poisson(poisMean);
	}
	else
	randomNumber =
	int(floor(r.PoissonD(poisMean/preFactor)*preFactor+r.Rndm()));
	
	if ( randomNumber < 0 ) randomNumber = 0; //jic
	
	return randomNumber;
}

int modBinom(int nTot, double prob, double preFactor) {
	
	int randomNumber, nTotp;
	
	if ( preFactor >= 1. ) {
		prob = r.Gaus(prob,sqrt((preFactor-1.)*prob*nTot)/double(nTot));
		if ( prob < 0. ) prob = 0.;
		if ( prob > 1. ) prob = 1.;
		randomNumber = r.Binomial(nTot,prob);
	}
	else {
		nTotp = int(floor(double(nTot)/preFactor+r.Rndm()));
		randomNumber = int(floor((double(nTot)/double(nTotp))*
								 double(r.Binomial(nTotp,prob))+r.Rndm()));
	}
	
	if ( randomNumber < 0 ) randomNumber = 0; //just in case
	
	return randomNumber;
}

int BinomFluct ( int N0, double prob ) {
	
	double mean = N0*prob;
	double sigma = sqrt(N0*prob*(1-prob));
	int N1 = 0;
	
	if ( prob <= 0.00 ) return N1; if ( prob >= 1.00 ) return N0;
	
	if ( N0 < 10 ) {
		for(int i = 0; i < N0; i++) {
			if(r.Uniform() < prob) N1++;
		}
	}
	else {
		N1 = int(floor(r.Gaus(mean,sigma)+0.5));
	}
	
	if ( N1 > N0 ) N1 = N0; if ( N1 < 0 ) N1 = 0;
	
	return N1;
}

double VonNeumann ( double xMin, double xMax, double yMin, double yMax ) {
	
	double xTry = xMin+(xMax-xMin)*r.Rndm();
	double yTry = yMin+(yMax-yMin)*r.Rndm();
	
	double FuncValue;
	if ( xTry < 9.8 )
	FuncValue = 4.1411-3.1592*log10(xTry);
	else
	FuncValue = 2.4348-0.19783*xTry+0.0060931*pow(xTry,2.)-8.2243e-5*pow(xTry,3.)+4.2505e-7*pow(xTry,4.);
	
	while ( yTry > FuncValue ) {
		xTry = xMin+(xMax-xMin)*r.Rndm();
		yTry = yMin+(yMax-yMin)*r.Rndm();
		if ( xTry < 9.8 )
		FuncValue = 4.1411-3.1592*log10(xTry);
		else
		FuncValue = 2.4348-0.19783*xTry+0.0060931*pow(xTry,2.)-8.2243e-5*pow(xTry,3.)+4.2505e-7*pow(xTry,4.);
	}
	
	return xTry; //selection under curve made
}

double GetSpikeCount ( int Nph, double s1area ) {
	
	// divide into top and bottom and do spike counting here
	int Nph_top = BinomFluct (Nph, .4);
	int Nph_bot = Nph - Nph_top;
	int spikes_top = 0;
	int spikes_bot = 0;
	
	int Nph_top_PMT[61];
	int Nph_bot_PMT[61];
	
	for (int i = 0; i < 61; i++) {
		Nph_top_PMT[i] = 0;
		Nph_bot_PMT[i] = 0;
	}
	
	int rand_index = 0;
	// distribute phtons to PMTs
	for (int i = 0; i < Nph_top; i++) {
		rand_index = r.Integer(61);
		Nph_top_PMT[rand_index]++;
	}
	for (int i = 0; i < Nph_bot; i++) {
		rand_index = r.Integer(61);
		Nph_bot_PMT[rand_index]++;
	}
	
	int spikes_top_PMT[61];
	int spikes_bot_PMT[61];
	
	for(int i = 0; i < 61; i++) {
		spikes_top_PMT[i] = 0;
		spikes_bot_PMT[i] = 0;
	}
	
	double radnom_num = 0;
	
	// model doesn't handle Nph_top or Nph_bot > 100
	if (Nph_top > 100) {
		Nph_top = 100;
	}
	if (Nph_bot > 100) {
		Nph_bot = 100;
	}
	
	int s1_spikes = 0;
	
	for (int i = 0; i < 61; i++) {
		if (Nph_top_PMT[i] > 14) {
			Nph_top_PMT[i] == 14;
		}
		if (Nph_bot_PMT[i] > 14) {
			Nph_bot_PMT[i] == 14;
		}
		
		double random_num = r.Uniform();
		if (Nph_top_PMT[i] == 0) {
			spikes_top_PMT[i] = 0;
		}
		for(int j = 0; j < Nph_top_PMT[i]; j++) {
			if (random_num <= spike_prob[Nph_top][Nph_top_PMT[i]][j] - .0000001) {
				spikes_top_PMT[i] = j + 1;
				break;
			}
			else {
				radnom_num = radnom_num - spike_prob[Nph_top][Nph_top_PMT[i]][j];
			}
			if(j == Nph_top_PMT[i] - 1) {
				spikes_top_PMT[i] = j + 1;
			}
		}
		
		radnom_num = r.Uniform();
		if (Nph_bot_PMT[i] == 0) {
			spikes_bot_PMT[i] = 0;
		}
		for(int j = 0; j < Nph_bot_PMT[i]; j++) {
			if (random_num <= spike_prob[Nph_bot][Nph_bot_PMT[i]][j] - .0000001) {
				spikes_bot_PMT[i] = j + 1;
				break;
			}
			else {
				radnom_num = radnom_num - spike_prob[Nph_bot][Nph_bot_PMT[i]][j];
			}
			if(j == Nph_bot_PMT[i] - 1) {
				spikes_bot_PMT[i] = j + 1;
			}
		}
		s1_spikes = s1_spikes + spikes_top_PMT[i] + spikes_bot_PMT[i];
	}
	
	double s1_spikes_overlap = s1_spikes * SpikeCorrection(s1_spikes, s1area);
	
	return s1_spikes_overlap;
	
}

void LoadSpikeProb() {
	
	std::ifstream spike_prob_file;
	spike_prob_file.open("spike_count_efficiency_model_v10.txt");
	double dump;
	
	for(int i = 0; i < 100; i ++) {
		for(int j = 0; j < 14; j++) {
			if(i >= 59 || j < 10) {
				spike_prob_file >> dump;
			}
			for (int k = 0; k < 6; k++) {
				if(i < 59 && j > 9) {
					spike_prob[i][j][k] = 0.;
				}
				else {
					spike_prob_file >> spike_prob[i][j][k];
				}
			}
		}
	}
	
}

void LoadOverlapBiasCorrection() {
	
	std::ifstream spike_corrections_file;
	spike_corrections_file.open("spike_count_correction_v10.dat");
	double dump;
	
	for(int i = 0; i < 80; i ++) {
		spike_corrections_file >> dump;
		for(int j = 0; j < 4; j++) {
			spike_corrections_file >> spike_corrections[i][j];
		}
	}
	
}

double SpikeCorrection(int s1_spikes, double s1area) {
	
	double par[4];
	int corr_min_spike = 1;
	int corr_max_spike = 80;
	
	if (s1_spikes < corr_min_spike) {
		par[0] = spike_corrections[0][0];
		par[1] = spike_corrections[0][1];
		par[2] = spike_corrections[0][2];
		par[3] = spike_corrections[0][3];
	}
	if (s1_spikes >= corr_min_spike && s1_spikes <= corr_max_spike) {
		par[0] = spike_corrections[s1_spikes-1][0];
		par[1] = spike_corrections[s1_spikes-1][1];
		par[2] = spike_corrections[s1_spikes-1][2];
		par[3] = spike_corrections[s1_spikes-1][3];
	}
	if (s1_spikes > corr_max_spike) {
		par[0] = 0;
		par[1] = 0;
		par[2] = 0;
		par[3] = 0;
	}
	Double_t f = par[0] + par[1] * s1area + par[2] * s1area * s1area + par[3] * s1area * s1area * s1area;
	return f;
	
}

double ZeplinBackground ( ) {
	
	double selector = r.Uniform();
	double selEnerg;
	
	if ( selector > 0.000000 && selector <= 0.038602 )
	selEnerg = 1.0482 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.038602 && selector <= 0.081630 )
	selEnerg = 1.1494 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.081630 && selector <= 0.085197 )
	selEnerg = 1.2603 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.085197 && selector <= 0.098211 )
	selEnerg = 1.3820 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.098211 && selector <= 0.116010 )
	selEnerg = 1.5153 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.116010 && selector <= 0.134960 )
	selEnerg = 1.6616 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.134960 && selector <= 0.181840 )
	selEnerg = 1.8219 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.181840 && selector <= 0.215600 )
	selEnerg = 1.9977 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.215600 && selector <= 0.250500 )
	selEnerg = 2.1905 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.250500 && selector <= 0.280450 )
	selEnerg = 2.4019 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.280450 && selector <= 0.307760 )
	selEnerg = 2.6337 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.307760 && selector <= 0.335780 )
	selEnerg = 2.8879 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.335780 && selector <= 0.362760 )
	selEnerg = 3.1665 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.362760 && selector <= 0.404200 )
	selEnerg = 3.4721 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.404200 && selector <= 0.437260 )
	selEnerg = 3.8072 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.437260 && selector <= 0.459880 )
	selEnerg = 4.1746 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.459880 && selector <= 0.493280 )
	selEnerg = 4.5775 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.493280 && selector <= 0.527320 )
	selEnerg = 5.0192 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.527320 && selector <= 0.548560 )
	selEnerg = 5.5036 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.548560 && selector <= 0.577610 )
	selEnerg = 6.0347 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.577610 && selector <= 0.609550 )
	selEnerg = 6.6171 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.609550 && selector <= 0.635570 )
	selEnerg = 7.2556 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.635570 && selector <= 0.656480 )
	selEnerg = 7.9558 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.656480 && selector <= 0.689470 )
	selEnerg = 8.7236 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.689470 && selector <= 0.720960 )
	selEnerg = 9.5654 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.720960 && selector <= 0.749250 )
	selEnerg = 10.489 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.749250 && selector <= 0.779750 )
	selEnerg = 11.501 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.779750 && selector <= 0.814330 )
	selEnerg = 12.611 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.814330 && selector <= 0.842290 )
	selEnerg = 13.828 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.842290 && selector <= 0.878470 )
	selEnerg = 15.162 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.878470 && selector <= 0.908490 )
	selEnerg = 16.625 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.908490 && selector <= 0.939570 )
	selEnerg = 18.230 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.939570 && selector <= 0.971280 )
	selEnerg = 19.989 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else if ( selector > 0.971280 && selector < 1.0000000 )
	selEnerg = 21.918 * ( 1. + 0.08801/2. * (2.*r.Rndm()-1.) );
	else
	selEnerg = r.Rndm()*20.;
	
	return selEnerg; //selection under curve made
	
}
