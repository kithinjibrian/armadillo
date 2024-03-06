#include "shell/builtins/builtins.hpp"

BOOL SaveToFile(LPCTSTR lpszFileName, LPCVOID lpData, DWORD dwDataSize)
{
	HANDLE hFile = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Failed to create file\n");
		return FALSE;
	}

	DWORD dwBytesWritten;
	BOOL bResult = WriteFile(hFile, lpData, dwDataSize, &dwBytesWritten, NULL);
	if (!bResult || dwBytesWritten != dwDataSize)
	{
		fprintf(stderr, "Failed to write to file\n");
		CloseHandle(hFile);
		return FALSE;
	}

	CloseHandle(hFile);
	return TRUE;
}

LPVOID GetDownloadFileContent(const char *url, const char *fileName)
{
	HINTERNET hInternet = InternetOpen("Example", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (!hInternet)
	{
		fprintf(stderr, "Failed to open internet connection\n");
		return nullptr;
	}

	HINTERNET hUrl = InternetOpenUrl(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (!hUrl)
	{
		fprintf(stderr, "Failed to open URL\n");
		InternetCloseHandle(hInternet);
		return nullptr;
	}

	DWORD dwFileSize = 0;
	DWORD dwSize = sizeof(dwFileSize);
	if (!HttpQueryInfo(hUrl, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &dwFileSize, &dwSize, NULL))
	{
		fprintf(stderr, "Failed to get file size\n");
		InternetCloseHandle(hUrl);
		InternetCloseHandle(hInternet);
		return nullptr;
	}

	// Allocate memory to hold the file content
	LPVOID lpFileContent = HeapAlloc(GetProcessHeap(), 0, dwFileSize);
	if (!lpFileContent)
	{
		fprintf(stderr, "Failed to allocate memory for file content\n");
		InternetCloseHandle(hUrl);
		InternetCloseHandle(hInternet);
		return nullptr;
	}

	DWORD bytesRead;
	BOOL success = InternetReadFile(hUrl, lpFileContent, dwFileSize, &bytesRead);
	if (!success || bytesRead != dwFileSize)
	{
		fprintf(stderr, "Failed to read entire file\n");
		InternetCloseHandle(hUrl);
		InternetCloseHandle(hInternet);
		HeapFree(GetProcessHeap(), 0, lpFileContent); // Free allocated memory
		return nullptr;
	}

	// Save content to file
	if (!SaveToFile(fileName, lpFileContent, dwFileSize))
	{
		fprintf(stderr, "Failed to save content to file\n");
		InternetCloseHandle(hUrl);
		InternetCloseHandle(hInternet);
		HeapFree(GetProcessHeap(), 0, lpFileContent); // Free allocated memory
		return nullptr;
	}

	InternetCloseHandle(hUrl);
	InternetCloseHandle(hInternet);
	return lpFileContent;
}

char *upload(char **commands)
{
	// printf("%s %s\n", commands[1], commands[2]);
	LPVOID content = GetDownloadFileContent(commands[1], commands[2]);
	HeapFree(GetProcessHeap(), 0, content);
	return (char *)"GUI initiated";
}
