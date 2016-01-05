# Usage: To set up simulation, use 'python distanceAnalysisLScint.py energy distance' with the energy in keV and the distance in cm
# this makes all changes described in Matt's 'Coincidence Analysis.pdf' so that the simulation can be correctly compiled. 
# To set up analysis (for coincidenceAnalysis.c), use 'python distanceAnalysisLScint.py energy distance 1' - the 1 limits the changes
# to those necessary for analysis.
# Some variables are saved in include/lscint_settings.hh, and the scintillator placement is appended to the src/NerixDetectorConstruction.cc
# code - these amendments can pile up, and should be deleted periodically at the line before '// germanium detectors' (the block beginning
# at line 1132)

from math import pi
from math import sqrt
from math import cos
from math import acos
from math import sin
from math import tan
from math import atan
import scipy
import ROOT
import sys

fheader = '/home/jhowlett/nerix/nerixsim/'
text_fname = fheader + 'distanceAnalysis_output.txt'
settings_fname = fheader + 'include/lscint_settings.hh'

sys.stdout = open(text_fname, 'w')

#Major variable of system
analysis=0
if len(sys.argv)>3:
	analysis = sys.argv[3]
recoilEnergy = 5
recoilEnergy = int(sys.argv[1])
positionSourceY = 40 #cm
positionSourcesY = [40, 80, 120, 160, 200, 240]
positionSourceY = int(sys.argv[2])
numberOfDetectors = 4
numberOfNeutrons = 1000000
nerixHalfHeight = 2.84
nerixRadius = 2.00167


def replace_angles_mac_file(fname, theta_small, theta_large, phi_small, phi_large):
	mac_file = open(fheader + fname, 'r')
	mac_lines = mac_file.readlines()
	mac_file.close()
	mac_file = open(fheader + fname, 'w')
	for line in mac_lines:
	  if 'thetarange' in line:
	    mac_file.write('/xe/gun/thetarange ' +str(theta_small) + ' ' + str(theta_large) + ' rad\n')
	  elif 'phirange' in line:
	    mac_file.write('/xe/gun/phirange ' +str(phi_small) + ' ' + str(phi_large) + ' rad\n')
	  else:
	    mac_file.write(line)
	mac_file.close()

def determineEnergy(angle):
	neutronEnergy = 2500. #kev
	neutronMass = 939.565 #MeV/c^2 but doesn't matter since mass terms cancel
	xenonMass = 131.293 * 931.494
	rEnergy = 2 * neutronEnergy *neutronMass * xenonMass * (1 - cos(angle)) /( (xenonMass + neutronMass)**2)
	return rEnergy

def angleLScint():
	#enter the inputs of the system
	neutronEnergy = 2500. #kev
	neutronMass = 939.565 #MeV/c^2 but doesn't matter since mass terms cancel
	xenonMass = 131.293 * 931.494
	angleInRadians = acos(1-((recoilEnergy/(2*neutronEnergy))*((neutronMass + xenonMass) ** 2/(neutronMass * xenonMass))))
	angleInDegrees = (180/pi)*angleInRadians
	#print "Angle in Radians: " + str(angleInRadians)
	#print "Angle in Degrees: " + str(angleInDegrees)
	return angleInRadians

def partitionCircle():
	partitionAngle = (2*pi)/numberOfDetectors
	return partitionAngle

#set the starting position of the source, LXe, and LScint ring
def initialDistancesAndPositions():
	#main input is positionSourceY bc the others are either
	#dependent on it or should remain the same
	positionSourceX = 0
	#positionSourceY is major variable so at the beginning
	positionSourceZ = -1.5

	#center of LXe detector
	positionLXeX = 0
	positionLXeY = 0
	positionLXeZ = -1.5

	d1 = sqrt((positionLXeX - positionSourceX)**2 + (positionLXeY - positionSourceY)**2 + (positionLXeZ - positionSourceZ)**2)
	d2 = d1 * 1.5
    
	detectorAngle = angleLScint()

	#center of detector ring
	positionRingX = 0
	positionRingY = -d2 * cos(detectorAngle)
	positionRingZ = -1.5

	d3 = sqrt((positionLXeX - positionRingX)**2 + (positionLXeY - positionRingY)**2 + (positionLXeZ - positionRingZ)**2)

	return { 'positionSourceX':positionSourceX, 'positionSourceY':positionSourceY, 'positionSourceZ':positionSourceZ, 'positionLXeX':positionLXeX, 'positionLXeY':positionLXeY, 'positionLXeZ':positionLXeZ, 'positionRingX':positionRingX, 'positionRingY':positionRingY, 'positionRingZ':positionRingZ, 'd1':d1, 'd2':d2, 'd3':d3 }

