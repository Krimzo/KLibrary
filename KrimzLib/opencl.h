#pragma once


namespace kl
{
	class opencl
	{
	public:
		// Initalizes needed stuff from OpenCL
		static void Init()
		{
			if (!initialized)
			{
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

		// Unnitalizes OpenCL stuff
		static void Uninit()
		{
			if (initialized)
			{
				clFinish(commandQueue);
				clReleaseCommandQueue(commandQueue);
				clReleaseContext(context);
				commandQueue = NULL;
				context = NULL;
				initialized = false;
			}
		}

		// Creates a new gpu buffer
		static gpumem CreateGpuBuffer(size_t byteSize)
		{
			return clCreateBuffer(context, CL_MEM_READ_WRITE, byteSize, NULL, NULL);
		}

		// Deletes a given gpu buffer
		static void Delete(gpumem buffer)
		{
			clReleaseMemObject(buffer);
		}

		// Copies data from the cpu memory to the gpu memory
		static void CpuToGpu(cpumem cpuMem, gpumem gpuMem, size_t byteSize)
		{
			clEnqueueWriteBuffer(commandQueue, gpuMem, CL_TRUE, 0, byteSize, cpuMem, 0, NULL, NULL);
		}

		// Copies data from the gpu memory to the cpu memory
		static void GpuToCpu(gpumem gpuMem, cpumem cpuMem, size_t byteSize)
		{
			clEnqueueReadBuffer(commandQueue, gpuMem, CL_TRUE, 0, byteSize, cpuMem, 0, NULL, NULL);
		}

		// Creates a new OpenCL program
		static clprogram CreateProgram(std::string source)
		{
			const char* kernelSourceAsChar = source.c_str();
			const size_t kernelSourceSize = source.size();
			clprogram tempProgram = clCreateProgramWithSource(context, 1, &kernelSourceAsChar, &kernelSourceSize, NULL);
			clBuildProgram(tempProgram, 1, &deviceID, NULL, NULL, NULL);
			return tempProgram;
		}
		static clprogram CreateProgram(std::wstring filepath)
		{
			std::string source = file::ReadText(filepath);
			const char* kernelSourceAsChar = source.c_str();
			const size_t kernelSourceSize = source.size();
			clprogram tempProgram = clCreateProgramWithSource(context, 1, &kernelSourceAsChar, &kernelSourceSize, NULL);
			clBuildProgram(tempProgram, 1, &deviceID, NULL, NULL, NULL);
			return tempProgram;
		}

		// Deletes a given OpenCL program
		static void Delete(clprogram program)
		{
			clReleaseProgram(program);
		}

		// Creates a new OpenCL kernel
		static clkernel CreateKernel(clprogram program, std::string kernelName)
		{
			return clCreateKernel(program, kernelName.c_str(), NULL);
		}

		// Sets the given kernel arguments
		static void SetKernelArguments(clkernel kernel, std::vector<gpumem> arguments)
		{
			for (int i = 0; i < arguments.size(); i++)
			{
				clSetKernelArg(kernel, i, 8, &arguments[i]);
			}
		}

		// Runs and waits for the kernel to finish
		static void RunKernel(clkernel kernel, size_t runCount)
		{
			if (!math::IsPrime(runCount))
			{
				clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, &runCount, NULL, 0, NULL, NULL);
			}
			else
			{
				kl::console::Print("Kernel run count can't be a prime number!\n", kl::constant::colorRed);
			}
		}

		// Deletes a given OpenCL kernel
		static void Delete(clkernel kernel)
		{
			clReleaseKernel(kernel);
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
