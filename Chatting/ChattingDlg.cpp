﻿
// ChattingDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Chatting.h"
#include "ChattingDlg.h"
#include "afxdialogex.h"
#include <mmsystem.h>
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


// CChattingDlg dialog



CChattingDlg::CChattingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATTING_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChattingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MSG, m_list_msg);
	DDX_Control(pDX, IDC_BUTTON_SIGNIN, m_btn_signin);
	DDX_Control(pDX, IDC_BUTTON_SIGNUP, m_btn_signup);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_edt_username);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_edt_password);
	DDX_Control(pDX, IDC_LIST_USER, m_list_user);
	DDX_Control(pDX, IDC_EDIT_MSG, m_edt_msg);
	DDX_Control(pDX, IDC_BUTTON_SEND, m_btn_send);
	DDX_Control(pDX, IDC_STATIC_US, m_txt_us);
	DDX_Control(pDX, IDC_STATIC_PS, m_txt_ps);
	DDX_Control(pDX, IDC_STATIC_LOGIN, m_txt_login);
	DDX_Control(pDX, IDC_EDIT_PORT, m_edt_port);
	DDX_Control(pDX, IDC_STATIC_PORT, m_static_port);
	DDX_Control(pDX, IDC_BUTTON_Connect, m_btn_connect);
	DDX_Control(pDX, IDC_BUTTON_LOGOUT, m_btn_logout);
	DDX_Control(pDX, IDC_STATIC_ONLINE, m_static_online);
	DDX_Control(pDX, IDC_EDIT_IP, m_edt_ip);
	DDX_Control(pDX, IDC_STATIC_IP, m_static_ip);
}

BEGIN_MESSAGE_MAP(CChattingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SIGNIN, &CChattingDlg::OnBnClickedButtonSignin)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CChattingDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_SIGNUP, &CChattingDlg::OnBnClickedButtonSignup)
	ON_LBN_SELCHANGE(IDC_LIST_USER, &CChattingDlg::OnLbnSelchangeListUser)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_Connect, &CChattingDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_LOGOUT, &CChattingDlg::OnBnClickedButtonLogout)
	ON_WM_KEYDOWN()
	ON_EN_CHANGE(IDC_EDIT_MSG, &CChattingDlg::OnEnChangeEditMsg)
END_MESSAGE_MAP()


// CChattingDlg message handlers

BOOL CChattingDlg::OnInitDialog()
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
	//Set giới hạn kí tự của edittext
	m_edt_msg.SetLimitText(80);
	m_edt_username.SetLimitText(15);
	m_edt_password.SetLimitText(15);
	m_edt_port.SetLimitText(5);

	m_btn_signin.EnableWindow(FALSE);
	m_btn_signup.EnableWindow(FALSE);
	AfxSocketInit(NULL);
	this->m_sockclient = new SocketClient(this);
	m_sockclient->Create();
	
	yourname = "";

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChattingDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChattingDlg::OnPaint()
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
HCURSOR CChattingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CChattingDlg::MsgBox(CString msg) {
	MessageBox(msg);
}

void CChattingDlg::OnBnClickedButtonSignin()
{
	CString username, password;
	//Lấy username, password từ edit text

	m_edt_username.GetWindowText(username);
	if (username.IsEmpty()) {
		MessageBox(_T("Bạn chưa nhập username"));
		return;
	}

	m_edt_password.GetWindowText(password);
	if (password.IsEmpty()) {
		MessageBox(_T("Bạn chưa nhập password"));
		return;
	}
	CString message;
	message.Format(_T("SIGNIN %s %s"), username, password);
	CW2A buff(message, CP_UTF8);
	if (!m_sockclient->Send(buff, message.GetLength())) {
		MessageBox(_T("Không thể gửi yêu cầu đến Server"));
	}
	//m_sockclient->Send(buff, message.GetLength());
	//m_sockclient->Send(_T())
}


void CChattingDlg::GetMsgBox(CString msg) {
	MessageBox(msg);
}

