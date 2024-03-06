#ifndef ARMADILLO_STRINGS_HPP
#define ARMADILLO_STRINGS_HPP

#include "master.hpp"

char *stitch(int numStrings, ...);
char **split(char *line, char *delimeter);

#endif