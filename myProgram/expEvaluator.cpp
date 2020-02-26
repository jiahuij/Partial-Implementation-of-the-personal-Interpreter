//cont/map1.cpp
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cstdlib>
#include "lexScanner.h"
#include "expEvaluator.h"
using namespace std;


//*********************************************************************************
//*********************************************************************************
int ExpressionEvaluator::precedenceLevel(const string & opToken)
//return the precedence level of an operator token
{
				if ( opToken == "||" )
					return 0;

				if ( opToken == "&&" )
					return 1;


				if ( opToken == "<" )
					return 2;
				if ( opToken == "==" )
					return 2;
				if (opToken == "!=")
					return 2;
				if ( opToken == ">" )
					return 2;
				if ( opToken == ">=" )
					return 2;
				if ( opToken == "<=" )
					return 2;

				if ( opToken == "+" )
					return 3;
				if ( opToken == "-" )
					return 3;

				if (opToken == "%")
					return 4;
				if ( opToken == "*" )
					return 4;
				if ( opToken == "/" )
					return 4;

				if (opToken == "!")
					return 5;

				if (opToken != ")" && opToken != "(")
				cout << "unknown op:" << opToken << ". precedence level?? " << endl;
				return -1;

}


//*********************************************************************************
//*********************************************************************************
bool ExpressionEvaluator::postfixEvaluator
(	const expVector & postfixExp, 
	floatVarValueTable & varTable, 
	float & expValue 
)
//Evaluate the value of a postfix expression postfixExp
//with respect to the variables and their current values  in varTable.
//finally store the value of the expresson in expValue.
//If postfixExp is not a valid postfix expression, return false; otherwsie return true.
{
	vector<float> stack;

	for ( expVector::const_iterator pos= postfixExp.begin();
		  pos != postfixExp.end();
		  pos++
		)
	{
		string token;

		token = *pos;
		if ( token.size()< 1)
		{	expValue = 0;
			cout << "Empty Token, not valid." << endl;
			return false;
		}

		if ( LexicalScanner::isNUMERICAL_LITERAL(token) )		
			//A numerical constant, convert it into a float value, and
			//store push it onto the stack
			stack.push_back(	atof( token.c_str() )		);

		else if ( LexicalScanner::isID_NAME(token) )		
			//An ID_NAME, try to look up its current value from the table.
			//If succeeds, push the value onto the stack.
			{	if (varTable.find(token) == varTable.end())
						return false; //Invalid name
				stack.push_back(	varTable[token]	);
			}
		else if (	LexicalScanner::isNUMERICAL_OP(token)  ||  
					LexicalScanner::isRELATIONAL_OP(token) || 
					LexicalScanner::isLOGICAL_OP(token)
				)
			//A numerical or relational operator
			//Try to pop two operands from the stack apply the operator.
			//If succeeds, push te result back to the stack.
			{
				float op1, op2; 

				//Try to pop op2 first
				if ( !stack.empty() )
					{	op2 = stack.back();
						stack.pop_back();
					}
				else
				{	cout << "No matching op2 for the op: " << token << endl;
					return false; //op2 not available, Not a valid expression
				}

				
				//Determine whether this is a bianry operator or a unary operator
				bool isUnaryOP, isBinaryOP;
				if	(	token == "!"	)	//unary logical negation operator
					{	isUnaryOP = true; isBinaryOP = false;
					}
				else
					{	isUnaryOP = false; isBinaryOP = true;
					}

				if (isUnaryOP) //When the operator is a unary operator
				{	//the only case at this moment: the logical negation operator ! 
					if (token == "!")
						stack.push_back(!op2);
					else
					{		cout << "Unknown unary op: " << token << endl;
							return false;
					}
				} //end the processing of a unary operator

				if (isBinaryOP)  //When the operator is a binary operator
				{
					if ( stack.empty() )
						return false; //op1 not available, Not a valid expression

					op1 = stack.back();
					stack.pop_back();
				
					//Apply the binary operator and push back the result to the stack
					if (token == "+")
						stack.push_back(op1 + op2);
					else if (token == "-")
						stack.push_back(op1 - op2);
					else if (token == "*")
						stack.push_back(op1*op2);
					else if (token == "/")
						stack.push_back(op1 / op2);
					else if (token == "%") {
						int op1int, op2int;
						op1int = op1;
						op2int = op2;
						stack.push_back(op1int % op2int);
					}

					else if (token == "<")
						stack.push_back(op1 < op2);
					else if (token == "==")
						stack.push_back(op1 == op2);
					else if (token == "!=")
						stack.push_back(op1 != op2);
					else if ( token == ">" )
						stack.push_back(op1>op2);
					else if ( token == ">=" )
						stack.push_back(op1>=op2);
					else if ( token == "<=" )
						stack.push_back(op1<=op2);

					else if ( token == "&&" )
						stack.push_back(op1&&op2);
					else if ( token == "||" )
						stack.push_back(op1||op2);
					else 
						{	cout << "Unkown binary op: " << token << endl;
							return false;
						}
				} //end the processing of a binary operator

			}//end the processing of the case that the token is an operator

		else
			{	cout << "Unkown token type (Not an op or operand): " << token << endl;
				return false;
			}
			// Every token in the expression must be either be a numerical constant, 
			// an ID_NAME or an operator; Otherwise it is NOT a valid expression.

	}//end for


	if (stack.size() == 1)
	//The evaluated value of the expression should be the only value left on the stack
		{	expValue = stack.back();
			return true;
		}
	else
	//Other wise it is not a valis expression.
		return false;

}

