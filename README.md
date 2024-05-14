
# K-Route Explorer

K-Route Explorer is a tool designed for analyzing and optimizing large-scale networks through the efficient computation of the K-shortest paths between nodes. Leveraging the power of OpenMP and MPI, this robust tool is engineered for high-performance parallel and distributed computing, making it ideal for complex, data-intensive environments. It serves a wide range of professionals from data scientists to network engineers, providing rapid, precise analyses of connectivity and route optimization in areas such as transportation, telecommunications, and social networks.


## Features

- K-Shortest Paths Calculation
- Optimized for Large Datasets
- Parallel Computing with OpenMP
- Distributed Computing with MPI
- Robustness to Network Changes


## Installation

Installation requirements
- Docker with image of nlknguyen/alpine-mpich
- OpenMP
- MPI 
- Python with Jupyter 


    
## How to run
- Run test.ipynb to covert DoctorWho.csv into a formatted txt file
- Copy all the files from the zip to alpine-mpich\cluster\project folder including code and txt files
- Open cmd in alpine-mpich\cluster and run the following commands:

```bash
./cluster.sh up size=4
./cluster.sh login
```
- Now the container of docker is running and you're in project folder
```bash
mpirun ./mpi_hello_world
```
