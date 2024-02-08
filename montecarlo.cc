#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>

// Where we get the objects to simulate stuff

// TODO: move to option.h, option.cpp, ... the core of the simulation
class Option
{
public:
    Option(double _S0, double _K, double _r, double _T, double _sigma, int _M, int _N)
        : S0(_S0), K(_K), r(_r), T(_T), sigma(_sigma), M(_M), N(_N) {}
    virtual double payoff(double S) const = 0;
    double price() const;
    double delta() const;
    double gamma() const;
    double vega() const;
    double theta() const;
    double rho() const;

protected:
    double S0, K, r, T, sigma;
    int M, N;
};

class EuropeanCall : public Option
{
public:
    EuropeanCall(double _S0, double _K, double _r, double _T, double _sigma, int _M, int _N)
        : Option(_S0, _K, _r, _T, _sigma, _M, _N) {}
    double payoff(double S) const { return std::max(S - K, 0.0); }
};

class BarrierOption : public Option
{
public:
    BarrierOption(double _S0, double _K, double _r, double _T, double _sigma, int _M, int _N, double _B)
        : Option(_S0, _K, _r, _T, _sigma, _M, _N), B(_B) {}
    double payoff(double S) const { return (S > B) ? std::max(S - K, 0.0) : 0.0; }

private:
    double B;
};

class DoubleNoTouch : public Option
{
public:
    DoubleNoTouch(double _S0, double _K, double _r, double _T, double _sigma, int _M, int _N, double _L, double _H)
        : Option(_S0, _K, _r, _T, _sigma, _M, _N), L(_L), H(_H) {}
    double payoff(double S) const { return (S < L || S > H) ? std::max(S - K, 0.0) : 0.0; }

private:
    double L, H;
};


class StockPricePath
{
    public:
        StockPricePath(double _S0, double _r, double _T, double _sigma, int _M, int _N)
            : S0(_S0), r(_r), T(_T), sigma(_sigma), M(_M), N(_N) {}
        void generate_path();
        double get_price() const { return S[M]; }
    
    private:
        double S0, r, T, sigma;
        int M, N;
        std::vector<double> S;
};

void StockPricePath::generate_path()
{
    S.resize(M + 1);
    S[0] = S0;
    for (int i = 1; i <= M; i++)
    {
        double W = sqrt(T / M) * rand() / (RAND_MAX + 1.0);
        S[i] = S[i - 1] * exp((r - 0.5 * sigma * sigma) * T / M + sigma * W);
    }
}

double Option::price() const
{
    double sum = 0.0;
    for (int i = 0; i < N; i++)
    {
        StockPricePath path(S0, r, T, sigma, M, N);
        path.generate_path();
        sum += payoff(path.get_price());
    }
    return exp(-r * T) * sum / N;
}

