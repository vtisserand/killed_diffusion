#include <pybind11/pybind11.h>
#include "montecarlo.hpp"
#include "closed_formula.hpp"

namespace py = pybind11;

PYBIND11_MODULE(killed_diffusion, m) {
    m.doc() = "Python bindings for killed_diffusion functions";

    m.def("bs_european_call", &bs_european_call, "Calculate European call option price using Black-Scholes formula");
    m.def("bs_european_put", &bs_european_put, "Calculate European put option price using Black-Scholes formula");
    m.def("bs_barrier_call_up_and_out", &bs_barrier_call_up_and_out, "Calculate barrier call option price using Black-Scholes formula");
    m.def("bs_double_no_touch_call", &bs_double_no_touch_call, "Calculate double no-touch call option price using Black-Scholes formula");

    // Add additional functions from montecarlo.hpp
    m.def("generate_ST", &generate_ST, "Generate ST in a Geometric Brownian Motion model");
    m.def("generate_path", &generate_path, "Generate a stock price using the Geometric Brownian Motion model");
    m.def("price_european_call", &price_european_call, "Price European call option using Monte Carlo simulation");
    m.def("price_european_put", &price_european_put, "Price European put option using Monte Carlo simulation");
    m.def("price_barrier_call_up_and_out", &price_barrier_call_up_and_out, "Price barrier call option using Monte Carlo simulation");
    m.def("price_double_no_touch_call", &price_double_no_touch_call, "Price double no-touch call option using Monte Carlo simulation");
    m.def("price_double_no_touch_put", &price_double_no_touch_put, "Price double no-touch put option using Monte Carlo simulation");
    m.def("probability_brownian_bridge_hit", &probability_brownian_bridge_hit, "Compute probability of hitting a barrier in-between two points of the path");
    m.def("price_barrier_call_up_and_out_gobet", &price_barrier_call_up_and_out_gobet, "Price barrier call option using Gobet method");
    m.def("price_double_no_touch_call_gobet", &price_double_no_touch_call_gobet, "Price double no-touch call option using Gobet method");
}
