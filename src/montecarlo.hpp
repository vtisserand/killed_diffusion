#ifndef MONTECARLO_HPP
#define MONTECARLO_HPP

#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <string>

// Random number generator
extern std::default_random_engine generator;

double generate_ST(double S0, double r, double T, double sigma, int N);
std::vector<double> generate_path(double S0, double r, double T, double sigma, int N);

double price_european_call(double S0, double K, double r, double T, double sigma, int N, int M);
double price_european_put(double S0, double K, double r, double T, double sigma, int N, int M);

double price_barrier_call_up_and_out(double S0, double K, double r, double T, double sigma, int N, int M, double B);
double price_double_no_touch_call(double S0, double K, double r, double T, double sigma, int N, int M, double L, double B);
double price_double_no_touch_put(double S0, double K, double r, double T, double sigma, int N, int M, double L, double B);

double probability_brownian_bridge_hit(double z1, double z2, double delta, double sigma, bool a = true, bool b = true, double a_value = 0.0, double b_value = 0.0, int k_limit = 1000);

double price_barrier_call_up_and_out_gobet(double S0, double K, double r, double T, double sigma, int N, int M, double B, int k_limit);
double price_double_no_touch_call_gobet(double S0, double K, double r, double T, double sigma, int N, int M, double L, double B, int k_limit);

#endif // MONTECARLO_HPP
