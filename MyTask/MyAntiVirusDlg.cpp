// MyAntiVirusDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTask.h"
#include "MyAntiVirusDlg.h"
#include "MyVirusLibDlg.h"
#include "MyProcessLibDlg.h"
#include"CAddVirus.h"
#include "afxdialogex.h"
#include <tlhelp32.h>
#include <Psapi.h>
#include <vector>
#include "MD5_FILE.h"
#include "LoadVirusLib.h"



using namespace std;

// MyAntiVirusDlg 对话框

IMPLEMENT_DYNAMIC(MyAntiVirusDlg, CDialogEx)

MyAntiVirusDlg::MyAntiVirusDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ANTIVIRUS_DIALOG, pParent)
{
	Create(IDD_ANTIVIRUS_DIALOG);
}

MyAntiVirusDlg::~MyAntiVirusDlg()
{
}

void MyAntiVirusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_RADIO1, m_Radio_All_Control);
	//DDX_Control(pDX, IDC_RADIO2, m_Radio_This_Control);
	//DDX_Radio(pDX, IDC_RADIO1, m_Radio_All_Value);
	//DDX_Control(pDX, IDC_BUTTON1, m_Button_Start_File);
	DDX_Control(pDX, IDC_LIST_FILE, m_List_File);
	DDX_Control(pDX, IDC_STATIC_PATH, m_Static_Path);
	DDX_Control(pDX, IDC_CHECK1, m_Check_Cloud);
	DDX_Control(pDX, IDC_STATIC_GROUP, m_Static_Group);
	DDX_Control(pDX, IDC_LIST_PROCESS, m_List_Process);
	DDX_Control(pDX, IDC_BUTTON7, m_Button_Start_Process);
	DDX_Control(pDX, IDC_CHECK2, m_Check_End);
	DDX_Control(pDX, IDC_CHECK4, m_check_antidebug);
	DDX_Control(pDX, IDC_CHECK6, m_check_antiinject);
	DDX_Control(pDX, IDC_CHECK7, m_check_protectThread);
	DDX_Control(pDX, IDC_CHECK8, m_check_mmprotect);
	DDX_Control(pDX, IDC_CHECK9, m_check_kernel_antidebug);
	DDX_Control(pDX, IDC_CHECK10, m_check_hideprocess);
}


BEGIN_MESSAGE_MAP(MyAntiVirusDlg, CDialogEx)
	//ON_BN_CLICKED(IDC_BUTTON2, &MyAntiVirusDlg::OnBnClickedButton2)
	//ON_BN_CLICKED(IDC_BUTTON1, &MyAntiVirusDlg::OnBnClickedButton1)
	//ON_BN_CLICKED(IDC_RADIO1, &MyAntiVirusDlg::OnBnClickedRadio1)
	//ON_BN_CLICKED(IDC_RADIO2, &MyAntiVirusDlg::OnBnClickedRadio2)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, &MyAntiVirusDlg::OnBnClickedButton3)
	ON_COMMAND(ID_CLEARONECE, &MyAntiVirusDlg::OnClearonece)
	ON_COMMAND(ID_CLEARALL, &MyAntiVirusDlg::OnClearall)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FILE, &MyAntiVirusDlg::OnRclickListFile)
	ON_BN_CLICKED(IDC_BUTTON7, &MyAntiVirusDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &MyAntiVirusDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_CHECK1, &MyAntiVirusDlg::OnBnClickedCheck1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PROCESS, &MyAntiVirusDlg::OnLvnItemchangedListProcess)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FILE, &MyAntiVirusDlg::OnNMClickListFile)
//	ON_NOTIFY(NM_SETFOCUS, IDC_LIST_FILE, &MyAntiVirusDlg::OnNMSetfocusListFile)
//ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PROCESS, &MyAntiVirusDlg::OnNMCustomdrawListProcess)
ON_BN_CLICKED(IDC_CHECK7, &MyAntiVirusDlg::OnBnClickedCheck7)
ON_BN_CLICKED(IDC_CHECK9, &MyAntiVirusDlg::OnBnClickedCheck9)
ON_BN_CLICKED(IDC_CHECK10, &MyAntiVirusDlg::OnBnClickedCheck10)
ON_BN_CLICKED(IDC_CHECK8, &MyAntiVirusDlg::OnBnClickedCheck8)
ON_BN_CLICKED(IDC_CHECK2, &MyAntiVirusDlg::OnBnClickedCheck2)
ON_BN_CLICKED(IDC_CHECK4, &MyAntiVirusDlg::OnBnClickedCheck4)
ON_BN_CLICKED(IDC_CHECK6, &MyAntiVirusDlg::OnBnClickedCheck6)
END_MESSAGE_MAP()

