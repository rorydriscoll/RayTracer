#include <Math/Random.h>
#include <random>

namespace
{
	thread_local std::mt19937 engine;
}

void Random::SetSeed(int seed)
{
	engine.seed(seed);
}

int Random::Integer(int min, int max)
{
	const std::uniform_int_distribution<int> distribution(min, max - 1);
	return distribution(engine);
}

float Random::Real()
{
	const std::uniform_real_distribution<float> distribution(0, 1);
	return distribution(engine);
}

float3 Random::PointOnSphere()
{	
	const float x = Real() * 2 - 1;
	const float y = Real() * 2 - 1;
	const float z = Real() * 2 - 1;

	return Normalize(float3(x, y, z));
}
