// ServerTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../wdf/socket.h"
#include "process.h"
#include <iostream>

using namespace std;

unsigned __stdcall Answer(void* a) 
{
	Socket* s = (Socket*) a;

	while (1) 
	{
		std::string r = s->ReceiveLine();
		if (r.empty()) break;
		s->SendLine(r);
	}

	delete s;

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	SocketServer in(2000,5);
	while (1) 
	{
		Socket* s=in.Accept();

		unsigned ret;
		_beginthreadex(0,0,Answer,(void*) s,0,&ret);
	}
	return 0;
}