void MyAntiVirusDlg::AddEditData(CEdit &m_Edit_Contral, CString &nData)
{
	m_Edit_Contral.SetSel(-1);
	m_Edit_Contral.ReplaceSel(nData);
	m_Edit_Contral.SetSel(-1);
	m_Edit_Contral.ReplaceSel(TEXT("\n"));

}
// MyAntiVirusDlg 消息处理程序


BOOL MyAntiVirusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Menu.LoadMenu(IDR_MENU6);
	SetMenu(&m_Menu);


	m_List_File.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List_Process.SetExtendedStyle(LVS_EX_FULLROWSELECT| LVS_EX_CHECKBOXES);
	m_List_File.InsertColumn(0, TEXT("进程名"), 0, 100);
	m_List_File.InsertColumn(1, TEXT("PID"), 0, 100);
	m_List_File.InsertColumn(2, TEXT("文件路径"), 0, 350);
	m_List_File.InsertColumn(3, TEXT("文件MD5"), 0, 200);

	m_List_Process.InsertColumn(0, TEXT("进程名"), 0, 150);
	m_List_Process.InsertColumn(1, TEXT("PID"), 0, 100);
	m_List_Process.InsertColumn(2, TEXT("文件路径"), 0, 200);
	m_List_Process.InsertColumn(3, TEXT("文件MD5"), 0, 200);


	LoadImageVirus();
	LoadImageProcess();

	return TRUE;
}
//载入图标
void MyAntiVirusDlg::LoadImageVirus()
{
	m_VirusImageList.Create(20, 20, ILC_COLOR32 | ILC_MASK, 2, 2);

	HICON hSmallIco;
	CString nDir = GetProgramDir();

	hSmallIco = ExtractIcon(AfxGetInstanceHandle(), nDir + TEXT("res\\virus.ico"), 0);
	m_VirusImageList.Add(hSmallIco);
	DestroyIcon(hSmallIco);

	m_List_File.SetImageList(&m_VirusImageList, LVSIL_SMALL);

}
//载入进程图标
void MyAntiVirusDlg::LoadImageProcess()
{
	m_ProcessImageList.Create(20, 20, ILC_COLOR32 | ILC_MASK, 2, 2);

	HICON hSmallIco;
	CString nDir = GetProgramDir();

	hSmallIco = ExtractIcon(AfxGetInstanceHandle(), nDir + TEXT("res\\safe.ico"), 0);
	m_ProcessImageList.Add(hSmallIco);
	DestroyIcon(hSmallIco);

	hSmallIco = ExtractIcon(AfxGetInstanceHandle(), nDir + TEXT("res\\virus.ico"), 0);
	m_ProcessImageList.Add(hSmallIco);
	DestroyIcon(hSmallIco);


	m_List_Process.SetImageList(&m_ProcessImageList, LVSIL_SMALL);
}

