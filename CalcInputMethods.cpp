#include "RPNCalc.h"
#include <iterator>
namespace TPUS_CALC
{
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
		// ADD, SUB, MULT, DIV, EXP, MOD, CLR, CLRE, DOWN, UP, FILE, GREG0,
		// GREG1, GREG2, GREG3, GREG4, GREG5, GREG6, GREG7, GREG8, GREG9,
		// SREG0, SREG1, SREG2, SREG3, SREG4, SREG5, SREG6, SREG7, SREG8, 
		// SREG9, HELP, LOAD, M, RECORD, RUN, EXIT
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
			cout << "Press \"Enter\" to exit the calculator.";
			break;
		default:
			m_error = true;
			break;
		}
	}
}