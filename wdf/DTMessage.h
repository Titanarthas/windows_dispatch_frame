#pragma once
class CDTMessage
{
public:
	CDTMessage(void);
	~CDTMessage(void);
};


class CRegistThreadID
{
public:
	CRegistThreadID();
	CRegistThreadID(UINT nThreadType, UINT nThreadID);
	~CRegistThreadID();
	UINT m_nThreadType;
	UINT m_nThreadID;
};


class CUnRegistThreadID
{
public:
	CUnRegistThreadID();
	CUnRegistThreadID(UINT nThreadType, UINT nThreadID);
	~CUnRegistThreadID();
	UINT m_nThreadType;
	UINT m_nThreadID;
};
