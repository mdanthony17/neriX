import neriX_analysis
import ROOT as root

import os, math, sys
from array import array
from rootpy.plotting import Hist

if(len(sys.argv) != 2):
	print 'Usage is python s2_trig_from_gas_gain.py <ROOT filename>'
	sys.exit(1)

filename = sys.argv[1]

# parameters
#s2_pars = [100, 0., 200.] #bottom


max_nb_electrons = 3
colors = [4, 2, 8, 7, 5, 9]
#s2_fit_limits = [10., 180.]
#s2_fit_limits = [0., 180.] #bottom
s2_fit_limits = [-50., 130.] #top
#s2m_min = 10

fermi_dirac_threshold = 0
fix_threshold_function = 1

s2_peak_finding_efficiency_threshold = 0#13.5583
s2_peak_finding_efficiency_rolloff = 1#3.805

preliminary = 0

s2_hist_title = '; S2 [pe];'

current_analysis = neriX_analysis.neriX_analysis(filename)

results_dir = './results/run_' + str(current_analysis.get_run()) + '/' + str(current_analysis.get_filename())

if not os.path.exists(results_dir):
	os.mkdir(results_dir)

results = open(results_dir + '/s2.txt', 'w')

#current_analysis.add_dt_cut()
#tree.SetAlias("ctgS2sTot","S2sTotBottom[]/GetGainCorrectionBottomPMT(TimeSec)")
#tree.SetAlias("ctgS2sTot","S2sTotBottom[]") # bottom only
#tree.SetAlias("ctgS2sTot","S2sTot[]") # total
#tree.SetAlias("ctgS2sTot","S2sTotTop[]") # top only
#tree.SetAlias("ctgS1sTot","czS1sTotBottom/GetRelativeCorrectionBottomPMT(TimeSec)")
#tree.SetAlias("czS1sTotBottom","(S1sTotBottom[]/(0.863098 + (-0.00977873)*Z))")

# will correct after the fact
#gROOT.ProcessLine(".L LoadFunctions.C");

# cuts

#print 'Currently using top!!!'
#s2_pars = [100, 0, 80.] #top
#current_analysis.add_cut('(S2sTotTop[] > %f)' % s2_pars[1])
#parameterToDraw = 'S2sTotTop[]'

s2_pars = [75, 5, 150.] #bottom
# look after S2
"""
parameterToDraw = 'S2sTotBottom[]'
current_analysis.add_cut('(S2sTot[] < %f)' % s2_pars[2])
current_analysis.add_cut('(S2sTotBottom[] > %f)' % s2_pars[1])
current_analysis.add_cut('S2sTotBottom[0] > 150')
#current_analysis.add_cut('(S2sCoin[0] > 14)')
current_analysis.add_cut('(S2sCoin[] > 3)')
current_analysis.add_cut('(S2sPeak[] > 3300 && S2sPeak[] < 3600)')
#current_analysis.add_cut('(S2sPeak[] > (S2sPeak[0] + 100) && S2sPeak[] < (S2sPeak[0] + 2000)) || (S2sPeak[] > 6000 && S2sPeak[] < 16000)')
current_analysis.add_dt_cut(2, 13)
current_analysis.add_cut('sqrt(pow(S2sPosCoG[][0],2)+pow(S2sPosCoG[][1],2)) < 0.25')
"""

# look after S1

peakToLookAt = 2

parameterToDraw = 'S2sTotBottom[%d]' % peakToLookAt
current_analysis.add_cut('(S2sTot[%d] < %f)' % (peakToLookAt, s2_pars[2]))
current_analysis.add_cut('(S2sTotBottom[%d] > %f)' % (peakToLookAt, s2_pars[1]))
current_analysis.add_cut('S1sTotBottom[0] > 1000')
#current_analysis.add_cut('(S2sCoin[%d] > 7)' % (peakToLookAt))
current_analysis.add_cut('(S2sCoin[%d] > 2)' % (peakToLookAt))
current_analysis.add_cut('(S2sPeak[%d] > (S1sPeak[0]+50) && S2sPeak[%d] < (S1sPeak[0]+250))' % (peakToLookAt, peakToLookAt))
#current_analysis.add_dt_cut(4, 13)
current_analysis.add_z_cut()
current_analysis.add_cut('sqrt(pow(S2sPosCoG[][0],2)+pow(S2sPosCoG[][1],2)) < 0.1')




