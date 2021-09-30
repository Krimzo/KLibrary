#pragma once
#include "KrimzLib/types.h"
#include "KrimzLib/incl/OpenCL/cl.hpp"


namespace kl {
	class opencl {
	public:
		// Initalizes needed stuff from OpenCL
		static void Init() {
			if (!initialized) {
				// Get platform and device information
				clGetPlatformIDs(1, &platformID, NULL);
				clGetDeviceIDs(platformID, CL_DEVICE_TYPE_DEFAULT, 1, &deviceID, NULL);

				// Create an OpenCL context
				context = clCreateContext(NULL, 1, &deviceID, NULL, NULL, NULL);

				// Create a command queue
				commandQueue = clCreateCommandQueue(context, deviceID, 0, NULL);
				initialized = true;
			}
		}

		// Creates a new gpu buffer
		static gpumem CreateGpuBuffer(size_t byteSize) {
			return clCreateBuffer(context, CL_MEM_READ_WRITE, byteSize, NULL, NULL);
		}

		// Deletes a given gpu buffer
		static void DeleteGpuBuffer(gpumem buffer) {
			clReleaseMemObject(buffer);
		}

		// Copies data from the cpu memory to the gpu memory
		static void CpuToGpu(cpumem cpuMem, gpumem gpuMem, size_t byteSize) {
			clEnqueueWriteBuffer(commandQueue, gpuMem, CL_TRUE, 0, byteSize, cpuMem, 0, NULL, NULL);
		}

		// Copies data from the gpu memory to the cpu memory
		static void GpuToCpu(gpumem gpuMem, cpumem cpuMem, size_t byteSize) {
			clEnqueueReadBuffer(commandQueue, gpuMem, CL_TRUE, 0, byteSize, cpuMem, 0, NULL, NULL);
		}

	private:
		static bool initialized;
		static cl_platform_id platformID;
		static cl_device_id deviceID;
		static cl_context context;
		static cl_command_queue commandQueue;
	};
	bool opencl::initialized = false;
	cl_platform_id opencl::platformID = NULL;
	cl_device_id opencl::deviceID = NULL;
	cl_context opencl::context = NULL;
	cl_command_queue opencl::commandQueue = NULL;
}
