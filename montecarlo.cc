#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>

// TODO
// Generaliser à une double no touch option (classic si L = None ou B = None);
// Généraliser à un schéma d'Euler pour le modèle de Black-Scholes avec une
// fonction de drift et de volatilité.

double generate_ST(double S0, double r, double T, double sigma, int M)
{
    // Generate ST in a Geometric Brownian Motion model
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::normal_distribution<double> distribution(0.0, 1.0);
    double dt = T / M;
    double S = S0;
    for (int i = 0; i < M; i++)
    {
        double dW = sqrt(dt) * distribution(generator);
        S *= exp((r - 0.5 * sigma * sigma) * dt + sigma * dW);
    }
    return S;
}

std::vector<double> generate_path(double S0, double r, double T, double sigma, int M)
{
    // Generate a stock price using the Geometric Brownian Motion model
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::normal_distribution<double> distribution(0.0, 1.0);
    double dt = T / M;
    std::vector<double> S;
    S.push_back(S0);
    for (int i = 0; i < M; i++)
    {
        double dW = sqrt(dt) * distribution(generator);
        double S_new = S.back() * exp((r - 0.5 * sigma * sigma) * dt + sigma * dW);
        S.push_back(S_new);
    }
    return S;
}

double price_european_call(double S0, double K, double r, double T, double sigma, int M, int N)
{
    double sum = 0.0;
    for (int i = 0; i < N; i++)
    {
        double ST = generate_ST(S0, r, T, sigma, M);
        double payoff = std::max(ST - K, 0.0);
        sum += payoff;
    }
    return exp(-r * T) * sum / N;
}

double price_european_put(double S0, double K, double r, double T, double sigma, int M, int N)
{
    double sum = 0.0;
    for (int i = 0; i < N; i++)
    {
        double ST = generate_ST(S0, r, T, sigma, M);
        double payoff = std::max(K - ST, 0.0);
        sum += payoff;
    }
    return exp(-r * T) * sum / N;
}

double price_barrier_call(double S0, double K, double r, double T, double sigma, int M, int N, double L)
{
    double sum = 0.0;
    for (int i = 0; i < N; i++)
    {
        std::vector<double> S = generate_path(S0, r, T, sigma, M);
        double payoff = 1;
        for (double s : S)
        {
            if (s < L)
            {
                payoff = 0.0;
                break;
            }
        }
        if (payoff == 1)
        {
            payoff = std::max(S.back() - K, 0.0);
        }
        sum += payoff;
    }
    return exp(-r * T) * sum / N;
}

double price_barrier_put(double S0, double K, double r, double T, double sigma, int M, int N, double L)
{
    double sum = 0.0;
    for (int i = 0; i < N; i++)
    {
        std::vector<double> S = generate_path(S0, r, T, sigma, M);
        double payoff = 1;
        for (double s : S)
        {
            if (s < L)
            {
                payoff = 0.0;
                break;
            }
        }
        if (payoff == 1)
        {
            payoff = std::max(K - S.back(), 0.0);
        }
        sum += payoff;
    }
    return exp(-r * T) * sum / N;
}

double price_double_no_touch_call(double S0, double K, double r, double T, double sigma, int M, int N, double L, double H)
{
    double sum = 0.0;
    for (int i = 0; i < N; i++)
    {
        std::vector<double> S = generate_path(S0, r, T, sigma, M);
        double payoff = 1;
        for (double s : S)
        {
            if (s < L || s > H)
            {
                payoff = 0.0;
                break;
            }
        }
        if (payoff == 1)
        {
            payoff = std::max(S.back() - K, 0.0);
        }
        sum += payoff;
    }
    return exp(-r * T) * sum / N;
}

double price_double_no_touch_put(double S0, double K, double r, double T, double sigma, int M, int N, double L, double H)
{
    double sum = 0.0;
    for (int i = 0; i < N; i++)
    {
        std::vector<double> S = generate_path(S0, r, T, sigma, M);
        double payoff = 1;
        for (double s : S)
        {
            if (s < L || s > H)
            {
                payoff = 0.0;
                break;
            }
        }
        if (payoff == 1)
        {
            payoff = std::max(K - S.back(), 0.0);
        }
        sum += payoff;
    }
    return exp(-r * T) * sum / N;
}

