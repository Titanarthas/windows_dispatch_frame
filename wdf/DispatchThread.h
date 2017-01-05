#pragma once
// #include "afxwin.h"
#include "CTHREAD.H"
#include "DTMessage.h"
#include "ThreadManage.h"
#include <map>
using namespace std;

#define BEGIN_DT_BIND \
	virtual int DispatchMsg(UINT nMsgType, WPARAM wParam, LPARAM lParam) \
	{ \

#define  DT_BIND(MSG_TYPE, DT_CLASS_TYPE) \
	if (nMsgType == MSG_TYPE) { \
		DT_CLASS_TYPE *pMsg = (DT_CLASS_TYPE *)wParam; \
		if (pMsg) { \
			OnDTMessage(*pMsg, lParam); \
			delete pMsg; \
			pMsg = NULL; \
		}\
		return 0;\
	} \

#define END_DT_BIND(BASE_CLASS) \
	return BASE_CLASS::DispatchMsg(nMsgType, wParam, lParam); \
	} \

#define END_BASE_DT_BIND \
	return -1;\
	}\

class CDispatchThread  : public CXThread
{
public:
	CDispatchThread(void);
	virtual ~CDispatchThread(void);

protected:
	virtual BOOL InitInstance();
	virtual void ExitInstance();

	virtual DWORD Run();

BEGIN_DT_BIND
	DT_BIND(REGIST_THREAD_ID, CRegistThreadID)
	DT_BIND(UNREGIST_THREAD_ID, CUnRegistThreadID)
END_BASE_DT_BIND

	template<class TYPE>
	BOOL PostThreadMassage(const TYPE& msg, UINT nMsgType, int nDstAddr);

	void OnDTMessage(CRegistThreadID msg, int nAddr);
	void OnDTMessage(CUnRegistThreadID msg, int nAddr);
	
	int m_nThreadType;

private:
	map<UINT, UINT> m_mapDTThread;
};

template<class TYPE>
BOOL CDispatchThread::PostThreadMassage(const TYPE& msg, UINT nMsgType, int nDstAddr)
{
	map<UINT, UINT>::iterator it = m_mapDTThread.find(nDstAddr);
	m_lockRW.LeaveWriter();

	if (it == m_mapDTThread.end())
	{
		return FALSE;
	}

	TYPE *pMsg = new TYPE(msg);
	
	if (!PostThreadMessage(it->second, nMsgType,  WPARAM(pMsg), m_nThreadType))
	{
		delete pMsg;
		pMsg = NULL;
	}

	return TRUE;
}