def placeDetectors():
	initial = initialDistancesAndPositions()
	detectorAngle = angleLScint()
	partitionAngle = partitionCircle()
	spannedAngle = partitionAngle
	
	i = 0
	
	detector0X = initial['d2'] * sin(detectorAngle)
	detector0Y = initial['positionRingY']
	detector0Z = initial['positionRingZ']
	
	ringRadius = sqrt((initial['positionRingX'] - detector0X)**2 + (initial['positionRingY'] - detector0Y)**2 + (initial['positionRingZ'] - detector0Z)**2)
	
	detectorPositions = {'detector0X':detector0X, 'detector0Y':detector0Y, 'detector0Z':detector0Z}
	
	if numberOfDetectors > 1:
	
		initialX = detector0X
		initialY = detector0Y
		initialZ = detector0Z
		
		
		
		i += 1
	
		while i < numberOfDetectors:
			detectorPositions['detector' + str(i) + 'X'] =  initial['positionRingX'] + ringRadius * cos(spannedAngle)
			detectorPositions['detector' + str(i) + 'Y'] =  initial['positionRingY']
			detectorPositions['detector' + str(i) + 'Z'] =  initial['positionRingZ'] + ringRadius * sin(spannedAngle)
			
			spannedAngle += partitionAngle
			i += 1
	
	return detectorPositions

def printSolidAngleLScint():
	detectorPositions = placeDetectors()
	radiusLScint = 3.81
	halfHeightLScint = 7.62/2

	#these angles are taken from a straight beam towards the LXe

	angleOne = atan((detectorPositions['detector0X'] + radiusLScint)/(-detectorPositions['detector0Y'] + positionSourceY))
	angleTwo = atan((detectorPositions['detector0X'] - radiusLScint)/(-detectorPositions['detector0Y'] + positionSourceY))
	
	#source and first detector are at same center height
	angleThree = atan((halfHeightLScint)/(sqrt((detectorPositions['detector0Y'] - positionSourceY) ** 2 + (detectorPositions['detector0X']) ** 2)))
	
	
	phiSmall = pi/2 - angleOne
	phiLarge = pi/2 - angleTwo
	
	thetaSmall = pi/2 - angleThree
	thetaLarge = pi/2 + angleThree

	#print 'Lower phi angle for coneLScint macro (rad): ' + str(phiSmall)
	#print 'Larger phi angle for coneLScint macro (rad): ' + str(phiLarge)

	#print 'Lower theta angle for coneLScint macro (rad): ' + str(thetaSmall)
	#print 'Larger theta angle for coneLScint macro (rad): ' + str(thetaLarge)
	solidAngle = -(phiLarge - phiSmall) * (cos(thetaLarge) - cos(thetaSmall))
	
	lscint_mac_fname = 'macros/minitronConeVisLScint.mac'
	replace_angles_mac_file(lscint_mac_fname, thetaSmall, thetaLarge, phiSmall, phiLarge)
	print 'LScint: '
	print 'Solid Angle = ' + str(solidAngle)
	print '/xe/gun/thetarange ' + str(thetaSmall) + ' ' + str(thetaLarge) + ' rad'
	print '/xe/gun/phirange ' + str(phiSmall) + ' ' + str(phiLarge) + ' rad'
	if analysis:
		settings_file = open(settings_fname, 'a')
		settings_file.write('double solid_angle_fraction_lscint = ' + str(solidAngle/(4*pi)) + ';\n')
		settings_file.close()

