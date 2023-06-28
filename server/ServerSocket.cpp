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
    ServerSocket* client = cServerDlg->clients.at(cServerDlg->clientCount).clientSock;
    if (!Accept(*client)) {
        cServerDlg->MsgBox(_T("Không thể chấp nhận kết nối."));
        delete client;
        return;
    }
    cServerDlg->clientCount++;
    //cServerDlg->MsgBox(_T("Kết nối thành công."));
    cServerDlg->GetClients(client);
    CAsyncSocket::OnAccept(nErrorCode);
}


void ServerSocket::OnReceive(int nErrorCode) {
    char buff[1024];
    ZeroMemory(buff, 1024);
    int len = this->Receive(buff, 100);
    if (len > 0) {
        CString msg(buff, len);
        CString signal = GetSignal(msg);
        if (signal == _T("SIGNIN")) {
            //CString userPass = GetUserPass(msg);
            if (GetUserPass(msg)) { //Gửi tín hiệu khi tài khoản đúng
                if (CheckIfUsernameLogin(tempName)) {//Kiểm tra đã tài khoản đã bị login chưa
                    CString signalSend = _T("SIGNINACP");
                    CW2A buff(signalSend, CP_UTF8);
                    Sleep(500);
                    if (!this->Send(buff, signalSend.GetLength())) {
                        cServerDlg->MessageBox(_T("Có lỗi xảy ra khi gửi thông báo chấp nhận đăng nhập tới client"));
                    }
                    cServerDlg->m_list_msg.AddString(tempName + _T(" logined"));
                    cServerDlg->clients.at(cServerDlg->clientCount-1).name = tempName;
                    //Sửa vector chưa client đó, thêm tên
                    CString tempName2 = tempName;
                    //Gửi thông tin có client mới online cho các client khác
                    for (int j = 0; j < cServerDlg->clientCount; j++) {
                        if (cServerDlg->clients.at(j).name != tempName2) {
                            CString signalSend;
                            signalSend.Format(_T("NEWCL %s "), tempName2);
                            CW2A buff(signalSend, CP_UTF8);
                            cServerDlg->clients.at(j).clientSock->Send(buff, signalSend.GetLength());
                        }
                    }
                    //Gửi các username đã online cho client mới
                    for (int j = 0; j < cServerDlg->clientCount; j++) {
                        if (cServerDlg->clients.at(j).name != tempName2) {
                            CString signalSend;
                            signalSend.Format(_T("OLDCL %s"), cServerDlg->clients.at(j).name);
                            CW2A buff(signalSend, CP_UTF8);
                            this->Send(buff, signalSend.GetLength());
                        }
                    }

                }
                else { //Nếu đã bị login trước thì gửi tín hiệu LOGIN2
                    CString signalSend = _T("LOGIN2");
                    CW2A buff(signalSend, CP_UTF8);
                    cServerDlg->clients.at(cServerDlg->clientCount-1).clientSock->Send(buff, signalSend.GetLength());
                }
            }
            else { //Gửi tín hiệu khi sai tài khoản
                CString signalSend = _T("WRONG");
                CW2A buff(signalSend, CP_UTF8);
                cServerDlg->clients.at(cServerDlg->clientCount).clientSock->Send(buff, signalSend.GetLength());
            }
        }
        else if (signal == _T("SENDMSG")) {
            SendMsgStruct temp = GetSendMsgStruct(msg);
            for (int j = 0; j < cServerDlg->clientCount; j++) {
                if (cServerDlg->clients.at(j).name == temp.targetName) {
                    CString newMsg;
                    newMsg.Format(_T("NEWMSG %s %s %s"), temp.targetName, temp.yourName, temp.msg);
                    CW2A buff(newMsg, CP_UTF8);
                    cServerDlg->clients.at(j).clientSock->Send(buff, newMsg.GetLength());
                }
            }
        }
        else if (signal == _T("SIGNUP")) {
            if (SignUpUser(msg)) {
                CString newMsg;
                newMsg.Format(_T("ACPSIGNUP"));
                CW2A buff(newMsg, CP_UTF8);
                cServerDlg->clients.at(cServerDlg->clientCount-1).clientSock->Send(buff, newMsg.GetLength());
            }
            else {
                CString newMsg;
                newMsg.Format(_T("NOACPSIGNUP"));
                CW2A buff(newMsg, CP_UTF8);
                cServerDlg->clients.at(cServerDlg->clientCount - 1).clientSock->Send(buff, newMsg.GetLength());
            }
        }
        else if (signal == _T("LOGOUT")) {
            CString username = GetUsernameLogout(msg);
            for (int i = 0; i < cServerDlg->clientCount; i++) {
                if (cServerDlg->clients.at(i).name == username) {
                    cServerDlg->clients.erase(cServerDlg->clients.begin() + i);
                }
                else {
                    CString newMsg;
                    newMsg.Format(_T("CLOFF %s", username));
                    CW2A buff(newMsg, CP_UTF8);
                    cServerDlg->clients.at(i).clientSock->Send(buff, newMsg.GetLength());
                }
            }
            cServerDlg->clientCount--;
            this->Close();
        }
    }

    CAsyncSocket::OnReceive(nErrorCode);
}

CString ServerSocket::GetSignal(CString msg) {
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

bool ServerSocket::GetUserPass(CString msg) {
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

bool ServerSocket::CheckIfUsernameLogin(CString username) {
    //Kiểm tra username đã đăng nhập chưa
    for (int i = 0; i < cServerDlg->clientCount; i++) {
        if (username == cServerDlg->clients.at(i).name) {
            return false;
        }
    }
    return true;
}

SendMsgStruct ServerSocket::GetSendMsgStruct(CString msg) {
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

bool ServerSocket::SignUpUser(CString msg) {
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

CString ServerSocket::GetUsernameLogout(CString msg) {
    CString username = NULL;
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
    }
    return username;
}