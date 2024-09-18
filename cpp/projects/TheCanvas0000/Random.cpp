#include <cstdlib>

namespace Random
{
	//#include <cstdlib>
	//#include <ctime>

	float generate_random_float_0_to_1()
	{
		// Seed the random number generator with the current time
		//std::srand(static_cast<unsigned>(std::time(nullptr)));
		// be awere when you reset the seed value

		// Generate a random float between 0.0 and 1.0
		return static_cast<float>(std::rand()) / RAND_MAX;
	}

	float generate_random_float_minus_one_to_plus_one()
	{
		return (generate_random_float_0_to_1() - 0.5f) * 2.0f;
	}
}