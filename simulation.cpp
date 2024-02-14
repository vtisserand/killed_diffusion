#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include "montecarlo.cc"

int main()
{

    // Parameters
    double K = 100.0, r = 0.05, T = 1.0, sigma = 0.2;
    int M = 100, N = 100;
    int L = 80, H = 120;

    // European call
    std::cout << "Generating results_call.csv" << std::endl;
    std::ofstream file("results_call.csv");
    file << "S0,Price\n";
    for (int i = 0; i < 1000; i++)
    {
        double S0 = 0.2 * i + 1.0;
        double price = price_european_call(S0, K, r, T, sigma, M, N);
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
        double price = price_barrier_call(S0, K, r, T, sigma, M, N, L);
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
        double price = price_double_no_touch_call(S0, K, r, T, sigma, M, N, L, H);
        file_dnt << S0 << "," << price << "\n";
    }
    file_dnt.close();


    // 2nd version : using a Bernoulli distribution to simulate the probability of hitting the barrier in between two points of the path
    
    int k_limit = 10; // iteration limit for the approximation of probability of hitting the barrier computation

    // Barrier call
    std::cout << "Generating results_barrier_2.csv" << std::endl;
    std::ofstream file_barrier_2("results_barrier_2.csv");
    file_barrier_2 << "S0,Price\n";
    for (int i = 0; i < 1000; i++)
    {
        double S0 = 0.2 * i + 1.0;
        double price = price_barrier_call_2(S0, K, r, T, sigma, M, N, L, k_limit);
        file_barrier_2 << S0 << "," << price << "\n";
    }
    file_barrier_2.close();

    // Double no touch call
    std::cout << "Generating results_dnt_2.csv" << std::endl;
    std::ofstream file_dnt_2("results_dnt_2.csv");
    file_dnt_2 << "S0,Price\n";
    for (int i = 0; i < 1000; i++)
    {
        double S0 = 0.2 * i + 1.0;
        double price = price_double_no_touch_call_2(S0, K, r, T, sigma, M, N, L, H, k_limit);
        file_dnt_2 << S0 << "," << price << "\n";
    }
    file_dnt_2.close();

    return 0;
}