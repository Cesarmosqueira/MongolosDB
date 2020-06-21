#include<sstream>
#include<string>
using namespace std;

struct Mongo_Types abstract {
	///cast from an string input o
	string description, type;
	stringstream* parser;
	virtual string string_it() abstract;
};
struct MG_INT : public Mongo_Types {
		int value; 
		MG_INT(const string& v) {
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
};

