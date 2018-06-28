#pragma once

struct File
{
	File() = default;

	File(const File&) = delete;
	File& operator=(const File&) = delete;

	File(File&& other) noexcept;
	File& operator=(File&& other) noexcept;

	~File();

	// Open a file with write access
	//
	bool OpenForWrite(const char* filename, size_t filesize);

	// Open a file with read-only access
	//
	bool OpenForRead(const char* filename);

	// Shrink the file to the specified size and close
	//
	void Close(size_t filesize);

	// Close the file
	//
	void Close();

	// Handle to the os file
	//
	void* file = nullptr;

	// Handle to the file mapping
	//
	void* mapping = nullptr;

	// File contents
	//
	void* contents = nullptr;

	// Total file size
	//
	size_t size = 0;
};
