#include "Jug.h"

void test1()
{
string solution;
Jug head(3, 5, 4, 1, 2, 3, 4, 5, 6);
if (head.solve(solution) != 1) {
	cout << "Error 3" << endl;
}
cout << solution << endl << endl;
}

void test2()
{
	string solution;
	Jug head(3, 5, 4, 1, 1, 1, 1, 1, 2);
	if (head.solve(solution) != 1) {
		cout << "Error 3" << endl;
	}
	cout << solution << endl;
}

int main()
{
	test1();

	test2();

	return 0;
}