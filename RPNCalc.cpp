//----------------------------------------------------------------------------
//    Class:		CRPNCalc
//
//    File:       RPNCalc.cpp
//
//    Description: This file contains the function definitions for CRPNCalc
//
//    Programmer:		Paul Bladek
//							Thurman Gillespy
//   
//    Date:             
// 
//    Version:          1.1
//  
//    Environment:	Intel Xeon PC 
//                  	Software:   MS Windows 7 for execution; 
//                  	Compiles under Microsoft Visual C++.Net 2010
// 
//	  class CRPNCalc:
//
//	  Properties:
//				double m_registers[10];
//				stack<string> m_stack;
//				vector<list<string>> m_programs(NUMPROGRAMS);
//				istringstream m_inStrStream;
//	
//
//	  Non-inline Methods:
//	
//				CRPNCalc(bool on)
//				run()
//				print(ostream& ostr)
//				parse()
//				void add() -- 
//				void bin_prep(double& d1, double& d2) -- 
//				void clear() -- 
//				void clearAll() -- 
//				void divide() -- 
//				void exp() -- 
//				void getReg(int reg) -- 
//				void loadProgram() -- 
//				void mod() -- 
//				void multiply() -- 
//				void neg() -- 
//				void parse() -- 
//				void recordProgram() -- 
//				void rotateUp() -- 
//				void rotateDown() -- 
//				void runProgram() -- 
//				void saveToFile() -- 
//				void setReg(int reg) -- 
//				void subtract() -- 
//				void unary_prep(double& d) -- 
	
//	  related functions:
//				ostream &operator <<(ostream &ostr, const CRPNCalc &calc)
//    				istream &operator >>(istream &istr, CRPNCalc &calc)
//
//    History Log:
//						6/12/16 TG completed version 1.1
// ---------------------------------------------------------------------------	
#include "RPNCalc.h"

namespace TPUS_CALC
{
	//------------------------------------------------------------------------
	//	Class			:	CRPNcalc
	//	Method		:	CRPNCalc
	//	Description	:	default constructor
	//					:		sets properties to default settings
	//					:		set registers to 0
	//	Calls			:	initMap()
	//					:	run()
	//	Called By	:	constructor
	//	Parameters	:	bool on -- sets the program to running
	//	History Log	:	
	//					  6/10/15 TG completed 1.0
	//-------------------------------------------------------------------------
	CRPNCalc::CRPNCalc(bool on): m_on(on), m_error(false), m_helpOn(true),
		m_programRunning(false), m_trigmode(DEG)
	{
		for(int i = 0; i < NUMREGS; i++)
			m_registers[i] = 0.0;
		initMap();
		if(m_on)
			run();
	}

	//-------------------------------------------------------------------------
	//	Class			:	CRPNcalc
	//	Method		:	run
	//	Description	:	runs the calculator while m_on is ON
	//	Input			:     n/a
	//	Output		:  to console
	//	Calls			:	print()
	//					:	input()
	//	Called By	:	constructor
	//	Parameters	:	none
	//	History Log	:	
	//					  6/10/15 TG completed 1.0
	//-------------------------------------------------------------------------
	void CRPNCalc::run()
	{
		while (m_on == ON)
		{
			print(cout);
			input(cin);
		}
	} 

	//-------------------------------------------------------------------------
	//	Class			:	CRPNcalc
	//	Method		:	print(ostream& ostr)
	//	Description	:	prints to console:
	//					:		header 
	//							top of stack
	//							help menu if m_helpOn is ture
	//							<<error>> if m_error; then resets
	//	Input			:     n/a
	//	Output		:  to console
	//	Calls			:	empty()
	//					:	size()
	//					:	front()
	//	Called By	:	run
	//	Parameters	:	ostream& ostr
	//	History Log	:	
	//					  6/10/15 TG completed 1.0
	//-------------------------------------------------------------------------
	void CRPNCalc::print(ostream& ostr)
	{
		double d = 0.0;
		
		system("cls");

		ostr << "[RPN Programmable Calculator]" << endl;
		ostr << "The Puget Unsound -- " 
					"Thurman Gillespy, David Landry, Jason Gautama" << endl;
		ostr << "original version by Paul Bladek" << endl;
		if (m_helpOn)
			cout << helpMenu;
		else
			cout << endl << endl << endl << endl;
		// status
		cout << endl;
		cout << "Stack size: " << m_stack.size() << "  Trig mode: ";
		cout << ((m_trigmode == RAD) ? "radians" : "degrees") << endl;
		cout << line;
		if(!m_stack.empty())
		{
			d = m_stack.front();
			ostr << d;
		}
		ostr << endl << endl;
		if(m_error)
		{
			ostr << "<<error>>" << endl;
			m_error = false;
		}
	} 

