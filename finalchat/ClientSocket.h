#pragma once
#include <afxsock.h>
#include "finalchatDlg.h"
class CfinalchatDlg;
class ClientSocket:public CAsyncSocket
{
public:
	CfinalchatDlg* cFinalchatDlg;
	ClientSocket(CfinalchatDlg* cFinalchatDlg);
	~ClientSocket();
	void OnReceive(int nErrorCode);
};

