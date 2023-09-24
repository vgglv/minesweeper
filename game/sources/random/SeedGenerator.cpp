#include "random/SeedGenerator.h"

using namespace game::random;

Seed SeedGenerator::generateRandomSeed(unsigned int seed_size) {
	Seed seed;
	seed.setSeedSize(seed_size);
	seed.generate();
	return seed;
}
