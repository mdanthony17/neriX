import ROOT as root
import neriX_analysis
from rootpy.plotting import Hist, Canvas
from math import log, exp
import array, sys
import numpy as np
import gas_gain_fit_results

if len(sys.argv) != 2:
    print 'Usage is \'python fit_threshold_function_for_run.py <run>\''

run_number = int(sys.argv[1])

if not run_number in gas_gain_fit_results.runs:
    print 'Must use one of the following runs: ' + str(gas_gain_fit_results.runs)
    sys.exit()

"""
dCathodeVoltages = {0.345:{'s1_mean':[], 's1_mean_err':[], 's2_mean':[], 's2_mean_err':[]},
					1.054:{'s1_mean':[], 's1_mean_err':[], 's2_mean':[], 's2_mean_err':[]},
					2.356:{'s1_mean':[], 's1_mean_err':[], 's2_mean':[], 's2_mean_err':[]},
					5.500:{'s1_mean':[], 's1_mean_err':[], 's2_mean':[], 's2_mean_err':[]}
				   }
"""

dThresholdFunction = {'fd_center_bot': [], 'fd_center_bot_err': [], 'fd_shape_bot': [], 'fd_shape_bot_err': [],
                      'fd_center_top': [], 'fd_center_top_err': [], 'fd_shape_top': [], 'fd_shape_top_err': [],
                      'time': []}

dFitParameters = gas_gain_fit_results.gas_gain_results[run_number]

neriX_analysis.debug_message('Need to add uncertainty from PMT gain')

# gather paramters of interest from fits
for file in dFitParameters:
    dFile = dFitParameters[file]

    print dFile

    for key in dThresholdFunction:
        if key in dFile:
            dThresholdFunction[key].append(dFile[key])
        elif key == 'time':
            dThresholdFunction[key].append(neriX_analysis.convert_name_to_unix_time(file))

lCorrection = []
lTime = []

num_points = len(dThresholdFunction['time'])
for key in dThresholdFunction:
    # if key is for the error need to propagate
    # gain uncertainty
    if key[-4:] == '_err':
        for i in xrange(len(dThresholdFunction[key])):
            correction = neriX_analysis.get_gain_correction(run_number, dThresholdFunction['time'][i])
            correction_err = neriX_analysis.get_gain_correction_err(run_number, dThresholdFunction['time'][i])

            lCorrection.append(correction)
            lTime.append(dThresholdFunction['time'][i])

            corrected_par = dThresholdFunction[key[:-4]][i]
            uncorrected_par = corrected_par * correction
            uncorrected_par_err = dThresholdFunction[key][i]

            print correction, correction_err, corrected_par, uncorrected_par, uncorrected_par_err
            # print dThresholdFunction[key][i]
            # print (  corrected_par**2 * ( (uncorrected_par_err/uncorrected_par)**2 + (correction_err / correction)**2 )  )**0.5

            if uncorrected_par == 0.:
                uncorrected_par = 1e-6
            dThresholdFunction[key][i] = (corrected_par ** 2 * (
            (uncorrected_par_err / uncorrected_par) ** 2 + (correction_err / correction) ** 2)) ** 0.5

    # convert list to numpy array
    dThresholdFunction[key] = np.asarray(dThresholdFunction[key], dtype=np.float64)

lTime, lCorrection = zip(*sorted(zip(lTime, lCorrection)))
# print lTime
# print lCorrection

# ---------------------------------------
# ---------------------------------------
#  bottom threshold shape
# ---------------------------------------
# ---------------------------------------


c1 = Canvas()

gThresholdShapeBot = root.TGraphErrors(num_points, dThresholdFunction['time'], dThresholdFunction['fd_shape_bot'],
                                       np.zeros(num_points), dThresholdFunction['fd_shape_bot_err'])
gThresholdShapeBot.SetTitle('Fermi Dirac Shape in Time (bottom) - run %d' % run_number)
gThresholdShapeBot.GetXaxis().SetTitle('Time')
gThresholdShapeBot.GetXaxis().SetTimeDisplay(1)
gThresholdShapeBot.GetXaxis().SetTimeFormat('%m/%d')
gThresholdShapeBot.GetYaxis().SetTitle('FD Shape (bottom) [PE]')

fG1 = root.TF1('fG1', 'pol0', dThresholdFunction['time'][0], dThresholdFunction['time'][-1])

fG1.SetLineStyle(9)
fG1.SetLineWidth(1)

gThresholdShapeBot.Draw('ap')
gThresholdShapeBot.Fit('fG1', 'L')

gConfIntervalShapeBot = root.TGraphAsymmErrors(
    *neriX_analysis.create_graph_with_confidence_interval_for_fit(gThresholdShapeBot, root.TVirtualFitter.GetFitter()))
gConfIntervalShapeBot.SetLineColor(root.kBlue)
gConfIntervalShapeBot.SetFillColor(root.kBlue)
gConfIntervalShapeBot.SetFillStyle(3005)
gConfIntervalShapeBot.Draw('3 same')

c1.Update()

# ---------------------------------------
# ---------------------------------------
#  bottom threshold center
# ---------------------------------------
# ---------------------------------------




c2 = Canvas()

gThresholdCenterBot = root.TGraphErrors(num_points, dThresholdFunction['time'], dThresholdFunction['fd_center_bot'],
                                        np.zeros(num_points), dThresholdFunction['fd_center_bot_err'])
