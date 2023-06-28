#pragma once
#include <afxsock.h>
#include "finalchatDlg.h"
class CfinalchatDlg;
class ClientSock:public CAsyncSocket
{
public:
	CfinalchatDlg* cFinalchatDlg;
	ClientSock(CfinalchatDlg* cFinalchatDlg);
	~ClientSock();
	void OnReceive(int nErrorCode);
};