"""
parameterToDraw = 'S2sTotBottom[]'
current_analysis.add_cut('(S2sTot[] < %f)' % s2_pars[2])
current_analysis.add_cut('(((S2sPeak[]-S2sPeak[0])/100.) > 14. && ((S2sPeak[]-S2sPeak[0])/100.) < 16.)')
current_analysis.add_cut('(S2sCoin[] > 16)')
current_analysis.add_cut('S2sTotBottom[0] > 150')
"""

#current_analysis.add_cut('sqrt(pow(S2sPosFann[][0],2)+pow(S2sPosFann[][1],2)) < 0.7')
#current_analysis.add_cut('S2sPosCoG[][0] > 0.05')

#current_analysis.add_cut('(S2sTotBottom[] > 10 && S2sTotBottom[] < 27)')
#current_analysis.get_T1().Scan('EventId:S2sTotTop[]:S2sTotBottom[]:S2sPeak[]', current_analysis.get_cuts())

s2_hist = Hist(s2_pars[0], s2_pars[1], s2_pars[2], name='s2_hist', title='s2_hist', drawstyle='hist')
s2_hist.SetStats(0)
#s2_hist.SetDrawStyle('hist')
#s2_hist.SetMarkerSize(1)

# problem with bad numerical expression GetGainCorrection...
# occurs here
current_analysis.Draw(parameterToDraw, hist=s2_hist, selection=current_analysis.get_cuts())
#print s2_hist.Draw()


#current_analysis.get_T1().Scan('EventId:%s:S2sPeak[%d]' % (parameterToDraw, peakToLookAt), current_analysis.get_cuts() + ' && %s > 15 && %s < 35' % (parameterToDraw, parameterToDraw))


s2_texts = []
s2_funcs = []
s2_total_func_exp = '[2]*exp(-0.5*((x-[0])/([1]+[%d]))**2)' % (max_nb_electrons+4)
for electron_nb in range(1, max_nb_electrons+1):
	s2_funcs.append(root.TF1('s2_func%d' % electron_nb, '[2]*exp(-0.5*((x-%d*[0])/(sqrt(%d)*[1]))**2)' % (electron_nb, electron_nb), s2_pars[1], s2_pars[2]))
	s2_texts.append(root.TLatex(0., 0., '%d e-' % electron_nb))
	if electron_nb > 1:
		# ***** see notes below - this is combination step
		s2_total_func_exp += '+[%d]*exp(-0.5*((x-%d*[0])/(sqrt(%d)*[1]))**2)' % (electron_nb+1, electron_nb, electron_nb)

if fermi_dirac_threshold:
	threshold_exp = '1./(1+exp(([%d]-x)/([%d])))' % (max_nb_electrons+2, max_nb_electrons+3)
else:
	threshold_exp = '0.5*(1+TMath::Erf((x-[%d])/(sqrt(2)*[%d])))' % (max_nb_electrons+2, max_nb_electrons+3)
# paper speaks of fermi-dirac so why use Erf?

# estimate mean and sigma for s2
s2m_estimate = s2_hist.GetXaxis().GetBinCenter(s2_hist.GetMaximumBin())
s2s_estimate = 1.5*math.sqrt(s2m_estimate)

s2_total_func_params = []
s2_total_func_params.append(s2m_estimate)
s2_total_func_params.append(s2s_estimate)
for electron_nb in range(1, max_nb_electrons+1):
	electron_peak_bin = s2_hist.GetXaxis().FindBin(electron_nb*s2m_estimate)
	s2_total_func_params.append(0.8*s2_hist.GetBinContent(electron_peak_bin))
s2_total_func_params.append(s2_peak_finding_efficiency_threshold)
s2_total_func_params.append(s2_peak_finding_efficiency_rolloff)

