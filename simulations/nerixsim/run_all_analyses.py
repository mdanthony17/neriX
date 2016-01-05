from subprocess import call

energy = 1
energies = [4,  4,  4]
distances =[40, 80, 120]
for i,distance in enumerate(distances):
	call(['python', 'distanceAnalysisLScint.py', str(energies[i]), str(distance), '1'])
	call(['root', '-l', '-q', 'coincidenceAnalysis.C'])
