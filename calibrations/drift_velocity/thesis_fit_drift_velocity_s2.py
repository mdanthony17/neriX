import ROOT as root
import neriX_analysis

import os, math, sys
from array import array
from rootpy.plotting import Hist
import numpy as np
import math




def fit_drift_velocity(filename, batch_mode=False):


    #--------------- Start Parameters to Change ----------------

    sampleNumBins = 300
    sampleMin = 0.
    sampleMax = 15.

    parameterToDraw = '(S2sPeak[]-S2sPeak[0])/100.'

    #--------------- End Parameters to Change ----------------


    if batch_mode:
        root.gROOT.SetBatch(True)

    current_analysis = neriX_analysis.neriX_analysis(filename)


    anodeVoltage = current_analysis.get_anode_setting()
    cathodeVoltage = current_analysis.get_cathode_setting()
    fileNameWithoutExt = current_analysis.get_filename()[:-5]

    # S2 cuts
    #current_analysis.add_cut('(S1sTot[0]>0 && S1sTot[0] < 5000)')
    current_analysis.add_dt_cut(0, 20)
    current_analysis.add_cut('(S2sTot[1]/S2sTot[0])<0.02')
    #current_analysis.add_cut('NbS2Peaks < 3')
    current_analysis.add_cut('S2sPeak[] != S2sPeak[0]')

    # S1 cuts
    #current_analysis.add_cut('S2sPeak[] < (S1sPeak[0] + 250)')
    #current_analysis.add_cut('S2sPeak[] != S2sPeak[0]')
    #current_analysis.add_cut('S2sLowWidth[] < 150')
    #current_analysis.add_cut('(S2sTot[]/S2sTot[0])<0.02')
    #current_analysis.add_xs1asym_cut()
    #current_analysis.add_xs2asym_cut()
    #current_analysis.add_radius_cut(0, 0.5)
    #current_analysis.add_cut('NbS2Peaks > 4')


    current_analysis.set_event_list()

    #current_analysis.get_T1().Scan('EventId:%s:S2sLowWidth[]' % parameterToDraw, '%s > 0 && %s < 2' % (parameterToDraw, parameterToDraw))


    c1 = root.TCanvas('c1','c1',640,480)

    hDriftVelocity = Hist(sampleNumBins, sampleMin, sampleMax, name='hDriftVelocity', title='hDriftVelocity', drawstyle='hist')
    current_analysis.Draw(parameterToDraw, hist=hDriftVelocity, selection=current_analysis.get_cuts())
    #hDriftVelocity.SetTitle('Drift Velocity: Anode ' + str(anodeVoltage) + ' kV, Cathode ' + str(cathodeVoltage) + ' kV')
    hDriftVelocity.GetXaxis().SetTitle('Drift Time [us]')
    hDriftVelocity.GetYaxis().SetTitleOffset(1.3)
    hDriftVelocity.GetYaxis().SetTitle('Counts')
    hDriftVelocity.SetStats(0)
    hDriftVelocity.SetTitle('')
    hDriftVelocity.SetMarkerSize(0)
    hDriftVelocity.SetLineColor(root.kCyan-1)

    hDriftVelocity.Draw('')


    # search for peaks using TSpectrum
    lPeakParameters = [[], []]
    fitDistanceFromPeakGate = 0.25
    fitDistanceFromPeakCathode = 1.0



    spec2 = root.TSpectrum(2)
    spec2.Search(hDriftVelocity, 2, 'nobackground new goff', 0.05)

    bPeaks = spec2.GetPositionX()
    bPeaks.SetSize(2) # two elements in buffer
    lPeaks = np.array(bPeaks, 'f')
    lPeaks = lPeaks[::-1]

    lPeaks.sort()

    #integral = hDriftVelocity.Integral(hDriftVelocity.FindBin(3), hDriftVelocity.FindBin(13))
    #print '\nIntegral between 3 and 13: %d\n' % integral


    c1.Update()

    """
    print '\nNow select peaks in the spectrum:\n\n'

    responseLeft = -1
    responseRight = -1

    while responseLeft < sampleMin or responseLeft > sampleMax:
        responseLeft = float(raw_input('Please approximate the peak from the gate (left peak): '))

    while responseRight < sampleMin or responseLeft > sampleMax:
        responseRight = float(raw_input('Please approximate the peak from the cathode (right peak): '))

    lPeaks = [responseLeft, responseRight]
    """

    #print lPeaks
    if len(lPeaks) == 2:
        for (i, lPeakToFill) in enumerate(lPeakParameters):
            if i == 1:
                leftEdge = lPeaks[i] - fitDistanceFromPeakGate
                rightEdge = lPeaks[i] + fitDistanceFromPeakGate
            else:
                leftEdge = lPeaks[i] - fitDistanceFromPeakCathode
                rightEdge = lPeaks[i] + fitDistanceFromPeakCathode

            if leftEdge < sampleMin:
                leftEdge = sampleMin
            if rightEdge > sampleMax:
                rightEdge = sampleMax

            lPeakToFill.append(leftEdge)
            lPeakToFill.append(rightEdge)
    else:
        lPeakParameters = [[1., 1.], [10., 20.]]

    # list of fits: lFits = [ [fTrialFirstPeak, fFirstPeak], [fTrialSecondPeak, fSecondPeak] ]
    lFits = [[], []]
    colors = [2, 4]
    lParameters = [] # [mean, sigma]
    lParametersError = []


    for i in xrange(0, 2):
        lFits[i].append(root.TF1('trialFit', 'gaus', lPeakParameters[i][0], lPeakParameters[i][1]))
        #if i == 1:
            #print lFits[i][0]
            #lFits[i][0].SetParameter(1, 1.5)
            #lFits[i][0].SetParameter(2, 0.25)
        hDriftVelocity.Fit(lFits[i][0], 'NRQ+')

        leftFitEdge = lFits[i][0].GetParameter(1) - 1.5*lFits[i][0].GetParameter(2)
        rightFitEdge = lFits[i][0].GetParameter(1) + 1.5*lFits[i][0].GetParameter(2)

        if leftFitEdge < sampleMin:
            leftFitEdge = sampleMin
        if rightFitEdge > sampleMax:
            rightFitEdge = sampleMax


        lFits[i].append(root.TF1('realFit', 'gaus', leftFitEdge, rightFitEdge))
        lFits[i][1].SetLineColor(colors[i])
        #lFits[i][1].SetLineWidth(1.5)
        hDriftVelocity.Fit(lFits[i][1], 'RQ+')
        #lFits[i][1].Draw('same')

        lParameters.append([lFits[i][1].GetParameter(1), lFits[i][1].GetParameter(2)])
        lParametersError.append([lFits[i][1].GetParError(1), lFits[i][1].GetParError(2)])



    mean1 = lParameters[0][0]
    mean1_e = lParametersError[0][0]
    sigma1 = lParameters[0][1]
    sigma1_e = lParametersError[0][1]
    mean2 = lParameters[1][0]
    mean2_e = lParametersError[1][0]
    sigma2 = lParameters[1][1]
    sigma2_e = lParametersError[1][1]

    drift_time = math.fabs(mean2-mean1)
    drift_time_err = (mean1_e**2 + mean2_e**2)**0.5 # mean1_e + mean2_e
    drift_velocity = math.fabs(23.39/(mean2-mean1))
    drift_velocity_err = drift_velocity * math.fabs(drift_time_err/drift_time) #(23.39/(mean2-mean1))*(mean2_e+mean1_e)/(mean2-mean1)


    sFitInfo1 = 't_{1} = %.3f +/- %.3f #mus, #sigma_{1} = %.3f +/- %.3f #mus' % (mean1,mean1_e,sigma1,sigma1_e)
    sFitInfo2 = 't_{2} = %.3f +/- %.3f #mus, #sigma_{2} = %.3f +/- %.3f #mus' % (mean2,mean2_e,sigma2,sigma2_e)
    sDtInfo = '#Delta t = %.4f +/- %.4f #mus' % (drift_time, drift_time_err)
    sVdInfo = 'v_{d} = %.4f +/- %.4f mm/#mus' % (drift_velocity, drift_velocity_err)

    pt1 = root.TPaveText(.2,.8,.8,.85,'blNDC')
    text1 = pt1.AddText(sFitInfo1)
    #pt1.SetTextColor(2)
    pt1.SetFillStyle(0)
    pt1.SetBorderSize(0)
    pt1.Draw('same')
    c1.Modified()

    pt2 = root.TPaveText(.2,.75,.8,.8,'blNDC')
    text2 = pt2.AddText(sFitInfo2)
    #pt2.SetTextColor(4)
    pt2.SetFillStyle(0)
    pt2.SetBorderSize(0)
    pt2.Draw('same')
    c1.Modified()

    pt3 = root.TPaveText(.2,.7,.8,.75,'blNDC')
    text3 = pt3.AddText(sDtInfo)
    pt3.SetTextColor(1)
    pt3.SetFillStyle(0)
    pt3.SetBorderSize(0)
    pt3.Draw('same')
    c1.Modified()

    pt4 = root.TPaveText(.2,.65,.8,.7,'blNDC')
    text4 = pt4.AddText(sVdInfo)
    pt4.SetTextColor(1)
    pt4.SetFillStyle(0)
    pt4.SetBorderSize(0)
    pt4.Draw('same')
    c1.Modified()
    
    c1.Update()

    if not batch_mode:
        raw_input('Please press enter to continue...')

    dParametersToPrint = {'start_time':neriX_analysis.convert_name_to_unix_time(filename), 'run':current_analysis.get_run(), 'anode':anodeVoltage, 'cathode':cathodeVoltage, 'degree':current_analysis.get_degree_setting(), 'mean_gate':mean1, 'mean_gate_err':mean1_e, 'width_gate':sigma1, 'width_gate_err':sigma1_e, 'mean_cathode':mean2, 'mean_cathode_err':mean2_e, 'width_cathode':sigma2, 'width_cathode_err':sigma2_e, 'drift_time':drift_time, 'drift_time_err':drift_time_err, 'drift_velocity':drift_velocity, 'drift_velocity_err':drift_velocity_err}

    s_to_return = ''

    s_to_return += '# ' + str(current_analysis.get_filename_no_ext()) + '\n'
    s_to_return += 'd_drift_velocity[\'' + str(filename) + '\'] = ' + str(dParametersToPrint)
    s_to_return += '\n\n'


    neriX_analysis.save_plot(['results', 'run_%d' % current_analysis.get_run(), current_analysis.get_filename_no_ext()], c1, 'thesis_drift_velocity_%s' % current_analysis.get_filename_no_ext(), batch_mode=batch_mode)

    del c1

    return s_to_return


