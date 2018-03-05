// SerialAppDlg.h : header file
//

#if !defined(AFX_SERIALAPPDLG_H__81B8D820_84F4_495C_A799_9659B5277C70__INCLUDED_)
#define AFX_SERIALAPPDLG_H__81B8D820_84F4_495C_A799_9659B5277C70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSerialAppDlg dialog
class SerialThread;
class CSerialAppDlg : public CDialog
{
// Construction
public:
	CSerialAppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSerialAppDlg)
	enum { IDD = IDD_SERIALAPP_DIALOG };
	CString	m_namePort;
	CString	m_baudRate;
	CString	m_monitorRec;
  DCB configSerial_;
	CString	m_monitorSend;
  bool openPortActivate;
  bool closePortActivate;
  bool sendActivate;
	CString	m_status_port;
  SerialThread* serialProcess;
  BOOL activeProccess;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialAppDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSerialAppDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpenPort();
	afx_msg void OnClosePort();
	afx_msg void OnExit();
	afx_msg void OnSendData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void UpdateConfig();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALAPPDLG_H__81B8D820_84F4_495C_A799_9659B5277C70__INCLUDED_)
