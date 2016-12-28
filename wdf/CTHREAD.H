///////////////////////////////////////////////////////////
//



#ifndef _CXTHREADHEADER_
#define _CXTHREADHEADER_

/***********************************************************/
// ͷ�ļ�
#include <windows.h>

class  CXThread 
{
public:
	CXThread ();
	virtual ~CXThread ();

	// �̹߳�������
	BOOL	CreateThread();
	BOOL	Suspend();   	// �����߳�
	BOOL	Resume(); 		// �ָ�������߳�
	BOOL	Kill(); 		// ��ֹ�߳�
	BOOL	Release();		// Release handle of the thread
	BOOL	SetPriority(int Priority);
   	DWORD	GetExitCode();

	BOOL	IsCreated ()			{ return (this->m_hThread != NULL); }
	BOOL	IsActive()				{ return this->m_bActive; } 	// ����Ƿ��Ծ
	BOOL	IsRunning();

	HANDLE	GetThreadHandle ()		{ return this->m_hThread; }
	DWORD	GetThreadId ()			{ return this->m_hThreadId; }

	void	SetKillingTime(DWORD dwKillingTime)	
									{ m_dwKillingWait = dwKillingTime; }
	DWORD	GetKillingTime()		{ return m_dwKillingWait; }

protected:

	// override these functions in the derived class 
 	virtual BOOL InitInstance()		{ return TRUE; }
 	virtual void ExitInstance()		{ }
 	virtual DWORD Run()				{ return 0; }

	DWORD		m_hThreadId;		// �߳�ID
	HANDLE		m_hThread;			// �߳̾��
 	BOOL 		m_bActive; 			// ��Ծ��־

	HANDLE		m_hEventKill;		// Kill event handle for elegent stop

	DWORD		m_dwKillingWait;	// Waiting timeout for kill thread

	int			m_iPriority;		// �߳����ȼ�

protected:
	// a friend
 	static DWORD WINAPI _ThreadFunc(LPVOID pvThread);
};

#endif // _CXThreadHEADER_