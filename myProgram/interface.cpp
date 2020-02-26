
#include <iostream>     //including I/O support
#include <fstream>      //including file I/O support
#include <map>
#include <vector>		//including support of vector container
#include <string>		//including the new C++ string class support
#include <cstring>		//including old C character array support
#include <conio.h>		//including support to use _getch()
#include "lexScanner.h"	
#include "interface.h"	
#include "expEvaluator.h"
using namespace std;

// 1. Read in a program (lines of statements) from a file
void Interface::loadProgramFromFile()
{
	char fileName[MaxLengthFileName];	
	// this char array stores the name of the file the program reads from.

	cout<<"Name of file to upload program from (ex. file.txt): ";
	cin >> fileName;
	while( strlen(fileName) == 0 )
	{
		cin >> fileName;
	}

	ifstream inFile;
	inFile.open( fileName );
	if ( inFile.fail() )
	{ cout << endl << "Cannot open this file!" << endl;
	return;
	}

	lines.clear();  //Delete all the lines of statements
	string inputBuffer;
	while( !inFile.eof() )
	{
		getline(inFile, inputBuffer);
		while( !inFile.eof() && inputBuffer.size() == 0 )
		{
			getline(inFile, inputBuffer);
		}

		if (!inFile.eof())
		{
  			cout<< "Line["
				<< lines.size()
				<< "] read:"
				<< inputBuffer << endl;
			lines.push_back(inputBuffer);
		}

	}

	inFile.close();
}



// 2. Display the lines of statements
void Interface::displayProgram()
{
	int count = 0;//use to count the line number

	// use vector iterator to display all elements in the lines vector
	for (vector<string>::iterator it = lines.begin(); it != lines.end(); it++,count++){
		cout<< "Line[" << count<<"]  read: " <<*it << endl;
	}
}



// 3. Enter new lines of code to the end of the program
void Interface::appendLines()
{
	string newLine;// to store user inputs
	bool flag = true;

	cout << "Enter . to end" << endl;

	while (flag){
		
		cout << "Enter your new line: ";
		getline(cin,newLine);// to get lines from user input ans save it into newLine
		

		while (newLine.empty()) //Check and skip empty line.
		{
			getline(cin, newLine);
		}

		// check if the user input is . or not. If the user input is == . end the loop, otherwise add newLine to the end of vector
		newLine == "." ? flag = false : lines.push_back(newLine);
	}
}


// 4. Insert new lines before an existing line
void Interface::insertLines()
{	
	string newLine;// to store user inputs for new line
	int index;// to store user inputs for the  index of the vector
	bool flag = true;

	
	cout << "Enter . to end." << endl;
	cout << "Enter the line number after your new line: ";
		cin >> index;


	while (flag){

		cout << "Enter your new line: ";
		getline(cin, newLine);// to get lines from user input ans save it into newLine

		

		if (newLine == ".")
			flag = false;

		else{

			while (newLine.empty()) //Check and skip empty line.
			{
				getline(cin, newLine); // if the user input is empty, ask user to enter new line
			}

			// call insert funtion to insert newLine to the line number of index.Add one to the iterator after every line is entered
			// so the lines entered will follow next to each other
			lines.insert(lines.begin() + index++, newLine );
		}
	}

}


// 5. Delete a range of existing line
void Interface::deleteLines()
{
	int deleteLineBegin;// to store the beginning line number to be deleted
	int deletLineEnd;// to store the last line number to be deleted 

	cout << "Line number to be deleted: from ";
	cin >> deleteLineBegin;
	cout << " to ";
		cin >> deletLineEnd;

		//call erase funtion to delete the lines from deleteLineBegin to deletLineEnd +1
		lines.erase(lines.begin() + deleteLineBegin, lines.begin() + deletLineEnd+1);
}


// 6. Replace the contents of an existing line
void Interface::replaceOneLine()
{
	int index; // to store user inputs for vector index
	string newLine;// to store user inputs for new line

	cout << "Enter the line number to be replaced: ";
	cin >> index;
	cout << "Line" << index << ": ";
		getline(cin, newLine);// to get line from user input and store into newLine

		while (newLine.empty()) //Check and skip empty line.
		{
			getline(cin, newLine);// if the user input is empty, ask user to enter new line
		}

	lines.erase(lines.begin() + index);// call erase function to erase line number index
	lines.insert(lines.begin() + index, newLine);// call insert function to insert newLine to line number index
}


