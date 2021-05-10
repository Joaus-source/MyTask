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
#include "NamePipe.h"
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
	bool m_DLLinit = false;
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
		if (ptmask <= protectthread && (!m_DLLinit))
		{
			if (InjectProcess(th32ProcessID))
			{
				PrintDbgInfo(_T("Inject Success"));

				m_DLLinit = true;
			}
			else
			{
				PrintDbgInfo(_T("Inject Failed"));
				bret = false;
				return bret;
			}

		}
		if (!g_DriverInit && ptmask > protectthread && ptmask <= k_hideprocess)
		{
			InitDriverCfg();
			if (LoadDriver())
			{
				ConnectDriver();
				g_DriverInit = true;
			}	
			else
			{
				PrintDbgInfo(_T("load Driver fail."));
				bret = false;
				return bret;
			}

		}
		switch (ptmask)
		{
		case dataprotect:
			if (SendData(th32ProcessID,OPENDATAPT))
			{
				PrintDbgInfo(_T("向DLL发送消息成功，PID:%d"), th32ProcessID);
				states[dataprotect] = true;
			}
			else
			{
				bret = false;
			}
			break;
		case antiinject:
			if (SendData(th32ProcessID,OPENANTIINJECT3))
			{
				PrintDbgInfo(_T("向DLL发送消息OPENANTIINJECT3成功，PID:%d"), th32ProcessID);
				states[antiinject] = true;
			}
			else
			{
				bret = false;
			}
			break;

		case antidebug:
			if (SendData(th32ProcessID,OPENANTIDEBUG))
			{
				PrintDbgInfo(_T("向DLL发送消息OPENANTIDEBUG成功，PID:%d"), th32ProcessID);
				states[antidebug] = true;
			}
			else
			{
				bret = false;
			}
			break;

		case protectthread:
			if (SendData(th32ProcessID,OPENTHREADPT))
			{
				PrintDbgInfo(_T("向DLL发送消息OPENTHREADPT成功，PID:%d"), th32ProcessID);
				states[protectthread] = true;
			}
			else
			{
				bret = false;
			}
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
			if (SendData(th32ProcessID,CLEARDATAPT))
			{
				PrintDbgInfo(_T("向DLL发送消息CLEARDATAPT成功，PID:%d"), th32ProcessID);
				states[dataprotect] = false;
			}
			else
			{
				bret = false;
			}
			break;
		case antiinject:
			if (SendData(th32ProcessID,CLEARANTIINJECT))
			{
				PrintDbgInfo(_T("向DLL发送消息CLEARANTIINJECT成功，PID:%d"), th32ProcessID);
				states[antiinject] = false;
			}
			else
			{
				bret = false;
			}
			break;

		case antidebug:
			if (SendData(th32ProcessID,CLEARANTIDEBUG))
			{
				PrintDbgInfo(_T("向DLL发送消息CLEARANTIDEBUG成功，PID:%d"), th32ProcessID);
				states[antidebug] = false;
			}
			else
			{
				bret = false;
			}
			break;

		case protectthread:
			if (SendData(th32ProcessID,CLEARTHREADPT))
			{
				PrintDbgInfo(_T("向DLL发送消息CLEARTHREADPT成功，PID:%d"), th32ProcessID);
				states[protectthread] = false;
			}
			else
			{
				bret = false;
			}
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

	bool InjectProcess(DWORD pid)
	{
		bool bret = false;
		BOOLEAN Old;
		typedef int(__stdcall* PRtlAdjustPrivilege)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
		PRtlAdjustPrivilege RtlAdjustPrivilege =
			(PRtlAdjustPrivilege)GetProcAddress(GetModuleHandle(_T("ntdll")), "RtlAdjustPrivilege");
		RtlAdjustPrivilege(0x14, TRUE, FALSE, &Old);//提权到DEBUG权限！
		TCHAR szModulePath[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, szModulePath, _countof(szModulePath));
		PathRemoveFileSpec(szModulePath);
		TCHAR szDLLPath[MAX_PATH] = { 0 };
		_stprintf_s(szDLLPath, _countof(szDLLPath), _T("%s\\%s"), szModulePath, _T("AntiCheat.dll"));
		PrintDbgInfo(szDLLPath);

		HANDLE targetprocess = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
		DWORD injBufSize = lstrlen((LPCWSTR)szDLLPath) + 1;
		LPVOID AllocAddr = VirtualAllocEx(targetprocess, NULL, injBufSize*2, MEM_COMMIT, PAGE_READWRITE);
		PrintDbgInfo(_T("AllocAddr : 0x%p"), AllocAddr);
		WriteProcessMemory(targetprocess, AllocAddr, (void*)szDLLPath, injBufSize*2, NULL);
		DWORD ER = GetLastError();
		PTHREAD_START_ROUTINE pfnStartAddr = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
		//cout << "The LoadLibrary's Address is:" << pfnStartAddr << endl;
		PrintDbgInfo(_T("The LoadLibrary's Address is: 0x%p"), pfnStartAddr);
		HANDLE hRemoteThread;
		if ((hRemoteThread = CreateRemoteThread(targetprocess, NULL, 0, pfnStartAddr, AllocAddr, 0, NULL)) == NULL)
		{
			ER = GetLastError();
			PrintDbgInfo(_T("Create Thread Failed"));
			bret = FALSE;
		}
		else
		{
			PrintDbgInfo(_T("Create Thread Success"));
			bret = TRUE;
		}
		CloseHandle(hRemoteThread);
		CloseHandle(targetprocess);
		return bret;
	}
	~MyProtectProcess()
	{
		//MessageBox(nullptr,TEXT(""),TEXT("destory"),0);
	}
};

