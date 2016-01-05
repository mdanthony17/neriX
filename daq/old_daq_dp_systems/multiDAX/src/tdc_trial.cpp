extern "C" {
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/stat.h>
#include <unistd.h>
}

#include "global.h"
#include "CAENVMElib.h"
#include "control.h"

#include <TH1D.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TFile.h>
#include <TLatex.h>

using namespace std;

typedef struct
{
  int NumberOfCrates;		// total Number of Crates
  int handle[1];	// device handle
} crate_simple;

typedef struct
{
  int NumberOfTDCs;		// total number of ADCs
  u_int32_t Address; 	// Base Addresses
  int Id;		// Board Identifier
  int Crate;		// Crate in which Board sits
  int BltSize;			// Size of a Block Transfer
  int BoardInfo;		// Show only Board Infos?
  u_int32_t ExpectedEvSize; 	// ...
  int BufferSize;		// max. buffer to store data of one module
  int NbSamplesEvt;		// max. lenght of waveform
  int NbMemoryBlocks;	// max. number of events in output buffer
  int ZLE;			// zero length encoding activated? (1/0)
  int baseline;			// position of baselines
  int trig;
  int busy;			// activate front panel to read busy status (1/0)?
  int NbChannels;		// how many channels are used
  // properties of the ADC
  float voltage_range;		// in Volts
  int sample_size;		// in bits
  int sampling_freq;		// in Hz
} timedc;



//performs vme write
int tdc_writereg(crate& crt, 		// crate
		timedc& tdc,		// adc
		u_int32_t addr,		// the register to write to
		u_int32_t data)		// the value to set
{
  char msg[100];
  int ret;

  //cvA32_U_DATA = A32 non-privileged data access
  //cvD32 = 32 bit data width
  //success of CAEN functions returns 0 (cvSuccess)
  //these are located in CAENVMEtypes.h
  if ((ret=CAENVME_WriteCycle(crt.handle[tdc.Crate], tdc.Address+addr,
			&data, cvA32_U_DATA, cvD32)) != cvSuccess) {
	sprintf(msg,":::: ERROR: VME Write failure at address %08X (code %d) ::::", (tdc.Address + addr),ret);
	cout << msg << endl;
	return -1;
  }  

  return 0;
}

//performs vme read for registers (except output buffer)
int tdc_readreg(crate& crt, 		// crate
		timedc& tdc,		// adc
		u_int32_t addr,		// the register to write to
		u_int32_t& data)	// the value to read
{
   u_int32_t temp;
   char msg[100];
   int ret;
   
   //uses 32 bit read but registers are 16 bits - check if issue
   //should be okay because registers are 32 bits apart (difference
   //of 2 in hexadecimal register addresses)
   //manual says read mode is d16 so changing it
   if ((ret=CAENVME_ReadCycle(crt.handle[tdc.Crate], tdc.Address+addr,
      		&temp, cvA32_U_DATA, cvD16)) != cvSuccess) {
     sprintf(msg,":::: ERROR: VME Read failure at address %08X (code %d) ::::", (tdc.Address + addr),ret);
     errormsg(0,msg);
     return -1;		
   }		
   
   data=temp;
   return 0;
}

//read output buffer (not other registers though)
int tdc_readoutput(crate& crt, 		// crate
		timedc& tdc,		// adc
		u_int32_t addr,		// the register to write to
		u_int32_t& data)	// the value to read
{
   u_int32_t temp;
   char msg[100];
   int ret;
   
   if ((ret=CAENVME_ReadCycle(crt.handle[tdc.Crate], tdc.Address+addr,
      		&temp, cvA32_U_DATA, cvD32)) != cvSuccess) {
     sprintf(msg,":::: ERROR: VME Read failure at address %08X (code %d) ::::", (tdc.Address + addr),ret);
     errormsg(0,msg);
     return -1;		
   }		
   
   data=temp;
   return 0;
}

//define useful register addresses
#define BitSet2 0x1032 
#define FullScaleRange 0x1060
#define OutputBuffer 0x0000÷0x0FFC


//crate variable
crate_simple crt;
crt.NumberOfCrates = 0;

// -------- still need to set tdc.Address
tdc.Address = 0x00A00000; //use the four rotary wheels on module

//taken from multiDAXconfig.ini
//initialize VME
CVBoardTypes BType = V2718;
short linker = 0;
short bdnum = 0;
long handle;
CAENVME_Init(BType, linker, bdnum, &handle);

crt.handle[crt.NumberOfCrates] = handle;
crt.NumberOfCrates++;

u_int16_t dataReg;

tdc_readreg(crt,tdc,BitSet2,data);
printf("Reading BitSet2 Register: %us\n", dataReg);


tdc_readreg(crt,tdc,FullScaleRange,data);
printf("Reading FullScaleRange Register: %us\n",dataReg);

u_int32_t data;

tdc_readoutput(crt,tdc,OutputBuffer,data);
printf("\nOutput Data %ud\n", data);


CAENVME_End(crt.handle);