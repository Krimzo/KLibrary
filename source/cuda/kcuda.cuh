#pragma once

#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include "cuda/cu_types.cuh"
#include "utility/console.cuh"


namespace kl::cuda {
	template<typename T> inline void alloc(T*& m_Buffer, uint64 count) {
		kl::console::error(cudaMallocManaged(&m_Buffer, count * sizeof(T)), "Failed to allocate gpu memory");
	}

	template<typename T> inline bool free(T*& m_Buffer) {
		if (m_Buffer) {
			cudaFree(m_Buffer);
			m_Buffer = nullptr;
			return true;
		}
		return false;
	}

	template<typename T> inline void realloc(T*& m_Buffer, uint64 count) {
		kl::cuda::free(m_Buffer);
		kl::cuda::alloc(m_Buffer, count);
	}

	enum class transfer {
		HH = 0, CC = 0,
		HD = 1, CG = 1,
		DH = 2, GC = 2,
		DD = 3, GG = 3,
	};
	template<typename T> inline bool copy(T* to, const void* from, uint64 count, kl::cuda::transfer type) {
		if (to && from && count) {
			cudaMemcpy(to, from, count * sizeof(T), cudaMemcpyKind(type));
			return true;
		}
		return false;
	}

	inline GPU uint64 getX() {
		return uint64(blockIdx.x) * blockDim.x + threadIdx.x;
	}
	inline GPU uint64 getY() {
		return uint64(blockIdx.y) * blockDim.y + threadIdx.y;
	}
	inline GPU uint64 getZ() {
		return uint64(blockIdx.z) * blockDim.z + threadIdx.z;
	}
}