	//------------------------------------------------------------------------
	//	Method:			input()
	//	Description:	Inputs a line from the given stream.
	//	Date:			6/12/2016
	//	Version:		1.0
	//	Programmers:	Thurman Gillespy and David Landry
	//	Parameters:		istream &instr, a reference to an input stream.
	//	Returns:		None
	//	Called by:		run()
	//	Calls:			parse()
	//	Input:			A number, constant escape sequence, or command.
	//	Output:			None
	//	Throws:			None
	//	Changelog:		6/6/2016	TG completed version 0.7
	//					6/7/2016	DL completed version 0.8
	//					6/11/2016	DL completed version 0.9, adding this
	//									method header.
	//					6/12/2016	DL completed version 0.9.5, adding a
	//									m_instrStream.clear() line to make
	//									sure the string stream is pure and
	//									ready for action.
	//					6/12/2016	DL completed version 1.0
	//------------------------------------------------------------------------
	void CRPNCalc::input(istream &instr)
	{
		m_instrStream.clear();
		getline(instr, m_buffer);
		m_buffer += '\n';
		m_instrStream.str(m_buffer);
		parse();
	}

	//------------------------------------------------------------------------
	//	Method:			parse()
	//	Description:	Parses the next command from m_instrStream
	//	Date:			6/12/2016
	//	Version:		1.0.1
	//	Programmers:	Thurman Gillespy and David Landry
	//	Parameters:		None
	//	Returns:		None
	//	Called by:		input()
	//	Calls:			cmd_parse()
	//	Input:			None
	//	Output:			None
	//	Throws:			None
	//	Changelog:		6/6/2016	TG completed version 0.6
	//					6/7/2016	DL completed version 0.7
	//					6/11/2016	DL completed version 0.8, adding this
	//									method header.
	//					6/12/2016	DL	completed version 0.8.5, fixing an
	//									issue with prefixing a number with
	//									+ or -.
	//					6/12/2016	DL	completed version 1.0
	//					6/12/2016	DL	completed version 1.0.1, fixing a
	//									bug where entering a decimal without
	//									a leading 0 would have caused an error
	//									(e.g., .5 instead of 0.5).
	//------------------------------------------------------------------------
	void CRPNCalc::parse()
	{
		string	icommand;
		double	afloat;
		ostringstream oss;
		char	c = '\0';
		bool isNegative = false;
		bool isDecimal = false;

		// input is either a number, a constant or a command
		// check for a number
		if (m_buffer == "\n")
			;
		else
		{
			if (m_instrStream.peek() == '-')
			{
				m_instrStream.get();
				if (isdigit(m_instrStream.peek()) || m_instrStream.peek() == '.')
					isNegative = true;
				else
					m_instrStream.putback('-');
			}
			if (m_instrStream.peek() == '+')
			{
				m_instrStream.get();
				if (isdigit(m_instrStream.peek()) || m_instrStream.peek() == '.')
					isNegative = false;
				else
					m_instrStream.putback('+');
			}
			if (m_instrStream.peek() == '.')
			{
				m_instrStream.get();
				isDecimal = true;
			}
			if (isdigit(m_instrStream.peek()))
			{
				if (m_instrStream >> afloat)
				{
					// Error if anything else in the stream.
					if (m_instrStream.peek() != '\n')
						m_error = true;
					else
					{
						afloat = isNegative ? -afloat : afloat;
						if (isDecimal)
							while (fabs(afloat) >= 1)
								afloat /= 10;
						m_stack.push_front(afloat);
					}
				}
				else
				{
					m_error = true;
				}
			}
			// check for constants
			else if (m_instrStream.peek() == '#')
			{
				// remove *
				m_instrStream.get();
				c = m_instrStream.get();
				c = tolower(c);
				switch (c)
				{
				case 'e':
					if (!m_programRunning)
						m_stack.push_front(CONST_E);
					else
						m_program.push_back("#e");
					break;
				case 'p':
					if (!m_programRunning)
						m_stack.push_front(CONST_PI);
					else
						m_program.push_back("#p");
					break;
				case 'c': // speed of light
					if (!m_programRunning)
						m_stack.push_front(CONST_C);
					else
						m_program.push_back("#c");
					break;
				default:
					m_error = true;
					break;
				}
			}
			else
			{
				// Call the method to handle commands
				m_buffer = m_instrStream.str();
				cmd_parse();
			}
		}
		m_instrStream.clear();
		m_instrStream.ignore(BUFFER_SIZE, '\n');
	}

