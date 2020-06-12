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
		if (starts_with(type, "int")) { //missing substr
			value = new MG_INT(type.substr(4));
			cout << str();  cin.get();
		}
		else if (starts_with(type, "string")) {
			value = new MG_STRING(type.substr(7));
			cout << str();  cin.get();
		}
		else if (starts_with(type, "float")) {
			value = new MG_FLOAT(type.substr(6));
			cout << str();  cin.get();
		}
	}
	~Data() { delete value; }
	string str() {
		return value->string_it();
	}
};

class Table {
	vector<Data> header;
	vector<int> paddings;
	vector<vector<Data>> data;
	bool failed;
	stringstream* aux;
	string row;
public:
	Table(const string& user, const string& tablename) {
		ifstream file("Tables\\" + user + "\\" + tablename + ".csv");
		failed = file.fail();
		if (!failed) {
			if (getline(file, row)) {
				_distribute_datatypes(row);
				_fill_matrix(file);
			}
			failed = true;
		}
	}
	~Table() {
		delete aux;
	}
	void display() {
		//headers
		int aux = 0;
		cout << "\n ";
		_display_horizontals();
		cout << " |";
		for (Data x : header) { cout << std::setw(paddings[aux]) << x.str() << " |"; aux++; }
		cout << "\n ";
		_display_horizontals();
		for (vector<Data> row : data) {
			cout << " |";
			for (int i = 0; i < row.size(); i++) {
				cout << std::setw(paddings[i]) << row[i].str() << " |";
			}
			cout << "\n";
		}
		cout << char(32);
		_display_horizontals();
	}
private:
	void _distribute_datatypes(std::string& word) {
		aux = new stringstream(word);
		while (getline(*aux, word, ',')) {
			cout << word; cin.get();
			header.push_back(Data(word));	
			paddings.push_back(header.back().str().length());
		}
	}
	void _fill_matrix(ifstream& file) {
		string word; int current_col;
		while (getline(file, row)) {
			current_col = 0;
			aux = new stringstream(row);
			data.push_back(vector<Data>());
			while (getline(*aux, word, ',')) {
				//data.back().push_back(word);




				paddings[current_col] = word.length() > paddings[current_col] ? word.length() : paddings[current_col];
				current_col++;
			}
		}
	}
	void _display_horizontals() {
		cout << '+';
		for (int i = 0; i < data[0].size(); i++) {
			cout << string((1 + paddings[i]), '-') << "+";
		}
		cout << char(8) << "+\n";
	}
};