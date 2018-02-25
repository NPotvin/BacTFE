#ifndef PARSING_HPP
#define PARSING_HPP

#include "gopt.h"


class Parser {

private:

    int _argc = 0;
    char** _argv = nullptr;
    struct option* _options = nullptr;
    const char** _help = nullptr;
    std::size_t _size = 0;


    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;


    void displayHelp();
    inline char* copyStr(const char*);
    inline void deleteArrays();
    option* findOption(const char&);
    option* findOption(const char*);

    template<typename T>
	char* getStrOptionValue(T optionName) {
	    option* tmp = findOption(optionName);
	    if (tmp)
	        return tmp->argument;
	    return nullptr;
	}
    
    template<typename T>
    int getIntOptionValue(T optionName) {
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
	    option* tmp = findOption(optionName);
	    if (tmp)
	        return static_cast<bool>(tmp->count);
	    return false;
	}

public:

    Parser(const std::size_t& size=0);
    Parser(Parser&& other) {
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

    void setOption(const char*, const char&, const char*, const char& argRequired=2);

	char* getStrOptionValue(const char* arg) {return getStrOptionValue<>(arg);}
	int getIntOptionValue(const char* arg) {return getIntOptionValue<>(arg);}
	bool hasOption(const char* arg) {return hasOption<>(arg);}
	char* getStrOptionValue(const char& arg) {return getStrOptionValue<>(arg);}
	int getIntOptionValue(const char& arg) {return getIntOptionValue<>(arg);}
	bool hasOption(const char& arg) {return hasOption<>(arg);}
    
    void parse(char**);
};


#endif