import neriX_led_analysis
import sys

if(len(sys.argv) < 2):
	print 'Usage is python spe_gain.py <ROOT filename>'
	sys.exit(1)

filename = sys.argv[1]

#led_cal = neriX_led_analysis.neriX_led_analysis(filename, True)
led_cal = neriX_led_analysis.neriX_led_analysis(filename)
#led_cal.draw_channel(17)
led_cal.fit_all_channels()
#led_cal.remove_spe_results_from_file()
led_cal.plot_all_channels_gain_vs_time()


