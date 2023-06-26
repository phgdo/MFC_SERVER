
// serverDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "server.h"
#include "serverDlg.h"
#include "afxdialogex.h"
#include <thread> 
#include <string>
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
	ON_LBN_SELCHANGE(IDC_LIST_MSG, &CserverDlg::OnLbnSelchangeListMsg)
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
	serversock = new ServerSocket(this);
	this->serversock->Create(8888);
//	this->serversock->Listen();
	
	//server.Create(8888);
	//server.Listen();
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
			char buff[1024];
			ZeroMemory(buff, 1024);
			int len = clients.at(i).clientSock->Receive(buff, 1024);
			if (len > 0) {
				CString msg(buff, len);
				CString signal = GetSignal(msg);
				
				//Xử lý nếu có client đăng nhập
				if (signal == _T("SIGNIN")) {
					//CString userPass = GetUserPass(msg);
					if (GetUserPass(msg)) { //Gửi tín hiệu khi tài khoản đúng
						if (CheckIfUsernameLogin(tempName)) {//Kiểm tra đã tài khoản đã bị login chưa
							CString signalSend = _T("SIGNINACP");
							CW2A buff(signalSend, CP_UTF8);
							clients.at(i).clientSock->Send(buff, signalSend.GetLength());
							//Sửa vector chưa client đó, thêm tên
							clients.at(i).name = tempName;
							CString tempName2 = tempName;
							//Gửi thông tin có client mới online cho các client khác
							for (int j = 0; j < clients.size(); j++) {
								if (clients.at(j).name != tempName2) {
									CString signalSend;
									signalSend.Format(_T("NEWCL %s "), tempName2);
									CW2A buff(signalSend, CP_UTF8);
									clients.at(j).clientSock->Send(buff, signalSend.GetLength());
								}
							}
						}
						else { //Nếu đã bị login trước thì gửi tín hiệu LOGIN2
							CString signalSend = _T("LOGIN2");
							CW2A buff(signalSend, CP_UTF8);
							clients.at(i).clientSock->Send(buff, signalSend.GetLength());
						}
					}
					else { //Gửi tín hiệu khi sai tài khoản
						CString signalSend = _T("WRONG");
						CW2A buff(signalSend, CP_UTF8);
						clients.at(i).clientSock->Send(buff, signalSend.GetLength());
					}
					
					
				}
				else if (signal == _T("SENDMSG")) {
					SendMsgStruct temp = GetSendMsgStruct(msg);
					for (int j = 0; j < clients.size(); j++) {
						if (clients.at(j).name == temp.targetName) {
							CString newMsg;
							newMsg.Format(_T("NEWMSG %s %s %s"), temp.targetName, temp.yourName, temp.msg);
							CW2A buff(newMsg, CP_UTF8);
							clients.at(j).clientSock->Send(buff, newMsg.GetLength());
						}
					}
				}
				else if (signal == _T("SIGNUP")) {
					if (SignUpUser(msg)) {
						CString newMsg;
						newMsg.Format(_T("ACPSIGNUP"));
						CW2A buff(newMsg, CP_UTF8);
						clients.at(i).clientSock->Send(buff, newMsg.GetLength());
					}
				}
				else if (signal == _T("LOGOUT")) {

				}
				m_list_msg.AddString(msg);
			}
			else {
				//MessageBox(_T("fail"));
			}
		}
	}
}

void CserverDlg::OnBnClickedButton1()
{	
	this->serversock->Listen();
	//std::thread recvThrea(&CserverDlg::RecvThread, this);
	//recvThrea.detach();
}


void CserverDlg::OnBnClickedButton2()
{
	for (int i = 0; i < clients.size(); i++) {
		CString message;
		message.Format(_T("SIGNIN   "));
		CW2A buff(message, CP_UTF8);
		clients.at(i).clientSock->Send(buff, message.GetLength());
	}
	//this->serversock->Send("aaa", 3);
//	server.Send("aaaa", 4);
	// TODO: Add your control notification handler code here
}

void CserverDlg::MsgBox(CString msg) {
	MessageBox(msg);
}

CString CserverDlg::GetSignal(CString msg) {
	CString signal = NULL;
	int wordCount = 0;
	for (int j = 0; j < msg.GetLength(); j++) {
		if (msg[j] == ' ') {
			wordCount++;
		}
		else {
			signal += msg[j];
		}
		if (wordCount == 1) {
			break;
		}
	}
	return signal;
}

