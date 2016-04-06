from ROOT import TChain, TCut, TH1F, TH2F, TF1, TF2, TCanvas, gPad, TGaxis, TLatex, gStyle, TPaletteAxis, TVirtualFitter
from rootpy.plotting import Canvas
import neriX_analysis
import neriX_datasets
import sys, os

if len(sys.argv) != 2:
	print 'Usage is \'python fit_s2_gain_bottom.py <filename>\''
else:
	sFileName = sys.argv[1]
	if sFileName[-5:0] == '.root':
		sFileName = sFileName[:-5]

current_analysis = neriX_analysis.neriX_analysis(sFileName)
#print 'Bypassing file and doing all!'
#current_analysis = neriX_analysis.neriX_analysis(neriX_analysis.pull_all_files_given_parameters(11, 4.500, 1.054, -1), -1, 1.054, 4.5)

sPathToSaveOutput ='./results/run_' + str(current_analysis.get_run()) + '/' + str(current_analysis.get_filename()[:-5])

aColors = [4, 2, 8, 7, 5, 9]

iNumElectrons = 4
mean_bot_guess = 14.5
width_bot_guess = 5.

s2_parameter = 'S2sTotBottom'

TVirtualFitter.SetMaxIterations(30000)

fS2Max = 200
fS2BottomMin = 0
fS2TopMin = 3
#iSampleWindow = [115,140] # run 15
iSampleWindow = [100,200] # after KNF
#iSampleWindow = [150,250] # run 10
iSamplesBetweenS2 = 100
iS2Wdith = 400
iCoinPMTs = 3


#implement cuts
#total S2 cut
current_analysis.add_cut('S2sTot[S2Order[0]] < %f'%fS2Max)


#S2 bottom low cut
current_analysis.add_cut('%s[S2Order[0]] > %f' % (s2_parameter, fS2BottomMin))


#S2 top low cut
current_analysis.add_cut('S2sTotTop[S2Order[0]] > %f'%fS2TopMin)


#coincidence cut
current_analysis.add_cut('S2sCoin[S2Order[0]] > %i'%iCoinPMTs)


#sample range after S1
current_analysis.add_cut('S2sPeak[S2Order[0]] > (S1sPeak[0] + %i) && S2sPeak[S2Order[0]] < (S1sPeak[0] + %i)'%(iSampleWindow[0],iSampleWindow[1]))


#noise cut
current_analysis.add_cut('log10((S1Tot + S2Tot)/(AreaTot - S1Tot - S2Tot)) > 0.')


#don't take S2s with another shortly after
current_analysis.add_cut('Alt$(S2sLeftEdge[S2Order[1]],100000) - S2sRightEdge[S2Order[0]] > %i'%iSamplesBetweenS2)


#S2 width cut
current_analysis.add_cut('S2sRightEdge[S2Order[0]] - S2sLeftEdge[S2Order[0]] < %i'%iS2Wdith)

current_analysis.set_event_list()



aS2sBottom = ['[%d]*exp(-0.5/%.1f*((x - %.1f*[0])/[1])**2)'%(iElectron + 1,iElectron,iElectron) for iElectron in range(1,iNumElectrons + 1)]
aS2sTop = ['exp(-0.5/%.1f*((y - %.1f*[%d])/[%d])**2)'%(iElectron,iElectron,iNumElectrons + 4,iNumElectrons + 5) for iElectron in range(1,iNumElectrons + 1)]
sThreshBottom = '1./(1+exp(([%d] - x)/([%d])))'%(iNumElectrons + 2,iNumElectrons + 3)
sThreshTop = '1./(1+exp(([%d] - y)/([%d])))'%(iNumElectrons + 6,iNumElectrons + 7)

S2TopBottom = TH2F('S2TopBottom','S2TopBottom - V_{c} %.3f kV' % current_analysis.get_cathode_setting(),60,0,100,60,0,100)
S2Top = TH1F('S2Top','S2Top',50,0,100)
S2Bottom = TH1F('S2Bottom','S2Bottom',50,0,100)

c1 = Canvas(1500,500)
c1.Divide(3,1)



#draw 2d plot with contour
c1.cd(1)
gPad.SetLogz()
gPad.SetRightMargin(0.12)
current_analysis.Draw('%s[S2Order[0]]:S2sTotTop[S2Order[0]]' % s2_parameter, hist=S2TopBottom)
S2TopBottom.Draw('colz')

