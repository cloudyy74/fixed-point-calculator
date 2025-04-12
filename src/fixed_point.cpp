#include "fixed_point.hpp"

#include <iostream>
#include <iomanip>

fixed_point_number::fixed_point_number(const std::string &input, const std::string &format, int rounding_format) : rounding_format(rounding_format) {
    auto dot_pos = format.find('.');
    if (dot_pos == std::string::npos) {
        std::cerr << "wrong a.b format" << std::endl;
        exit(1);
    }
    try {
        a = std::stoi(format.substr(0, dot_pos));
        b = std::stoi(format.substr(dot_pos + 1));
        decimal = std::stoll(input, nullptr, 16);
    }
    catch (...) {
        std::cerr << "wrong a.b format or number" << std::endl;
        exit(1); 
    }
    if (a + b > 32 || a < 1 || b < 0) {
        std::cerr << "wrong a.b format" << std::endl;
        exit(1);
    }
    modular_arithmetic();
}

void fixed_point_number::modular_arithmetic() {
    long long mask = (1LL << (a + b)) - 1;
    decimal &= mask;
    if (decimal > (1LL << (a + b - 1)) - 1)
        decimal -= (1LL << (a + b));
}

void fixed_point_number::print() {
    if (rounding_format == 0)
        print_toward_zero();
    else if (rounding_format == 1)
        print_toward_nearest_even();
    else if (rounding_format == 2)
        print_toward_pos_infinity();
    else if (rounding_format == 3)
        print_toward_neg_infinity();
}

void fixed_point_number::print_toward_zero() {
    std::string sign = "";
    if (decimal < 0)
        sign = "-";
    int integer_part = std::llabs(decimal) / (1LL << b);
    int fraction_part = (std::llabs(decimal) * 1000 / (1LL << b)) % 1000;
    if (integer_part == 0 && fraction_part == 0)
        sign = "";
    std::cout << sign << integer_part << '.' << std::setfill('0') << std::setw(3) << fraction_part;
}

void fixed_point_number::print_toward_nearest_even() {
    if (((decimal * 1000) % (1LL << b)) == 0) {
        print_toward_zero();
        return;
    }
    int fourth_digit = std::llabs(decimal) * 10000 / (1LL << b) % 10;
    if (fourth_digit > 5 && decimal > 0)
        print_toward_pos_infinity();
    else if (fourth_digit > 5 && decimal < 0)
        print_toward_neg_infinity();
    else if (fourth_digit < 5)
        print_toward_zero();
    else if (fourth_digit == 5) {
        if ((decimal * 10000) % (1LL << b) == 0) {
            int third_digit = std::llabs(decimal) * 1000 / (1LL << b) % 10;
            if (third_digit % 2 == 0)
                print_toward_zero();
            else
                (decimal > 0) ? print_toward_pos_infinity()
                              : print_toward_neg_infinity();
        } 
        else
            (decimal > 0) ? print_toward_pos_infinity() : print_toward_neg_infinity();
    }
}

void fixed_point_number::print_toward_pos_infinity() {
    if (decimal < 0)
        print_toward_zero();
    else {
        int integer_part = decimal / (1LL << b);
        int fraction_part = ((decimal * 1000) / (1LL << b)) % 1000;
        if ((decimal * 1000) % (1LL << b) != 0) {
            if (++fraction_part == 1000) {
                fraction_part = 0;
                ++integer_part;
            }
        }
        std::cout << integer_part << '.' << std::setfill('0') << std::setw(3) << fraction_part;
    }
}

void fixed_point_number::print_toward_neg_infinity() {
    if (decimal >= 0)
        print_toward_zero();
    else {
        std::cout << '-';
        decimal = std::llabs(decimal);
        print_toward_pos_infinity();
        decimal *= -1;
    }
}

void fixed_point_number::operator+=(const fixed_point_number &num) {
    decimal += num.decimal;
    modular_arithmetic();
}

void fixed_point_number::operator-=(const fixed_point_number &num) {
    decimal -= num.decimal;
    modular_arithmetic();
}

void fixed_point_number::operator*=(const fixed_point_number &num) {
    decimal *= num.decimal;
    rounding_after_operation(1LL << b);
    modular_arithmetic();
}

void fixed_point_number::operator/=(const fixed_point_number &num) {
    if (num.decimal == 0) {
        std::cout << "div_by_0";
        exit(0);
    }
    decimal *= (1LL << b);
    rounding_after_operation(num.decimal);
    modular_arithmetic();
}

void fixed_point_number::rounding_after_operation(long long divisor) {
    if (rounding_format == 0)
        rounding_zero(divisor);
    else if (rounding_format == 1)
        rounding_nearest_even(divisor);
    else if (rounding_format == 2)
        rounding_pos_infinity(divisor);
    else if (rounding_format == 3)
        rounding_neg_infinity(divisor);
}

void fixed_point_number::rounding_zero(long long divisor) {
    decimal /= divisor;
}

void fixed_point_number::rounding_nearest_even(long long divisor) {
    if ((decimal > 0 && divisor > 0) || (decimal < 0 && divisor < 0)) {
        int first_digit = (((std::llabs(decimal) % std::llabs(divisor)) * 10) / std::llabs(divisor)) % 10;
        if ((first_digit == 5) && ((((std::llabs(decimal) % std::llabs(divisor)) * 10) % std::llabs(divisor)) == 0)) {
            if (((decimal / divisor) % 10) % 2 == 0)
                decimal /= divisor;
            else
                decimal = decimal / divisor + 1;
        } 
        else if (first_digit >= 5)
            decimal = decimal / divisor + 1;
        else
            decimal /= divisor;
    } 
    else if ((decimal > 0 && divisor < 0) || (decimal < 0 && divisor > 0)) {
        int first_digit = ((std::llabs(decimal) % std::llabs(divisor)) * 10) / std::llabs(divisor) % 10;
        if ((first_digit == 5) && ((((std::llabs(decimal) % std::llabs(divisor)) * 10) % std::llabs(divisor)) == 0)) {
            if (((std::llabs(decimal) / std::llabs(divisor)) % 10) % 2 == 0)
                decimal /= divisor;
            else
                decimal = decimal / divisor - 1;
        } 
        else if (first_digit >= 5)
            decimal = decimal / divisor - 1;
        else
            decimal /= divisor;
    }
}

void fixed_point_number::rounding_pos_infinity(long long divisor) {
    if ((decimal > 0 && divisor < 0) || (decimal < 0 && divisor > 0))
        decimal /= divisor;
    else if ((decimal > 0 && divisor > 0) || (decimal < 0 && divisor < 0)) {
        if (decimal % divisor == 0)
            decimal /= divisor;
        else
            decimal = decimal / divisor + 1;
    }
}

void fixed_point_number::rounding_neg_infinity(long long divisor) {
    if ((decimal > 0 && divisor < 0) || (decimal < 0 && divisor > 0)) {
        if (decimal % divisor == 0)
            decimal /= divisor;
        else
            decimal = (decimal / divisor) - 1;
    } 
    else if ((decimal > 0 && divisor > 0) || (decimal < 0 && divisor < 0))
    decimal /= divisor;
}

