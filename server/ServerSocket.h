#pragma once
#include <afxsock.h>
#include "serverDlg.h"
#include <thread>
class CserverDlg;

class ServerSocket : public CAsyncSocket
{
public:
	CserverDlg* cServerDlg;
	ServerSocket(CserverDlg* cServerDlg);
	~ServerSocket();
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);	
	void ReceiveThread();
};

