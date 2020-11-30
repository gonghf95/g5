#include "src/net/AtomicInteger.h"

#include <iostream>
#include <assert.h>

using namespace std;
using namespace net;

int main(int argc, char* argv[])
{
	{
		AtomicInt64 a0;
		assert(a0.get() == 0);
	}
	return 0;
}
