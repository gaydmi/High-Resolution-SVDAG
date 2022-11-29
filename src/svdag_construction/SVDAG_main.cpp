#include <iostream>     
#include <fstream>     
#include <ctime>

#include "SVDAG.h"

std::string filename;

void print_help() 
{
    std::cout << "Example: svdag_constructor -f ../models/bunny.octree" << endl;
    std::cout << "" << endl;
    std::cout << "All available program options:" << endl;
    std::cout << "" << endl;
    std::cout << "-f <filename.octree>     Path to a .octree input file." << endl;
    std::cout << "-h                       Print help and exit." << endl;
}

void print_invalid() 
{
    std::cout << "Not enough or invalid arguments, please try again." << endl;
    std::cout << "At the bare minimum, I need a path to a .octree file" << endl << "" << endl;
    print_help();
}

void parse_program_parameters(int argc, char* argv[]) 
{
    cout << "Reading program parameters ..." << endl;
    // Input argument validation
    if (argc < 3) 
    {
        print_invalid();
        exit(0);
    }
    for (int i = 1; i < argc; i++) 
    {
        // Parse filename
        if (string(argv[i]) == "-f") 
        {
            filename = argv[i + 1];
            std::cout << filename << std::endl;
            size_t check_tri = filename.find(".octree");
            if (check_tri == string::npos) 
            {
                cout << "Data filename does not end in .octree - I only support that file format" << endl;
                print_invalid();
                exit(0);
            }
            i++;
        }
        else if (string(argv[i]) == "-h") 
        {
            print_help(); exit(0);
        }
        else 
        {
            print_invalid(); exit(0);
        }
    }
}


int main (int argc, char *argv[]) 
{

    parse_program_parameters(argc, argv);

    std::clock_t start;
    double duration;

    // DAG construction
    start = std::clock();
    SVDAG SVDAG(filename.substr(0, filename.size()-7));
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"Construct DAG: "<< duration << "s" << '\n';

    // DAG reduction
    start = std::clock();
    SVDAG.reduce();
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"Reduce DAG: "<< duration << "s" << '\n';

    // Write the DAG into file and info to the output stream
    SVDAG.print_SVDAG_info();
    SVDAG.write_SVDAG();

    return 0;
}