#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <string>
// #include "logging.h" // use it if you want to log the results ( e.g. logging("results.csv", "S0,Price\n", S0, price);)
// #include "logging.cpp"

// TODO
// Généraliser à un schéma d'Euler pour le modèle de Black-Scholes avec une
// fonction de drift et de volatilité homogène: dS = b(S)* dt + sigma(S) * dW
// Par exemple, pour le modèle de Black-Scholes, b(S) = r * S et sigma(S) = sigma * S

// Random number generator
std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());

double generate_ST(double S0, double r, double T, double sigma, int N)
{
    // Generate ST in a Geometric Brownian Motion model
    std::normal_distribution<double> distribution(0.0, 1.0);
    double dt = T / N;
    double S = S0;
    for (int i = 0; i < N; i++)
    {
        double dW = sqrt(dt) * distribution(generator);
        S *= exp((r - 0.5 * sigma * sigma) * dt + sigma * dW);
    }
    return S;
}

std::vector<double> generate_path(double S0, double r, double T, double sigma, int N)
{
    // Generate a stock price using the Geometric Brownian Motion model
    std::normal_distribution<double> distribution(0.0, 1.0);
    double dt = T / N;
    std::vector<double> S;
    S.push_back(S0);
    for (int i = 0; i < N; i++)
    {
        double dW = sqrt(dt) * distribution(generator);
        double S_new = S.back() * exp((r - 0.5 * sigma * sigma) * dt + sigma * dW);
        S.push_back(S_new);
    }
    return S;
}

// Vanilla options
double price_european_call(double S0, double K, double r, double T, double sigma, int N, int M)
{
    double sum = 0.0;
    for (int i = 0; i < M; i++)
    {
        double ST = generate_ST(S0, r, T, sigma, N);
        double payoff = std::max(ST - K, 0.0);
        sum += payoff;
    }
    return exp(-r * T) * sum / M;
}

double price_european_put(double S0, double K, double r, double T, double sigma, int N, int M)
{
    double sum = 0.0;
    for (int i = 0; i < M; i++)
    {
        double ST = generate_ST(S0, r, T, sigma, N);
        double payoff = std::max(K - ST, 0.0);
        sum += payoff;
    }
    return exp(-r * T) * sum / M;
}

// Barrier options
double price_barrier_call_up_and_out(double S0, double K, double r, double T, double sigma, int N, int M, double B)
{
    double sum = 0.0;
    for (int i = 0; i < M; i++)
    {
        std::vector<double> S = generate_path(S0, r, T, sigma, N);
        double payoff = 1;
        for (double s : S)
        {
            if (s > B)
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
    return exp(-r * T) * sum / M;
}

// Double no touch options (Barrier options with two barriers)
double price_double_no_touch_call(double S0, double K, double r, double T, double sigma, int N, int M, double L, double B)
{
    double sum = 0.0;
    for (int i = 0; i < M; i++)
    {
        std::vector<double> S = generate_path(S0, r, T, sigma, N);
        double payoff = 1;
        for (double s : S)
        {
            if (s < L || s > B)
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
    return exp(-r * T) * sum / M;
}

double price_double_no_touch_put(double S0, double K, double r, double T, double sigma, int N, int M, double L, double B)
{
    double sum = 0.0;
    for (int i = 0; i < M; i++)
    {
        std::vector<double> S = generate_path(S0, r, T, sigma, N);
        double payoff = 1;
        for (double s : S)
        {
            if (s < L || s > B)
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
    return exp(-r * T) * sum / M;
}


// Emmanuel Gobet (2000): refining the approximation using a Bernoulli distribution to simulate the probability of hitting the barrier in-between two points of the path
double probability_brownian_bridge_hit(
    double z1,
    double z2,
    double delta,
    double sigma,
    bool a = true, // a = true if the barrier is active on the left
    bool b = true, // b = true if the barrier is active on the right
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
        // BUG : j'ai ôté le 1-exp() car sinon on est sur une Bernoulli de paramètre 1-p...
        // C'est une erreur dans le papier de Gobet (?) -> A vérifier (cohérence avec le papier de Gobet)
        return exp(-2 * (b_value - z1) * (b_value - z2) / (sigma * z1 * sigma * z1 * delta));
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
        // BUG : j'ai ôté le 1-exp() car sinon on est sur une Bernoulli de paramètre 1-p...
        // C'est une erreur dans le papier de Gobet (?) -> A vérifier (cohérence avec le papier de Gobet)
        return exp(-2 * (a_value - z1) * (a_value - z2) / (sigma * z1 * sigma * z1 * delta));
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
            sum += exp(-2 * (k * (b_value - a_value) * (k * (b_value - a_value) + z2 - z1)) / (sigma * z1 * sigma * z1 * delta));
            sum -= exp(-2 * ((k * (b_value - a_value) + z1 - b_value) * (k * (b_value - a_value) + z2 - b_value)) / (sigma * z1 * sigma * z1 * delta));
        }

        return 1 - sum;
    }
}

// Barrier options
double price_barrier_call_up_and_out_gobet(double S0, double K, double r, double T, double sigma, int N, int M, double B, int k_limit)
{
    std::ofstream file("debug_barrier.csv", std::ios_base::app);
    double sum = 0.0;
    for (int i = 0; i < M; i++)
    {
        std::vector<double> S = generate_path(S0, r, T, sigma, N);
        double payoff = 1;
        for (int j = 1; j < N; j++)
        {
            // 2/ Check if the barrier is hit between S[j-1] and S[j]
            double probability = probability_brownian_bridge_hit(
                S[j - 1],
                S[j],
                T / N,
                sigma,
                false,
                true,
                0.0,
                B,
                k_limit);
            file << S[j - 1] << "," << S[j] << "," << probability << "\n";
            // 1/ Check if S[j-1] hits the barrier L
            if (S[j - 1] > B)
            {
                payoff = 0.0;
                break;
            }

            std::bernoulli_distribution distribution(probability);
            if (distribution(generator))
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
    return exp(-r * T) * sum / M;
}

// Double no touch options (Barrier options with two barriers)
double price_double_no_touch_call_gobet(double S0, double K, double r, double T, double sigma, int N, int M, double L, double B, int k_limit)
{
    std::ofstream file("debug_dnt.csv", std::ios_base::app);
    double sum = 0.0;
    for (int i = 0; i < M; i++)
    {
        std::vector<double> S = generate_path(S0, r, T, sigma, N);
        double payoff = 1;
        for (int j = 1; j < N; j++)
        {
            // 2/ Check if the barrier is hit between S[j-1] and S[j]
            double probability = probability_brownian_bridge_hit(
                S[j - 1],
                S[j],
                T / N,
                sigma,
                true,
                true,
                L,
                B,
                k_limit);
            file << S[j - 1] << "," << S[j] << "," << probability << "\n";

            // 1/ Check if S[j-1] hits the barrier L or B
            if (S[j - 1] < L || S[j - 1] > B)
            {
                payoff = 0.0;
                break;
            }

            std::bernoulli_distribution distribution(probability);
            if (distribution(generator))
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
    return exp(-r * T) * sum / M;
}
