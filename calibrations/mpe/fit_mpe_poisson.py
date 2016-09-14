import ROOT as root
from rootpy.plotting import Canvas, Hist
from rootpy.io import File, root_open
import neriX_analysis
import neriX_datasets, neriX_config
import sys, os
import numpy as np
from scipy import stats

#print '\n\n\n\nBatch mode on.\n\n\n\n\n'
#gROOT.SetBatch(True)


def mpe_fitting(filename, run, num_photons, use_ideal=True):

    run_number = int(run)

    if filename[-5:] == '.root':
        filename = filename[:-5]

    sDataPath = '%s/run_%d/%s.root' % (neriX_config.pathToData, run_number, filename)

    sPathToSaveOutput = './results/run_' + str(run_number) + '/' + filename
    aColors = [4, 2, 8, 7, 5, 9]*5
    max_num_events = 15

    bkg_mean_low = -5e5
    bkg_mean_high = 5e5
    bkg_width_low = 1e4
    bkg_width_high = 4e5
    spe_mean_low = 5e5
    spe_mean_high = 2e6
    spe_width_low = 1e5
    spe_width_high = 1.5e6

    mpe_par_guesses = [2, 1.5e5, 1e5, 8e5, 5e5, 5e9]


    par_names = ['poisson_mean', 'mean_bkg', 'width_bkg', 'mean_spe', 'width_spe', 'normalization']# + ['p%d_ampl' % (i + 1) for i in xrange(num_photons)]


    # 50 bins used previously
    mpe_spec_binning = [45, -0.2e6, 3e6]

    file_mpe = File(sDataPath, 'read')

    if use_ideal:
        l_mpe_fit_func = ['[5]/(2*3.14*%d*[4]**2.)**0.5*TMath::Poisson(%d, [0])*exp(-0.5/%.1f*((x - %.1f*[3])/[4])**2)' % (iElectron, iElectron, iElectron, iElectron) for iElectron in xrange(1, num_photons + 1)]
    else:
        l_mpe_fit_func = ['[5]/(2*3.14*([2]**2. + %d*[4]**2.))**0.5*TMath::Poisson(%d, [0])*exp(-0.5*((x - %.1f*[3] - [1])/(%.1f*[4]**2 + [2]**2)**0.5)**2)' % (iElectron, iElectron, iElectron, iElectron) for iElectron in xrange(1, num_photons + 1)]

    h_mpe_spec = Hist(*mpe_spec_binning, name='h_mpe_spec', title='MPE fit - %s' % filename)
    h_mpe_spec.SetMarkerSize(0)

    c1 = Canvas()

    channel = 16
    parameter_to_draw = 'SingleIntegral[%d]' % (channel)
    #parameter_to_draw = 'SinglePeak[%d] + SingleBefore[%d][0] + SingleBefore[%d][1] + SingleAfter[%d][0] + SingleAfter[%d][1]' % (5*(channel,))
    tree_mpe = file_mpe.T0
    tree_mpe.Draw(parameter_to_draw, hist=h_mpe_spec)
    h_mpe_spec.Draw()

    #s_bkg = '[5]/(2*3.14*[2]**2.)**0.5*TMath::Poisson(0, [0])*exp(-0.5*((x - [1])/[2])**2)'
    s_bkg = '[5]/(2*3.14*[2]**2.)**0.5*TMath::Poisson(0, [0])*exp(-0.5*((x - [1])/[2])**2)'
    s_fit_mpe = '(%s) + (%s)' % (s_bkg, ' + '.join(l_mpe_fit_func))
    s_fit_mpe = '(%s)*([1] < [3] ? 1. : 0.)' % (s_fit_mpe)
    print 'Fit function: %s' % s_fit_mpe
    fit_mpe = root.TF1('fit_mpe', s_fit_mpe, *mpe_spec_binning[1:])
    fit_mpe.SetLineColor(46)
    fit_mpe.SetLineStyle(2)
    fit_mpe.SetLineWidth(3)


    h_mpe_spec.GetXaxis().SetTitle('Integrated Charge [e-]')
    h_mpe_spec.GetYaxis().SetTitle('Counts')
    h_mpe_spec.GetYaxis().SetTitleOffset(1.4)
    h_mpe_spec.SetStats(0)

    fit_mpe.SetParLimits(0, 0.8, max_num_events)
    fit_mpe.SetParLimits(1, bkg_mean_low, bkg_mean_high)
    fit_mpe.SetParLimits(2, bkg_width_low, bkg_width_high)
    fit_mpe.SetParLimits(3, spe_mean_low, spe_mean_high)
    fit_mpe.SetParLimits(4, spe_width_low, spe_width_high)
    #fit_mpe.FixParameter(5, 200000)

    for i, guess in enumerate(mpe_par_guesses):
        fit_mpe.SetParameter(i, guess)

    for i in xrange(len(par_names)):
        fit_mpe.SetParName(i, par_names[i])

    #for photon in xrange(num_photons):
    #    fit_mpe.SetParLimits(5 + photon, 0, max_num_events)

    fitResult = h_mpe_spec.Fit('fit_mpe', 'SL')


    # draw individual peaks
    s_gaussian = '[0]*exp(-0.5/%.1f*((x - %.1f*[1])/[2])**2)'
    l_functions = []
    l_individual_integrals = [0. for i in xrange(num_photons+1)]
    for i in xrange(num_photons + 1):
        l_functions.append(root.TF1('peak_%d' % i, '[0]*exp(-0.5*((x - [1])/[2])**2)', *mpe_spec_binning[1:]))

        # set parameters
        if i == 0:
            ampl = fit_mpe.GetParameter(5)*root.TMath.Poisson(0, fit_mpe.GetParameter(0))
            mean = fit_mpe.GetParameter(1)
            width = fit_mpe.GetParameter(2)
            if width > 0:
                ampl /= (2*3.14*width**2.)**0.5
            l_functions[i].SetParameters(ampl, mean, width)
        else:
            ampl = fit_mpe.GetParameter(5)*root.TMath.Poisson(i, fit_mpe.GetParameter(0))
            if use_ideal:
                mean = fit_mpe.GetParameter(3) * i
                width = fit_mpe.GetParameter(4)*i**0.5
            else:
                mean = fit_mpe.GetParameter(3)*i + fit_mpe.GetParameter(1)
                width = (fit_mpe.GetParameter(4)**2*i + fit_mpe.GetParameter(2)**2)**0.5

            if width > 0:
                ampl /= (2*3.14*width**2.)**0.5

            l_functions[i].SetParameters(ampl, mean, width)

        l_individual_integrals[i] = ampl*width*(2*3.1415)**0.5
        l_functions[i].SetLineColor(aColors[i])
        l_functions[i].Draw('same')

    """
    print np.asarray(l_individual_integrals) / sum(l_individual_integrals)

    mean_from_spe_bkg = l_individual_integrals[1] / l_individual_integrals[0]
    print 'Mean from SPE and bkg peak: %.2f' % mean_from_spe_bkg
    if num_photons > 2:
        print 'P(2) / P(0) using above mean = %.2f' % (mean_from_spe_bkg**2/2.)
        print 'P(2) / P(0) from data = %.2f' % (l_individual_integrals[2] / l_individual_integrals[0])
    """
    c1.Update()

    fitStatus = fitResult.CovMatrixStatus()
    if fitStatus != 3:
        neriX_analysis.failure_message('Fit failed, please adjust guesses and try again.')
        fit_successful = False
    else:
        neriX_analysis.success_message('Fit successful, please copy output to appropriate files.')
        fit_successful = True

    #if not os.path.exists(sPathToSaveOutput):
    #    os.makedirs(sPathToSaveOutput)

    fitter = root.TVirtualFitter.Fitter(fit_mpe)
    amin = np.asarray([0], dtype=np.float64)
    dum1 = np.asarray([0], dtype=np.float64)
    dum2 = np.asarray([0], dtype=np.float64)
    dum3 = np.asarray([0], dtype=np.int32)
    dum4 = np.asarray([0], dtype=np.int32)

    fitter.GetStats(amin, dum1, dum2, dum3, dum4)


    print '\n\namin for %d photons: %f\n\n' % (num_photons, amin)


    raw_input('Press enter to continue...')
    return (0,0,0)

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

    file_mpe.close()
    del current_analysis
    return string_to_return, -amin / 2., forTextFile


