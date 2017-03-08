#include "Random.h"

std::random_device Random::randomDevice;
std::mt19937 Random::randomNumberGenerator = std::mt19937(randomDevice());

int Random::Range(int min, int max)
{
	std::uniform_int_distribution<int> dis(min, max-1);
	return dis(randomNumberGenerator);
}

float Random::Range(float min, float max)
{
	std::uniform_real_distribution<float> dis(min, max);
	return dis(randomNumberGenerator);
}

double Random::Range(double min, double max)
{
	std::uniform_real_distribution<double> dis(min, max);
	return dis(randomNumberGenerator);
}
