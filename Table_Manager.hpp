#pragma once
#include <iostream>		
#include <fstream>
#include <algorithm>
#include "Datatypes.h"
#include "BinaryTree.hpp"
#include <iomanip>
#define _OrderCrit_ up_Down
#define _OrderIndex_ header_index
#define _DATA_TO_ADD_ row_to_add
#define _REMOVAL_INDEX_ removal_index
enum Cr { ASC, DESC };
namespace Useless { enum Constructor { EMPTY }; }
int header_index = 0;
int removal_index = 0;
Cr up_Down = DESC;
string row_to_add;
//also shell aids
bool starts_with(string const& source, string const& prefix) {
	return prefix == source.substr(0, prefix.length());
}
bool ends_with(string const& source, string const& ending) {
	if (ending.size() > source.size()) return false;
	return equal(ending.rbegin(), ending.rend(), source.rbegin());
}
struct Data {
	Mongo_Types* value;
	Data(const string& type) { //input = "int 70"
		if (starts_with(type, "int")) {
			value = new MG_INT(type.substr(4));
		}
		else if (starts_with(type, "string")) {
			value = new MG_STRING(type.substr(7));
		}
		else if (starts_with(type, "float")) {
			value = new MG_FLOAT(type.substr(6));
		}
	}
	~Data() { delete value; }
	string get_type() { return value->type; } //int
	string get_description() { return value->description; }

