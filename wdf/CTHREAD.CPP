///////////////////////////////////////////////////////////
//

/***********************************************************/
// 头文件
#include "stdafx.h"
#include "cthread.h"
#include "process.h"

///////////////////////////////////////////////////////////
//	CXThread
//		构造函数		
//	输入：	无     			 	
//	输出:	无
//  返回:	无
CXThread::CXThread ()
{
	this->m_hThread			= NULL;
	this->m_hThreadId		= 0;

	this->m_hEventKill		= ::CreateEvent(NULL, TRUE, FALSE, NULL);
	::ResetEvent(this->m_hEventKill) ;

	this->m_bActive			= FALSE;

	//
	// This value is the waiting period for killing routine.
	// When the main process call kill() to stop the main loop of this thread,
	// in normal case, a kill event set first, and then the main loop will be
	// noticed and stop itself, but if the main loop blocked on some blocking
	// function, such as recv(), it cannot see that kill event, so we must
	// kill it manually. After the killing wait period, the main process will
	// call terminatethread() to kill this thread directly.
	//
	this->m_dwKillingWait	= 1000 * 30;	// 30 seconds
	m_iPriority = THREAD_PRIORITY_NORMAL;
}

///////////////////////////////////////////////////////////
//	~CXThread
//		析构		
//	输入：	无     			 	
//	输出:	无
//  返回:	无
CXThread::~CXThread ()
{
	Kill();
	Release();

	::CloseHandle(m_hEventKill);
}

///////////////////////////////////////////////////////////
//	CreateThread
//		创建线程		
//	输入：	无     			 	
//	输出:	无
//  返回:	BOOL
BOOL CXThread::CreateThread()
{
	// 判断是否已经创建
	if (!this->IsCreated ()) 
	{

#ifdef _RUN_WIN32_THREAD
		// Win32 version of create thread routine
 		m_hThread = ::CreateThread(
							NULL, 
							0,
							(LPTHREAD_START_ROUTINE)_ThreadFunc,
							(LPVOID)this,
							CREATE_SUSPENDED,	// create thread as suspended
							(LPDWORD) &m_hThreadId
							);
#else
		// _crt version of create thread routine
		m_hThread = (HANDLE)_beginthreadex(
							NULL,
							0, 
							(unsigned int (__stdcall*)(void*))_ThreadFunc, 
							(LPVOID) this, 
							CREATE_SUSPENDED,	// create thread as suspended, 
							(unsigned int*)&m_hThreadId
							);
#endif

		if (m_hThread == NULL) 
		{
  			return FALSE;
		}

 		m_bActive = TRUE;

		// 设置线程优先级并继续线程，此时继续可以保证 m_hThread的变量已经赋值
		if (m_iPriority != THREAD_PRIORITY_NORMAL)
			::SetThreadPriority(m_hThread, m_iPriority);
		::ResumeThread(m_hThread);

 		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////
//	Suspend
//		挂起当前线程
//	输入：	无     			 	
//	输出:	无
//  返回:	BOOL
BOOL CXThread::Suspend()
{
 	m_bActive = FALSE;
 	return (-1 != ::SuspendThread(m_hThread));//win32 API         
}

///////////////////////////////////////////////////////////
//	Kill
//		Elegant kill thread using event communication scheme. Not using
//		TerminateThread(), because this function may cause some resource
//		leak in some cases.
//	输入：	无     			 	
//	输出:	无
//  返回:	BOOL
BOOL CXThread::Kill()
{
	if (this->m_hThread == NULL)
		return FALSE;


	// reset the m_hEventKill which signals the thread to shutdown
	::SetEvent(this->m_hEventKill);

	// allow thread to run at higher priority during kill process
	if (::WaitForSingleObject(this->m_hThread, m_dwKillingWait) == WAIT_TIMEOUT)
		::TerminateThread(this->m_hThread, 0);

	::ResetEvent(this->m_hEventKill);

	m_bActive = FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////
//	Resume
//		恢复执行
//	输入：	无     			 	
//	输出:	无
//  返回:	BOOL
BOOL CXThread::Resume()
{
 	m_bActive = TRUE;
 	return (-1 != ::ResumeThread(m_hThread)); //win32 API
}

///////////////////////////////////////////////////////////
//	Resume
//		恢复执行
//	输入：	无     			 	
//	输出:	无
//  返回:	BOOL
BOOL CXThread::Release()
{
	if ( m_hThread == NULL )
		return TRUE;
    
    BOOL bClosed = ::CloseHandle(m_hThread);

	m_hThread = NULL;
	return bClosed;
}

///////////////////////////////////////////////////////////
//	SetPriority
//		设置优先级
//	输入：	无     			 	
//	输出:	无
//  返回:	BOOL， FALSE表示设置当前的线程优先级失败
BOOL CXThread::SetPriority(int Priority)
{
	// ASSERT(m_hThread);
	m_iPriority = Priority;

	if (m_hThread != NULL)
		return ::SetThreadPriority(m_hThread, Priority);
	else
		return FALSE;
}

///////////////////////////////////////////////////////////
//	GetExitCode
//		获取线程退出代码
//	输入：	无
//	输出:	无
//  返回:	int
DWORD CXThread::GetExitCode()
{
	// ASSERT(m_hThread);
	DWORD dwExitCode = 0;
	if (m_hThread != NULL && ::GetExitCodeThread(m_hThread, &dwExitCode))
		return dwExitCode;
	else
		return WAIT_FAILED;
}

///////////////////////////////////////////////////////////
//	IsRunning
//		判断线程是否还在运行
//	输入：	无
//	输出:	无
//  返回:	int
BOOL CXThread::IsRunning()
{
	return (GetExitCode() == STILL_ACTIVE);
}

///////////////////////////////////////////////////////////
//	_ThreadFunc
//		Friend of Thread class
//		Actual thread around which the OO wrapper is built.
//		Do not call twice on the same object.
//		do something (initializing and finalizing) in ThreadEntry and ThreadExit functions.
//	输入：	pvThread	     			 	
//	输出:	无
//  返回:	DWORD
DWORD WINAPI CXThread::_ThreadFunc(LPVOID pvThread)
{
 	CXThread* pThread = (CXThread*)pvThread;  //typecast

 	if (pThread->InitInstance() == FALSE)
		return 0xFFFFFFFF; // initialize failed

 	DWORD dwRet = pThread->Run();

 	pThread->ExitInstance(); //finalize
	pThread->m_bActive = FALSE;

	return dwRet;
}

