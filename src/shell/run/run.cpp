#include "shell/run/run.hpp"

char *flatten(char **array)
{
	int rows = sizeof(array) / sizeof(array[0]);
	int total_length = 0;
	for (int i = 0; i < rows; ++i)
	{
		total_length += strlen(array[i]);
	}

	char *flattened = (char *)malloc(total_length + 1);
	if (flattened == NULL)
	{
		// fprintf(stderr, "Memory allocation failed\n");
		exit(1);
	}

	int current_index = 0;
	for (int i = 0; i < rows; ++i)
	{
		strcpy(flattened + current_index, array[i]);
		current_index += strlen(array[i]);
	}

	flattened[current_index] = '\0';

	return flattened;
}

char *run(char **cmds)
{
	char *command_line = flatten(cmds);
	free(cmds);
	HANDLE hReadPipe, hWritePipe;
	SECURITY_ATTRIBUTES saAttr;
	STARTUPINFO siStartInfo;
	PROCESS_INFORMATION piProcInfo;
	BOOL bSuccess;
	DWORD dwRead;
	static CHAR chBuf[BUFFER_SIZE]; // Make chBuf static so it persists after the function returns

	// Set the security attributes for the pipe
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	// Create the pipe
	if (!CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0))
	{
		// fprintf(stderr, "CreatePipe failed\n");
		return NULL;
	}

	// Set up members of the STARTUPINFO structure
	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError = hWritePipe;	 // Redirect standard error to the write end of the pipe
	siStartInfo.hStdOutput = hWritePipe; // Redirect standard output to the write end of the pipe
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	// Create the child process
	if (!CreateProcess(NULL,		 // No module name (use command line)
					   command_line, // Child process command line
					   NULL,		 // Process handle not inheritable
					   NULL,		 // Thread handle not inheritable
					   TRUE,		 // Set handle inheritance to TRUE
					   0,			 // No creation flags
					   NULL,		 // Use parent's environment block
					   NULL,		 // Use parent's starting directory
					   &siStartInfo, // Pointer to STARTUPINFO structure
					   &piProcInfo))
	{ // Pointer to PROCESS_INFORMATION structure
		// fprintf(stderr, "CreateProcess failed\n");
		return NULL;
	}

	// Close the write end of the pipe as we don't need it in this process
	CloseHandle(hWritePipe);

	// Wait for the child process to exit
	WaitForSingleObject(piProcInfo.hProcess, INFINITE);

	// Close process and thread handles
	CloseHandle(piProcInfo.hProcess);
	CloseHandle(piProcInfo.hThread);

	// Read from the pipe
	bSuccess = ReadFile(hReadPipe, chBuf, BUFFER_SIZE, &dwRead, NULL);
	if (!bSuccess || dwRead == 0)
	{
		// fprintf(stderr, "ReadFile from pipe failed\n");
		return NULL;
	}

	// Close the read end of the pipe
	CloseHandle(hReadPipe);

	// Return the read data
	return chBuf;
}