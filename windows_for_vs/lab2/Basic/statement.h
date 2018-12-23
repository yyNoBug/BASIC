/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface specified in Chapter 17, which is an
 * excellent model for the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include "evalstate.h"
#include "exp.h"

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

   Statement();
   Statement(string line);

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

   virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);(已改动)
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

   virtual void execute(EvalState & state, int &nxt) = 0;

public:
	std::string line;//这里我要让statement这个东西本身就包含整句话的string的信息,line包含行首的数字
};

class REM : public Statement {
public:
	REM();
	REM(const string &line);
	virtual ~REM();
	virtual void execute(EvalState & state, int &nxt);
};

class LET : public Statement {
public:
	LET();
	LET(const string &line, const string &var, Expression* exp);
	virtual ~LET();
	virtual void execute(EvalState & state, int &nxt);

private:
	string var;
	Expression* exp;
};

class PRINT : public Statement {
public:
	PRINT();
	PRINT(const string &line, Expression *exp);
	virtual ~PRINT();
	virtual void execute(EvalState & state, int &nxt);
private:
	Expression* exp;
};

class INPUT : public Statement {
public:
	INPUT();
	INPUT(const string &line, string var);
	virtual ~INPUT();
	virtual void execute(EvalState & state, int &nxt);
private:
	string var;
};

class END : public Statement {
public:
	END();
	END(const string &line);
	virtual ~END();
	virtual void execute(EvalState & state, int &nxt);
private:

};

class GOTO : public Statement {
public:
	GOTO();
	GOTO(const string &line, int num);
	virtual ~GOTO();
	virtual void execute(EvalState & state, int &nxt);
private:
	int num;
};

class IF : public Statement{
public:
	IF();
	IF(const string &line, Expression *lhs, const string &op, Expression *rhs, int lineNum);
	virtual ~IF();
	virtual void execute(EvalState & state, int &nxt);
private:
	Expression *lhs, *rhs;
	string cmp;
	int lineNum;
};


/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */

#endif
