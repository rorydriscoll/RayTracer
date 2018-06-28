#include <System/Dialog.h>
#include <Core/Log.h>
#include <Windows.h>

bool Dialog::GetOpenFilename(char* buffer, int capacity, const char* directory, const char* filter)
{
	buffer[0] = 0;

	OPENFILENAMEA of;

	ZeroMemory(&of, sizeof(of));

	of.lStructSize = sizeof(OPENFILENAME);
	of.hwndOwner = GetActiveWindow();
	of.lpstrFile = buffer;
	of.nMaxFile = capacity;
	of.lpstrFilter = filter;
	of.nFilterIndex = 1;
	of.lpstrFileTitle = nullptr;
	of.nMaxFileTitle = 0;
	of.lpstrInitialDir = directory;
	of.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ENABLESIZING | OFN_NONETWORKBUTTON | OFN_NOCHANGEDIR;

	const BOOL result = GetOpenFileNameA(&of);

	if (result == 0)
	{
		const DWORD error = CommDlgExtendedError();

		if (error != 0)
			LOG_ERROR("Failed to get save filename with CommDlgExtendedError %i", error);
	}

	return result != 0;
}

bool Dialog::GetSaveFilename(char* buffer, int capacity, const char* directory, const char* filter)
{
	buffer[0] = 0;

	OPENFILENAMEA of;

	ZeroMemory(&of, sizeof(of));

	of.lStructSize = sizeof(OPENFILENAME);
	of.hwndOwner = GetActiveWindow();
	of.lpstrFile = buffer;
	of.nMaxFile = capacity;
	of.lpstrFilter = filter;
	of.nFilterIndex = 1;
	of.lpstrFileTitle = nullptr;
	of.nMaxFileTitle = 0;
	of.lpstrInitialDir = directory;
	of.Flags = OFN_OVERWRITEPROMPT | OFN_ENABLESIZING | OFN_NONETWORKBUTTON | OFN_NOCHANGEDIR;

	const BOOL result = GetSaveFileNameA(&of);

	if (result == 0)
	{
		const DWORD error = CommDlgExtendedError();

		if (error != 0)
			LOG_ERROR("Failed to get save filename with CommDlgExtendedError %i", error);
	}

	return result != 0;
}

bool Dialog::ShowYesNo(const char* title, const char* text)
{
	return MessageBoxA(nullptr, text, title, MB_YESNO | MB_ICONWARNING | MB_SYSTEMMODAL) == IDYES;
}
