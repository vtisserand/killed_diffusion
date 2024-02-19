#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include "montecarlo.cc"
#include "closed_formula.cc"
#include "logging.h"
#include <cstdio>

int main()
{
    bool delete_files = true;
    if (delete_files)
    {
        std::system("rm -f debug_*.csv");
    }

    // Parameters
    double K = 100.0, r = 0.05, T = 1.0, sigma = 1;
    int N = 1000, M = 1; // N : number of time steps, M : number of simulations
    int L = 50, B = 150;
    int k_limit = 100;
    double S0 = 100;

    std::cout << "Generating results_call.csv" << std::endl;
    double price = price_barrier_call_up_and_out_gobet(S0, K, r, T, sigma, N, M, B, k_limit);
    double price2 = price_double_no_touch_call_gobet(S0, K, r, T, sigma, N, M, L, B, k_limit);

    return 0;
}