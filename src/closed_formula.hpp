#ifndef CLOSED_FORMULA_HPP
#define CLOSED_FORMULA_HPP

double normal_cdf(double x);
double d1(double x, double r, double T, double sigma);
double d2(double x, double r, double T, double sigma);
double bs_european_call(double S0, double K, double r, double T, double sigma);
double bs_european_put(double S0, double K, double r, double T, double sigma);
double bs_barrier_call_up_and_out(double S0, double K, double r, double T, double sigma, double B);
double bs_double_no_touch_call(double S0, double K, double r, double T, double sigma, double L, double B);

#endif // CLOSED_FORMULA_HPP
