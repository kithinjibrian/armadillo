#ifndef ARMADILLO_ENCRYPT_HPP
#define ARMADILLO_ENCRYPT_HPP

#include "master.hpp"

KeyIV genKeyIv();
KeyIV *genIV(KeyIV *keyiv);
char *XOR(char *data, char key);

#endif