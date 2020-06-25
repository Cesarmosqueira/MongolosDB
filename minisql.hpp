#pragma once
#include <conio.h>	
#include "Table_Manager.hpp"

struct Accounts_Manager {
	vector<int> tables_per_user;
	int users_num;
	hash<string> hash;	
	list<pair<string,size_t>> users;
	Accounts_Manager() {
		//imagine file is just 1 line  [root][root]
		users_num = 2; //get from eof	
		tables_per_user.reserve(users_num);
		//input 
		users.push_back({ "root", hash("password") });	   //WORK WITH TEXT FILES
		tables_per_user.push_back(0);
		users.push_back({ "ASDSD" , hash("admin") });    //WORK WITH TEXT FILES
		tables_per_user.push_back(0);
	}
	~Accounts_Manager() {}
	bool valid_password(string user, string password) { 
		for (auto& x : users) {
			if (x.first == user) {
				return(x.second == hash(password));
			}
		}
		return false;
	}
	bool valid_user(string user) {
		for (auto& x : users) {
			if (x.first == user) {
				return true;
			}
		}
		return false;
	}
	void add_user(string username, string password) {
		users.push_back({ username, hash(password) });
		users_num++;
		tables_per_user.push_back(0);
	}
};
class DBshell {
	string current, user, input, name;
	bool loged, used_flag;
	Accounts_Manager accounts;
	Table* current_table;
public:
	DBshell() : user(""), name("MongolosDB"), loged (false) {
		cout << name + " ZQL Fierbeis [Version 0.0.00.0000.001] Powered by AVL Trees\n";
		cout << "(c) 2020 " + name + " Corporation. All rights reserved.\n";
		current_table = nullptr;
		used_flag = false;
	}
	void read_command() {
		input = "";
		get_command();
		if(current_table) use_table_commands();
		if (ends_with(input, "-p")) {
			current_table = nullptr;
			login(input.substr(0, input.length() - 3));
		}
		else if (starts_with(input, "help")) {
			cout << " <username> -p  " << std::setw(20) << " Login\n";
			cout << " tableadd <table>  " << std::setw(20) << " No funciona aun xdxdxd osi osi\n";
			cout << " useradd <username>  " << std::setw(20) << " register uwu uwu\n";
			cout << " use table <tablename>" << std::setw(20) <<" when loged in, to use a table you are able to use\n";
			cout << " display table <tablename>" << std::setw(20) <<" to display a table without using it\n";
			cout << " drop table  " << std::setw(20) << " stops using a table\n";
			cout << " clear  " << std::setw(20) << " clears the console\n";
			cout << " \n";
			cout << " When Using a Table \n";
			cout << " display  " << std::setw(20) << " displays the table ure using\n";
			cout << " tree display" << std::setw(20) << " Displays current table as a tree\n";
			cout << " order by <headername> <ASC/DESC>" << std::setw(20) << " Changes de order index to the headername provided\n";
		}
		else if (starts_with(input, "useradd")) {
			current_table = nullptr;
			user_add(input.substr(8, input.length()));
		}
		else if (starts_with(input, "tableadd")) {
			current_table = nullptr;
		}
		else if (starts_with(input, "use table")) {
			current_table = nullptr;
			if (loged) {
				if (input.substr(10) == "") cout << "Enter a table name to use";
				else {
					current_table = new Table(user, input.substr(10));
					if (current_table->failed) {
						current_table = nullptr;
						cout << "Table '" << input.substr(10) << "' is not available for '" << user << "'\n";
					}
				}
			}
			else {
				cout << "You have to login in order to acces a table.\n";
			}
		}
		else if (starts_with(input, "display table")) {
			if (loged) {
				if (input.substr(13) == "") cout << "Enter a table name to display\n";
				else {
					current_table = new Table(user, input.substr(14));
					if (current_table->failed) {
						current_table = nullptr;
						cout << "Table '" << input.substr(14) << "' is not available for '" << user << "'\n";
					}
					else {
						current_table->display();
						current_table = nullptr;
					}
				}
			}
			else {
				cout << "You have to login in order to acces a table.\n";
			}
		}
		else if (starts_with(input, "drop table")) {
			current_table = nullptr;
		}
		else if (starts_with(input, "clear")) {
			current_table = nullptr;
			system("cls");
		}
		else {
			if (!used_flag) {
				cout << "'" << input.substr(0, input.find(" ")) << "' is not recognized as a valid command\n";
			}
		}
		used_flag = false;
		input = "";
	}
private:
	string secret_input() {
		char a;
		string password;
		System::Console::CursorVisible = false;
		while (true) {
			a = _getch();
			if (a == 13) break;
			else if (a == 8) {
				if (!password.empty()) {
					cout << char(8);
					password.pop_back();
				}
			}
			else {
				if (a >= 32) {
					password += a;
				}
			}
		}
		cout << "\n";
		System::Console::CursorVisible = true;
		return password;
	}
	void get_command(string prompt = "", bool show_only = false) {
		if (!loged) current = name + " > " + prompt;
		else current = name + "\\" + user + ((current_table ? "\\" + (current_table->name) : "" )) +  " > " + prompt;
		cout << "\n" << current;
		if (!show_only) getline(cin, input);
	}
	void login(string const& new_user) {	
		if(loged) {
			get_command("logging off " + name + ", sure? [Y/N]: ");
			loged = (starts_with(input, "n"));
			if(new_user.length() > 3) login(new_user);
		}
		else {
			if (accounts.valid_user(new_user)) {
				//get pass char by char 
				get_command("[Password]: ", true); //read only
				//get password
				loged = accounts.valid_password(new_user, secret_input());
				if (!loged) cout << "Incorrect password\n";
				user = loged ? new_user : "";
			}
			else {
				cout << "There is no " << new_user << " on the user list\n";
				cout << "You can check the user list using the table: <acctable> command\n";
			}
		}
		user = loged ? user : "";
	}
	void user_add(string const& new_user) {
		cout << "\nEnter new password for '" << new_user << "': ";
		string password = secret_input();
		cout << "\nRe-enter new password for '" << new_user << "': ";
		if (password == secret_input()) {
			accounts.add_user(new_user, password);
		}
		else cout << "Password does not match\n";
	}
	void use_table_commands() {
		if (starts_with(input, "display")) {
			if (current_table) {
				current_table->display();
				used_flag = true;
			}
			else {
				cout << "You have to be using a table to use display\n";
			}
		}
		else if (starts_with(input, "save")) {
			if (current_table) {
				current_table->save_table();
				used_flag = true;
			}
			else {
				cout << "You have to be using a table to save it\n";
			}
		}
		else if (starts_with(input, "use table")) {
			if (loged) {
				if (input.substr(10) == "") cout << "Enter a table name to use";
				else {
					current_table = new Table(user, input.substr(14));
					if (current_table->failed) {
						current_table = nullptr;
						cout << "Table '" << input.substr(14) << "' is not available for '" << user << "'\n";
					}
				}
			}
			else {
				cout << "You have to login in order to acces a table.";
			}
			used_flag = true;
		}
		else if (starts_with(input, "order by")) {
			if (input.substr(8) != "") { //order by "<header name>" ASC/DESC
				current_table->now_order_by(input.substr(9));
				if (current_table->wrong_command) {
					cout << "Trying to use 'order by <header name> ASC/DESC'?\n";
					current_table->wrong_command = false;
				}
				else {
					current_table->save_table();
					current_table = new Table(user, current_table->name);
				}
			}
			else cout << "Trying to use 'order by <header name> ASC/DESC'?\n";
			used_flag = true;
		}
		else if (starts_with(input, "tree display")) {
			current_table->tree_display();
			used_flag = true;
		}
		else if (starts_with(input, "add row")) {
			if (input.substr(7).length() > 1) {
				input = input.substr(8);
				current_table->add_row(input);
			}
			used_flag = true;
		}
	}
};
