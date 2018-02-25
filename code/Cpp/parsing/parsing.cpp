#include <iostream>
#include "parsing.hpp"


void Parser::displayHelp() {
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
    std::ptrdiff_t size = 0;
    for (; str[size] != '\0'; ++size);
    char* res = new char[static_cast<std::size_t>(size+1)]; // for strings, actual size is size+1 due to null char
    do {
        res[size] = str[size];
        size--;
    }
    while (size>=0);
    return res;
}

inline void Parser::deleteArrays() {
    for (std::ptrdiff_t i=0; !(_options[i].flags & GOPT_LAST); ++i) {
        delete _help[i];
        _help[i] = nullptr;
        delete _options[i].long_name;
        _options[i].long_name = nullptr;
    }
    delete[] _help;
    delete[] _options;
    _help = nullptr;
    _options = nullptr;
}

option* Parser::findOption(const char& optionName) {
    for (std::ptrdiff_t i=0; static_cast<std::size_t>(i)<_size; ++i)
        if (_options[i].short_name == optionName)
            return &_options[i];
    return nullptr;
        
}

option* Parser::findOption(const char* optionName) {
    for (std::ptrdiff_t i=0; static_cast<std::size_t>(i)<_size; ++i) {
        bool isEqual = true;
        for (std::ptrdiff_t j=0; isEqual && (optionName[j]!='\0' || _options[i].long_name[j]!='\0'); ++j)
            isEqual = optionName[j] == _options[i].long_name[j] && optionName[j]!='\0' && _options[i].long_name[j]!='\0';
        if (isEqual)
            return &_options[i];
    }
    return nullptr;
}


Parser::Parser(const std::size_t& size) {
    _options = new struct option [size+1];
    _help = new const char* [size];
    for (std::ptrdiff_t i=0; static_cast<std::size_t>(i)<size+1; ++i) {
        _options[i].flags = 0;
        _options[i].argument = nullptr;
    }
    _options[size].flags = GOPT_LAST;
}

Parser::~Parser() {
    _argv = nullptr;
    deleteArrays();
}

void Parser::setOption(const char* longName, const char& shortName, const char* help, const char& argRequired) {
    if (_options[_size].flags & GOPT_LAST) {
        struct option* newOpt = new struct option [_size+2];
        const char** newHelp = new const char* [_size+1];
        for (std::ptrdiff_t i=0; static_cast<std::size_t>(i)<_size; ++i) {
            newOpt[i] = _options[i];
            newHelp[i] = _help[i];
            _options[i].long_name = nullptr;
        }
        deleteArrays();
        _options = newOpt;
        _help = newHelp;
        _options[_size+1].flags = GOPT_LAST;
        _options[_size].flags = _options[_size].flags & ~GOPT_LAST;
    }

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
    _size++;
}

void Parser::parse(char** argv) {
    _argv = argv;
    _argc = gopt(_argv, _options);
    gopt_errors(_argv[0], _options);
}