#import pycuda.autoinit
from pycuda.compiler import SourceModule
import pycuda.driver as drv
import pycuda.tools
import numpy as np
import time
import cuda_full_observables_production
import multiprocessing
import gc

gc.disable()

#import pycuda.autoinit
#mod = SourceModule(cuda_full_observables_production.cuda_full_observables_production_code, no_extern_c=True)
#func = mod.get_function('gpu_full_observables_production')

threads_per_block = 1024
blocks = 64
num_elements = threads_per_block*blocks

lMod = []
lFunc = []

def load_gpu(lMod, lFunc):
	import pycuda.autoinit
	#mod = SourceModule(cuda_full_observables_production.cuda_full_observables_production_code, no_extern_c=True)
	#func = mod.get_function('gpu_full_observables_production')
	lMod.append(SourceModule(cuda_full_observables_production.cuda_full_observables_production_code, no_extern_c=True))
	lFunc.append(mod.get_function('gpu_full_observables_production'))

# ---------------------------------------------
# Time test - CUDA
# ---------------------------------------------


def cuda_multithread_func(numIterations, kwargs_for_func):
	
	# this MUST be in the function call!!
	#drv.init()
	#ctx = drv.Device(0).make_context()
	#ctx.get_device()
	
	print lFunc
	
	aS1 = np.zeros(num_elements, dtype=np.float32)
	aS2 = np.zeros(num_elements, dtype=np.float32)
	aEnergy = np.asarray([10]*num_elements, dtype=np.float32)

	seed = np.asarray(int(time.time()*1000), dtype=np.int32)
	num_trials = np.asarray(num_elements, dtype=np.int32)
	photonYield = np.asarray(7.5, dtype=np.float32)
	chargeYield = np.asarray(5.5, dtype=np.float32)
	excitonToIonRatio = np.asarray(0.9, dtype=np.float32)
	g1Value = np.asarray(0.09, dtype=np.float32)
	extractionEfficiency = np.asarray(0.8, dtype=np.float32)
	gasGainValue = np.asarray(30., dtype=np.float32)
	gasGainWidth = np.asarray(8., dtype=np.float32)
	speRes = np.asarray(0.7, dtype=np.float32)
	intrinsicResS1 = np.asarray(1.0, dtype=np.float32)
	intrinsicResS2 = np.asarray(1.0, dtype=np.float32)
	
	#print num_trials, threads_per_block, blocks
	
	
	tArgs = (drv.In(seed), drv.In(num_trials), drv.Out(aS1), drv.Out(aS2), drv.In(aEnergy), drv.In(photonYield), drv.In(chargeYield), drv.In(excitonToIonRatio), drv.In(g1Value), drv.In(extractionEfficiency), drv.In(gasGainValue), drv.In(gasGainWidth), drv.In(speRes), drv.In(intrinsicResS1), drv.In(intrinsicResS2))
	
	
	for i in xrange(numIterations):
		func(*tArgs, **kwargs_for_func)
		#print aS1
		#print aS2
	



numProcesses = 1
numLoops = 100
lJobs = [0 for i in xrange(numProcesses)]
startTime = time.time()


pool = multiprocessing.Pool(processes=1, initializer=load_gpu, initargs=(lMod, lFunc))



for i in xrange(numProcesses):
	p = multiprocessing.Process(target=cuda_multithread_func, args=(numLoops, {'grid':(blocks,1), 'block':(threads_per_block,1,1)}))
	lJobs[i] = p
	p.start()

for p in lJobs:
	p.join()


print 'Time for %d iterations of %d samples: %f' % (numLoops*numProcesses, num_elements, time.time() - startTime)



