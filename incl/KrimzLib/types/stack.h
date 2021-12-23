#pragma once


namespace kl {
	template<typename T> struct stack {
		// Pushes an item
		kl::uint64 push(T elem) {
			data <= elem;
		}

		// Pops the last item
		T pop(kl::uint64* updatedSize = nullptr) {
			T tempVal = data.back();
			kl::uint64 newSize = data--;
			if (updatedSize) *updatedSize = newSize;
			return tempVal;
		}

	private:
		kl::array<T> data;
	};
}
