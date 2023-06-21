#include "pch.h"
#include "ClientSocket.h"

ClientSocket::ClientSocket(CfinalchatDlg* cFinalchatDlg) {
	this->cFinalchatDlg = cFinalchatDlg;
}

ClientSocket::~ClientSocket() {

}

void ClientSocket::OnReceive(int nErrorCode) {
    if (nErrorCode == 0) {
        // Nhận được dữ liệu từ server, xử lý và hiển thị nó trên giao diện người dùng
        char buff[512];
        ZeroMemory(buff, 512);
        int len = Receive(buff, 512, 0);
        if (len > 0) {
            CString msg(buff, len);
            if (cFinalchatDlg != NULL) {
                cFinalchatDlg->GetMessageFromSocket(msg);
            }
        }
    }
    else {
        // Lỗi nhận dữ liệu
    }
}