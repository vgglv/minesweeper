#pragma once
#include <vector>

namespace game::random {
	class Seed {
	public:
		Seed() = default;
		~Seed() = default;

		unsigned int size();
		unsigned int getValue();
		void setSeedSize(unsigned int _seed_size);
		void generate();
		const std::vector<unsigned int>& getGeneratedSeedVector();

	private:
		unsigned int seed_size = 5u;
		unsigned int seed_value = 0u;
		std::vector<unsigned int> generated_seed_vector;
	};
}