bool MyAntiVirusDlg::refreshListPT()
{
	deleteallcheck();
	m_List_File.DeleteAllItems();
	for (auto& it : ProtectProcess)
	{
		//将要保护的进程展示出来
		int nIndex = m_List_File.GetItemCount();
		m_List_File.InsertItem(nIndex, it.szExeFile, 0);
		m_List_File.SetItemText(nIndex, 1, it.th32ProcessID);
		m_List_File.SetItemText(nIndex, 2, it.szExeFilePath);
		m_List_File.SetItemText(nIndex, 3, it.FileMd5);
	}
	if (m_List_File.GetItemCount() != 0)
	{
		m_List_File.SetItemState(m_List_File.GetItemCount() - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_List_File.SetFocus();
		updateallcheck(ProtectProcess[m_List_File.GetItemCount() - 1]);
	}

	return true;
}

void MyAntiVirusDlg::deleteallcheck()
{

	m_check_antidebug.SetCheck(0);
	m_check_antiinject.SetCheck(0);
	m_check_protectThread.SetCheck(0);
	m_check_mmprotect.SetCheck(0);
	m_check_kernel_antidebug.SetCheck(0);
	m_check_hideprocess.SetCheck(0);
	m_Check_Cloud.SetCheck(0);
	m_Check_End.SetCheck(0);

}

void MyAntiVirusDlg::updateallcheck(MyProtectProcess &pt)
{
	if (pt.states[dataprotect])
	{
		m_Check_Cloud.SetCheck(1);
	}
	if (pt.states[antidebug])
	{
		m_check_antidebug.SetCheck(1);
	}
	if (pt.states[antiinject])
	{
		m_check_antiinject.SetCheck(1);
	}
	if (pt.states[protectthread])
	{
		m_check_protectThread.SetCheck(1);
	}
	if (pt.states[k_handlept])
	{
		m_Check_End.SetCheck(1);
	}
	if (pt.states[k_mmpt])
	{
		m_check_mmprotect.SetCheck(1);
	}
	if (pt.states[k_antidebug])
	{
		m_check_kernel_antidebug.SetCheck(1);
	}
	if (pt.states[k_hideprocess])
	{
		m_check_hideprocess.SetCheck(1);
	}
	SetDlgItemText(IDC_STATIC_PATH, TEXT("等待配置"));
}



//遍历文件杀毒
void MyAntiVirusDlg::ScanFile(CString dir)
{
	CString nTemp;
	WIN32_FIND_DATA fData = { 0 };
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = FindFirstFile(dir + L"\\*", &fData); //开始遍历文件
	if (hFind == INVALID_HANDLE_VALUE || m_IsScaning == FALSE) return;

	do {
		if (m_IsScaning == FALSE)
			return;

		if (CString(fData.cFileName) == L"." || CString(fData.cFileName) == L"..")
			continue;
			
		if (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)	ScanFile(dir + L"\\" + fData.cFileName);
		else
		{
			InterlockedIncrement(&m_ScaningNum); //开锁
			m_ScaningPath = dir + L"\\" + fData.cFileName;


			CStringA nFileMd5 = GetFileMd5(m_ScaningPath);//获取文件MD5

			for (DWORD i = 0; i < m_VirusInfoNum; i++)//循环与病毒库MD5比较
			{
				if (strcmp(m_VirusInfo[i].FileMd5, nFileMd5) == 0)//如果MD5吻合，及即发现病毒
				{
					InterlockedIncrement(&m_VirusNum);//病毒数量+1
					DWORD nIndex = m_List_File.GetItemCount();
					nTemp.Format(TEXT("%d"), nIndex + 1);
					//将病毒信息输出到列表框
					m_List_File.InsertItem(nIndex, nTemp,0);
					m_List_File.SetItemText(nIndex , 1, dir + L"\\" + fData.cFileName);
					m_List_File.SetItemText(nIndex , 2, CString(nFileMd5));
					break;
				}
			}
		}
	} while (FindNextFile(hFind, &fData));

}

//启动线程，开启扫描
/*
DWORD WINAPI ScanFileThread(LPVOID lpThreadParameter)
{
	MyAntiVirusDlg *nDlg = (MyAntiVirusDlg*)lpThreadParameter;

	if (nDlg->m_Radio_All_Control.GetCheck())
	{
		vector<CString>nDiverList;
		nDiverList = GetDriverList();
		for (auto &val: nDiverList) nDlg->ScanFile(val);

	}
	else
	{
		nDlg->ScanFile(nDlg->m_ThisPath);
	}

	nDlg->m_Button_Start_File.EnableWindow(TRUE);
	nDlg->SetTimer(nDlg->REFERSHPATH_TIMER, -1, NULL);//停止定时刷新
	nDlg->m_Static_Path.SetWindowText(TEXT("扫描完成"));
	nDlg->m_Button_Start_File.SetWindowText(TEXT("开始扫描"));
	nDlg->m_IsScaning = FALSE;

	return 0;
}
*/
//停止扫描，停止线程
/*
void MyAntiVirusDlg::OnBnClickedButton1()
{

	LoadVirus();

	if (!m_IsScaning)
	{
		m_ScaningNum = 0;
		m_VirusNum = 0;
		m_Button_Start_File.SetWindowText(TEXT("停止扫描"));
		m_List_File.DeleteAllItems();

		SetTimer(REFERSHPATH_TIMER, 300, NULL);//设置定时刷新
		CreateThread(NULL, NULL, ScanFileThread, (LPVOID)this, NULL, NULL);
		m_IsScaning = TRUE;
	}
	else
	{
		m_Button_Start_File.SetWindowText(TEXT("正在停止"));
		m_Button_Start_File.EnableWindow(FALSE);
		m_IsScaning = FALSE;

	}
}*/


void MyAntiVirusDlg::LoadVirus()
{
	if (m_VirusInfo != nullptr)delete[]m_VirusInfo;
	LoadVirusLib nLoadVirusLib;
	m_VirusInfoNum = nLoadVirusLib.LoadVirusLibData();
	m_VirusInfo = new VIRUSINFO[m_VirusInfoNum];

	nLoadVirusLib.GetVirusLib(m_VirusInfo);
	nLoadVirusLib.ClearVirusLibData();
}

/*
//同步云端病毒库
void MyAntiVirusDlg::UpDataCloud()
{
	LoadVirusLib nLoadVirusLib;
	BOOL nRet = nLoadVirusLib.DownDataVirus();
	if (nRet)
	{
		PVIRUSINFO nVIRUSINFO = nullptr;
		DWORD nVirusNum = nLoadVirusLib.RecvDataVirus(nVIRUSINFO);

		if (nVIRUSINFO == nullptr)return;

		nLoadVirusLib.SetVirusLib(nVIRUSINFO, nVirusNum);
		delete[]nVIRUSINFO;
	}
}
*/

//选中全盘扫描
/*
void MyAntiVirusDlg::OnBnClickedRadio1()
{
	m_Static_Group.SetWindowText(TEXT("全盘扫描"));
}
*/

//指定路径扫描
/*
void MyAntiVirusDlg::OnBnClickedRadio2()
{
	m_ThisPath = GetPathFrom();
	if (m_ThisPath ==TEXT(""))
	{
		m_Radio_All_Control.SetCheck(TRUE);
		m_Radio_This_Control.SetCheck(FALSE);
		return;
	}
	m_Static_Group.SetWindowText(TEXT("指定位置 - ")+ m_ThisPath);
}
*/


void MyAntiVirusDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);

	if (nIDEvent == REFERSHPATH_TIMER)
	{
		CString nTemp;
		nTemp.Format(TEXT("文件数量：%d 正在扫描："), m_ScaningNum);
		m_Static_Path.SetWindowText(nTemp + m_ScaningPath);
	}
}

