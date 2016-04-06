from ROOT import TChain, TCut, TH1F, TH2F, TF1, TF2, TCanvas, gPad, TGaxis, TLatex, gStyle, TPaletteAxis
import neriX_datasets
import sys, os

if len(sys.argv) != 2:
	print 'Usage is \'python fit_s2_gain_bottom.py <filename>\''
else:
	sFileName = sys.argv[1]
	if sFileName[-5:0] == '.root':
		sFileName = sFileName[:-5]

iRun = 15
sPathToData = '/Users/Zach/XENON/neriX/data/'
sPathToSaveOutput = '/Users/Zach/XENON/neriX/Calibrations/s2_gain/results/run_' + str(iRun) + '/' + sFileName
iNumElectrons = 3
aColors = [4, 2, 8, 7, 5, 9]

T1=TChain('T1')
T2=TChain('T2')
T1.AddFriend(T2)

T1.AddFile(sPathToData + 'run_' + str(iRun) + '/' + sFileName + '.root')
T2.AddFile(sPathToData + 'run_' + str(iRun) + '/' + sFileName + '.root')


fS2Max = 200
fS2BottomMin = 0
fS2TopMin = 5
iSampleWindow = [115,140]
iSamplesBetweenS2 = 100
iS2Wdith = 400
iCoinPMTs = 0


#implement cuts
#total S2 cut
XS2TotMax = TCut('S2sTot[S2Order[0]] < %f'%fS2Max)

#S2 bottom low cut
XS2BottomMin = TCut('S2sTotBottom[S2Order[0]] > %f'%fS2BottomMin)

#S2 top low cut
XS2TopMin = TCut('S2sTotTop[S2Order[0]] > %f'%fS2TopMin)

#coincidence cut
XCoin = TCut('S2sCoin[S2Order[0]] > %i'%iCoinPMTs)

#sample range after S1
XSampleAfterS1 = TCut('S2sPeak[S2Order[0]] > (S1sPeak[0] + %i) && S2sPeak[S2Order[0]] < (S1sPeak[0] + %i)'%(iSampleWindow[0],iSampleWindow[1]))

#noise cut
XNoise = TCut('log10((S1Tot + S2Tot)/(AreaTot - S1Tot - S2Tot)) > 0.')

#don't take S2s with another shortly after
XMinSampleBetweenS2 = TCut('Alt$(S2sLeftEdge[S2Order[1]],100000) - S2sRightEdge[S2Order[0]] > %i'%iSamplesBetweenS2)

#S2 width cut
XS2Width = TCut('S2sRightEdge[S2Order[0]] - S2sLeftEdge[S2Order[0]] < %i'%iS2Wdith)


XCuts = XS2TotMax + XS2BottomMin + XCoin + XSampleAfterS1 + XNoise + XMinSampleBetweenS2 + XS2Width + XS2TopMin




aS2sBottom = ['[%d]*exp(-0.5/%.1f*((x - %.1f*[0])/[1])**2)'%(iElectron + 1,iElectron,iElectron) for iElectron in range(1,iNumElectrons + 1)]
aS2sTop = ['exp(-0.5/%.1f*((y - %.1f*[%d])/[%d])**2)'%(iElectron,iElectron,iNumElectrons + 4,iNumElectrons + 5) for iElectron in range(1,iNumElectrons + 1)]
sThreshBottom = '1./(1+exp(([%d] - x)/([%d])))'%(iNumElectrons + 2,iNumElectrons + 3)
sThreshTop = '1./(1+exp(([%d] - y)/([%d])))'%(iNumElectrons + 6,iNumElectrons + 7)

S2TopBottom = TH2F('S2TopBottom','S2TopBottom',50,0,120,50,0,120)
S2Top = TH1F('S2Top','S2Top',50,0,100)
S2Bottom = TH1F('S2Bottom','S2Bottom',50,0,100)

c1 = TCanvas('c1','c1',1500,500)
c1.Divide(3,1)



#draw 2d plot with contour
c1.cd(1)
gPad.SetLogz()
gPad.SetRightMargin(0.12)
T1.Draw('S2sTotTop[S2Order[0]]:S2sTotBottom[S2Order[0]]>> S2TopBottom',XCuts,'colz')

aS2sComb = ['(%s)*(%s)'%(aS2sBottom[i],aS2sTop[i]) for i in range(iNumElectrons)]
FitS2TopBottom = TF2('FitS2TopBottom','(%s)*(%s)*(%s)'%(sThreshBottom,sThreshTop,' + '.join(aS2sComb)),0,100,0,100)
S2TopBottom.GetXaxis().SetTitle('S2sTotBottom[S2Order[0]]')
S2TopBottom.GetYaxis().SetTitle('S2sTotTop[S2Order[0]]')
S2TopBottom.GetYaxis().SetTitleOffset(1.4)
S2TopBottom.SetStats(0)
FitS2TopBottom.SetParLimits(0,15,45)
FitS2TopBottom.SetParLimits(1,2,20)
FitS2TopBottom.SetParLimits(iNumElectrons + 2,0,10)
FitS2TopBottom.SetParLimits(iNumElectrons + 3,0,10)
FitS2TopBottom.SetParLimits(iNumElectrons + 4,15,35)
FitS2TopBottom.SetParLimits(iNumElectrons + 5,2,20)
FitS2TopBottom.SetParLimits(iNumElectrons + 6,0,10)
FitS2TopBottom.SetParLimits(iNumElectrons + 7,0,10)

