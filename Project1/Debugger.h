#pragma once

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string>

class Debug
{
public:
	Debug()
	{
		AllocConsole();
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	}

	void Log(std::string message)
	{
		std::cout << message << std::endl;
	}
};