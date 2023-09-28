# Algorithms for convex adjustable robust optimization

This code depends on [idol](https://hlefebvr.github.io/idol/), [Mosek](https://www.mosek.com/) and [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page). 

Tested on Linux Ubuntu 20.04 with Mosek 10.1, cmake 3.22.1 and gcc 11.4.0.

## Citing

If you are using this repository for your research, please cite our paper.
```latex
@misc{Lefebvre2023,
  title={An Exact Algorithm for Two-stage Robust Optimization with Mixed Integer Recourse Problems},
  author={Henri Lefebvre and Enrico Malaguti and Michele Monaci},
  year={2023},
  url = {https://optimization-online.org/2022/11/an-exact-approach-for-convex-adjustable-robust-optimization/}
}
```


## How to use

### Compiling

You can compile using CMake and gcc by running the following commands at the root directory of this repository.

```shell
mkdir build
cd build
cmake ..
make
```

Be sure to have your environment variable `MOSEK_HOME` and `EIGEN_HOME` properly defined; e.g., 
`MOSEK_HOME=/home/<username>/mosek/10.1/tools/platform/linux64x86`. See also this [idol documentation page](https://hlefebvr.github.io/idol/installation/options.html).

### Solving an instance 

You may run our code with the following commands from the root directory of this repository, after the project is compiled.
```shell
./build/FLP/solver/FLP_solve <path_to_instance> <p> <deviation> <method=GBD|CCG|Nominal> <time_limit> # For the Facility Location Problem (FLP-old)
./build/RAP/solver/RAP_solve <path_to_instance> <p> <deviation> <method=GBD|CCG|Nominal> <time_limit> # For the Resource Allocation Problem (RAP-old)
```

### Generating new instances

You may generate new instances with the following commands from the root directory of this repository, after the project is compiled.
```shell
./build/FLP/generator/FLP_generate <destination_folder> <n_facilities> <n_customers> <capacity_over_demand_ratio> <n_instances> # For the Facility Location Problem (FLP-old)
./build/RAP/generator/RAP_generate <destination_folder> <n_servers> <n_clients> <n_instances> # For the Resource Allocation Problem (RAP-old)
```
