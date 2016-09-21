import ROOT as root
from rootpy.plotting import Canvas, Hist2D, Hist
import neriX_analysis
import neriX_datasets
import sys, os
import numpy as np
from scipy import stats
from tqdm import tqdm
#from fit_average_thresholds import fit_average_thresholds

root.TVirtualFitter.SetMaxIterations(10000)

def make_string_for_1d_gaussian(electron_number, bAfterMainS2):
    # 0 = mean
    # 1 = width
    # 2 = first peak amplitude
    # 3-(1+num_electrons) = relative peak amplitudes


    string_func = '1./(2*TMath::Pi()*%d*[1]**2.)**0.5*exp(-0.5/%.1f*((x - %.1f*[0])/[1])**2)' % (electron_number, electron_number, electron_number)
    #if bAfterMainS2:
    #	string_func += ' + [%d] + [%d]*exp(-x/[%d])' % (electron_number+4, electron_number+5, electron_number+6)

    if electron_number == 1:
        string_func = '[2]*' + string_func
    else:
        string_func = ('[%d]*[2]*' % (electron_number+1)) + string_func

    return '( %s )' % string_func


def make_string_for_2d_gaussian(electron_number):
    # 0 = bot mean
    # 1 = top mean
    # 2 = bot width
    # 3 = top width
    # 4 = correlation
    # 5 = first peak ampl
    # 6-(3+num_electrons) = relative peak ampl

    string_func = '1./(2*TMath::Pi()*[2]*[3]*%.1f*(1-[4]**2)**0.5)*exp( -0.5/(1-[4]**2)*( (x-%.1f*[0])**2/[2]**2/%.1f + (y-%.1f*[1])**2/[3]**2/%.1f - 2*[4]*(x-%.1f*[0])*(y-%.1f*[1])/[2]/[3]/%.1f ) )' % (electron_number, electron_number, electron_number, electron_number, electron_number, electron_number, electron_number, electron_number)
    if electron_number == 1:
        string_func = '[5]*' + string_func
    else:
        string_func = ('[%d]*[5]*' % (electron_number+4)) + string_func

    return '( %s )' % string_func





