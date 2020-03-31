// loxpp.cpp: 定义应用程序的入口点。
//

#include "loxpp.h"

using namespace std;

int main()
{
	cout << "Hello CMake." << endl;
	const token t{token_type::IDENTIFIER, "tmp", 1, 1, };
	cout << t << endl;
	return 0;
}
