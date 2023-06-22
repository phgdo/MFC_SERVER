
// serverDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "server.h"
#include "serverDlg.h"
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


// CserverDlg dialog



CserverDlg::CserverDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//void CserverDlg::ListenThread()
//{

//}
/*
void CserverDlg::ReceiveThreadProc()
{
	CString aaa;
	char msg[100] = { 0 };
	int len = 0;
	while (true)
	{
		client.Receive(&len, sizeof(int), 0);
		char* temp = new char[len + 1];
		client.Receive(temp, len, 0);
		temp[len] = 0;
		aaa = temp;
	}
	m_list_msg.AddString(aaa);
	// Close the client socket
	client.Close();
}*/

void CserverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MSG, m_list_msg);
}

BEGIN_MESSAGE_MAP(CserverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CserverDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CserverDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CserverDlg message handlers

BOOL CserverDlg::OnInitDialog()
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
	AfxSocketInit(NULL);
//	serversock = new ServerSocket(this);
	serversock.Create(8888);
	serversock.Listen();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CserverDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CserverDlg::OnPaint()
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
HCURSOR CserverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CserverDlg::RecvThread() {
	while (true) {
		for (int i = 0; i < clients.size(); i++) {
			char buff[512];
			ZeroMemory(buff, 512);
			int len = clients.at(i)->Receive(buff, 512, 0);
			if (len > 0) {
				CString msg(buff, len);
				m_list_msg.AddString(msg);
			}
			else {
				MessageBox(_T("fail"));
			}
		}
		
	}
}

void CserverDlg::OnBnClickedButton1()
{
	std::thread recvThrea(&CserverDlg::RecvThread, this);
	recvThrea.join();
	//this->serversock->Bind(8888);
	//this->serversock->Listen();
	/*
	AfxSocketInit(NULL);
	server.Create(8888);
	server.Bind(8888);
	server.Listen();
	if (server.Accept(client)) {
		MessageBox(_T("Connected"), _T("Thong bao"));
	}

	CString aaa;
	char msg[100] = { 0 };
	int len = 0;
	
	while (true)
	{
		server.Send()
		client.Receive(&len, sizeof(int), 0);
		char* temp = new char[len + 1];
		client.Receive(temp, len, 0);
		temp[len] = 0;
		aaa = temp;
		break;
	}
	
	m_list_msg.AddString(aaa);
//	AfxBeginThread(&CserverDlg::ReceiveThreadProc, this);
	//m_thread = std::thread(&CserverDlg::ReceiveThreadProc, this);
	//m_thread.detach();
	// TODO: Add your control notification handler code here
	*/
}


void CserverDlg::OnBnClickedButton2()
{
	serversock.Send("aaa", 3);
//	server.Send("aaaa", 4);
	// TODO: Add your control notification handler code here
}

void CserverDlg::MsgBox(CString msg) {
	MessageBox(msg);
}