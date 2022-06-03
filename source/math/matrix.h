#pragma once

#include <array>
#include <functional>
#include <iomanip>

#include "math/vector4.h"


namespace kl {
	template<typename T, size_t W, size_t H> struct matrix : public std::array<T, W* H> {

		matrix() : std::array<T, W* H>() {
			for (size_t i = 0; i < (W * H); i += (W + 1)) {
				(*this)[i] = 1;
			}
		}

		// Addition
		void add(const kl::matrix<T, W, H>& obj, kl::matrix<T, W, H>& out) const {
			for (size_t i = 0; i < (W * H); i++) {
				out[i] = (*this)[i] + obj[i];
			}
		}
		kl::matrix<T, W, H> operator+(const kl::matrix<T, W, H>& obj) const {
			kl::matrix<T, W, H> temp;
			add(obj, temp);
			return temp;
		}
		void operator+=(const kl::matrix<T, W, H>& obj) {
			add(obj, *this);
		}

		// Subtraction
		void sub(const kl::matrix<T, W, H>& obj, kl::matrix<T, W, H>& out) const {
			for (size_t i = 0; i < (W * H); i++) {
				out[i] = (*this)[i] - obj[i];
			}
		}
		kl::matrix<T, W, H> operator-(const kl::matrix<T, W, H>& obj) const {
			kl::matrix<T, W, H> temp;
			sub(obj, temp);
			return temp;
		}
		void operator-=(const kl::matrix<T, W, H>& obj) {
			sub(obj, *this);
		}

		// Multiplication
		void mul(const T& val, kl::matrix<T, W, H>& out) const {
			for (size_t i = 0; i < (W * H); i++) {
				out[i] = (*this)[i] * val;
			}
		}
		kl::matrix<T, W, H> operator*(const T& val) const {
			kl::matrix<T, W, H> temp;
			mul(val, temp);
			return temp;
		}
		void operator*=(const T& val) {
			mul(val, *this);
		}
		template<size_t S> void mul(const kl::matrix<T, S, W>& obj, kl::matrix<T, S, H>& out) const {
			for (size_t y = 0; y < H; y++) {
				for (size_t x = 0; x < S; x++) {
					out[y * S + x] = {};
					for (size_t i = 0; i < W; i++) {
						out[y * S + x] += (*this)[y * W + i] * obj[i * S + x];
					}
				}
			}
		}
		template<size_t S> kl::matrix<T, S, H> operator*(const kl::matrix<T, S, W>& obj) const {
			kl::matrix<T, S, H> temp;
			mul(obj, temp);
			return temp;
		}
		template<size_t S> void operator*=(const kl::matrix<T, S, W>& obj) {
			mul(obj, *this);
		}
		void mul(const kl::vector2<T>& obj, kl::vector2<T>& out) const {
			if constexpr (W == 2 && H == 2) {
				for (size_t y = 0; y < 2; y++) {
					T sum = {};
					for (size_t i = 0; i < 2; i++) {
						sum += (*this)[y * 2 + i] * obj[i];
					}
					out[y] = sum;
				}
			}
		}
		kl::vector2<T> operator*(const kl::vector2<T>& obj) const {
			kl::vector2<T> temp;
			mul(obj, temp);
			return temp;
		}
		void mul(const kl::vector3<T>& obj, kl::vector3<T>& out) const {
			if constexpr (W == 3 && H == 3) {
				for (size_t y = 0; y < 3; y++) {
					T sum = {};
					for (size_t i = 0; i < 3; i++) {
						sum += (*this)[y * 3 + i] * obj[i];
					}
					out[y] = sum;
				}
			}
		}
		kl::vector3<T> operator*(const kl::vector3<T>& obj) const {
			kl::vector3<T> temp;
			mul(obj, temp);
			return temp;
		}
		void mul(const kl::vector4<T>& obj, kl::vector4<T>& out) const {
			if constexpr (W == 4 && H == 4) {
				for (size_t y = 0; y < 4; y++) {
					T sum = {};
					for (size_t i = 0; i < 4; i++) {
						sum += (*this)[y * 4 + i] * obj[i];
					}
					out[y] = sum;
				}
			}
		}
		kl::vector4<T> operator*(const kl::vector4<T>& obj) const {
			kl::vector4<T> temp;
			mul(obj, temp);
			return temp;
		}

		// Division
		void div(const T& val, kl::matrix<T, W, H>& out) const {
			const double recVal = 1.0 / val;
			for (size_t i = 0; i < (W * H); i++) {
				out[i] = T((*this)[i] * recVal);
			}
		}
		kl::matrix<T, W, H> operator/(const T& val) const {
			kl::matrix<T, W, H> temp;
			div(val, temp);
			return temp;
		}
		void operator/=(const T& val) {
			div(val, *this);
		}

		// Comparison
		bool equals(const kl::matrix<T, W, H>& obj) const {
			for (size_t i = 0; i < (W * H); i++) {
				if ((*this)[i] != obj[i]) {
					return false;
				}
			}
			return true;
		}
		bool operator==(const kl::matrix<T, W, H>& obj) const {
			return equals(obj);
		}
		bool operator!=(const kl::matrix<T, W, H>& obj) const {
			return !equals(obj);
		}

		// Sign change
		void abs(kl::matrix<T, W, H>& out) const {
			for (size_t i = 0; i < (W * H); i++) {
				out[i] = std::abs((*this)[i]);
			}
		}
		kl::matrix<T, W, H> abs() const {
			kl::matrix<T, W, H> temp;
			abs(temp);
			return temp;
		}
		void neg(kl::matrix<T, W, H>& out) const {
			mul(-1.0f, out);
		}
		kl::matrix<T, W, H> neg() const {
			kl::matrix<T, W, H> temp;
			neg(temp);
			return temp;
		}

		// Transpose
		void transp(kl::matrix<T, H, W>& out) const {
			for (size_t y = 0; y < H; y++) {
				for (size_t x = 0; x < W; x++) {
					out[x * H + y] = (*this)[y * W + x];
				}
			}
		}
		kl::matrix<T, H, W> transp() const {
			kl::matrix<T, H, W> temp;
			transp(temp);
			return temp;
		}

		// std::cout
		template<typename T, size_t W, size_t H> friend std::ostream& operator<<(std::ostream& stream, const kl::matrix<T, W, H>& mat) {
			stream << std::fixed << std::setprecision(2);
			for (size_t y = 0; y < H; y++) {
				stream << ((y == 0) ? char(218) : (y == (H - 1) ? char(192) : char(179)));
				for (size_t x = 0; x < (W - 1); x++) {
					stream << mat[y * W + x] << " ";
				}
				stream << mat[y * W + (W - 1)] << ((y == 0) ? char(191) : (y == (H - 1) ? char(217) : char(179))) << '\n';
			}
			return stream;
		}
	};
}