//上传病毒
void MyAntiVirusDlg::OnBnClickedButton3()
{
	MyVirusLibDlg nMyVirusLibDlg;
	nMyVirusLibDlg.DoModal();
}

//取消单个进程的保护
void MyAntiVirusDlg::OnClearonece()
{
	int pid = _ttoi(m_List_File.GetItemText(m_List_File.GetSelectionEx(), 1));
	for (auto it = ProtectProcess.begin(); it != ProtectProcess.end();it++) {
		if (it->th32ProcessID == pid)
		{
			
			if(!it->clearallprotect())
				MessageBox(TEXT("protect close fail"));
			ProtectProcess.erase(it);
			break;
		}
	}
	refreshListPT();
	//取消备选区选择
	for (int i = 0; i < m_List_Process.GetItemCount(); i++)
	{
		if (pid == _ttoi(m_List_Process.GetItemText(i, 1)))
		{
			m_List_Process.SetCheck(i, 0);
			break;
		}
	}
}

//清除所有病毒文件
void MyAntiVirusDlg::OnClearall()
{
	CString nFileName;
	DWORD nCount;
	nCount = m_List_File.GetItemCount();
	for (DWORD i =0 ; i < nCount ; i++)
	{
		OnClearonece();
	}

}

//打开病毒库信息
void MyAntiVirusDlg::OnBnClickedButton8()
{
	MyProcessLibDlg nMyProcessLibDlg;
	nMyProcessLibDlg.DoModal();
}

