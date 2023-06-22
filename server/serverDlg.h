
// serverDlg.h : header file
//

#pragma once
#include "afxsock.h"
#include <thread>
#include "ServerSocket.h"
#include <vector>

class ServerSocket;

// CserverDlg dialog
class CserverDlg : public CDialogEx
{
// Construction
public:
	CserverDlg(CWnd* pParent = nullptr);	// standard constructor
	//CSocket server;
	//CSocket client;
	//std::thread m_thread;
	//void ListenThread();
	//void ReceiveThreadProc();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CListBox m_list_msg;
	afx_msg void OnBnClickedButton2();
	//ServerSocket* serversock;
	CAsyncSocket serversock;
	std::vector <CAsyncSocket* > clients;
	void MsgBox(CString msg);
	void RecvThread();
};