FitS2TopBottom.SetParameter(0,25)
FitS2TopBottom.SetParameter(1,10)
for iElectron in range(1,iNumElectrons + 1):
	FitS2TopBottom.SetParameter(iElectron + 1,200/iElectron)
FitS2TopBottom.SetParameter(iNumElectrons + 2,10)
FitS2TopBottom.SetParameter(iNumElectrons + 3,10)
FitS2TopBottom.SetParameter(iNumElectrons + 4,25)
FitS2TopBottom.SetParameter(iNumElectrons + 5,10)
FitS2TopBottom.SetParameter(iNumElectrons + 6,10)
FitS2TopBottom.SetParameter(iNumElectrons + 7,10)

FitS2TopBottom.SetContour(15)
S2TopBottom.Fit('FitS2TopBottom')

#draw S2sTotTop
c1.cd(2)
gPad.SetGrid()
T1.Draw('S2sTotTop[S2Order[0]]>> S2Top',XCuts)
S2Top.GetXaxis().SetTitle('S2sTotTop[S2Order[0]]')
S2Top.SetStats(0)
FitS2Top = TF1('FitS2Top','(%s)*(%s)'%(sThreshBottom,' + '.join(aS2sBottom)))
FitS2Top.FixParameter(0,FitS2TopBottom.GetParameter(iNumElectrons + 4))
FitS2Top.FixParameter(1,FitS2TopBottom.GetParameter(iNumElectrons + 5))
for iElectron in range(1,iNumElectrons + 1):
	FitS2Top.SetParameter(iElectron + 1,FitS2TopBottom.GetParameter(iElectron + 1))
	FitS2Top.SetParLimits(iElectron + 1,0,1000)

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
T1.Draw('S2sTotBottom[S2Order[0]]>> S2Bottom',XCuts)
S2Bottom.GetXaxis().SetTitle('S2sTotBottom[S2Order[0]]')
S2Bottom.SetStats(0)
FitS2Bottom = TF1('FitS2Bottom','(%s)*(%s)'%(sThreshBottom,' + '.join(aS2sBottom)))
FitS2Bottom.FixParameter(0,FitS2TopBottom.GetParameter(0))
FitS2Bottom.FixParameter(1,FitS2TopBottom.GetParameter(1))
for iElectron in range(1,iNumElectrons + 1):
	FitS2Bottom.SetParameter(iElectron + 1,FitS2TopBottom.GetParameter(iElectron + 1))
	FitS2Bottom.SetParLimits(iElectron + 1,0,1000)

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
c1.SaveAs(sPathToSaveOutput + '/gas_gain_' + sFileName + '.png')
c1.SaveAs(sPathToSaveOutput + '/gas_gain_' + sFileName + '.C')

if not os.path.exists(sPathToSaveOutput):
	os.mkdir(sPathToSaveOutput)

OutputFile = open(sPathToSaveOutput + '/gas_gain_' + sFileName + '.txt','w')
OutputFile.write('#anode\tcathode\t\ttop_eff_thresh\ttop_eff_thresh_err\ttop_eff_rolloff\ttop_eff_rolloff_err\ttop_mu\ttop_mu_err\ttop_width\ttop_width_err\tbottom_eff_thresh\tbottom_eff_thresh_err\tbottom_eff_rolloff\tbottom_eff_rolloff_err\tbottom_mu\tbottom_mu_err\tbottom_width\tbottom_width_err\n')
OutputFile.write('%f\t%f\t'%(neriX_datasets.run_files[iRun][sFileName + '.root'][0],neriX_datasets.run_files[iRun][sFileName + '.root'][1]))
OutputFile.write('%f\t%f\t'%(FitS2TopBottom.GetParameter(iNumElectrons + 6),FitS2TopBottom.GetParError(iNumElectrons + 6)))
OutputFile.write('%f\t%f\t'%(FitS2TopBottom.GetParameter(iNumElectrons + 7),FitS2TopBottom.GetParError(iNumElectrons + 7)))
OutputFile.write('%f\t%f\t'%(FitS2TopBottom.GetParameter(iNumElectrons + 4),FitS2TopBottom.GetParError(iNumElectrons + 4)))
OutputFile.write('%f\t%f\t'%(FitS2TopBottom.GetParameter(iNumElectrons + 5),FitS2TopBottom.GetParError(iNumElectrons + 5)))
OutputFile.write('%f\t%f\t'%(FitS2TopBottom.GetParameter(iNumElectrons + 2),FitS2TopBottom.GetParError(iNumElectrons + 2)))
OutputFile.write('%f\t%f\t'%(FitS2TopBottom.GetParameter(iNumElectrons + 3),FitS2TopBottom.GetParError(iNumElectrons + 3)))
OutputFile.write('%f\t%f\t'%(FitS2TopBottom.GetParameter(0),FitS2TopBottom.GetParError(0)))
OutputFile.write('%f\t%f'%(FitS2TopBottom.GetParameter(1),FitS2TopBottom.GetParError(1)))
OutputFile.close()



raw_input('')