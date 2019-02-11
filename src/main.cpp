#ifndef MAJOR_VERSION_NUMBER
#define MAJOR_VERSION_NUMBER 0
#endif

#ifndef MINOR_VERSION_NUMBER
#define MINOR_VERSION_NUMBER 0
#endif


#include <iostream>

int main(int argc, char const *argv[])
{
    std::cout << "Version" << std::endl;
    std::cout << MAJOR_VERSION_NUMBER << "." << MINOR_VERSION_NUMBER << std::endl;
    return 0;
}