aS2sComb = ['(%s)*(%s)'%(aS2sBottom[i],aS2sTop[i]) for i in range(iNumElectrons)]
FitS2TopBottom = TF2('FitS2TopBottom','(%s)*(%s)*(%s)'%(sThreshBottom,sThreshTop,' + '.join(aS2sComb)),0,100,0,100)
print '(%s)*(%s)*(%s)'%(sThreshBottom,sThreshTop,' + '.join(aS2sComb))
S2TopBottom.GetXaxis().SetTitle('%s[S2Order[0]]' % s2_parameter)
S2TopBottom.GetYaxis().SetTitle('S2sTotTop[S2Order[0]]')
S2TopBottom.GetYaxis().SetTitleOffset(1.4)
S2TopBottom.SetStats(0)
FitS2TopBottom.SetParLimits(0,11,25)
FitS2TopBottom.SetParLimits(1,2,20)
FitS2TopBottom.SetParLimits(iNumElectrons + 2,0.1,10)
FitS2TopBottom.SetParLimits(iNumElectrons + 3,0,5)
FitS2TopBottom.SetParLimits(iNumElectrons + 4,5,35)
FitS2TopBottom.SetParLimits(iNumElectrons + 5,2,20)
FitS2TopBottom.SetParLimits(iNumElectrons + 6,0,10)
FitS2TopBottom.SetParLimits(iNumElectrons + 7,0,10)

FitS2TopBottom.SetParameter(0,mean_bot_guess)
FitS2TopBottom.SetParameter(1,width_bot_guess)
for iElectron in range(1,iNumElectrons + 1):
	FitS2TopBottom.SetParameter(iElectron + 1,200/iElectron)
FitS2TopBottom.SetParameter(iNumElectrons + 2,6)
FitS2TopBottom.SetParameter(iNumElectrons + 3,2)
FitS2TopBottom.SetParameter(iNumElectrons + 4,15)
FitS2TopBottom.SetParameter(iNumElectrons + 5,7)
FitS2TopBottom.SetParameter(iNumElectrons + 6,7)
FitS2TopBottom.SetParameter(iNumElectrons + 7,0)

FitS2TopBottom.SetContour(15)
S2TopBottom.Fit('FitS2TopBottom', 'L')

#draw S2sTotTop
c1.cd(2)
gPad.SetGrid()
current_analysis.Draw('S2sTotTop[S2Order[0]]',hist=S2Top)
S2Top.Draw()
S2Top.GetXaxis().SetTitle('S2sTotTop[S2Order[0]]')
S2Top.SetStats(0)
FitS2Top = TF1('FitS2Top','(%s)*(%s)'%(sThreshBottom,' + '.join(aS2sBottom)))
FitS2Top.FixParameter(0,FitS2TopBottom.GetParameter(iNumElectrons + 4))
FitS2Top.FixParameter(1,FitS2TopBottom.GetParameter(iNumElectrons + 5))
for iElectron in range(1,iNumElectrons + 1):
	FitS2Top.SetParameter(iElectron + 1,FitS2TopBottom.GetParameter(iElectron + 1))
	FitS2Top.SetParLimits(iElectron + 1,0,10000)

FitS2Top.FixParameter(iNumElectrons + 2,FitS2TopBottom.GetParameter(iNumElectrons + 6))
FitS2Top.FixParameter(iNumElectrons + 3,FitS2TopBottom.GetParameter(iNumElectrons + 7))
S2Top.Fit('FitS2Top')

c1.Update()

FitEfficiencyY = TF1('FitEfficiencyY','%f/3./(1+exp(([0] - x)/[1]))'%gPad.GetUymax(),0,100)
FitEfficiencyY.FixParameter(0,FitS2TopBottom.GetParameter(iNumElectrons + 6))
FitEfficiencyY.FixParameter(1,FitS2TopBottom.GetParameter(iNumElectrons + 7))
FitEfficiencyY.SetLineStyle(5)
FitEfficiencyY.Draw('same')


FitPEsY = [TF1('FitPEY_%i'%iElectron,'[2]*exp(-0.5/%.1f*((x - %.1f*[0])/[1])**2)'%(iElectron,iElectron),0,100) for iElectron in range(1,iNumElectrons + 1)]
aLatexFitPEsY = []
for iElectron in range(1,iNumElectrons + 1):
	FitPEsY[iElectron - 1].FixParameter(0,FitS2TopBottom.GetParameter(iNumElectrons + 4))
	FitPEsY[iElectron - 1].FixParameter(1,FitS2TopBottom.GetParameter(iNumElectrons + 5))
	FitPEsY[iElectron - 1].FixParameter(2,FitS2Top.GetParameter(iElectron + 1))
	FitPEsY[iElectron - 1].SetLineColor(aColors[iElectron - 1])
	FitPEsY[iElectron - 1].Draw('same')
	aLatexFitPEsY.append(TLatex(iElectron*FitS2TopBottom.GetParameter(iNumElectrons + 4),0.6*FitS2Top.GetParameter(iElectron + 1),'%ie-'%iElectron))
	aLatexFitPEsY[iElectron - 1].SetTextColor(aColors[iElectron - 1])
	aLatexFitPEsY[iElectron - 1].SetTextAlign(22)
	aLatexFitPEsY[iElectron - 1].Draw('same')

