#include "shell/builtins/builtins.hpp"

char *dir(char **commands)
{
	free(commands);
	const char *y = "hello\n";
	return (char *)y;
}