#include <System/Host.h>
#include <System/Windows.h>

int Host::GetCpuCoreCount()
{
	SYSTEM_INFO sysinfo;

	GetSystemInfo(&sysinfo);

	return sysinfo.dwNumberOfProcessors;
}
