#pragma once

namespace String
{
	// Printf-style formatting to a buffer. Returns the formatted size on success, and size on error.
	//
	size_t Format(char* buffer, size_t size, const char* format, ...);

	// Copy
	//
	void Copy(char* target, const char* source, size_t count);
};
