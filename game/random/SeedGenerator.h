#pragma once
#include <string>
#include "Seed.h"

namespace game::random {
	class SeedGenerator {
	public:
		SeedGenerator() = delete;
		~SeedGenerator() = delete;

		static Seed generateRandomSeed(unsigned int size);
	};
}
