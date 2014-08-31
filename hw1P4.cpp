#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <math.h>

void isNumeric(std::string num)
{
	int i;
	for(i=0; i < num.size(); i++)
	{
		if( num[i] < '0' || num[i] > '9')
		{
			std::cout << "Error - first line of file is non-numeric." << std::endl;
			exit (EXIT_FAILURE);
		}
	}
}

void checkCharLimit(int charLimit)
{
	if (charLimit > 80)
	{
		std::cout << "Error - File contains line of text over 80 characters" << std::endl;
		exit (EXIT_FAILURE);
	}
}

void checkLineNumber(int currentLine,int expectedLineTotal)
{
	if ( (currentLine) > expectedLineTotal)
	{
		std::cout << "Error - More lines being read than specified in file" << std::endl;
		exit (EXIT_FAILURE);
	}
}

int main (int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cout << "Error - Incorrect number of arguments. When running this program, enter './<name of executable>' followed by only one text file. Eg. somefile.txt." << std::endl;
		exit (EXIT_FAILURE);
	}
	else
	{
		std::ifstream text (argv[1]);
		if ( !text.is_open() )
		{
			std::cout << "Error - Could not open file" << std::endl;
			exit (EXIT_FAILURE);
		}
		else
		{
			char currentChar, prevChar = ' ';
			int lineCount = 0;
			int lineNumber = 0;
			int charLimit = 0;
			int * wordCountArray;
			std::string size;
			bool numRead = false;
			while ( text.get( currentChar ) )
			{
				if (numRead == false)
				{
					if ( currentChar != '\n' )
					{
						//std::cout << currentChar << std::endl;
						size = size + currentChar;
					}
					else 
					{				
						if(size.empty())
						{
							std::cout << "Error - no data in first line" << std::endl;
							exit (EXIT_FAILURE);
						}
						isNumeric(size);
						lineCount = atoi(size.c_str());
						wordCountArray = new int [lineCount];
						numRead = true;
					}
				}
				else
				{
					if ( (currentChar >= 'a' && currentChar <= 'z') || (currentChar >= 'A' && currentChar <= 'Z') )
					{
						if (prevChar == ' ' || prevChar == '\t' || prevChar == '\n')
						{
							wordCountArray[lineNumber]++;
							charLimit++;
							checkCharLimit(charLimit);
						}
						else
						{
							charLimit++;
							checkCharLimit(charLimit);
						}
					}
					else if ( currentChar == ' ' || currentChar == '\t')
					{
						charLimit++;
						checkCharLimit(charLimit);
					}
					else if ( currentChar == '\n' )
					{
						lineNumber++;
					 std::cout << "lineNumber is now " << lineNumber << std::endl;
						checkLineNumber(lineNumber, lineCount);
						charLimit = 0;
					}
					else
					{
						std::cout << "Error - Non-Alphabetical or Whitespace character detected" << std::endl;
						exit (EXIT_FAILURE);
					}

					prevChar = currentChar;
				}
			}
			for (lineNumber=lineNumber; lineNumber >= 0; lineNumber--)
			{
				std::cout << wordCountArray[lineNumber] << std::endl << std::endl;
			}
			delete wordCountArray;
		}
	}
	return 0;
}
