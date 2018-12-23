/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
   /* Empty */
}

Statement::Statement(string line){
	this->line = line;
}

Statement::~Statement() {
   /* Empty */
}

REM::REM(){
	/* Empty */
}

REM::REM(const string &line):Statement(line){
	/* Empty */
}

REM::~REM(){
	/* Empty */
}

void REM::execute(EvalState & state, int &nxt){
	/* Empty */
}

LET::LET(){
	/* Empty */
}

LET::LET(const string & line, const string &var, Expression* exp) :Statement(line) {
	this->var = var;
	this->exp = exp;
}

LET::~LET(){
	delete exp;
}

void LET::execute(EvalState & state, int &nxt){
	int val = exp->eval(state);
	state.setValue(var, val);
}

PRINT::PRINT(){
	/* Empty */
}

PRINT::PRINT(const string &line, Expression * exp) :Statement(line) {
	this->exp = exp;
}

PRINT::~PRINT(){
	delete exp;
}

void PRINT::execute(EvalState & state, int &nxt){
	int val = exp->eval(state);
	cout << val << endl;
}

INPUT::INPUT() {
	/* Empty */
}

INPUT::INPUT(const string &line, string var):Statement(line){
	this->var = var;
}

INPUT::~INPUT(){
	/* Empty */
}

void INPUT::execute(EvalState & state, int &nxt){
	int val;
	bool jdg = false;
	while (!jdg) {
		try {
			cout << " ? ";
			string str;
			getline(cin, str);
			val = stringToInteger(str);
			jdg = true;
		}
		catch (...) {
			cout << "INVALID NUMBER" << endl;
		}
	}
	state.setValue(var, val);
}

END::END() {
	/* Empty */
}

END::END(const string & line):Statement(line){
	/* Empty */
}

END::~END(){
	/* Empty */
}

void END::execute(EvalState & state, int &nxt){
	nxt = -1;
}

GOTO::GOTO() {
	/* Empty */
}

GOTO::GOTO(const string & line, int num):Statement(line){
	this->num = num;
}

GOTO::~GOTO() {
	/* Empty */
}

void GOTO::execute(EvalState & state,int & nxt){
	nxt = num;
}

IF::IF() {
	/* Empty */
}

IF::IF(const string & line, Expression * lhs, const string & cmp, Expression * rhs,
	int lineNum):Statement(line){
	this->lhs = lhs;
	this->rhs = rhs;
	this->cmp = cmp;
	this->lineNum = lineNum;
}

IF::~IF(){
	delete lhs;
	delete rhs;
}

void IF::execute(EvalState & state, int &nxt){
	bool jdg;
	if (cmp == "=") {
		jdg = lhs->eval(state) == rhs->eval(state);
		//cout << lhs->eval(state) << ' ' << rhs->eval(state);
	}
	else if (cmp == "<") jdg = lhs->eval(state) < rhs->eval(state);
	else if (cmp == ">") jdg = lhs->eval(state) > rhs->eval(state);

	if (jdg) nxt = lineNum;
}
