#include"minisql.hpp"
int main() {
	DBshell shell;
	while (true) {
		shell.read_command();
	}
	system("pause>0");
	return 0;
}