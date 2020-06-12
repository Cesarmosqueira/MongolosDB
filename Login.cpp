#include"minisql.hpp"

int main() {
	//Display example
	DBshell shell;
	while (true) {
		shell.read_command();
	}
	system("pause>0");
	return 0;
}