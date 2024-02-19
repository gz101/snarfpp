# A Study in Kernel Functions for SNARF Implementation Repository

## Project Title: A Study in Kernel Functions for Sparse Numerical Array-Based Range Filters

### Introduction
This repository contains the implementation code for "A Study in Kernel Functions for Sparse Numerical Array-Based Range Filters," a project focused on enhancing the Sparse Numerical Array-Based Range Filters (SNARF) data structure by exploring the integration of various non-linear kernel functions. Our aim is to investigate if non-linear models can provide improved performance, accuracy, and space efficiency over the linear model currently used in SNARF.

### Research Problem and Aims
The project addresses the limitations of the linear spline model in SNARF, proposing the use of different non-linear kernel functions (quadratic, cubic, logarithmic, and exponential) to potentially reduce the model size and achieve comparable error rates with greater efficiency.

### Objectives
1. Implement a configurable SNARF version supporting various kernel functions.
2. Theoretically analyze and evaluate the trade-offs between model size, error rates, and performance efficiency.
3. Conduct comprehensive performance comparisons with existing data structures (Cuckoo Filter, SuRF, Rosetta) using real-world and synthetic datasets.

### Implementation Details
- Language: C++11 (GNU GCC 13.2.0), Python 3.11 for simulations and benchmarking.
- Environment: Tested on Apple Mac Sonoma 14.2.1 with M1 chip, using single-thread execution.

### Repository Structure
- `/include`: Header files for the modified SNARF implementation.
- `/src`: Source code for the modified SNARF implementation.
- `/data`: Scripts and utilities for generating and managing datasets.
- `/benchmarks`: Benchmarking scripts and result analysis tools.
- `/docs`: Documentation on the algorithms, data structures, and experimental setup.
- `/examples`: A simple tutorial on how to run the configurable SNARF.
- `/tests`: Unit tests that encompass the entire implementation.

### How to Use
Instructions on building the project, generating datasets, and running benchmarks are provided in the corresponding directories. Ensure to follow the setup procedures outlined in `/docs` for compiling the source code.

### Local Development

The project is dockerized, so the following commands will need to be run in order to use it on a local environment.

Build the container:

```sh
docker build -t snarf .
```

Run the app with a `make` command:

```sh
docker run -v $(pwd):/usr/src/snarfpp snarfpp <make_command>
```

For example, to run tests, use the following command:

```sh
docker run -v $(pwd):/usr/src/snarfpp snarfpp tests
```

### Contributions
This work contributes to the field of learned index structures by providing insights into the potential benefits of integrating non-linear kernel functions into SNARF, offering a more adaptable and efficient solution for range filtering tasks.

### Acknowledgements
We acknowledge the foundational work on SNARF and the learned index structures that inspired this research. Special thanks to the authors of the original SNARF paper for their invaluable contributions to the domain.

### License
This project is licensed under the MIT License - see the LICENSE file for details.

### Contact
For any inquiries or contributions, please open an issue in this repository.
