#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>

#include "Math/Math.h"


namespace kl::Random {
	bool BOOL();

	byte BYTE();
	Color COLOR(bool gray = false);

	int INT(int startInclusive, int endExclusive);
	int INT(int endExclusive);

	float FLOAT(float startInclusive, float endInclusive);
	float FLOAT(float endInclusive);
	float FLOAT();

	char CHAR(bool upper = false);
	String STRING(uint length);

	template<typename T>
	inline Vector2<T> VECTOR2(T startInclusive, T endInclusive) {
		return {
			FLOAT(startInclusive, endInclusive),
			FLOAT(startInclusive, endInclusive)
		};
	}

	template<typename T>
	inline Vector2<T> VECTOR2(T endInclusive) {
		return {
			FLOAT(endInclusive),
			FLOAT(endInclusive)
		};
	}

	template<typename T>
	inline Vector2<T> VECTOR2() {
		return { FLOAT(), FLOAT() };
	}

	template<typename T>
	inline Vector3<T> VECTOR3(T startInclusive, T endInclusive) {
		return {
			FLOAT(startInclusive, endInclusive),
			FLOAT(startInclusive, endInclusive),
			FLOAT(startInclusive, endInclusive)
		};
	}

	template<typename T>
	inline Vector3<T> VECTOR3(T endInclusive) {
		return {
			FLOAT(endInclusive),
			FLOAT(endInclusive),
			FLOAT(endInclusive)
		};
	}

	template<typename T>
	inline Vector3<T> VECTOR3() {
		return { FLOAT(), FLOAT(), FLOAT() };
	}

	template<typename T>
	inline Vector4<T> VECTOR4(T startInclusive, T endInclusive) {
		return {
			FLOAT(startInclusive, endInclusive),
			FLOAT(startInclusive, endInclusive),
			FLOAT(startInclusive, endInclusive),
			FLOAT(startInclusive, endInclusive)
		};
	}

	template<typename T>
	inline Vector4<T> VECTOR4(T endInclusive) {
		return {
			FLOAT(endInclusive),
			FLOAT(endInclusive),
			FLOAT(endInclusive),
			FLOAT(endInclusive)
		};
	}

	template<typename T>
	inline Vector4<T> VECTOR4() {
		return { FLOAT(), FLOAT(), FLOAT(), FLOAT() };
	}

	template<typename T, int W, int H>
	inline Matrix<T, W, H> MATRIX(T startInclusive, T endInclusive) {
		Matrix<T, W, H> m;
		for (auto& val : m) {
			val = FLOAT(startInclusive, endInclusive);
		}
		return m;
	}

	template<typename T, int W, int H>
	inline Matrix<T, W, H> MATRIX(T endInclusive) {
		Matrix<T, W, H> m;
		for (auto& val : m) {
			val = FLOAT(endInclusive);
		}
		return m;
	}

	template<typename T, int W, int H>
	inline Matrix<T, W, H> MATRIX() {
		Matrix<T, W, H> m;
		for (auto& val : m) {
			val = FLOAT();
		}
		return m;
	}
}
