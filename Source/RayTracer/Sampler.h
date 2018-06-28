#pragma once

#include <Math/Random.h>
#include <Core/Types.h>
#include <Core/Assert.h>
#include <Core/MortonCode.h>
#include <random>

struct Sampler
{
	virtual ~Sampler() {}

	// Start a new pixel
	//
	virtual void StartPixel(int x, int y) = 0;

	// Start the next sample
	//
	virtual void StartSample() = 0;

	// Return the next 2D vector in the current sample
	//
	virtual float2 Get() = 0;

	// Return the max sample count
	//
	virtual int GetSampleCount() const = 0;
};

struct XorShiftRandomSampler : Sampler
{
	XorShiftRandomSampler(int quality) : samples(quality * quality) {}

	void StartPixel(int x, int y) override
	{
		state = MortonCode::Encode(x, y);
	}

	void StartSample() override
	{
	}

	float2 Get() override
	{
		return { Real(), Real() };
	}

	int GetSampleCount() const override
	{
		return samples;
	}

	float Real()
	{
		state += 0x9E3779B97F4A7C15ull;

		uint64_t z = state;

		z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
		z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
		z = (z ^ (z >> 31));

		constexpr float normalization = 1.0f / 0xffffffffffffffffull;

		return z * normalization;
	}

	// Number of samples to provide
	//
	int samples = 0;

	// Current random state
	//
	uint64_t state = 0;
};

struct MersenneTwisterRandomSampler : Sampler
{
	MersenneTwisterRandomSampler(int quality) : samples(quality * quality) {}

	void StartPixel(int x, int y) override
	{
		engine.seed(MortonCode::Encode(x, y));
	}

	void StartSample() override
	{
	}

	float2 Get() override
	{
		return { Real(), Real() };
	}

	int GetSampleCount() const override
	{
		return samples;
	}

	float Real()
	{
		const std::uniform_real_distribution<float> distribution(0, 1);
		return distribution(engine);
	}

	// Number of samples to provide
	//
	int samples = 0;

	// Current random state
	//
	std::mt19937 engine;
};

// Correlated Multi-Jittered Sampling (Andrew Kensler)
//
// https://graphics.pixar.com/library/MultiJitteredSampling/paper.pdf
//
// The idea is that there are n x n strata within the sampling space, and within
// each stratum, there are further n x n substrata. The samples in the substrata
// are arranged in the N-Rooks pattern which guarantees that all substrata will
// receive one sample.
//
// Within each substratum, the sample location will have the standard random
// jitter applied to it to further randomize it.
//
// For example, with 4 x 4 strata, we have 4 cells, each with 4 sub-cells. The
// standard N-rooks sampling pattern puts one sample inside each row and column
// of the substrata:
//
// +---------------+---------------+---------------+---------------+
// | ■ |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
// |---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---|
// |   |   |   |   | ■ |   |   |   |   |   |   |   |   |   |   |   |
// |---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---|
// |   |   |   |   |   |   |   |   | ■ |   |   |   |   |   |   |   |
// |---+---|---+---|---+---|---+---|---+---|---+---|---+---|---+---|
// |   |   |   |   |   |   |   |   |   |   |   |   | ■ |   |   |   |
// +---------------+---------------+---------------+---------------+
// |   | ■ |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
// |---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---|
// |   |   |   |   |   | ■ |   |   |   |   |   |   |   |   |   |   |
// |---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---|
// |   |   |   |   |   |   |   |   |   | ■ |   |   |   |   |   |   |
// |---+---|---+---|---+---|---+---|---+---|---+---|---+---|---+---|
// |   |   |   |   |   |   |   |   |   |   |   |   |   | ■ |   |   |
// +---------------+---------------+---------------+---------------+
// |   |   | ■ |   |   |   |   |   |   |   |   |   |   |   |   |   |
// |---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---|
// |   |   |   |   |   |   | ■ |   |   |   |   |   |   |   |   |   |
// |---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---|
// |   |   |   |   |   |   |   |   |   |   | ■ |   |   |   |   |   |
// |---+---|---+---|---+---|---+---|---+---|---+---|---+---|---+---|
// |   |   |   |   |   |   |   |   |   |   |   |   |   |   | ■ |   |
// +---------------+---------------+---------------+---------------+
// |   |   |   | ■ |   |   |   |   |   |   |   |   |   |   |   |   |
// |---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---|
// |   |   |   |   |   |   |   | ■ |   |   |   |   |   |   |   |   |
// |---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---|
// |   |   |   |   |   |   |   |   |   |   |   | ■ |   |   |   |   |
// |---+---|---+---|---+---|---+---|---+---|---+---|---+---|---+---|
// |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   | ■ |
// +---------------+---------------+---------------+---------------+
//
// The N-rooks pattern is very regular, so this can introduce aliasing artefacts. 
// It is less objectionable to have noise rather than aliasing, so we will actually
// permute the rows and columns of the N-rooks pattern to mix things up.
//
// This applies the same shuffle to both the x and y axis substrata indices. For
// example, after shuffles x = (3, 1, 2, 0), y = (2, 3, 0, 1) we have:
//
// +---------------+---------------+---------------+---------------+
// |   |   |   |   |   |   |   |   |   |   |   |   |   |   | ■ |   |
// |---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---|
// |   |   |   |   |   |   | ■ |   |   |   |   |   |   |   |   |   |
// |---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---|
// |   |   |   |   |   |   |   |   |   |   | ■ |   |   |   |   |   |
// |---+---|---+---|---+---|---+---|---+---|---+---|---+---|---+---|
// |   |   | ■ |   |   |   |   |   |   |   |   |   |   |   |   |   |
// +---------------+---------------+---------------+---------------+
// |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   | ■ |
// |---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---|
// |   |   |   |   |   |   |   | ■ |   |   |   |   |   |   |   |   |
// |---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---|
// |   |   |   |   |   |   |   |   |   |   |   | ■ |   |   |   |   |
// |---+---|---+---|---+---|---+---|---+---|---+---|---+---|---+---|
// |   |   |   | ■ |   |   |   |   |   |   |   |   |   |   |   |   |
// +---------------+---------------+---------------+---------------+
// |   |   |   |   |   |   |   |   |   |   |   |   | ■ |   |   |   |
// |---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---|
// |   |   |   |   | ■ |   |   |   |   |   |   |   |   |   |   |   |
// |---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---|
// |   |   |   |   |   |   |   |   | ■ |   |   |   |   |   |   |   |
// |---+---|---+---|---+---|---+---|---+---|---+---|---+---|---+---|
// | ■ |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
// +---------------+---------------+---------------+---------------+
// |   |   |   |   |   |   |   |   |   |   |   |   |   | ■ |   |   |
// |---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---|
// |   |   |   |   |   | ■ |   |   |   |   |   |   |   |   |   |   |
// |---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---|
// |   |   |   |   |   |   |   |   |   | ■ |   |   |   |   |   |   |
// |---+---|---+---|---+---|---+---|---+---|---+---|---+---|---+---|
// |   |   | ■ |   |   |   |   |   |   |   |   |   |   |   |   |   |
// +---------------+---------------+---------------+---------------+
//
// The thing that makes this usable for path tracing is that we don't have to precompute
// the shuffles. We are able to calculate the permuatations based on the sample index
// and an identifier representing the pattern by using hashing functions.
//
struct CorrelatedMultiJitterSampler : Sampler
{
	CorrelatedMultiJitterSampler(int n) : n(n)
	{
	}