	//------------------------------------------------------------------------
	//	Method:			cmd_parse()
	//	Description:	Handles commands and operators, directing traffic to
	//						the appropriate methods.
	//	Date:			6/12/2016
	//	Version:		1.0
	//	Programmers:	Thurman Gillespy and David Landry
	//	Parameters:		None
	//	Returns:		None
	//	Called by:		parse()
	//	Calls:			add(); subtract(); multiply(); divide(); mod(); exp();
	//					clearEntry(); clearAll();
	//					rotateDown(); rotateUp();
	//					saveToFile(); loadProgram();
	//					recordProgram(); runProgram()
	//					getReg(); saveReg()
	//	Input:			None
	//	Output:			None
	//	Throws:			None
	//	Changelog:		6/6/2016	TG completed version 0.6
	//					6/7/2016	DL completed version 0.7
	//					6/10/2016	DL completed version 0.8, adding a missing
	//									command.
	//					6/11/2016	DL completed version 0.9, adding this
	//									method header.
	//					6/12/2016	DL completed version 1.0
	//------------------------------------------------------------------------
	void CRPNCalc::cmd_parse()
	{
		string command;
		char c = ' ';
		int counter = -1;
		// Put together the command.
		while (m_buffer[++counter] != '\n')	// This works for 1 cmd/line
		{
			if (isalpha(m_buffer[counter]))
				command += toupper(m_buffer[counter]);
			else
				command += m_buffer[counter];
		}

		// Find the matching mapped command or write the command to a program
		//	node.
		cmd	thecmd = NOVAL;
		int regVal = -1;
		map<string, cmd>::iterator it;
		it = m_map.find(command);
		if (it != m_map.end())
			thecmd = it->second;
		
		switch (thecmd)
		{
		case ADD:
			add();
			break;
		case SUB:
			subtract();
			break;
		case MULT:
			multiply();
			break;
		case DIV:
			divide();
			break;
		case EXP:
			exp();
			break;
		case MOD:
			mod();
			break;
		case CLRE:
			clearEntry();
			break;
		case CLRA:
			clearAll();
			break;
		case DOWN:
			rotateDown();
			break;
		case UP:
			rotateUp();
			break;
		case HELP:
			m_helpOn = (m_helpOn == true) ? false : true; //toggle help on/off
			break;
		case FILE:
			saveToFile();
			break;
		case LOAD:
			loadProgram();
			break;
		case M:
			neg();
			break;
		case TRIGM:
			m_trigmode = (m_trigmode == RAD) ? DEG : RAD; // toggle trig mode
			break;
		case SIN:
			_sin();
			break;
		case ASIN:
			_asin();
			break;
		case COS:
			_cos();
			break;
		case ACOS:
			_acos();
			break;
		case TAN:
			_tan();
			break;
		case ATAN:
			_atan();
			break;
		case SQRT:
			_sqrt();
			break;
		case GR0: case GR1: case GR2: case GR3: case GR4: case GR5:
		case GR6: case GR7: case GR8: case GR9:
			regVal = thecmd - GR0;
			getReg(regVal);
			break;
		case SR0: case SR1: case SR2: case SR3: case SR4: case SR5:
		case SR6: case SR7: case SR8: case SR9:
			regVal = thecmd - SR0;
			setReg(regVal);
			break;
		case RECORD:
			recordProgram();
			break;
		case RUN:
			runProgram();
			break;
		case EXIT:
			m_on = OFF;
			break;
		default:
			m_error = true;
			break;
		}
	}

	//------------------------------------------------------------------------
	//	Class		:     void
	//	Method		:	  add()
	//	Description	:	  if possible, pops top 2 elements from the stack, 
	//					  adds them and pushes the result onto the stack
	//	Input		:     
	//	Output		:     
	//	Calls		:     
	//	Called By	:     
	//	Parameters	:     
	//	History Log	:	
	//					  6/7/16  JM completed version 1.0
	//------------------------------------------------------------------------
	void CRPNCalc::add()
	{
		double first = 0;
		double second = 0;
		binary_prep(second, first);
		if (m_error == false)
			m_stack.push_front(first + second);
	}

	//------------------------------------------------------------------------
	//	Class		:     void
	//	Method		:	  binary_prep()
	//	Description	:	  sets the arg to the popped values from the stack, 
	//					  if possible sets error state otherwise
	//	Input		:     n/a
	//	Output		:     n/a
	//	Calls		:     pop_front()
	//	Called By	:     add()
	//					  subtract()
	//					  multiply()
	//					  divide()
	//					  exp()
	//				      mod()
	//	Parameters	:     double& d1 -- first value reference
	//					  double& d2 -- second value reference
	//	History Log	:	
	//					  6/10/16  JM completed version 1.0
	//					  6/10/16  TG fixed m_stack.size() >= 2
	//------------------------------------------------------------------------

	void CRPNCalc::binary_prep(double& d1, double& d2)
	{
		if (m_error == false && m_stack.size() >= 2)
		{
			d1 = m_stack.front();
			m_stack.pop_front();
			d2 = m_stack.front();
			m_stack.pop_front();
		}
		else
			m_error = true;
	}

	//-------------------------------------------------------------------------
	//	Class			:	CRPNcalc
	//	Method		:	clearEntry
	//	Description	:	removes the top element from the stack (the last entry)
	
	//	Calls			:	empty()
	//					:	pop_front()
	//	Called By	:	cmd_parse
	//	Parameters	:	none
	//	History Log	:	
	//					  6/10/15 TG completed 1.0
	//-------------------------------------------------------------------------
	void CRPNCalc::clearEntry()
	{
		if (!m_stack.empty())
			m_stack.pop_front();
	} 

	//-------------------------------------------------------------------------
	//	Class			:	CRPNcalc
	//	Method		:	clearAll
	//	Description	:	empties the stack
	//	Calls			:	empty()
	//					:	pop_front()
	//	Called By	:	cmd_parse
	//	Parameters	:	none
	//	History Log	:	
	//					  6/10/15 TG completed 1.0
	//-------------------------------------------------------------------------
	void CRPNCalc::clearAll()
	{
		while(!m_stack.empty())	
			m_stack.pop_front();
	} 

