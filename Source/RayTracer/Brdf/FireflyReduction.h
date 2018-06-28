#pragma once

namespace FireflyReduction
{
	// Prepare for a new sample
	//
	void RegisterNewSample();

	// Register that a bounce has occurred at the given roughness value
	//
	void RegisterBounce(float roughness = 1);

	// Get the current roughness to use
	//
	float GetRoughness(float alpha);
}