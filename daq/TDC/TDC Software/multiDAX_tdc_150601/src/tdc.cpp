//to compile use the following:
//g++ -O0 -g -Wall -Wno-deprecated -fPIC -DLINUX -O -lCAENVME -lm -lxdio $(root-config --libs) -I. $(root-config --cflags) tdc_trial.cpp -o tdc_trial

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


//performs vme write
int tdc_writereg(crate& crt, 		// crate
		timedc& tdc,		// adc
		u_int32_t addr,		// the register to write to
		u_int16_t data)		// the value to set
{
  char msg[100];
  int ret;

  //cvA32_U_DATA = A32 non-privileged data access
  //cvD32 = 32 bit data width
  //success of CAEN functions returns 0 (cvSuccess)
  //these are located in CAENVMEtypes.h
  if ((ret=CAENVME_WriteCycle(crt.handle[tdc.Crate], tdc.Address+addr,
			&data, cvA32_U_DATA, cvD16)) != cvSuccess) {
	sprintf(msg,":::: ERROR: VME Write failure at address %08X (code %d) ::::", (tdc.Address + addr),ret);
	printf("%s", msg);
	return -1;
  }  

  return 0;
}

//performs vme read for registers (except output buffer)
int tdc_readreg(crate& crt, 		// crate
		timedc& tdc,		// adc
		u_int32_t addr,		// the register to write to
		u_int16_t& data)	// the value to read
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
	printf("%s", msg);
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
	 printf("%s", msg);
     return -1;		
   }		
   
   data=temp;
   return 0;
}

//need to set BufferSize and BltSize in timedc struct
int tdc_mblt(crate& crt, timedc& tdc, u_int32_t *buff)
{
	int blt_bytes=0, nb=0, ret;
	int cycles=0;
	
	do {
		ret = CAENVME_FIFOBLTReadCycle(crt.handle[tdc.Crate], tdc.Address,
									   ((unsigned char*)buff)+blt_bytes, tdc.BltSize, cvA32_U_MBLT, cvD64, &nb);
		cycles++;
		if ((ret != cvSuccess) && (ret != cvBusError)) {
			printf(":::: ERROR: CAENVME_FIFOBLTReadCycle Error ::::");
			return -1;
		}
		blt_bytes += nb;
		//buffer size is 4 kB (buffer does not include header or EOB)
		if (blt_bytes > tdc.BufferSize) {
			printf(":::: ERROR: BUFFER_SIZE too small! Please increase it and run again. ::::");
			return -1;
		}
	} while (ret != cvBusError);
	return blt_bytes;
}

// get the largest number of events stored
int tdc_eventstored(crate& crt, timedc& tdc)
{
	// low is first 16 of 24 bits
	// high is last 8 of 24 bits
	u_int16_t lowCounts;
	u_int16_t highCounts;
	int convertHighCounts = 0;
	
	int stored = 0;
	tdc_readreg(crt,tdc,EventCounterLow,lowCounts);
	tdc_readreg(crt,tdc,EventCounterHigh,highCounts);
	
	convertHighCounts = highCounts;
	
	convertHighCounts *= pow(2,16);
	stored = convertHighCounts + lowCounts;
	return stored;
}

// initialize certain registers
int tdc_init(crate& crt, timedc& tdc)
{
	tdc.Address = 0x0A000000; // use the four rotary wheels on module
	tdc.Crate = 0;

	//may need interrupt (via register) but not including it for now
	tdc_writereg(crt, tdc, BitSet1, dataBS1);
	tdc_writereg(crt, tdc, BitClear1, dataBS1);
	
	usleep(10000);
	
	tdc_writereg(crt, tdc, FullScaleRange, dataFSR);
	tdc_writereg(crt, tdc, BitSet2, dataBS2On);
	tdc_writereg(crt, tdc, ControlRegister1, dataCR1);
	tdc_writereg(crt, tdc, FCLRRegister, dataFCLR17);
	
	//tdc.BltSize = 128;
	//tdc.BltSize = 32;
	//tdc.BltSize = 4092;
	tdc.BltSize = 256; // same as ADC (see man page 17)
	tdc.BufferSize = 32*34*4; // 32 events * 34 words * 4 bytes per word
	//tdc.BufferSize = 4352;
	
	
	
	tdc.NbSamplesEvt = 1; // number of samples per event
	tdc.NbChannels = 6;		// how many channels are used
	tdc.time_range = 202.524;		// in us
	tdc.sample_size = 16;		// in bits (include data and validity check)
	tdc.sampling_freq = 0;		// in Hz
	
	return 0;
}

