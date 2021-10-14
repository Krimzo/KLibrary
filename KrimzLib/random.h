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

		// Fills a vector with random integers
		static void FillVector(std::vector<int>& vectorToFill, int startInclusive, int endExclusive)
		{
			for (int i = 0; i < vectorToFill.size(); i++)
			{
				vectorToFill[i] = GetInt(startInclusive, endExclusive);
			}
		}
	};
}
