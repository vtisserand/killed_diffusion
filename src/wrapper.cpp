#include <pybind11/pybind11.h>
#include "montecarlo.hpp"
#include "closed_formula.hpp"

namespace py = pybind11;

PYBIND11_MODULE(killed_diffusion, m) {
    m.doc() = "Python bindings for killed_diffusion functions";

    m.def("bs_european_call", &bs_european_call, 
          "Calculate European call option price using Black-Scholes formula",
          py::arg("S0"), py::arg("K"), py::arg("r"), py::arg("T"), py::arg("sigma"));

    m.def("bs_european_put", &bs_european_put, 
          "Calculate European put option price using Black-Scholes formula",
          py::arg("S0"), py::arg("K"), py::arg("r"), py::arg("T"), py::arg("sigma"));

    m.def("bs_barrier_call_up_and_out", &bs_barrier_call_up_and_out, 
          "Calculate barrier call option price using Black-Scholes formula",
          py::arg("S0"), py::arg("K"), py::arg("r"), py::arg("T"), py::arg("sigma"), py::arg("B"));

    m.def("bs_double_no_touch_call", &bs_double_no_touch_call, 
          "Calculate double no-touch call option price using Black-Scholes formula",
          py::arg("S0"), py::arg("K"), py::arg("r"), py::arg("T"), py::arg("sigma"), py::arg("L"), py::arg("B"));

    // Add additional functions from montecarlo.hpp with argument names and docstrings
    m.def("generate_ST", &generate_ST, 
          "Generate ST in a Geometric Brownian Motion model",
          py::arg("S0"), py::arg("r"), py::arg("T"), py::arg("sigma"), py::arg("N"));

    m.def("generate_path", &generate_path, 
          "Generate a stock price using the Geometric Brownian Motion model",
          py::arg("S0"), py::arg("r"), py::arg("T"), py::arg("sigma"), py::arg("N"));

    m.def("price_european_call", &price_european_call, 
          "Price European call option using Monte Carlo simulation",
          py::arg("S0"), py::arg("K"), py::arg("r"), py::arg("T"), py::arg("sigma"), py::arg("N"), py::arg("M"));

    m.def("price_barrier_call_up_and_out", &price_barrier_call_up_and_out, 
          "Price barrier call option using Monte Carlo simulation",
          py::arg("S0"), py::arg("K"), py::arg("r"), py::arg("T"), py::arg("sigma"), py::arg("N"), py::arg("M"), py::arg("B"));

    m.def("price_double_no_touch_call", &price_double_no_touch_call, 
          "Price double no-touch call option using Monte Carlo simulation",
          py::arg("S0"), py::arg("K"), py::arg("r"), py::arg("T"), py::arg("sigma"), py::arg("N"), py::arg("M"), py::arg("L"), py::arg("B"));

    m.def("probability_brownian_bridge_hit", &probability_brownian_bridge_hit, 
          "Compute probability of hitting a barrier in-between two points of the path",
          py::arg("z1"), py::arg("z2"), py::arg("delta"), py::arg("sigma"), py::arg("a") = true, py::arg("b") = true, py::arg("a_value") = 0.0, py::arg("b_value") = 0.0, py::arg("k_limit") = 1000);

    m.def("price_barrier_call_up_and_out_gobet", &price_barrier_call_up_and_out_gobet, 
          "Price barrier call option using Gobet method",
          py::arg("S0"), py::arg("K"), py::arg("r"), py::arg("T"), py::arg("sigma"), py::arg("N"), py::arg("M"), py::arg("B"), py::arg("k_limit"));

    m.def("price_double_no_touch_call_gobet", &price_double_no_touch_call_gobet, 
          "Price double no-touch call option using Gobet method",
          py::arg("S0"), py::arg("K"), py::arg("r"), py::arg("T"), py::arg("sigma"), py::arg("N"), py::arg("M"), py::arg("L"), py::arg("B"), py::arg("k_limit"));
}