//选中进程
void MyAntiVirusDlg::OnRclickListFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	CMenu *nMenu = m_Menu.GetSubMenu(0);
	POINT pos;
	GetCursorPos(&pos);

	if (pNMItemActivate->iItem == -1)nMenu->EnableMenuItem(ID_CLEARONECE, TRUE);
	else nMenu->EnableMenuItem(ID_CLEARONECE, FALSE);
	nMenu->EnableMenuItem(ID_CLEARALL, !m_List_File.GetItemCount());
	nMenu->TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, this);
}

//创建一个线程进行扫描
DWORD WINAPI ScanProcessThread(LPVOID lpThreadParameter)
{
	MyAntiVirusDlg *nDlg = (MyAntiVirusDlg*)lpThreadParameter;

	nDlg->ScanProcess();

	nDlg->m_Button_Start_Process.EnableWindow(TRUE);
	return 0;
}

//扫描当前进程
void MyAntiVirusDlg::ScanProcess()
{
	m_List_Process.DeleteAllItems();  //删除以前列表记录

	TASKLIST nTaskList = {};
	DWORD nNum = EnumTaskList(nTaskList);  //枚举进程列表
	CStringA nFileMd5;
	DWORD nIndex;

	LoadVirusLib nLoadProcessLib;
	DWORD nCount = nLoadProcessLib.LoadProcessLibData(); //统计进程数
	PVIRUSINFO m_VirusInfo = new VIRUSINFO[nCount];
	nLoadProcessLib.GetProcessLib(m_VirusInfo);    //获取白名单信息
	nLoadProcessLib.ClearProcessLibData();


	for (DWORD i = 0 ; i < nNum; i++)
	{
		if (nTaskList.szExeFilePath[i].IsEmpty())continue;

		nFileMd5 = GetFileMd5(nTaskList.szExeFilePath[i]);   //文件md5转换

		if (nFileMd5.IsEmpty())continue;


		BOOL nSafe = FALSE;
		for (DWORD x = 0 ;x <nCount;x++)//循环判断进程名是否在白名单内
		{
			if (strcmp(m_VirusInfo[x].FileName, CStringA(nTaskList.szExeFile[i])) == 0)
			{	//如果进程名在白名单内，则标记为安全
				nSafe = TRUE; break;
			}
		}
		//如果进程不安全，且勾上了自动结束进程，则自动结束当前进程
		if (m_Check_End.GetCheck()&& !nSafe)EndProcess(nTaskList.th32ProcessID[i]);
		//将进程信息输出到列表框
		nIndex = m_List_Process.GetItemCount();
		m_List_Process.InsertItem(nIndex, nTaskList.szExeFile[i], nSafe ? 0 : 1);
		m_List_Process.SetItemText(nIndex, 1, nTaskList.th32ProcessID[i]);
		m_List_Process.SetItemText(nIndex, 2, nTaskList.szExeFilePath[i]);
		m_List_Process.SetItemText(nIndex, 3, nFileMd5);
		for (auto& it : ProtectProcess)
		{
			if (it.th32ProcessID == nTaskList.th32ProcessID[i])
			{
				m_List_Process.SetCheck(nIndex, 1);
			}
		}
		m_List_Process.SendMessage(LVM_SCROLL, 0, 100);
	}
	ClearTaskList(nTaskList);

	delete[]m_VirusInfo;

}

//开始扫描，停止扫描
void MyAntiVirusDlg::OnBnClickedButton7()
{
	m_Button_Start_Process.EnableWindow(FALSE); //响应后禁止用户继续操作原UI
	CreateThread(NULL, NULL, ScanProcessThread, (LPVOID)this, NULL, NULL);  //创建扫描线程扫描
}




void MyAntiVirusDlg::OnBnClickedCheck1()
{
	if (changestate(m_Check_Cloud, k_handlept))
	{
		m_Static_Path.SetWindowTextW(_T("成功！"));
	}
	else
	{
		m_Static_Path.SetWindowTextW(_T("失败！"));
	}
}

