#pragma once


#define GLOBAL __global__
#define DEVICE __device__
#define HOST __host__
#define SHRD __shared__
#define CNST __constant__

#define RUN GLOBAL
#define GPU DEVICE
#define CPU HOST
#define ALL CPU GPU