def printDetectors():
	
	j = 0
	detectorPositions = placeDetectors()
	if not analysis:
    		detector_fname = 'src/NerixDetectorConstruction.cc'
    		detector_file = open(fheader + detector_fname, 'r')
    		detector_lines = detector_file.readlines()
    		detector_file.close()
    		detector_file = open(fheader + detector_fname, 'w')
    		for line in detector_lines:
      			if '// germanium detectors' in line:
        			while j < numberOfDetectors:
          				detector_file.write('m_hGeometryParameters["Recoil' + str(recoilEnergy) + 'keVrLiquidScintillator' + str(j) + 'PositionX"] = ' + str(detectorPositions['detector' + str(j) + 'X']*10) + '*mm;\n')
          				detector_file.write('m_hGeometryParameters["Recoil' + str(recoilEnergy) + 'keVrLiquidScintillator' + str(j) + 'PositionY"] = ' + str(detectorPositions['detector' + str(j) + 'Y']*10) + '*mm;\n')
          				detector_file.write('m_hGeometryParameters["Recoil' + str(recoilEnergy) + 'keVrLiquidScintillator' + str(j) + 'PositionZ"] = ' + str(detectorPositions['detector' + str(j) + 'Z']*10) + '*mm;\n')
          				j += 1
      			detector_file.write(line)
    		detector_file.close()
	
	
	
def allowedAnglesFromSource():
	
	#source is in line with middle of Nerix detector
	theta = atan(nerixHalfHeight/(positionSourceY))
	phi = atan(nerixRadius/positionSourceY)
	
	angles = {'upperTheta': pi/2 - theta, 'lowerTheta': pi/2 + theta, 'upperPhi': -phi, 'lowerPhi': phi}
	
	solidAngle = -(angles['upperPhi'] - angles['lowerPhi']) * (cos(angles['upperTheta']) - cos(angles['lowerTheta']))
	
	lxe_mac_fname = 'macros/minitronConeVis.mac'
	lxe_mac_fname_novis = 'macros/minitronConeNoVis.mac'
	replace_angles_mac_file(lxe_mac_fname, angles['upperTheta'], angles['lowerTheta'], pi/2 + angles['upperPhi'], pi/2 + angles['lowerPhi'])
	replace_angles_mac_file(lxe_mac_fname_novis, angles['upperTheta'], angles['lowerTheta'], pi/2 + angles['upperPhi'], pi/2 + angles['lowerPhi'])
	
	print 'LXe: '
	print 'Solid Angle = ' + str(solidAngle)
	print '/xe/gun/thetarange ' +str(angles['upperTheta']) + ' ' + str(angles['lowerTheta']) + ' rad'
	print '/xe/gun/phirange ' +str(pi/2 + angles['upperPhi']) + ' ' + str(pi/2 + angles['lowerPhi']) + ' rad'

	if analysis:
		settings_file = open(settings_fname, 'a')
		settings_file.write('double solid_angle_fraction_lxe = ' + str(solidAngle/(4*pi)) + ';\n')
		settings_file.close()
	return


	
def findAngle(x1, y1, z1, x2, y2, z2, x3, y3, z3):
	from math import acos
	mag1 = sqrt((x2-x1)**2 + (y2-y1)**2 + (z2-z1)**2)
	mag2 = sqrt((x2-x3)**2 + (y2-y3)**2 + (z2-z3)**2)
	dotProduct = (x2-x1)*(x3-x2) + (y2-y1)*(y3-y2) + (z2-z1)*(z3-z2)
	angle = acos(dotProduct/(mag1 * mag2))
	return angle
	
	
