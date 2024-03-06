#ifndef ARMADILLO_UTILS_HPP
#define ARMADILLO_UTILS_HPP

#include "file.hpp"
#include "sxor.hpp"
#include "master.hpp"
#include "encrypt.hpp"

int onlyMe();
int getSleep();
char *getUUID();
void saveUUID();
int copyMyself();
void hideWindow();
bool IsUserAdmin();
void unhookNtdll();
char *generateUUID();
bool isDebuggerPresent();
void saveSleep(char *min);
void DeleteRestorePoints();
int calculatePrimeNo(int n);
void executeCommand(const char *command);
bool CopyFileTo(const char *fileName, const char *destDir);

#endif