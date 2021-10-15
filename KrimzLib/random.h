#pragma once
#include <iostream>
#include <vector>
#include "KrimzLib/types.h"


namespace kl
{
	class random
	{
	public:
		// Sets the seed for random number generation
		static void SetSeed()
		{
			srand((unsigned)std::time(NULL));
		}

		// Returns a random integer
		static int GetInt(int startInclusive, int endExclusive)
		{
			return rand() % (endExclusive - startInclusive) + startInclusive;
		}

		// Returns a vector with random integers
		static std::vector<int> GetIntVector(int startInclusive, int endExclusive, int vectorSize)
		{
			std::vector<int> tempVec(vectorSize);
			for (int i = 0; i < vectorSize; i++)
				tempVec[i] = GetInt(startInclusive, endExclusive);
			return tempVec;
		}
	};
}
