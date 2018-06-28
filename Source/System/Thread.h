#pragma once

struct Thread
{
	using EntryPoint = unsigned long(__stdcall*)(void*);

	Thread() = default;
	Thread(EntryPoint entry_point, void* parameter);

	Thread(const Thread&) = delete;
	void operator=(const Thread&) = delete;

	Thread(Thread&& other) noexcept;
	Thread& operator=(Thread&& other) noexcept;

	~Thread();

	// Tie to a specific processor
	//
	void SetIdealProcessor(int i) const;

	// Wait for the thread to die
	//
	void Join() const;

	// Make the current thread go to sleep
	//
	static void Sleep(int ms);

	// The native handle
	//
	void* handle = nullptr;
};

// Wait for multiple threads
//
void Join(const Thread* threads, int count);
