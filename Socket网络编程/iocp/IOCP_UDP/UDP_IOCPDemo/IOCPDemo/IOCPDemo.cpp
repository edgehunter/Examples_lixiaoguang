// IOCPDemo.cpp : 定义应用程序的类行为。
//

#pragma comment(lib,"ws2_32.lib")
#include "stdafx.h"
#include "IOCPDemo.h"
#include "IOCPDemoDlg.h"
#include <WinSock2.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIOCPDemoApp

BEGIN_MESSAGE_MAP(CIOCPDemoApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CIOCPDemoApp 构造
CIOCPDemoApp::CIOCPDemoApp()
{
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CIOCPDemoApp 对象
CIOCPDemoApp theApp;


// CIOCPDemoApp 初始化

BOOL CIOCPDemoApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	{
		WSAData wsaData={0};
		int res=WSAStartup(0x0202,&wsaData);

		if (res!=0)
		{
			TRACE("ERROR:WSAStartup error;ErrorCode:%d\n\n",WSAGetLastError());
			return FALSE;
		}
		
		if (wsaData.wVersion!=0x0202)
		{
			TRACE("ERROR:Can't find usable winsock DLL;ErrorCode:%d\n\n",WSAGetLastError());
			return FALSE;
		}
	}

	CIOCPDemoDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此处放置处理何时用“确定”来关闭
		//  对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭
		//  对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。

	WSACleanup();
	return FALSE;
}
