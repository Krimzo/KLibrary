#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>

#include "math/math.h"


namespace kl
{
	namespace random
	{
		bool BOOL();

		byte BYTE();
		kl::color COLOR(bool gray = false);

		int INT(int startInclusive, int endExclusive);
		int INT(int endExclusive);

		float FLOAT(float startInclusive, float endInclusive);
		float FLOAT(float endInclusive);
		float FLOAT();

		char CHAR(bool upper = false);
		std::string STRING(uint length);

		template<typename T>
		inline kl::vector2<T> VECTOR2(T startInclusive, T endInclusive)
		{
			return kl::vector2<T>(
				FLOAT(startInclusive, endInclusive),
				FLOAT(startInclusive, endInclusive)
				);
		}

		template<typename T>
		inline kl::vector2<T> VECTOR2(T endInclusive)
		{
			return kl::vector2<T>(
				FLOAT(endInclusive),
				FLOAT(endInclusive));
		}

		template<typename T>
		inline kl::vector2<T> VECTOR2()
		{
			return kl::vector2<T>(
				FLOAT(),
				FLOAT()
				);
		}

		template<typename T>
		inline kl::vector3<T> VECTOR3(T startInclusive, T endInclusive)
		{
			return kl::vector3<T>(
				FLOAT(startInclusive, endInclusive),
				FLOAT(startInclusive, endInclusive),
				FLOAT(startInclusive, endInclusive)
				);
		}

		template<typename T>
		inline kl::vector3<T> VECTOR3(T endInclusive)
		{
			return kl::vector3<T>(
				FLOAT(endInclusive),
				FLOAT(endInclusive),
				FLOAT(endInclusive)
				);
		}

		template<typename T>
		inline kl::vector3<T> VECTOR3()
		{
			return kl::vector3<T>(
				FLOAT(),
				FLOAT(),
				FLOAT()
				);
		}

		template<typename T>
		inline kl::vector4<T> VECTOR4(T startInclusive, T endInclusive)
		{
			return kl::vector4<T>(
				FLOAT(startInclusive, endInclusive),
				FLOAT(startInclusive, endInclusive),
				FLOAT(startInclusive, endInclusive),
				FLOAT(startInclusive, endInclusive)
				);
		}

		template<typename T>
		inline kl::vector4<T> VECTOR4(T endInclusive)
		{
			return kl::vector4<T>(
				FLOAT(endInclusive),
				FLOAT(endInclusive),
				FLOAT(endInclusive),
				FLOAT(endInclusive)
				);
		}

		template<typename T>
		inline kl::vector4<T> VECTOR4()
		{
			return kl::vector4<T>(FLOAT(), FLOAT(), FLOAT(), FLOAT());
		}

		template<typename T, uint64 W, uint64 H> kl::matrix<T, W, H> inline MATRIX(T startInclusive, T endInclusive)
		{
			kl::matrix<T, W, H> m;
			for (auto& val : m)
			{
				val = FLOAT(startInclusive, endInclusive);
			}
			return m;
		}

		template<typename T, uint64 W, uint64 H> kl::matrix<T, W, H> inline MATRIX(T endInclusive)
		{
			kl::matrix<T, W, H> m;
			for (auto& val : m)
			{
				val = FLOAT(endInclusive);
			}
			return m;
		}

		template<typename T, uint64 W, uint64 H> kl::matrix<T, W, H> inline MATRIX()
		{
			kl::matrix<T, W, H> m;
			for (auto& val : m)
			{
				val = FLOAT();
			}
			return m;
		}
	};
}
