#include <System/Semaphore.h>
#include <System/Windows.h>

Semaphore::Semaphore(const char* name, int count) : 
	handle(CreateSemaphoreA(nullptr, 0, count, name)), count(count)
{
}

Semaphore::~Semaphore()
{
	if (handle)
		CloseHandle(handle);
}

void Semaphore::Raise() const
{
	ReleaseSemaphore(handle, count, nullptr);
}

void Semaphore::Wait() const
{
	WaitForSingleObject(handle, INFINITE);
}
