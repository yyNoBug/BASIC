/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
using namespace std;

Program::Program() {}

Program::~Program() {
	clear();
}

void Program::clear() {
	
	std::map<int, Statement*>::iterator it;
	for (it = mp.begin(); it != mp.end(); it++){
		delete it->second;
	}
	mp.clear();
}

void Program::addSourceLine(int lineNumber, string line) {
	
	//注意传入的line包含行首数字

	removeSourceLine(lineNumber);
	TokenScanner scanner(line);
	Statement* stmt;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	scanner.nextToken(); //跳过行首数字
	string stmType = scanner.nextToken();
	stmt = convertToStatement(scanner, line, *this, stmType);
	mp[lineNumber] = stmt;

}

void Program::removeSourceLine(int lineNumber) {
	
	std::map<int, Statement*>::iterator it;
	it = mp.find(lineNumber);
	if (it != mp.end()) {
		delete it->second;
		mp.erase(it);
		//cout << "haha";
	}
	// Replace this stub with your own code
}

string Program::getSourceLine(int lineNumber) {
	
	return mp[lineNumber]->line;    
	// Replace this stub with your own code
}

void Program::setParsedStatement(int lineNumber, Statement* stmt) {

	removeSourceLine(lineNumber);
	mp[lineNumber] = stmt;
	// Replace this stub with your own code
}

Statement *Program::getParsedStatement(int lineNumber) {
	
	return mp[lineNumber];  
	// Replace this stub with your own code
}

int Program::getFirstLineNumber() {

	if (mp.begin() != mp.end()) return mp.begin()->first;
	return -1;
	// 当program中啥也没有的时候返回-1
}

int Program::getNextLineNumber(int lineNumber) {
	
	std::map<int, Statement*>::iterator it;
	it = mp.find(lineNumber);
	it ++;
	if (it != mp.end()) return it->first;
	return -1;
	// Replace this stub with your own code
}

void Program::run(EvalState & state){
	
	int now = getFirstLineNumber();
	while (now != -1) {
		if (mp.find(now) == mp.end()) {
			error("LINE NUMBER ERROR");
		}
		else {
			int nxt = getNextLineNumber(now);
			mp[now]->execute(state, nxt);
			now = nxt;
		}
	}
}

void Program::list(){
	int now = getFirstLineNumber();
	while (now != -1) {
		cout << mp[now]->line << endl;
		now = getNextLineNumber(now);
	}
}

Statement* convertToStatement(TokenScanner & scanner, const string & line, Program &program,
	const string &stmType){

	if (stmType == "REM"){
		return new REM(line);
	}
	else if (stmType == "LET"){
		string var = scanner.nextToken();
		if (!jdg(var)) error("SYNTAX ERROR");
		else {
			scanner.nextToken();//跳过等号
			Expression *exp = parseExp(scanner);
			return new LET(line, var, exp);
		}
	}
	else if (stmType == "PRINT") {
		
		Expression *exp = parseExp(scanner);
		return new PRINT(line, exp);
	}
	else if (stmType == "INPUT") {
		string var = scanner.nextToken();
		if (!jdg(var)) error("SYNTAX ERROR");
		else return new INPUT(line, var);
	}
	else if (stmType == "END") {
		return new END(line);
	}
	else if (stmType == "GOTO") {
		string num = scanner.nextToken();
		int n = stringToInteger(num);

		return new GOTO(line, n);
	}
	else if (stmType == "IF") {
		string s1, cmp, s2, num;

		int p1 = line.find("IF");
		int p2 = line.find("=");
		if (p2 == string::npos) p2 = line.find("<");
		if (p2 == string::npos) p2 = line.find(">");
		int p3 = line.find("THEN");
		
		s1 = line.substr(p1 + 3, p2 - p1 - 3);
		TokenScanner scaExpa(s1);
		scaExpa.ignoreWhitespace();
		scaExpa.scanNumbers();
		Expression *lhs = parseExp(scaExpa);

		cmp = line.substr(p2, 1);

		s2 = line.substr(p2 + 1, p3 - p2 - 1);
		TokenScanner scaExpb(s2);
		scaExpb.ignoreWhitespace();
		scaExpb.scanNumbers();
		Expression *rhs = parseExp(scaExpb);

		num = line.substr(p3 + 5);
		int n = stringToInteger(num);
		
		return new IF(line, lhs, cmp, rhs, n);
	}
	
	//这里可能有小问题，可能没有返回值
}

bool jdg(const string & var){
	if (var == "LET" || var == "REM" || var == "PRINT" || var == "INPUT" || var == "END" 
		|| var == "GOTO" || var == "IF" || var == "THEN" || var == "RUN" || var == "LIST" 
		|| var == "CLEAR" || var == "QUIT" || var == "HELP" )
		return false;
	return true;
}
