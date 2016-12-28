///////////////////////////////////////////////////////////
//



#ifndef _CXTHREADHEADER_
#define _CXTHREADHEADER_

/***********************************************************/
// 头文件
#include <windows.h>

class  CXThread 
{
public:
	CXThread ();
	virtual ~CXThread ();

	// 线程管理函数
	BOOL	CreateThread();
	BOOL	Suspend();   	// 挂起线程
	BOOL	Resume(); 		// 恢复挂起的线程
	BOOL	Kill(); 		// 终止线程
	BOOL	Release();		// Release handle of the thread
	BOOL	SetPriority(int Priority);
   	DWORD	GetExitCode();

	BOOL	IsCreated ()			{ return (this->m_hThread != NULL); }
	BOOL	IsActive()				{ return this->m_bActive; } 	// 检查是否活跃
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

	DWORD		m_hThreadId;		// 线程ID
	HANDLE		m_hThread;			// 线程句柄
 	BOOL 		m_bActive; 			// 活跃标志

	HANDLE		m_hEventKill;		// Kill event handle for elegent stop

	DWORD		m_dwKillingWait;	// Waiting timeout for kill thread

	int			m_iPriority;		// 线程优先级

protected:
	// a friend
 	static DWORD WINAPI _ThreadFunc(LPVOID pvThread);
};

#endif // _CXThreadHEADER_