// 7. Save program to a file
void Interface::saveProgramIntoFile()
{
	char fileName[MaxLengthFileName];//to store the name of file  and set the max length of the file name  to MaxLengthFileName
	

	cout << "Name of file to save the program (ex. file.txt): ";
	cin >> fileName;


	while (strlen(fileName) == 0)// if user input is empty, ask user to input new file name
	{
		cout << "ERROR: Empty line entered. Please enter new file name:  ";
		cin >> fileName;
	}

	ofstream inFile;
	inFile.open(fileName);// open the file

	if (inFile.fail())// if the file fail to open, display error message and return.
	{
		cout << endl << "Cannot open this file!" << endl;
		return;
	}
    

	//user vector iterator to access all elements in the lines vector and save them into the file
	for (vector<string>::iterator it = lines.begin(); it != lines.end(); it++){
		inFile << *it << endl;
	}

	inFile.close();// close the file.
	
}

// 8. Lexical analysis of the program line by line.
void Interface::lexialAnalysis(){

	
	vectOfTokenVects vectorOfTokenVectors;// a vector that stores  vectors of tokens from every line in the program
	vectOfCategoryVects vectorOfCategoryVectors;// a vector that stores  vectors of vecotor categorys  from every line in the program

	cout << endl << "" << endl;

	// display all the lines of the program with line numbers beside
	for (size_t i = 0; i< lines.size(); i++)
		cout << "Line["
		<< i
		<< "].\t"
		<< lines[i]
		<< endl;

	cout << endl << "Lexical analysis of the program line by line." << endl << endl;


	//Get the lexical information 
	LexicalScanner::getLexicalInfo(
		lines,
		vectorOfTokenVectors,
		vectorOfCategoryVectors);

	//Display the lexical information 
	LexicalScanner::displayLexicalInfo(
		vectorOfTokenVectors,
		vectorOfCategoryVectors);

	// release all memories of vectorOfTokenVectors and vectorOfCategoryVectors to prepare for next call
	vectorOfTokenVectors.clear();
	vectorOfCategoryVectors.clear();
}


void Interface::calculator() {

	perLineTokenVector infixExp, postfixExp;// two vector of string to store tokens
	map <string, float> table;// useless, declared because the function postfixEvaluator need a table to be passed in as an argument.
	string expression;// variable that will be used to store user  expression
	float expValue;// variable that will be used to store the value of the expression

	cout << "Please enter the arithmetic or logic expression to be evaluated." << endl;
	cout << "Only numeral constants, parenthesis, arithmeric or logical operators are allow." << endl;

		getline(cin, expression);

		// call getPerLineTokenVectFromOneStringObject to get every token in the expression that user entered and store the tokens into infixExp
		LexicalScanner::getPerLineTokenVectFromOneStringObject(expression,infixExp);

		// according to the assignment requirements, only numeral constant, arithmetic operator, logical operator, and parenthesis are allowed.
		for ( int i = 0; i < infixExp.size(); i++) {
			if (!LexicalScanner::isNUMERICAL_LITERAL(infixExp[i]) && !LexicalScanner::isLOGICAL_OP(infixExp[i]) &&
				!LexicalScanner::isNUMERICAL_OP(infixExp[i]) && !LexicalScanner::isLEFT_PARENTHESIS(infixExp[i]) &&
				!LexicalScanner::isRIGHT_PARENTHESIS(infixExp[i])) {
				cout << "ERROR: Invalid key entered." << endl;
				cout << "Valid keys are:  Numerical Literals, Logical Operators, Numerical Operators, Parenthesis" << endl;
				return;
			}
		}
		// call infixToPostfixConversion to convert the infix expression entered by the user into a postfix expression and it in postfixExp
		ExpressionEvaluator::infixToPostfixConversion(infixExp, postfixExp);

	// call postfixEvaluator to get the value of the postfixExp 
		if (ExpressionEvaluator::postfixEvaluator(postfixExp, table, expValue)) {

			for (int i = 0; i < infixExp.size(); i++) 
				cout << infixExp[i];
		
			cout << " = " << expValue << endl;
		}
		
	}


