#include "random/Seed.h"
#include <random>
#include <chrono>
#include <iostream>

using namespace game::random;

unsigned int Seed::size() {
	return seed_size;
}

void Seed::setSeedSize(unsigned int _seed_size) {
	seed_size = _seed_size;
}

void Seed::generate() {
	std::random_device rd;
	auto current_time = std::chrono::high_resolution_clock::now();
	seed_value = rd() ^ std::chrono::duration_cast<std::chrono::milliseconds>(current_time.time_since_epoch()).count() % 100000;
	std::cout << "Generated seed value: " << seed_value << std::endl;
	generated_seed_vector.clear();
	generated_seed_vector.reserve(seed_size);
	for (unsigned int i = 0; i < seed_size; i++) {
		std::mt19937 rng(seed_value);
		generated_seed_vector.emplace_back(rng());
	}
}

const std::vector<unsigned int>& Seed::getGeneratedSeedVector() {
	return generated_seed_vector;
}