#include <RayTracer/Tile.h>
#include <Math/Random.h>
#include <Core/Generic.h>
#include <Core/MortonCode.h>

RowTiler::RowTiler(Allocator& allocator, int w, int h, int size) : Tiler(allocator, w, h, size)
{
	for (uint16_t i = 0; i < count; ++i)
		tiles[i] = i;
}

RandomTiler::RandomTiler(Allocator& allocator, int w, int h, int size) : Tiler(allocator, w, h, size)
{
	for (uint16_t i = 0; i < count; ++i)
		tiles[i] = i;

	for (int i = 0; i < count; ++i)
		Swap(tiles[i], tiles[Random::Integer(i, count)]);
}

MortonTiler::MortonTiler(Allocator& allocator, int w, int h, int size) : Tiler(allocator, w, h, size)
{
	int index = 0;
	int code = 0;

	while (index < count)
	{
		int x, y;
		MortonCode::Decode(x, y, code++);

		if (x < tiles.w && y < tiles.h)
			tiles[index++] = uint16_t(y * tiles.w + x);
	}
}
