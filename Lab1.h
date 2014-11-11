#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <istream>
#include <sstream>
#include <fstream>
#include <string>
using namespace std;


enum errs{ Success, ReadError, IncorrectArgumentNumber, FormatError };

int reader(vector<string> & strs, char * c, int& i);

void usage(char * c);