void interpreter::executeProgram(vector<string>& sourceProgram)
{
	typedef map<string, float> stringFloatMap; // define a map with string and float 

	stringFloatMap varTable; // declare a stringFloatMap table for mapping string and float
	vectOfTokenVects vectorOfTokenVectors;// vector to store vectors on tokens
	perLineTokenVector segmentTokenVec, PTV, postExp, infixExp;// vectors to store tokens from one single line
	lineCategoryVec vectorOfLineCategory;// vector to store category of line such as read, display and assignment
	string statement; // to store one line from sourceProgram
	string varName;// to store the name of variable
	string token, segment;// to store one token from the line as string
	float varValue; // to store variable value
	float expValue;// to store variable value after calculation
	bool VarInTable;// to check if the variable is in the table


	// set up for loop to loop through every line in the source program
	for (int i = 0; i < sourceProgram.size(); i++) {

		// assign the single line form the source program into statement
		statement = sourceProgram[i];

		// call getPerLineTokenVectFromOneStringObject from lexicalScanner to break the statement into tokens and store them in vector PTV
		LexicalScanner::getPerLineTokenVectFromOneStringObject(statement, PTV);

		// if the first token stored in PTV is display, it means the line is a display statement. 
		// Then store PTV into vectorofTokenVectors and store word display into vectorOfLineCategory
		if (PTV[0] == "display") {
			vectorOfTokenVectors.push_back(PTV);
			vectorOfLineCategory.push_back("display");
		}

		// else if the first token stored in PTV is read, it means the line is a read statement. 
		// Then store PTV into vectorofTokenVectors and store word read into vectorOfLineCategory
		else if (PTV[0] == "read") {
			vectorOfTokenVectors.push_back(PTV);
			vectorOfLineCategory.push_back("read");
		}

		// else if the first token stored in PTV is an ID name, it means the line is a assign statement. 
		// Then store PTV into vectorofTokenVectors and store word assignment into vectorOfLineCategory
		else if (LexicalScanner::isID_NAME(PTV[0])) {
			vectorOfTokenVectors.push_back(PTV);
			vectorOfLineCategory.push_back("assignment");
		}

		// if the first  token stored in PTV is none of above, it is a invalid statement in Biola language 
		else {
			cout << endl << "ERROR: Unknown statement type: " << statement << endl << endl;
			return;
		}
	}

	// set up a loop to loop through all elements in the vectorOfLineCategory vector
	for (int i = 0; i < vectorOfLineCategory.size(); i++) {

		// if the line category is read, set the variable name to the first token of the line
		if (vectorOfLineCategory[i] == "read") {
			varName = vectorOfTokenVectors[i][1];

			// ask user to enter the value of the variable
			cout << "Enter value for " << varName << ": ";
			;
			
			// check for valid user input
			if (cin >> varValue)
				cout <<"";
				
			else {
				cout << "ERROR: Invalid variable type." << endl;
				return;
			}

			// store the variable name and its value into the variable table
			varTable.insert(pair<string, float>(varName, varValue));

		}

		// if the line category is display,
		else if (vectorOfLineCategory[i] == "display") {

			// set up a loop to loop through every token in the line
			// since a comma and a semicolon mean it's either the end of a segment or the end the sentence,
			// store all the tokens before the comma and the semicolon in to a vetore of tokens and check if its a string literal or a mathemetical expression
			for (int a = 1; a < vectorOfTokenVectors[i].size(); a++) {

				// assign the token into token
				token = vectorOfTokenVectors[i][a];

				// if the token is a comma or a semicolon,
				if (LexicalScanner::isCOMMA(token) || LexicalScanner::isSEMICOLON(token)) {

					// set up a loop to loop through the vector of tokens before the comma or semicolon: segmentTokenVec  and add all tokens into a sentence
					for (int b = 0; b < segmentTokenVec.size(); b++) {

						// if the token is a semicolon, dont add it to the sentence, kust break,
						if (LexicalScanner::isSEMICOLON(segmentTokenVec[b]))
							break;
							segment += segmentTokenVec[b];
							segment += " ";
						
					}
					
					// if the segment is a string literal, erase the first character in the segment which will be a quotation mark
					// erase the last character, which will be null and erase the second to the last character  which will be another quotation mark
					//and the display the segment and clear the segment and  clear the segmentTokenVEc.
					if (LexicalScanner::isSTRING_LITERAL(segment)) {

						segment.erase(segment.begin());
						segment.erase(segment.end()-1);
						segment.erase(segment.end()-1);

						cout << segment ;
						
						segment.clear();
						segmentTokenVec.clear();
					}

					// if the segment is a number or an ID name, 
					else if (LexicalScanner::isNUMERICAL_LITERAL(segmentTokenVec[0]) || LexicalScanner::isID_NAME(segmentTokenVec[0]))
					{
						//call getPerLineTokenVectFromOneStringObject to get all tokens in the segment and store them in infixExp
						LexicalScanner::getPerLineTokenVectFromOneStringObject(segment, infixExp);
					
						//Checking runtime error, set up a loop to loop through infixExp
						for (int z = 0; z < infixExp.size(); z++) {

							VarInTable = false;

							// If the token in the infixExp is an ID name,
							if (LexicalScanner::isID_NAME(infixExp[z])) {

								// loop through the variable table and search for the variable ID name
								for (stringFloatMap::iterator pos = varTable.begin(); pos != varTable.end(); ++pos) {

									// if the variable is found, set varInTable to true and break
									if (pos->first == infixExp[z]) {
										VarInTable = true;
										break;
									}
								}	

								// if the ID name was not found in the table, display error message and return
							 if (VarInTable == false) {
								cout<<endl<< "Variable " << infixExp[z] << " not initialized." << endl;
								return;

							}
							}
						}

						// After checking run time error, call infixToPostfixConversion to convert the infixExp to post expression and store it into postExp
						ExpressionEvaluator::infixToPostfixConversion(infixExp, postExp);

						// call postfixEvaluator to get the value of the postfixExp and store the value into expValue and clear the segment and the vector
						if (ExpressionEvaluator::postfixEvaluator(postExp, varTable, expValue)) {
							cout << expValue;
							segment.clear();
							segmentTokenVec.clear();
						}
					}
				}
				// if the token is not a comma nor a semicolon, store them in the segmentTokenVec
				else
					segmentTokenVec.push_back(token);
			}
			cout << endl;

		}

		// if the sentence category is assignment
		else if (vectorOfLineCategory[i] == "assignment") {

			//run time error check, if the second token of the setence is not a =, it is not a valid assign statement
			if (vectorOfTokenVectors[i][1] != "=") {
				cout << "ERROR: Invalid assign statement: " ;
				for (int q=0; q < vectorOfTokenVectors[i].size(); q++)
					cout << vectorOfTokenVectors[i][q] << " ";
				cout << endl;
				return;
			}

			//After checking the error, set up a loop to loop through every token in the sentence
			for (int a = 0; a < vectorOfTokenVectors[i].size(); a++) {

				// assign the token from the sentence to the token 
				token = vectorOfTokenVectors[i][a];

				// if the token is =
				if (LexicalScanner::isASSIGNMENT_OP(token)) {

					// add all the tokens after the = into a sentence
					for (a ; a < vectorOfTokenVectors[i].size(); a++) {
						if (LexicalScanner::isSEMICOLON(vectorOfTokenVectors[i][a+1]))
							break;

							segment += vectorOfTokenVectors[i][a+1];
							segment += " ";	
					}

					//call the getPerLineTokenVectFromOneStringObject to get all the token from the sentence and store them into infixExp
					LexicalScanner::getPerLineTokenVectFromOneStringObject(segment, infixExp);

					
					// check run time error: 
					for (int x = 0; x < infixExp.size(); x++) {

						VarInTable = false;

						// if the token in the infixExp is an ID name
						if (LexicalScanner::isID_NAME(infixExp[x])) {

							// loop through the variable table and search for the ID name
							for (stringFloatMap::iterator pos = varTable.begin(); pos != varTable.end(); ++pos) {

								// if the variable is found,set VarInTable to true and break,
								if (pos->first == infixExp[x]) {
									VarInTable = true;
									break;
								}

							
							}

							// if the ID name was not found, display error message and return.
							if (VarInTable == false) {
								cout << "Variable " << infixExp[x] << " not initialized." << endl;
								return;

							}
						}
					}

					//after checking the error, call infixToPostfixConversion to over the infixExp to postfix expression and store it into postExp
					ExpressionEvaluator::infixToPostfixConversion(infixExp, postExp);

					// set the variable name to the first token in the sentence
					varName = vectorOfTokenVectors[i][0];

					// call postfixEvaluator to get the value of the postfixExp  and  store it into expValue
					if (ExpressionEvaluator::postfixEvaluator(postExp, varTable, expValue)) {

						// insert the variable name and value into the table
						varTable.insert(pair<string, float>(varName, expValue));
						segment.clear();
					}
				}

			}


		}

	}
}
//An auxilliary message displaying function
void Interface::endOfService(const string service)
{
	cout<<endl << service << " done." << endl << "Press any key to continue.";
	_getch();
	cout << endl;
	cout << "****************************************************" <<endl<< endl;

}

