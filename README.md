# Algorithms for convex adjustable robust optimization

This code uses C++17 features and depends on Gurobi. 

Tested on Linux Ubuntu 20.04 with Gurobi 10.0.1, cmake 3.22.1 and gcc 11.4.0.

## How to use

## Compiling

You can compile using CMake and gcc by running the following commands at the root directory of this repository.

```shell
mkdir build
cd build
cmake ..
make
```

Be sure to have your environment variable `GUROBI_HOME` properly defined; e.g., 
`GUROBI_HOME=/home/username/gurobi1001/linux64`.

## Running 

You may run our code with the following commands from the root directory of this repository, after the project is compiled.
```shell
./build/FLP/FLP_solve <path_to_instance> <Gamma> # For the Facility Location Problem (FLP)
./build/RAP/RAP_solve <path_to_instance> <Gamma> # For the Resource Allocation Problem (RAP)
```

## Citing

If you want to cite this repository, please cite our paper.
```latex
@misc{Lefebvre2023,
  title={An Exact Algorithm for Two-stage Robust Optimization with Mixed Integer Recourse Problems},
  author={Henri Lefebvre and Enrico Malaguti and Michele Monacig},
  year={2023},
  url = {https://optimization-online.org/2022/11/an-exact-approach-for-convex-adjustable-robust-optimization/}
}
```
