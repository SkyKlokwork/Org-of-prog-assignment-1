#include <iostream>
#include <vector>
#include <regex>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;


static vector<string> TOKENS;
string FileManager();
string PreProcessor(string lineOfCode);
bool istoken(char s);
void VectorHandler(string builtString);
void Scanner(const string& inboundCode);


int main() {
	string str;
	str = PreProcessor(FileManager());
	Scanner(str);
	return 0;
}

string FileManager() {
	//TODO: possibly a local file search for files that match the extension
	string filepath = "./test.opcs";
	string lineOfCode;
	ifstream sourceFile(filepath);
	cout << "please give a valid file path:\n";
	cin >> filepath;
	if (sourceFile.bad()) {
		cout << "bad file";
		exit(131);
	}

	cout << "valid file type detected!\n";
	while(sourceFile.good()) {
		string temp;
		getline(sourceFile,temp);
		lineOfCode.append(temp + " ");
	}
	return lineOfCode;
}

		/*TODO: figure out solutions to double qoutes. Possible solutions:
						1) giving up
						2) rewriting code base
						3) memory access magic
						4) a good nights sleep
						5) iterating twice?*/


string PreProcessor(string lineOfCode) {
	regex comments("\"\"(.*?)\"\"");
	regex whitespace("\\s+");
	string sourceCode,temp;
	temp = regex_replace(lineOfCode,comments,"");

	regex_token_iterator<string::const_iterator> spaceRemover(temp.begin(),temp.end(),whitespace,-1);
	regex_token_iterator<string::const_iterator> end;
	string output;
	while(spaceRemover != end) {
		output = spaceRemover->str();
		if(output.compare("then") == 0 ||
			output.compare("if") == 0 ||
			output.compare("and") == 0 ||
			output.compare("or") == 0) {
				output.push_back('$');
					output.insert(output.begin(),'$');
			}
		spaceRemover++;
		sourceCode.append(output);	
	}
	return sourceCode;
}

bool istoken(char s) {
	return (isalnum(s) || s == '_');
}


void Scanner(const string& inboundCode) {
	stringstream codeStream(inboundCode);
	string stringBuilder;
	char temp;
	while(!codeStream.eof()) {
		codeStream >> temp;
		if(!stringBuilder.empty()) {
			if(istoken(temp) != istoken(stringBuilder.front()) || //checcks to see if the iterator type is different from string front
				(isdigit(stringBuilder.front()) && (isalpha(temp)|| temp=='_'))) { //check for a digit variable by checking if first is digit
					TOKENS.push_back(stringBuilder.c_str());
					stringBuilder.clear();
			}
		}
		if(temp != '$')
			stringBuilder.push_back(temp);

	}
	stringBuilder.pop_back();
	TOKENS.push_back(stringBuilder.c_str());
	for(string x: TOKENS) { //checks for what each of the variables are
		cout << "(";
		if(istoken(x[0])) {
			if(isalpha(x[0]) || x[0] == '_') {	
				if(x.compare("then") == 0 ||
				x.compare("if") == 0) {
					cout << "KEYWORD: " << x <<") ";
				}
				else if(x.compare("and") == 0 ||
				x.compare("ior") == 0) {
					cout << "LOGICAL OPERATOR: " << x <<") ";
				} else
					cout << "IDENTIFIER: " << x <<") ";
			} else 
				cout << "DIGIT: " << x <<") ";
			
		}else if(x.size()>1 && !x.compare("**") == 0) {
			cout << "LOGICAL OPERATOR: " << x <<") ";
		}else if(x[0] != '=') 
			cout << "ARITHMETIC OPERATOR: " << x <<") ";
		else
			cout << "ASSIGNMENT OPERATOR: " << x <<") ";
	}
			
}
