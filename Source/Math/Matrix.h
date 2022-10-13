#pragma once

#include <array>
#include <format>
#include <iomanip>
#include <functional>

#include "Math/Vector4.h"


namespace kl {
	template<typename T, int W, int H>
	struct Matrix : public std::array<T, W * H> {

		Matrix() : std::array<T, W* H>() {
			if constexpr (W == H) {
				for (int i = 0; i < (W * H); i += (W + 1)) {
					get(i) = 1;
				}
			}
		}

		T& get(int index) {
			return (*this)[index];
		}

		const T& get(int index) const {
			return (*this)[index];
		}

		T& get(int x, int y) {
			return (*this)[y * W + x];
		}

		const T& get(int x, int y) const {
			return (*this)[y * W + x];
		}

		template<typename T0>
		operator Matrix<T0, W, H>() const {
			Matrix<T0, W, H> temp;
			for (int i = 0; i < (W * H); i++) {
				temp[i] = T0(get(i));
			}
			return temp;
		}

		Matrix<T, W, H> operator+(const Matrix<T, W, H>& obj) const {
			Matrix<T, W, H> result;
			for (int i = 0; i < (W * H); i++) {
				result[i] = get(i) + obj[i];
			}
			return result;
		}

		void operator+=(const Matrix<T, W, H>& obj) {
			*this = *this + obj;
		}

		Matrix<T, W, H> operator-(const Matrix<T, W, H>& obj) const {
			Matrix<T, W, H> result;
			for (int i = 0; i < (W * H); i++) {
				result[i] = get(i) - obj[i];
			}
			return result;
		}

		void operator-=(const Matrix<T, W, H>& obj) {
			*this = *this - obj;
		}

		Matrix<T, W, H> operator*(const T& val) const {
			Matrix<T, W, H> result;
			for (int i = 0; i < (W * H); i++) {
				result[i] = get(i) * val;
			}
			return result;
		}

		void operator*=(const T& val) {
			*this = *this * val;
		}

		template<int S>
		Matrix<T, S, H> operator*(const Matrix<T, S, W>& obj) const {
			Matrix<T, S, H> result = {};
			for (int y = 0; y < H; y++) {
				for (int x = 0; x < S; x++) {
					result.get(x, y) = {};
					for (int i = 0; i < W; i++) {
						result.get(x, y) += get(i, y) * obj.get(x, i);
					}
				}
			}
			return result;
		}

		template<int S>
		void operator*=(const Matrix<T, S, W>& obj) {
			*this = *this * obj;
		}

		Vector2<T> operator*(const Vector2<T>& obj) const {
			Vector2<T> result;
			if constexpr (W == 2 && H == 2) {
				for (int y = 0; y < 2; y++) {
					for (int i = 0; i < 2; i++) {
						result[y] += get(i, y) * obj[i];
					}
				}
			}
			return result;
		}

		Vector3<T> operator*(const Vector3<T>& obj) const {
			Vector3<T> result;
			if constexpr (W == 3 && H == 3) {
				for (int y = 0; y < 3; y++) {
					for (int i = 0; i < 3; i++) {
						result[y] += get(i, y) * obj[i];
					}
				}
			}
			return result;
		}

		Vector4<T> operator*(const Vector4<T>& obj) const {
			Vector4<T> result;
			if constexpr (W == 4 && H == 4) {
				for (int y = 0; y < 4; y++) {
					for (int i = 0; i < 4; i++) {
						result[y] += get(i, y) * obj[i];
					}
				}
			}
			return result;
		}

		bool operator==(const Matrix<T, W, H>& obj) const {
			for (int i = 0; i < (W * H); i++) {
				if (get(i) != obj[i]) {
					return false;
				}
			}
			return true;
		}

		bool operator!=(const Matrix<T, W, H>& obj) const {
			return !(*this == obj);
		}

		Matrix<T, W, H> absolute() const {
			Matrix<T, W, H> result;
			for (int i = 0; i < (W * H); i++) {
				result[i] = std::abs(get(i));
			}
			return result;
		}

		Matrix<T, W, H> negate() const {
			return *this * -1.0f;
		}

		Matrix<T, H, W> transpose() const {
			Matrix<T, H, W> result;
			for (int y = 0; y < H; y++) {
				for (int x = 0; x < W; x++) {
					result.get(x, y) = get(y, x);
				}
			}
			return result;
		}

		Matrix<T, W - 1, H - 1> cofactor(int index) const {
			return cofactor(index % W, index / W);
		}

		Matrix<T, W - 1, H - 1> cofactor(int X, int Y) const {
			Matrix<T, W - 1, H - 1> result;
			if constexpr (W == H) {
				int counter = 0;
				for (int y = 0; y < H; y++) {
					for (int x = 0; x < W; x++) {
						if (x != X && y != Y) {
							result[counter++] = get(x, y);
						}
					}
				}
			}
			return result;
		}

		Matrix<T, W, H> cofactor() const {
			Matrix<T, W, H> result;
			if constexpr (W == H) {
				for (int y = 0; y < H; y++) {
					for (int x = 0; x < W; x++) {
						result.get(x, y) = (((y + x + 2) % 2) ? -1 : 1) * cofactor(y * W + x).determinant();
					}
				}
			}
			return result;
		}

		T determinant() const {
			if constexpr (W == H) {
				if constexpr (W == 2) {
					return get(0) * get(3) - get(1) * get(2);
				}
				if constexpr (W > 2) {
					T result = {};
					int multi = -1;
					for (int i = 0; i < W; i++) {
						result += (multi *= -1) * get(i) * cofactor(i).determinant();
					}
					return result;
				}
			}
			return {};
		}

		Matrix<T, W, H> adjoint() const {
			if constexpr (W == H) {
				return cofactor().transpose();
			}
			return {};
		}

		Matrix<T, W, H> inverse() const {
			if constexpr (W == H) {
				T deter = determinant();
				if (deter) {
					return adjoint() * T(1.0f / deter);
				}
			}
			return {};
		}
	};

	template<typename T, int W, int H>
	inline std::ostream& operator<<(std::ostream& stream, const Matrix<T, W, H>& mat) {
		int maxLenghts[W] = {};
		String outputData[W * H] = {};

		for (int x = 0; x < W; x++) {
			for (int y = 0; y < H; y++) {
				outputData[y * W + x] = std::format("{:.2f}", mat[y * W + x]);
				maxLenghts[x] = std::max(maxLenghts[x], int(outputData[y * W + x].size()));
			}
		}

		for (int y = 0; y < H; y++) {
			stream << ((y == 0) ? char(218) : (y == (H - 1) ? char(192) : char(179)));
			for (int x = 0; x < (W - 1); x++) {
				stream << std::setw(maxLenghts[x]) << outputData[y * W + x] << " ";
			}
			stream << std::setw(maxLenghts[W - 1]) << outputData[y * W + (W - 1)]
				<< ((y == 0) ? char(191) : (y == (H - 1) ? char(217) : char(179)))
				<< (y == (H - 1) ? "" : "\n");
		}
		
		return stream;
	}
}