def calculateUncertainty():
	initial = initialDistancesAndPositions()
	positions = placeDetectors()
	
	#this is not the max uncertainty...get that by using top of one detector and bottom of other
	#but this should suffice bc looking at 1 sigma
	#since isotropic, only keep 33% of radius since again looking at one sigma (uniform distribution)
	radiusLXe = 4.0132  #ChamberWallRadius in Geant4 code
	radiusLScint = 3.81  #LiquidScintillatorRadius in Geant4 code
	
	sourceLXeAngle = acos(initial['d1'] / sqrt(initial['d1']**2 + radiusLXe**2))
	effectiveRadiusLXe = initial['d1']  * tan(sourceLXeAngle/3)
	
	LXeLScintAngle = acos(initial['d2'] / sqrt(initial['d2']**2 + radiusLScint**2))
	effectiveRadiusLScint = initial['d2']  * tan(LXeLScintAngle/3)
	
	#upper is for upper bound (larger angle) and lower is for lower bound (smaller angle)
	upperLXeX = initial['positionLXeX']  - effectiveRadiusLXe
	upperLScintX = positions['detector0X'] + effectiveRadiusLScint
	
	lowerLXeX = initial['positionLXeX']  + effectiveRadiusLXe
	lowerLScintX = positions['detector0X'] - effectiveRadiusLScint
	
	upperAngle = findAngle(initial['positionSourceX'], initial['positionSourceY'], initial['positionSourceZ'], upperLXeX, initial['positionLXeY'], initial['positionLXeZ'], upperLScintX,  positions['detector0Y'],  positions['detector0Z'])
	lowerAngle = findAngle(initial['positionSourceX'], initial['positionSourceY'], initial['positionSourceZ'], lowerLXeX, initial['positionLXeY'], initial['positionLXeZ'], lowerLScintX,  positions['detector0Y'],  positions['detector0Z'])
	
	upperEnergy = determineEnergy(upperAngle)
	lowerEnergy = determineEnergy(lowerAngle)
	
	upperUncertainty = upperEnergy - recoilEnergy
	lowerUncertainty = recoilEnergy - lowerEnergy
	
	print 'Upper uncertainty (keV): ' + str(upperUncertainty)
	print 'Lower uncertainty (keV): ' + str(lowerUncertainty)


def CalculateRates(totalRate, d1, sourceY):
    #this is approximate (treats small detector as curved to edge of the sphere)
    #does not account for potential shielding between LXe and LScint detectors
    
    #load necessary functions
    initial = initialDistancesAndPositions()
    detectors = placeDetectors()
    
    #cross sections are approximate
    areaLXe = 5.6895 * 4.0034 #cm^2
    areaLScint = 2 * 3.81 * 7.62
    
    percentSphereLXe = areaLXe / (4 * pi * (d1 ** 2))
    percentSphereLScint = areaLScint / (4 * pi * (sqrt((detectors['detector0X'] - initial['positionSourceX'])**2 + (detectors['detector0Y'] - sourceY)**2 + (detectors['detector0Z'] - initial['positionSourceZ'])**2))**2)

    #print percentSphereLXe
    #print percentSphereLScint

    rateLXe = totalRate * percentSphereLXe * .34
    rateLScint = totalRate * percentSphereLScint * .849608
    elapsedTime = numberOfNeutrons/(rateLXe)
    combinedWidthNanoseconds = 10000
    combinedWidthSeconds = .000000001 * combinedWidthNanoseconds
    coincidenceFactor = rateLScint * combinedWidthSeconds
    
    rateInformation = {'rateLXe':rateLXe, 'rateLScint':rateLScint, 'elapsedTime':elapsedTime, 'combinedWidthSeconds':combinedWidthSeconds, 'coincidenceFactor':coincidenceFactor}
    
    print rateInformation
    return rateInformation
    
        
print 'Source distance (cm): ' + str(positionSourceY)



#now trying more accurate estimate of uncertainty

def pointToPointDistance(x1, y1, z1, x2, y2, z2):
	distance = sqrt((x2-x1)**2 + (y2-y1)**2 + (z2-z1)**2)
	return distance

def probability(distance, area):
	p = area / (4 * pi * distance ** 2)
	return p


