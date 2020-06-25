#pragma once
#include <iostream>		
#include <fstream>
#include <algorithm>
#include "Datatypes.h"
#include "BinaryTree.hpp"
#include <iomanip>
#define _OrderCrit_ up_Down
#define _OrderIndex_ header_index
enum Cr { ASC, DESC };
int header_index = 0;
Cr up_Down = DESC;

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
	Data(const string& type) { //string title
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
	string get_description() { return value->description; }
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

};
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
		cout << "Initalizing: \nIndex " << _OrderIndex_ << "\nMode: " << ((_OrderCrit_ == ASC) ? "Asc\n" : "Desc\n"); 
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
	void add_row(string line) {
		size_t pos;
		while ((pos = line.find('-')) != string::npos) {
			line.replace(pos, 1, ",");
		}
		cout << line; cin.get();
		stringstream aux(line);
		vector<Data*> row;
		int current_col = 0;
		while (getline(aux, line, ',')) {
			row.push_back(new Data(line)); //line must be example(string hello,string world,int 2010
			paddings[current_col] = line.length() > paddings[current_col] ? line.length() : paddings[current_col];
			current_col++;
		}
		data.insert(row, [](const vector<Data*>& a1, const vector<Data*>& a2) {
			if (_OrderCrit_ == DESC) {
				return *a1[_OrderIndex_] >= *a2[_OrderIndex_];
			}
			else {
				return *a1[_OrderIndex_] <= *a2[_OrderIndex_];
			}
		});
	}
	void now_order_by(string&& prompt) {
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
		string word; int current_col;
		while (getline(file, row)) {
			line.clear();
			line.reserve(header.size());
			current_col = 0;
			aux = stringstream(row);
			while (getline(aux, word, ',')) {
				line.push_back(new Data(datatypes[current_col] + " " + word));
				paddings[current_col] = word.length() > paddings[current_col] ? word.length() : paddings[current_col];
				current_col++;
			}
			data.insert(line, [](const vector<Data*>& a1, const vector<Data*>& a2)->bool {
				if (_OrderCrit_ == DESC) {
					return *a1[_OrderIndex_] >= *a2[_OrderIndex_];
				}
				else {
					return *a1[_OrderIndex_] <= *a2[_OrderIndex_];
				}
			});
		}
	}
	void _display_horizontals() {
		cout << '+';
		for (int i = 0; i < header.size(); i++) {
			cout << string((1 + paddings[i]), '-') << "+";
		}
		cout << char(8) << "+\n";
	}
	void _add_row(const vector<Data*>& row) {
		int current_col = 0;
		for (Data* word : row) {
			paddings[current_col] = word->get_description().length() > paddings[current_col] ? word->get_description().length() : paddings[current_col];
			current_col++;
		}
		data.insert(row, [](const vector<Data*>& a1, const vector<Data*>& a2) {
			if (_OrderCrit_ == DESC) {
				return *a1[_OrderIndex_] >= *a2[_OrderIndex_];
			}
			else {
				return *a1[_OrderIndex_] <= *a2[_OrderIndex_];
			}
		});
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
