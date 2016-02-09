import pycuda.autoinit
import pycuda.driver as drv
import numpy as np
import time
import cuda_full_observables_production
import multiprocessing

from pycuda.compiler import SourceModule
mod = SourceModule(cuda_full_observables_production.cuda_full_observables_production_code, no_extern_c=True)

gpu_full_observables_producetion = mod.get_function('gpu_full_observables_producetion')

threads_per_block = 1024
blocks = 64
num_elements = threads_per_block*blocks

startTime = time.time()

aS1 = np.zeros(num_elements, dtype=np.float32)
aS2 = np.zeros(num_elements, dtype=np.float32)
aEnergy = np.asarray([10]*num_elements, dtype=np.float32)

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

stopLoads = time.time()

iterationStartTime = time.time()
gpu_full_observables_producetion(drv.In(num_trials), drv.Out(aS1), drv.Out(aS2), drv.In(aEnergy), drv.In(photonYield), drv.In(chargeYield), drv.In(excitonToIonRatio), drv.In(g1Value), drv.In(extractionEfficiency), drv.In(gasGainValue), drv.In(gasGainWidth), drv.In(speRes), drv.In(intrinsicResS1), drv.In(intrinsicResS2),
		grid=(blocks,1),
        block=(threads_per_block,1,1))
print 'Total time for array creation: %f' % (stopLoads - startTime)
print 'Total time for %d iterations: %f' % (num_elements, time.time() - iterationStartTime)
print 'Time per 1000 iteration: %f' % (float(time.time() - iterationStartTime) / float(num_elements/1000.))

print aS1
print aS2