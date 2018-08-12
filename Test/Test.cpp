#include <iostream>
using namespace std;
#include <IexBaseExc.h>

using namespace Iex;

extern "C" int getInteger()
{
	return 7;
}

/*
int main()
{
	std::cout << getInteger() << std::endl;
	return getInteger();
}
*/
