#include "pch.h"
#include "ServerSocket.h"

ServerSocket::ServerSocket(CserverDlg* cServerDlg) {
	this->cServerDlg = cServerDlg;
}

ServerSocket::~ServerSocket() {

}

void ServerSocket::OnAccept(int nErrorCode) {
    if (nErrorCode != 0) {
        cServerDlg->MsgBox(_T("Lỗi khi chấp nhận kết nối."));
        return;
    }
    CAsyncSocket* client = new CAsyncSocket;
    if (!Accept(*client)) {
        cServerDlg->MsgBox(_T("Không thể chấp nhận kết nối."));
        delete client;
        return;
    }
    cServerDlg->clients.push_back(client);
    cServerDlg->MsgBox(_T("Kết nối thành công."));

    client->AsyncSelect(FD_READ);
    CAsyncSocket::OnAccept(nErrorCode);
}

void ServerSocket::ReceiveThread() {
    
}

void ServerSocket::OnReceive(int nErrorCode) {


    char buff[512];
    ZeroMemory(buff, 512);
    if (nErrorCode == 0) {
        cServerDlg->MsgBox(_T("ccccaaa"));
    }
    else {
        cServerDlg->MsgBox(_T("aaaa"));
    }
    int len = Receive(buff, 512, 0);
    if (len <= 0) {
        Close();
        cServerDlg->MsgBox(_T("fail"));
        return;
    }
    CString str(buff);
    cServerDlg->m_list_msg.AddString(str);
    
    for (int i = 0; i < cServerDlg->clients.size(); i++) {
        CAsyncSocket* client = cServerDlg->clients.at(i);
        client->Send(buff, len);
    }
    CAsyncSocket::OnReceive(nErrorCode);
}