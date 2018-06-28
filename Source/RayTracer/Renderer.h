#pragma once

struct Tile;
struct Window;
struct Camera;
struct Scene;
struct Integrator;

struct Renderer
{
	Renderer(int quality) : quality(quality) {}

	// Render the scene to the window immediately
	//
	void Render(Window& window, const Camera& camera, const Scene& scene, const Integrator& integrator) const;

	// Render a tile
	//
	void RenderTile(Window& window, Tile tile, const Camera& camera, const Scene& scene, const Integrator& integrator) const;

	// Overall quality settings
	//
	int quality = 1;
};
