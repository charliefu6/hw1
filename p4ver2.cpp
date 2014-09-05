#include <iostream>
#include <string>
#include <sstream>
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

void isWord(std::string word)
{
	int i;
	for(i=0; i <word.size(); i++)
	{
		if( !(word[i] >= 'a' && word[i] <= 'z') && !(word[i] >= 'A' && word[i] <= 'z') )
		{
			std::cout << "Error- Nonletter character detected." << std::endl;
			exit (EXIT_FAILURE);
		}
	}
} 

void checkLineSize(std::string line)
{
	if (line.size() > 80)
	{
		std::cout << "Error - File contains line of text over 80 characters" << std::endl;
		exit (EXIT_FAILURE);
	}
}

void checkLineNumber(int currentLine,int expectedLineTotal)
{
	if ( (currentLine) >= expectedLineTotal)
	{
		std::cout << "Error - More lines being read than specified in file" << std::endl;
		exit (EXIT_FAILURE);
	}
}

void getLineCount(std::string line, int *lineCount)
{
	std::istringstream iss;
	std::string numGetter;
	iss.str(line);
	iss >> numGetter;
	if(numGetter.empty())
	{
		std::cout << "Error - First line of data is empty." << std::endl;
		exit(EXIT_FAILURE);
	}
	isNumeric(numGetter);
	*lineCount=atoi(numGetter.c_str());
	//std::cout << *lineCount << std::endl;
	numGetter.erase(numGetter.begin(), numGetter.end());
	iss >> numGetter;
	if (!(numGetter.empty()) )
	{
		std::cout << "Error - Too many arguments in first line." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void countWords( std::string line, int *wordArray, int lineNumber )
{
	std::istringstream iss;
	std::string wordGetter;
	iss.str(line);
	do{
		wordGetter.erase(wordGetter.begin(),wordGetter.end() );
		iss >> wordGetter;
		if (!(wordGetter.empty()) )
		{
			isWord(wordGetter);
			wordArray[lineNumber]++;
		}
	}while(!(wordGetter.empty()) );
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
			std::string line;
			int lineCount = 0;
			int lineNumber = 0;
			int * wordCountArray;
			int i;
			bool numRead = false;
			while ( getline( text, line ) )
			{
				if (numRead == false)
				{
					getLineCount(line, &lineCount);
					wordCountArray = new int [lineCount];
					for(i = 0; i < lineCount; i++)
					{
						wordCountArray[i] = 0;
					}
					numRead = true;
				}
				else
				{
					checkLineSize(line);
					checkLineNumber( lineNumber, lineCount );
					countWords(line, wordCountArray, lineNumber);
					lineNumber++;
				}
			}
			if(lineNumber != lineCount)
			{
				std::cout << "Finished reading lines. Number of lines read does not match number of lines specified. Continuing program regardless." << std::endl;
			}
			for (i=lineNumber-1; i >= 0; i--)
			{
				std::cout << wordCountArray[i] << std::endl;
				if (i != 0)
				{
					std::cout << std::endl;
				}
			}
		delete []wordCountArray;
		}
	}
	return 0;
}
