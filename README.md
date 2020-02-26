# Partial-Implementation-of-the-personal-Interpreter

 Purpose: Implementing a partial interpreter

Basic Specification:

We want to get a taste of implementing an interpreter for a programming language like our BIOLA language. Instead of the full set of the BIOLA language, we would confine the interpreter to deal with only three kinds of statements: the read statement, the display statement, and the assignment statement. We would like to develop an Interpreter class that provides a static method for doing this job. We will then add one additional interpreter option into the startInterface member function of the Interface class. This new option should invoke the static method of the Interpreter class to, statement by statement, check the syntax of the statements stored in the vector of strings lines, and execute the statement if it is syntactically correct, or report error messages if there are syntactic or semantic problems (NOTE:  You may already have an unimplemented "E. [EXECUTE] Execute (run) the program" option in the menu which you can now use for this interpreter option)

 

Simplifying Assumptions:

To make this programming assignment really easy, you can assume the source programs to be executed by the interpreter are all well formatted in the following way:

For the read statement, the display statement, the assignment statement, we assume each statement appears in exactly one single line. In other words, a statement will never spread across several lines, nor will a single line contains multiple statements.
For example, see the partial BIOLA programs in testCylinderVolume1.htm链接到外部网站。 and testCylinderVolume2.htm链接到外部网站。.
 

About the Interpreter class and the executeProgram method:

You need to utilize the lexScanner class and the expressionEvaluator class implemented in the previous programming assignments to implement the Interpreter class. The Interpreter class should have a static member function for executing a Biola source program. For example, the prototype of the member function may look like

void executeProgram( vector< string > & sourceProgram)

which can execute the lines of source code represented as a vector of strings in vector< string > sourceProgram. Note that you are not confined to the suggested prototype and the member function you are free to design your own interpreter class and devise the member functions and/or data members needed to accomplish the job.

 

The following is what we should do in the executeProgram method:

Lexical Analysis: By appropriately declaring data structures for storing the lexical information and calling methods of the code implemented in the lexical scanner module, determine and store the tokens and their categories of the lines of source code. We should also declare a string-to-float map as a symbol table to maintain the names of variables and their values during the program execution.
 

Syntax checking & Execution of statements:
For each line of the source code (one by one), we should then check the corresponding token vector to determine the kind of statements (one of read, display, or assignment). It is an syntax error if it does not correspond to one of the three kinds of statements. If the statement is one of the three kinds of statements, we should execute the statement in the following way:

read : To execute an read statement, get an numerical input from the user and store it appropriately in the corresponding variable in the map as symbol table.
display: To execute a display statement, look at each segment of the tokens separated by commas following the display If the segment is simply a string literal, just display the contents (excluding the enclosing double quotes) to the screen. If the segment is an expression, use the code in expression evaluator module to evaluate it and then display the result to the screen.
assignment statements: To execute an assignment statement, you need to evaluate the value of the expression to the right of the assignment operator. Then you just store the value appropriately into the symbol table (a C++ map) for the variable whose name is shown in the first token.
 

Runtime checking: When executing the statements, it is possible to have run-time errors. All the variables in the Biola languages are of the same numerical float type and variables are declared by (i) using them in the read statement for storing a numerical value or (ii) using them on the left-hand side of an assignment statement to store a numerical value. Note that (i) when a variable is used in the arithmetic expression on the right-hand side of an assignment statement, that variable must have appeared before, i.e. it must have been stored in the symbol table and thus has a valid value, and (ii) similarly when a variable is used in the arithmetic expression in an display statement, that variable must have appeared before, i.e. it must have been stored in the symbol table and thus has a valid value. Otherwise, they end in runtime errors, which should be detected by the interpreter when calling the expression evaluator to evaluate the expression.
