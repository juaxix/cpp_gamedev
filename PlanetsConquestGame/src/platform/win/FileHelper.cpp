#include "FileHelper.h"
#include <windows.h>
#include <iostream>

std::string FileHelper::resourcesDirectory()
{
	char ownPth[MAX_PATH];

	// Will contain exe path
	HMODULE hModule = GetModuleHandle(NULL);
	if (hModule != NULL)
	{
		// When passing NULL to GetModuleHandle, it returns handle of exe itself
		GetModuleFileName(hModule, ownPth, (sizeof(ownPth)));

		// Use above module handle to get the path using GetModuleFileName()
		std::string path = ownPth;
		size_t idx = path.find_last_of("\\");
		
		// remove the .exe part
		path = path.substr(0, idx);

		// go back to root directory where resources dir is
		path = path + std::string("\\..\\..\\..\\");
		
		return path;
	}
	else
	{
		std::cout << "Module handle is NULL" << std::endl;

		return "";
	}
}