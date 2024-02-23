#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>

// Exact formulas for European options and barrier options

double normal_cdf(double x)
{
    // CDF of the standard normal distribution
    return 0.5 * erfc(-x / sqrt(2));
}

double d1(double x, double r, double T, double sigma)
{
    // d1 of the Black-Scholes formula
    return (log(x) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
}
double d2(double x, double r, double T, double sigma)
{
    // d2 of the Black-Scholes formula
    return d1(x, r, T, sigma) - sigma * sqrt(T);
}

double bs_european_call(double S0, double K, double r, double T, double sigma)
{
    // Provides the Black-Scholes price of a European call option
    double d1_ = d1(S0/K, r, T, sigma);
    double d2_ = d2(S0/K, r, T, sigma);
    return S0 * normal_cdf(d1_) - K * exp(-r * T) * normal_cdf(d2_);
}

double bs_european_put(double S0, double K, double r, double T, double sigma)
{
    // Provides the Black-Scholes price of a European put option
    double d1_ = d1(S0/K, r, T, sigma);
    double d2_ = d2(S0/K, r, T, sigma);
    return K * exp(-r * T) * normal_cdf(-d2_) - S0 * normal_cdf(-d1_);
}

double bs_barrier_call_up_and_out(
    double S0, double K, double r, double T, double sigma, double B)
{
    if (B <= S0)
    {
        return 0.0;
    }

    return (
        S0*(normal_cdf(d1(S0/K, r, T, sigma)) - normal_cdf(d1(S0/B, r, T, sigma)))
        - K*exp(-r*T)*(normal_cdf(d2(S0/K, r, T, sigma)) - normal_cdf(d2(S0/B, r, T, sigma)))
        - B*(pow(S0/B, -2*r/(sigma*sigma)))*(normal_cdf(d1(B*B/(K*S0), r, T, sigma)) - normal_cdf(d1(B/S0, r, T, sigma)))
        + K*exp(-r*T)*(pow(S0/B, 1-(2*r/(sigma*sigma))))*(normal_cdf(d2(B*B/(K*S0), r, T, sigma)) - normal_cdf(d2(B/S0, r, T, sigma)))
    );
}

double bs_dnt(double S0, double K, double r, double T, double sigma, double L, double B) {
    if (S0 < L || S0 > B) {
        return 0;
    }
    
    double alpha = 1/2.0 - r / (sigma * sigma);
    double beta = r + 0.5 * sigma * sigma * alpha * alpha;
    double H = log(B / L);
    
    double sum_value = 0;
    for (int n = 1; n <= 99; ++n) {
        double term1 = (1 - pow(-1, n) * exp(-alpha * H)) / (alpha * alpha * H * H + n * n * M_PI * M_PI);
        double term2 = exp(-0.5 * sigma * sigma * (n * n * M_PI * M_PI / (H * H)) * T);
        double term3 = sin(n * M_PI / H * log(S0 / L));
        sum_value += n * term1 * term2 * term3;
    }
    
    double result = 2 * M_PI * pow(S0 / L, alpha) * exp(-beta * T) * sum_value;
    return result;
}