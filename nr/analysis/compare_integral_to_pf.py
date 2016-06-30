#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend, Canvas

#--------------- Start Parameters to Change ----------------

s1NumBins = 12
s1Min = 0
s1Max = 12

#--------------- End Parameters to Change ----------------


if(len(sys.argv) != 5):
	print 'Usage is python compare_integral_to_pf.py <run> <anode> <cathode> <degree>'
	sys.exit(1)

run = int(sys.argv[1])
anodeSetting = float(sys.argv[2])
cathodeSetting = float(sys.argv[3])
degreeSetting = int(sys.argv[4])


current_analysis = neriX_analysis.neriX_analysis(neriX_analysis.pull_all_files_given_parameters(run, anodeSetting, cathodeSetting, degreeSetting), degreeSetting, cathodeSetting, anodeSetting)
sForHistograms = '%d deg, %.3f kV' % (degreeSetting, cathodeSetting)

colzOption = 'COLZ'



s1_branch_integral = 'S1IntegralBeforeLiqSci[0]'
s1_branch_pf = 'S1sTotBottom[0]'


# add cuts

current_analysis.add_radius_cut(0, 0.85)
current_analysis.add_single_scatter_cut()
#current_analysis.add_xs1asym_cut() # no xs1asym because it uses S1sTotBottom
current_analysis.add_xs2asym_cut()
current_analysis.add_temp_neutron_cut(degreeSetting)
current_analysis.add_cut('S2sPeak[0] > 600 && S2sPeak[0] < 2400') # makeshift dt cut that applies to both S1 branches
current_analysis.add_s1_liqsci_peak_cut()
current_analysis.add_cut('%s > 0' % s1_branch_integral)

current_analysis.set_event_list()




c1 = Canvas()

h_s1_comparison = Hist2D(s1NumBins, s1Min, s1Max, s1NumBins, s1Min, s1Max, title='S1 Method Comparison - %s' % (sForHistograms), name='h_s1_comparison')
current_analysis.Draw('%s:%s' % (s1_branch_integral, s1_branch_pf), hist=h_s1_comparison, selection=current_analysis.get_cuts())


h_s1_comparison.GetXaxis().SetTitle('%s [PE]' % s1_branch_integral)
h_s1_comparison.GetYaxis().SetTitle('%s [PE]' % s1_branch_pf)
h_s1_comparison.SetStats(0)

h_s1_comparison.Draw('colz')

c1.Update()





tProfile = neriX_analysis.profile_y_median(h_s1_comparison)
gProfileS1Comparison = root.TGraphAsymmErrors(*tProfile)
#gProfileS1Comparison.SetTitle('S1 Method Comparison - %s' % (sForHistograms))
#gProfileS1Comparison.GetXaxis().SetRangeUser(s1Min, s1Max)
#gProfileS1Comparison.GetYaxis().SetRangeUser(s1Min, s1Max)

#gProfileS1Comparison.Draw('ap')



c1.Update()


# 2d fit does not work well

f1 = root.TF1('f1', 'pol1', s1Min, s1Max)
f1.SetParameters(0, 1)
f1.FixParameter(0, 0)

#hProfile.Fit('f1', 'MELR')
fitResult = gProfileS1Comparison.Fit('f1', 'MELRS')
f1.SetLineStyle(1)
f1.SetLineWidth(2)
f1.SetLineColor(root.kRed)

f1.Draw('same')



sFitInfo1 = 'Slope: %.3f +/- %.3f' % (f1.GetParameter(1), f1.GetParError(1))
#sFitInfo1 = 'Intercept: %.3f +/- %.3f, Slope: %.3f +/- %.3f' % (f1.GetParameter(0), f1.GetParError(0), f1.GetParameter(1), f1.GetParError(1))


pt1 = root.TPaveText(.1,.65,.5,.9,'blNDC')
text1 = pt1.AddText(sFitInfo1)
pt1.SetTextColor(root.kBlue)
pt1.SetFillStyle(0)
pt1.SetBorderSize(0)
pt1.Draw('same')




c1.Update()



raw_input('Enter to continue...')


neriX_analysis.save_plot(['s1_method_comparison', current_analysis.get_filename_no_ext()], c1, 'comparison_integral_to_pf_%s' % current_analysis.get_filename_no_ext())


