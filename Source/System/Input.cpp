#include <System/Input.h>
#include <System/Windows.h>

bool Input::KeyDown(int code)
{
	return (GetAsyncKeyState(code) & 0x8000) != 0;
}
