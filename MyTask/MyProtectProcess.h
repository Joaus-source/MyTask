#pragma once
#include "stdafx.h"
#include <windows.h>
#include <TlHelp32.h>
#include "MyHead.h"
#include <Psapi.h>
#include <ws2tcpip.h>
#include "DriverOperation.h"
#include "Dbg.h"
#include "CheatMsg.h"
extern boolean g_DriverInit;

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
	bool openprotect(int ptmask)
	{
		bool bret = true;
		if (!g_DriverInit)
		{
			InitDriverCfg();
			if (LoadDriver())
			{
				ConnectDriver();
				g_DriverInit = true;
			}	
			else
				PrintDbgInfo(_T("load Driver fail."));
		}
		switch (ptmask)
		{
		case dataprotect:

			break;
		case antiinject:

			break;

		case antidebug:

			break;

		case protectthread:

			break;

		case k_antidebug:
			PrintDbgInfo(_T("向驱动发送反调试消息，PID:%d"), th32ProcessID);
			if (SendDriver(SEND_PROTECTED_ACTION_ANTIDEBUG,th32ProcessID))
				states[k_antidebug] = true;
			else
			{
				bret = false;
			}
			break;

		case k_handlept:
			PrintDbgInfo(_T("向驱动发送句柄保护，PID:%d"), th32ProcessID);
			if (SendDriver(SEND_PROTECTED_ACTION_HNADLEPT, th32ProcessID))
				states[k_handlept] = true;
			else
			{
				bret = false;
			}
			break;

		case k_hideprocess:
			PrintDbgInfo(_T("向驱动发送进程隐藏，PID:%d"), th32ProcessID);
			if (SendDriver(SEND_PROTECTED_ACTION_HIDEPROCESS, th32ProcessID))
				states[k_hideprocess] = true;
			else
			{
				bret = false;
			}
			break;

		case k_mmpt:
			PrintDbgInfo(_T("向驱动发送内存保护，PID:%d"), th32ProcessID);
			if (SendDriver(SEND_PROTECTED_ACTION_MMPT, th32ProcessID))
				states[k_mmpt] = true;
			else
			{
				bret = false;
			}
			break;

		default:
			break;
		}
		return bret;
	}
	bool clearprotect(int ptmask)
	{
		bool bret = true;
		switch (ptmask)
		{
		case dataprotect:

			break;
		case antiinject:

			break;

		case antidebug:

			break;

		case protectthread:

			break;

		case k_antidebug:
			if (SendDriver(CLEAR_PROTECTED_ACTION_ANTIDEBUG, th32ProcessID))
				states[k_antidebug] = false;
			else
			{
				bret = false;
			}
			break;

		case k_handlept:
			if (SendDriver(CLEAR_PROTECTED_ACTION_HNADLEPT, th32ProcessID))
				states[k_handlept] = false;
			else
			{
				bret = false;
			}
			break;

		case k_hideprocess:
			if (SendDriver(CLEAR_PROTECTED_ACTION_HIDEPROCESS, th32ProcessID))
				states[k_hideprocess] = false;
			else
			{
				bret = false;
			}
			break;

		case k_mmpt:
			if (SendDriver(CLEAR_PROTECTED_ACTION_MMPT, th32ProcessID))
				states[k_mmpt] = false;
			else
			{
				bret = false;
			}
			break;

		default:
			break;
		}
		return bret;
	}
	bool clearallprotect()
	{
		for (int i=0;i<8;i++)
		{
			if (states[i])
			{
				if(!clearprotect(i))
					return false;
			}
		}
		return true;
	}
	~MyProtectProcess()
	{
		//MessageBox(nullptr,TEXT(""),TEXT("destory"),0);
	}
};

