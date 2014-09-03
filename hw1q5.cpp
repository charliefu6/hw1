#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>

void isNumeric(std::string num)
{
	int i;
	for(i=0; i < num.size(); i++)
	{
		if( num[i] < '0' || num[i] > '9')
		{
			std::cout << "Error - An expected value is Non-Numeric." << std::endl;
			exit (EXIT_FAILURE);
		}
	}
}

void isCommand(std::string command)
{
	if( command != "PUT" && command != "EMPTY" && command != "OUTPUT" )
	{
		std::cout << "Error - Invalid command." << std::endl;
		exit (EXIT_FAILURE);
	}
}

void checkLineNumber(int currentLine,int expectedLineTotal)
{
	if ( currentLine > expectedLineTotal)
	{
		std::cout << "Error - More lines being read than specified in file" << std::endl;
		exit (EXIT_FAILURE);
	}
}

void put(int * bucket, int size, int* numbers)
{
	int i;
	for( i=0; i<size; i++ )
	{
		bucket[i] = numbers [i];
		//std::cout << bucket[i] << std::endl;
	}
}

void empty(int * bucket, int * size)
{
	delete bucket;
	*size = 0;
}

void output(int * bucket, int size)
{
	int i;
	for( i=0; i<size; i++)
	{
		std::cout << bucket[i];
		if( !(i==size-1) )
		{
			std::cout << " ";
		}
	}
	std::cout << std::endl;
}

bool entryCountNonZero(int entryNumber)
{
	if(entryNumber == 0)
	{
		std::cout << "Specified 0 entry input for PUT command - no action taken" << std::endl;
		return false;
	}
	else return true;
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
			std::istringstream iss;
			int numOfBuckets = 0;
			int ** bucketArray;
			int * sizeOfBucket;
			int numOfLines = 0;
			int lineNumber = 0;
			bool fLineRead = false;
			bool *isEmpty;
			
			while (getline(text, line))
			{
				checkLineNumber(lineNumber, numOfLines);
				iss.str(line);
				int i;
				if( fLineRead == false )
				{
					for( i=0; i<3; i++)
					{
						std::string numGetter;
						iss >> numGetter;
						if( i != 2 && numGetter.empty() )
						{
							std::cout << "Error - Too few startup arguments" <<  std::endl;
							exit (EXIT_FAILURE);
						}
						else if ( i !=2 && !(numGetter.empty()) )
						{
							isNumeric(numGetter);
							if ( i == 0 )
							{
								numOfBuckets = atoi(numGetter.c_str());
								//std::cout << numOfBuckets << std::endl;
							}
							if ( i == 1 )
							{
								numOfLines = atoi(numGetter.c_str());
								//std::cout << numOfLines << std::endl;
							}
						}
						else if ( i == 2 && !(numGetter.empty()) )
						{
							std::cout << "Error - Too many startup arguments" << std::endl;
							exit (EXIT_FAILURE);
						}
												
					}
					//std::cout << numOfBuckets << " " << numOfLines << std::endl;
					bucketArray = new int * [numOfBuckets];
					sizeOfBucket = new int [numOfBuckets];
					isEmpty = new bool [numOfBuckets];
					for (i=0; i<numOfBuckets; i++)
					{
						isEmpty[i] = true;
						sizeOfBucket[i] = 0;
					}
					fLineRead = true;
				}
				else
				{
					std::string command;
					std::string numGetter;
					int whichBucket;
					iss >> command;
					isCommand(command);
					if (command != "PUT")
					{
						iss >> numGetter;
						if(numGetter.empty())
						{
							std::cout << "Error - command line incomplete." << std::endl;
							exit(EXIT_FAILURE);
						}
						isNumeric(numGetter);
						whichBucket=atoi(numGetter.c_str());
						if (whichBucket <= 0 || whichBucket > numOfBuckets)
						{
							std::cout << "Error, bucket " << whichBucket << " does not exist." << std::endl;
						}
						else
						{
							//std::cout<< numGetter << std::endl;
							if(command == "OUTPUT")
							{
								if ( isEmpty[whichBucket-1]==true )
								{
									std::cout << "Empty." << std::endl;
								}
								else
								{
									output(bucketArray[whichBucket-1], sizeOfBucket[whichBucket-1]);
								}
							}
							else
							{
								if ( isEmpty[whichBucket-1]==true )
								{
									std::cout << "Bucket " << whichBucket << " is already empty." << std::endl;
								}
								else
								{
									empty(bucketArray[whichBucket-1], &sizeOfBucket[whichBucket-1]);
									isEmpty[whichBucket-1]=true;
									//std::cout << sizeOfBucket[whichBucket-1] << std::endl;
								}
							}
							//else *implement command for empty later
							numGetter.erase( numGetter.begin(), numGetter.end() );
							iss >> numGetter;
							if ( !(numGetter.empty()) )
							{
								std::cout << "Error - extra data read in command lines." << std::endl;
								exit(EXIT_FAILURE);
							}
						}

					}
					else
					{
						iss >> numGetter;
						isNumeric(numGetter);
						whichBucket=atoi(numGetter.c_str());
						if (whichBucket <= 0 || whichBucket > numOfBuckets)
						{
							std::cout << "Error, bucket " << whichBucket << " does not exist." << std::endl;
						}
						else
						{
							int numOfEntries;
							int *entryValue;
							iss >> numGetter;
							isNumeric(numGetter);
							numOfEntries = atoi(numGetter.c_str());
							entryValue = new int[numOfEntries];
							int i;
							if( entryCountNonZero(numOfEntries) )
							{
								for ( i=0; i<numOfEntries; i++)
								{
									std::string valGetter;
									iss >> valGetter;
									if ( valGetter.empty() )
									{
										std::cout << "Error - less arguments given than specified for PUT in command line." << std::endl;
										exit(EXIT_FAILURE);
									}
									isNumeric(valGetter);
									entryValue[i] = atoi( valGetter.c_str() );
									//std::cout << entryValue[i] << " ";
								}
								if ( isEmpty[whichBucket-1] == false )
								{
									std::cout << "Error - bucket " << whichBucket << " is not empty" << std::endl;
								}
								else if ( isEmpty[whichBucket-1] == true )
								{
									bucketArray[whichBucket-1] = new int [numOfEntries];
									put(bucketArray[whichBucket-1], numOfEntries, entryValue);
									sizeOfBucket[whichBucket-1] = numOfEntries;
									isEmpty[whichBucket-1] = false;
								}
								numGetter.erase( numGetter.begin(), numGetter.end() );
								iss >> numGetter;
								if ( !(numGetter.empty()) )
								{
									std::cout << "Error - extra data read in command lines." << std::endl;
									exit(EXIT_FAILURE);
								}
								//std::cout << sizeOfBucket[whichBucket-1] << std::endl;
							}
						}
					}
					//std::cout << line << std::endl;
				}
				iss.clear();
				lineNumber++;
				//std::cout << lineNumber << " " << numOfLines << std::endl;
			}
			if (lineNumber-1 != numOfLines)
			{
				std::cout << "All lines read. However, number of lines read is not equal to number of lines expected." << std::endl;
			}
			int i;
			for (i=0; i<numOfBuckets; i++)
			{
				if(isEmpty[i]==false)
				{
					delete bucketArray[i];
				}
			}
			delete isEmpty;
			delete sizeOfBucket;
			delete bucketArray;
		}
	}
	return 0;
}
		