	//------------------------------------------------------------------------
	//	Class		:     void
	//	Method		:	  divide()
	//	Description	:	  if possible, pops top 2 elements from the stack, 
	//	  				  divides them and pushes the result onto the stack
	//	Input		:     n/a
	//	Output		:     n/a
	//	Calls		:     binary_prep()
	//					  push_front()
	//	Called By	:     n/a
	//	History Log	:	
	//					  6/7/16  JM completed version 1.0
	//					  6/10/16 JM completed version 1.1
	//------------------------------------------------------------------------
	void CRPNCalc::divide()
	{
		double first = 0;
		double second = 0;
		binary_prep(second, first);
		if (m_error == false)
		{
			if (second == 0)
			{
				m_stack.push_front(first);
				m_stack.push_front(second);
				m_error = true;

			}
			else
				m_stack.push_front(first / second);
		}
		else
			m_error = false;

	}

	//------------------------------------------------------------------------
	//	Class		:     void
	//	Method		:	  exp()
	//	Description	:	  if possible, pops top 2 elements from the stack,
	//	  				  raises one element to the other power
	//	  				  and pushes the result onto the stack
	//	Input		:     n/a
	//	Output		:     n/a
	//	Calls		:     push_front()
	//					  binary_prep()
	//	Called By	:     
	//	Parameters	:     
	//	History Log	:	
	//					  6/7/16  JM completed version 1.0
	//					  6/10/16 JM completed version 1.1
	//------------------------------------------------------------------------
	void CRPNCalc::exp()
	{
		double first = 0;
		double second = 0;
		binary_prep(second, first);
		if (m_error == false)
		{
			if (first == 0 && second == 0)
			{
				m_stack.push_front(first);
				m_stack.push_front(second);
				m_error = true;
			}
			else
				m_stack.push_front(pow(first, second));
		}
	}

	//-------------------------------------------------------------------------
	//	Class			:	CRPNcalc
	//	Method		:	getReg
	//	Description	:	pushes the given register's value onto the stack
	//	Calls			:	push_front()
	//	Called By	:	cmd_parse
	//	Parameters	:	none
	//	History Log	:	
	//					  6/10/15 TG completed 1.0
	//-------------------------------------------------------------------------
	void CRPNCalc::getReg(int reg)
	{
		m_stack.push_front(m_registers[reg]);
	}  

	//------------------------------------------------------------------------
	//	Method:			loadProgram()
	//	Description:	Retrieves the filename from the user and loads it into
	//						m_program
	//	Date:			6/11/2016
	//	Version:		0.9
	//	Programmers:	David Landry
	//	Parameters:		None
	//	Returns:		None
	//	Called by:		cmd_parse()
	//	Calls:			ofstream::open(); ofstream::close()
	//	Input:			The filename.
	//	Output:			A prompt for the filename and various error messages,
	//						if applicable.  The method also prints the program
	//						list if the file loads correctly.
	//	Throws:			File stream error could throw an exception.  This
	//						method catches any errors thrown by the file
	//						stream.
	//	Changelog:		6/10/2016	DL completed version 0.7
	//					6/10/2016	DL completed version 0.8, correctly
	//									using the methods of the list class
	//									(apparently, "m_program[0] =" doesn't
	//									work) and using a list iterator to
	//									print the program list upon successful
	//									retrieval.
	//					6/11/2016	DL completed version 0.9, adding this
	//									method header.
	//------------------------------------------------------------------------
	void CRPNCalc::loadProgram()
	{
		ifstream fileStream;
		char fileName[BUFFER_SIZE];
		list<string>::iterator scanProgram;
		char testChar;
		string individualCommand;
		cout << "Please enter a file name to load your program from." << endl;
		cout << "(The .clc extention will automatically be appended)  ";
		(cin >> fileName).get();
		strcat(fileName, ".clc");
		try
		{
			fileStream.open(fileName);
			if (!fileStream)
			{
				cout << "Could not find the indicated file."
					"  Press \"Enter\" to continue.";
				fileStream.clear();
			}
			else
			{
				m_program.clear();
				while (fileStream.peek() != EOF)
				{
					individualCommand += fileStream.get();
					if (fileStream.peek() == '\n')
					{
						individualCommand += fileStream.get();
						m_program.push_back(individualCommand);
						individualCommand.clear();
					}
				}
				cout << "Index:\tCommand:\n";
				unsigned short i = 0;
				for (scanProgram = m_program.begin();
					scanProgram != m_program.end(); scanProgram++)
				{
					cout << "  " << i++ << "\t" << *scanProgram;
				}
				cout << "Press \"Enter\" to continue.";
			}
			fileStream.close();
			cin.get();
		}
		catch (exception e)
		{
			cout << "Could not open the file.  Press \"Enter\" to continue.";
			fileStream.clear();
		}
	}