# below is awesome way to combine functions
# above is a combination step for each of the electron peaks
# these peaks are added together above and then applied here
# with threshold function
s2_total_func = root.TF1('s2_total_func', '(%s)*(%s)' % (s2_total_func_exp, threshold_exp), s2_pars[1], s2_pars[2])
s2_total_func.SetLineWidth(1)
#s2_total_func.SetParameters(30., 6., 2300., 2000., 750., 200., 20., 5.)
#s2_total_func.SetParameters(30., 10., 400., 250., 150., 100., 20., 5.)
s2_total_func.SetParameters(*s2_total_func_params)
#s2_total_func.SetParLimits(0, s2m_min, 1.5*s2m_min)
#s2_total_func.SetParLimits(1, 0, 15.)

# set limits for mean and sigma
s2_total_func.SetParLimits(0, s2m_estimate*0.65, s2m_estimate*1.35)
s2_total_func.SetParLimits(1, s2s_estimate*0, s2s_estimate*1.9)

for electron_nb in range(1, max_nb_electrons+1):
	s2_total_func.SetParLimits(electron_nb+1, 0., s2_hist.GetMaximum())
# max_nb_electrons+{2,3} is just a way to keep track of indices
# these parameters are seperate fromt he rest
s2_total_func.SetParLimits(max_nb_electrons+2, 0., 1.5*s2_peak_finding_efficiency_threshold)
s2_total_func.SetParLimits(max_nb_electrons+3, 0., 1.5*s2_peak_finding_efficiency_rolloff)
#s2_fit_limits = [0.5*s2_peak_finding_efficiency_threshold, 4.5*s2m_estimate]

# systematic width in first peak
s2_total_func.SetParLimits(max_nb_electrons+4, 0, 10)

if fix_threshold_function:
	s2_total_func.FixParameter(max_nb_electrons+2, s2_peak_finding_efficiency_threshold)
	s2_total_func.FixParameter(max_nb_electrons+3, s2_peak_finding_efficiency_rolloff)

s2_hist.Fit(s2_total_func, '0MEL', '', *s2_fit_limits)
#s2_hist.Fit(s2_total_func, 'q0M', '', 0.5*s2_peak_finding_efficiency_threshold, 2.*s2_total_func.GetParameter(0))

for i, s2_func in enumerate(s2_funcs):
	s2_func.SetParameters(s2_total_func.GetParameter(0), s2_total_func.GetParameter(1) + (s2_total_func.GetParameter(max_nb_electrons+4) if i==0 else 0.), s2_total_func.GetParameter(i+2))

	s2_func.SetLineWidth(1)
	s2_func.SetLineColor(colors[i])

for i, s2_text in enumerate(s2_texts):
	s2_text.SetTextColor(colors[i])
	s2_text.SetTextSize(0.03)
	s2_text.SetTextAlign(22)
	s2_text.SetIndiceSize(0.03)
	s2_text.SetX((i+1)*s2_total_func.GetParameter(0))
	s2_text.SetY(0.5*s2_total_func.GetParameter(i+2))

s2_mu = s2_total_func.GetParameter(0)
s2_emu = s2_total_func.GetParError(0)
s2_sigma = s2_total_func.GetParameter(1)
s2_esigma = s2_total_func.GetParError(1)
efficiency_threshold = s2_total_func.GetParameter(max_nb_electrons+2)
efficiency_rolloff = s2_total_func.GetParameter(max_nb_electrons+3)

s2_fit_text = root.TLatex(0.46, 0.76, '#mu_{S2, e-} #pm #sigma_{#mu, e-} = %0.2f #pm %0.2f pe' % (s2_total_func.GetParameter(0), s2_total_func.GetParError(0)))
s2_fit_text.SetNDC()

y_max = int(1.4*s2_hist.GetMaximum())

s2_threshold_text = root.TLatex(0.46, 0.70, '#sigma_{S2, e-} #pm #sigma_{#mu, e-} = %0.2f #pm %0.2f pe' % (s2_total_func.GetParameter(1), s2_total_func.GetParError(1)))
s2_threshold_text.SetNDC()

#s2_total_text = root.TText(65., 2500., 'data+fit')
s2_total_text = root.TText(65., 5000., 'data+fit')
s2_total_text.SetTextSize(0.03)