# fit s2total spectrum to determine parameters
# that can be fixed during 2d fit
# parameters we want: # electrons, fractional heights, mean_tot
def fit_s2_tot_2d_systematic_study(filename, l_num_electrons_to_check, tot_threshold_high, methods, radial_cut=(0, 18), show_fits=False, fix_fd_threshold=False):

    current_analysis = neriX_analysis.neriX_analysis(filename)

    if type(radial_cut) != type((0,0)):
        neriX_analysis.failure_message('Radial cut must be given as a tuple in the form (inner radius, outer radius).')
        sys.exit()




    #photoionization_window_gate = [150,250] # run 10
    photoionization_window_gate = [100,200] # run 16

    if current_analysis.get_cathode_setting() == 0.345:
        photoionization_window_cathode = [1600, 1800]
    elif current_analysis.get_cathode_setting() == 1.054:
        photoionization_window_cathode = [1450, 1600]
    elif current_analysis.get_cathode_setting() == 2.356:
        photoionization_window_cathode = [1300, 1450]
    elif current_analysis.get_cathode_setting() == 5.500:
        photoionization_window_cathode = [1150, 1310]


    iS2Wdith = 300
    iSamplesBetweenS2 = 200
    iSamplesAfterS2 = 1500

    guessS2BotMean = 16.5
    limitsS2BotMean = [12.0, 19.0]
    guessS2BotWidth = 7
    limitsS2BotWidth = [3, 20]

    guessS2TopMean = 11.5
    limitsS2TopMean = [4.0, 20.0]
    guessS2TopWidth = 7
    limitsS2TopWidth = [2, 19.0]

    guessCorrelation = -0.5
    limitsCorrelation = [-1, 1]

    guessFDTotCenter = 25
    guessFDTotShape = 3
    limitsFDTotCenter = [0, 25]
    limitsFDTotShape = [0.5, 10]

    guessFDTopCenter = 3
    guessFDTopShape = 1
    limitsFDTopCenter = [0, 15]
    limitsFDTopShape = [0.1, 8]

    #guessFDBotCenter = 8
    #guessFDBotShape = 3
    #limitsFDBotCenter = [0, 20]
    #limitsFDBotShape = [1, 10]


    # set FD guesses to weighted average fits
    # and fix in fit if told to do so

    """
    l_time_ranges, d_pars_weighted_avg = fit_average_thresholds(current_analysis.get_run())
    for i in xrange(len(l_time_ranges)-1):
        file_time = neriX_analysis.convert_name_to_unix_time(current_analysis.get_filename_no_ext())
        if file_time > l_time_ranges[i] and file_time < l_time_ranges[i+1]:
            guessFDTotCenter = d_pars_weighted_avg['fd_center_tot']['fits'][i].GetParameter(0)
            guessFDTotShape = d_pars_weighted_avg['fd_shape_tot']['fits'][i].GetParameter(0)
            guessFDTopCenter = d_pars_weighted_avg['fd_center_top']['fits'][i].GetParameter(0)
            guessFDTopShape = d_pars_weighted_avg['fd_shape_top']['fits'][i].GetParameter(0)
    """



    # 50 bins used previously
    s2TopBinning = [30, 0, 60]
    s2BotBinning = [30, 0, 60]

    fS2Max = tot_threshold_high
    fS2BottomMin = 0
    fS2TopMin = 0

    # check to make sure methods are correct
    for method in methods:
        if not method in ['gate_pi', 'cathode_pi', 'misc_e']:
            neriX_analysis.failure_message('Method given (%s) is not implemented in this script.' % method)
            sys.exit()


    # create dictionaries for histograms and fits
    d_histograms = {}
    d_best_fits = {}
    d_num_electrons = {}


    string_to_return = ''


    # --------------------------------------------
    # --------------------------------------------
    # Begin loop through methods
    # --------------------------------------------
    # --------------------------------------------

    for method in methods:

        # --------------------------------------------
        # --------------------------------------------
        # Set branches for each method
        # --------------------------------------------
        # --------------------------------------------

        if method == 'cathode_pi' or method == 'misc_e':
            bot_branch = 'S2sTotBottom[]'# 'ctS2sTotBottom[]'
            top_branch = 'S2sTotTop[]'
            asym_branch = '(ctS2sTotBottom[] - S2sTotTop[])/(ctS2sTotBottom[] + S2sTotTop[])'
            width_branch = 'S2sRightEdge[] - S2sLeftEdge[]'
            peak_branch = 'S2sPeak[]'
            left_edge_branch = 'S2sLeftEdge[]'
            right_edge_branch = 'S2sRightEdge[]'
        elif method == 'gate_pi':
            bot_branch = 'S2sTotBottom[S2Order[0]]'#'ctS2sTotBottom[S2Order[0]]'
            top_branch = 'S2sTotTop[S2Order[0]]'
            asym_branch = '(ctS2sTotBottom[S2Order[0]] - S2sTotTop[S2Order[0]])/(ctS2sTotBottom[S2Order[0]] + S2sTotTop[S2Order[0]])'
            width_branch = 'S2sRightEdge[S2Order[0]] - S2sLeftEdge[S2Order[0]]'
            peak_branch = 'S2sPeak[S2Order[0]]'
            left_edge_branch = 'S2sLeftEdge[S2Order[0]]'
            right_edge_branch = 'S2sRightEdge[S2Order[0]]'


        # --------------------------------------------
        # --------------------------------------------
        # Add cuts for analysis
        # --------------------------------------------
        # --------------------------------------------

        # tot max cut
        current_analysis.add_cut('%s + %s < %f' % (bot_branch, top_branch, fS2Max))

        # bot min cut
        current_analysis.add_cut('%s > %f' % (bot_branch, fS2BottomMin))

        # top min cut
        current_analysis.add_cut('%s > %f' % (top_branch, fS2TopMin))

        # S2 bottom high cut
        current_analysis.add_cut('%s < %f' % (bot_branch, s2BotBinning[2]))

        # S2 top high cut
        current_analysis.add_cut('%s < %f' % (top_branch, s2TopBinning[2]))

        # noisy waveform cut
        current_analysis.add_cut('log10((S1Tot + S2Tot)/(AreaTot - S1Tot - S2Tot)) > 0.')

        # width cut
        current_analysis.add_cut('%s - %s < %d' % (right_edge_branch, left_edge_branch, iS2Wdith))

        # radial cut
        current_analysis.add_radius_cut(*radial_cut)

        if method == 'cathode_pi':
            # time cut for cathode
            current_analysis.add_cut('%s - S2sPeak[0] > %d && %s - S2sPeak[0] < %d' % (peak_branch, photoionization_window_cathode[0], peak_branch, photoionization_window_cathode[1]))


        elif method == 'misc_e':
            # look a few us after main S2 but NOT photoionization of cathode
            current_analysis.add_cut('(%s > (S2sPeak[0] + %d)) && !(%s - S2sPeak[0] > %d && %s - S2sPeak[0] < %d)' % (peak_branch, iSamplesAfterS2, peak_branch, photoionization_window_cathode[0], peak_branch, photoionization_window_cathode[1]))


        elif method == 'gate_pi':
            # time cut for gate
            current_analysis.add_cut('%s - S1sPeak[0] > %d && %s - S1sPeak[0] < %d' % (peak_branch, photoionization_window_gate[0], peak_branch, photoionization_window_gate[1]))

            # make sure that next S2 (typically main one) isn't too close
            current_analysis.add_cut('Alt$(S2sLeftEdge[S2Order[1]],100000) - S2sRightEdge[S2Order[0]] > %i' % (iSamplesBetweenS2))

        #print current_analysis.get_cuts()

        #current_analysis.get_T1().Scan('EventId:%s:%s:%s' % (bot_branch, top_branch, peak_branch), current_analysis.get_cuts())

        # --------------------------------------------
        # --------------------------------------------
        # Fill Histogram
        # --------------------------------------------
        # --------------------------------------------

        d_histograms[method] = Hist2D(*(s2BotBinning+s2TopBinning), name='hS2BottomTop_%s' % (method), title='Gas Gain (%s) - V_{c} %.3f kV' % (method, current_analysis.get_cathode_setting()))
        d_histograms[method].SetStats(0)
        current_analysis.Draw('%s:%s' % (bot_branch, top_branch), hist=d_histograms[method], selection=current_analysis.get_cuts())



        # --------------------------------------------
        # --------------------------------------------
        # Make threshold strings for fit
        # --------------------------------------------
        # --------------------------------------------

        sTotHighThreshold = '(x+y < %f ? 1. : 0.)' % (fS2Max)
        sBotLowThreshold = '(x > %f ? 1. : 0.)' % (fS2BottomMin)
        sBotHighThreshold = '(x < %f ? 1. : 0.)' % (s2BotBinning[2])
        sTopLowThreshold = '(y > %f ? 1. : 0.)' % (fS2TopMin)
        sTopHighThreshold = '(y < %f ? 1. : 0.)' % (s2TopBinning[2])



        # --------------------------------------------
        # --------------------------------------------
        # Loop through number of electron
        # possibilities to find most likely
        # with least free parameters
        # --------------------------------------------
        # --------------------------------------------


        lFits = [0. for i in xrange(len(l_num_electrons_to_check))]
        lLnlikelihoods = [0. for i in xrange(len(l_num_electrons_to_check))]
        lFitsSuccessful = [0. for i in xrange(len(l_num_electrons_to_check))]

        for i, tot_number_electrons in enumerate(l_num_electrons_to_check):
            lGaussianFunctions = [make_string_for_2d_gaussian(electron_number+1) for electron_number in xrange(tot_number_electrons)]
            sGaussianFunctions = ' + '.join(lGaussianFunctions)


            # set parameter names
            par_names = ['mean_bot', 'mean_top', 'width_bot', 'width_top', 'correlation'] + ['p%d_ampl' % (j + 1) for j in xrange(tot_number_electrons)] + ['fd_center_tot', 'fd_shape_tot', 'fd_center_top', 'fd_shape_top']


            #print sGaussianFunctions

            sFDTot = '1./(1+exp(([%d] - x - y)/([%d])))' % (tot_number_electrons+5, tot_number_electrons+6)
            sFDTop = '1./(1+exp(([%d] - y)/([%d])))' % (tot_number_electrons+7, tot_number_electrons+8)
            #sFDBot = '1./(1+exp(([%d] - x)/([%d])))' % (tot_number_electrons+9, tot_number_electrons+10)

            sFitFunction = '(%s)*(%s)*(%s)*(%s)*(%s)*(%s)*(%s)*(%s)' % (sTotHighThreshold, sBotLowThreshold, sTopLowThreshold, sBotHighThreshold, sTopHighThreshold, sGaussianFunctions, sFDTot, sFDTop)



            lFits[i] = root.TF2('fit_%d_electrons_%s' % (tot_number_electrons, method), sFitFunction, *(s2BotBinning[1:]+s2TopBinning[1:]))
            lFits[i].SetContour(5)

            #print sFitFunction



            # --------------------------------------------
            # --------------------------------------------
            # Set parameters and limits
            # --------------------------------------------
            # --------------------------------------------

            # 0 = bot mean
            # 1 = top mean
            # 2 = bot width
            # 3 = top width
            # 4 = correlation
            # 5 = first peak ampl
            # 6-(4+num_electrons) = relative peak ampl

            lFits[i].SetParameter(0, guessS2BotMean)
            lFits[i].SetParameter(1, guessS2TopMean)
            lFits[i].SetParameter(2, guessS2BotWidth)
            lFits[i].SetParameter(3, guessS2TopWidth)
            lFits[i].SetParameter(4, guessCorrelation)
            lFits[i].SetParameter(5, 1000)

            lFits[i].SetParLimits(0, *limitsS2BotMean)
            lFits[i].SetParLimits(1, *limitsS2TopMean)
            lFits[i].SetParLimits(2, *limitsS2BotWidth)
            lFits[i].SetParLimits(3, *limitsS2TopWidth)
            lFits[i].SetParLimits(4, *limitsCorrelation)
            lFits[i].SetParLimits(5, 1, 1e8)

            if tot_number_electrons > 1:
                for j in xrange(tot_number_electrons-1):
                    lFits[i].SetParameter(j+1+5, 0.2)
                    lFits[i].SetParLimits(j+1+5, 0.00, 1)


            if fix_fd_threshold:
                lFits[i].FixParameter(tot_number_electrons+5, guessFDTotCenter)
                lFits[i].FixParameter(tot_number_electrons+6, guessFDTotShape)
                lFits[i].FixParameter(tot_number_electrons+7, guessFDTopCenter)
                lFits[i].FixParameter(tot_number_electrons+8, guessFDTopShape)
            else:
                lFits[i].SetParameter(tot_number_electrons+5, guessFDTotCenter)
                lFits[i].SetParameter(tot_number_electrons+6, guessFDTotShape)
                lFits[i].SetParameter(tot_number_electrons+7, guessFDTopCenter)
                lFits[i].SetParameter(tot_number_electrons+8, guessFDTopShape)
                #lFits[i].SetParameter(tot_number_electrons+9, guessFDBotCenter)
                #lFits[i].SetParameter(tot_number_electrons+10, guessFDBotShape)

                lFits[i].SetParLimits(tot_number_electrons+5, *limitsFDTotCenter)
                lFits[i].SetParLimits(tot_number_electrons+6, *limitsFDTotShape)
                lFits[i].SetParLimits(tot_number_electrons+7, *limitsFDTopCenter)
                lFits[i].SetParLimits(tot_number_electrons+8, *limitsFDTopShape)
                #lFits[i].SetParLimits(tot_number_electrons+9, *limitsFDBotCenter)
                #lFits[i].SetParLimits(tot_number_electrons+10, *limitsFDBotShape)


            for j in xrange(tot_number_electrons+9):
                lFits[i].SetParName(j, par_names[j])


            # --------------------------------------------
            # --------------------------------------------
            # Fit and extract values of interest
            # --------------------------------------------
            # --------------------------------------------

            current_fit_result = d_histograms[method].Fit('fit_%d_electrons_%s' % (tot_number_electrons, method), 'MELSN')


            fitStatus = current_fit_result.CovMatrixStatus()
            if fitStatus != 3:
                #neriX_analysis.failure_message('Fit failed, please adjust guesses and try again.')
                lFitsSuccessful[i] = False
            else:
                #neriX_analysis.success_message('Fit successful, please copy output to appropriate files.')
                lFitsSuccessful[i] = True


            fitter = root.TVirtualFitter.Fitter(d_histograms[method])
            amin = np.asarray([0], dtype=np.float64)
            dum1 = np.asarray([0], dtype=np.float64)
            dum2 = np.asarray([0], dtype=np.float64)
            dum3 = np.asarray([0], dtype=np.int32)
            dum4 = np.asarray([0], dtype=np.int32)

            fitter.GetStats(amin, dum1, dum2, dum3, dum4)
            lLnlikelihoods[i] = -amin/2.



        if len(l_num_electrons_to_check) > 1:
            for i in xrange(len(l_num_electrons_to_check) - 1):
                testStat = -2 * (lLnlikelihoods[i] - lLnlikelihoods[i + 1])
                pValue = 1 - stats.chi2.cdf(testStat, 1)
                indexToUse = i
                if pValue > 0.05:
                    break
        else:
            indexToUse = 0



        d_best_fits[method] = lFits[indexToUse]
        fit_successful = lFitsSuccessful[indexToUse]
        d_num_electrons[method] = l_num_electrons_to_check[indexToUse]
        current_analysis.reset_cuts()

        # remake par names with correct num electrons
        par_names = ['mean_bot', 'mean_top', 'width_bot', 'width_top', 'correlation'] + ['p%d_ampl' % (j + 1) for j in xrange(d_num_electrons[method])] + ['fd_center_tot', 'fd_shape_tot', 'fd_center_top', 'fd_shape_top']


        # get correct uncertainty on mean since we use time correction
        corrected_mean = d_best_fits[method].GetParameter(0)
        uncorrected_mean = corrected_mean*neriX_analysis.get_gain_correction(current_analysis.get_run(), current_analysis.get_timestamp())
        uncorrected_err = d_best_fits[method].GetParError(0)*neriX_analysis.get_gain_correction(current_analysis.get_run(), current_analysis.get_timestamp())
        ctErrMeanBottom = corrected_mean*( (uncorrected_err / uncorrected_mean)**2 + (neriX_analysis.get_gain_correction_err(current_analysis.get_run(), current_analysis.get_timestamp()) / neriX_analysis.get_gain_correction(current_analysis.get_run(), current_analysis.get_timestamp()))**2)**0.5




        sForDictionary = '\'num_electrons\':%d, \'fit_successful\':%s, ' % (d_num_electrons[method], fit_successful)
        #sForDictionary += '\'mean_bot\':%.2f, \'mean_bot_err\':%.2f, ' % (corrected_mean, ctErrMeanBottom)

        print '\nUsing uncorrected error!!!!\n\n'
        sForDictionary += '\'mean_bot\':%.2f, \'mean_bot_err\':%.2f, ' % (corrected_mean, uncorrected_err)
        for i in xrange(1, d_num_electrons[method]+9):
            sForDictionary += '\'%s\':%.5f, \'%s_err\':%.5f, ' % (par_names[i], d_best_fits[method].GetParameter(i), par_names[i], d_best_fits[method].GetParError(i))

        sForDictionary += '\'Chi2\':%.1f, \'NDF\':%d, \'ln(L)\':%.3f' % (
        d_best_fits[method].GetChisquare(), d_best_fits[method].GetNDF(), -amin / 2.)

        if string_to_return == '':
            string_to_return += '#%s\n' % current_analysis.get_filename_no_ext()

        string_to_return += 'gas_gain_results[%d][\'%s\'][(%.2f,%.2f)][\'%s\'] = {%s}\n\n' % (current_analysis.get_run(), method, radial_cut[0], radial_cut[1], current_analysis.get_filename_no_ext(), sForDictionary)



    return d_histograms, d_best_fits, string_to_return



