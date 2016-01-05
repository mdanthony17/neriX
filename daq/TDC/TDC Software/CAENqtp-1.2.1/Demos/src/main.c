/////////////////////////////////////////////////////////////////////////////
// Name:        main.c
// Purpose:
// Author:      NDA
// Modified by:
// Created:     14/05/07
// RCS-ID:
// Copyright:   CAEN S.p.A. All rights reserved
// Licence:
/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////
// File includes
////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#ifdef WIN32
#include <time.h>
#include <sys/timeb.h>
#include <conio.h>
#include <process.h>
#define popen  _popen   // redefine 'deprecated' popen as _popen
#define pclose _pclose  // redefine 'deprecated' pclose as _pclose
#else
#include <unistd.h>
#include <sys/time.h>
#define Sleep(t)  usleep((t)*1000)
#include <memory.h>
#include <ctype.h>
#endif

#include "common_defs.h"
#include "../include/CAENVMETool/cvt_board_commons.h"
#include "../include/CAENVMETool/cvt_common_defs.h"
#include "../include/CAENVMETool/cvt_V792.h"
#include "user_settings.h"


// Executable gnuplot. NOTE: use pgnuplot instead of wgnuplot in Windows, otherwise
// the pipe will not work.
#ifdef WIN32
#define GNUPLOT_COMMAND  "pgnuplot"
#else
#define GNUPLOT_COMMAND  "gnuplot"
#endif

////////////////////////////////////////////
// File local defines
////////////////////////////////////////////
#define HDR_STR				" HDR              - GEO  : %08x CRATE         : %08x CH COUNT : %08x \n"
#define DATUM_STR			"  DATUM           - GEO  : %08x CHANNEL       : %08x ADC      : %08x UN : %08x OV : %08x \n"
#define NOT_VALID_DATUM_STR	"  NOT VALID DATUM - \n"
#define EOB_STR				" EOB              - GEO  : %08x EVENT COUNTER : %08x \n"
#define UNKNOWN_STR			"\n??? UNKNOWN TAG ??? -          READ WORD     : %08x \n\n"

#define MAX_HISTO_SAMPLES	(CVT_V792_DATUM_ADC_MSK+ 1)
#define MAX_HISTO_CHANNELS	MAX_THRESHOLD_NUM
////////////////////////////////////////////
// File local variables declaration
////////////////////////////////////////////
const char* HISTO_FILENAME= "histo.dat";

////////////////////////////////////////////
// Global visible variables declaration
////////////////////////////////////////////

