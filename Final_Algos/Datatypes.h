#include<sstream>
#include<string>
using namespace std;

/// casted by a string input
struct Mongo_Types abstract {
	string description, type;
	stringstream* parser;
	virtual string string_it() abstract;
	bool operator<(const Mongo_Types& other) {
		if (other.type == "int") {
			return stoi(other.description) > stoi(description);
		}
		else if (other.type == "string") {
			return other.description > description;
		}
		else if (other.type == "float") {
			return stof(other.description) > stof(description);
		}
		else cout << "wtf"; cin.get(); exit('☺');
	}
	bool operator>(const Mongo_Types& other) {
		if (other.type == "int") {
			return stoi(other.description) < stoi(description);
		}
		else if (other.type == "string") {
			return other.description < description;
		}
		else if (other.type == "float") {
			return stof(other.description) < stof(description);
		}
		else cout << "wtf"; cin.get();
	}
	bool operator<=(const Mongo_Types& other) {
		if (other.type == "int") {
			return stoi(other.description) >= stoi(description);
		}
		else if (other.type == "string") {
			return other.description >= description;
		}
		else if (other.type == "float") {
			return stof(other.description) >= stof(description);
		}
		else cout << "wtf"; cin.get(); exit('☺');
	}
	bool operator>=(const Mongo_Types& other) {
		if (other.type == "int") {
			return stoi(other.description) <= stoi(description);
		}
		else if (other.type == "string") {
			return other.description <= description;
		}
		else if (other.type == "float") {
			return stof(other.description) <= stof(description);
		}
		else cout << "wtf"; cin.get();
	}
	bool operator==(const Mongo_Types& other) {
		if (other.type == "int") {
			return stoi(other.description) == stoi(description);
		}
		else if (other.type == "string") {
			return other.description == description;
		}
		else if (other.type == "float") {
			return stof(other.description) == stof(description);
		}
		else cout << "wtf"; cin.get();
	}
};
struct MG_INT : public Mongo_Types {
	int value;
	MG_INT(const string& v) { //"345"
		description = v;
		parser = new stringstream(v);
		*parser >> value;
		type = "int";
	}
	~MG_INT() { delete parser; }

	string string_it() {
		parser->clear();
		*parser << value;
		return string(parser->str());
	}
	bool asc_order(const MG_INT& ot) {
		return value > ot.value;
	}
	bool desc_order(const MG_INT& ot) {
		return value < ot.value;
	}
};						  
struct MG_STRING : public Mongo_Types {
	string value;
	MG_STRING(const string& v) { 
		description = v;
		parser = nullptr;
		value = v;
		type = "string";
	}
	string string_it() {
		return value;
	}
	bool asc_order(const MG_STRING& ot) {
		return value > ot.value;
	}
	bool desc_order(const MG_STRING& ot) {
		return value < ot.value;
	}
};					  
struct MG_FLOAT : public Mongo_Types {		  
	float value;						  
	MG_FLOAT(const string& v) {
		description = v;
		parser = new stringstream(v);
		*parser >> value;
		type = "float";
	}
	~MG_FLOAT() { delete parser; }
	string string_it() {
		parser->clear();
		*parser << value;
		return string(parser->str());
	}
	bool asc_order(const MG_FLOAT& ot) {
		return value > ot.value;
	}
	bool desc_order(const MG_FLOAT& ot) {
		return value < ot.value;
	}
};

