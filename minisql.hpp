#pragma once
#if __has_include(<conio.h>)
#  include <conio.h>	
#endif
#include "Table_Manager.hpp"
#include <list>


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
	~Accounts_Manager() {
		//delete displayer;
	}
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
	bool loged;
	Accounts_Manager accounts;
public:
	DBshell() : user(""), name("MongolosDB"), loged (false) {
		cout << name + " ZQL Fierbeis [Version 0.0.00.0000.001]\n";
		cout << "(c) 2020 " + name + " Corporation. All rights reserved.\n";
	}
	void read_command() {
		input = "";
		get_command();
		if (ends_with(input, "-p")) {
			login(input.substr(0, input.length() - 3));
		}
		else if (starts_with(input, "useradd")) {
			user_add(input.substr(8, input.length()));
		}
		else if (starts_with(input, "use table")) {
			///
			//search 
			input.erase(0, 10); //input = tablename		
			
			

		} //main part
		else if (starts_with(input, "display table")) {
			input.erase(0, 14); //input = tablename		
			//only table available yet														
			//if(input == "acctable")	accounts.display_users();								
			// assuming tablemanager(user) is up to date									
			// table_manager[input] displayer												
			// if not able to display, show msg	
			if (input.substr(0, input.find(" ")).empty()) cout << "You have to provide a table name\n";
			else cout << "There is no '" << input << "' table that you are allowed to use\n";	
		}
		else if (starts_with(input, "clear")) system("cls");
		else {
			cout << "'" << input.substr(0, input.find(" ")) << "' is not recognized as a valid command\n";
		}
		input = "";
	}
private:
	string secret_input() {
		char a;
		string password;
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
		return password;
	}
	void get_command(string prompt = "", bool show_only = false) {
		if (!loged) current = name + " > " + prompt;
		else current = name + "\\" + user + " > " + prompt;
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
};