lFiles = ['nerix_160407_1533']

#lNumPhotons = [2, 3, 4, 5, 6]
lNumPhotons = [10, 11, 12, 13, 14]

sForFile = ''
sForTextFile = ''

for file in lFiles:
    lLnlikelihoods = [0 for i in xrange(len(lNumPhotons))]
    lStrings = [0 for i in xrange(len(lNumPhotons))]
    for i, num_photons in enumerate(lNumPhotons):
        print '\n\n\nStarting %s with %d electrons.\n\n\n' % (file, lNumPhotons[i])
        lStrings[i], lLnlikelihoods[i], forTextFile = mpe_fitting(file, 16, num_photons, use_ideal=False)

    # find first step with p-value > 0.05 and take previous

    # ex:
    # 2->3: p < 0.05
    # 3->4: p > 0.05 - STOP!
    # use 3 electrons

    for i in xrange(len(lNumPhotons) - 1):
        testStat = -2 * (lLnlikelihoods[i] - lLnlikelihoods[i + 1])
        pValue = 1 - stats.chi2.cdf(testStat, 1)
        indexToUse = i
        if pValue > 0.05:
            break

    # run one more time to save image of "correct"
    # number of electrons
    dummy1, dummy2, forTextFile = mpe_fitting(file, 16, num_photons, use_ideal=False)

    sForTextFile += forTextFile

    sSingleFile = lStrings[indexToUse]
    sForFile += '# %s\n%s\n\n\n' % (file, sSingleFile)

print sForFile

print '\n\n\nCopy and paste into text file\n\n\n'

print sForTextFile
