#pragma once

#include <string>

class fixed_point_number {
private:
    long long decimal;
    int a;
    int b;
    int rounding_format;

    void modular_arithmetic();

    void print_toward_zero();
    void print_toward_nearest_even();
    void print_toward_pos_infinity();
    void print_toward_neg_infinity();
    
    void rounding_after_operation(long long divisor);

    void rounding_zero(long long divisor);
    void rounding_nearest_even(long long divisor);
    void rounding_pos_infinity(long long divisor);
    void rounding_neg_infinity(long long divisor);
public:
    void operator+=(const fixed_point_number& num);
    void operator-=(const fixed_point_number& num);
    void operator*=(const fixed_point_number& num);
    void operator/=(const fixed_point_number& num);

    void print();

    fixed_point_number(const std::string& input, const std::string& format, int rounding);
};
