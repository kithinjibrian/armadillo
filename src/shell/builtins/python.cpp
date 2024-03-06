#include "shell/builtins/builtins.hpp"

char *script(char **lines)
{
	int num_strings = 0;
	while (lines[num_strings] != NULL)
	{
		num_strings++;
	}

	int total_length = 0;
	for (int i = 0; i < num_strings - 1; i++)
	{
		total_length += strlen(lines[i]);
	}

	char *result = (char *)malloc(total_length + 1);
	if (result == NULL)
	{
		printf("Memory allocation failed\n");
		return NULL;
	}

	int index = 0;
	for (int i = 1; i < num_strings; i++)
	{
		strcpy(result + index, lines[i]);
		index += strlen(lines[i]);
		result[index++] = '\n';
	}

	result[index] = '\0';
	return result;
}

char *python(char **commands)
{
	char *py = script(commands);
	char srcPath[MAX_PATH];
	HRESULT result;
	result = SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, srcPath);
	if (result != S_OK)
	{
		return NULL;
	}
	strcat(srcPath, "python310.dll");
	HINSTANCE hinstLib = LoadLibrary(srcPath);
	if (hinstLib == NULL)
	{
		printf("Failed to load python310.dll\n");
		return NULL;
	}

	FARPROC pyInitializeFunc = GetProcAddress(hinstLib, "Py_Initialize");
	if (pyInitializeFunc == NULL)
	{
		printf("Failed to get the address of Py_Initialize\n");
		FreeLibrary(hinstLib);
		return NULL;
	}

	void (*Py_Initialize)(void) = (void (*)(void))pyInitializeFunc;
	Py_Initialize();

	FARPROC pyRunSimpleStringFunc = GetProcAddress(hinstLib, "PyRun_SimpleString");
	if (pyRunSimpleStringFunc == NULL)
	{
		printf("Failed to get the address of PyRun_SimpleString\n");
		FreeLibrary(hinstLib);
		return NULL;
	}
	void (*PyRun_SimpleString)(const char *) = (void (*)(const char *))pyRunSimpleStringFunc;

	PyRun_SimpleString(py);

	FARPROC pyFinalizeFunc = GetProcAddress(hinstLib, "Py_Finalize");
	if (pyFinalizeFunc == NULL)
	{
		printf("Failed to get the address of Py_Finalize\n");
		FreeLibrary(hinstLib);
		return NULL;
	}
	void (*Py_Finalize)(void) = (void (*)(void))pyFinalizeFunc;
	Py_Finalize();

	free(py);
	FreeLibrary(hinstLib);
	return NULL;
}