#pragma once
#include <Windows.h>

VOID InitDriverCfg();
BOOL LoadDriver();
BOOL ConnectDriver();
BOOL UnLoadDriver();
BOOL SendDriver(int action,int pid);
void Router(char* buff, int nRcvSize);