	//------------------------------------------------------------------------
	//	Class		:     void
	//	Method		:	  mod()
	//	Description	:	  if possible, pops top 2 elements from the stack,
	//	  				  mods them and pushes the result onto the stack
	//	Input		:     
	//	Output		:     
	//	Calls		:     
	//	Called By	:     
	//	Parameters	:     
	//	History Log	:	
	//					  6/7/16  JM completed version 1.0
	//------------------------------------------------------------------------
	void CRPNCalc::mod()
	{
		double total = 0;
		double first = 0;
		double second = 0;
		binary_prep(second, first);
		if (m_error == false)
			m_stack.push_front(fmod(first, second));
	}

	//------------------------------------------------------------------------
	//	Class		:     void
	//	Method		:	  multiply()
	//	Description	:	  if possible, pops top 2 elements from the stack, 
	//					 multiplies them and pushes the result onto the stack
	//	Input		:     
	//	Output		:     
	//	Calls		:     
	//	Called By	:     
	//	Parameters	:     
	//	History Log	:	
	//					  6/7/16  JM completed version 1.0
	//------------------------------------------------------------------------
	void CRPNCalc::multiply()
	{
		double first = 0;
		double second = 0;
		binary_prep(second, first);
		if (m_error == false)
			m_stack.push_front(first * second);
	}

	//------------------------------------------------------------------------
	//	Class		:     void
	//	Method		:	  neg()
	//	Description	:	  resets the top element of the stack to it's negative
	//	Input		:     n/a
	//	Output		:     n/a
	//	Calls		:     push_front()
	//					  unary_prep()	
	//	Called By	:     cmd_parse()
	//	History Log	:	
	//					  6/9/16  JM completed version 1.0
	//					  6/10/16 JM completed version 1.1
	//------------------------------------------------------------------------
	void CRPNCalc::neg()
	{
		double d = 0;
		unary_prep(d);
		if (m_error == false)
		{
			d *= -1; // change sign
			m_stack.push_front(d);
		}
	}

	//------------------------------------------------------------------------
	//	Class		:     void
	//	Method		:	  unary_prep(double& d)
	//	Description	:	  sets the arg to the popped value from the stack, 
	//					  if possible sets error state otherwise
	//	Input		:     n/a
	//	Output		:     n/a
	//	Calls		:     pop_front()
	//	Called By	:     changeSign()
	//	Parameters	:     double& d -- value reference
	//	History Log	:	
	//					  6/10/16  JM completed version 1.0
	//------------------------------------------------------------------------
	void CRPNCalc::unary_prep(double& d)
	{
		if (!m_stack.empty())
		{
			d = m_stack.front();
			m_stack.pop_front();
		}
		else
			m_error = true;
	}

	//------------------------------------------------------------------------
	//	Method:			recordProgram()
	//	Description:	Takes command-line input and loads it into m_program.
	//	Date:			6/11/2016
	//	Version:		0.9
	//	Programmers:	David Landry
	//	Parameters:		None
	//	Returns:		None
	//	Called by:		cmd_parse()
	//	Calls:			None
	//	Input:			Command lines.
	//	Output:			The line number and a list of commands once recording
	//						is complete.
	//	Throws:			None
	//	Changelog:		6/10/2016	DL completed version 0.6
	//					6/10/2016	DL completed version 0.7, streamlining
	//									and consolidating the code.
	//					6/10/2016	DL completed version 0.8, adding a lot
	//									of comments to make the code easier
	//									to follow.
	//					6/11/2016	DL completed version 0.9, adding this
	//									method header.
	//------------------------------------------------------------------------
	void CRPNCalc::recordProgram()
	{
		char choice = ' ';
		bool isEmpty = false;
		list<string>::iterator scanProgram = m_program.begin();
		// Selecting N will clear out m_program before recording a new
		//	program.  Selecting C will keep m_program as is, allowing the user
		//	to insert new lines starting at the end of the program.
		cout << "(N)ew program or (C)ontinue recording?  ";
		(cin >> choice).get();
		if (toupper(choice) == 'N')
			m_program.clear();
		else if (toupper(choice) == 'C')
			;
		else
			m_error = true;
		if (!m_error)
		{
			cout << "Enter P at any line to view program and exit programming"
				" mode.\n";
			// This program uses m_buffer, so clear out any leftover junk.
			m_buffer.clear();
			int lineCount = m_program.size() - 1;
			m_programRunning = true;	// Turns on program recording mode.
			while (m_programRunning)
			{
				cout << ++lineCount << ">";	// Display line number.
				getline(cin, m_buffer);		// User inputs a line here.
				m_buffer += '\n';			// Newline separates lines.
				// If the user entered a P as a line of programming, the
				//	program will exit program recording mode.  If there is at
				//	least one program line, it will display the lines of
				//	programming.  Finally, m_programRunning will be turned
				//	off (set to false) and the text buffer will be cleared.
				if (toupper(m_buffer[0]) == 'P')
				{
					if (m_program.size() > 0)
					{
						cout << "Index:\tCommand:\n";
						unsigned short i = 0;
						for (scanProgram = m_program.begin();
							scanProgram != m_program.end(); scanProgram++)
						{
							cout << "  " << i++ << "\t" << *scanProgram;
						}
						cout << "Press \"Enter\" to return to the calculator.";
						cin.get();
					}
					m_programRunning = false;
					m_buffer.clear();
				}
				else
					// As long as any text besides P is entered, the entered
					//	line of programming gets pushed into m_program.
					m_program.push_back(m_buffer);
			}
		}
	}