void CChattingDlg::OnBnClickedButtonSend()
{
	int len = 0;
	char msg[1024];
	CString msg_from_edt;
	m_edt_msg.GetWindowText(msg_from_edt);
	if (msg_from_edt == _T("")) {
		return;
	}
	if (m_list_user.GetCurSel() < 0) {
		return;
	}
	CString msgToLog;
	msgToLog.Format(_T("Tôi: %s"), msg_from_edt);
	int checkIfTargetName = 0;
	for (int i = 0; i < msgLogs.size(); i++) {
		if (msgLogs.at(i).targetName == targetName) {
			checkIfTargetName++;
			msgLogs.at(i).msg.push_back(msgToLog);
		}
	}
	if (checkIfTargetName == 0) {
		MessageStruct newMsgStruct;
		newMsgStruct.msg.push_back(msgToLog);
		newMsgStruct.targetName = targetName;
		msgLogs.push_back(newMsgStruct);
	}

	CString message;
	message.Format(_T("SENDMSG %s %s %s"), targetName, yourname, msg_from_edt);
	CW2A buff(message, CP_UTF8);
	//CT2W unicode(message, CP_UTF8);
	if (!m_sockclient->Send(buff, message.GetLength())) {
		MessageBox(_T("Không thể gửi yêu cầu đến Server"));
	}
//	m_sockclient->Send(buff, message.GetLength());
	m_edt_msg.SetWindowTextW(_T(""));
	m_list_msg.AddString(msgToLog);
	//PrintMsg(targetName);

	// TODO: Add your control notification handler code here
}

void CChattingDlg::LoginSuccess() {
	CString username;
	m_edt_username.GetWindowTextW(username);
	this->SetWindowTextW(_T("Hello ")+username);
	//Nhưng control cần ẩn đi
	m_edt_username.GetWindowText(yourname);
	m_edt_username.ShowWindow(SW_HIDE);
	m_edt_password.ShowWindow(SW_HIDE);
	m_btn_signin.ShowWindow(SW_HIDE);
	m_btn_signup.ShowWindow(SW_HIDE);
	m_txt_login.ShowWindow(SW_HIDE);
	m_txt_us.ShowWindow(SW_HIDE);
	m_txt_ps.ShowWindow(SW_HIDE);
	m_edt_port.ShowWindow(SW_HIDE);
	m_static_port.ShowWindow(SW_HIDE);
	m_btn_connect.ShowWindow(SW_HIDE);
	m_edt_ip.ShowWindow(SW_HIDE);
	m_static_ip.ShowWindow(SW_HIDE);

	//Những control cần show ra
	m_list_msg.ShowWindow(SW_SHOW);
	m_list_user.ShowWindow(SW_SHOW);
	m_btn_send.ShowWindow(SW_SHOW);
	m_edt_msg.ShowWindow(SW_SHOW);
	m_btn_logout.ShowWindow(SW_SHOW);
	m_static_online.ShowWindow(SW_SHOW);


	//
	m_edt_username.EnableWindow(FALSE);
	m_edt_password.EnableWindow(FALSE);
	//m_static_username.ShowWindow(SW_SHOW);
	//CString printUsername;
	//printUsername.Format(_T("Username: %s"), username);
	//m_static_username.SetWindowTextW(_T("Username: ") + username);
}


void CChattingDlg::OnBnClickedButtonSignup()
{
	CString username, password;
	//Lấy username, password từ edit text
	m_edt_username.GetWindowText(username);
	if (username.IsEmpty()) {
		MessageBox(_T("Vui lòng nhập username để đăng ký"));
		return;
	}

	m_edt_password.GetWindowText(password);
	if (password.IsEmpty()) {
		MessageBox(_T("Vui lòng nhập password để đăng ký"));
		return;
	}
	for (int i = 0; i < username.GetLength(); i++) {
		if (username.GetAt(i) == ' ') {
			MessageBox(_T("Username có chứa dấu khoảng trắng. Vui lòng nhập lại."));
			return;
		}
	}
	for (int i = 0; i < password.GetLength(); i++) {
		if (password.GetAt(i) == ' ') {
			MessageBox(_T("Password có chứa dấu khoảng trắng. Vui lòng nhập lại."));
			return;
		}
	}
	CString message;
	message.Format(_T("SIGNUP %s %s"), username, password);
	CW2A buff(message, CP_UTF8);
	if (!m_sockclient->Send(buff, message.GetLength())) {
		MessageBox(_T("Không thể gửi yêu cầu đến Server"));
	}
	//m_sockclient->Send(buff, message.GetLength());
	//m_edt_username.ShowWindow(SW_HIDE);
	// TODO: Add your control notification handler code here
}


