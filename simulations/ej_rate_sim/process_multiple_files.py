#!/usr/bin/python

#Usage: 'python process_multiple_files.py n_processors' where n_processors is the number of cores to simultaneously use
#(or number of simultaneous processes created). Use the arrays recoil_energies, distances, and nb_events to list the
#simulation parameters you want. Output is to stdout so run on a screen or redirect to text file. Analyzes files as
#well by default - set analyze_files to 0 to turn this off.


import sys, array, os
from subprocess import call
import time

max_processors = 16
for i in xrange(0, max_processors):
	call(['cp', 'nerixsim.cc', 'nerixsim_' + str(i) + '.cc'])
analyzed = 0
if len(sys.argv) == 2:
  max_processors = int(sys.argv[1])


skipped_analysis = []
#recoil_energies = [1,   2,   2,   3,   3,   3,   3,   8,   8,   8,   8,   8,   8,   10,  10,  10 ]
#distances = 	  [230, 150, 155, 125, 130, 135, 140, 40,  70,  120, 160, 200, 240, 60,  65,  70 ]
#nb_events = 	  [6e6, 6e6, 6e6, 3e6, 3e6, 3e6, 3e6, 3e6, 3e6, 3e6, 3e6, 3e6, 3e6, 3e6, 3e6, 3e6]

analyze_files = 1
recoil_energies = [1,   1,   4,   8,   8,   8,   8,   8,   10,  10] 
distances = 	  [235, 240, 120, 80,  85,  90,  95,  100, 70,  75]
nb_events = 	  [6e6, 6e6, 6e6, 3e6, 3e6, 3e6, 3e6, 3e6, 6e6, 6e6]
nb_jobs = len(recoil_energies)


job = 0
while job<nb_jobs:
  if job>0:
	print 'second time around'
  children = []
  if (nb_jobs-job)>max_processors:
    numProcesses=max_processors
  else:
    numProcesses = nb_jobs-job
  for process in xrange(0, numProcesses):
    makefile = open('Makefile', 'r')
    makefile_lines = makefile.readlines()
    makefile.close()
    makefile = open('Makefile', 'w')
    for line in makefile_lines:
      if 'PROG=nerixsim' in line:
        makefile.write('PROG=nerixsim_' + str(process) + '\n')
      else:
        makefile.write(line)
    makefile.close()
  
    call(['make', 'clean']) 
    call(['python', 'distanceAnalysisLScint.py', str(recoil_energies[job]), str(distances[job])])
    call(['make'])
  
    try:
      pid = os.fork()
      if pid:
        # this is the parent's if statement
        # fork returns process number for parent
        children.append(pid)
	time.sleep(60)
      else:
        print '\n\n\n\n\n\n\n\nFORKED'
        # this is the child's path
        # fork returns None for child
	call(['./nerixsim_' + str(process), '-f', 'macros/minitronConeNoVis.mac', '-n', str(nb_events[job]),  '-o', 'rCoincidenceAnalysis' + str(recoil_energies[job]) + 'keV' + str(distances[job]) + 'cm.root'])
        call(['python', 'distanceAnalysisLScint.py', str(recoil_energies[job]), str(distances[job]), '1'])
        python_output_file = open('distanceAnalysis_output.txt', 'r')
        if python_output_file.readlines()[0].split()[-1] == str(distances[job]):
         	print 'analyzing'
		python_output_file.close()
		if analyze_files:
 		    call(['root', '-l', '-q', 'coincidenceAnalysis.C'])
		analyzed = analyzed+1
        else:
          	python_output_file.close()
          	skipped_analysis.append(job)
        sys.exit(0)
      
    except OSError:
        print 'error: couldn\'t fork!'
        sys.exit(0)
    job = job+1

  for child in children:
    try:
      os.waitpid(child, 0)
    except KeyboardInterrupt:
      for child in children:
        os.kill(child, signal.SIGTERM)
      sys.exit(0)
  print 'all children done'
  print(str(job)+'/'+str(nb_jobs))

if len(skipped_analysis)>0:
  print('Skipped analysis for jobs: ')
  print skipped_analysis
else:
  print(str(analyzed)+' files analyzed.')
