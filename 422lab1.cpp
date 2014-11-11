// 422lab1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Lab1.h"
#include <string.h>

//helper method for reading a file and storing the strings in a vector of strings
int reader(vector<string> & strs, char * c, unsigned int& i){
	ifstream infile(c);

	if (!infile){
		cout << "File Read Error";
		return errs::ReadError;
	}
	else {
		while (!infile.eof()){
			string temps;
			infile >> temps;
			if (temps.length() > i) {
				i = temps.length();
			}
			strs.push_back(temps);
		}
		return errs::Success;
	}

}

//usage function to guide the user on the usage of the program.
void usage(char * c){
	cout << "Incorrect Program Usage!" << endl
		<< "Program Executed: " << c << endl
		<< "The Correct usage must be one of the following:" << endl << endl
		<< "(1) Program Name - Word to Search - File to Search" << endl << endl
		<< "(2) Program Name - -f (flag to indicate dictionary file) - Dictionary File - File to Search" << endl << endl
		<< "(3) Program Name - -f (flag to indicate dictionary file) - Dictionary File - -w (flag to indicate file for write) - File to Write - File to Search" << endl;
}

int main(int argc, char * argv[])
{
	//1st case. Most simple usage
	if (argc == 3){
		vector<string> results;
		unsigned int longest = 0;
		if (reader(results, argv[2], longest) == Success){
			//cout << "longest string is: " << longest << endl;
			char * token;
			char * nxtToken;
			int count = 0;
			string wordToSearch;
			istringstream argIss(argv[1]);
			argIss >> wordToSearch;
			vector<string> lineWords;
			
			for (int i = 0; i < results.size(); i++){
				int lineCheck = 0;
				char * c = NULL;
				for (char& c : results.at(i)){
					//check for a non alphabet
					//isalpha did not work for ’ which seems to be a different symbol compared to '
					if (c == ',' || c == '.' || c == ' / ' || c == ' ? ' || c == '\'' || c == '"' || c == ';' || c == '-' || c == '!' || c == '’' || c == ':' || c == '—'){
						lineCheck++;
					}
				}
				//if a word string is not entirely alphabets
				if (lineCheck != 0){
					char temp[100];
					istringstream iss(results.at(i));
					iss >> temp;
					token = strtok_s(temp, ",./?'!;:—’-\"", &nxtToken);
					//while there is a token
					while (token != NULL){
						string tTemp;
						istringstream tIss(token);
						tIss >> tTemp;
						//if the token matches the word that is being searched increase the count
						if (tTemp == wordToSearch){
							count++;
						}
						lineWords.push_back(tTemp);
						//next token
						token = strtok_s(NULL, ".,/?;!’:—'-\"", &nxtToken);
					}
					//flush istream
					iss.str("");
				}
				else {
					string sTemp;
					string aTemp;
					istringstream iss(results.at(i));
					iss >> sTemp;
					if (sTemp == wordToSearch){
						count++;
						iss.str("");
					}
				}
				
			}
			//prints the result
			cout << wordToSearch << " , " << count << endl;
		}
		else {
			return ReadError;
		}
	}
	// 2nd case with dictionary file
	else if (argc == 4){
		string flag;
		istringstream flagIss(argv[1]);
		flagIss >> flag;
		// check if the format is correct
		if (flag == "-f"){
			vector<string> dictResult;
			vector<string> fileResult;
			vector<string> wordResult;
			unsigned int longest = 0;
			//if the dictionary file read is a success
			if (reader(dictResult, argv[2], longest) == Success){
				//and if the file to search was successfully read
				if (reader(fileResult, argv[3], longest) == Success){
					//for all strings read in the file to search
					for (int i = 0; i < fileResult.size(); i++){
						int lineCheck = 0;
						char * c = NULL;
						// for each character in the string. Isalpha could not be used due to ’ being out of bounds, or an unrecognized character.
						for (char& c : fileResult.at(i)){
							//cout << "results at (" << i << "): " << results.at(i) << endl;
							if (c == ',' || c == '.' || c == ' / ' || c == ' ? ' || c == '\'' || c == '"' || c == ';' || c == '-' || c == '!' || c == '’' || c == ':' || c == '—'){
								//increment the count
								lineCheck++;
							}
						}
						//if there is at least one symbol
						if (lineCheck != 0){
							char temp[100];
							char * token;
							char * nxtToken;
							//wrapping and extracting the string into a char array to be used to extract tokens from
							istringstream iss(fileResult.at(i));
							iss >> temp;
							token = strtok_s(temp, ",./?';:-!’-\"", &nxtToken);
							while (token != NULL){
								string tTemp;
								istringstream tIss(token);
								tIss >> tTemp;
								wordResult.push_back(tTemp);
								token = strtok_s(NULL, ",./?'!:-’;-\"", &nxtToken);
							}
							//flush istream
							iss.str("");
						}
						else {
							string sTemp;
							istringstream iss(fileResult.at(i));
							iss >> sTemp;
							wordResult.push_back(sTemp);
						}
					}
					vector<int> wordCounts;
					//for each word in the dictionary file
					for (int o = 0; o < dictResult.size(); o++){
						int count = 0;
						//search each word that is in the result vector from file-to-search
						for (int k = 0; k < wordResult.size(); k++){
							if (dictResult.at(o) == wordResult.at(k)){
								count++;
							}
						}
						//store the count result in the wordcount vector
						wordCounts.push_back(count);
					}
					
					// for printing purposes
					if (dictResult.size() == wordCounts.size()){
						for (int o = 0; o < dictResult.size(); o++){
							cout << dictResult.at(o) << " , " << wordCounts.at(o) << endl;
						}
					}
					else{
						cout << "something went wrong" << endl;
					}

				}

				else {
					return ReadError;
				}
			}
			else {
				return ReadError;
			}
		}
		else{
			usage(argv[0]);
			return FormatError;
		}
	}
	//3rd case with dictionary file & output file
	else if (argc == 6){
		string flag;
		string fFlag;
		istringstream flagIss(argv[1]);
		istringstream flag2Iss(argv[3]);
		flagIss >> flag;
		flag2Iss >> fFlag;
		// check if the format is correct with the flags
		if (flag == "-f" && fFlag == "-w") {
			vector<string> dictResult;
			vector<string> fileResult;
			vector<string> wordResult;
			unsigned int longest = 0;
			//if dictionary file was successfully read
			if (reader(dictResult, argv[2], longest) == Success){
				//if file-to-search was successfully read
				if (reader(fileResult, argv[5], longest) == Success){
					for (int i = 0; i < fileResult.size(); i++){
						int lineCheck = 0;
						char * c = NULL;
						// for all characters in the string
						for (char& c : fileResult.at(i)){
							//checking if there are any symbols in the string. Isalpha was not used due to ’ not being recognized as a character within range.
							if (c == ',' || c == '.' || c == ' / ' || c == ' ? ' || c == '\'' || c == '"' || c == ';' || c == '-' || c == '!' || c == '’' || c == ':' || c == '—'){
								lineCheck++;
							}
						}
						// if there is at least one symbol
						if (lineCheck != 0){
							char temp[100];
							char * token;
							char * nxtToken;
							istringstream iss(fileResult.at(i));
							iss >> temp;
							token = strtok_s(temp, ",./?':-;-!’\"", &nxtToken);
							//retrieves the tokens using strtok_s
							while (token != NULL){
								string tTemp;
								istringstream tIss(token);
								tIss >> tTemp;
								wordResult.push_back(tTemp);
								token = strtok_s(NULL, ",./:-?!’';-\"", &nxtToken);
							}
							//flush istream
							iss.str("");
						}
						else {
							string sTemp;
							istringstream iss(fileResult.at(i));
							iss >> sTemp;
							wordResult.push_back(sTemp);
						}
					}
					vector<int> wordCounts;
					//storing results into the new vector of wordCounts
					for (int o = 0; o < dictResult.size(); o++){
						int count = 0;
						for (int k = 0; k < wordResult.size(); k++){
							//if the string searched is equal to the wordToSearch
							if (dictResult.at(o) == wordResult.at(k)){
								count++;
							}
						}
						wordCounts.push_back(count);
					}
					
					//puts the file-to-write in a outputfile stream
					ofstream outputFile(argv[4]);

					//writing to the file specified to be written to
					if (dictResult.size() == wordCounts.size()){
						for (int o = 0; o < dictResult.size(); o++){
							string toWrite;
							int counted;
							istringstream converting(wordCounts.at(o));
							converting >> counted;
							toWrite = dictResult.at(o) + " , ";
							cout << toWrite << endl;
							outputFile << toWrite;
							outputFile << wordCounts.at(o);
							outputFile << endl;
						}
					}
					else{
						cout << "something went wrong" << endl;
					}



				}

				else {
					return ReadError;
				}
			}
			else {
				return ReadError;
			}
		}
		else{
			usage(argv[0]);
			return FormatError;
		}

	}
	else {
		usage(argv[0]);
		return IncorrectArgumentNumber;
	}
}

