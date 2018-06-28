#include <System/Thread.h>
#include <System/Windows.h>
#include <Core/Assert.h>

Thread::Thread(EntryPoint entry_point, void* parameter) :
	handle(CreateThread(nullptr, 0, entry_point, parameter, 0, nullptr))
{
}

Thread::Thread(Thread&& other) noexcept : handle(other.handle)
{
	other.handle = nullptr;
}

Thread& Thread::operator=(Thread&& other) noexcept
{
	if (handle)
		CloseHandle(handle);

	handle = other.handle;

	other.handle = nullptr;

	return *this;
}

Thread::~Thread()
{
	if (handle)
		CloseHandle(handle);
}

void Thread::SetIdealProcessor(int i) const
{
	SetThreadIdealProcessor(handle, i);
}

void Thread::Join() const
{
	WaitForSingleObject(handle, INFINITE);
}

void Thread::Sleep(int ms)
{
	::Sleep(ms);
}

void Join(const Thread* threads, int count)
{
	ASSERT(count < MAXIMUM_WAIT_OBJECTS, "Too many threads");

	HANDLE handles[MAXIMUM_WAIT_OBJECTS] = { nullptr };

	for (int i = 0; i < count; ++i)
		handles[i] = threads[i].handle;

	WaitForMultipleObjects(count, handles, true, INFINITE);
}
