#pragma once

namespace Dialog
{
	// Return a file to open after applying filters e.g. "Clips (*.clip)\0*.clip\0"
	//
	bool GetOpenFilename(char* buffer, int capacity, const char* directory, const char* filter);

	// Return a path to save to after applying filters e.g. "Clips (*.clip)\0*.clip\0"
	//
	bool GetSaveFilename(char* buffer, int capacity, const char* directory, const char* filter);

	// Show a yes/no dialog box
	//
	bool ShowYesNo(const char* title, const char* text);
}