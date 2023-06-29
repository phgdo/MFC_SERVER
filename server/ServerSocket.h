#pragma once
#include <afxsock.h>
#include "serverDlg.h"
#include <thread>
#include <fstream>
#include <string>
class CserverDlg;

struct SendMsgStruct {
	CString targetName;
	CString yourName;
	CString msg;
};


class ServerSocket : public CAsyncSocket
{
public:
	CserverDlg* cServerDlg;
	ServerSocket(CserverDlg* cServerDlg);
	~ServerSocket();
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);	
	CString GetSignal(CString msg);
	bool GetUserPass(CString msg);
	bool CheckIfUsernameLogin(CString username);
	CString tempName;
	std::ifstream fileIn;
	std::ofstream fileOut;
	SendMsgStruct GetSendMsgStruct(CString msg);
	bool SignUpUser(CString msg);
	CString GetUsernameLogout(CString msg);

	void AcptCL(ServerSocket* client);
	void SendMsgProcess(ServerSocket* client, CString msg);
};



