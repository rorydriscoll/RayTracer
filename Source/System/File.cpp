#include <System/File.h>
#include <System/Windows.h>
#include <Core/Log.h>

#ifdef _WIN64
size_t ToSize(LARGE_INTEGER li) { return li.QuadPart; }
#else
size_t ToSize(LARGE_INTEGER li) { return li.LowPart; }
#endif

File::File(File&& other) noexcept : 
	file(other.file),
	mapping(other.mapping),
	contents(other.contents),
	size(other.size)
{
	other.file = nullptr;
	other.mapping = nullptr;
	other.contents = nullptr;
}

File& File::operator=(File&& other) noexcept
{
	Close();

	file = other.file;
	mapping = other.mapping;
	contents = other.contents;
	size = other.size;

	other.file = nullptr;
	other.mapping = nullptr;
	other.contents = nullptr;

	return *this;
}

File::~File()
{
	Close();
}

bool File::OpenForWrite(const char* filename, size_t filesize)
{
	file = HANDLE(CreateFileA(filename, GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr));

	if (file == INVALID_HANDLE_VALUE)
	{
		LOG_ERROR("[%i] Couldn't create file handle for %s", GetLastError(), filename);
		return false;
	}

	LARGE_INTEGER li;
	li.QuadPart = filesize;

	mapping = CreateFileMapping(file, nullptr, PAGE_READWRITE, li.HighPart, li.LowPart, nullptr);

	if (!mapping)
	{
		LOG_ERROR("[%i] Couldn't create mapping handle for %s", GetLastError(), filename);
		return false;
	}

	contents = MapViewOfFile(mapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	if (!contents)
	{
		LOG_ERROR("[%i] Couldn't map a view onto %s", GetLastError(), filename);
		return false;
	}

	size = filesize;

	return true;
}

bool File::OpenForRead(const char* filename)
{
	file = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (file == INVALID_HANDLE_VALUE)
	{
		LOG_ERROR("Failed to open file '%s' with error %i", filename, GetLastError());
		return false;
	}

	mapping = CreateFileMapping(file, nullptr, PAGE_READONLY, 0, 0, nullptr);

	if (!mapping)
	{
		LOG_ERROR("Failed to create file mapping for file '%s' with error %i", filename, GetLastError());
		return false;
	}

	contents = MapViewOfFile(mapping, FILE_MAP_COPY, 0, 0, 0);

	if (!contents)
	{
		LOG_ERROR("Failed to map file '%s' with error %i", filename, GetLastError());
		return false;
	}

	LARGE_INTEGER filesize;
	
	if (!GetFileSizeEx(file, &filesize))
	{
		LOG_ERROR("Failed to read the size of file '%s'", filename);
		return false;
	}

	size = ToSize(filesize);

	return true;
}

void File::Close(size_t filesize)
{
	if (contents)
	{
		UnmapViewOfFile(contents);
		contents = nullptr;
	}

	if (mapping)
	{
		CloseHandle(mapping);
		mapping = nullptr;
	}

	if (file)
	{
		LARGE_INTEGER li;
		li.QuadPart = filesize;

		SetFilePointer(file, li.LowPart, &li.HighPart, FILE_BEGIN);
		SetEndOfFile(file);

		CloseHandle(file);

		file = nullptr;
	}
}

void File::Close()
{
	if (contents)
	{
		UnmapViewOfFile(contents);
		contents = nullptr;
	}

	if (mapping)
	{
		CloseHandle(mapping);
		mapping = nullptr;
	}

	if (file)
	{
		CloseHandle(file);
		file = nullptr;
	}
}
