#!/usr/bin/python

import sys, array, os
from subprocess import call
import time

max_processors = 9
for i in xrange(0, max_processors):
	call(['cp', 'nerixsim.cc', 'nerixsim_' + str(i) + '.cc'])
analyzed = 0
if len(sys.argv) == 2:
  max_processors = int(sys.argv[1])


skipped_analysis = []
recoil_energies = [4,  4,  4,   4,   4,   4,   2,   2,   2,   2,   1,   1,   10, 10, 10,  10,  10,  10 ]
distances = 	  [40, 80, 120, 160, 200, 240, 145, 150, 155, 160, 200, 205, 40, 80, 120, 160, 200, 240]
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
        call(['python', 'distanceAnalysisLScint.py', str(recoil_energies[job]), str(distances[job]), '1'])
        python_output_file = open('distanceAnalysis_output.txt', 'r')
        if python_output_file.readlines()[0].split()[-1] == str(distances[job]):
         	print 'analyzing'
		python_output_file.close()
 		call(['root', '-l', '-q', 'coincidenceAnalysis.C'])
#		analyzed = analyzed+1
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
