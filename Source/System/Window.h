#pragma once

#include <Core/Types.h>

struct HWND__;
struct HBITMAP__;

struct Message
{
	enum class Type
	{
		Unknown = -1,

		KeyDown,
		KeyUp
	};

	Message() = default;
	Message(Type type, int data) : type(type), data(data) {}

	// High-level type
	//
	Type type = Type::Unknown;

	// Data
	//
	int data = 0;
};

struct Window
{
	Window(const char* name, uint w, uint h);

	Window(const Window&) = delete;
	void operator=(const Window&) = delete;

	Window(Window&&) = delete;
	void operator=(Window&&) = delete;

	~Window();

	// Add a message to the message queue
	//
	void AddMessage(const Message& message);

	// Read a message from the message queue
	//
	bool ReadMessage(Message& message);

	// Read all window messages and return true if still open
	//
	bool Update();

	// Close the window immediately
	//
	void Close();

	// Blit a compatible bgra texel array to the window buffer (stretching allowed)
	//
	void Blit(void* texels, int sw, int sh) const;

	// Blit a compatible bgra texel array to the window buffer at the given offset (no stretching)
	//
	void Blit(void* texels, int sx, int sy, int sw, int sh) const;

	// Capture the contents of the window to a bgra texel array (stretching allowed)
	//
	bool Capture(void* texels, int sw, int sh) const;

	// Return true if the window has focus
	//
	bool HasFocus() const;

	// Return true if the window is usable
	//
	bool IsValid() const;

	// Dimensions in pixels
	//
	int w = 0, h = 0;

	// Native handle
	//
	HWND__* window = nullptr;

	// Message buffer
	//
	Message messages[64];

	// Message queue offsets
	//
	int read = 0, write = 0;
};