# this function can be used in tandem with the
# above function to check the consistency
# of the results
def compare_methods(d_histograms, d_best_fits):

    l_keys = d_histograms.keys()
    num_methods = len(l_keys)

    c1 = Canvas(500*num_methods, 400)
    if num_methods > 1:
        c1.Divide(num_methods)

    for i, method in enumerate(l_keys):
        c1.cd(i+1)
        c1.cd(i+1).SetLogz()
        d_histograms[method].Draw('colz')
        d_best_fits[method].Draw('same')

    c1.Update()

    raw_input('press enter to continue')




# report on the different methods and print out
# results for user
def summarize_methods(filename, d_histograms, d_best_fits):

    l_keys = d_histograms.keys()
    num_methods = len(l_keys)

    current_analysis = neriX_analysis.neriX_analysis(filename)

    for method in l_keys:
        print '\n\nMethod: %s' % (method)

        corrected_mean = d_best_fits[method].GetParameter(0)
        uncorrected_mean = corrected_mean*neriX_analysis.get_gain_correction(current_analysis.get_run(), current_analysis.get_timestamp())
        uncorrected_err = d_best_fits[method].GetParError(0)*neriX_analysis.get_gain_correction(current_analysis.get_run(), current_analysis.get_timestamp())
        ctErrMeanBottom = corrected_mean*( (uncorrected_err / uncorrected_mean)**2 + (neriX_analysis.get_gain_correction_err(current_analysis.get_run(), current_analysis.get_timestamp()) / neriX_analysis.get_gain_correction(current_analysis.get_run(), current_analysis.get_timestamp()))**2)**0.5

        print 'Bottom mean = %.2f +/- %.2f\n' % (corrected_mean, ctErrMeanBottom)



