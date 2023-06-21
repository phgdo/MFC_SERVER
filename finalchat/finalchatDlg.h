
// finalchatDlg.h : header file
//

#pragma once
#include "afxsock.h"

// CfinalchatDlg dialog
class CfinalchatDlg : public CDialogEx
{
// Construction
public:
	CfinalchatDlg(CWnd* pParent = nullptr);	// standard constructor
	CSocket client;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FINALCHAT_DIALOG };
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
	CListBox m_list_username;
	CEdit m_edt_username;
	CEdit m_edt_password;
	CButton m_btn_signin;
	CButton m_btn_signup;
	afx_msg void OnBnClickedButtonSignin();
	void GetMessageFromSocket(CString msg);
	CEdit m_edt_msg;
	CButton m_btn_send;
	CListBox m_list_msg;
};