// check if the token is an operator
bool isOperator(const string & token) {
	if (LexicalScanner::isNUMERICAL_OP(token) || LexicalScanner::isRELATIONAL_OP(token) || LexicalScanner::isLOGICAL_OP(token))
		return true;

		return false;
}

//check if the token is operand
bool isOperan(const string & token) {
	if (LexicalScanner::isNUMERICAL_LITERAL(token) || LexicalScanner::isID_NAME(token))
		return true;
	
		return false;
}

//Check is the token is a parenthesis
bool isParenthesis(const string & token) {
	if (LexicalScanner::isLEFT_PARENTHESIS (token) || LexicalScanner::isRIGHT_PARENTHESIS(token))
		return true;

	return false;
}


//*********************************************************************************
//*********************************************************************************
bool ExpressionEvaluator::infixToPostfixConversion
(const expVector & infixExp, expVector & postfixExp)
//Convert the infix expression in infixExp into a corresponding postfix expression.
//Store the corresponding postfix expression in postfixExp.
//If infixExp is not a valid infix expression, return false; otherwsie return true.
{

	postfixExp.clear();//  clear the postfixExp before pushing things in
	vector<string>postStack;// stack that will be used to store operators in the expression
	string ExpToken;// variable that will be used to store one token from the expression
	string nextToken;// variable that will be used to store next token
	bool validInfixExp = true;// check if the expression is a valid infix expression

	// if user entered an empty expression, return false
	if (infixExp.size() < 1)
	{
		cout << "ERROR: Empty Infix Expression." << endl;
		return false;
	}

	
	for (int i = 0; i < infixExp.size(); i++)
	{
		// assign the token from infixExp to ExpToken
		ExpToken = infixExp[i];

		// if next token is not the end of the infixExp, set nextToken to the token next to ExpToken
		if (i + 1 != infixExp.size())
			nextToken = infixExp[i + 1];
		else// if the token next to ExpToken is the end of infixExp, set nextToken to " ";
			nextToken = " ";

		// if ExpToken is not an operand nor a operator nor a parenthesis, 
		// it is not a valid infix expression. So clear all the tokens in the postfixExp and return false
		if (!isOperan(ExpToken) && !isOperator(ExpToken) && !isParenthesis(ExpToken)) {
			postfixExp.clear();
			return false;
		}

		// no two operators can be next to each other except ! and relational operators
		if (ExpToken == "!" && LexicalScanner::isRELATIONAL_OP(nextToken)) {
			validInfixExp == true;
		}
		else if (isOperator(ExpToken) && isOperator(nextToken))
		{
			cout << "ERROR: Invalid Infix Expression." << endl;
			postfixExp.clear();
			return false;
		}

		if (validInfixExp) {

			// if the token isan operand, just push back the token into the postfixExp
			if (isOperan(ExpToken))
				postfixExp.push_back(ExpToken);

			// if postStack is empty and the token is an token is not an operand just push back the token into postStack
			else if (postStack.size() == 0)
				postStack.push_back(ExpToken);

			//if the tokem is a left parenthesis, just push it back into postStack
			else if (LexicalScanner::isLEFT_PARENTHESIS(ExpToken)) 
				postStack.push_back(ExpToken);

			// if the token is a right parenthesis, set up a loop to push all the operators between the left and the right parenthesis
			else if (LexicalScanner::isRIGHT_PARENTHESIS(ExpToken)) {

				while (!LexicalScanner::isLEFT_PARENTHESIS(postStack.back()) && postStack.size() != 0)
				{
					// push back the token into postfixExp and pop off the the token at the postStack
					postfixExp.push_back(postStack.back());
					postStack.pop_back();

					// if the postStack is already empty and no left parenthesis is found, clear the postfixExp and return false
					if (postStack.size() == 0) {
						cout << "ERROR: Missing Left Parenthesis." << endl;
						postfixExp.clear();
						return false;
					}

				}
			       // if the left parenthesis if found, pop off it from the postStack
				if (LexicalScanner::isLEFT_PARENTHESIS(postStack.back()))
					postStack.pop_back();
			}

			//if the the token has a higher precedence level than the precedence level of the operator at the end of the postStack,
			//just push the token in to the stack.
			else if (precedenceLevel(ExpToken) > precedenceLevel(postStack.back()))
				postStack.push_back(ExpToken);

			// if the the token has a lower or equel precedence level than the precedence level of the operator at the end of the postStack,
			//set up a loop to pop off the operator at the postStack and push them back into postfiExp.
			else if (precedenceLevel(ExpToken) <= precedenceLevel(postStack.back())) {

				while (precedenceLevel(ExpToken) <= precedenceLevel(postStack.back())) {
					postfixExp.push_back(postStack.back());
					postStack.pop_back();

					// if the stack is empty, just push the token into the stack
					if (postStack.size() == 0) {
						postStack.push_back(ExpToken);
						break;
					}

					 // if the the token has a higher precedence level than the precedence level of the operator at the end of the postStack,
					//  just push the token in to the stack.
					else if (precedenceLevel(ExpToken) > precedenceLevel(postStack.back())) {
						postStack.push_back(ExpToken);
						break;
					}

				}
			}
		}

		// if ExpToken is the last token in the infixExp, set up a loop to push all the operators in the postStack into the postfixExp.
		if (i + 1 == infixExp.size()) {

			while (postStack.size() != 0) {
				postfixExp.push_back(postStack.back());
				postStack.pop_back();

			}
		}
	}

	return true;
}

