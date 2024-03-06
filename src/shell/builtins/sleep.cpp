#include "shell/builtins/builtins.hpp"

char *sleep(char **commands)
{
	saveSleep(commands[1]);
	return (char *)"Sleep Set";
}