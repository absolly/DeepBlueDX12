#pragma once

#include <random>

class Random
{
public:
	static int Range(int min, int max);
	static float Range(float min, float max);
	static double Range(double min, double max);
private:
		static std::random_device randomDevice;
		static std::mt19937 randomNumberGenerator;
};