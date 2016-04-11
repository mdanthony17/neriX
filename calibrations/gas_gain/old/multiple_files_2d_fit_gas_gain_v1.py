from ROOT import TChain, TCut, TH1F, TH2F, TF1, TF2, TCanvas, gPad, TGaxis, TLatex, gStyle, TPaletteAxis, \
    TVirtualFitter, gROOT
from rootpy.plotting import Canvas
import neriX_analysis
import neriX_datasets
import sys, os
import numpy as np
from scipy import stats

print '\n\n\n\nBatch mode on.\n\n\n\n\n'
gROOT.SetBatch(True)


def gas_gain_fitting(filename, num_electrons):
    current_analysis = neriX_analysis.neriX_analysis(filename)
    # print 'Bypassing file and doing all!'
    # current_analysis = neriX_analysis.neriX_analysis(neriX_analysis.pull_all_files_given_parameters(11, 4.500, 1.054, -1), -1, 1.054, 4.5)

    sPathToSaveOutput = './results/run_' + str(current_analysis.get_run()) + '/' + str(
        current_analysis.get_filename()[:-5])

    aColors = [4, 2, 8, 7, 5, 9]

    iNumElectrons = num_electrons

    mean_bot_guess = 16.5
    width_bot_guess = 6.5

    bot_mean_low = 10.
    bot_mean_high = 19.

    bot_width_low = 4.
    bot_width_high = 10.

    #s2_parameter = 'S2sTotBottom'
    s2_parameter = 'ctS2sTotBottom'

    TVirtualFitter.SetMaxIterations(30000)

    par_names = ['mean_bot', 'width_bot'] + ['p%d_ampl' % (i + 1) for i in xrange(iNumElectrons)] + ['fd_center_tot',
                                                                                                     'fd_shape_tot', 'fd_center_top', 'fd_shape_top',
                                                                                                     'mean_top',
                                                                                                     'width_top']

    #fd_fixed = True
    fd_fixed = False

    print '\n\nThreshold function fixed: %s\n\n' % fd_fixed

    if fd_fixed:
        if current_analysis.get_run() == 10:
            fd_center_tot = 9.66
            fd_shape_tot = 2.49
            fd_center_top = 9.66
            fd_shape_top = 2.49
        elif current_analysis.get_run() == 11:
            fd_center_tot = 9.44
            fd_shape_tot = 2.68
            fd_center_top = 9.66
            fd_shape_top = 2.49

    fS2Max = 200
    fS2BottomMin = 0
    fS2TopMin = 0
    # iSampleWindow = [115,140] # run 15
    #iSampleWindow = [100, 200]  # after KNF
    iSampleWindow = [150,250] # run 10
    iSamplesBetweenS2 = 100
    iS2Wdith = 400
    iCoinPMTs = 3

    # 50 bins used previously
    s2TopBinning = [50, 0, 100]
    s2BotBinning = [50, 0, 100]

    #s2_order = ''
    s2_order = '0'

    # implement cuts
    # total S2 cut
    current_analysis.add_cut('S2sTot[S2Order[%s]] < %f' % (s2_order, fS2Max))

    # S2 bottom low cut
    current_analysis.add_cut('%s[S2Order[%s]] > %f' % (s2_parameter, s2_order, fS2BottomMin))

    # S2 top low cut
    current_analysis.add_cut('S2sTotTop[S2Order[%s]] > %f' % (s2_order, fS2TopMin))

    # coincidence cut
    # current_analysis.add_cut('S2sCoin[S2Order[0]] > %i'%iCoinPMTs)


    # sample range after S1
    #print '\n\nNOT USING GATE PHOTOIONIZATION\n\n'
    current_analysis.add_cut('S2sPeak[S2Order[%s]] > (S1sPeak[0] + %i) && S2sPeak[S2Order[%s]] < (S1sPeak[0] + %i)' % (s2_order, iSampleWindow[0], s2_order, iSampleWindow[1]))
    #current_analysis.add_cut('S2sPeak[S2Order[%s]] - S2sPeak[0] > 3000' % s2_order)

    # noise cut
    current_analysis.add_cut('log10((S1Tot + S2Tot)/(AreaTot - S1Tot - S2Tot)) > 0.')

    # don't take S2s with another shortly after
    #print 'Must re-add distance cut'
    current_analysis.add_cut('Alt$(S2sLeftEdge[S2Order[1]],100000) - S2sRightEdge[S2Order[0]] > %i' % (iSamplesBetweenS2))

    # S2 width cut
    current_analysis.add_cut('S2sRightEdge[S2Order[%s]] - S2sLeftEdge[S2Order[%s]] < %i' % (s2_order, s2_order, iS2Wdith))

    # make sure S2Order[0] is not largest
    #current_analysis.add_cut('S2sPeak[S2Order[%s]] != S2sPeak[0]' % s2_order)

    current_analysis.set_event_list()

    aS2sBottom = ['[%d]*exp(-0.5/%.1f*((x - %.1f*[0])/[1])**2)' % (iElectron + 1, iElectron, iElectron) for iElectron in
                  range(1, iNumElectrons + 1)]
    aS2sTop = ['exp(-0.5/%.1f*((y - %.1f*[%d])/[%d])**2)' % (iElectron, iElectron, iNumElectrons + 6, iNumElectrons + 7)
               for iElectron in range(1, iNumElectrons + 1)]
    sThreshTopBottom = '1./(1+exp(([%d] - x - y)/([%d])))' % (iNumElectrons + 2, iNumElectrons + 3)
    sThreshTop = '1./(1+exp(([%d] - x - y)/([%d])))' % (iNumElectrons + 4, iNumElectrons + 5)

    S2TopBottom = TH2F('S2TopBottom', 'Gas Gain - V_{c} %.3f kV' % current_analysis.get_cathode_setting(),
                       *(s2BotBinning + s2TopBinning))
    S2Top = TH1F('S2Top', 'Gas Gain, Top Only - V_{c} %.3f kV' % current_analysis.get_cathode_setting(), *s2TopBinning)
    S2Bottom = TH1F('S2Bottom', 'Gas Gain, Bottom Only - V_{c} %.3f kV' % current_analysis.get_cathode_setting(),
                    *s2BotBinning)

    c1 = Canvas()

    # draw 2d plot with contour
    c1.cd(1)
    gPad.SetLogz()
    gPad.SetRightMargin(0.12)
    current_analysis.Draw('%s[S2Order[0]]:S2sTotTop[S2Order[0]]' % s2_parameter, hist=S2TopBottom)
    S2TopBottom.Draw('colz')

    aS2sComb = ['(%s)*(%s)' % (aS2sBottom[i], aS2sTop[i]) for i in range(iNumElectrons)]
    FitS2TopBottom = TF2('FitS2TopBottom', '(x > 0 ? 1. : 0.)*(y > 0 ? 1. : 0.)*(%s)*(%s)*(%s)' % (sThreshTopBottom, sThreshTop, ' + '.join(aS2sComb)), 0, 100,
                         0, 100)
    # print '(%s)*(%s)*(%s)'%(sThreshBottom,sThreshTop,' + '.join(aS2sComb))
    S2TopBottom.GetXaxis().SetTitle('%s[S2Order[0]]' % s2_parameter)
    S2TopBottom.GetYaxis().SetTitle('S2sTotTop[S2Order[0]]')
    S2TopBottom.GetYaxis().SetTitleOffset(1.4)
    S2TopBottom.SetStats(0)
    FitS2TopBottom.SetParLimits(0, bot_mean_low, bot_mean_high)
    FitS2TopBottom.SetParLimits(1, bot_width_low, bot_width_high)
    FitS2TopBottom.SetParLimits(iNumElectrons + 2, 0.1, 40)
    FitS2TopBottom.SetParLimits(iNumElectrons + 3, 0, 5)
    FitS2TopBottom.SetParLimits(iNumElectrons + 4, 0.1, 10)
    FitS2TopBottom.SetParLimits(iNumElectrons + 5, 0, 5)
    FitS2TopBottom.SetParLimits(iNumElectrons + 6, 5, 35)
    FitS2TopBottom.SetParLimits(iNumElectrons + 7, 2, 20)

    for i in xrange(len(par_names)):
        FitS2TopBottom.SetParName(i, par_names[i])

    FitS2TopBottom.SetParameter(0, mean_bot_guess)  # mean
    FitS2TopBottom.SetParameter(1, width_bot_guess)  # width

    # set amplitudes
    for iElectron in range(1, iNumElectrons + 1):
        FitS2TopBottom.SetParameter(iElectron + 1, 200 / iElectron)

    if fd_fixed:
        FitS2TopBottom.FixParameter(iNumElectrons + 2, fd_center_tot)
        FitS2TopBottom.FixParameter(iNumElectrons + 3, fd_shape_tot)
        FitS2TopBottom.FixParameter(iNumElectrons + 4, fd_center_top)
        FitS2TopBottom.FixParameter(iNumElectrons + 5, fd_shape_top)
    else:
        FitS2TopBottom.SetParameter(iNumElectrons + 2, 6)
        FitS2TopBottom.SetParameter(iNumElectrons + 3, 2)
        FitS2TopBottom.SetParameter(iNumElectrons + 4, 2)
        FitS2TopBottom.SetParameter(iNumElectrons + 5, 0.1)
    FitS2TopBottom.SetParameter(iNumElectrons + 6, 15)
    FitS2TopBottom.SetParameter(iNumElectrons + 7, 7)


    FitS2TopBottom.SetContour(15)
    fitResult = S2TopBottom.Fit('FitS2TopBottom', 'SL')


    c1.Update()

    fitStatus = fitResult.CovMatrixStatus()
    if fitStatus != 3:
        neriX_analysis.failure_message('Fit failed, please adjust guesses and try again.')
        fit_successful = False
    else:
        neriX_analysis.success_message('Fit successful, please copy output to appropriate files.')
        fit_successful = True

    if not os.path.exists(sPathToSaveOutput):
        os.makedirs(sPathToSaveOutput)

    fitter = TVirtualFitter.Fitter(S2TopBottom)
    amin = np.asarray([0], dtype=np.float64)
    dum1 = np.asarray([0], dtype=np.float64)
    dum2 = np.asarray([0], dtype=np.float64)
    dum3 = np.asarray([0], dtype=np.int32)
    dum4 = np.asarray([0], dtype=np.int32)

    fitter.GetStats(amin, dum1, dum2, dum3, dum4)

    sFileName = current_analysis.get_filename_no_ext()

    c1.SaveAs(sPathToSaveOutput + '/gas_gain_' + sFileName + '.png')
    c1.SaveAs(sPathToSaveOutput + '/gas_gain_' + sFileName + '.C')

    OutputFile = open(sPathToSaveOutput + '/gas_gain_' + sFileName[:-5] + '.txt', 'w')
    OutputFile.write(
        '#anode\tcathode\t\ttop_eff_thresh\ttop_eff_thresh_err\ttop_eff_rolloff\ttop_eff_rolloff_err\ttop_mu\ttop_mu_err\ttop_width\ttop_width_err\tbottom_eff_thresh\tbottom_eff_thresh_err\tbottom_eff_rolloff\tbottom_eff_rolloff_err\tbottom_mu\tbottom_mu_err\tbottom_width\tbottom_width_err\n')
    OutputFile.write('%f\t%f\t' % (neriX_datasets.run_files[current_analysis.get_run()][sFileName + '.root'][0],
                                   neriX_datasets.run_files[current_analysis.get_run()][sFileName + '.root'][1]))
    OutputFile.write(
        '%f\t%f\t' % (FitS2TopBottom.GetParameter(iNumElectrons + 6), FitS2TopBottom.GetParError(iNumElectrons + 6)))
    OutputFile.write(
        '%f\t%f\t' % (FitS2TopBottom.GetParameter(iNumElectrons + 7), FitS2TopBottom.GetParError(iNumElectrons + 7)))
    OutputFile.write(
        '%f\t%f\t' % (FitS2TopBottom.GetParameter(iNumElectrons + 4), FitS2TopBottom.GetParError(iNumElectrons + 4)))
    OutputFile.write(
        '%f\t%f\t' % (FitS2TopBottom.GetParameter(iNumElectrons + 5), FitS2TopBottom.GetParError(iNumElectrons + 5)))
    OutputFile.write(
        '%f\t%f\t' % (FitS2TopBottom.GetParameter(iNumElectrons + 2), FitS2TopBottom.GetParError(iNumElectrons + 2)))
    OutputFile.write(
        '%f\t%f\t' % (FitS2TopBottom.GetParameter(iNumElectrons + 3), FitS2TopBottom.GetParError(iNumElectrons + 3)))
    OutputFile.write('%f\t%f\t' % (FitS2TopBottom.GetParameter(0), FitS2TopBottom.GetParError(0)))
    OutputFile.write('%f\t%f' % (FitS2TopBottom.GetParameter(1), FitS2TopBottom.GetParError(1)))
    OutputFile.close()

    # make string for dictionary
    sForDictionary = '\'num_electrons\':%d, \'fd_fixed\':%s, \'fit_successful\':%s, ' % (
    num_electrons, fd_fixed, fit_successful)
    for i in xrange(len(par_names)):
        sForDictionary += '\'%s\':%.5f, \'%s_err\':%.5f, ' % (
        par_names[i], FitS2TopBottom.GetParameter(i), par_names[i], FitS2TopBottom.GetParError(i))

    sForDictionary += '\'Chi2\':%.1f, \'NDF\':%d, \'ln(L)\':%.3f' % (
    FitS2TopBottom.GetChisquare(), FitS2TopBottom.GetNDF(), -amin / 2.)

    string_to_return = 'gas_gain_results[%d][\'%s\'] = {%s}' % (
    current_analysis.get_run(), current_analysis.get_filename_no_ext(), sForDictionary)

    forTextFile = ''
    if fit_successful:
        forTextFile = '%s\t%d\t%.3f\t%.3f\t%.2f\t%.2f\t%.2f\t%.2f\n' % (
        current_analysis.get_filename_no_ext(), current_analysis.get_timestamp(), current_analysis.get_anode_setting(),
        current_analysis.get_cathode_setting(), FitS2TopBottom.GetParameter(0), FitS2TopBottom.GetParError(0),
        FitS2TopBottom.GetParameter(1), FitS2TopBottom.GetParError(1))

    del current_analysis
    return string_to_return, -amin / 2., forTextFile