	// "string abcde fgs" || "int 201321" || "float 9232.23f"
	string header_format() { 
		return value->type + " " + value->description;
	}
	bool operator<(const Data& s) {
		return *value < *s.value;
	}
	bool operator>(const Data& s) {
		return *value > *s.value;
	}
	bool operator<=(const Data& s) {
		return *value <= *s.value;
	}
	bool operator>=(const Data& s) {
		return *value >= *s.value;
	}
	bool operator == (const Data& s) {
		return *value == *s.value;
	}
};
bool found_row(const vector<Data*>& src, const vector<Data*>& comp) {
	for (int i = 0; i < src.size(); i++) {
		if (!(*src.at(i) == *comp.at(i))) return false;
	}
	return true;
}
class Table {
	vector<Data*> header;
	Binary_Tree<vector<Data*>> data;
	list<vector<Data*>> linear;
	vector<string> datatypes; 
	vector<int> paddings;
	stringstream aux;
	string row, user;
	Cr up_down;
	int col_index;
public:
	string name;
	bool wrong_command;
	bool failed;
	Table(const string& user, const string& tablename) {
		this->user = user; name = tablename;
		wrong_command = false;
		this->col_index = col_index;
		this->up_down = up_down;
		ifstream file("Tables\\" + user + "\\" + tablename + ".csv");
		failed = file.fail();
		if (!failed) {
			if (getline(file, row)) {
				failed = false;
				_distribute_datatypes(row);
				_fill_matrix(file);
			}
			failed = header.size() < 1 && data.get_size() < 1;
		}
		else failed = true;
	}
	Table(const string& prompt, const string& user, const Useless::Constructor& none) {
		this->user = user;
		name = (prompt.substr(0, prompt.find('|')));
		if (name.back() == ' ') name.pop_back();
		string h = prompt.substr(prompt.find('|') + 1);
		size_t pos;
		while (true) {
			if(h[0] == ' ') h.erase(0, 1);
			else break;
		}
		while ((pos = h.find('-')) != string::npos) {
			h.replace(pos, 1, ",");
		}
		_distribute_datatypes(h);
	}
	~Table() {
		for (Data* p : header) delete p;
		data.for_each([](vector<Data*> row) {for (auto x : row) delete x; });
		list<vector<Data*>> linear;
		for (vector<Data*> x: linear) { for (Data* v : x) delete v; }
	}
	void display() {
		data.load_linear();
		linear = data.get_linear();
		//headers
		int aux = 0;
		cout << "\n ";
		_display_horizontals();
		cout << " |";
		for (Data *x : header) { cout << std::setw(paddings[aux]) << x->get_description() << " |"; aux++; }
		cout << "\n ";
		_display_horizontals();
		for (vector<Data*> row : linear) {
			cout << " |";
			for (int i = 0; i < row.size(); i++) {
				cout << std::setw(paddings[i]) << row.at(i)->get_description() << " |";
			}
			cout << "\n";
		}
		cout << char(32);
		_display_horizontals();
	}
	void save_table() {
		data.load_linear(Binary_Criteria::IDR);
		linear = data.get_linear();
		ofstream file("Tables\\" + user + "\\" + name + ".csv", ios::out | ios::trunc);
		string out = "";
		for (auto v : header) {
			out += v->header_format() + ",";
		}
		out.pop_back();
		file << out << "\n";
		data.load_linear();
		linear = data.get_linear();
		for (auto row : linear) {
			file << line_to_csv(row) << "\n";
		}
		file.close();
	}
	void add_column(string line) {
		//columnadd <header name>,<header name>
		aux = stringstream(line);
		while (getline(aux, line, '-')) {
			header.push_back(new Data(line));
			datatypes.push_back(line.substr(0, line.find(' ')));
			paddings.push_back(header.back()->get_description().length());
			if (datatypes.back() == "string") {
				_DATA_TO_ADD_ = "string .";
			}
			else if(datatypes.back() == "int") {
				_DATA_TO_ADD_ = "int 0";
			}
			else if(datatypes.back() == "float") {
			_DATA_TO_ADD_ = "float 0";
			}
			data.for_each([](vector<Data*>& row) {
				row.push_back(new Data(_DATA_TO_ADD_));
			});
		}
	}
	void edit_row(const string& prompt) {
		ifstream file("Tables\\" + user + "\\" + name + ".csv");
		vector<Data*> to_add;
		vector<Data*> to_replace;
		string row;
		string command = prompt.substr(prompt.find('|') + 1);
		while (command[0] == ' ')command.erase(0, 1);
		int row_num = stoi(prompt.substr(0, prompt.find('|')));
		if (row_num < 0||row_num >= data.get_size()) { cout << "Wrong data provided\n"; return; }
		getline(file, string(""));
		int count = 0;
		while (getline(file, row)) {
			if (count == row_num) {
				aux = stringstream(row);
				count = 0;
				while (getline(aux, row, ',')) {
					to_replace.push_back(new Data(header[count]->get_type() + " " + row));
					to_add.push_back(new Data(header[count]->get_type() + " " + row));
					count++;
				}
				break;
;			}
			count++;
		}
		aux = stringstream(command);
		int current_col;
		while (getline(aux, command, '-')) {
			current_col = stoi(command.substr(0, command.find(' ')));
			if(current_col >= header.size()) { cout << "Wrong data provided\n"; return; }
			command = command.substr(command.find(' '));
			while (command[0] == ' ')command.erase(0, 1);
			to_add[current_col] = new Data(header[current_col]->get_type() + " " + command);
		}
		data.replace(to_replace, to_add, found_row);
	}
	void add_row(string line) {
 		stringstream aux(line);
		vector<Data*> row;
		int current_col = 0;
		while (getline(aux, line, '-')) {
			if (current_col < header.size()) {
				row.push_back(new Data(header.at(current_col)->get_type() + " " + line)); 
				paddings[current_col] = line.length() > paddings[current_col] ? line.length() : paddings[current_col];
			}
			else { 
				cout << "Given row doesn't fit '" << name << "' characteristics\n";
				return;
			}
			current_col++;
		}
		if (row.size() != header.size()) {
			cout << "Given row doesn't fit '" << name << "' characteristics\n";
			return;
		}
		if (row.size() == header.size()) {
			data.insert(row, [](const vector<Data*>& a1, const vector<Data*>& a2) {
				if (_OrderCrit_ == DESC) {
					return *a1[_OrderIndex_] >= *a2[_OrderIndex_];
				}
				else {
					return *a1[_OrderIndex_] <= *a2[_OrderIndex_];
				}
			});
		}
	}
	void now_order_by(string&& prompt) { // order by edad ASC
		if (ends_with(prompt, "ASC")) {
			prompt.erase(prompt.length() - 4, 4);
			int i = 0;
			for (auto x : header) {
				if (x->get_description() == prompt) {
					_OrderIndex_ = i;
					_OrderCrit_ = ASC;
					wrong_command = false;
					return;
				}
				i++;
			}
			wrong_command = true;
		}
		else if (ends_with(prompt, "DESC")) {
			prompt.erase(prompt.length() - 5, 5);
			int i = 0;
			for (auto x : header) {
				if (x->get_description() == prompt) {
					_OrderIndex_ = i;
					_OrderCrit_ = DESC;
					wrong_command = false;
					return;
				}
				i++;
			}
			wrong_command = true;
		}
		else wrong_command = true;
	}
	void tree_display() {
		print_tree_form([](vector<Data*>& v) {
			cout << v.at(_OrderIndex_)->get_description();
		});
	}
	void remove_header(const string& headername) {
		int pos = 0;
		for (auto x : header) {
			if (x->get_description() == headername) {
				_REMOVAL_INDEX_ = pos;
				data.for_each([](vector<Data*>& v) {
					v.erase(v.begin() + _REMOVAL_INDEX_);
				});
				header.erase(header.begin() + pos);
				return;
			}
			pos++;
		}
	}
private:
	void print_tree_form(const function<void(vector<Data*>&)>& print_crit) {
		data.print2D(print_crit);
		cout << "\n";
	}
	void _distribute_datatypes(std::string word) {
		aux = stringstream(word);
		while (getline(aux, word, ',')) {
			header.push_back(new Data(word));
			datatypes.push_back(word.substr(0, word.find(' ')));
			paddings.push_back(header.back()->get_description().length());
		}
	}
	void _fill_matrix(ifstream& file) {
		data.clear();
		vector<Data*> line;
		string word; int current_col = 0;
		while (getline(file, row)) {
			line.clear();
			line.reserve(header.size());
			current_col = 0;
			aux = stringstream(row);
			while (getline(aux, word, ',')) {
				line.push_back(new Data(header.at(current_col)->get_type() + " " + word)); //line must be example(string hello,string world,int 2010
				if (current_col < header.size()) paddings[current_col] = word.length() > paddings[current_col] ? word.length() : paddings[current_col];
				else {
					cout << "Given row doesn't fit '" << name << "' characteristics\n";
					return;
				}
				current_col++;
			}
			if (line.size() > 0) {
				if (_OrderCrit_ == DESC) data.insert(line,
					[](const vector<Data*>& a1, const vector<Data*>& a2)->bool {
					return *a1[_OrderIndex_] >= *a2[_OrderIndex_];
				});
				else if (_OrderCrit_ == ASC)data.insert(line,
					[](const vector<Data*>& a1, const vector<Data*>& a2)->bool {
					return *a1[_OrderIndex_] <= *a2[_OrderIndex_];
				});
			}
		}
	}
	void _display_horizontals() {
		cout << '+';
		for (int i = 0; i < header.size(); i++) {
			cout << string((1 + paddings[i]), '-') << "+";
		}
		cout << char(8) << "+\n";
	}
	string line_to_csv(const vector<Data*>& current_row) {
		string out = "";
		for (Data* v : current_row) {
			out += v->get_description() + ",";
		}
		out.pop_back();
		return out;
	}
};
