#include "sorting.hpp"
#include "parsing.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>



std::size_t parseTestFile(const char* fileName, unsigned*& array) {
    /*
        Function used to parse the test file:
        
            Format expected : one unsigned integer per line
            args: -fileName, a C-style string containing the name of the file
                  -array, a reference to a pointer. Its value will point to the
                        array of integers parsed from the file once the parsing
                        is done
            return value : the size of the array
    */
    std::vector<unsigned> vect;
    std::ifstream file;
    file.open(fileName);
    char tmp = static_cast<char>(file.get());
    while (!file.eof()) { // for each line until the end of the file
        unsigned value = 0;
        while (!file.eof() && tmp != '\n') { // for each char until the end of the line
            // shifting the decimal numbers one row on the left and adding the new number
            value = value*10 + static_cast<unsigned>(tmp - '0'); // tmp is char, tmp-'0' is its int value
            tmp = static_cast<char>(file.get());
        }
        tmp = static_cast<char>(file.get());
        vect.push_back(value); 
    }
    file.close();
    // converting the dynamic array to a static one
    array = new unsigned[vect.size()];
    for (std::ptrdiff_t i=0; static_cast<std::size_t>(i)<vect.size(); ++i)
    	array[i] = vect[i];
    return vect.size();
}



int main (int argc, char* argv[]) { // argc unused
    /*
        Main function of the program, set the parser, parse the user input and run the mergesort algorithm
    */
    int numThread, executions; // number of threads used, number of executions ran
    float avDuration = 0; // average duration of one execution
    unsigned* array; // the array of integers to sort
    std::size_t length; // the size of the array
    Parser* parser = new Parser(4);


    // --- parsing part ---
    parser->setOption("single",'s',"Using single thread mode",0);
    parser->setOption("multi",'m',"Using multi thread mode",1);
    parser->setOption("file",'f',"File containing the integers to sort",1);
    parser->setOption("executions",'e',"The number of executions to be run",1);
    //TODO : parser.setGroup('s','m');
    //TODO : ajouter un switch "option required/optional" dans le parser

    parser->parse(argv);
    numThread = parser->hasOption('s') ?
            1 :
            parser->getIntOptionValue('m');
    executions = parser->getIntOptionValue('e');
    length = parseTestFile(parser->getStrOptionValue('f'), array);


    // --- Test part ---
    for (std::ptrdiff_t i=0; static_cast<int>(i)<executions; ++i) { // for each execution
        // getting the time
        std::chrono::time_point start = std::chrono::high_resolution_clock::now();
        // running the test
        const unsigned* tmp = mergeSort<unsigned>(array, length, numThread);
        // calculating the ellapsed time and adding to avDuration
        avDuration += static_cast<float>(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now()-start
                ).count()
            );
        // checking if the array is actually sorted (it should not be useful, but better be careful)
        if (!checkArray<unsigned>(tmp, length))
            std::cerr<<("Array not sorted !\n");
    }
    // calculating the average duration (avDuration was the sum of durations of all executions so far)
    avDuration /= static_cast<float>(executions);


    // --- printing results ---
    std::cout<<executions;
    std::cout<<" executions completed\n\tAverage duration of ";
    std::cout<<avDuration;
    std::cout<<" ms\n\t";
    std::cout<<numThread;
    std::cout<<" threads used\n\tArray of ";
    std::cout<<length;
    std::cout<<" integers\n";
}
