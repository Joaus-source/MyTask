#pragma  once
#include <windows.h>

//����һ�������õĿ���̨
void CreateDbgConsole();
//��ʽ�������Ϣ�ַ�������������� =>��������������԰ѳ������־��Ϣ��λ���ļ���ȥ
void SprintfDbg(TCHAR *pOutBuff, int nMaxOutBuffSize, LPCSTR lpszModuleName, int nLine, LPCTSTR lptstrFormat, va_list argList);

//��ӡ��������Ϣ
void PrintDbg(LPCSTR lpszModuleName, int nLine, LPCTSTR lptstrFormat, ...);

//��ӡ��������Ϣ��Ϣ
void PrintWindowsMsg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LPCSTR lpszModuleName = __FILE__, int nLine = __LINE__);


#ifdef _DEBUG

#define  PrintDbgInfo(lptstrFormat, ...)    do{PrintDbg(__FILE__, __LINE__, lptstrFormat, __VA_ARGS__);}while(0);
#define  PrintWindowsMsgInfo(hwnd, uMsg, wParam, lParam) do{ PrintWindowsMsg(hwnd, uMsg, wParam, lParam, __FILE__, __LINE__);}while (0);

#else

#define  PrintDbgInfo(lptstrFormat, ...) 
#define  PrintWindowsMsgInfo(hwnd, uMsg, wParam, lParam)

#endif

