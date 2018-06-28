#include <System/Window.h>
#include <System/Windows.h>
#include <Core/Assert.h>
#include <Core/Log.h>

namespace
{
	LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Window* window = static_cast<Window*>(GetPropA(hWnd, "Owner"));

		if (!window)
			return DefWindowProcA(hWnd, msg, wParam, lParam);

		switch (msg)
		{
			case WM_CLOSE:
				window->Close();
				break;

			case WM_SIZE:
				if (wParam != SIZE_MINIMIZED)
				{
					window->w = LOWORD(lParam);
					window->h = HIWORD(lParam);
				}
				break;

			case WM_KEYDOWN:
				window->AddMessage(Message(Message::Type::KeyDown, int(wParam)));
				break;

			case WM_KEYUP:
				window->AddMessage(Message(Message::Type::KeyUp, int(wParam)));
				break;

			default:
				break;
		}

		return DefWindowProcA(hWnd, msg, wParam, lParam);
	}

	RECT CalculateRect(int w, int h, DWORD style)
	{
		RECT rect = { 0, 0, LONG(w), LONG(h) };;

		AdjustWindowRect(&rect, style, FALSE);

		RECT desktop;

		SystemParametersInfoA(SPI_GETWORKAREA, 0, &desktop, 0);

		const int dx = ((desktop.right - desktop.left) - (rect.right - rect.left)) / 2;
		const int dy = ((desktop.bottom - desktop.top) - (rect.bottom - rect.top)) / 2;

		rect.left += dx;
		rect.top += dy;
		rect.right += dx;
		rect.bottom += dy;

		return rect;
	}

	HWND CreateWindow(const char* name, RECT rect, DWORD style)
	{
		WNDCLASSEXA window_class;

		window_class.cbSize = sizeof(WNDCLASSEX);
		window_class.style = CS_CLASSDC;
		window_class.lpfnWndProc = MsgProc;
		window_class.cbClsExtra = 0L;
		window_class.cbWndExtra = 0L;
		window_class.hInstance = GetModuleHandle(nullptr);
		window_class.hIcon = nullptr;
		window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
		window_class.hbrBackground = nullptr;
		window_class.lpszMenuName = nullptr;
		window_class.lpszClassName = "Candy";
		window_class.hIconSm = nullptr;

		RegisterClassExA(&window_class);

		return CreateWindowA("Candy", name, style, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, GetDesktopWindow(), nullptr, window_class.hInstance, nullptr);
	}
}

Window::Window(const char* name, uint w, uint h) :
	w(w), h(h)
{
	const DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	const RECT rect = CalculateRect(w, h, style);

	window = CreateWindow(name, rect, style);

	CRITICAL(window, "Failed to create window");

	SetPropA(window, "Owner", this);
	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);

	//SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
}

Window::~Window()
{
	Close();
}

void Window::AddMessage(const Message& message)
{
	ASSERT(write < countof(messages));

	messages[write++] = message;
}

bool Window::ReadMessage(Message& message)
{
	if (read >= write)
		return false;

	message = messages[read++];

	return true;
}

bool Window::Update()
{
	ASSERT(read == write, "Messages for the previous update were not fully processed");

	read = write = 0;

	MSG msg;

	while (PeekMessageA(&msg, window, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	return window != nullptr;
}

void Window::Close()
{
	if (window)
	{
		DestroyWindow(window);
		UnregisterClassA("Candy", GetModuleHandle(nullptr));
		window = nullptr;
	}
}

void Window::Blit(void* texels, int sw, int sh) const
{
	const HDC dc = GetDC(window);

	BITMAPINFO info;

	ZeroMemory(&info, sizeof(BITMAPINFO));

	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biWidth = sw;
	info.bmiHeader.biHeight = -WORD(sh); // -ve indicates top-down
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biSizeImage = sw * sh * sizeof(uint8_t) * 4;
	info.bmiHeader.biCompression = BI_RGB;

	StretchDIBits(dc, 0, 0, w, h, 0, 0, sw, sh, texels, &info, DIB_RGB_COLORS, SRCCOPY);

	ReleaseDC(window, dc);
}

void Window::Blit(void* texels, int sx, int sy, int sw, int sh) const
{
	const HDC dc = GetDC(window);

	BITMAPINFO info;

	ZeroMemory(&info, sizeof(BITMAPINFO));

	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biWidth = sw;
	info.bmiHeader.biHeight = -WORD(sh); // -ve indicates top-down
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biSizeImage = sw * sh * sizeof(uint8_t) * 4;
	info.bmiHeader.biCompression = BI_RGB;

	StretchDIBits(dc, sx, sy, sw, sh, 0, 0, sw, sh, texels, &info, DIB_RGB_COLORS, SRCCOPY);

	ReleaseDC(window, dc);
}

bool Window::Capture(void* texels, int sw, int sh) const
{
	const HDC display_dc = GetDC(window);
	const HDC capture_dc = CreateCompatibleDC(display_dc);

	const HBITMAP bitmap = CreateCompatibleBitmap(display_dc, sw, sh);

	SelectObject(capture_dc, bitmap);
	SetStretchBltMode(capture_dc, HALFTONE);

	if (!StretchBlt(capture_dc, 0, 0, sw, sh, display_dc, 0, 0, w, h, SRCCOPY | CAPTUREBLT))
	{
		LOG_ERROR("Failed to capture window buffer");
		return false;
	}

	BITMAPINFO info;

	ZeroMemory(&info, sizeof(BITMAPINFO));

	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biWidth = sw;
	info.bmiHeader.biHeight = -WORD(sh); // -ve indicates top down
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biSize = sizeof(info.bmiHeader);
	info.bmiHeader.biCompression = BI_RGB;

	if (!GetDIBits(capture_dc, bitmap, 0, sh, texels, &info, DIB_RGB_COLORS))
	{
		LOG_ERROR("Failed to capture window buffer");
		return false;
	}

	ReleaseDC(window, display_dc);

	DeleteDC(capture_dc);
	DeleteObject(bitmap);

	return true;
}

bool Window::HasFocus() const
{
	return GetFocus() == window;
}

bool Window::IsValid() const
{
	return window != nullptr;
}
