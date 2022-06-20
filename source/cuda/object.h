#pragma once

#include "cuda/kcuda.h"


namespace kl::cuda {
	template<typename T> class object {
	private:
		T* m_Buffer = nullptr;

	public:
		object() {
			kl::cuda::alloc(m_Buffer, 1);
		}
		object(const T& obj) : object() {
			operator=(obj);
		}
		object(const object& obj) : object() {
			operator=(obj);
		}
		void operator=(const T& obj) {
			*m_Buffer = obj;
		}
		void operator=(const object& obj) {
			*m_Buffer = *obj.m_Buffer;
		}
		~object() {
			kl::cuda::free(m_Buffer);
		}

		T& operator*() {
			return *m_Buffer;
		}
		const T& operator*() const {
			return *m_Buffer;
		}
		T* operator->() {
			return m_Buffer;
		}
		const T* operator->() const {
			return m_Buffer;
		}
	};
}
