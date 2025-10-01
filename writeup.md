## Warm-up
Assume vector Y, A and B, variable s and C all need to be loaded into memory.

| Kernel | FLOPs / iteration | Memory access / iteration | Arithmetic Intensity |
|--------|-------------------|---------------------------|----------------------| 
| Y[j] += Y[j] + A[j][i] * B[i] | 3 | 32 | 0.09375|
| s += A[i] * A[i] | 2 | 24 | 0.0833 |
| s += A[i] * B[i] | 2 | 32 | 0.0625 |
| Y[i] = A[i] + C*B[i] | 2 | 40 | 0.05 |

## Part 1: Matrix-matrix Multiplication

2. For a given matrix size N, what is the total number of floating point operations performed by this operator?

For each matrix, N^2 memory access, 3N^2 in total.
For each element in matrix C, N add and N mul, 2N ops * N^2 elements = 2N^3 in total.

3. Using the supplied C routine get_walltime.c, or any other accurate means of measuring time, compute the performance in Mflop/s of the matrix-matrix multiply for N=100. Be sure to perform enough repeat calculations of the timing to overcome any statistical noise in the measurement.

sunzeai@unicorn:~/CSE822/CSE822_Project1$ ./matrix_mul 100
matmul took: 0.00525584 seconds
sunzeai@unicorn:~/CSE822/CSE822_Project1$ ./matrix_mul 100
matmul took: 0.011317 seconds
sunzeai@unicorn:~/CSE822/CSE822_Project1$ ./matrix_mul 100
matmul took: 0.00597877 seconds
sunzeai@unicorn:~/CSE822/CSE822_Project1$ ./matrix_mul 100
matmul took: 0.00464253 seconds
sunzeai@unicorn:~/CSE822/CSE822_Project1$ ./matrix_mul 100
matmul took: 0.00688236 seconds

Average time = (0.00525584 + 0.011317 + 0.00597877 + 0.00464253 + 0.00688236) / 5 = 0.0068153 seconds
Total FLOPs = 2(100)^3 = 2000000, 2000000 / 1000 / 1000 = 2 MFLOPs
2 / 0.0068153 = 293 MFLOP/s

4. For the system you are running on, determine the clock speed of the processor and the cache size/layout. Use this information to estimate the theoretical peak performance of the system, assuming that the processor is capable of one flop per clock cycle (generally NOT true on modern architectures). How does the performance you measured in (3) compare to the theoretical peak performance of your system?

CPU max MHz: 5800.0000
CPU min MHz: 800.0000
Take the max MHz, which is about 5.8GHz.

Caches (sum of all):      
  L1d: 896 KiB (24 instances)
  L1i: 1.3 MiB (24 instances)
  L2: 32 MiB (12 instances)
  L3: 36 MiB (1 instance)

Assuming 1 FLOP/cycle, the theoretical peak of the system is 1 FLOP/cycle * 5.8GHz = 5.8GFLOP.

The performance I measured in (3) is a lot lower than this theoretical peak.

5. Now repeat the performance measurement for a range of matrix size N from 1 to 10,000,000. Make a plot of the resulting measured Gflop/s vs. N. On this plot place a horizontal line representing the theoretical peak performance based upon your system's clock speed.