# Killed diffusion and pricing path-dependent options

We implement a discrete and a continuous Euler scheme to compute payoffs of the form $$\mathbb{E}\left[\mathbb{1}_{T < \tau} f(X_T)\right],$$ where $$\tau \coloneqq \inf {{t > 0 : X_t \notin D }}$$ is a stopping time for when the diffusion exits a domain $D$.

![Call up-and-out price under Black-Scholes](report/img/cuo_mc.png)

# Navigating the repository

The code essentially lies as C++ files in the src folder. There is a wrapper file that allows to expose functions to Python and use them in a more convenient environment (e.g. Jupyter notebooks). The plots are generated through nbs/main.ipynb while the report can be found at report/main.pdf

# TO-DOs
- [ ] Add the new theoretical price of a Black-Scholes double no touch to closed_formula.cpp (very important); see main.ipynb
- [ ] Improve the compatibility of the pybind11 wrapping module to Linux/Windows users -> use a fancy CMake setup and detail the?
- [ ] To go further: Monte Carlo pricing for higher dimension (d>1) barriers -> try out more complex ones (breaking H1, H2, ... regularity conditions; cf. Gobet's paper)


# Compiling the dynamic library

To expose the C++ functions in Python through the killed_diffusion module, we run the following command:

```console
g++ -O3 -Wall -shared -std=c++11 -fPIC -Wl,-undefined,dynamic_lookup `python3 -m pybind11 \
--includes` src/closed_formula.cpp src/montecarlo.cpp src/wrapper.cpp -o \
killed_diffusion/killed_diffusion`python3-config --extension-suffix`
```