#lFiles = ['nerix_140917_1010']
#lFiles = ['nerix_140915_1158', 'nerix_140917_1010', 'nerix_140930_0925', 'nerix_140930_1029', 'nerix_140930_1127', 'nerix_140930_1626', 'nerix_141003_1232', 'nerix_141006_0951', 'nerix_141014_1024', 'nerix_141014_1414', 'nerix_141015_1155', 'nerix_141022_1127', 'nerix_141029_1110', 'nerix_141103_1101', 'nerix_141103_1553', 'nerix_141103_1627', 'nerix_141103_1655']


#lFiles = ['nerix_141203_1506', 'nerix_141208_1123', 'nerix_141210_1028','nerix_141217_1041', 'nerix_150106_1100', 'nerix_150114_1407', 'nerix_150128_1158', 'nerix_150128_1327', 'nerix_150128_1454', 'nerix_150128_1622']


lFiles = []

#lFiles.append('nerix_160329_2245')
lFiles.append('nerix_160404_1059')
"""
lFiles.append('nerix_160407_1357')
lFiles.append('nerix_160412_0919')
lFiles.append('nerix_160414_1215')
lFiles.append('nerix_160418_0919')
lFiles.append('nerix_160421_1356')
#lFiles.append('nerix_160425_0918') # missing
lFiles.append('nerix_160428_1124')
#lFiles.append('nerix_160429_1124') # missing
lFiles.append('nerix_160502_0940')

#lFiles.append('nerix_160505_0935') # bad
lFiles.append('nerix_160509_1050')
lFiles.append('nerix_160512_1224')
lFiles.append('nerix_160516_0958')
"""

