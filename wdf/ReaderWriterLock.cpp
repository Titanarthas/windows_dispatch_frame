#include "stdafx.h"
#include "ReaderWriterLock.h"


CReaderWriterLock::CReaderWriterLock(void)
{
	m_Readers = 0; 
	InitializeCriticalSection(&m_Writer); 
	InitializeCriticalSection(&m_ReaderCount); 
	m_ClearReadersEvent = CreateEvent(NULL,TRUE,TRUE,NULL); 
}


CReaderWriterLock::~CReaderWriterLock(void)
{
	WaitForSingleObject(m_ClearReadersEvent,INFINITE); 
	CloseHandle(m_ClearReadersEvent); 
	DeleteCriticalSection(&m_Writer); 
	DeleteCriticalSection(&m_ReaderCount); 
}
