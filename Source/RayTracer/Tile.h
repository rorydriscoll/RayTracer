#pragma once

#include <Core/Types.h>
#include <Image/Image.h>

struct Tile
{
	Tile() = default;
	Tile(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}

	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
};

struct Tiler
{
	Tiler(Allocator& allocator, int w, int h, int size) : tiles(allocator, (w + size - 1) / size, (h + size - 1) / size), size(size), count(tiles.w * tiles.h)
	{
	}

	// GenerateTile a tile
	//
	Tile GenerateTile(int i) const
	{
		const int t = tiles[i];

		const int x = (t % tiles.w) * size;
		const int y = (t / tiles.w) * size;

		return { x, y, size, size };
	}

	// Tile indices
	//
	Image<uint16_t> tiles;

	// Tile size in pixels
	//
	int size = 1;
	
	// Number of tiles
	//
	int count = 0;
};

struct RowTiler : Tiler
{
	RowTiler(Allocator& allocator, int w, int h, int size);
};

struct RandomTiler : Tiler
{
	RandomTiler(Allocator& allocator, int w, int h, int size);
};

struct MortonTiler : Tiler
{
	MortonTiler(Allocator& allocator, int w, int h, int size);
};
