
//performs vme write
int tdc_writereg(crate& crt, 		// crate
		timedc& tdc,		// tdc
		u_int32_t addr,		// the register to write to
		u_int16_t data);		// the value to set


//performs vme read for registers (except output buffer)
int tdc_readreg(crate& crt, 		// crate
		timedc& tdc,		// tdc
		u_int32_t addr,		// the register to write to
		u_int16_t& data);	// the value to read


//performs vme read for registers (except output buffer)
int tdc_readreg(crate& crt, 		// crate
		timedc& tdc,		// tdc
		u_int32_t addr,		// the register to write to
		u_int16_t& data);	// the value to read


//need to set BufferSize and BltSize in timedc struct
int tdc_mblt(crate& crt, // crate
		timedc& tdc, // tdc
		u_int32_t *buff); // buffer containing data


// get the number of events
int tdc_eventstored(crate& crt, //crate
		timedc& tdc); // tdc


// initialize certain registers
int tdc_init(crate& crt, // crate
		timedc& tdc); // tdc


// disable the TDC
int tdc_disable(crate& crt, // crate
		timedc& tdc); // tdc


// enable the TDC: resets data reset bit
int tdc_enable(crate& crt, // crate
		timedc& tdc); // tdc


// clear data buffer to prevent false reads
int tdc_clear_data_buffer(timedc& tdc, // tdc
		u_int32_t *dataBuffer, // data buffer in use
		int bytesCopied); // number of bytes copied to overwrite