//清理内存
void MyAntiVirusDlg::OnBnClickedButton2()
{
	//获取清理前的内存信息
	MEMORYSTATUSEX memStatus = { sizeof(MEMORYSTATUSEX) };
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE preUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	//开始清理内存
	ClearMemory();
	//获取清理后的内存信息
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE afterUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	//输出清理信息
	CString m_szStatus;
	m_szStatus.Format(_T("内存清理完毕！本次清理 : %.2f MB"), preUsedMem - afterUsedMem);

	MessageBox(m_szStatus, TEXT("提示"), MB_ICONINFORMATION);
}

void MyAntiVirusDlg::OnLvnItemchangedListProcess(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	CString mychoiseitem;
	if ((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(1)) /* old state : unchecked */
		&& (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2)) /* new state : checked */
		)
	{
		//排除重复项
		for (vector<MyProtectProcess>::iterator it = ProtectProcess.begin(); it < ProtectProcess.end(); it++)
		{
			if (it->th32ProcessID == _ttoi(m_List_Process.GetItemText(pNMLV->iItem, 1)) && \
				it->szExeFile == m_List_Process.GetItemText(pNMLV->iItem,0))
			{
				return;
			}
		}
		mychoiseitem.Format(L"选中ITEM%d", pNMLV->iItem);
		//选中一个进程需要保护
		MyProtectProcess pt = MyProtectProcess(m_List_Process.GetItemText(pNMLV->iItem, 0), \
			m_List_Process.GetItemText(pNMLV->iItem, 2),\
			m_List_Process.GetItemText(pNMLV->iItem, 3),\
			_ttoi(m_List_Process.GetItemText(pNMLV->iItem,1)));
		ProtectProcess.push_back(pt);
		
		refreshListPT();
		m_List_Process.SetItemState(pNMLV->iItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		//MessageBox(mychoiseitem);
	}
	else if ((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(2)) /* old state : checked */
		&& (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1)) /* new state : unchecked */
		)
	{
		mychoiseitem.Format(L"取消ITEM%d", pNMLV->iItem);
		//从需要保护的进程数组
		for (vector<MyProtectProcess>::iterator it = ProtectProcess.begin();it<ProtectProcess.end();it++)
		{
			if (it->th32ProcessID == _ttoi(m_List_Process.GetItemText(pNMLV->iItem, 1)))
			{
				it->clearallprotect();
				ProtectProcess.erase(it);
				break;
			}
		}
		refreshListPT();
		m_List_Process.SetItemState(pNMLV->iItem, 0, LVIS_SELECTED | LVIS_FOCUSED);
		//MessageBox(mychoiseitem);
	}
	*pResult = 0;
}


void MyAntiVirusDlg::OnNMClickListFile(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CString tmp;
	if (pNMItemActivate->iItem != -1)
	{
		deleteallcheck();
		POSITION ps = m_List_File.GetFirstSelectedItemPosition();
		int nIndex = m_List_File.GetNextSelectedItem(ps);
		int pid = _ttoi(m_List_File.GetItemText(nIndex, 1));
		for (auto& it : ProtectProcess)
		{
			if (it.th32ProcessID == pid)
			{
				updateallcheck(it);
			}
		}
	}
	tmp.Format(L"%d", pNMItemActivate->iItem);
	MessageBox(tmp);
	*pResult = 0;
}


//void MyAntiVirusDlg::OnNMSetfocusListFile(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	// TODO: 在此添加控件通知处理程序代码
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	CString tmp;
//	POSITION ps;
//	ps = m_List_File.GetFirstSelectedItemPosition();
//	int item = m_List_File.GetNextSelectedItem(ps);
//	tmp.Format(L"%d", item);
//	MessageBox(tmp);
//
//	*pResult = 1;
//}


