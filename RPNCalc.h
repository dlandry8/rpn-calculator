//----------------------------------------------------------------------------
//    File:		RPNCalc.h
//
//    Class:	CRPNCalc
//----------------------------------------------------------------------------
#ifndef RPNCALC_H
#define RPNCALC_H

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <new>
#include <sstream>
#include <stack>
#include <map>
//----------------------------------------------------------------------------
//
//    Title:		RPNCalc Class
//
//    Description:	This file contains the class definition for CRPNCalc
//
//    Programmer:	Paul Bladek
//						Thurman Gillespy
//   
//    Date:		6/12/16
// 
//    Version:		1.1
//  
//    Environment:
//       Hardware: Intel Xeon PC
//       Software: Windows 7
//       Compiles under Microsoft Visual Studio 2013
// 
//	  class CRPNCalc:
//
//	  Properties:
//		double m_registers[10] -- registers 0 - 9
//		string m_buffer -- used in handling input
//		stack<string> m_stack -- calculator numbers added and removed as needed
//		list<string> m_program  --  the current program
//		istringstream m_inStrStream -- for handling input
//		m_on -- determines when program is to quit
//		bool m_error -- error flag; cleared by print
//		bool m_helpOn --  if true, help menu displayed
//		bool m_programRunning -- program mode is on, recroding commands
//		RPNmap m_map -- map<string, cmd> - maps command line string to an enum
//		trigmode m_trigmode -- radians vs degrees 
//		
//
//	  Methods:
//	
//		inline:	None		
//
//		non-inline:
//		public:
//			CRPNCalc(bool on);
//			void run();                                        
//			void print(ostream& ostr);
//			void input(istream& istr);
//		private:
//				
//			void add() -- 
//			void bin_prep(double& d1, double& d2) -- 
//			void clearEntry() -- 
//			void clearAll() -- 
//			void cmd_parse() --
//			void divide() -- 
//			void exp() -- 
//			void getReg(int reg) -- 
//			void loadProgram() -- 
//			void mod() -- 
//			void multiply() -- 
//			void neg() -- 
//			void parse() -- 
//			void recordProgram() -- 
//			void rotateUp() -- 
//			void rotateDown() -- 
//			void runProgram() -- 
//			void saveToFile() -- 
//			void setReg(int reg) -- 
//			void subtract() -- 
//			void unary_prep(double& d) --
//			void initMap();
//			void _sin();
//			void _cos();
//			void _tan();
//			void _asin();
//			void _acos();
//			void _atan();
//			double deg2rad(double deg);
//			double rad2deg(double rad);
//
//    History Log:
//	4/20/03	PB  completed version 1.0
//	5/27/05	PB  minor modifications 1.01
//	5/3/11	PB  minor modifications 1.02
//	6/3/12  PB  minor modifications 1.03
//
//		Dev log:
//			0.1 6/5/16 tg
//			6/8/16 r2 added constant constants
//			6/9/16 added changeSign declaration
//			6/11/16 updated file header
//			6/12/16 deg2rad added; full trig support
//			6/12/16 rad2deg added for inverse trig; final headers and bug fixes
//			6/12/16 - version 1.1 by TG
// ----------------------------------------------------------------------------

using namespace std;

namespace TPUS_CALC
{

	const char helpMenu[] =
	"C clear stack   | CE clear entry  | D rotate down  | F save program to file\n"
	"G0-G9 get reg n | H help on/off   | L load program | M +/-  | P program on/off\n"
	"R run program   | S0-S9 set reg n | U rotate up    | X exit | T toggle rad/deg\n"
	"Constants: #e e, #p pi, #c c      | Trig: cos, sin, tan, acos, asin, atan\n";

	const char line[] = "______________________________"
								"______________________________________________\n";

	const unsigned short NUMREGS = 10;
	const bool ON = true;
	const bool OFF = false;
	const short BUFFER_SIZE = 256;
	const double CONST_E = 2.7182818;
	const double CONST_PI = 3.14159265;
	const double CONST_C = 299792458;

	typedef enum trigmode { RAD, DEG };

	typedef enum cmd {
		NOVAL, ADD, SUB, MULT, DIV, EXP, MOD, CLRE, CLRA, DOWN, UP,
		FILE, HELP, LOAD, M, RECORD, RUN, TRIGM, EXIT, SQRT,
		COS, ACOS, SIN, ASIN, TAN, ATAN,
		GR0, GR1, GR2, GR3, GR4, GR5, GR6, GR7, GR8, GR9,
		SR0, SR1, SR2, SR3, SR4, SR5, SR6, SR7, SR8, SR9
	};

	typedef map<string, cmd> RPNmap;

	class CRPNCalc
	{
	public:
		CRPNCalc(bool on = true);
		void run();                                        
		void print(ostream& ostr);  // changes m_error on error, so not const
		void input(istream& istr);

	private:
	// private methods
		void add();
		void binary_prep(double& d1, double& d2);
		void clearEntry();
		void clearAll();
		void cmd_parse();
		void divide();
		void exp();
		void getReg(int reg);
		void loadProgram();
		void mod();
		void multiply();
		void neg();
		void parse(); 
		void recordProgram();
		void rotateUp();
		void rotateDown();
		void runProgram();
		void saveToFile();
		void setReg(int reg);
		void subtract();
		void unary_prep(double& d);
		void initMap();
		void _sqrt();
		void _sin();
		void _cos();
		void _tan();
		void _asin();
		void _acos();
		void _atan();
		double deg2rad(double deg);
		double rad2deg(double rad);

	// private properties
		double m_registers[NUMREGS];
		string m_buffer;
		deque<double> m_stack;
		list<string> m_program;
		istringstream m_instrStream;
		bool m_error;
		bool m_helpOn;
		bool m_on;
		bool m_programRunning;
		RPNmap m_map;
		trigmode m_trigmode;
	};

	ostream &operator <<(ostream &ostr, CRPNCalc &calc);
	istream &operator >>(istream &istr, CRPNCalc &calc);

} // end namespace PB_CALC

#endif
