#include <cstring>
#include <iostream>

#include "fixed_point.hpp"

int main(int argc, char *argv[]) {
    if (argc == 4) {
        int rounding_format;
        try {
            rounding_format = std::stoi(argv[2]);
        }
        catch (...) {
            std::cerr << "wrong rounding format" << std::endl;
            return 1;
        }
        if (rounding_format < 0 || rounding_format > 3) {
            std::cerr << "wrong rounding format" << std::endl;
            return 1;
        }
        fixed_point_number number(argv[3], argv[1], rounding_format);
        number.print();
    }
    else if (argc == 6) {
        int rounding_format;
        try {
            rounding_format = std::stoi(argv[2]);
        }
        catch (...) {
            std::cerr << "wrong rounding format" << std::endl;
            return 1;
        }
        if (rounding_format < 0 || rounding_format > 3) {
            std::cerr << "wrong rounding format" << std::endl;
            return 1;
        }
        fixed_point_number num1(argv[3], argv[1], rounding_format);
        fixed_point_number num2(argv[5], argv[1], rounding_format); 
        if (strcmp(argv[4], "+") == 0)
            num1 += num2;
        else if (strcmp(argv[4], "-") == 0)
            num1 -= num2;
        else if (strcmp(argv[4], "*") == 0)
            num1 *= num2;
        else if (strcmp(argv[4], "/") == 0)
	          num1 /= num2;
        else {
            std::cerr << "wrong operation" << std::endl;
            return 1;
        }
        num1.print();
    }
    else {
        std::cerr << "not enough args" << std::endl;
    }
    return 0;
}

