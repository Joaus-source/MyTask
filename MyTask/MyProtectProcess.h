#pragma once
#include "stdafx.h"
#include <windows.h>
#include <TlHelp32.h>
#include "MyHead.h"
#include <Psapi.h>
#include <ws2tcpip.h>

enum protectstates
{
	dataprotect = 0,
	antiinject,
	antidebug,
	protectthread,
	k_handlept,
	k_mmpt,
	k_antidebug,
	k_hideprocess

};

class MyProtectProcess
{
public:
	CString szExeFile;
	DWORD th32ProcessID ;
	DWORD cntThreads;
	DWORD th32ParentProcessID;
	CString szExeFilePath;
	CString WorkingSetSize;
	CString szPriority;
	CString FileMd5;
	bool states[8] = { false };
	MyProtectProcess(CString _szexeFIle,CString _szExeFilePath,CString _FileMd5)
	{
		szExeFile = _szexeFIle;
		szExeFilePath = _szExeFilePath;
		FileMd5 = _FileMd5;
	}
	MyProtectProcess(CString _szexeFIle, CString _szExeFilePath, CString _FileMd5,DWORD _PID)
	{
		szExeFile = _szexeFIle;
		szExeFilePath = _szExeFilePath;
		FileMd5 = _FileMd5;
		th32ProcessID = _PID;
	}
};

