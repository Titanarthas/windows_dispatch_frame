#pragma once
#include <map>
#include "ReaderWriterLock.h"
// #include <atomic> 
using namespace std;


enum
{
	THREAD_TEST = 1000,
	REGIST_THREAD_ID,
	UNREGIST_THREAD_ID
};

enum
{
	MSG_TEST = WM_USER + 100,
	MSG_DISPATCH_THREAD_MSG
};

class CThreadManage
{
public:
	static CThreadManage *GetInstance();

	BOOL RegistThread(UINT nThreadType, UINT nThreadID);
	BOOL UnRegistThread(UINT nThreadType);

	BOOL PostThreadMassage(UINT nThreadType, UINT nMsgType, WPARAM wParam,LPARAM lParam);

protected:

private:
	CThreadManage();
	~CThreadManage();
	CThreadManage(const CThreadManage &);
	CThreadManage& operator = (const CThreadManage &);

	static CThreadManage *m_pInstance;

	map<UINT, UINT> m_mapDTThread;
	CReaderWriterLock m_lockRW;
};

BOOL PostUtsMessage(UINT nThreadType, UINT nMsgType, WPARAM wParam,LPARAM lParam);
BOOL RegistUtsThread(UINT nThreadType, UINT nThreadID);
BOOL UnRegistThread(UINT nThreadType);

