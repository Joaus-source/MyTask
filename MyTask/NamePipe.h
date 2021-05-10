#pragma once
#include <windows.h>



typedef struct ContrlDLLMessage
{
	int action;
}CDM,* pCDM;

#define OPENDATAPT (int)1
#define OPENANTIDEBUG (int)2
#define OPENANTIINJECT3 (int)3
#define OPENTHREADPT (int)4

#define CLEARDATAPT (int)5
#define CLEARANTIDEBUG (int)6
#define CLEARANTIINJECT (int)7
#define CLEARTHREADPT (int)8

bool SendData(HANDLE pipe, int action);
bool SendData(int action);
bool SendData(DWORD pid, int action);
bool CreatePipe(DWORD pid);
