
// serverDlg.h : header file
//

#pragma once
#include "afxsock.h"
#include <thread>
#include "ServerSocket.h"
#include <vector>
#include <fstream>

class ServerSocket;

struct ClientSocketStruct {
	CString name;
	ServerSocket* clientSock;
};



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
	//thuộc tính
	CListBox m_list_msg;
	ServerSocket* serversock;
	std::vector <ClientSocketStruct> clients;
	CString tempName;
	CString GetTargetName(CString msg);
	int clientCount;

	//phương thức
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	//CAsyncSocket serversock;
	void MsgBox(CString msg);
	void RecvThread();
	CString GetSignal(CString msg);
	bool GetUserPass(CString msg);
	void GetClients(CAsyncSocket* client);
	afx_msg void OnLbnSelchangeListMsg();
	bool CheckIfUsernameLogin(CString username);
	bool SignUpUser(CString msg);
};