	//------------------------------------------------------------------------
	//	Class		:     void
	//	Method		:	  rotateDown()
	//	Description	:	  remove the top element and place it at the bottom
	//	Input		:     n/a
	//	Output		:     n/a
	//	Calls		:     pop_front()
	//					  push_back()	
	//	Called By	:     cmd_parse()
	//	History Log	:	
	//					  6/11/16 JM completed version 1.0
	//------------------------------------------------------------------------
	void CRPNCalc::rotateDown()
	{
		if (m_stack.empty())
			m_error = true;
		else
		{
			double lastVal = m_stack.front();
			m_stack.pop_front();
			m_stack.push_back(lastVal);
		}
	}

	//------------------------------------------------------------------------
	//	Class		:     void
	//	Method		:	  rotateUp()
	//	Description	:	  remove the bottom element and place it at the top
	//	Input		:     n/a
	//	Output		:     n/a
	//	Calls		:     pop_back()
	//					  push_front()	
	//	Called By	:     cmd_parse()
	//	History Log	:	
	//					  6/11/16 JM completed version 1.0
	//------------------------------------------------------------------------
	void CRPNCalc::rotateUp()
	{
		if (m_stack.empty())
			m_error = true;
		else
		{
			double lastVal = m_stack.back();
			m_stack.pop_back();
			m_stack.push_front(lastVal);
		}
	}

	//------------------------------------------------------------------------
	//	Method:			runProgram()
	//	Description:	Runs the program in m_program.
	//	Date:			6/11/2016
	//	Version:		0.9
	//	Programmers:	DL
	//	Parameters:		None
	//	Returns:		None
	//	Called by:		cmd_parse()
	//	Calls:			parse()
	//	Input:			None
	//	Output:			None
	//	Throws:			None
	//	Changelog:		6/10/2016	DL completed version 0.8.
	//					6/10/2016	DL completed version 0.8.5, adding an
	//									important comment.
	//					6/11/2016	DL completed version 0.9, adding this
	//									method header.
	//------------------------------------------------------------------------
	void CRPNCalc::runProgram()
	{
		list<string>::iterator programScanner = m_program.begin();
		// Run each list element through parse.  Each element should represent
		//	one line of recorded programming.  Error lines will be processed,
		//	but will set the error flag, displaying error at the next print
		//	method call.  However, each line of the program will be run
		//	regardless.
		for (; programScanner != m_program.end(); programScanner++)
		{
			m_instrStream.str(*programScanner);
			m_buffer = m_instrStream.str();
			if (toupper(m_buffer[0]) == 'P')
				programScanner = --m_program.end();
			else
				parse();
		}
	}

	//------------------------------------------------------------------------
	//	Method:			saveToFile()
	//	Description:	Asks the user for a filename and saves m_program to 
	//						that file
	//	Date:			6/11/2016
	//	Version:		0.9
	//	Programmers:	David Landry
	//	Parameters:		None
	//	Returns:		None
	//	Called by:		cmd_parse()
	//	Calls:			ofstream::open(); ofstream::close()
	//	Input:			The file name.
	//	Output:			Prompts for the file name and error messages, if
	//						applicable.
	//	Throws:			File stream error could throw an exception.  This
	//						method catches any errors thrown by the file
	//						stream.
	//	Changelog:		6/10/2016	DL completed version 0.8.
	//					6/11/2016	DL completed version 0.9, adding this
	//									method header.
	//------------------------------------------------------------------------
	void CRPNCalc::saveToFile()
	{
		ofstream fileStream;
		char fileName[BUFFER_SIZE];
		list<string>::iterator programScanner;
		cout << "Please enter a file name to save your program to." << endl;
		cout << "(The file will be automatically saved as a .clc file.)  ";
		(cin >> fileName).get();
		strcat(fileName, ".clc");
		try
		{
			fileStream.open(fileName);
			if (!fileStream)
			{
				cout << "Could not open the file."
					"  Press \"Enter\" to continue.";
				fileStream.clear();
			}
			else
			{
				for (programScanner = m_program.begin();
					programScanner != m_program.end(); programScanner++)
					fileStream << *programScanner;
				fileStream.close();
				cout << "Done.  Press \"Enter\" to continue.";
			}
			fileStream.close();
		}
		catch (exception e)
		{
			cout << "Could not open the file.  Press \"Enter\" to continue.";
			fileStream.clear();
		}
		cin.get();
	}

// ----------------------------------------------------------------------------
//	gets the value from the top of the stack
//	  and places it into the given register
// ----------------------------------------------------------------------------
	void CRPNCalc::setReg(int reg)
	{
		if (!m_stack.empty())
			m_registers[reg] = m_stack.front(); 
		else
			m_error = ON;
	} 

