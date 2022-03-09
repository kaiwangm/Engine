#include <stdio.h>

__global__ void helloFromGPU(void) { printf("Hello World from GPU!\n"); }