//void MyAntiVirusDlg::OnNMCustomdrawListProcess(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
//
//	switch (pLVCD->nmcd.dwDrawStage)
//
//	{
//
//	case CDDS_PREPAINT:
//
//		*pResult = CDRF_NOTIFYITEMDRAW;
//
//		break;
//
//	caseCDDS_ITEMPREPAINT:
//
//		*pResult = CDRF_NOTIFYSUBITEMDRAW;
//
//		break;
//
//	case  CDDS_ITEMPREPAINT | CDDS_SUBITEM:
//
//		COLORREF clrNewTextColor, clrNewBkColor;
//		int    nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);
//
//		BOOL bSelect = FALSE;
//		POSITION pos = m_List_Process.GetFirstSelectedItemPosition();
//		while (pos)
//		{
//			int index = m_List_Process.GetNextSelectedItem(pos);
//			if (index == nItem)
//			{
//				bSelect = TRUE;
//				break;
//			}
//		}
//		if (bSelect)
//		{
//			clrNewTextColor = RGB(255, 0, 0);//Set the text to red
//			clrNewBkColor = RGB(51, 102, 205);//Set the bkgrnd color to blue
//		}
//		else
//		{
//			clrNewTextColor = RGB(0, 0, 0);//Leave the text black
//			clrNewBkColor = RGB(204, 232, 207); //leave the bkgrnd color white
//		}
//		pLVCD->clrText = clrNewTextColor;
//		pLVCD->clrTextBk = clrNewBkColor;
//
//		*pResult = CDRF_DODEFAULT;
//
//		break;
//
//	Default:
//
//		*pResult = CDRF_DODEFAULT;
//
//		break;
//	}
//}


void MyAntiVirusDlg::OnBnClickedCheck7()
{
	// TODO: 在此添加控件通知处理程序代码
	if (changestate(m_check_protectThread, protectthread))
	{
		m_Static_Path.SetWindowTextW(_T("成功！"));
	}
	else
	{
		m_Static_Path.SetWindowTextW(_T("失败！"));
	}
}


void MyAntiVirusDlg::OnBnClickedCheck9()
{
	// TODO: 在此添加控件通知处理程序代码

	if (changestate(m_check_kernel_antidebug,k_antidebug))
	{
		m_Static_Path.SetWindowTextW(_T("成功！"));
	}
	else
	{
		m_Static_Path.SetWindowTextW(_T("失败！"));
	}
}
bool MyAntiVirusDlg::changestate(CButton &button, int ptmask)
{
	//获取当前选中的进程的pid
	POSITION ps = m_List_File.GetFirstSelectedItemPosition();
	int nIndex = m_List_File.GetNextSelectedItem(ps);
	int pid = _ttoi(m_List_File.GetItemText(nIndex, 1));
	bool bret = false;
	for (auto& pt : ProtectProcess)
	{
		if (pt.th32ProcessID == pid)
		{
			if (pt.states[ptmask])
			{
				bret = pt.clearprotect(ptmask);
				if (!bret)
				{
					button.SetCheck(1);
				}
			}
			else
			{
				bret = pt.openprotect(ptmask);
				if (!bret)
				{
					button.SetCheck(0);
				}
			}
		}
	}
	return bret;
}

void MyAntiVirusDlg::OnBnClickedCheck10()
{
	// TODO: 在此添加控件通知处理程序代码
	if (changestate(m_check_hideprocess, k_hideprocess))
	{
		m_Static_Path.SetWindowTextW(_T("成功！"));
	}
	else
	{
		m_Static_Path.SetWindowTextW(_T("失败！"));
	}
}


void MyAntiVirusDlg::OnBnClickedCheck8()
{
	// TODO: 在此添加控件通知处理程序代码

	if (changestate(m_check_mmprotect, k_mmpt))
	{
		m_Static_Path.SetWindowTextW(_T("成功！"));
	}
	else
	{
		m_Static_Path.SetWindowTextW(_T("失败！"));
	}
}


void MyAntiVirusDlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (changestate(m_Check_End, dataprotect))
	{
		m_Static_Path.SetWindowTextW(_T("成功！"));
	}
	else
	{
		m_Static_Path.SetWindowTextW(_T("失败！"));
	}
}


void MyAntiVirusDlg::OnBnClickedCheck4()
{
	// TODO: 在此添加控件通知处理程序代码
	if (changestate(m_check_antidebug, antidebug))
	{
		m_Static_Path.SetWindowTextW(_T("成功！"));
	}
	else
	{
		m_Static_Path.SetWindowTextW(_T("失败！"));
	}
}


void MyAntiVirusDlg::OnBnClickedCheck6()
{
	// TODO: 在此添加控件通知处理程序代码
	if (changestate(m_check_antiinject, antiinject))
	{
		m_Static_Path.SetWindowTextW(_T("成功！"));
	}
	else
	{
		m_Static_Path.SetWindowTextW(_T("失败！"));
	}
}
