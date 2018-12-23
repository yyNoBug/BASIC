/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */

void processLine(string line, Program & program, EvalState & state);

/* Main program */

int main() {
   EvalState state;
   Program program;
   //cout << "Stub implementation of BASIC" << endl;
   while (true) {
	   try {
		   processLine(getLine(), program, state);
	   }
	   catch (ErrorException & ex) {
         cout << ex.getMessage() << endl;
      }
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processWord   (Program &program, TokenScanner &scanner, EvalState &state, string &fst,
	const string &line);

void processLine(string line, Program & program, EvalState & state) {
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	scanner.setInput(line);
	string fst = scanner.nextToken();

	switch (scanner.getTokenType(fst)) {
	case WORD:
		processWord(program, scanner, state, fst, line);
		break;

	case NUMBER: {
		int n = stringToInteger(fst);
		if (!scanner.hasMoreTokens()) program.removeSourceLine(n);
		else program.addSourceLine(n, line);
		break;
	}
	default:
		error("SYNTAX ERROR");
	}
}


void processWord(Program &program, TokenScanner &scanner, EvalState &state, string &fst,
	const string &line){
	if (fst == "LET" || fst == "REM" || fst == "PRINT" || fst == "INPUT" || fst == "END"
		|| fst == "GOTO" || fst == "IF" || fst == "THEN") {

		Statement* stmt = convertToStatement(scanner, line, program, fst);
		int fake = 0;
		stmt->execute(state, fake);

	}
	else if (scanner.hasMoreTokens())
		error("SYNTAX ERROR");
	else if (fst == "RUN") {
		program.run(state);
	}
	else if (fst == "LIST") {
		program.list();
	}
	else if (fst == "CLEAR") {
		program.clear();
		state.clear();
	}
	else if (fst == "QUIT") {
		exit(0);
	}
	else if (fst == "HELP") {
		cout << "I'm tired of OOP!" << endl;
	}
	else {
		error("SYNTAX ERROR");
	}
}

