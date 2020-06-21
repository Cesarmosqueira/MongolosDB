#pragma once
#include <iostream>		
#include <fstream>
#include "Datatypes.h"
#include <vector>
#include <iomanip>

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
	string str() {
		return value->string_it();
	}
	string get_description() { return value->description; }
	string header_format() {
		return value->type + " " + value->description;
	}
};

class Table {
	vector<Data*> header;
	vector<vector<Data*>> data;
	vector<string> datatypes;
	vector<int> paddings;
	stringstream* aux;
	string row, user;
	//cual estamos libres oe 
public:
	string name;
	bool failed;
	Table(const string& user, const string& tablename) {
		this->user = user; name = tablename;
		ifstream file("Tables\\" + user + "\\" + tablename + ".csv");
		failed = file.fail();
		if (!failed) {
			if (getline(file, row)) {
				failed = false;
				_distribute_datatypes(row);
				_fill_matrix(file);
			}
			failed = header.size() < 1 && data.size() < 1;
		}
		else failed = true;
	}
	~Table() {
		delete aux;
		for (auto p : header) delete p;
		for (auto p : data) {
			for (auto j : p) delete j;
		}
	}
	void display() {
		//headers
		int aux = 0;
		cout << "\n ";
		_display_horizontals();
		cout << " |";
		for (Data *x : header) { cout << std::setw(paddings[aux]) << x->get_description() << " |"; aux++; }
		cout << "\n ";
		_display_horizontals();
		for (vector<Data*> row : data) {
			cout << " |";
			for (int i = 0; i < row.size(); i++) {
				cout << std::setw(paddings[i]) << row.at(i)->str() << " |";
			}
			cout << "\n";
		}
		cout << char(32);
		_display_horizontals();
	}
	void save_table() {
		ofstream file("Tables\\" + user + "\\" + name + ".csv", ios::out | ios::trunc);
		string out = "";
		for (auto v : header) {
			out += v->header_format() + ",";
		}
		out += "\n";
		file << out;
		for (auto row : data) {
			file << line_to_csv(row);
		}

		file.close();
	}
private:
	void _distribute_datatypes(std::string& word) {
		aux = new stringstream(word);
		while (getline(*aux, word, ',')) {
			header.push_back(new Data(word));
			datatypes.push_back(word.substr(0, word.find(' ')));
			paddings.push_back(header.back()->str().length());
		}
	}
	void _fill_matrix(ifstream& file) {
		string word; int current_col, current_row = 0;
		while (getline(file, row)) {
			current_col = 0;
			aux = new stringstream(row);
			data.push_back(vector<Data*>());
			while (getline(*aux, word, ',')) {
				data.at(current_row).push_back(new Data(datatypes[current_col] + " " + word));
				paddings[current_col] = word.length() > paddings[current_col] ? word.length() : paddings[current_col];
				current_col++;
			}
			current_row++;
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
		out += "\n";
		return out;
	}
};
