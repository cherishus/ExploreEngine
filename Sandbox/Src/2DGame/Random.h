#pragma once
#include <random>

/*
*@ brief: random generator
*@ implementation: random device/random engine/random distribution
*/
class Random
{
public:
	static void Init()
	{
		s_RandomEngine.seed(std::random_device()()); //random device is used to get random value as a seed
	}

	static float Float()
	{
		return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max(); //numeric_limits is used to get min/max value of different type
	}
private:
	static std::mt19937 s_RandomEngine; //result type is unsigned int
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};