#use odd number of rows and tiles so that there is a center
def createTileBoards(numberOfRows, numberOfColumns):
	#columns and rows refer to the tiles on the LXe and LScint
	radiusLScint = 3.81
	halfHeightLScint = 7.62/2
	radiusLXe = 4.0132
	halfHeightLXe = 10.711/2
	#approximating that centers are at same z height

	numberOfTiles = numberOfRows * numberOfColumns
	areaTileLXe = (4 * halfHeightLXe * radiusLXe)/numberOfTiles
	areaTileLScint = (4 * halfHeightLScint * radiusLScint)/numberOfTiles
	#approximating detectors as vertical sheets at center
	
	widthTileLScint = 2 * radiusLScint / numberOfRows
	heightTileLScint = 2 * halfHeightLScint / numberOfColumns
	
	widthTileLXe = 2 * radiusLXe / numberOfRows
	heightTileLXe = 2 * halfHeightLXe / numberOfColumns
	
	d1 = positionSourceY
	d2 = 1.5 * d1
	
	#note that y will be constant on LXe but will not be for LScint due to angle (z
	#is unaffected by the angle)
	
	#top left corner (neg x and pos z) will be the zero zero tile
	
	#find center tile
	#bottom right as viewed by observer at source
	middleTileLXe = [0, 0, -1.5] #refers to center of the middle tile
	bottomRightTileLXe = [middleTileLXe[0] - (((numberOfRows - 1)/2) * widthTileLXe), middleTileLXe[1], middleTileLXe[2] - (((numberOfColumns - 1)/2) * heightTileLXe)]
	
	#defines dictionary with the center of each of the tiles for LXe
	#format: tileLXeij
	tileLXe = {}
	i = 0
	#i is row number and j is column number
	while i < numberOfRows:
		j = 0
		while j < numberOfColumns:
			tileLXe['tileLXe' + str(i) + str(j)] = [bottomRightTileLXe[0] + (i * widthTileLXe), bottomRightTileLXe[1], bottomRightTileLXe[2] + (j * heightTileLXe)]
			j += 1
		i += 1
	
	angle = angleLScint()
	#set first LScint detector
	initial = initialDistancesAndPositions()
	detector0X = d2 * sin(angle)
	detector0Y = initial['positionRingY']
	detector0Z = initial['positionRingZ']
	
	
	middleTileLScint = [detector0X, detector0Y, detector0Z]
	bottomRightTileLScint = [middleTileLScint[0] - (((numberOfRows - 1)/2) * widthTileLScint) * cos(angle), middleTileLScint[1] - (((numberOfRows - 1)/2) * widthTileLScint) * cos(angle), middleTileLScint[2] - (((numberOfColumns - 1)/2) * heightTileLScint)]

	i = 0
	tileLScint = {}
	while i < numberOfRows:
		j = 0
		while j < numberOfColumns:
			tileLScint['tileLScint' + str(i) + str(j)] = [bottomRightTileLScint[0] + (i * widthTileLScint) * cos(angle), bottomRightTileLScint[1] + (i * widthTileLScint) * sin(angle), bottomRightTileLScint[2] + (j * heightTileLScint)]
			j += 1
		i += 1
	
	#now want to create dictionary filled with probabilities and corresponding energies
	prob = {}
	#format: probijxy, i and j are the row and column of LXe and x and y ar r and c for LScint
	probabilityCounter = 0
	counter = 0
	energyCounter = 0
	i = 0
	while i < numberOfRows:
		j = 0
		while j < numberOfColumns:
			x = 0
			while x < numberOfRows:
				y = 0
				while y < numberOfColumns:
					distanceLXe = pointToPointDistance(tileLXe['tileLXe' + str(i) +str(j)][0], tileLXe['tileLXe' + str(i) +str(j)][1], tileLXe['tileLXe' + str(i) +str(j)][2], initial['positionSourceX'], initial['positionSourceY'], initial['positionSourceZ'])
					
					distanceLScint = pointToPointDistance(tileLXe['tileLXe' + str(i) +str(j)][0], tileLXe['tileLXe' + str(i) +str(j)][1], tileLXe['tileLXe' + str(i) +str(j)][2], tileLScint['tileLScint' + str(x) +str(y)][0], tileLScint['tileLScint' + str(x) +str(y)][1], tileLScint['tileLScint' + str(x) +str(y)][2])
					
					angleBetweenPoints = findAngle(initial['positionSourceX'], initial['positionSourceY'], initial['positionSourceZ'], tileLXe['tileLXe' + str(i) +str(j)][0], tileLXe['tileLXe' + str(i) +str(j)][1], tileLXe['tileLXe' + str(i) +str(j)][2], tileLScint['tileLScint' + str(x) +str(y)][0], tileLScint['tileLScint' + str(x) +str(y)][1], tileLScint['tileLScint' + str(x) +str(y)][2])
					
					energyOfRecoil = determineEnergy(angleBetweenPoints)
					energyCounter += energyOfRecoil
					
					probabilityOfRecoil = probability(distanceLScint, widthTileLScint * heightTileLScint) * probability(distanceLXe, widthTileLXe * heightTileLXe)
					probabilityCounter += probabilityOfRecoil
					
					prob['prob' + str(i) + str(j) + str(x) +str(y)] = [probabilityOfRecoil, energyOfRecoil]
					#print energyOfRecoil
					counter +=1
					
					y+=1
				x+=1
			j+=1
		i+=1

	meanEnergy = round(energyCounter/counter,2)
	print meanEnergy
	#print probabilityCounter
	#print prob
	#need to normalize probabilites to one 
	#go from 1.5 to 2.4 keV when looping through energies

	#print prob
	#label energyLevels by upper bounds

	energyLevels = {}
	g = 0
	i = 0
	energyRange = 4 #distance above and below desired energy we are sampling
	lowerEnergy = recoilEnergy - energyRange
	upperEnergy = recoilEnergy + energyRange
	energyIncrement = .01
	levelsToLoop = (upperEnergy - lowerEnergy)/energyIncrement
	sumOfProbabilities = 0
	while g < levelsToLoop:
		energyLevels[str(g)] = 0
		g+=1
	while i < numberOfRows:
		j = 0
		while j < numberOfColumns:
			x = 0
			while x < numberOfRows:
				y = 0
				while y < numberOfColumns:
					
					q = 0
					while q < levelsToLoop:
						if (prob['prob' + str(i) + str(j) + str(x) +str(y)][1] > (lowerEnergy + (q-1)*energyIncrement)) and (prob['prob' + str(i) + str(j) + str(x) +str(y)][1] < (lowerEnergy + (q)*energyIncrement)):
							energyLevels[str(q)] += float(prob['prob' + str(i) + str(j) + str(x) +str(y)][0])/ (probabilityCounter)
							sumOfProbabilities += energyLevels[str(q)]
						q+=1
					y+=1
				x+=1
			j+=1
		i+=1

	print sumOfProbabilities
	q = 0
	while q < levelsToLoop:
		#print q
		#print energyLevels[str(q)]
		q += 1
	oneSigmaCounter = 0
	meanBin = int(energyRange / energyIncrement)
	print meanBin
	#print meanBin
	probabilityOfMean = energyLevels[str(meanBin)]
	#print probabilityOfMean
	q = meanBin + 1 #set equal to mean's bin
	while oneSigmaCounter < (.383 - probabilityOfMean):
		#print q
		#print energyLevels[str(q)]
		oneSigmaCounter += energyLevels[str(q)]
		if (q == levelsToLoop - 1):
			print 'range too small'
			break
		q += 1
			
	energyWidth = float(lowerEnergy + (q)*energyIncrement) - float(lowerEnergy + (meanBin)*energyIncrement)
	
			
	print energyWidth
	return


def main(source_position):
  print('\nSource Position = ' + str(source_position) + ':')
  settings_file = open(settings_fname, 'w')
  settings_file.write('const double position_source_y = ' + str(float(source_position)) + ';\n')
  settings_file.write('const int nb_lscints = ' + str(numberOfDetectors) + ';\n')
  settings_file.write('const double recoil_energy = ' + str(float(recoilEnergy)) + ';\n')
  if analysis:
	settings_file.write('std::string energy_distance = \"' + str(recoilEnergy) + 'keV' + str(source_position) + 'cm\";\n')
  settings_file.close()

  printSolidAngleLScint()
  printDetectors()
  allowedAnglesFromSource()
  #calculateUncertainty()
  initial = initialDistancesAndPositions()
  #CalculateRates(100000, initial['d1'], positionSourceY)
  #createTileBoards(15, 15)
  #take results of Tile Boards with grain of salt


if __name__ == "__main__":
  main(positionSourceY)



