#include "shell/shell.hpp"

char *execute(char *command)
{
	char **commands = split(command, (char *)"\n");
	Function fn;
	fn = builtin(commands[0]);
	if (fn)
	{
		return fn(commands);
	}
	return run(commands);
}