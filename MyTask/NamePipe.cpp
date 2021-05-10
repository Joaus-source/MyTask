#include "NamePipe.h"
#include "Dbg.h"
#include <tchar.h>
#include <stdlib.h>
HANDLE pipe;

bool SendData(DWORD pid,int action)
{
	if (CreatePipe(pid))
	{
		PrintDbgInfo(_T("CreatePipe Success"));
	}
	else
	{
		PrintDbgInfo(_T("CreatePipe Failed"));
		return false;
	}
	bool result;
	DWORD numBytesWritten = 0;
	CDM data = { 0 };
	data.action = action;
	result = WriteFile(
		pipe, // handle to our outbound pipe
		&data, // data to send
		sizeof(CDM), // length of data to send (bytes)
		&numBytesWritten, // will store actual amount of data sent
		NULL // not using overlapped IO
	);

	if (numBytesWritten) {
		CloseHandle(pipe);
		return true;
	}
	else
	{
		CloseHandle(pipe);
		return false;
	}
}

bool CreatePipe(DWORD pid)
{
	BOOL   fConnected = FALSE;
	DWORD  dwThreadId = 0;
	HANDLE hPipe = INVALID_HANDLE_VALUE, hThread = NULL;
	LPCTSTR lpszPipename = TEXT("\\\\.\\pipe\\NmaePipe");
	TCHAR PipeName[MAX_PATH] = { 0 };
	_stprintf_s(PipeName, _countof(PipeName), _T("%s_%d"), lpszPipename, pid);
	// The main loop creates an instance of the named pipe and 
	// then waits for a client to connect to it. When the client 
	// connects, a thread is created to handle communications 
	// with that client, and this loop is free to wait for the
	// next client connect request. It is an infinite loop.
	//PrintDbgInfo(TEXT("\nPipe Server: Main thread awaiting client connection on %s\n"), lpszPipename);
	hPipe = CreateNamedPipe(
			PipeName,             // pipe name 
			PIPE_ACCESS_DUPLEX,       // read/write access 
			PIPE_TYPE_MESSAGE |       // message type pipe 
			PIPE_READMODE_MESSAGE |   // message-read mode 
			PIPE_WAIT,                // blocking mode 
			PIPE_UNLIMITED_INSTANCES, // max. instances  
			sizeof(CDM),                  // output buffer size 
			sizeof(CDM),                  // input buffer size 
			0,                        // client time-out 
			NULL);                    // default security attribute 

		if (hPipe == INVALID_HANDLE_VALUE)
		{
			PrintDbgInfo(TEXT("CreateNamedPipe failed, GLE=%d.\n"), GetLastError());
			return false;
		}

		// Wait for the client to connect; if it succeeds, 
		// the function returns a nonzero value. If the function
		// returns zero, GetLastError returns ERROR_PIPE_CONNECTED. 

		fConnected = ConnectNamedPipe(hPipe, NULL) ?
			TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
		if (fConnected)
		{
			PrintDbgInfo(TEXT("Client connected!"));
			pipe = hPipe;
			return true;
		}
		else
		{
			CloseHandle(hPipe);
			return false;
		}
}
