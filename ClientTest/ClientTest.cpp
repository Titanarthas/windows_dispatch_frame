// ClientTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../wdf/socket.h"
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	SocketClient s("192.168.1.35", 57789);

	s.SendLine("GET / HTTP/1.0");
	s.SendLine("Host: www.google.com");
	s.SendLine("");

	while (1) 
	{
		string l = s.ReceiveLine();

		cout << l << endl;
	}

	return 0;
}

