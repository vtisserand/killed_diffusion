#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include "montecarlo.cc"
#include "closed_formula.cc"
#include <cstdio>

int main()
{
    bool delete_files = true;
    if (delete_files)
    {
        std::system("rm -f debug_*.csv");
        std::system("rm -f results_*.csv");
    }

    // Parameters
    double K = 100.0, r = 0.05, T = 1.0, sigma = 1;
    int N = 100, M = 100; // N : number of time steps, M : number of simulations
    int L = 50, B = 150;


    // European call
    std::cout << "Generating results_call.csv" << std::endl;
    std::ofstream file("results_call.csv");
    file << "S0,Price\n";
    for (int i = 0; i < 1000; i++)
    {
        double S0 = 0.2 * i + 1.0;
        double price = price_european_call(S0, K, r, T, sigma, N, M);
        file << S0 << "," << price << "\n";
    }
    file.close();

    // Barrier call
    std::cout << "Generating results_barrier.csv" << std::endl;
    std::ofstream file_barrier("results_barrier.csv");
    file_barrier << "S0,Price\n";
    for (int i = 0; i < 1000; i++)
    {
        double S0 = 0.2 * i + 1.0;
        double price = price_barrier_call_up_and_out(S0, K, r, T, sigma, N, M, B);
        file_barrier << S0 << "," << price << "\n";
    }
    file_barrier.close();

    // Double no touch call
    std::cout << "Generating results_dnt.csv" << std::endl;
    std::ofstream file_dnt("results_dnt.csv");
    file_dnt << "S0,Price\n";
    for (int i = 0; i < 1000; i++)
    {
        double S0 = 0.2 * i + 1.0;
        double price = price_double_no_touch_call(S0, K, r, T, sigma, N, M, L, B);
        file_dnt << S0 << "," << price << "\n";
    }
    file_dnt.close();

    // 2nd version : using a Bernoulli distribution to simulate the probability of hitting the barrier in between two points of the path

    int k_limit = 10; // iteration limit for the approximation of probability of hitting the barrier computation

    // Barrier call gobet
    std::cout << "Generating results_barrier_gobet.csv" << std::endl;
    std::ofstream file_barrier_2("results_barrier_gobet.csv");
    file_barrier_2 << "S0,Price\n";
    for (int i = 0; i < 1000; i++)
    {
        double S0 = 0.2 * i + 1.0;
        double price = price_barrier_call_up_and_out_gobet(S0, K, r, T, sigma, N, M, B, k_limit);
        file_barrier_2 << S0 << "," << price << "\n";
    }
    file_barrier_2.close();

    // Double no touch call gobet
    std::cout << "Generating results_dnt_gobet.csv" << std::endl;
    std::ofstream file_dnt_2("results_dnt_gobet.csv");
    file_dnt_2 << "S0,Price\n";
    for (int i = 0; i < 1000; i++)
    {
        double S0 = 0.2 * i + 1.0;
        double price = price_double_no_touch_call_gobet(S0, K, r, T, sigma, N, M, L, B, k_limit);
        file_dnt_2 << S0 << "," << price << "\n";
    }
    file_dnt_2.close();

    //  closed_formula.cc
    std::cout << "Generating results_call_bs.csv" << std::endl;
    std::ofstream file_bs("results_call_bs.csv");
    file_bs << "S0,Price\n";
    for (int i = 0; i < 1000; i++)
    {
        double S0 = 0.2 * i + 1.0;
        double price = bs_european_call(S0, K, r, T, sigma);
        file_bs << S0 << "," << price << "\n";
    }
    file_bs.close();

    std::cout << "Generating results_barrier_bs.csv" << std::endl;
    std::ofstream file_barrier_bs("results_barrier_bs.csv");
    file_barrier_bs << "S0,Price\n";
    for (int i = 0; i < 1000; i++)
    {
        double S0 = 0.2 * i + 1.0;
        double price = bs_barrier_call_up_and_out(S0, K, r, T, sigma, B);
        file_barrier_bs << S0 << "," << price << "\n";
    }
    file_barrier_bs.close();

    std::cout << "Generating results_dnt_bs.csv" << std::endl;
    std::ofstream file_dnt_bs("results_dnt_bs.csv");
    file_dnt_bs << "S0,Price\n";
    for (int i = 0; i < 1000; i++)
    {
        double S0 = 0.2 * i + 1.0;
        double price = bs_double_no_touch_call(S0, K, r, T, sigma, L, B);
        file_dnt_bs << S0 << "," << price << "\n";
    }
    file_dnt_bs.close();
    return 0;

}

// Refactoring :

