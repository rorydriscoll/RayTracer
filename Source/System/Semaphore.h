#pragma once

struct Semaphore
{
	Semaphore() = default;
	Semaphore(const char* name, int count);
	
	Semaphore(const Semaphore&) = delete;
	void operator=(const Semaphore&) = delete;

	Semaphore(Semaphore&& other) = delete;
	void operator=(Semaphore&& other) = delete;

	~Semaphore();

	// Set the semaphore to the max value
	//
	void Raise() const;

	// Wait until the semaphore is non-zero and decrement it
	//
	void Wait() const;

	// The native handle
	//
	void* handle = nullptr;

	// The max count
	//
	int count = 0;
};

