//-------------------------------------------------------------------
//  global.h   --   multiDAX: DAQ for neriX
// 
//  contains global #defines and variable definitions
//
//  25.01.2008 MS (changed 24.06.2008)
//-----------------
//  30.07.2013 GP	multiDAX version
//-------------------------------------------------------------------

#include <sys/types.h>

// maximal number of ADCs and Crates
#define ADC_MAX	32	// max. possible value: 8x20=160
#define CRATE_MAX 2	// max. possible value: 8

// Digitizer Register Map (the list in uncomplete)
#define BoardInfoReg                0x8140
#define SoftTriggerReg              0x8108
#define AcquisitionControlReg       0x8100
#define PostTriggerReg              0x8114
#define ChannelEnableMaskReg        0x8120
#define TriggerSourceMaskReg        0x810C
#define ChannelFWRevision           0x108C
#define MotherBoardFWRevision       0x8124
#define EventSizeReg                0x814C
#define BlockOrganizationReg        0x800C
#define ChannelConfigReg            0x8000
#define VMEControlReg               0xEF00
#define BltEvNumReg                 0xEF1C
#define FrontPanelIOData	    0x811C
#define MonitorDacReg               0x8138
#define MonitorModeReg              0x8144
#define ZeroThreshold 		    0x1024
#define ZeroPrePost		    0x1028
#define DACRegister		    0x1098
#define SoftwareTrigger		    0x8108
#define ZLEThreshold		    0x8024
#define SingleZLEThresh		    0x1024		
#define LookBackForward		    0x8028
#define AnalogMonitorSettings	    0x8150
#define S1Threshold		    0x8080
#define CustomWindow				0x8020
#define ChannelNBufferOccupancyReg  0x1094
#define EventStoredReg              0x812C

// Board Types
#define V1724   0
#define V1721   1
#define V1731   2
#define V1720   3

// definitions
#define FILLER       	0xFFFFFFFF
#define NO_DATA_TIME 	3600		// DAQ will stop if no counts are recorded for this time (sec) 
#define SC_FILENAME	"multiDAX2sc"	// Path to Slow Control Communication File
#define FILE_ID		"nerix"		// file identifier to create filename

#define NOISE_SAMPLESIZE 40
#define NOISE_ITERATIONS 50

#define TTT_SYNC 1300	// max. allowed deviation for TTT synchronization check

//-------------------------------------------------------------------
// Variable to store information on ADCs
typedef struct
{
  int NumberOfADCs;		// total number of ADCs
  u_int32_t Address[ADC_MAX]; 	// Base Addresses
  int Id[ADC_MAX];		// Board Identifier
  int Crate[ADC_MAX];		// Crate in which Board sits
  int BltSize;			// Size of a Block Transfer
  int BoardInfo;		// Show only Board Infos?
  u_int32_t ExpectedEvSize; 	// ...
  int BufferSize;		// max. buffer to store data of one module
  int NbSamplesEvt;		// max. lenght of waveform
  int NbMemoryBlocks;	// max. number of events in output buffer
  int ZLE;			// zero length encoding activated? (1/0)
  int baseline;			// position of baselines
  int trig;			// 1: S1 trigger, 2: S2 trigger
  int busy;			// activate front panel to read busy status (1/0)?
  u_int32_t trigmask;		// trigger mask: which modules send signal to AnalogSum Output?
  int pmt[ADC_MAX][8];		// Store Pmt to Channel mapping: pmt[Module][Channel]=PMT_Number
  int mask[ADC_MAX];		// store mask of channels present on a board
  int pmtmax;			// store highest PMT number
  int NbChannels;		// how many channels are used
  u_int32_t ttt0[ADC_MAX];	// initial trigger time tag value
  u_int32_t tttCompare;	// TTT for comparison
  u_int32_t tttLast;	// TTT of previous event
  int tttLastMaxDev;	// max. TTT deviation of the previous event
  // properties of the ADC
  float voltage_range;		// in Volts
  int sample_size;		// in bits
  int sampling_freq;		// in Hz
  int TriggerLatency;
  int compression;
  int EventLength;		// in case a non-standard event window should be used
} digitizer;