lFiles = ['nerix_140917_1010']
#lFiles = ['nerix_140915_1158', 'nerix_140917_1010', 'nerix_140924_1007', 'nerix_140924_1158', 'nerix_140929_1115', 'nerix_140929_1212', 'nerix_140930_0925', 'nerix_140930_1029', 'nerix_140930_1127', 'nerix_140930_1626', 'nerix_140930_1626', 'nerix_141003_1232', 'nerix_141006_0951', 'nerix_141014_1024', 'nerix_141014_1414', 'nerix_141015_1155', 'nerix_141022_1127', 'nerix_141029_1110', 'nerix_141103_1101', 'nerix_141103_1553', 'nerix_141103_1627', 'nerix_141103_1655']


#lFiles = ['nerix_141203_1506', 'nerix_141208_1123', 'nerix_141210_1028', 'nerix_141217_1041', 'nerix_150106_1100', 'nerix_150106_1137', 'nerix_150106_1234', 'nerix_150106_1404', 'nerix_150114_1407', 'nerix_150128_1158', 'nerix_150128_1327', 'nerix_150128_1454', 'nerix_150128_1622', 'nerix_150204_1210', 'nerix_150210_1210']


lNumElectrons = [2, 3, 4, 5]

sForFile = ''
sForTextFile = ''

