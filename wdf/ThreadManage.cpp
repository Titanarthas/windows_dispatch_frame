#include "stdafx.h"
#include "ThreadManage.h"
#include "DTMessage.h"


CThreadManage *CThreadManage::m_pInstance = NULL;
// map<UINT, UINT> CThreadManage::m_mapIDThread;

CThreadManage::CThreadManage(void)
{
}


CThreadManage::~CThreadManage(void)
{
}

CThreadManage * CThreadManage::GetInstance()
{
	if(m_pInstance == NULL)
	{   
		CReaderWriterLock lock;
		lock.EnterWriter();

		if(m_pInstance == NULL)
		{
			m_pInstance = new CThreadManage();
		}

		lock.LeaveWriter();
	}

	return m_pInstance;
}

BOOL CThreadManage::RegistThread( UINT nThreadType, UINT nThreadID )
{
	m_lockRW.EnterWriter();
	map<UINT, UINT>::iterator it = m_mapDTThread.find(nThreadType);
	
	if (it != m_mapDTThread.end())
	{
		m_lockRW.LeaveWriter();
		return FALSE;
	}

	for (it = m_mapDTThread.begin(); it != m_mapDTThread.end(); ++it)
	{
		CRegistThreadID *pRegist = new CRegistThreadID(it->first, it->second);
		PostThreadMassage(nThreadID, REGIST_THREAD_ID, WPARAM(pRegist), 0);

		CRegistThreadID *pRegist2 = new CRegistThreadID(nThreadType, nThreadID);
		PostThreadMassage(it->second, REGIST_THREAD_ID, WPARAM(pRegist2), 0);
	}

	m_mapDTThread[nThreadType] = nThreadID;
	m_lockRW.LeaveWriter();

	return TRUE;
}

BOOL CThreadManage::UnRegistThread( UINT nThreadType )
{
	m_lockRW.EnterWriter();
	map<UINT, UINT>::iterator it = m_mapDTThread.find(nThreadType);
	UINT nThreadID = 0;

	if (it != m_mapDTThread.end())
	{
		nThreadID = it->second;
		m_mapDTThread.erase(it);
	}

	for (it = m_mapDTThread.begin(); it != m_mapDTThread.end(); ++it)
	{
		CUnRegistThreadID *pUnRegist = new CUnRegistThreadID(nThreadType, nThreadID);
		PostThreadMassage(it->second, UNREGIST_THREAD_ID, WPARAM(pUnRegist), 0);
	}

	m_lockRW.LeaveWriter();

	return TRUE;
}

BOOL CThreadManage::PostThreadMassage( UINT nThreadType, UINT nMsgType, WPARAM wParam,LPARAM lParam )
{
	m_lockRW.EnterReader();
	map<UINT, UINT>::iterator it2 = m_mapDTThread.find(nThreadType);
	m_lockRW.LeaveWriter();

	if (it2 == m_mapDTThread.end())
	{
		return FALSE;
	}

	return PostThreadMessage(it2->second, nMsgType, wParam, lParam);
}

BOOL PostUtsMessage( UINT nThreadType, UINT nMsgType, WPARAM wParam,LPARAM lParam )
{
	return CThreadManage::GetInstance()->PostThreadMassage(nThreadType, nMsgType, wParam, lParam);
}

BOOL RegistUtsThread( UINT nThreadType, UINT nThreadID )
{
	return CThreadManage::GetInstance()->RegistThread(nThreadType, nThreadID);
}

BOOL UnRegistThread( UINT nThreadType )
{
	return CThreadManage::GetInstance()->UnRegistThread(nThreadType);
}