#lFiles.append('nerix_160524_1140')
#lFiles.append('nerix_160531_1031')
#lFiles.append('nerix_160606_1046')
#lFiles.append('nerix_160614_1054')
#lFiles.append('nerix_160620_1048')
#lFiles.append('nerix_160627_1046')


#lFiles.append('nerix_160705_1049')
#lFiles.append('nerix_160711_0911')
#lFiles.append('nerix_160718_1028')
#lFiles.append('nerix_160725_1039')

"""
lFiles.append('nerix_160728_1046')
lFiles.append('nerix_160801_1036')
lFiles.append('nerix_160808_1141')
"""


# choose whether to look for late S2 or
# for single electrons from photoionization of
# the gate


lNumElectrons = [1, 2, 3, 4, 5]
l_methods = ['gate_pi']
tot_threshold_high = 80

l_radial_cuts = radii_sets = [(0., 0.85)]

"""
d_histograms, d_best_fits, dummy = fit_s2_tot_2d_systematic_study(lFiles[0], lNumElectrons, tot_threshold_high, l_methods, radial_cut=(0., 18.), fix_fd_threshold=False)
print dummy
compare_methods(d_histograms, d_best_fits)
summarize_methods(lFiles[0], d_histograms, d_best_fits)
"""

full_fit_string = ''

for file in lFiles:
    for radial_set in l_radial_cuts:
        print '\n\n\nWorking on %s with radial cut: %s\n\n' % (file, str(radial_set))
        d_histograms, d_best_fits, fit_string = fit_s2_tot_2d_systematic_study(file, lNumElectrons, tot_threshold_high, l_methods, radial_cut=radial_set, fix_fd_threshold=False)
        print fit_string
        compare_methods(d_histograms, d_best_fits)

        full_fit_string += '%s\n\n\n' % fit_string


print '\n\n\n\n\n\n\n'

print full_fit_string

print '\n\n\nCopy and paste into text file\n\n\n'


