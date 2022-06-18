#pragma once

#include "cuda/kcuda.cuh"
#include "math/math.cuh"


namespace kl::cuda {
	template<typename... Args> class kernel {
	private:
		void (*m_Kernel)(Args...);

	public:
		uint runs = 0;
		uint threads = 256;

		kernel(void(*kernel)(Args...)) : m_Kernel(kernel) {}

		void run(const Args&... args) const {
			m_Kernel << <(runs / threads) + 1, threads >> > (args...);
			cudaDeviceSynchronize();
		}
	};
}
