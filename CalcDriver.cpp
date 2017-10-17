//----------------------------------------------------------------------------
// CalcDriver.cpp
//
// functions:  main()
//					testOstream()
//----------------------------------------------------------------------------
#include <iostream>
#include <cstdlib>
#include "RPNCalc.h"

using namespace std;

int testOstream();

//----------------------------------------------------------------------------
//
//	Function:	main()
//
//	Title:		Driver for RPN Calculator
//
//	Description:	This file contains function main()
//				which creates and starts a calculator
//
//	Programmer:	Paul Bladek
//					Thurman Gillespy
//   
//	Date:		5/20/14
// 
//	Version:	1.0
//  
//	Environment:    Intel Xeon PC 
//                  	Software:   MS Windows 7 for execution; 
//                  	Compiles under Microsoft Visual C++.Net 2013
// 
//	Calls:		CRPNCalc constructor
// 
//	Returns:	EXIT_SUCCESS  = successful 
//
//	History Log:
//			4/205/14  PB  completed version 1.0
// Dev log:
//			6/12/16 TG completed version 1.1
//----------------------------------------------------------------------------
int main(void)
{
	using TPUS_CALC::CRPNCalc;

	CRPNCalc myCalc;

	testOstream();

	return EXIT_SUCCESS;
}

//------------------------------------------------------------------------
//	Method:			testOstream()
//	Description:	tests << and >> operators
//						user prompted to enter 2 numbers and an operator
//						output to the console
//	Date:				6/12/2016
//	Version:			1,0
//	Programmer:		Thurman Gillespy
//	Parameters:		none
//	Returns:			int - exit status
//	Called by:		main()
//	Calls:			overloaded << and >> operators
//	Input:			users enters 2 numbers and an operator fron the console
//	Output:			the calculator screen showing the result
//	Throws:			None
//	Changelog:		6/6/2016	TG verson 1.0
//------------------------------------------------------------------------
int testOstream()
{
	using TPUS_CALC::CRPNCalc;

	CRPNCalc calc2(false);
	
	cout << endl << "testing the << and >> operators" << endl;
	cout << "enter a number: " << endl;
	cin >> calc2;
	cout << "and another: " << endl;
	cin >> calc2;
	cout << "and an operator: " << endl;
	cin >> calc2;
	cout << calc2;

	getchar();
	return EXIT_SUCCESS;
}
