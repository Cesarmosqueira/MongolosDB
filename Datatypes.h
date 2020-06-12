#include<sstream>
#include<string>
using namespace std;

struct Mongo_Types abstract {
	///cast from an string input o
	virtual string string_it() abstract;
};
struct MG_INT : public Mongo_Types {
		int value;
		MG_INT(const string& v) {
			parser = new stringstream(v);
			*parser >> value;
		}
		~MG_INT() { delete parser; }

		string string_it() {
			return string(parser->str());
		}
private:
	stringstream* parser;
};
struct MG_STRING : public Mongo_Types {
		string value;
		MG_STRING(const string& v) {
			value = v;
		}
		string string_it() {
			return value;
		}
};
struct MG_FLOAT : public Mongo_Types {
		float value;
		MG_FLOAT(const string& v) {
			parser = new stringstream(v);
			*parser >> value;
		}
		~MG_FLOAT() { delete parser; }

		string string_it() {
			return string(parser->str());
		}
private:
	stringstream* parser;
};

