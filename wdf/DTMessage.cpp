#include "stdafx.h"
#include "DTMessage.h"


CDTMessage::CDTMessage(void)
{
}


CDTMessage::~CDTMessage(void)
{
}

CRegistThreadID::CRegistThreadID()
	: m_nThreadType(0)
	, m_nThreadID(0)
{

}

CRegistThreadID::CRegistThreadID( UINT nThreadType, UINT nThreadID )
	: m_nThreadType(nThreadType)
	, m_nThreadID(nThreadID)
{

}

CRegistThreadID::~CRegistThreadID()
{

}

CUnRegistThreadID::CUnRegistThreadID()
	: m_nThreadType(0)
	, m_nThreadID(0)
{

}

CUnRegistThreadID::CUnRegistThreadID( UINT nThreadType, UINT nThreadID )
	: m_nThreadType(nThreadType)
	, m_nThreadID(nThreadID)
{

}

CUnRegistThreadID::~CUnRegistThreadID()
{

}
