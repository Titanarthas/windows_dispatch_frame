#include "stdafx.h"
#include "DispatchThread.h"
#include "ThreadManage.h"


CDispatchThread::CDispatchThread(void)
{
}


CDispatchThread::~CDispatchThread(void)
{
}

BOOL CDispatchThread::InitInstance()
{
	RegistUtsThread(m_nThreadType, m_hThreadId);
	return CXThread::InitInstance();
}

void CDispatchThread::ExitInstance()
{
	UnRegistThread(m_nThreadType);
	return CXThread::ExitInstance();
}

DWORD CDispatchThread::Run()
{
	MSG nMsg = { 0 };

	while (1)
	{
		if ( PeekMessage (&nMsg, NULL, 0, 0, PM_NOREMOVE) )
		{
			//有消息
			if (!GetMessage ( &nMsg, NULL, 0, 0 ) )
			{
				this->ExitInstance();
				return 0;
			}
			else
			{
				if (this->DispatchMsg(nMsg.message, nMsg.wParam, nMsg.lParam))
				{
					TranslateMessage ( &nMsg );
					DispatchMessage ( &nMsg );
				}
			}
		}
		else
		{
			//空闲处理
			Sleep(1);
		}
	}

	return 0;
}

void CDispatchThread::OnDTMessage( CRegistThreadID msg, int nAddr )
{
	map<UINT, UINT>::iterator it = m_mapDTThread.find(msg.m_nThreadType);

	if (it != m_mapDTThread.end())
	{
		return ;
	}

	m_mapDTThread[msg.m_nThreadType] = msg.m_nThreadID;

	return ;
}

void CDispatchThread::OnDTMessage( CUnRegistThreadID msg, int nAddr )
{
	map<UINT, UINT>::iterator it = m_mapDTThread.find(msg.m_nThreadType);

	if (it != m_mapDTThread.end())
	{
		m_mapDTThread.erase(it);
	}
}

