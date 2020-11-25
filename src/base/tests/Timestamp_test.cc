#include <iostream>
#include <string>

#include "src/base/Timestamp.h"

using namespace std;

int main(int argc, char* argv[])
{
	Timestamp ts = Timestamp::now();
	cout << ts.toString() << endl;
	cout << ts.toFormattedString() << endl;
	cout << ts.toFormattedString(false) << endl;
	return 0;
}
