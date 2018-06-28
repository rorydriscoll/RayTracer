#pragma once

#include <Image/Image.h>

template<typename T> void DrawRect(Image<T>& image, int x0, int y0, int x1, int y1, T value)
{
	for (int i = y0; i < y1; ++i)
	{
		image(x0, i) = value;
		image(x1, i) = value;
	}

	for (int i = x0; i < x1; ++i)
	{
		image(i, y0) = value;
		image(i, y1) = value;
	}
}