	//------------------------------------------------------------------------
	//	Class		:     void
	//	Method		:	  subtract()
	//	Description	:	 if possible, pops top 2 elements from the stack, 
	//					 subtracts them and pushes the result onto the stack
	//	Input		:     
	//	Output		:     
	//	Calls		:     
	//	Called By	:     
	//	Parameters	:     
	//	History Log	:	
	//					  6/7/16  JM completed version 1.0
	//------------------------------------------------------------------------
	void CRPNCalc::subtract()
	{
		double first = 0;
		double second = 0;
		binary_prep(second, first);
		if (m_error == false)
			m_stack.push_front(first - second);
	}

	//-------------------------------------------------------------------------
	//	Class			:	CRPNcalc
	//	Method		:	_sqrt
	//	Description	:	square root; wrapper for sqrt from <cmath>
	//					:		do nothing if m_error is set
	//	Calls			:	unary_prep()
	//					:	push_front()
	//					:	front()
	//	Called By	:	parse
	//	Parameters	:	none
	//	History Log	:	
	//					  6/10/15 TG completed 1.0
	//-------------------------------------------------------------------------
	void CRPNCalc::_sqrt()
	{
		double d = 0;
		unary_prep(d);
		if (m_error == false)
		{
			m_stack.push_front(sqrt(d));
		}
	}

	//-------------------------------------------------------------------------
	//	Class			:	CRPNcalc
	//	Method		:	_cos
	//	Description	:	cosine; wrapper for cos from <cmath>
	//					:		do nothing if m_error is set
	//	Calls			:	unary_prep()
	//					:	push_front()
	//					:	front()
	//					:	deg2rad
	//	Called By	:	cnd_parse
	//	Parameters	:	none
	//	History Log	:	
	//					  6/10/15 TG completed 1.0
	//-------------------------------------------------------------------------
	void CRPNCalc::_cos()
	{
		double d = 0;

		unary_prep(d);
		if (m_error == false)
			m_stack.push_front((m_trigmode == DEG) ? cos(deg2rad(d)) : cos(d));
	}

	//-------------------------------------------------------------------------
	//	Class			:	CRPNcalc
	//	Method		:	_acos
	//	Description	:	square root; wrapper for sqrt from <cmath>
	//					:		do nothing if m_error is set
	//	Calls			:	unary_prep()
	//					:	push_front()
	//					:	front()
	//					:	rad2deg
	//	Called By	:	cmd_parse
	//	Parameters	:	none
	//	History Log	:	
	//					  6/10/15 TG completed 1.0
	//-------------------------------------------------------------------------
	void CRPNCalc::_acos()
	{
		double d = 0;

		unary_prep(d);
		if (m_error == false)
			m_stack.push_front((m_trigmode == DEG) ? rad2deg(acos(d)) : acos(d));
	}

	//-------------------------------------------------------------------------
	//	Class			:	CRPNcalc
	//	Method		:	_sin
	//	Description	:	sine; wrapper for sine from <cmath>
	//					:		do nothing if m_error is set
	//	Calls			:	unary_prep()
	//					:	push_front()
	//					:	front()
	//					:	deg2rad
	//	Called By	:	cnd_parse
	//	Parameters	:	none
	//	History Log	:	
	//					  6/10/15 TG completed 1.0
	//-------------------------------------------------------------------------
	void CRPNCalc::_sin()
	{
		double d = 0;

		unary_prep(d);
		if (m_error == false)
			m_stack.push_front((m_trigmode == DEG) ? sin(deg2rad(d)) : sin(d));
	}

	//-------------------------------------------------------------------------
	//	Class			:	CRPNcalc
	//	Method		:	_asin
	//	Description	:	arcsin; wrapper for acos from <cmath>
	//					:		do nothing if m_error is set
	//	Calls			:	unary_prep()
	//					:	push_front()
	//					:	front()
	//					:	rad2deg
	//	Called By	:	cmd_parse
	//	Parameters	:	none
	//	History Log	:	
	//					  6/10/15 TG completed 1.0
	//-------------------------------------------------------------------------
	void CRPNCalc::_asin()
	{
		double d = 0;

		unary_prep(d);
		if (m_error == false)
			m_stack.push_front((m_trigmode == DEG) ? rad2deg(asin(d)) : asin(d));
	}

	//-------------------------------------------------------------------------
	//	Class			:	CRPNcalc
	//	Method		:	_tan
	//	Description	:	tangent; wrapper for tan from <cmath>
	//					:		do nothing if m_error is set
	//	Calls			:	unary_prep()
	//					:	push_front()
	//					:	front()
	//					:	deg2rad
	//	Called By	:	cnd_parse
	//	Parameters	:	none
	//	History Log	:	
	//					  6/10/15 TG completed 1.0
	//-------------------------------------------------------------------------
	void CRPNCalc::_tan()
	{
		double d = 0;

		unary_prep(d);
		if (m_error == false)
			m_stack.push_front((m_trigmode == DEG) ? tan(deg2rad(d)) : tan(d));
	}

	//-------------------------------------------------------------------------
	//	Class			:	CRPNcalc
	//	Method		:	_atan
	//	Description	:	arctan; wrapper for atan from <cmath>
	//					:		do nothing if m_error is set
	//	Calls			:	unary_prep()
	//					:	push_front()
	//					:	front()
	//					:	rad2deg
	//	Called By	:	cmd_parse
	//	Parameters	:	none
	//	History Log	:	
	//					  6/10/15 TG completed 1.0
	//-------------------------------------------------------------------------
	void CRPNCalc::_atan()
	{
		double d = 0;

		unary_prep(d);
		if (m_error == false)
			m_stack.push_front((m_trigmode == DEG) ? rad2deg(atan(d)) : atan(d));
	}

