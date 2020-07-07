#pragma once
#include <conio.h>	
#include "Table_Manager.hpp"

struct Accounts_Manager {
	vector<int> tables_per_user;
	hash<string> hash;	
	list<pair<string,size_t>> users;
	ofstream file;
	Accounts_Manager() {
		pair<string, size_t> aux;
		string u;
		stringstream parse;
		ifstream input("usernames.txt", ios::in);
		while (getline(input, u)) {
			aux.first = u.substr(0, u.find('|'));
			u = u.substr(u.find('|')); u.erase(0, 1);
			parse = stringstream(u);
			parse >> aux.second;
			users.push_back(aux);
		}
	}	
	~Accounts_Manager() {}
	bool valid_password(const string& user, const string& password) { 
		for (auto& x : users) {
			if (x.first == user) {
				return(x.second == hash(password));
			}
		}
		return false;
	}
	bool valid_user(const string& user) {
		for (auto& x : users) {
			if (x.first == user) {
				return true;
			}
		}
		return false;
	}
	void add_user(const string& username, const string& password) {
		if (valid_user(username)) {
			cout << "User '" <<username << "' is already on the system" << endl;
			return;
		}
		users.push_back({ username, hash(password) });
		tables_per_user.push_back(0);
		system(("mkdir Tables\\" + username).c_str());
		file.open("usernames.txt", ios_base::app);
		file << username << "|" << hash(password) << "\n";
		file.close();
	}
};
class DBshell {
	string current, user, input, name;
	bool loged, used_flag;
	Accounts_Manager accounts;
	Table* current_table;
public:
	DBshell() : user(""), name("MongolosDB"), loged (false) {
		cout << name + " ZQL Fierbeis [Version 0.8.40.0000.000] Powered by AVL Trees\n";
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
			cout << " tableadd <tablename> <datatype headername> -   " << std::setw(20) << " Creates an emtpy table\n";
			cout << " useradd <username>  " << std::setw(20) << " register uwu uwu\n";
			cout << " use table <tablename>" << std::setw(20) <<" when loged in, to use a table you are able to use\n";
			cout << " display table <tablename>" << std::setw(20) <<" to display a table without using it\n";
			cout << " drop table  " << std::setw(20) << " stops using a table\n";
			cout << " clear  " << std::setw(20) << " clears the console\n";
			cout << " dir " << std::setw(20) << " shows directory where csv are stored \n";
			cout << " \n";
			cout << " When Using a Table \n";
			cout << " display  " << std::setw(20) << " displays the table ure using\n";
			cout << " tree display" << std::setw(20) << " Displays current table as a tree\n";
			cout << " rowadd <data-data-data> " << std::setw(20) << " Add a row to current table\n";
			cout << " order by <headername> <ASC/DESC>" << std::setw(20) << " Changes de order index to the headername provided\n";
			cout << "rm this " << std::setw(20) << " Deletes current table in use\n";
			cout << "columnadd <Datatype> <Headername>" << std::setw(20) << " Adds a new column starting in 0\n";
			cout << "rowadd " << std::setw(20) << " adds data splited by a '-' \n";
			cout << "rowedit <nRow> | <nCol> <headername>-<nCol> <headername> " << std::setw(20) << " edit chosen row and columns \n";
		}
		else if (starts_with(input, "useradd")) {
			current_table = nullptr;
			user_add(input.substr(8, input.length()));
		}
		else if (starts_with(input, "dir") || starts_with(input, "DIR")) {
			if (loged) {
				system(("dir Tables\\" + user).c_str());
			}
			else {
				system("dir Tables");
			}
		}
		else if (starts_with(input, "tableadd")) { //string name-string last name 
			current_table = nullptr;
			if (loged) {
				if (input.substr(8) == "") cout << "May wanna use tableadd <tablename> | <headername>-<headername> instead";
				else {
					current_table = new Table(input.substr(9), user, Useless::EMPTY);
					ofstream f("Tables\\" + user + "\\" + name + ".csv");
					f.close();
				}
			}
			else cout << "Must log in to add a table\n";
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
			if (!used_flag && input.length() > 0) {
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
		else if (starts_with(input, "rm this")) {
			char ans;
			cout << "Are you sure sure sure 100% sure???? [Y/N]"; cin >> ans;
			toupper(ans);
			if (ans == 'Y') {
				system(("del Tables\\" + user + "\\" + current_table->name + ".csv").c_str());
				cout << "Deleted perhaps\n";
			}
			else {
				cout << "Cancelled succesfully\n";
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
		else if (starts_with(input, "rowadd")) {
			if (input.substr(6).length() > 1) {
				input = input.substr(7);
				current_table->add_row(input);
			}
			used_flag = true;
		}
		else if (starts_with(input, "columnadd")) {
			if (input.substr(9).length() > 1) {
				input = input.substr(10);
				current_table->add_column(input);
			}
			else {
				cout << "Trying to use 'columnadd <header name>,<header name>'?\n";
			}
			used_flag = true;
		}
		else if (starts_with(input, "rowedit")) {
			if (input.substr(7).length() > 1) { //rowedit n | ncol <val>-ncol <val> 
				input = input.substr(8);
				current_table->edit_row(input);
			}
			else {
				cout << "Trying to use 'columnadd <header name>,<header name>'?\n";
			}
			used_flag = true;
		}
		else if (starts_with(input, "columnrmv")) {
			if (input.substr(9).length() > 1) {
				input = input.substr(10);
				cout << input; cin.get();
				current_table->remove_header(input);
			}
			else cout << "Trying to use 'columnrmv <header name>'?\n";
			used_flag = true;
		}
	}
};