#s2_threshold_func = root.TF1('s2_threshold_func', '10000.*(%s)' % threshold_exp, s2_pars[1], s2_pars[2])
s2_threshold_func = root.TF1('s2_threshold_func', '(%f/3.)*(%s)' % (y_max, threshold_exp), s2_pars[1], s2_pars[2])
s2_threshold_func.SetParameter(max_nb_electrons+2, s2_total_func.GetParameter(max_nb_electrons+2))
s2_threshold_func.SetParameter(max_nb_electrons+3, s2_total_func.GetParameter(max_nb_electrons+3))
s2_threshold_func.SetLineWidth(1)
s2_threshold_func.SetLineStyle(2)

preliminary_text = root.TText(0.29, 0.28, 'Preliminary')
preliminary_text.SetNDC(1)
preliminary_text.SetTextSize(0.15)
preliminary_text.SetTextAngle(34.)
preliminary_text.SetTextColor(590)

# canvas
c1 = root.TCanvas('c1', '', 100, 50, 800, 600)
c1.SetLeftMargin(0.112)
c1.SetRightMargin(0.10)
c1.SetTopMargin(0.05)
c1.SetBottomMargin(0.112)
c1.SetTicky(0)
c1.SetGridx()
c1.SetGridy()

# drawing canvas
c1.cd()
#c1.SetLogy()
s2_hist.SetTitle(s2_hist_title)
s2_hist.GetXaxis().SetRangeUser(s2_pars[1], s2_pars[2])
s2_hist.GetYaxis().SetRangeUser(0., y_max)
s2_hist.GetXaxis().CenterTitle()
s2_hist.Draw('')
if preliminary:
	preliminary_text.Draw('same')
s2_hist.Draw('same')
for s2_func in s2_funcs:
	s2_func.Draw('same')
for s2_text in s2_texts[:3]:
	s2_text.Draw('same')
s2_fit_text.Draw('same')
s2_threshold_func.Draw('same')
s2_total_func.Draw('same')
s2_threshold_text.Draw('same')
#s2_total_text.Draw('same')
c1.Update()
axis = root.TGaxis(root.gPad.GetUxmax(), root.gPad.GetUymin(), root.gPad.GetUxmax(), root.gPad.GetUymax(), 0., 3., 510, '+L')
axis.SetTitle('Efficiency')
axis.CenterTitle()
axis.SetDecimals()
axis.SetLabelSize(0.045)
axis.SetTitleSize(0.05)
axis.SetTextFont(132)
axis.SetLabelFont(132)
axis.SetTitleOffset(1.0)
axis.Draw()
c1.SaveAs(results_dir + '/s2.png')
c1.SaveAs(results_dir + '/s2.C')

results.write('anode (kV)'.rjust(10) + 'cathode (kV)'.rjust(10) + 'mu (pe)'.rjust(10) + 'mu_err (pe)'.rjust(12) + 'sigma (pe)'.rjust(12) + 'sigma_err (pe)'.rjust(14) + 'threshold (pe)'.rjust(16) + 'rolloff (pe)'.rjust(14) + '\n')
results.write(('%0.4f' % current_analysis.get_anode_setting()).rjust(10) + ('%0.4f' % current_analysis.get_cathode_setting()).rjust(10) + ('%0.4f' % s2_mu).rjust(10) + ('%0.4f' % s2_emu).rjust(12) + ('%0.4f' % s2_sigma).rjust(12) + ('%0.4f' % s2_esigma).rjust(14) + ('%0.4f' % efficiency_threshold).rjust(16) + ('%0.4f' % efficiency_rolloff).rjust(14) + '\n')

raw_input('Please press enter to continue...')



#print 'anode (kV) \tmu (pe) \tmu_err (pe) \tsigma (pe) \tsigma_err (pe) \tthreshold (pe) \trolloff (pe)'
#print '%0.4f \t%0.4f  \t%0.4f \t%0.4f \t%0.4f \t%0.4f \t%0.4f' % anodeVoltage % s2_mu % s2_emu % s2_sigma % s2_esigma % efficiency_threshold % efficiency_rolloff
