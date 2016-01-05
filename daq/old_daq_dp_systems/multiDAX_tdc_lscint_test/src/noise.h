//-------------------------------------------------------------------
//  noise.h   --   DAX: DAQ for XENON100 
// 
//  the noise library contains functions to fo a noise analysis
//
//  07.07.2009 MS  (07.07.09)
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// perform a noise analysis
//   Board must be configured in non ZLE mode
//   internal trigger
int noise_analysis(crate& crt,	// crate
		digitizer& adc,	// ADC
		TApplication *theApp, 
		TCanvas *win);

