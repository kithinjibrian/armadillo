#ifndef ARMADILLO_BUILTINS_HPP
#define ARMADILLO_BUILTINS_HPP

#include "utils.hpp"
#include "master.hpp"

Function builtin(char *command);

/*builtin commands*/
char *dir(char **commands);
char *upload(char **commands);
char *python(char **commands);

#endif