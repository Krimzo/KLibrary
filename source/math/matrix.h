#pragma once

#include <array>
#include <functional>
#include <iomanip>

#include "math/vector4.h"


namespace kl {
	template<typename T, uint64 W, uint64 H> struct matrix : public std::array<T, W* H> {

		matrix() : std::array<T, W* H>() {
			for (uint64 i = 0; i < (W * H); i += (W + 1)) {
				(*this)[i] = 1;
			}
		}

		// Addition
		void add(const kl::matrix<T, W, H>& obj, kl::matrix<T, W, H>& out) const {
			for (uint64 i = 0; i < (W * H); i++) {
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
			for (uint64 i = 0; i < (W * H); i++) {
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
			for (uint64 i = 0; i < (W * H); i++) {
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
		template<uint64 S> void mul(const kl::matrix<T, S, W>& obj, kl::matrix<T, S, H>& out) const {
			for (uint64 y = 0; y < H; y++) {
				for (uint64 x = 0; x < S; x++) {
					out[y * S + x] = {};
					for (uint64 i = 0; i < W; i++) {
						out[y * S + x] += (*this)[y * W + i] * obj[i * S + x];
					}
				}
			}
		}
		template<uint64 S> kl::matrix<T, S, H> operator*(const kl::matrix<T, S, W>& obj) const {
			kl::matrix<T, S, H> temp;
			mul(obj, temp);
			return temp;
		}
		template<uint64 S> void operator*=(const kl::matrix<T, S, W>& obj) {
			mul(obj, *this);
		}
		void mul(const kl::vector2<T>& obj, kl::vector2<T>& out) const {
			if constexpr (W == 2 && H == 2) {
				for (uint64 y = 0; y < 2; y++) {
					T sum = {};
					for (uint64 i = 0; i < 2; i++) {
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
				for (uint64 y = 0; y < 3; y++) {
					T sum = {};
					for (uint64 i = 0; i < 3; i++) {
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
				for (uint64 y = 0; y < 4; y++) {
					T sum = {};
					for (uint64 i = 0; i < 4; i++) {
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

		// Comparison
		bool equ(const kl::matrix<T, W, H>& obj) const {
			for (uint64 i = 0; i < (W * H); i++) {
				if ((*this)[i] != obj[i]) {
					return false;
				}
			}
			return true;
		}
		bool operator==(const kl::matrix<T, W, H>& obj) const {
			return equ(obj);
		}
		bool operator!=(const kl::matrix<T, W, H>& obj) const {
			return !equ(obj);
		}

		// Sign change
		void abs(kl::matrix<T, W, H>& out) const {
			for (uint64 i = 0; i < (W * H); i++) {
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
		void tra(kl::matrix<T, H, W>& out) const {
			for (uint64 y = 0; y < H; y++) {
				for (uint64 x = 0; x < W; x++) {
					out[x * H + y] = (*this)[y * W + x];
				}
			}
		}
		kl::matrix<T, H, W> tra() const {
			kl::matrix<T, H, W> temp;
			tra(temp);
			return temp;
		}

		// Cofactor
		bool cof(uint64 ind, kl::matrix<T, W - 1, H - 1>& out) const {
			if constexpr (W == H) {
				if (ind < (W * H)) {
					uint64 counter = 0;
					const uint64 xInd = ind % W;
					const uint64 yInd = ind / W;
					for (uint64 y = 0; y < H; y++) {
						for (uint64 x = 0; x < W; x++) {
							if (x != xInd && y != yInd) {
								out[counter++] = (*this)[y * W + x];
							}
						}
					}
					return true;
				}
			}
			return false;
		}
		kl::matrix<T, W - 1, H - 1> cof(uint64 ind) const {
			kl::matrix<T, W - 1, H - 1> temp;
			cof(ind, temp);
			return temp;
		}
		bool cof(kl::matrix<T, W, H>& out) const {
			if constexpr (W == H) {
				for (uint64 y = 0; y < H; y++) {
					for (uint64 x = 0; x < W; x++) {
						out[y * W + x] = (((y + x + 2) % 2) ? -1 : 1) * cof(y * W + x).det();
					}
				}
				return true;
			}
			return false;
		}
		kl::matrix<T, W, H> cof() const {
			kl::matrix<T, W, H> temp;
			cof(temp);
			return temp;
		}

		// Determinant
		T det() const {
			if constexpr (W == H) {
				if constexpr (W == 2) {
					return (*this)[0] * (*this)[3] - (*this)[1] * (*this)[2];
				}
				if constexpr (W > 2) {
					T val = {};
					int multi = -1;
					for (uint64 i = 0; i < W; i++) {
						val += (multi *= -1) * (*this)[i] * cof(i).det();
					}
					return val;
				}
			}
			return T(0);
		}

		// Adjoint
		bool adj(kl::matrix<T, W, H>& out) const {
			if constexpr (W == H) {
				out = cof().tra();
				return true;
			}
			return false;
		}
		kl::matrix<T, W, H> adj() const {
			kl::matrix<T, W, H> temp;
			adj(temp);
			return temp;
		}

		// Inverse
		bool inv(kl::matrix<T, W, H>& out) const {
			if constexpr (W == H) {
				const T det = this->det();
				if (det) {
					out = adj() * T(1.0 / det);
					return true;
				}
			}
			return false;
		}
		kl::matrix<T, W, H> inv() const {
			kl::matrix<T, W, H> temp;
			inv(temp);
			return temp;
		}
	};

	// std::cout
	template<typename T, uint64 W, uint64 H> inline std::ostream& operator<<(std::ostream& stream, const kl::matrix<T, W, H>& mat) {
		stream << std::fixed << std::setprecision(2);
		for (uint64 y = 0; y < H; y++) {
			stream << ((y == 0) ? char(218) : (y == (H - 1) ? char(192) : char(179)));
			for (uint64 x = 0; x < (W - 1); x++) {
				stream << std::setw(6) << mat[y * W + x] << " ";
			}
			stream << std::setw(6) << mat[y * W + (W - 1)] << ((y == 0) ? char(191) : (y == (H - 1) ? char(217) : char(179))) << '\n';
		}
		return stream;
	}
}