void CChattingDlg::OnLbnSelchangeListUser()
{
	
	int index = m_list_user.GetCurSel();
	if (index != LB_ERR) {
		 m_list_user.GetText(index, targetName);
	}
	if (GetFirstWordFromDialog() == targetName) {
		CString username;
		m_edt_username.GetWindowTextW(username);
		this->SetWindowTextW(_T("Hello ") + username);
	}
	PrintMsg(targetName);
	// TODO: Add your control notification handler code here
}

void CChattingDlg::F5IfNewMSG() {

}


void CChattingDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if (yourname != "") {
		CString message;
		message.Format(_T("LOGOUT %s"), yourname);
		CW2A buff(message, CP_UTF8);
		m_sockclient->Send(buff, message.GetLength());
	}
	m_sockclient->Close();
	CDialogEx::OnClose();
}

void CChattingDlg::PrintMsg(CString targetName) {
	m_list_msg.ResetContent();
	for (int i = 0; i < msgLogs.size(); i++) {
		if (msgLogs.at(i).targetName == targetName) {
			for (int j = 0; j < msgLogs.at(i).msg.size(); j++) {
				m_list_msg.AddString(msgLogs.at(i).msg.at(j));
			}
		}
	}
}

void CChattingDlg::BoldUsernameNewMsg(CString username) {
	for (int i = 0; i < m_list_user.GetCount(); i++) {
		CString item = _T("");
		m_list_user.GetText(i, item);
		if (item == username) {
			//CFont* font;
			//font->CreateFontW(16, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, _T("Arial"));
			m_list_user.DeleteString(i);
			m_list_user.InsertString(0, username);
		}
	}
}

