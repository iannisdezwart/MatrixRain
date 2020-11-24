#ifndef RANDOM_HEADER
#define RANDOM_HEADER

#include <bits/stdc++.h>

using namespace std;

class RandomIntGenerator {
	private:
		random_device device;
		mt19937 generator;
		uniform_int_distribution<uint64_t> distribution;

	public:
		RandomIntGenerator(uint64_t lower_bound, uint64_t upper_bound)
		{
			generator = mt19937(device());
			distribution = uniform_int_distribution<uint64_t>(lower_bound, upper_bound);
		}

		uint64_t generate()
		{
			return distribution(generator);
		}
};

RandomIntGenerator random_int_generator(0, 18446744073709551615U);

uint64_t random_int()
{
	return random_int_generator.generate();
}

uint64_t random_int(uint64_t lower_bound, uint64_t upper_bound)
{
	uint64_t random = random_int_generator.generate();
	return (random % (upper_bound - lower_bound + 1)) + lower_bound;
}

class RandomFloatGenerator {
	private:
		RandomIntGenerator random_int_generator;

	public:
		RandomFloatGenerator() : random_int_generator(0, 9223372036854775807) {}

		double generate()
		{
			return (double) random_int_generator.generate() / 9223372036854775807.0;
		}
};

RandomFloatGenerator random_float_generator;

double random_float()
{
	return random_float_generator.generate();
}

#endif