	void StartPixel(int x, int y) override
	{
		pattern = MortonCode::Encode(x, y);
		sample = -1;
	}

	void StartSample() override
	{
		dimension = 0;
		sample++;

		ASSERT(sample < n * n);
	}

	float2 Get() override
	{
		// Each time this is called, we'll increment the pattern index so that it can be called
		// for each bounce within a single pixel and still get random but stratified coverage
		// over multiple passes.

		const int p = pattern + dimension++;

		// It's not a good idea to use the sample index directly since it will pick the same
		// general direction (i.e. somewhere in the stratum) for each bounce. Instead we'll
		// use the permute function to change the stratum order for each sample/pattern combination.

		const int s = Permute(sample, n * n, p * 0x51633e2d);

		// Calculate the permuted substrata indices for the current stratum and pattern.

		const int sx = Permute(s % n, n, p * 0xa511e9b3);
		const int sy = Permute(s / n, n, p * 0x63d83595);

		// Now use random offsets within the substratum to break up the pattern

		const float jx = RandomReal(s, p * 0xa399d265);
		const float jy = RandomReal(s, p * 0x711ad6a5);

		return { (s % n + (sy + jx) / n) / n,  (s / n + (sx + jy) / n) / n };
	}

	int GetSampleCount() const override
	{
		return n * n;
	}

	static float RandomReal(uint i, uint p)
	{
		// This is a repeatable pseudorandom number generator. We don't really require the
		// random numbers to be repeatable, but I'm leaving this in here since it's about as 
		// good as the current split mix random number generator.

		i ^= p;
		i ^= i >> 17;
		i ^= i >> 10; i *= 0xb36534e5;
		i ^= i >> 12;
		i ^= i >> 21; i *= 0x93fc4795;
		i ^= 0xdf6e307f;
		i ^= i >> 17; i *= 1 | p >> 18;

		return i * (1.0f / 4294967808.0f);
	}

	static uint Permute(uint i, uint l, uint p)
	{
		// This is the psuedo random permute function from the paper. Given an input i
		// in the range [0, l] and the pattern index p, it returns a consistent permuted 
		// index in the same range.

		uint w = l - 1;

		w |= w >> 1;
		w |= w >> 2;
		w |= w >> 4;
		w |= w >> 8;
		w |= w >> 16;

		do
		{
			i ^= p; i *= 0xe170893d;
			i ^= p >> 16;
			i ^= (i & w) >> 4;
			i ^= p >> 8; i *= 0x0929eb3f;
			i ^= p >> 23;
			i ^= (i & w) >> 1; i *= 1 | p >> 27;
			i *= 0x6935fa69;
			i ^= (i & w) >> 11; i *= 0x74dcb303;
			i ^= (i & w) >> 2; i *= 0x9e501cc3;
			i ^= (i & w) >> 2; i *= 0xc860a3df;
			i &= w;
			i ^= i >> 5;
		} while (i >= l);

		return (i + p) % l;
	}

	// Square root of the sample count
	//
	int n = 0;

	// Base pattern index
	//
	int pattern = 0;

	// Current dimension
	//
	int dimension = 0;

	// Current sample index
	//
	int sample = -1;
};
