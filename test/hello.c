#include <iostream>
#include <string>
using namespace std;

int main(int argc, char const *argv[])
{
	string to_whom(" World");
	if (argc > 1) {
		to_whom = "";
		for (int i = 1; i != argc; i++) {
			string tmp_str;
			tmp_str += " ";
			tmp_str += argv[i];
			to_whom += tmp_str;
		}
	}

	cout << "Hello" << to_whom << endl;
	return 0;
}