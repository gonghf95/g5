#include "src/net/tcpserver.h"

#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	TcpServer s;
	s.start();
	return 0;
}