RightAxisY1 = TGaxis(gPad.GetUxmax(), gPad.GetUymin(), gPad.GetUxmax(), gPad.GetUymax(), 0., 3., 510, '+L')
RightAxisY1.SetTitle('Efficiency')
RightAxisY1.CenterTitle()
RightAxisY1.SetDecimals()
RightAxisY1.SetLabelSize(0.045)
RightAxisY1.SetTitleSize(0.05)
RightAxisY1.SetTextFont(132)
RightAxisY1.SetLabelFont(132)
RightAxisY1.SetTitleOffset(1.0)
RightAxisY1.Draw()

LatexTopMean = TLatex(0.4, 0.72, '#mu_{S2, e-} = %.2f #pm %.2f pe'%(FitS2TopBottom.GetParameter(iNumElectrons + 4),FitS2TopBottom.GetParError(iNumElectrons + 4)))
LatexTopSigma = TLatex(0.4, 0.66, '#sigma_{S2, e-} = %.2f #pm %.2f pe'%(FitS2TopBottom.GetParameter(iNumElectrons + 5),FitS2TopBottom.GetParError(iNumElectrons + 5)))
LatexTopMean.SetNDC()
LatexTopSigma.SetNDC()
LatexTopMean.Draw('same')
LatexTopSigma.Draw('same')


#draw S2sTotBottom
c1.cd(3)
gPad.SetGrid()
current_analysis.Draw('%s[S2Order[0]]' % s2_parameter,hist=S2Bottom)
S2Bottom.Draw()
S2Bottom.GetXaxis().SetTitle('%s[S2Order[0]]' % s2_parameter)
S2Bottom.SetStats(0)
FitS2Bottom = TF1('FitS2Bottom','(%s)*(%s)'%(sThreshBottom,' + '.join(aS2sBottom)))
FitS2Bottom.FixParameter(0,FitS2TopBottom.GetParameter(0))
FitS2Bottom.FixParameter(1,FitS2TopBottom.GetParameter(1))
for iElectron in range(1,iNumElectrons + 1):
	FitS2Bottom.SetParameter(iElectron + 1,FitS2TopBottom.GetParameter(iElectron + 1))
	FitS2Bottom.SetParLimits(iElectron + 1,0,10000)

FitS2Bottom.FixParameter(iNumElectrons + 2,FitS2TopBottom.GetParameter(iNumElectrons + 2))
FitS2Bottom.FixParameter(iNumElectrons + 3,FitS2TopBottom.GetParameter(iNumElectrons + 3))
S2Bottom.Fit('FitS2Bottom')

c1.Update()

FitEfficiencyX = TF1('FitEfficiencyX','%f/3./(1+exp(([0] - x)/[1]))'%gPad.GetUymax(),0,100)
FitEfficiencyX.FixParameter(0,FitS2TopBottom.GetParameter(iNumElectrons + 2))
FitEfficiencyX.FixParameter(1,FitS2TopBottom.GetParameter(iNumElectrons + 3))
FitEfficiencyX.SetLineStyle(2)
FitEfficiencyX.Draw('same')

FitPEsX = [TF1('FitPEX_%i'%iElectron,'[2]*exp(-0.5/%.1f*((x - %.1f*[0])/[1])**2)'%(iElectron,iElectron),0,100) for iElectron in range(1,iNumElectrons + 1)]
aLatexFitPEsX = []
for iElectron in range(1,iNumElectrons + 1):
	FitPEsX[iElectron - 1].FixParameter(0,FitS2TopBottom.GetParameter(0))
	FitPEsX[iElectron - 1].FixParameter(1,FitS2TopBottom.GetParameter(1))
	FitPEsX[iElectron - 1].FixParameter(2,FitS2Bottom.GetParameter(iElectron + 1))
	FitPEsX[iElectron - 1].SetLineColor(aColors[iElectron - 1])
	FitPEsX[iElectron - 1].Draw('same')
	aLatexFitPEsX.append(TLatex(iElectron*FitS2TopBottom.GetParameter(0),0.6*FitS2Bottom.GetParameter(iElectron + 1),'%ie-'%iElectron))
	aLatexFitPEsX[iElectron - 1].SetTextColor(aColors[iElectron - 1])
	aLatexFitPEsX[iElectron - 1].SetTextAlign(22)
	aLatexFitPEsX[iElectron - 1].Draw('same')

