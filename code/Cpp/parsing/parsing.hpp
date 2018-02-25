#ifndef PARSING_HPP
#define PARSING_HPP

#include "gopt.h"


class Parser {
	/*
		Parser class built above the C gopt library.

		Used to parse user command line input more or less the way a java parser would do it
		
		It bahaves like a dynamic object, but underlying are static arrays, meaning that it is better
		to set the actual size once and for all at the construction

		TODO : implement the -h switch, manage the help and display it, add a "required" bool in set option
	*/

private:

	// argc and argv from passed to the main method
    int _argc = 0;
    char** _argv = nullptr;
    struct option* _options = nullptr; // an option array representing possible passed options
    const char** _help = nullptr; // an array of C-style string containig the description of each option (displayed in help)
    std::size_t _size = 0; // the size of the options array


    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;


    void displayHelp();
    inline char* copyStr(const char*); // simple method to copy a C-style string
    inline void deleteArrays(); // used to delete arrays (used in destructor and when a reallocation is needed)
    option* findOption(const char&); //base method used to get an option (works with a letter here)
    option* findOption(const char*); //base method used to get an option (works with a string here)

    template<typename T>
	const char* getStrOptionValue(T optionName) {
		/*
			Template option getter
			returns the C-style string containing the argument passed by the user
		*/
	    option* tmp = findOption(optionName);
	    if (tmp)
	        return tmp->argument;
	    return nullptr;
	}
    
    template<typename T>
    int getIntOptionValue(T optionName) {
		/*
			Template option getter
			returns an integer passed by the user

			IMPORTANT : no error checking, if the value was not an integer, the result might not mean anything
			TODO : checking user input and managing negative values
		*/
	    option* tmp = findOption(optionName);
	    if (tmp) {
	        int count = 0;
	        char* charPtr = tmp->argument;
	        while (*(charPtr) != '\0')
	            count = count*10 + static_cast<int>(*(charPtr++) - '0');
	        return count;
	    }
	    return -1;
	}

    template<typename T>
	bool hasOption(T optionName) {
		/*
			Template getter used to check if an option has been passed by the user or not
		*/
	    option* tmp = findOption(optionName);
	    if (tmp)
	        return static_cast<bool>(tmp->count);
	    return false;
	}

public:

    Parser(const std::size_t& size=0);
    Parser(Parser&& other) {
    	// transfer constructor
	    _argc = other._argc;
	    _argv = other._argv;
	    _options = other._options;
	    _help = other._help;
	    _size = other._size;
	    other._argv = nullptr;
	    other._options = nullptr;
	    other._help = nullptr;
    }

    ~Parser();
    
    Parser& operator=(Parser&& other) {
    	// transfer operator
	    _argc = other._argc;
	    _argv = other._argv;
	    _options = other._options;
	    _help = other._help;
	    _size = other._size;
	    other._argv = nullptr;
	    other._options = nullptr;
	    other._help = nullptr;
	    return *this;
    }

    // method used to add an option (see cpp file for more info)
    void setOption(const char*, const char&, const char*, const char& argRequired=2);

    // specializations of the template getters
	const char* getStrOptionValue(const char* arg) {return getStrOptionValue<>(arg);}
	int getIntOptionValue(const char* arg) {return getIntOptionValue<>(arg);}
	bool hasOption(const char* arg) {return hasOption<>(arg);}
	const char* getStrOptionValue(const char& arg) {return getStrOptionValue<>(arg);}
	int getIntOptionValue(const char& arg) {return getIntOptionValue<>(arg);}
	bool hasOption(const char& arg) {return hasOption<>(arg);}
    
    // method parsing the input (argv)
    void parse(char**);
};


#endif