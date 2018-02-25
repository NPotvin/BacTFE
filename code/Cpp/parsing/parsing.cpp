#include <iostream>
#include "parsing.hpp"


void Parser::displayHelp() {
	// method displaying help (not yet in use)
    std::cout<<"Usage :\n";
    for (std::ptrdiff_t i=0; static_cast<std::size_t>(i)<_size; ++i) {
        std::cout<<" -"<<_options[i].short_name<<",--"<<_options[i].long_name;
        if (_options[i].flags & GOPT_ARGUMENT_REQUIRED)
            std::cout<<"<arg>\t";
        else if (_options[i].flags & GOPT_ARGUMENT_OPTIONAL)
            std::cout<<"[arg]\t";
        else
            std::cout<<"\t\t";
        std::cout<<_help[i]<<"\n";
    }

}

inline char* Parser::copyStr(const char* str) {
	/*
		Method copying string (needed to avoid keeping a pointer to a static
		string defined outside this class and being deleted after an option is set)
    */
    std::ptrdiff_t size = 0;
    for (; str[size] != '\0'; ++size); // counting the number of char in the string
    char* res = new char[static_cast<std::size_t>(size+1)]; // for strings, actual size is size+1 due to null char
    do { // copying
        res[size] = str[size];
        size--;
    }
    while (size>=0);
    return res;
}

inline void Parser::deleteArrays() {
	/*
		Method deleting the arrays, usefull when the number of set option exceed the actual size of the arrays
		or during destruction of an instance of this class
	*/
    for (std::ptrdiff_t i=0; !(_options[i].flags & GOPT_LAST); ++i) {
    	// deleting dynamic arrays (C-strings) inside of option and _help
        delete _help[i];
        _help[i] = nullptr;
        delete _options[i].long_name;
        _options[i].long_name = nullptr;
    }
    // deleting arrays
    delete[] _help;
    delete[] _options;
    _help = nullptr;
    _options = nullptr;
}

option* Parser::findOption(const char& optionName) {
	// generic getter to find an option, used in all other getters (short name is used here)
    for (std::ptrdiff_t i=0; static_cast<std::size_t>(i)<_size; ++i)
        if (_options[i].short_name == optionName)
            return &_options[i];
    return nullptr;
        
}

option* Parser::findOption(const char* optionName) {
	// generic getter to find an option, used in all other getters (long name is used here)
    for (std::ptrdiff_t i=0; static_cast<std::size_t>(i)<_size; ++i) {
        bool isEqual = true; // used to compare strings (while it is true, the strings are the same)
        for (std::ptrdiff_t j=0; isEqual && (optionName[j]!='\0' || _options[i].long_name[j]!='\0'); ++j)
            isEqual = optionName[j] == _options[i].long_name[j] && optionName[j]!='\0' && _options[i].long_name[j]!='\0';
        // now comparison is finished, checking the flag to see if the two were the same
        if (isEqual)
            return &_options[i];
    }
    return nullptr;
}


Parser::Parser(const std::size_t& size) {
	// basic constructor, gopt needs kind of "null terminated" option array, so the _options is one slot longer
    _options = new struct option [size+1];
    _help = new const char* [size];
    for (std::ptrdiff_t i=0; static_cast<std::size_t>(i)<size+1; ++i) {
        _options[i].flags = 0;
        _options[i].argument = nullptr;
    }
    _options[size].flags = GOPT_LAST;
}

Parser::~Parser() {
	// Destructor of this class, the important work is made by deleteArrays()
    _argv = nullptr;
    deleteArrays();
}

void Parser::setOption(const char* longName, const char& shortName, const char* help, const char& argRequired) {
	/*
		Method used to set a new option in the parser.

		args : -longName : a C-string it is the word used by the user in the format --[NAME] to pass args to the program
			   -shortName : a char used by the user in the format -[CHAR] to pass args to the program
			   -help : the line printed in the help about this option, it should tell what this option is for
			   -argRequired : three possible values, either an arg is required after the flag (-e example) or optional or not allowed
	*/
    if (_options[_size].flags & GOPT_LAST) {
    	// in this case, the arrays are too short and need to be reallocated one slot longer
        struct option* newOpt = new struct option [_size+2];
        const char** newHelp = new const char* [_size+1];
        for (std::ptrdiff_t i=0; static_cast<std::size_t>(i)<_size; ++i) {
        	// copying existing options and help
            newOpt[i] = _options[i];
            newHelp[i] = _help[i];
            // avoiding the copied strings to be deleted by setting their pointers to null
            _help[i] = nullptr;
            _options[i].long_name = nullptr;
        }
        deleteArrays(); // deleting old arrays
        // setting the new ones
        _options = newOpt;
        _help = newHelp;
        // setting the new "null option" for gopt to be happy
        _options[_size+1].flags = GOPT_LAST;
        _options[_size].flags = _options[_size].flags & ~GOPT_LAST;
    }

    // copying strings and setting the necessary flags
    _help[_size] = copyStr(help);
    _options[_size].long_name = copyStr(longName);
    _options[_size].short_name = shortName;
    _options[_size].flags = 0;
    _options[_size].argument = nullptr;
    switch (argRequired) {
        case 0:
            _options[_size].flags = _options[_size].flags | GOPT_ARGUMENT_FORBIDDEN;
            break;
            
        case 1:
            _options[_size].flags = _options[_size].flags | GOPT_ARGUMENT_REQUIRED | GOPT_ARGUMENT_NO_HYPHEN;
            break;
            
        //case 2:
        default:
            _options[_size].flags = _options[_size].flags | GOPT_ARGUMENT_OPTIONAL;
            //break;

        //default:
            //throw error
    }
    _size++; // incrementing the number of options
}

void Parser::parse(char** argv) {
	// parsing method, just using the two gopt functions
    _argv = argv;
    _argc = gopt(_argv, _options);
    gopt_errors(_argv[0], _options);
}