//*********************************************************************************
//*********************************************************************************
bool ExpressionEvaluator::infixEvaluator
(	const expVector & infixExp, 
	floatVarValueTable & varTable, 
	float& expValue
)
//Evaluate the value of the infix expression in postfixExp
//with respect to the variables and their current values in varTable.
//finally store the value of the expresson in expValue.
//If postfixExp is not a valid postfix expression, return false; otherwsie return true.
{	expVector postfixExp;
	infixToPostfixConversion(infixExp, postfixExp);
	return postfixEvaluator(postfixExp, varTable, expValue);
}



//*********************************************************************************
//*********************************************************************************
bool ExpressionEvaluator::infixEvaluator	
(	expVector::iterator begin, 
	expVector::iterator end, 
	floatVarValueTable & varTable, 
	float &expValue
)
//Evaluate the value of an infix expression 
//that is embedded within an expression vector 
//The two expVector::iterator iterators: begin and end specify 
//where the infix expression is embedded.
//
//Evaluate the value of an infix expression 
//with respect to the variables and their current values in varTable.
//finally store the value of the expresson in expValue.
//If postfixExp is not a valid postfix expression, return false; otherwsie return true.
{	expVector infixExp(begin,end);
	return infixEvaluator(infixExp, varTable, expValue);
}
