#include "RPNCalc.h"
namespace TPUS_CALC
{
	//------------------------------------------------------------------------
	//	Method:			recordProgram()
	//	Description:	Takes command-line input and loads it into m_program.
	//	Date:			6/12/2016
	//	Version:		1.0
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
	//					6/12/2016	DL completed version 1.0.
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
	//	Method:			runProgram()
	//	Description:	Runs the program in m_program.
	//	Date:			6/12/2016
	//	Version:		1.0
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
	//					6/12/2016	DL completed version 0.9.5, fixing an
	//									issue with setting the m_error flag
	//									in the middle of a running program.
	//					6/12/2016	DL completed version 1.0.
	//------------------------------------------------------------------------
	void CRPNCalc::runProgram()
	{
		bool tempError = false;
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
			{
				parse();
				// Temporarily clear out any errors so that the program may
				//	run in its entirety.  Reset the error flag after the
				//	program runs if there was one in the program.
				if (m_error)
				{
					tempError = true;
					m_error = false;
				}
			}
			m_instrStream.clear();
		}
		if (tempError)
			m_error = true;
	}

	//------------------------------------------------------------------------
	//	Method:			saveToFile()
	//	Description:	Asks the user for a filename and saves m_program to 
	//						that file
	//	Date:			6/12/2016
	//	Version:		1.0
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
	//					6/12/2016	DL completed version 1.0
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

	//------------------------------------------------------------------------
	//	Method:			loadProgram()
	//	Description:	Retrieves the filename from the user and loads it into
	//						m_program
	//	Date:			6/12/2016
	//	Version:		1.0
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
	//					6/12/2016	DL completed version 1.0
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
}