//-------------------------------------------------------------------
// Variable to store information on Crates
typedef struct
{
  int NumberOfCrates;		// total Number of Crates
  int handle[CRATE_MAX];	// device handle
} crate;

//-------------------------------------------------------------------
// Variable to store time information (time, triggers, etc)
typedef struct
{
    u_int64_t totnb; 		// total number of bytes read
    u_int64_t last_totnb; 	// to calculate the difference
    int32_t CurrentTime;	// timing
    int32_t PreviousTime;	// ...
	int32_t msTime;		    // ...
    u_int32_t tottrg;		// total number of triggers
    u_int32_t last_tottrg;  	// to calculate the difference
    u_int32_t last_ave;     	// to calculate the average
    int store10[10];		// to calculate 10s trigger average
    int MeasSeconds;		// the number of seconds the measurement is running    
    int DisplayTime;		// print to screen every x seconds
	int32_t StartTime;		// seconds to synchronize event time to computer
	u_int32_t lastTTT;		// store TTT of last event to detect clock overflow
	u_int32_t reps;			// clock overflow counter
} timeandtrigger;

//-------------------------------------------------------------------
// Variable to store scaler information 
typedef struct
{
  int Crate;                    // crate controller identifier
  u_int32_t Address;        	// base address
  u_int32_t cnt[32];        	// counter values
  u_int32_t events;		// store number of recorded events per scaler cycle to logfile
  int timecnt;			// internal counter to check wether scaler have to be read
  int scalertime;		// scalers are read every <scalertime> seconds
  int32_t start;		// timing
  int32_t stop;			// timing
  int32_t old;			// timing
  double totaltime;		// total time (from external clock)
  double lifetime; 		// lifetime (ext. clock)
  double deadtime;		// deadtime (ext. clock)
  int totalcnt;			// scaler channels ...
  int lifecnt;
  int deadcnt;
} scaler;

//-------------------------------------------------------------------
// Variable for General Purpose Module
typedef struct
{
  int Crate;			// crate controller identifier
  u_int32_t Address;		// base address
  int HEVeto;			// High Energy Veto Status
} multipurposemodule;

//-------------------------------------------------------------------
// Variable to hold information for oscilloscope mode
typedef struct
{
  int on;			// graphics display switched on?
  int module;			// module to display
  int channel;			// channel to display
  int pmtNb;			// PMT to display
  int x_display;		// 0..all, 1..manual, 2..auto
  int y_display;		// 0..all, 1..manual, 2..auto
  int x_low;			// lower display border x-axis
  int x_high;			// upper display border x-axis
  int y_low;			// lower display border y-axis
  int y_high;			// upper display border y-axis
  int x_autorange;		// sets the scale for the x-axis auto-range

} var_graphics;






 // ----TDC variables



 
//define useful register addresses
#define BitSet1 0x1006
#define BitClear1 0x1008
#define BitClear2 0x1034
#define BitSet2 0x1032
#define FullScaleRange 0x1060
#define OutputBuffer 0x0000
#define EventCounterLow 0x1024
#define EventCounterHigh 0x1026
#define ControlRegister1 0x1010
#define FCLRRegister 0x102E

#define dataBS2On 0x5C90 // acquistion on (check validity of data)
//#define dataBS2On 0x5CB0 // acquistion on (keep all channels regardless of validity of data)
#define dataBS2Off 0x5C92 // acquisition off
#define dataBSC2 0x4 // reset data clear
#define dataFSR 0xB4 // set full scale range
#define dataBS1 0x0080 // software reset on board
#define dataCR1 0x20 // enable BERR
#define dataFCLR15 0x140 // set FCLR window for 15 us
#define dataFCLR16 0x160 // set FCLR window for 16 us
#define dataFCLR17 0x180 // set FCLR window for 17 us
#define dataFCLR19 0x1C0 // set FCLR window for 19 us
/*
typedef struct
{
  int NumberOfCrates;		// total Number of Crates
  int handle[1];	// device handle
} crate_simple;
*/
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
  int baseline;			// position of baselines
  int NbChannels;		// how many channels are used
  // properties of the ADC
  float time_range;		// in Volts
  int sample_size;		// in bits
  int sampling_freq;		// in Hz
  int mask[16]; // stores information about which channels have data
				// by taking 2**channel number
} timedc;