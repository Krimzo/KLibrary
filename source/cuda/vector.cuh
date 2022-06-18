#pragma once

#include "cuda/kcuda.cuh"
#include "math/math.cuh"


namespace kl::cuda {
	template<typename T> class vector {
	private:
		T* m_Buffer = nullptr;
		uint64 m_Size = 0;

	public:
		vector() {}
		vector(uint64 size) {
			resize(size);
		}
		vector(const vector<T>& obj) : vector(obj.m_Size) {
			kl::cuda::copy(m_Buffer, obj.m_Buffer, m_Size, kl::cuda::transfer::DD);
		}
		vector& operator=(const vector& obj) {
			resize(obj.size());
			kl::cuda::copy(m_Buffer, obj.m_Buffer, m_Size, kl::cuda::transfer::DD);
		}
		~vector() {
			clear();
		}

		T& operator[](uint64 ind) {
			if (ind < m_Size) {
				return m_Buffer[ind];
			}
			std::cout << "Cuda vector out of scope!" << std::endl;
			exit(69);
		}
		const T& operator[](uint64 ind) const {
			if (ind < m_Size) {
				return m_Buffer[ind];
			}
			std::cout << "Cuda vector out of scope!" << std::endl;
			exit(69);
		}

		T& front() {
			return operator[](0);
		}
		const T& front() const {
			return operator[](0);
		}
		T& back() {
			return operator[](m_Size - 1);
		}
		const T& back() const {
			return operator[](m_Size - 1);
		}

		T* pointer() const {
			return m_Buffer;
		}
		uint64 size() const {
			return m_Size;
		}

		bool resize(uint64 newSize) {
			if (newSize != m_Size) {
				T* tempBuffer = nullptr;
				kl::cuda::alloc(tempBuffer, m_Size);
				kl::cuda::copy(tempBuffer, m_Buffer, m_Size, kl::cuda::transfer::DD);
				kl::cuda::realloc(m_Buffer, newSize);
				kl::cuda::copy(m_Buffer, tempBuffer, std::min(m_Size, newSize), kl::cuda::transfer::DD);
				kl::cuda::free(tempBuffer);
				m_Size = newSize;
				return true;
			}
			return false;
		}
		void clear() {
			kl::cuda::free(m_Buffer);
			m_Size = 0;
		}

		T& push_back(const T& obj) {
			resize(m_Size + 1);
			back() = obj;
			return back();
		}

		void toCPU(T* cpuBuff, uint64 size = 0) const {
			kl::cuda::copy(cpuBuff, m_Buffer, (size > 0 && size < m_Size) ? size : m_Size, kl::cuda::transfer::DH);
		}
		void fromCPU(const T* cpuBuff, uint64 size = 0) {
			kl::cuda::copy(m_Buffer, cpuBuff, (size > 0 && size < m_Size) ? size : m_Size, kl::cuda::transfer::HD);
		}

		void operator>>(T* cpuBuff) const {
			toCPU(cpuBuff);
		}
		void operator<<(const T* cpuBuff) {
			fromCPU(cpuBuff);
		}
	};
}
