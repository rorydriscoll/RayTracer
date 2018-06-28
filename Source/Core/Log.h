#pragma once

enum class Severity
{
	Spew,
	Info,
	Warning,
	Error,
	Fatal
}; 

namespace Log
{
	// Initialize all log memory
	//
	bool Initialize(Severity filter);

	// Log a message. It may be filtered out if the log filter is set
	//
	void LogMessage(Severity severity, const char* message, ...);

	// Clean up all log memory
	//
	void Shutdown();
}

#define LOG_SPEW(Message, ...) Log::LogMessage(Severity::Spew, Message, ## __VA_ARGS__)
#define LOG_INFO(Message, ...) Log::LogMessage(Severity::Info, Message, ## __VA_ARGS__)
#define LOG_WARNING(Message, ...) Log::LogMessage(Severity::Warning, Message, ## __VA_ARGS__)
#define LOG_ERROR(Message, ...) Log::LogMessage(Severity::Error, Message, ## __VA_ARGS__)
#define LOG_FATAL(Message, ...) Log::LogMessage(Severity::Fatal, Message, ## __VA_ARGS__)