// This menu() function is the interface
void Interface::startInterface() {

	// "Then I suppose now is the time for me to say something profound....
	// Nothing comes to mind. Let's do it."

	bool inMenu = true;	// while the user is using the program this var is set to true and the menu keeps looping.
						// when the user wants to quit, it is set to false and the loop ends.
	char keyPressed;	// This var stores which menu selection the user picks.

	while ( inMenu )
	{
		cout<< endl << endl << endl;
		cout << "****************************************************" << endl;
		cout  <<"**  MENU:(press a character to select an option)  **" << endl;
 		cout << "****************************************************" << endl;
		cout<<"Q. [QUIT]     Quit."<<endl;
		cout<<"L. [LOAD}     Read in a program (lines of statements) from a file"<<endl;
  		cout<<"S. [SAVE]     Save lines of statement as a program into a file"<<endl;
		cout << endl;

		cout<<"D. [DISPLAY]  Display the source code as lines of statements"<<endl;
		cout<<"U. [ANALYSIS] Lexical analysis of all lines of statements currently in the program." << endl;
		cout<<"C. [CALCULATOR]  Evaluate an arithmetic or logic expression." << endl;
		cout << endl;

		cout<<"A. [APPEND]   Append new lines to the end of the program"<<endl;
		cout<<"I. [INSERT}   Insert new lines before an existent line"<<endl;
		cout<<"X. [DELETE]   Delete a range of existent lines"<<endl;
		cout<<"R. [REPLACE]  Replace the contents of an existent line"<<endl;
		cout << endl;

		cout<<"P. [PARSE]    Parse and ident the code" << endl;
		cout<<"E. [EXECUTE]  Execute (run) the program"<<endl;
		cout<<"T. [TOGGLE]   Toggle the execution debug mode"<<endl;

		cout << "****************************************************" <<endl<< endl;

		cout<<"Your choice is: ";
		keyPressed = _getch();
		cout<<keyPressed << endl;

		switch(keyPressed) {
		case 'Q': case 'q'://Quit
			cout << "[QUIT]:" << endl;
			inMenu = false;
			break;

		case 'L': case 'l':// 1. Read in a program (lines of statements) from a file
 			cout << "[LOAD PROGRAM]:" << endl;
			loadProgramFromFile();
			endOfService("[LOAD PROGRAM]");
			break;

		case 'D': case 'd':// 2. Display the lines of statements
			cout << "[DISPLAY PROGRAM]:" << endl;
			displayProgram();
			endOfService("[DISPLAY PROGRAM]");
			break;

		case 'U': case 'u':// 6. Replace the contents of an existing line
			cout << "[ANALYSIS]:" << endl;
			lexialAnalysis();
			endOfService("[LEXICAL ANALYSIS]");
			break;

		case 'A': case 'a':// 3. Append new lines of code to the end
			cout << "[APPEND]:" << endl;
			appendLines();
			endOfService("[APPEND LINES]");
			break;

		case 'I': case 'i':// 4. Insert a new line after an existing line
			cout << "[INSERT LINES]:" << endl;
			insertLines();
			endOfService("[INSERT LINES]");
			break;

		case 'C': case 'c':// 1. Read in a program (lines of statements) from a file
			cout <<"[CALCULATOR]" << endl;
			calculator();
			endOfService("[CALCULATION]");
			break;

		case 'X': case 'x':// 5. Delete an existing line
			cout << "[DELETE LINES]:" << endl;
			deleteLines();
			endOfService("[DELETE LINES]");
			break;

		case 'R': case 'r':// 6. Replace the contents of an existing line
			cout << "[REPLACE]:" << endl;
			replaceOneLine();
			endOfService("[REPLACE ONE LINE]");
			break;

		case 'S': case 's':// 7. Save program to a file
			cout << "[SAVE]:" << endl;
			saveProgramIntoFile();
			endOfService("[SAVE CODE INTO A FILE]");
			break;

		case 'P': case 'p':// 8. Parse and Indent the program
			cout << "[PARSE AND INDENT SOURCE CODE]:" << endl;
			endOfService("[PARSE AND INDENT SOURCE CODE]");
			break;

		case 'E': case 'e':// 9. Run program
			cout << "[EXECUTE TO RUN PROGRAM]:" << endl;
			interpreter::executeProgram(lines);
			endOfService("[EXECUTE TO RUN PROGRAM]");
			break;

		case 'T': case 't':// Toggle the debug mode
			cout << "[TOGGLE the debug mode]:" << endl;
			break;


		default:
			cout << "[?? UNKNOWN COMMAND]:" << endl;
			// Unrecognized key, do nothing
			break;
		}

	} // End of while loop.
}