RightAxisY2 = TGaxis(gPad.GetUxmax(), gPad.GetUymin(), gPad.GetUxmax(), gPad.GetUymax(), 0, 3, 510, '+L')
RightAxisY2.SetTitle('Efficiency')
RightAxisY2.CenterTitle()
RightAxisY2.SetDecimals()
RightAxisY2.SetLabelSize(0.045)
RightAxisY2.SetTitleSize(0.05)
RightAxisY2.SetTextFont(132)
RightAxisY2.SetLabelFont(132)
RightAxisY2.SetTitleOffset(1.0)
RightAxisY2.Draw()

LatexBottomMean = TLatex(0.4, 0.72, '#mu_{S2, e-} = %.2f #pm %.2f pe'%(FitS2TopBottom.GetParameter(0),FitS2TopBottom.GetParError(0)))
LatexBottomSigma = TLatex(0.4, 0.66, '#sigma_{S2, e-} = %.2f #pm %.2f pe'%(FitS2TopBottom.GetParameter(1),FitS2TopBottom.GetParError(1)))
LatexBottomMean.SetNDC()
LatexBottomSigma.SetNDC()
LatexBottomMean.Draw('same')
LatexBottomSigma.Draw('same')

c1.Update()

raw_input('Press enter to save...')

if not os.path.exists(sPathToSaveOutput):
	os.makedirs(sPathToSaveOutput)



c1.SaveAs(sPathToSaveOutput + '/gas_gain_' + sFileName + '.png')
c1.SaveAs(sPathToSaveOutput + '/gas_gain_' + sFileName + '.C')


OutputFile = open(sPathToSaveOutput + '/gas_gain_' + sFileName[:-5] + '.txt','w')
OutputFile.write('#anode\tcathode\t\ttop_eff_thresh\ttop_eff_thresh_err\ttop_eff_rolloff\ttop_eff_rolloff_err\ttop_mu\ttop_mu_err\ttop_width\ttop_width_err\tbottom_eff_thresh\tbottom_eff_thresh_err\tbottom_eff_rolloff\tbottom_eff_rolloff_err\tbottom_mu\tbottom_mu_err\tbottom_width\tbottom_width_err\n')
OutputFile.write('%f\t%f\t'%(neriX_datasets.run_files[current_analysis.get_run()][sFileName + '.root'][0],neriX_datasets.run_files[current_analysis.get_run()][sFileName + '.root'][1]))
OutputFile.write('%f\t%f\t'%(FitS2TopBottom.GetParameter(iNumElectrons + 6),FitS2TopBottom.GetParError(iNumElectrons + 6)))
OutputFile.write('%f\t%f\t'%(FitS2TopBottom.GetParameter(iNumElectrons + 7),FitS2TopBottom.GetParError(iNumElectrons + 7)))
OutputFile.write('%f\t%f\t'%(FitS2TopBottom.GetParameter(iNumElectrons + 4),FitS2TopBottom.GetParError(iNumElectrons + 4)))
OutputFile.write('%f\t%f\t'%(FitS2TopBottom.GetParameter(iNumElectrons + 5),FitS2TopBottom.GetParError(iNumElectrons + 5)))
OutputFile.write('%f\t%f\t'%(FitS2TopBottom.GetParameter(iNumElectrons + 2),FitS2TopBottom.GetParError(iNumElectrons + 2)))
OutputFile.write('%f\t%f\t'%(FitS2TopBottom.GetParameter(iNumElectrons + 3),FitS2TopBottom.GetParError(iNumElectrons + 3)))
OutputFile.write('%f\t%f\t'%(FitS2TopBottom.GetParameter(0),FitS2TopBottom.GetParError(0)))
OutputFile.write('%f\t%f'%(FitS2TopBottom.GetParameter(1),FitS2TopBottom.GetParError(1)))
OutputFile.close()


print '\n\n%s\t%d\t%.3f\t%.3f\t%.2f\t%.2f\t%.2f\t%.2f\n\n' % (current_analysis.get_filename_no_ext(), current_analysis.get_timestamp(), current_analysis.get_anode_setting(), current_analysis.get_cathode_setting(), FitS2TopBottom.GetParameter(0), FitS2TopBottom.GetParError(0), FitS2TopBottom.GetParameter(1), FitS2TopBottom.GetParError(1))



