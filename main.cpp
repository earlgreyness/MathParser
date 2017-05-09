#include "MathParser.h"
#include <iostream>
#include <string>
#include <ctime>
//#include <cstdlib>
//#include <conio.h>
//#include <vector>

int main()
{
    MathParser parser("(2 + 3) * 2 / 10 - 1 / ((7 - 3) / (4 + 4))");

    //
    char* line = new char[1024];
    while (true)
	{
        std::cout << "Enter expression:\n";
        std::cin.getline(line, 256);
        if (std::string(line) == "quit" || std::string(line) == "exit")
		{
            break;
        }
        parser.compile(line);
        if (parser.isSuccess())
		{
			std::cout << "TRUE ";
        }
		else
		{
            std::cout << "SYNTAX ERROR ";
        }
        std::cout.precision(5);
        std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
        //std::cout.setf(std::ios_base::left, std::ios_base::adjustfield);
        std::cout << "VALUE = " << parser.evaluate(1.0) << "\n\n";
    }
    //*/

	while (true)
	{
        std::clock_t tStart = std::clock();
        for (int i = 0; i < 1000000; ++i)
		{
            parser.evaluate(i);
	    }
        std::cout << "Delta time = " << static_cast<double>(std::clock() - tStart) / CLOCKS_PER_SEC << std::endl;
    }

    return 0;
}