void CChattingDlg::PlaySoundIfNewMsg() {
	PlaySound(_T("new_message.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void CChattingDlg::OnBnClickedButtonConnect()
{
	CString ip = _T("");
	CString port =_T("");
	m_edt_port.GetWindowText(port);
	m_edt_ip.GetWindowText(ip);

	int num = _ttoi(port);
	if (num == 0) {
		num = 8888;
		MessageBox(_T("Không thể kết nối server trên port") + port + _T(". Kết nối server trên port mặc định 8888"));
		port = _T("8888");
		m_edt_port.SetWindowTextW(port);
	}

	
	if (ip.IsEmpty()) {
		ip = _T("127.0.0.1");
		MessageBox(_T("Địa chỉ IP không đúng. Chương trình tự động kết nối đến IP: 127.0.0.1"));
		m_edt_ip.SetWindowTextW(ip);
	}
	else {
		for (int i = 0; i < port.GetLength(); i++) {
			if (ip[i] == ' ') {
				ip.Delete(i, 1);
			}
		}
	}
	m_sockclient->Connect(ip, num);


	// TODO: Add your control notification handler code here
}

void CChattingDlg::SetTextDialogWhenNewMsg(CString sender) {
	this->SetWindowTextW(sender + _T(" đã gửi tin nhắn cho bạn."));
}

CString CChattingDlg::GetFirstWordFromDialog() {
	CString msg;
	this->GetWindowTextW(msg);
	CString signal;
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

void CChattingDlg::OnBnClickedButtonLogout()
{
	CString message;
	message.Format(_T("LOGOUT %s"), yourname);
	CW2A buff(message, CP_UTF8);
	if (!m_sockclient->Send(buff, message.GetLength())) {
		MessageBox(_T("Không thể gửi yêu cầu đến Server"));
	}
	m_sockclient->Close();
	Logout();
	// TODO: Add your control notification handler code here
}

void CChattingDlg::Logout() {
	yourname = _T("");
	m_list_user.ResetContent();
	m_list_msg.ResetContent();
	msgLogs.clear();
	this->SetWindowTextW(_T("ChatClient"));
	m_edt_username.SetWindowTextW(_T(""));
	m_edt_password.SetWindowTextW(_T(""));
	//Các control cần hiển thị
	m_edt_username.ShowWindow(SW_SHOW);
	m_edt_password.ShowWindow(SW_SHOW);
	m_btn_signin.ShowWindow(SW_SHOW);
	m_btn_signup.ShowWindow(SW_SHOW);
	m_txt_login.ShowWindow(SW_SHOW);
	m_txt_us.ShowWindow(SW_SHOW);
	m_txt_ps.ShowWindow(SW_SHOW);
	m_edt_port.ShowWindow(SW_SHOW);
	m_static_port.ShowWindow(SW_SHOW);
	m_btn_connect.ShowWindow(SW_SHOW);
	m_edt_ip.ShowWindow(SW_SHOW);
	m_static_ip.ShowWindow(SW_SHOW);
	//Những control cần ẩn đi
	m_list_msg.ShowWindow(SW_HIDE);
	m_list_user.ShowWindow(SW_HIDE);
	m_btn_send.ShowWindow(SW_HIDE);
	m_edt_msg.ShowWindow(SW_HIDE);
	m_btn_logout.ShowWindow(SW_HIDE);
	m_static_online.ShowWindow(SW_HIDE);
	m_btn_signin.EnableWindow(FALSE);
	m_btn_signup.EnableWindow(FALSE);
	m_btn_connect.EnableWindow(TRUE); //Mở nút kết nối
	m_edt_port.EnableWindow(TRUE);
	m_edt_ip.EnableWindow(TRUE);

	m_edt_username.EnableWindow(TRUE);
	m_edt_password.EnableWindow(TRUE);
	m_sockclient->Create();
}


void CChattingDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	HWND keyboardFocus = ::GetFocus();
	if (!(keyboardFocus == this->m_edt_msg)) {
		return;
	}
	if (!(keyboardFocus == this->m_edt_username)) {
		return;
	}
	if (!(keyboardFocus == this->m_edt_password)) {
		return;
	}
	if (!(keyboardFocus == this->m_edt_port)) {
		return;
	}
	if (nChar == VK_RETURN) {
		OnBnClickedButtonSend();
	}
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CChattingDlg::PreTranslateMessage(MSG* msg) {
	if (msg->message == WM_KEYDOWN) {
		if (msg->wParam == VK_RETURN) {
			HWND keyboardFocus = ::GetFocus();
			//Xử lý đăng nhập
			if ((keyboardFocus == this->m_edt_username)) {
				OnBnClickedButtonSignin();
				return TRUE;
			}
			if ((keyboardFocus == this->m_edt_password)) {
				OnBnClickedButtonSignin();
				return TRUE;
			}
			if ((keyboardFocus == this->m_btn_signin)) {
				OnBnClickedButtonSignin();
				return TRUE;
			}

			//Xử lý kết nối
			if ((keyboardFocus == this->m_edt_port)) {
				OnBnClickedButtonConnect();
				return TRUE;
			}
			if ((keyboardFocus == this->m_edt_ip)) {
				OnBnClickedButtonConnect();
				return TRUE;
			}
			if ((keyboardFocus == this->m_btn_connect)) {
				OnBnClickedButtonConnect();
				return TRUE;
			}
			//Xử lý gửi tin nhắn
			if ((keyboardFocus == this->m_edt_msg)) {
				OnBnClickedButtonSend();
				return TRUE;
			}
			if ((keyboardFocus == this->m_btn_send)) {
				OnBnClickedButtonSend();
				return TRUE;
			}
			//Xử lý đăng ký
			if ((keyboardFocus == this->m_btn_signup)) {
				OnBnClickedButtonSignup();
				return TRUE;
			}
			if ((keyboardFocus == this->m_btn_logout)) {
				OnBnClickedButtonLogout();
				return TRUE;
			}
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(msg);
}


void CChattingDlg::OnEnChangeEditMsg()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
