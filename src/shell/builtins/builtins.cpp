#include "shell/builtins/builtins.hpp"

Function builtin(char *command)
{
	Operation operations[] = {
		{"dir", dir},
		{"upload", upload},
		{"python", python},
		{NULL, NULL},
	};
	int i = 0;

	while (operations[i].name)
	{
		if (strcmp(operations[i].name, command) == 0)
		{
			break;
		}
		i++;
	}

	return operations[i].Fn;
}