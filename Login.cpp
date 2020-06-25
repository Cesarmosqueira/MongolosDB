#include"minisql.hpp"
bool insert_int(const int& a, const int& b) {
	return a > b;
}
int main() {
	DBshell shell;
	while (true) {
		shell.read_command();
	}
	system("pause>0");

	return 0;
}