if __name__ == '__main__':

    #lFiles = ['nerix_160329_2245', 'nerix_160330_0656', 'nerix_160330_0918', 'nerix_160330_1037', 'nerix_160330_1159']
    #lFiles = ['nerix_160404_1059', 'nerix_160404_1204', 'nerix_160404_1232', 'nerix_160404_1259', 'nerix_160404_1325', 'nerix_160404_1350']
    #lFiles = ['nerix_160407_1357', 'nerix_160411_0612', 'nerix_160411_0644', 'nerix_160411_0712', 'nerix_160411_0739', 'nerix_160411_0925']
    #lFiles = ['nerix_160412_0919', 'nerix_160414_1215', 'nerix_160418_0919', 'nerix_160418_1026', 'nerix_160418_1052', 'nerix_160418_1120', 'nerix_160418_1145', 'nerix_160418_1210']
    #lFiles = ['nerix_160421_1356', 'nerix_160425_1206', 'nerix_160425_1234', 'nerix_160425_1327', 'nerix_160425_1355', 'nerix_160425_1442'] # missing 0425 gas gain
    #lFiles = ['nerix_160428_1124', 'nerix_160502_0940', 'nerix_160502_1059', 'nerix_160502_1134', 'nerix_160502_1208', 'nerix_160502_1234', 'nerix_160502_1302'] # missing 0429 gas gain
    #lFiles = ['nerix_160505_0935', 'nerix_160509_1050', 'nerix_160509_1157', 'nerix_160509_1226', 'nerix_160509_1252', 'nerix_160509_1320', 'nerix_160509_1350']
    #lFiles = ['nerix_160512_1224', 'nerix_160516_0958', 'nerix_160516_1226', 'nerix_160516_1255', 'nerix_160516_1322', 'nerix_160516_1351', 'nerix_160516_1418']
    #lFiles = ['nerix_160523_1215', 'nerix_160523_1242', 'nerix_160523_1308', 'nerix_160523_1337', 'nerix_160523_1405']
    #lFiles = ['nerix_160524_1140', 'nerix_160531_1031', 'nerix_160531_1144', 'nerix_160531_1234', 'nerix_160531_1303', 'nerix_160531_1331', 'nerix_160531_1357']
    #lFiles = ['nerix_160606_1046', 'nerix_160606_1155', 'nerix_160606_1225', 'nerix_160606_1254', 'nerix_160606_1347', 'nerix_160606_1421']

    # override if filename passed
    if len(sys.argv) == 2:
        lFiles = [sys.argv[1]]


    if len(lFiles) == 1:
        batch_mode = False
    else:
        batch_mode = True



    s_for_file = ''

    for file in lFiles:
        s_for_file += fit_drift_velocity(file, batch_mode)


    print '\n\nCopy and paste into fit summary file!\n\n\n'
    print s_for_file



