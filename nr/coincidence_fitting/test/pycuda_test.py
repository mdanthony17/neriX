import pycuda.autoinit
import pycuda.driver as drv
import numpy

from pycuda.compiler import SourceModule
mod = SourceModule("""
#include <curand_kernel.h>

extern "C" {

__device__ int gpu_binomial(curandState_t *rand_state, int num_successes, float prob_success)
{
	int x = 0;
	for(int i = 0; i < num_successes; i++) {
    if(curand_uniform(rand_state) < prob_success)
		x++;
	}
	return x;
}

__global__ void multiply_them(int *dest)
{
  const int i = blockIdx.x * blockDim.x + threadIdx.x;
  //dest[i] = a[i] * b[i] + c[0];
  
  //thrust::default_random_engine rng;
  //rng.discard(i);
  //thrust::uniform_real_distribution<float> rand01(0,1);
  //dest[i] = rand01(rng);
  
  //TRandom r3 = TRandom3();
  //dest[i] = r3.Binomial(10, 0.1);
  
  curandState s;

  // seed a random number generator
  curand_init(i, 0, 0, &s);
  //dest[i] = curand_poisson(&s, 1);
  dest[i] = gpu_binomial(&s, 10, 0.1);
  
}

}
""", no_extern_c=True)

multiply_them = mod.get_function("multiply_them")

threads_per_block = 1024
blocks = 512
num_elements = threads_per_block*blocks

a = numpy.random.randn(num_elements).astype(numpy.int32)
b = numpy.random.randn(num_elements).astype(numpy.float32)
c = numpy.asarray(1.).astype(numpy.float32)

#dest = numpy.zeros_like(a)
dest = numpy.zeros(num_elements, dtype=numpy.int32)
multiply_them(
        drv.Out(dest), grid=(blocks,1),
        block=(threads_per_block,1,1))

#print dest-a*b
print list(dest)
#print a*b