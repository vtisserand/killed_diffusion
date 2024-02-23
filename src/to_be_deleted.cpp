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
    bool delete_files = false;
    if (delete_files)
    {
        std::system("rm -f to_be_deleted*.csv");
    }

    double S0_ = 100;
    double K_ = 100;
    double r_ = 0.05;
    double T_ = 1;
    double sigma_ = 0.2;
    int N_ = 1000;
    int M_ = 1;
    double B_ = 120;
    // Let's make sure that price_option generic works with the black-scholes formula
    double price_1 = price_barrier_call_up_and_out_gobet(S0_, K_, r_, T_, sigma_, N_, M_, B_);
    std::cout << "price_1 (black scholes mc gobet): " << price_1 << std::endl;

    // // leaving drift, sigma, and f as default values (for default bs call barrier option)
    double price_2 = price_option_generic(S0_, K_, r_, T_);
    std::cout << "price_2 (generic with mc gobet and default bs drift/vol): " << price_2 << std::endl;

    // Parameters
    double L = -100; // L : dummy value
    double B = std::exp(1);
    double K = 1;
    double r = 0;
    auto drift = [](double S)
    { return S; };
    auto sigma = [](double S)
    { return 0; };
    double S0 = 1;
    double T = 1;
    int M = 10;
    auto f = [](double S, double K)
    { return 1; }; // digital option
    double k_limit = 10;

    std::vector<int> N_range;
    for (int i = 1; i < 150; i++)
    {
        N_range.push_back(static_cast<int>(std::pow(i, 1.5)));
    }

    std::vector<double> cuo_conv;
    for (auto N : N_range)
    {
        double price_option_generic_ = price_option_generic(
            S0,
            K,
            r,
            T,
            drift,
            sigma,
            N,
            M,
            L,
            B,
            k_limit,
            f,
            false);
        cuo_conv.push_back(price_option_generic_);
    }

    std::vector<double> cuo_gobet_conv;
    for (auto N : N_range)
    {
        auto price_option_generic_ = price_option_generic(
            S0,
            K,
            r,
            T,
            drift,
            sigma,
            N,
            M,
            L,
            B,
            k_limit,
            f,
            true);
        cuo_gobet_conv.push_back(price_option_generic_);
    }

    std::ofstream file("to_be_deleted.csv");
    file << "N,cuo,cuo_gobet\n";
    for (int i = 0; i < N_range.size(); i++)
    {
        file << N_range[i] << "," << cuo_conv[i] << "," << cuo_gobet_conv[i] << "\n";
    }
    file.close();

    std::ofstream file_2("to_be_deleted_theoretical.csv");
    file_2 << "price_theoretical\n";
    file_2 << bs_barrier_call_up_and_out(S0, K, r, T, 0, B) << "\n";
    file_2.close();

    return 0;
}
