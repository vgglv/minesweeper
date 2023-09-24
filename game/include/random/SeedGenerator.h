#pragma once
#include <string>
#include "random/Seed.h"

namespace game::random {
	class SeedGenerator {
	public:
		SeedGenerator() = delete;
		~SeedGenerator() = delete;

		static Seed generateRandomSeed(unsigned int size);
	};
}