double probability_brownian_bridge_hit(
    double z1,
    double z2,
    double delta,
    double sigma,
    bool a = true,
    bool b = true,
    double a_value = 0.0,
    double b_value = 0.0,
    int k_limit = 1000)
{
    if (b && !a)
    {
        // Domain = (-inf, b)
        if (z1 >= b_value)
        {
            return 0.0;
        }
        else if (z2 >= b_value)
        {
            return 0.0;
        }
        return 1 - exp(-2 * (b_value - z1) * (b_value - z2) / (sigma * sigma * delta));
    }
    else if (a && !b)
    {
        // Domain = (a, +inf)
        if (z1 <= a_value)
        {
            return 0.0;
        }
        else if (z2 <= a_value)
        {
            return 0.0;
        }
        return 1 - exp(-2 * (a_value - z1) * (a_value - z2) / (sigma * sigma * delta));
    }
    else
    {
        // Domain = (a, b)
        if (z1 <= a_value || z1 >= b_value)
        {
            return 0.0;
        }
        else if (z2 <= a_value || z2 >= b_value)
        {
            return 0.0;
        }
        double sum = 0.0;
        for (int k = -k_limit + 1; k < k_limit; k++)
        {
            sum += exp(-2 * (k * (b_value - a_value) * (k * (b_value - a_value) + z2 - z1)) / (sigma * sigma * delta));
            sum -= exp(-2 * ((k * (b_value - a_value) + z1 - b_value) * (k * (b_value - a_value) + z2 - b_value)) / (sigma * sigma * delta));
        }

        return 1 - sum;
    }
}

// 2ND VERSION : using a Bernoulli distribution to simulate the probability of hitting the barrier in between two points of the path

double price_barrier_call_2(double S0, double K, double r, double T, double sigma, int M, int N, double L, int k_limit)
{
    double sum = 0.0;
    for (int i = 0; i < N; i++)
    {
        std::vector<double> S = generate_path(S0, r, T, sigma, M);
        bool hit = false;
        for (int j = 1; j < M; j++)
        {
            double probability = probability_brownian_bridge_hit(
                S[j - 1],
                S[j],
                T / M,
                sigma,
                true,
                false,
                L,
                0.0,
                k_limit);
            std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
            std::bernoulli_distribution distribution(probability);

            if (distribution(generator))
            {
                hit = true;
                break;
            }
        }
        if (!hit)
        {
            sum += std::max(S.back() - K, 0.0);
        }
    }
    return exp(-r * T) * sum / N;
}

double price_barrier_put_2(double S0, double K, double r, double T, double sigma, int M, int N, double L, int k_limit)
{
    double sum = 0.0;
    for (int i = 0; i < N; i++)
    {
        std::vector<double> S = generate_path(S0, r, T, sigma, M);
        bool hit = false;
        for (int j = 1; j < M; j++)
        {
            double probability = probability_brownian_bridge_hit(
                S[j - 1],
                S[j],
                T / M,
                sigma,
                true,
                false,
                L,
                0.0,
                k_limit);
            std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
            std::bernoulli_distribution distribution(probability);

            if (distribution(generator))
            {
                hit = true;
                break;
            }
        }
        if (!hit)
        {
            sum += std::max(K - S.back(), 0.0);
        }
    }
    return exp(-r * T) * sum / N;
}

double price_double_no_touch_call_2(double S0, double K, double r, double T, double sigma, int M, int N, double L, double H, int k_limit)
{
    double sum = 0.0;
    for (int i = 0; i < N; i++)
    {
        std::vector<double> S = generate_path(S0, r, T, sigma, M);
        bool hit = false;
        for (int j = 1; j < M; j++)
        {
            double probability = probability_brownian_bridge_hit(
                S[j - 1],
                S[j],
                T / M,
                sigma,
                true,
                true,
                L,
                H,
                k_limit);
            std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
            std::bernoulli_distribution distribution(probability);

            if (distribution(generator))
            {
                hit = true;
                break;
            }
        }
        if (!hit)
        {
            sum += std::max(S.back() - K, 0.0);
        }
    }
    return exp(-r * T) * sum / N;
}

double price_double_no_touch_put_2(double S0, double K, double r, double T, double sigma, int M, int N, double L, double H, int k_limit)
{
    double sum = 0.0;
    for (int i = 0; i < N; i++)
    {
        std::vector<double> S = generate_path(S0, r, T, sigma, M);
        bool hit = false;
        for (int j = 1; j < M; j++)
        {
            double probability = probability_brownian_bridge_hit(
                S[j - 1],
                S[j],
                T / M,
                sigma,
                true,
                true,
                L,
                H,
                k_limit);
            std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
            std::bernoulli_distribution distribution(probability);

            if (distribution(generator))
            {
                hit = true;
                break;
            }
        }
        if (!hit)
        {
            sum += std::max(K - S.back(), 0.0);
        }
    }
    return exp(-r * T) * sum / N;
}