// disable the TDC
int tdc_disable(crate& crt, timedc& tdc)
{
	u_int16_t data;
    
	// enable bit 0x4=100 to clear data
	tdc_readreg(crt,tdc,BitSet2,data);
	data |= 0x4;
	tdc_writereg(crt,tdc,BitSet2,data);
	
	return 0;
}


// enable the TDC: resets data reset bit
int tdc_enable(crate& crt, timedc& tdc)
{

	tdc_writereg(crt,tdc,BitClear2,dataBSC2);
	
	return 0;
}

int tdc_clear_data_buffer(timedc& tdc, u_int32_t *dataBuffer, int bytesCopied)
{
	int eventsToErase = bytesCopied / 4;
	for (int i = 0; i < eventsToErase; i++)
	{
		//printf("tdc.cpp data: %lX\n", dataBuffer[i]);
		if(dataBuffer[i] == 0x0) break;
		dataBuffer[i] = 0x0;
	}
	return 0;
}


/*


int main()
{

//crate variable
crate crt;
crt.NumberOfCrates = 0;

timedc tdc;




//taken from multiDAXconfig.ini
//initialize VME
CVBoardTypes BType = cvV2718;
short linker = 0;
short bdnum = 0;
int32_t handle;
CAENVME_Init(BType, linker, bdnum, &handle);

crt.handle[crt.NumberOfCrates] = handle;
crt.NumberOfCrates++;


tdc_init(crt, tdc);

//check TDC register values
u_int16_t dataReg = 0x0;

tdc_readreg(crt,tdc,BitSet2,dataReg);
printf("Reading BitSet2 Register: %hx\n", dataReg);

dataReg = 0x0;

tdc_readreg(crt,tdc,FullScaleRange,dataReg);
printf("Reading FullScaleRange Register: %hx\n",dataReg);

dataReg = 0x0;

tdc_readreg(crt,tdc,BitSet1,dataReg);
printf("Reading BitSet1 Register: %hx\n",dataReg);

dataReg = 0x0;

tdc_readreg(crt,tdc,ControlRegister1,dataReg);
printf("Reading ControlRegister1 Register: %hx\n",dataReg);

// malloc space for the data
u_int32_t *dataBuffer;
if ((dataBuffer = (u_int32_t *)malloc(tdc.BufferSize)) == NULL)
{
	printf(":::: ERROR: Can't allocate memory buffer of %d B ::::", tdc.BufferSize);
	exit(1);
}

for(int i = 0; i < 10; i++)
{
	//tdc_init(crt, tdc);
	tdc_enable(crt, tdc);
	usleep(15);
	
	
	//tdc_readreg(crt,tdc,BitSet2,dataReg);
	//printf("Reading BitSet2 Register: %hx\n", dataReg);
	//u_int32_t data;
	//tdc_readoutput(crt,tdc,OutputBuffer,data);
	//printf("\nOutput Data: 0x%08X\n", data);
	
	int bytesCopied = 0;
	bytesCopied = tdc_mblt(crt, tdc, dataBuffer);
	printf("\nBytes copied: %d", bytesCopied);
	printf("\nOutput Data 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X\n", dataBuffer[0], dataBuffer[1], dataBuffer[2], dataBuffer[3], dataBuffer[4], dataBuffer[5]);
	printf("Output Data 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X\n", dataBuffer[6], dataBuffer[7], dataBuffer[8], dataBuffer[9], dataBuffer[10], dataBuffer[11]);
	// remember that event counter counts all events (not just accepted)
	
	tdc_clear_data_buffer(tdc, dataBuffer, bytesCopied);
	tdc_disable(crt, tdc);
}
// remember to free malloced memory
free(dataBuffer);

tdc_disable(crt, tdc);

CAENVME_End(crt.handle[0]);

}
*/
