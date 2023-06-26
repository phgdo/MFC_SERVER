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
    //cServerDlg->MsgBox(_T("Kết nối thành công."));
    cServerDlg->GetClients(client);
    client->AsyncSelect(FD_READ);
    CAsyncSocket::OnAccept(nErrorCode);
}

void ServerSocket::ReceiveThread() {
    
}

void ServerSocket::OnReceive(int nErrorCode) {
    char buff[100];
    this->Receive(buff, 100);
    cServerDlg->MessageBox(_T("Oke nè"));

    CAsyncSocket::OnReceive(nErrorCode);
}