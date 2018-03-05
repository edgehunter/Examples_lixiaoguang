// SerialAppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "serialCtl.hpp"
#include "serialThread.hpp"
#include "SerialApp.h"
#include "SerialAppDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialAppDlg dialog

CSerialAppDlg::CSerialAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSerialAppDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSerialAppDlg)
	m_namePort = _T("");
	m_baudRate = _T("");
	m_monitorRec = _T("");
	m_monitorSend = _T("");
	m_status_port = _T("");
  openPortActivate = false;
  closePortActivate = false;
  sendActivate = false;
  activeProccess = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_baudRate = "9600";
	m_namePort = "COM1";
}

void CSerialAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSerialAppDlg)
	DDX_CBString(pDX, IDC_NAME_PORT, m_namePort);
	DDX_CBString(pDX, IDC_BOUDRATE, m_baudRate);
	DDX_Text(pDX, IDC_MONITOR_REC, m_monitorRec);
	DDX_Text(pDX, IDC_MONITOR_SEND, m_monitorSend);
	DDX_Text(pDX, IDC_STATUS_PORT, m_status_port);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSerialAppDlg, CDialog)
	//{{AFX_MSG_MAP(CSerialAppDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_PORT, OnOpenPort)
	ON_BN_CLICKED(IDC_CLOSE_PORT, OnClosePort)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_BN_CLICKED(IDC_SEND_DATA, OnSendData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialAppDlg message handlers

BOOL CSerialAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
  GetDlgItem(IDC_CLOSE_PORT)->EnableWindow(FALSE);
    serialProcess = 
	(SerialThread*)AfxBeginThread(RUNTIME_CLASS(SerialThread), 
	THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
  serialProcess->setOwner(this);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSerialAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSerialAppDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSerialAppDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSerialAppDlg::OnOpenPort() 
{
	// TODO: Add your control notification handler code here
  UpdateData(TRUE);
  UpdateConfig();
  openPortActivate = true;
  closePortActivate = false;
  activeProccess = TRUE;
  UpdateData(TRUE);
  serialProcess->ResumeThread();
  GetDlgItem(IDC_CLOSE_PORT)->EnableWindow(TRUE);
  GetDlgItem(IDC_OPEN_PORT)->EnableWindow(FALSE);
  UpdateData(FALSE);
}

void CSerialAppDlg::OnClosePort() 
{
	// TODO: Add your control notification handler code here
  // Set signal of closing port serial communication.
  closePortActivate =  true;
  openPortActivate = false;
  GetDlgItem(IDC_CLOSE_PORT)->EnableWindow(FALSE);
  GetDlgItem(IDC_OPEN_PORT)->EnableWindow(TRUE);
  UpdateData(FALSE);
}

void CSerialAppDlg::OnExit() 
{
	// TODO: Add your control notification handler code here
  // Set signal of closing port serial communication.
  serialProcess->SuspendThread();
	this->DestroyWindow();
}

void CSerialAppDlg::OnSendData() 
{
	// TODO: Add your control notification handler code here
  // Set signal to send data of serial communication.
  UpdateData(TRUE);
  sendActivate = true;
}

void CSerialAppDlg::UpdateConfig()
{
  // constant parameter.
  configSerial_.ByteSize = 8;
  configSerial_.StopBits = ONESTOPBIT;
  configSerial_.Parity = NOPARITY;
  
  switch (_ttoi(m_baudRate))
  {
  case 110:
    configSerial_.BaudRate = CBR_110;
    break;
  case 300:
    configSerial_.BaudRate = CBR_300;
    break;
  case 600:
    configSerial_.BaudRate = CBR_600;
    break;
  case 1200:
    configSerial_.BaudRate = CBR_1200;
    break;
  case 2400:
    configSerial_.BaudRate = CBR_2400;
    break;
  case 4800:
    configSerial_.BaudRate = CBR_4800;
    break;
  case 9600:
    configSerial_.BaudRate = CBR_9600;
    break;
  case 14400:
    configSerial_.BaudRate = CBR_14400;
    break;
  case 19200:
    configSerial_.BaudRate = CBR_19200;
    break;
  case 38400:
    configSerial_.BaudRate = CBR_38400;
    break;
  case 56000:
    configSerial_.BaudRate = CBR_56000;
    break;
  case 57600:
    configSerial_.BaudRate = CBR_57600;
    break;
  case 115200 :
    configSerial_.BaudRate = CBR_115200;
    break;
  case 128000:
    configSerial_.BaudRate = CBR_128000;
    break;
  case 256000:
    configSerial_.BaudRate = CBR_256000;
    break;
  default:
    break;
  }
}
