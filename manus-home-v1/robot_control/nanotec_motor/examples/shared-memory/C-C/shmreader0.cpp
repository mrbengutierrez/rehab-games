

#include "NanotecSharedMemory.h"


// TESTING TESTING TESTING
void printDog(char* dogName, int dogAge)
{
	cout << "printDog, name: " << dogName << ", age: " << dogAge << endl;
}
// TESTING TESTING TESTING
void printCat(char* catName, double catAge = 12.5)
{
	cout << "printCat, name: " << catName << ", age: " << catAge << endl;
}



NanotecSharedMemory::NanotecSharedMemory()
{
	// ftok to generate unique key 
	const int keyValue = 65;
    key_t key = ftok("shmfile",keyValue); 
  
    // shmget returns an identifier in shmid 
    _numBytes = 1024; // number of bytes of the shared memory location
    _shmid = shmget(key,_numBytes,0666|IPC_CREAT); 
  
    // shmat to attach to shared memory 
    _strPointer = (char*) shmat(_shmid,(void*)0,0);
    
    // Write empty character '\0' as the empty sequence
    char emptyChar = '\0';
    char * emptyCharPointer = &emptyChar;
    this->writeMemory(emptyCharPointer); 
    
}

NanotecSharedMemory::~NanotecSharedMemory()
{
	//detach from shared memory  
    shmdt(_strPointer); 
    
    // destroy the shared memory 
    shmctl(_shmid,IPC_RMID,NULL); 
}

 /**
  * Returns a copy of the string in the shared memory location
  */
char* NanotecSharedMemory::readMemory()
{
	char* dataPointer = (char *)malloc(sizeof(char)*_numBytes);
	strcpy( dataPointer, _strPointer );
	return dataPointer; 
}

/** Writes a sequence of characters to the shared memory location
 * 
 * @param sequenceToWrite sequence to write to shared memory location
 */
void NanotecSharedMemory::writeMemory(char* sequenceToWrite)
{
	strcpy(_strPointer,sequenceToWrite);
}

/** Returns the number of delimiters in a char* sequence
 * 
 * @param delimiter delimiter to find
 * @param sequencePointer sequence to find delimiters, 
 * @param sequenceLength length of sequence to check for delimiters
 * 
 */
int NanotecSharedMemory::countDelimiters(const char delimiter, char * const sequencePointer, const int sequenceLength){
	char* currentCharPointer = sequencePointer; // keep track of the current character
	
	int numDelimiters = 0; // initialize number of delimiters to zero
	for(int i=0; i < sequenceLength; i++){ // for each character in sequence
		if ( *currentCharPointer == delimiter) {
			numDelimiters++;  // increment numDelimiters if character is a delimiters
		}
		currentCharPointer++; // increment current character pointer
	}
	return numDelimiters;
}


/** Splits a string based on delimiters and stores splitted elements is a vector
 * Does not modify input string.
 * 
 * @param stringToSplit Pointer to a sequence of characters terminated with '\0'
 * @param delimiter to use to split string
 */
std::vector<std::string> NanotecSharedMemory::splitString(std::string stringToSplit, std::string delimiter)
{
	// initialize return vector for splitted strings
	std::vector<std::string> splittedStringVector;
	
	int curr = 0; 
	int next = stringToSplit.find(delimiter,curr); 
	// string::npos is a constant (probably -1) representing a non-position.
	// It is returned by find when a pattern is not found.
	while (next != string::npos) 
	{ 
		std::string currentString = stringToSplit.substr(curr, next-curr); // Get substring
		splittedStringVector.emplace_back( currentString); // Add substring
		curr = next + delimiter.length();
		next = stringToSplit.find(delimiter, curr); 
	}
	
	// Add last substring
	splittedStringVector.emplace_back( stringToSplit.substr(curr,next) );
	
	return splittedStringVector;	
}

bool NanotecSharedMemory::callFunctionUsingVector(std::vector<std::string> splittedStringVector){
	std::string funcName = splittedStringVector.at(0);
	int numArguments = splittedStringVector.size() - 1;
	cout << "numArguments: " << numArguments << endl; // TESTING
	
	// printDog function call
	if ( funcName.compare("printDog") == 0) {
		std::string dogNameString = splittedStringVector.at(1);
		char* dogName =  &dogNameString[0u];
		
		int dogAge = std::stoi( splittedStringVector.at(2) );
		
		printDog(dogName, dogAge);
		return true;
	}
	
	
	// printCat function call
	if (funcName.compare("printCat") == 0) {
		std::string catNameString = splittedStringVector.at(1);
		char* catName =  &catNameString[0u];
		
		if (numArguments == 1) {
			printCat(catName);
			return true;
		} 
		// numArguments = 2
		double catAge = std::stod( splittedStringVector.at(2) );
			
		printCat(catName,catAge);
		return true;
	}
	
	
	
	return false;
}
	
/** Reads the shared memory location, and executes the instruction located there
 * 
 * @return true if there was a valid instruction in the shared memory location
 *              and the instruction was executed successfully, else return false/
 */
bool NanotecSharedMemory::executeMemory() {
	// Read the memory
	char * const memoryPointer = this->readMemory(); // constant address, variable data
	
	// Parse the memory by splitting into strings
	std::string delimiter = ",";
	std::string stringToSplit = memoryPointer;
	std::vector<std::string> splittedStringVector = NanotecSharedMemory::splitString(stringToSplit, delimiter);
	
	for (int i = 0; i != splittedStringVector.size(); i++)
	{
		cout << "  Split: " << splittedStringVector.at(i) << endl;
		
	}
	
	// executed the parsed instruction
	return NanotecSharedMemory::callFunctionUsingVector(splittedStringVector);
}




int main() 
{ 
    
    NanotecSharedMemory *memObjPointer = new NanotecSharedMemory();
    
    while(true) {
		cout << endl;
		char* dataRead = memObjPointer->readMemory();
		memObjPointer->executeMemory();
		
		
		sleep(2);
	}
	
	delete memObjPointer;
  
    return 0; 
} 

