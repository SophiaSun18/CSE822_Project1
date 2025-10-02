# CSE 822 Project 1 Write-up

## Warm-up
Assume vector Y, A and B, variable s and C all need to be loaded into memory, and save into memory is also a memory access.

| Kernel | FLOPs / iteration | Memory access / iteration | Arithmetic Intensity |
|--------|-------------------|---------------------------|----------------------| 
| Y[j] += Y[j] + A[j][i] * B[i] | 3 | 32 | 0.09375|
| s += A[i] * A[i] | 2 | 24 | 0.0833 |
| s += A[i] * B[i] | 2 | 32 | 0.0625 |
| Y[i] = A[i] + C*B[i] | 2 | 40 | 0.05 |

## Part 1: Matrix-matrix Multiplication
### All the following test cases are run on HPCC dev-amd24.

2. For a given matrix size N, what is the total number of floating point operations performed by this operator?

For each matrix, N^2 memory access, 3N^2 in total.  
For each element in matrix C, N add and N mul, 2N ops * N^2 elements = 2N^3 in total.

3. Using the supplied C routine get_walltime.c, or any other accurate means of measuring time, compute the performance in Mflop/s of the matrix-matrix multiply for N=100. Be sure to perform enough repeat calculations of the timing to overcome any statistical noise in the measurement.

```
sunzeai@dev-amd24:~/Documents/CSE822_labs/CSE822_Project1$ ./matrix_mul 100
matmul took: 0.00411394 seconds
sunzeai@dev-amd24:~/Documents/CSE822_labs/CSE822_Project1$ ./matrix_mul 100
matmul took: 0.00424058 seconds
sunzeai@dev-amd24:~/Documents/CSE822_labs/CSE822_Project1$ ./matrix_mul 100
matmul took: 0.00413384 seconds
sunzeai@dev-amd24:~/Documents/CSE822_labs/CSE822_Project1$ ./matrix_mul 100
matmul took: 0.0041517 seconds
sunzeai@dev-amd24:~/Documents/CSE822_labs/CSE822_Project1$ ./matrix_mul 100
matmul took: 0.00411555 seconds
```

Average time = 0.004151122 seconds.  
Total FLOPs = 2(100)^3 = 2000000, 2000000 / 1000 / 1000 = 2 MFLOPs.  
2 / 0.004151122 = 482 MFLOP/s.  

4. For the system you are running on, determine the clock speed of the processor and the cache size/layout. Use this information to estimate the theoretical peak performance of the system, assuming that the processor is capable of one flop per clock cycle (generally NOT true on modern architectures). How does the performance you measured in (3) compare to the theoretical peak performance of your system?

Model name: AMD EPYC 9654 96-Core Processor.  
Base clock: 2.4 GHz.  
Max boost clock: 3.7 GHz.  

Caches (sum of all):      
  L1d: 6 MiB (192 instances).  
  L1i: 6 MiB (192 instances).  
  L2: 192 MiB (192 instances).  
  L3: 768 MiB (24 instances).  

Assuming 1 FLOP/cycle, the theoretical peak of the system is 1 FLOP/cycle * 3.7GHz = 3.7GFLOP/s.  
The performance I measured in (3) is a lot lower than this theoretical peak.  

5. Now repeat the performance measurement for a range of matrix size N from 1 to 10,000,000. Make a plot of the resulting measured Gflop/s vs. N. On this plot place a horizontal line representing the theoretical peak performance based upon your system's clock speed.

https://docs.google.com/spreadsheets/d/1HJ5jjuVW6aS_KAYE_9Ultma5y-JfK6XSLCQPEsOfBdo/edit?usp=sharing

6. How does the measured performance for multiple N's compare to peak? Are there any "features" in your plot? Explain them in the context of the hardware architecture of your system. Include in your write-up a description of your system's architecture (processor, cache, etc.).

My system takes a long time to run N=10000, so the maximum measurement I can take is N=10000. All the arithmetic intensity I computed <=10000 is a lot lower than the peak performance of the system, which shows that the matrix multiplication is mainly memory-bound. The arithmetic operations are not expensive, but reading and writing so many numbers into the memory takes up most of the execution time.

## Part 2: The Roofline Model

3. Run the ERT in serial mode on your local machine. Report the peak performances and bandwidths (for all caches levels as well as DRAM). Where is the "ridge point" of the roofline for the various cases?

"FP64 GFLOPs", 62.79 GFLOP/s.  
"L1", 344.7 GB/s.  
"L2", 219.47 GB.s.  
"L3", 93.21 GB/s.  
"DRAM", 48.28 GB/s.  

L1 ridge: 62.79 / 344.7 = 0.1821 FLOPs/byte.  
L2 ridge: 62.79 / 219.47 = 0.286 FLOPs/byte.  
L3 ridge: 62.79 / 93.21 = 0.6736 FLOPs/byte.  
DRAM ridge: 62.79 / 48.28 = 1.3 FLOPs/byte.  

4. Consider the four FP kernels in "Roofline: An Insightful Visual Performance Model for Floating-Point Programs and Multicore Architectures" (see their Table 2). Assuming the high end of operational (i.e., "arithmetic") intensity, how would these kernels perform on the platforms you are testing? What optimization strategy would you recommend to increase performance of these kernels?

SpMV: 0.25 FLOPs/byte * 48.28 GB/s = 12.07 GFLOPs/s.  
LBMHD: 1.07 FLOPs/byte * 48.28 GB/s = 51.59 GFLOPs/s.  
Stencil: 0.5 FLOPs/byte * 48.28 GB/s = 24.14 GFLOPs/s.  
3-D FFT: 1.64 FLOPs/byte * 48.28 GB/s = 79.13 GFLOPs/s.  

The arithmetic intensity of the first 3 kernels are lower than the peak performance of the CPU, mainly memory-bound. To improve memory access efficiency, use tiling/prefetching to improve cache locality, therefore improve the time spent on accessing memory.  
The last one is mainly compute bound. The CPU's arithmetic intensity is lower than the last kernel. Use SIMD to do more arithmetic operations to improve the AI.

5. Address the same questions in (4) for the four kernels given in the Warm-up above.

All the 4 kernels in the warm-up question has lower arithmetic intensity than the CPU, so they're all memory bound. Use the same tiling/prefetching techniques to improve cache locality and the memory access efficiency.

6. Compare your results for the roofline model to what you obtained for the matrix-matrix multiplication operation from Part 1. How are the rooflines of memory bandwidth related to the features in the algorithmic performance as a function of matrix size?

N=1, N=5: the arithmetic intensity falls around L1/L2 ridge, so the matrix can fit into L1 and L2 cache.  
N=10, 50, 100, 1000, 5000, 10000: the arithmetic intensity falls under L3 ridge, so the matrix no longer fits into lower level cache and has to be fetched from either L3 cache or DRAM.  
Performance(N) = min(Memory bandwidth of the cache level that N^2 fits into * Arithmetic intensity of current matrix size N, peak performance of the system)