	//-------------------------------------------------------------------------
	//	Class			:	CRPNcalc
	//	Method		:	deg2rad
	//	Description	:	convet degrees to radians
	//	Calls			:	none
	//	Called By	:	sin, cos, tan
	//	Parameters	:	double deg -- degrees
	// Returns		:  double - degrees converted to radians
	//	History Log	:	
	//					  6/10/15 TG completed 1.0
	//-------------------------------------------------------------------------
	double CRPNCalc::deg2rad(double deg)
	{
		return (deg * CONST_PI) / 180.0;
	}

	//-------------------------------------------------------------------------
	//	Class			:	CRPNcalc
	//	Method		:	rad2deg
	//	Description	:	convet radians to degrees
	//	Calls			:	none
	//	Called By	:	the inverse trig functions (arccos, arcsin, arctan)
	//	Parameters	:	double rad -- radians
	// Returns		:  double -  readians coverted to degrees
	//	History Log	:	
	//					  6/12/15 TG completed 1.0
	//-------------------------------------------------------------------------
	double CRPNCalc::rad2deg(double rad)
	{
		return (rad * 180) / CONST_PI;
	}

	//-------------------------------------------------------------------------
	//	Class			:	CRPNcalc
	//	Method		:	initMap
	//	Description	:	initialize map database of type map<sting, cmd>
	//					:		maps a string from the command line to an enum
	//	Calls			:	emplace
	//	Called By	:	constructor
	//	Parameters	:	none
	//	History Log	:	
	//					  6/10/15 TG completed 1.0
	//-------------------------------------------------------------------------
	void CRPNCalc::initMap(void)
	{
		m_map.emplace("+", ADD);
		m_map.emplace("-", SUB);
		m_map.emplace("*", MULT);
		m_map.emplace("/", DIV);
		m_map.emplace("^", EXP);
		m_map.emplace("%", MOD);
		m_map.emplace("C", CLRA);
		m_map.emplace("CE", CLRE);
		m_map.emplace("D", DOWN);
		m_map.emplace("F", FILE);
		m_map.emplace("H", HELP);
		m_map.emplace("L", LOAD);
		m_map.emplace("M", M);
		m_map.emplace("P", RECORD);
		m_map.emplace("R", RUN);
		m_map.emplace("U", UP);
		m_map.emplace("X", EXIT);
		m_map.emplace("T", TRIGM);
		m_map.emplace("SQRT", SQRT);
		m_map.emplace("COS", COS);
		m_map.emplace("SIN", SIN);
		m_map.emplace("TAN", TAN);
		m_map.emplace("ACOS", ACOS);
		m_map.emplace("ASIN", ASIN);
		m_map.emplace("ATAN", ATAN);
		m_map.emplace("G0", GR0);
		m_map.emplace("G1", GR1);
		m_map.emplace("G2", GR2);
		m_map.emplace("G3", GR3);
		m_map.emplace("G4", GR4);
		m_map.emplace("G5", GR5);
		m_map.emplace("G6", GR6);
		m_map.emplace("G7", GR7);
		m_map.emplace("G8", GR8);
		m_map.emplace("G9", GR9);
		m_map.emplace("S0", SR0);
		m_map.emplace("S1", SR1);
		m_map.emplace("S2", SR2);
		m_map.emplace("S3", SR3);
		m_map.emplace("S4", SR4);
		m_map.emplace("S5", SR5);
		m_map.emplace("S6", SR6);
		m_map.emplace("S7", SR7);
		m_map.emplace("S8", SR8);
		m_map.emplace("S9", SR9);
	}

	//-------------------------------------------------------------------------
	//	Class			:	CRPNcalc
	//	Method		:	operator <<
	//	Description	:	print calculator ouptput
	//	Input			:	n/a
	//	Output		:  to console
	//	Calls			:	inputt()
	//	Called By	:	main
	//	Parameters	:	ostream &ostr -- output goes here
	//					:	CRPNCalc &calc -- calculator object
	//	History Log	:	
	//					  original by Paul Bladek
	//-------------------------------------------------------------------------
   ostream &operator <<(ostream &ostr, CRPNCalc &calc)
	{
		calc.print(ostr);
		return ostr;
	} 

	//-------------------------------------------------------------------------
	//	Class			:	CRPNcalc
	//	Method		:	operator >>
	//	Description	:	calculator input fron an istream
	//	Input			:	n/a
	//	Output		:  to console
	//	Calls			:	inputt()
	//	Called By	:	main
	//	Parameters	:	istream &istr -- the input stream
	//					:	CRPNCalc &calc -- calculator object
	//	History Log	:	
	//					  original by Paul Bladek
	//-------------------------------------------------------------------------
	istream &operator >>(istream &istr, CRPNCalc &calc)
	{
		calc.input(istr);
		return istr;
	} 
} // end namespace TPUS_CALC
