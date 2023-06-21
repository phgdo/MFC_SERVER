
// finalchatDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "finalchat.h"
#include "finalchatDlg.h"
#include "afxdialogex.h"
#include <thread>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CfinalchatDlg dialog



CfinalchatDlg::CfinalchatDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FINALCHAT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CfinalchatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_edt_username);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_edt_password);
	DDX_Control(pDX, IDC_BUTTON_SIGNIN, m_btn_signin);
	DDX_Control(pDX, IDC_BUTTON_SIGNUP, m_btn_signup);
	DDX_Control(pDX, IDC_LIST_USERNAME, m_list_username);
	DDX_Control(pDX, IDC_EDIT_MSG, m_edt_msg);
	DDX_Control(pDX, IDC_BUTTON_SEND, m_btn_send);
	DDX_Control(pDX, IDC_LIST_MSG, m_list_msg);
}

BEGIN_MESSAGE_MAP(CfinalchatDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SIGNIN, &CfinalchatDlg::OnBnClickedButtonSignin)
END_MESSAGE_MAP()


// CfinalchatDlg message handlers

BOOL CfinalchatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CfinalchatDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CfinalchatDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CfinalchatDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



/*
void CfinalchatDlg::OnBnClickedBtnGui()
{
	CString msg;
	m_edt_msg.GetWindowTextW(msg);

	int len = 0;
	len = msg.GetLength();
	client.Send(&len, sizeof(int), 0);
	client.Send(msg, len, 0);
	// TODO: Add your control notification handler code here
}
*/
void ReceiveThread() {

	char buff[512];
	ZeroMemory(buff, 512);
	while (true) {
		
	}
}
void CfinalchatDlg::OnBnClickedButtonSignin()
{
	
	AfxSocketInit(NULL);
	client.Create();
	if (!client.Connect(_T("127.0.0.1"), 8888)) {
		MessageBox(_T("Không thể kết nối"));
	}
	

	fd_set read_fds;
	FD_ZERO(&read_fds);
	FD_SET(client, &read_fds);
	while (true) {
		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 1000;  // 1 millisecond timeout
		int result = select(0, &read_fds, NULL, NULL, &timeout);
		if (result == SOCKET_ERROR) {
			// Handle select error
			break;
		}
		else if (result == 0) {
			// Timeout, no data received
			continue;
		}
		else {
			// Data received, process it
			char buff[512];
			ZeroMemory(buff, 512);
			int len = client.Receive(buff, 512, 0);
			if (len == SOCKET_ERROR) {
				// Handle receive error
				break;
			}
			else if (len == 0) {
				// Connection closed
				break;
			}
			else {
				// Process received data
				// 
				// ...
				m_list_username.AddString(_T("a"));
			}
		}
	}
	client.Close();
	//std::thread receiveThread(ReceiveThread);
	//receiveThread.detach();
	// TODO: Add your control notification handler code here
}

void CfinalchatDlg::GetMessageFromSocket(CString msg) {
	m_list_msg.AddString(msg);
}
