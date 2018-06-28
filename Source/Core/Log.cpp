#include <Core/Log.h>
#include <Windows.h>
#include <cstdio>
#include <Core/Assert.h>

namespace
{
	const WORD colors[] =
	{
		0x08,	// Spew: Dark grey on black
		0x07,	// Info: Normal grey on black
		0x0e,	// Warning: Yellow on black
		0x0c,	// Error: Red on black
		0xcf,	// Fatal: White on red
	};

	struct LogSystem
	{
		LogSystem(const char* path, Severity filter) : filter(filter)
		{
			file = CreateFileA(path, GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		}

		LogSystem(const LogSystem&) = delete;
		void operator=(const LogSystem&) = delete;

		LogSystem(LogSystem&&) = delete;
		void operator=(LogSystem&&) = delete;

		~LogSystem()
		{
			if (file != INVALID_HANDLE_VALUE)
				CloseHandle(file);
		}

		void LogMessage(Severity severity, const char* message, size_t length)
		{
			const HANDLE console = severity == Severity::Error ? stderr_handle : stdout_handle;

			SetConsoleTextAttribute(console, colors[int(severity)]);

			WriteFile(file, message, DWORD(length), nullptr, nullptr);
			WriteFile(console, message, DWORD(length), nullptr, nullptr);

			OutputDebugStringA(message);

			SetConsoleTextAttribute(console, colors[int(Severity::Info)]);
		}

		// Current log filter level (messages >= will pass)
		//
		Severity filter = Severity::Info;

		// Console handles
		//
		HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		HANDLE stderr_handle = GetStdHandle(STD_ERROR_HANDLE);

		// File handle for the log file
		//
		HANDLE file = INVALID_HANDLE_VALUE;
	};

	LogSystem* log = nullptr;
}

bool Log::Initialize(Severity filter)
{
	log = new LogSystem("Log.txt", filter);

	if (!log)
		return false;

	LOG_INFO("Log initialized");

	return true;
}

void Log::LogMessage(Severity severity, const char* message, ...)
{
	if (int(severity) < int(log->filter))
		return;

	char buffer[4096];

	va_list values;

	va_start(values, message);
	const size_t length = vsprintf_s(buffer, message, values);
	va_end(values);

	ASSERT(length >= 0 && length < sizeof(buffer) - 1);

	buffer[length + 0] = '\n';
	buffer[length + 1] = '\0';

	log->LogMessage(severity, buffer, length + 1);
}

void Log::Shutdown()
{
	LOG_INFO("Log shut down");

	delete log;
	log = nullptr;
}