for file in lFiles:
    lLnlikelihoods = [0 for i in xrange(len(lNumElectrons))]
    lStrings = [0 for i in xrange(len(lNumElectrons))]
    for i, num_electrons in enumerate(lNumElectrons):
        print '\n\n\nStarting %s with %d electrons.\n\n\n' % (file, lNumElectrons[i])
        lStrings[i], lLnlikelihoods[i], forTextFile = gas_gain_fitting(file, num_electrons)

    # find first step with p-value > 0.05 and take previous

    # ex:
    # 2->3: p < 0.05
    # 3->4: p > 0.05 - STOP!
    # use 3 electrons

    for i in xrange(len(lNumElectrons) - 1):
        testStat = -2 * (lLnlikelihoods[i] - lLnlikelihoods[i + 1])
        pValue = 1 - stats.chi2.cdf(testStat, 1)
        indexToUse = i
        if pValue > 0.05:
            break

    # run one more time to save image of "correct"
    # number of electrons
    dummy1, dummy2, forTextFile = gas_gain_fitting(file, lNumElectrons[indexToUse])

    sForTextFile += forTextFile

    sSingleFile = lStrings[indexToUse]
    sForFile += '# %s\n%s\n\n\n' % (file, sSingleFile)

print sForFile

print '\n\n\nCopy and paste into text file\n\n\n'

print sForTextFile
