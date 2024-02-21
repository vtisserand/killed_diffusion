// TODO!
// illustrate the weak / strong convergence of the Euler scheme using the 2 methods of the article
// 1/ Discrete Euler scheme without correction
// 2/ Discrete Euler scheme with correction (brownian bridge)

#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include "montecarlo.cpp"
#include "closed_formula.cpp"
#include <cstdio>

int main()
{
    bool delete_files = true;
    if (delete_files)
    {
        std::system("rm -f convergence_*.csv");
    }

    // Parameters
    double K = 100.0, r = 0.05, T = 1.0, sigma = 1;
    double S0 = 100.0;
    int M = 2000; // N : number of time steps, M : number of simulations
    int L = 50, B = 150;

    // Let's make N vary in log scale
    std::vector<int> N_values;
    for (int i = 1; i < 20; i++)
    {
        N_values.push_back(pow(1.5, i));
    }

    // Barrier call
    std::cout << "Generating convergence_barrier.csv" << std::endl;
    std::ofstream file_barrier("convergence_barrier.csv");
    file_barrier << "N,Price\n";
    for (int N : N_values)
    {
        std::cout << "N = " << N << std::endl;
        double price = price_barrier_call_up_and_out(S0, K, r, T, sigma, N, M, B);
        file_barrier << N << "," << price << "\n";
    }
    file_barrier.close();

    // Barrier call Gobet
    std::cout << "Generating convergence_barrier_gobet.csv" << std::endl;
    std::ofstream file_barrier_gobet("convergence_barrier_gobet.csv");
    file_barrier_gobet << "N,Price\n";
    for (int N : N_values)
    {
        std::cout << "N = " << N << std::endl;
        double price = price_barrier_call_up_and_out_gobet(S0, K, r, T, sigma, N, M, B, 10);
        file_barrier_gobet << N << "," << price << "\n";
    }
    file_barrier_gobet.close();

    // Reference price
    std::ofstream file("convergence_barrier_theoretical.csv");
    file << bs_barrier_call_up_and_out(S0, K, r, T, sigma, B) << "\n";
    file.close();

    return 0;
}