bool CserverDlg::CheckIfUsernameLogin(CString username) {
	//Kiểm tra username đã đăng nhập chưa
	for (int i = 0; i < clients.size(); i++) {
		if (username == clients.at(i).name) {
			return false;
		}
	}
	return true;
}

CString CserverDlg::GetTargetName(CString msg) {
	CString targetName;
	int wordCount = 0;
	for (int j = 0; j < msg.GetLength(); j++) {
		if (msg[j] == ' ') {
			wordCount++;
		}
		if (wordCount == 2 && msg[j] != ' ') {
			targetName += msg[j];
		}
		if (wordCount == 2) {
			break;
		}
	}
	return targetName;
}

bool CserverDlg::GetUserPass(CString msg) {
	CString username = NULL;
	CString password = NULL;
	int wordCount = 0;
	for (int j = 0; j < msg.GetLength(); j++) {
		if (msg[j] == ' ') {
			wordCount++;
		}
		if (wordCount == 3) {
			break;
		}
		if (wordCount == 1 && msg[j] != ' ') {
			username += msg[j];
		}
		else if (wordCount == 2 && msg[j] != ' ') {
			password += msg[j];
		}
	}


	tempName = username;
	CString user;
	user.Format(_T("%s %s"), username, password);
	fileIn.open("taikhoan.txt");
	if (fileIn.is_open()) {
		std::string userStr;
		std::string passStr;
		while (std::getline(fileIn, userStr)) {
			std::getline(fileIn, passStr);
			CString ca(userStr.c_str());
			CString cb(passStr.c_str());
			//CStringA ca(line.c_str(), CP_UTF8);
			if (username == ca && password == cb) {
				fileIn.close();
				return true;
			}
		}
		fileIn.close();
	}
	

	return false;
}

void CserverDlg::OnLbnSelchangeListMsg()
{
	int index = m_list_msg.GetCurSel();
	if (index != LB_ERR) {
		CString a;
		m_list_msg.GetText(index, a);
	}
	//MessageBox(_T("1"));
	// TODO: Add your control notification handler code here
}

void CserverDlg::GetClients(CAsyncSocket* client) {
	ClientSocketStruct temp;
	temp.clientSock = client;
	temp.name = _T("");
	clients.push_back(temp);
}

SendMsgStruct CserverDlg::GetSendMsgStruct(CString msg) {
	SendMsgStruct temp;
	CString targetName = NULL;
	CString yourName = NULL;
	CString msgA;
	int wordCount = 0;
	for (int j = 0; j < msg.GetLength(); j++) {
		if (msg[j] == ' ') {
			wordCount++;
		}
		if (wordCount == 1 && msg[j] != ' ') {
			targetName += msg[j];
		}
		else if (wordCount == 2 && msg[j] != ' ') {
			yourName += msg[j];
		}
		else if (wordCount > 2) {
			msgA += msg[j];
		}
	}
	temp.msg = msgA;
	temp.targetName = targetName;
	temp.yourName = yourName;
	return temp;
}

bool CserverDlg::SignUpUser(CString msg) {
	CString username = NULL;
	CString password = NULL;
	int wordCount = 0;
	for (int j = 0; j < msg.GetLength(); j++) {
		if (msg[j] == ' ') {
			wordCount++;
		}
		if (wordCount == 3) {
			break;
		}
		if (wordCount == 1 && msg[j] != ' ') {
			username += msg[j];
		}
		if (wordCount == 2 && msg[j] != ' ') {
			password += msg[j];
		}
	}

	CString user;
	user.Format(_T("%s %s"), username, password);
	fileIn.open("taikhoan.txt");
	if (fileIn.is_open()) {
		std::string userStr;
		std::string passStr;
		while (std::getline(fileIn, userStr)) {
			std::getline(fileIn, passStr);
			CString ca(userStr.c_str());
			CString cb(passStr.c_str());
			//CStringA ca(line.c_str(), CP_UTF8);
			if (username == ca) {
				fileIn.close();
				return false;
			}
		}
		fileIn.close();
	}
	std::string strU, strP;
	CT2A cvStrU(username);
	CT2A cvStrP(password);
	strU = cvStrU;
	strP = cvStrP;
	fileOut.open("taikhoan.txt", std::ios_base::app);
	fileOut << strU << std::endl << strP << std::endl;
	fileOut.close();
	return true;
}