gThresholdCenterBot.SetTitle('Fermi Dirac Center in Time (bottom) - run %d' % run_number)
gThresholdCenterBot.GetXaxis().SetTitle('Time')
gThresholdCenterBot.GetXaxis().SetTimeDisplay(1)
gThresholdCenterBot.GetXaxis().SetTimeFormat('%m/%d')
gThresholdCenterBot.GetYaxis().SetTitle('FD Center (bottom) [PE]')

fG2 = root.TF1('fG2', 'pol0', dThresholdFunction['time'][0], dThresholdFunction['time'][-1])

fG2.SetLineStyle(9)
fG2.SetLineWidth(1)

"""
print np.mean(dThresholdFunction['fd_center_bot'])
print np.power(dThresholdFunction['fd_center_bot_err'], -2)
print np.average(dThresholdFunction['fd_center_bot'], weights=np.power(dThresholdFunction['fd_center_bot_err'], -2))
"""

gThresholdCenterBot.Draw('ap')
gThresholdCenterBot.Fit('fG2', 'L')

gConfIntervalCenterBot = root.TGraphAsymmErrors(
    *neriX_analysis.create_graph_with_confidence_interval_for_fit(gThresholdCenterBot, root.TVirtualFitter.GetFitter()))
gConfIntervalCenterBot.SetLineColor(root.kBlue)
gConfIntervalCenterBot.SetFillColor(root.kBlue)
gConfIntervalCenterBot.SetFillStyle(3005)
gConfIntervalCenterBot.Draw('3 same')

c2.Update()

# ---------------------------------------
# ---------------------------------------
#  top threshold shape
# ---------------------------------------
# ---------------------------------------



c3 = Canvas()

gThresholdShapeTop = root.TGraphErrors(num_points, dThresholdFunction['time'], dThresholdFunction['fd_shape_top'],
                                       np.zeros(num_points), dThresholdFunction['fd_shape_top_err'])
gThresholdShapeTop.SetTitle('Fermi Dirac Shape in Time (top) - run %d' % run_number)
gThresholdShapeTop.GetXaxis().SetTitle('Time')
gThresholdShapeTop.GetXaxis().SetTimeDisplay(1)
gThresholdShapeTop.GetXaxis().SetTimeFormat('%m/%d')
gThresholdShapeTop.GetYaxis().SetTitle('FD Shape (top) [PE]')

fG3 = root.TF1('fG3', 'pol0', dThresholdFunction['time'][0], dThresholdFunction['time'][-1])

fG3.SetLineStyle(9)
fG3.SetLineWidth(1)

gThresholdShapeTop.Draw('ap')
gThresholdShapeTop.Fit('fG3', 'L')

gConfIntervalShapeTop = root.TGraphAsymmErrors(
    *neriX_analysis.create_graph_with_confidence_interval_for_fit(gThresholdShapeTop, root.TVirtualFitter.GetFitter()))
gConfIntervalShapeTop.SetLineColor(root.kBlue)
gConfIntervalShapeTop.SetFillColor(root.kBlue)
gConfIntervalShapeTop.SetFillStyle(3005)
gConfIntervalShapeTop.Draw('3 same')

c3.Update()

# ---------------------------------------
# ---------------------------------------
#  top threshold shape
# ---------------------------------------
# ---------------------------------------



c4 = Canvas()

gThresholdCenterTop = root.TGraphErrors(num_points, dThresholdFunction['time'], dThresholdFunction['fd_center_top'],
                                        np.zeros(num_points), dThresholdFunction['fd_center_top_err'])
gThresholdCenterTop.SetTitle('Fermi Dirac Center in Time (top) - run %d' % run_number)
gThresholdCenterTop.GetXaxis().SetTitle('Time')
gThresholdCenterTop.GetXaxis().SetTimeDisplay(1)
gThresholdCenterTop.GetXaxis().SetTimeFormat('%m/%d')
gThresholdCenterTop.GetYaxis().SetTitle('FD Center (top) [PE]')

fG4 = root.TF1('fG4', 'pol0', dThresholdFunction['time'][0], dThresholdFunction['time'][-1])

fG4.SetLineStyle(9)
fG4.SetLineWidth(1)

gThresholdCenterTop.Draw('ap')
gThresholdCenterTop.Fit('fG4', 'L')

gConfIntervalCenterTop = root.TGraphAsymmErrors(
    *neriX_analysis.create_graph_with_confidence_interval_for_fit(gThresholdCenterTop, root.TVirtualFitter.GetFitter()))
gConfIntervalCenterTop.SetLineColor(root.kBlue)
gConfIntervalCenterTop.SetFillColor(root.kBlue)
gConfIntervalCenterTop.SetFillStyle(3005)
gConfIntervalCenterTop.Draw('3 same')

c4.Update()

neriX_analysis.save_plot(['threshold_results', 'run_%d' % (run_number)], c1, 'fd_shape_bot_run_%d' % run_number)
neriX_analysis.save_plot(['threshold_results', 'run_%d' % (run_number)], c2, 'fd_center_bot_run_%d' % run_number)
neriX_analysis.save_plot(['threshold_results', 'run_%d' % (run_number)], c3, 'fd_shape_top_run_%d' % run_number)
neriX_analysis.save_plot(['threshold_results', 'run_%d' % (run_number)], c4, 'fd_center_top_run_%d' % run_number)

raw_input('Press enter to continue...')
