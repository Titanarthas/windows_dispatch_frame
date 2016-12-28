#pragma once
class CReaderWriterLock
{
public:
	CReaderWriterLock(void);
	~CReaderWriterLock(void);


	/*Read, reset events */ 
	void EnterReader(void) 
	{ 
		EnterCriticalSection(&m_Writer); 
		EnterCriticalSection(&m_ReaderCount); 
		if (++m_Readers == 1) 
		{ 
			::ResetEvent(m_ClearReadersEvent); 
		} 
		LeaveCriticalSection(&m_ReaderCount); 
		LeaveCriticalSection(&m_Writer); 
	}

	/*leave read, triggered events */ 
	void LeaveReader(void) 
	{ 
		EnterCriticalSection(&m_ReaderCount); 
		if (--m_Readers == 0) 
		{ 
			::SetEvent(m_ClearReadersEvent); 
		} 
		LeaveCriticalSection(&m_ReaderCount); 
	}

	/*Only after reading, to be able to write*/ 
	void EnterWriter(void) 
	{ 
		EnterCriticalSection(&m_Writer); 
		WaitForSingleObject(m_ClearReadersEvent,INFINITE); 
	}

	void LeaveWriter(void) 
	{ 
		LeaveCriticalSection(&m_Writer); 
	}

private: 
	CRITICAL_SECTION m_Writer; 
	CRITICAL_SECTION m_ReaderCount; 
	int m_Readers; 
	HANDLE m_ClearReadersEvent;
};

