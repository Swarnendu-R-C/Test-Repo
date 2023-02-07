#include <iostream>

int main (int argc, char** argv)
{
    std::cout << "================<Main Program started>==================" << std::endl << std::endl;

    if (argc < 2)
        std::cout << "No argument passed with the program " << argv[0] << std::endl;
    std::cout << "================<Main Program exited>==================" << std::endl << std::endl;

    return 0;
}