////////////////////////////////////////////
// File local methods declaration
////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn      int channel_select( int act_channel)
*   \brief   Select the new channel index to be plotted
*
*   \param   act_channel The actual selected channel index
*   \return  The new selected channel index
*/
////////////////////////////////////////////////////////////////////////////////////////////////
static int channel_select( int act_channel) {
    char new_ch_string[10];
    // Clear line
    printf( "\r                                                                        ");
    printf( "\rInsert the new channel value [0..%d] (actual: %d) : ", MAX_HISTO_CHANNELS- 1, act_channel);
    if ( getstring( new_ch_string, 3)) {
        act_channel= atoi( new_ch_string);
        if ( act_channel< 0) {
            act_channel= 0;
        } else if ( act_channel> MAX_HISTO_CHANNELS- 1) {
            act_channel= MAX_HISTO_CHANNELS- 1;
        }
    }
    // Clear line
    printf( "\r                                                                        ");
    // restore position
    printf( "\r");
    return act_channel;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn      long get_time( void)
*   \brief   Returns the time in milliseconds since first procedure call
*
*   \return  The time in milliseconds since first procedure call
*/
////////////////////////////////////////////////////////////////////////////////////////////////
long get_time()
{
	static long startup= 0;
	long time_ms;

#ifdef WIN32
	struct _timeb timebuffer;

	_ftime( &timebuffer );
	if( !startup) {
		startup= (long)timebuffer.time;
	}
	time_ms = (long)( timebuffer.time- startup) * 1000 + (long)timebuffer.millitm;
#else
	struct timeval t1;
	struct timezone tz;

	gettimeofday(&t1, &tz);
	if( !startup) {
		startup= (long)t1.tv_sec;
	}
	time_ms = (t1.tv_sec- startup) * 1000 + t1.tv_usec / 1000;
#endif

	return time_ms;
}

/**************************************************
**************************************************/

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \fn      int main(int argc, char **argv)
*   \brief   CAENVMETool demo usage for the V792 board
*
*            Setups a V792 board, reads some events and stores into output file
*   \param   argc number of command line arguments
*   \param   *argv[] command line arguments' list
*   \return  = 0 procedure completed correctly: < 0 some error occurred
*/
////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)

{
    int ret_val= 0;									// procedure exit value
    cvt_V792_data board_data;						// board data
    user_setting_data user_setting;					// user settings
    FILE* parsed_out_file= NULL;					// parsed output file
    FILE* raw_out_file= NULL;						// raw output file
    UINT8 *data_buff= NULL;							// read data buffer
    UINT32 data_size;
    int32_t vme_handle= -1;							// The CAENVMELib handle
    long read_events= 0;
    const int DATA_BUFF_SIZE= 1024*1024;			// The data buffer size
    BOOL paused= FALSE;
    unsigned long *histo_buff[ MAX_HISTO_CHANNELS];	// Histogram data buffer
    FILE* gnu_plot_pipe= NULL;						// gnu_plot pipe
    long next_refresh= 0;
    char aux_string[ 100];
    int act_channel= 0;


    /////////////////////////////////////////
    // Demo application specific
    /////////////////////////////////////////

    memset( &user_setting, 0, sizeof( user_setting));
    memset( &board_data, 0, sizeof( board_data));
    memset( histo_buff, 0, MAX_HISTO_CHANNELS* sizeof( histo_buff[0]));


    //
    // print header
    printf( "\n");
    printf( "-------------------------------------------------------\n");
    printf( "-              C A E N   Q T P    D E M O             -\n");
    printf( "-                                                     -\n");
    printf( "-   www.caen.it                             rev.01.02 -\n");
    printf( "-------------------------------------------------------\n");
    printf( "\n");

    //
    // init user setting module
    if ( !user_settings_open( &user_setting)) {
        ret_val= -1;
        goto exit_point;
    }

    //
    // input parameter check
    if ( !user_settings_parse_input_param( &user_setting, argc, (char**)argv)) {
        ret_val= -2;
        goto exit_point;
    }
    //
    // Vme handle initialization
    while ( TRUE) {
		if ( CAENVME_Init( cvV1718, user_setting.m_link_number, user_setting.m_board_number, &vme_handle)== cvSuccess) {
            user_setting.m_board_type= cvV1718;
            break;
        }
        if ( CAENVME_Init( cvV2718, user_setting.m_link_number, user_setting.m_board_number, &vme_handle)== cvSuccess) {
            user_setting.m_board_type= cvV2718;
            break;
        }
        TRACE("VME INIT ERROR :  press 'Q' to quit or any other key to retry\n");
        if ( toupper( getch()) == 'Q') {
            ret_val= -3;
            goto exit_point;
        }
    }

    /////////////////////////////////////////
    // Library specific
    /////////////////////////////////////////

    //
    // init V792 board data
    TRACE(  " Initializing QTP board data ... ");
    if ( !cvt_V792_open( &board_data, user_setting.m_base_address, vme_handle, user_setting.m_qtp_type)) {
        TRACE( "\nError executing cvt_V792_open \n");
        ret_val= -4;
        goto exit_point;
    }
    TRACE(  " Ok \n");

    //
    // Get system informations
    {
        UINT16 firmware_rev;
        UINT8  piggy_back_type;
        UINT16 serial_number;

        TRACE(  " Getting system informations ... ");
        if ( !cvt_V792_get_system_info( &board_data, &firmware_rev, &piggy_back_type, &serial_number)) {
            TRACE( "\nError executing cvt_V792_get_system_info \n");
            ret_val= -5;
            goto exit_point;
        }
        TRACE(  " Ok \n\n");

        // Show system infos
        TRACE1( "   Firmware Rev.       : %04x\n", firmware_rev);
        TRACE1( "   Piggy back type     : %s\n", cvt_V792_get_piggy_back_str( &board_data, piggy_back_type));
        TRACE1( "   Serial Number       : %04x\n", serial_number);
        TRACE(  "\n");
    }
    //
    // data clear
    TRACE(  " Sending data clear ... ");
    if ( !cvt_V792_data_clear( &board_data)) {
        TRACE( "\nError executing cvt_V792_data_clear \n");
        ret_val= -5;
        goto exit_point;
    }
    TRACE(  " Ok \n");

    //
    // Sliding constant
    TRACE(  " Setting sliding constant... ");
    if ( !cvt_V792_set_sliding_scale( &board_data,
                                      user_setting.m_acquisition_mode_param.m_sliding_scale_enable)) {
        TRACE( "Error executing cvt_V792_set_sliding_scale\n");
        ret_val= -5;
        goto exit_point;
    }
    TRACE(  " Ok\n");

    //
    // Zero suppression
    TRACE(  " Setting zero suppression... ");
	switch( board_data.m_type) {
		case CVT_V965:
		case CVT_V965_TYPE_A:
		case CVT_V1785:
			// Dual range board
			if ( !cvt_V792_set_dual_range_zero_suppression( &board_data,
												user_setting.m_acquisition_mode_param.m_zero_suppression_enable,
												user_setting.m_zero_suppression_param.m_step_threshold,
												&user_setting.m_zero_suppression_param.m_high_thresholds_buff[0],
												&user_setting.m_zero_suppression_param.m_thresholds_buff[0])) {
				TRACE( "Error executing cvt_V792_set_dual_range_zero_suppression\n");
				ret_val= -5;
				goto exit_point;
			}
			break;
		default:
			if ( !cvt_V792_set_zero_suppression( &board_data,
												user_setting.m_acquisition_mode_param.m_zero_suppression_enable,
												user_setting.m_zero_suppression_param.m_step_threshold,
												&user_setting.m_zero_suppression_param.m_thresholds_buff[0])) {
				TRACE( "Error executing cvt_V792_set_zero_suppression\n");
				ret_val= -5;
				goto exit_point;
			}
			break;
	}
    TRACE(  " Ok\n");

    //
    // Acquisition mode
    TRACE(  " Setting acquisition mode ... ");
    if ( !cvt_V792_set_acquisition_mode( &board_data,
                                         user_setting.m_acquisition_mode_param.m_sliding_scale_enable,
                                         user_setting.m_acquisition_mode_param.m_zero_suppression_enable,
                                         user_setting.m_acquisition_mode_param.m_overflow_suppression_enable,
                                         user_setting.m_acquisition_mode_param.m_valid_suppression_enable,
					 user_setting.m_acquisition_mode_param.m_common_stop_enable,
                                         user_setting.m_acquisition_mode_param.m_empty_enable,
                                         user_setting.m_acquisition_mode_param.m_count_all_events
                                       )) {
        TRACE( "Error executing cvt_V792_set_continuous_acquisition_mode \n");
        ret_val= -5;
        goto exit_point;
    }
    TRACE(  " Ok\n");

    //
    // readout mode
    TRACE(  " Setting readout mode ...");
    if ( !cvt_V792_set_readout_mode( &board_data, TRUE, TRUE, TRUE)) {
        TRACE( "Error executing cvt_V792_set_readout_mode \n");
        ret_val= -5;
        goto exit_point;
    }
    TRACE(  " Ok\n");

    // Allocate buffer storage
    data_buff= malloc( DATA_BUFF_SIZE);
    if ( data_buff== NULL) {
        // Insufficient memory
        TRACE1( "Error allocating events' buffer (%i bytes)", DATA_BUFF_SIZE);
        ret_val= -5;
        goto exit_point;
    }
    // Allocate histogram storage
    {
        int i;
        for ( i= 0; i< MAX_HISTO_CHANNELS; i++) {
            histo_buff[ i]= malloc( MAX_HISTO_SAMPLES* sizeof( *histo_buff[i]));
            if ( histo_buff[ i]== NULL) {
                // Insufficient memory
                TRACE1( "Error allocating histogram' buffer (%li bytes)", (long)( MAX_HISTO_SAMPLES* sizeof( *histo_buff[i])));
                ret_val= -5;
                goto exit_point;
            }
            memset( histo_buff[i], 0, MAX_HISTO_SAMPLES* sizeof( *histo_buff[i]));
        }
    }
    if ( strlen( user_setting.m_parsed_output_filename)) {
        // Create output files
        if ( ( parsed_out_file= fopen( user_setting.m_parsed_output_filename, "wt"))== NULL) {
            TRACE1( "Error creating parsed output file '%s'", user_setting.m_parsed_output_filename);
            ret_val= -5;
            goto exit_point;
        }
    }
    if ( strlen( user_setting.m_raw_output_filename)) {
        if ( ( raw_out_file= fopen( user_setting.m_raw_output_filename, "w+b"))== NULL) {
            TRACE1( "Error creating raw output file '%s' \n", user_setting.m_raw_output_filename);
            ret_val= -5;
            goto exit_point;
        }
    }

    //
    // Start acquisition
    TRACE( "\n");
    TRACE( "\nHit 's' to start acquisition , any other to quit ...\n");
    switch ( getch()) {
    case 's':
    case 'S':
        break;
    default:
        ret_val= -4;
        goto exit_point;
    }
    // open gnuplot for the first time
    {
        char *gnu_plot_filename= (char*)malloc( MAX_FILENAME_LENGHT);
        sprintf( gnu_plot_filename, "%s%s", user_setting.m_gnu_plot_path, GNUPLOT_COMMAND);

        // open the pipe
        if (( gnu_plot_pipe = popen( gnu_plot_filename, "w"))== NULL) {
            TRACE( "N957 GNU Plot pipe creation failure");
            ret_val= -3;
            free( gnu_plot_filename);
            goto exit_point;
        }

        //// set some variables in gnuplot
        //fprintf( gnu_plot_pipe, "ch = %d; mod = %d; unit = %d; sens = %f\n", ChSel, ModSel, Unit, Sensitivity);
        free( gnu_plot_filename);
    }

    // gnu plot commands
    strcpy( aux_string, "load 'plot_histo.cfg'\n");
    if ( fwrite( aux_string, 1, strlen( aux_string), gnu_plot_pipe)!= strlen( aux_string)) {
        // out file write error
        TRACE( "\n Pipe write failure\n");
        ret_val= -3;
        goto exit_point;
    }

    TRACE( "\nAcquisition running ...\n");
    TRACE( "\nHit 'r' to reset histogram");
    TRACE( "\nHit 'p' to pause/resume");
    TRACE( "\nHit 'c' to select the channel to plot");
    TRACE( "\nAny other to quit ...");
    TRACE( "\n");

    while(( read_events< user_setting.m_num_events)|| (user_setting.m_num_events<= 0)) {
        //static long last_read_bytes= 0;
        //static long read_bytes= 0;
        data_size= DATA_BUFF_SIZE;
        while ( paused) {
            if ( kbhit()) {
                switch ( getch()) {
                case 'r':
                case 'R': {
                    // reset the histogram here
                    int i;
                    for ( i= 0; i< MAX_HISTO_CHANNELS; i++) {
                        memset( histo_buff[i], 0, MAX_HISTO_SAMPLES* sizeof( *histo_buff[i]));
                    }
                }
                break;
                case 'p':
                case 'P':
                    paused= !paused;
                    break;
                case 'c':
                case 'C':
                    act_channel= channel_select( act_channel);
                    break;
                default:
                    ret_val= 0;
                    goto exit_point;
                }
            }
            Sleep( 1);
        }

        //
        // Read from MEB .....
        if ( !cvt_V792_read_MEB( &board_data, data_buff, &data_size)) {
            TRACE( " \nError executing cvt_V792_read_MEB \n");
            ret_val= -5;
            goto exit_point;
        }

        //
        // .... and store raw data into file (if enabled)
        if ( raw_out_file) {
            if ( fwrite( data_buff, 1, data_size, raw_out_file)!= data_size) {
                TRACE( " \nError writing raw data file \n");
                ret_val= -5;
                goto exit_point;
            }
        }

        //read_bytes+= data_size;
        //if( ( read_bytes>> 10)!= ( last_read_bytes>> 10))
        //{
        //	//
        //	// Give user a feedback every 1KB data
        //	TRACE( ".");
        //}
        //last_read_bytes= read_bytes;
        // Process the data
        {
            UINT32 *tmp_buff= (UINT32*)data_buff;
            char line[ 400];
            size_t str_len;

            while (((long)(data_size-=4) >= 0)&&
                    (( read_events< user_setting.m_num_events)|| (user_setting.m_num_events<= 0))) {
                UINT32 data= *(tmp_buff++);
                *line= '\0';

                if ( kbhit()) {
                    switch ( getch()) {
                    case 'r':
                    case 'R': 
						{
							// reset the histogram here
							int i;
							for ( i= 0; i< MAX_HISTO_CHANNELS; i++) {
								memset( histo_buff[i], 0, MAX_HISTO_SAMPLES* sizeof( *histo_buff[i]));
							}
						}
						break;
                    case 'p':
                    case 'P':
                        paused= !paused;
                        break;
                    case 'c':
                    case 'C':
                        act_channel= channel_select( act_channel);
                        break;
                    default:
                        ret_val= 0;
                        goto exit_point;
                    }
                }

                switch ( data& CVT_QTP_DATA_TYPE_MSK) {
                case CVT_QTP_HEADER: 
					{
						// header
						UINT32 geo= CVT_QTP_GET_HDR_GEO( data);
						UINT32 crate= CVT_QTP_GET_HDR_CRATE( data);
						UINT32 ch_count= CVT_QTP_GET_HDR_CH_COUNT( data);

						sprintf( line, HDR_STR, geo, crate, ch_count);
					}
					break;
                case CVT_QTP_EOB: 
					{
						// EOB
						UINT32 geo= CVT_QTP_GET_EOB_GEO( data);
						UINT32 event_count= CVT_QTP_GET_EOB_EVENT_COUNT( data);

						sprintf( line, EOB_STR, geo, event_count);

						++read_events;
	                }
		            break;
                case CVT_QTP_DATUM: 
					{
						// DATUM
						UINT32 geo= CVT_V792_GET_DATUM_GEO( data);
						UINT32 ch= 0;
						UINT32 adc= CVT_V792_GET_DATUM_ADC( data);
						UINT32 un= CVT_V792_GET_DATUM_UN( data);
						UINT32 ov= CVT_V792_GET_DATUM_OV( data);

						switch ( user_setting.m_qtp_type) {
						case CVT_V792_TYPE_A:
							ch= CVT_V792A_GET_DATUM_CH( data);
							break;
						case CVT_V792_TYPE_N:
							ch= CVT_V792N_GET_DATUM_CH( data);
							break;
						case CVT_V965:
							ch= CVT_V965_GET_DATUM_CH( data);
							break;
						case CVT_V965_TYPE_A:
							ch= CVT_V965A_GET_DATUM_CH( data);
							break;
						case CVT_V785_TYPE_A:
							ch= CVT_V785A_GET_DATUM_CH( data);
							break;
						case CVT_V785_TYPE_N:
							ch= CVT_V785N_GET_DATUM_CH( data);
							break;
						case CVT_V1785:
							ch= CVT_V1785_GET_DATUM_CH( data);
							break;
						case CVT_V775_TYPE_A:
							ch= CVT_V775A_GET_DATUM_CH( data);
							break;
						case CVT_V775_TYPE_N:
							ch= CVT_V775N_GET_DATUM_CH( data);
							break;
						default:
							// Unmatched case !??!
							ret_val= -5;
							goto exit_point;
						}
						// Histogram update
						// sanity checks
						if ( ch< sizeof( histo_buff)/sizeof( histo_buff[0])) {
							++histo_buff[ ch][ adc& (MAX_HISTO_SAMPLES- 1)];
						}
						sprintf( line, DATUM_STR, geo, ch, adc, un, ov);
					}
					break;
                case CVT_QTP_NOT_VALID_DATUM: 
                    // NOT VALID DATUM
                    sprintf( line, NOT_VALID_DATUM_STR);
					break;
				default: 
                    // UNKNOWM data type !!?!
                    // sprintf( line, UNKNOWN_STR, data);
					break;
                }
                if ( (str_len= strlen( line))> 0) {
                    if ( parsed_out_file) {
                        if ( fwrite( line, 1, str_len, parsed_out_file)!= str_len) {
                            // error writing file
                            TRACE1( "\nError writing parsed output file '%s' \n", user_setting.m_parsed_output_filename);
                            ret_val= -5;
                            goto exit_point;
                        }
                    }
                }
            }
        }
        if ( get_time()>= next_refresh) {
            int sample;
            FILE* histo_file= NULL;

            next_refresh= get_time()+ user_setting.m_gnu_plot_refresh;
            if (( histo_file= fopen( HISTO_FILENAME, "wt"))== NULL) {
                // out file write error
                TRACE1( "\nHistogramn output file creation failure '%s'", HISTO_FILENAME);
                ret_val= -3;
                goto exit_point;
            }

            // Save data to histogram file
            for ( sample= 0; sample< MAX_HISTO_SAMPLES; sample++) {
                // Check for user commands
                if ( kbhit()) {
                    switch ( getch()) {
                    case 'r':
                    case 'R': 
						{
							// reset the histogram here
							int i;
							for ( i= 0; i< MAX_HISTO_CHANNELS; i++) {
								memset( histo_buff[i], 0, MAX_HISTO_SAMPLES* sizeof( *histo_buff[i]));
							}
						}
	                    break;
                    case 'p':
                    case 'P':
                        paused= !paused;
                        break;
                    case 'c':
                    case 'C':
                        act_channel= channel_select( act_channel);
                        break;
                    default:
                        ret_val= 0;
                        goto exit_point;
                    }
                }
                sprintf( aux_string, "%ld\n", histo_buff[ act_channel][ sample]);
                //sprintf( aux_string, "%f\t%ld\n", (float)i* user_setting.m_gnu_plot_x_scale, histo_buff[ i]);
                if ( fwrite( aux_string, 1, strlen( aux_string), histo_file)!= strlen( aux_string)) {
                    // out file write error
                    TRACE( "\n Histogram file write failure\n");
                    ret_val= -3;
                    goto exit_point;
                }
            }
            fclose( histo_file);

            // write to gnuplot
            strcpy( aux_string, "plot 'histo.dat' with histogram 2\n");
            // strcpy( aux_string, "set style histogram rows\n plot 'histo.dat' using 2, '' using 4, '' using 6:xticlabels(1)\n");
            if ( fwrite( aux_string, 1, strlen( aux_string), gnu_plot_pipe)!= strlen( aux_string)) {
                // out file write error
                TRACE( "\n Pipe write failure\n");
                ret_val= -3;
                goto exit_point;
            }
            fflush( gnu_plot_pipe);
            Sleep( 1);
        }
    }
    TRACE(  "\n Done \n");

exit_point:
    /////////////////////////////////////////
    // Library specific
    /////////////////////////////////////////

    //
    // Release board resources
    if ( !cvt_V792_close( &board_data)) {
        TRACE( "\nError executing cvt_V792_close\n");
    }

    /////////////////////////////////////////
    // Demo application specific
    /////////////////////////////////////////

    if ( gnu_plot_pipe!= NULL) {
        fprintf( gnu_plot_pipe, "exit\n");
        pclose( gnu_plot_pipe);
    }

    if ( raw_out_file!= NULL) {
        fclose( raw_out_file);
    }
    if ( parsed_out_file!= NULL) {
        fclose( parsed_out_file);
    }
    if ( data_buff!= NULL) {
        free( data_buff);
    }
    {
        int i;
        for ( i= 0; i< MAX_HISTO_CHANNELS; i++) {
            if ( histo_buff[ i])
                free( histo_buff[ i]);
        }
    }

    // close modules
    user_settings_close( &user_setting);

    TRACE( "Hit a key to exit ...");
    getch();

    return ret_val;
}
