# Introduction

BDHM (Brownian Dynamics with Hydrodynamics and Magnetic forces) combines magnetic dipole interactions and hydrodynamics on the point particle level with a heuristic repulsion model for the spatial expansion of the colloids to imitate the dynamics of the experiments described in 

[1] *Scalable high-throughput microfluidic separation of magnetic microparticles* by Hongti Gu et al.

By reproducing the qualitative behavior of the experiments in [1] with the heavily simplified toy model implemented in BDHM, we can show the generality of the underlying dynamic phenomena. All information and methods employed by BDHM are described in detail in the supporting information of [1]. While BDHM can be used to repeat the corresponding simulations, we strongly encourage full reproduction using the information in said supporting information.

# Table of content:
 - [What BDHM is and what it is not](#What)
 - [Requirements](#Requirements)
 - [How to build BDHM](#Build)
 - [How to use BDHM](#Use)
 - [What BDHM does](#What)
 - [How to interprete BDHM data](#Data)
 - [Assumptions and simplifications](#Assumptions)
 - [Contents of the particular files](#Contents)
 - [Disclaimer](#Disclaimer)

 <a id="What"></a>
# What BDHM is and what it is not

BDHM is a heavily simplified toy model that trades physical rigorosity for straightforward concepts to act as a proof of concept for the experiments and the suggested system of [1]. It successfully reproduces the dynamic behavior of the experiments on a qualitative level and, thus, indicates that the found results are independent of the concrete realization of the system. However, BDHM does not generate precise quantitative results, and the corresponding numerical data should be interpreted qualitatively. Nevertheless, compared to the resolution of the experiments, BDHM can be used to get a rough idea of the behavior of the system on the particle level.

 <a id="Requirements"></a>
# Requirements

The program can successfully be built with gcc 13.2 (which was tested through GitHub checks). The functionality was heavily tested for binaries made with gcc 6.3.0 and gcc 7.5.0. The data presented in [1] was mostly obtained through a binary obtained with icc (version 2020u4).

To compile BDHM, the library GSL must be linked. However, in the studies presented in [1], GSL is only used for random numbers to generate the random initial conditions. Replacing these initial conditions with fixed ones, the program can easily be adjusted so that GSL is not needed (when repeating the numerical calculations of [1]).

 <a id="Build"></a>
# How to build BDHM

An example makefile is given in the repository. As usual, it can be used to compile the program by typing 

``
make
``

in the console while being in the folder that contains the source files. If BDHM must be built "by hand", the particular C files that contain the different functions (i.e., all seperate C files) can be compiled with

``
gcc -Wall -std=c99 -O3 -g -flto -c filename.c 
``

first. Afterward, the main file BDHM.c must be compiled while linking everything together. This can be done by typing

``
gcc -Wall -std=c99 -O3 -g -flto -c lists.o inputdata.o distance.o init.o printdata.o hi.o bda.o force.o magnetics.o verletlist.o printdisplay.o flow.o partmanagement.o calc.o BDHM.o -o BDHM3.out -lgsl -lgslcblas -lm
``

in the console.

 <a id="Use"></a>
# How to use BDHM

BDHM must be executed in the console using

``
./BDHM3.out 
``

where the parameters of the simulation must be stated in a file called **inputfile** which is in the same folder as the binary. Note that this input file is "static" meaning that parameter must always be stated at the same position. If inputfile is not present in the folder where BDHM is executed, the program exits with an error. To generate a new example input file, use the line

``
./BDHM3.out Setup
``

in the console. The input file has always the form of the following example:

```
Geometry	6.000000	4	15	10.000000 
InitBox	24.000000	4.000000	7.000000
N	2
dt	0.000000005
StepNumber	10000	10
Magnetics	145.0 0.000000	1.000000	0.000000	9314.47
HiMode	1
VerletList	1.500000
Seed	100
Flow	1000000.0
ParticleManagement	1	30
StationaryMode 1
CalcVelocity	0	4
CalcParticlesPerPole	0	5000
```

The particular lines of such an input file and the meaning of the numbers are summarized in the following list:

- **Geometry:** This line defines the layout of the system. The first number is the lattice constant of the micromagnet grid (in units of the colloid diameter). The second and third numbers are the number of micromagnets in the x and y direction, respectively. The final number is the height of the simulation box (in units of the colloid diameter).
- **InitBox:** Here, the box is defined, where the particles are randomly placed at the start of the simulations. The three numbers are the measurements of the corresponding cuboid volume (in units of the colloid diameter).
- **N:** The corresponding value is the total number of colloids that can be in the simulation at the same time. For StationaryMode 1, N is the number of particles in the system. For StationaryMode 0, N is the maximum number of colloids that is possible.
- **dt:** Length of the simulation step (in units of the Brownian time given by the squared diameter divided by diffusion coefficient).
- **StepNumber:** This line of the input file defines the number of the performed simulation steps. The first value is the total number of simulation steps. The second number defines the distance of consecutive simulation steps that are saved in a generated output file (i.e., in the example, every 10th simulation step is saved).
- **Magnetics:** All parameters of the external magnetic field are given in this line. The first value is the magnitude of the magnetic field (in units of mT) and the following three values are the coordinates of the initial direction of the field. The last value is the frequency with which the field is rotating (in units 1 per Brownian time).
- **HiMode:** Using the numbers 0, 1, and 2, the algorithm for the integration of the particle dynamics can be adjusted. With the option 0, all hydrodynamic interactions between the colloids are neglected. Option 1 means that the magnetic forces result in hydrodynamic interactions but flow fields generated due to steric interactions are neglected. This is the default case used for all studies described in [1]. The last option 2 includes hydrodynamic interactions for all particle forces.
- **VerletList:** To reduce the computational cost, Verlet lists are implemented. The number in this line is the corresponding Verlet radius (in units of the colloid diameter).
- **Seed:** Random seed for the generation of the pseudo-random numbers.
- **Flow:** Here, the additional flow field can be tuned (when StationaryMode 0 is chosen). The corresponding value is the maximum velocity of the parabolic Poiseuille flow (in units of colloid diameter divided by the Brownian time).
- **ParticleManagement:** If StationaryMode 0 is chosen, then the particles are deleted and added to mimic the in and out flux of colloidal particles in the experiments. This line controls the number of particles in the system. The first number is the initial number of particles with which the simulation starts (while N is the maximum number that can be present). The second number is the rate which dictates when new particles are added. In detail, it is the number of steps between two consecutive additions of new particles.
- **StationaryMode:** BDHM possesses two main modes. In the first mode, no additional flow field is added, which means the dispersion medium is "stationary" (i.e., StationaryMode 0). The second option (StationaryMode 1) adds a Poiseuille flow to the system.
- **CalcVelocity:** This option can be used to calculate the velocity of the "first" particles of a traveling "particle band". The first number enables the option (i.e., CalcVelocity 1). The second number chooses the position of the "finish line" (in units of micromagnets). After the first particles pass the "finish line", the simulation ends.
- **CalcParticlesPerPole:** If this option is chosen (i.e., CalcParticlesPerPole	1), the average number of particles per occupied micromagnet is computed. Additionally, an output file is written, where the absolute number of particles per pole is given. The second value corresponds to the number of steps after which the second file with the absolute pole occupation is written.

Note that the word and number separation in the input file should be done with the **tab key** (the program is only tested for this case).  

# What BDHM does

All details of BDHM are given in the supporting information of [1]. Using the parameters defined in the input file, BDHM generates an array of micromagnets that are separated by a repulsive wall from superparamagnetic colloids that are located in the "positive half volume" of the setup. Additionally, a rotating magnetic is applied which influences the the colloid dynamics and the "soft-magnetic" micromagnets. 

BDHM possesses two main modes. The first mode (StationaryMode 1) corresponds to the studies without an additional flow field. Here, the function **CalcVelocity** can be used to determine the velocity of "particle bands" moving along the grid due to the rotation of the external field. The velocity calculation starts when the first 20 colloids pass the fourth line of micromagnets perpendicular to the movement direction. The "finish line" (i.e., the line of micromagnets perpendicular to the movement direction where the velocity calculation stops) can be defined in the input file. When the first 20 particles pass the finish line, the simulation stops, and the velocity is "dumped" in the console. Also in this mode, the number of colloids per occupied micromagnet can be computed with the option **CalcParticlesPerPole**. The corresponding data is written in separate output files. During this mode, simplified periodic boundary conditions are applied in the x and y direction. 

The second main mode (StationaryMode 0) corresponds to the studies in [1] with a Poiseuille flow field. Here, additional repulsive walls are added to the system (parallel to the first wall and perpendicular to the movement direction at the end of the array). These walls are not considered in the computations of the hydrodynamic interactions for simplicity. If this mode is chosen, new particles "spawn" at one side of the grid, and particles that leave the micromagnetic array are deleted (they are stored at a position outside of the array). During a simulation, the particle positions are written in an xyz file, which can be used in programs such as VMD to visualize the dynamics.     
 <a id="Data"></a>
# How to interprete BDHM data

Internally, the calculations are performed in units of the colloid diameter, the thermal energy *kT*, the Brownian time, and the diffusion coefficient of a colloid. Accordingly, the output data is also given in said units and must be converted to SI  for a comparison with the experiments. For instance, the particle band velocity is "dumped" in units of colloid diameter divided by the Brownian time and the particle positions in the xyz file are given in multiples of the particle diameter. Note also that the dimensions of the experimental setup can be used to approximate a connection between the maximum velocity of the Poiseuille flow and the volume flux measured in the experiments. A PDF file can be found in the repository, which lists the assumptions and conversion factors used for the comparisons in [1].

 <a id="Assumptions"></a>
# Assumptions and simplifications

In the following, we list some assumptions and simplifications made by BDHM. In future studies, a more rigorous model can be implemented by eliminating items from the following list.

- Only hydrodynamic interactions on the point particle level are used. This means that the Oseen tensor and the Blake tensor are applied.
- The colloids and the micromagnets of the grid are modeled as point dipoles.
- The hydrodynamic interactions resulting from the steric particle repulsions are neglected in the studies of [1].
- When multiple walls are present (which is the case for the systems with the additional flow field), only the influence of the wall corresponding to the magnetic grid on the hydrodynamic interactions is taken into account.
- Gravity acting on the colloids is neglected.
- Brownian fluctuations are neglected.
- Contact friction between touching colloids is not taken into account.
- The magnetic interactions are cut after a certain distance (see the supporting information of [1]).
- Minimum image convention is used for the hydrodynamic and magnetic interactions when periodic boundary conditions are applied. Even for these long-ranged interactions, only interactions with the nearest periodic image are taken into account for simplicity.
- The magnitude of the rotating magnetic field is kept constant.

Note that the toy model BDHM successfully reproduces the main qualitative behavior found in the experiments, even with all these simplifications.

 <a id="Contents"></a>
# Contents of the particular files

BDHM consits of multiple C files. Here, a summary of the corresponding content of the particular files is given.

- **BDHM.c:** Main file which contains the simulation loop. It calls functions grouped together in the other files.
- **bda.c:** Here, the integrators are implemented. How the particles are moved during each simulation step can be found in this file.
- **calc.c:** Functions for observables that can be computed during a simulation are given in calc.c.
- **distance.c:** Contains functions to compute the distance between particles.
- **flow.c:** Corresponds to the optional Poiseuille flow.
- **force.c:** All functions that correspond to the different forces between the components of the systems are summarized here.
- **hi.c:** The treatment of the hydrodynamic interactions of the system is implemented in this file.
- **init.c:** Initialization of the system at the start of the simulation is organized in init.c.
- **inputdata.c:** All functions in the context of the input file with the parameters are given here.
- **lists.c:** BDHM uses linked lists as a data structure. Here, the corresponding functions are implemented.
- **magnetics.c:** Contains the functions in the context of the magnetic properties of the systems such as the magnetic moments and the external field.
- **partmanagement.c:** This file contains functions to add or delete particles to the system.
- **printdata.c:** Here, the functions that "dump" the output data are summarized.
- **printdisplay.c:** This file contains the text that is written to the console.
- **verletlist.c:** In verletlist.c, the functions to check if the Verlet lists must be updated are implemented. Verlet lists are a method to reduce the computation time of the simulations.

 <a id="Disclaimer"></a>
# Disclaimer

Note that BDHM is **not** an ongoing software project with active support. Instead, it is part of the supporting information of [1] and reflects the state of the software used to obtain the corresponding numerical results. Hence, this program is not optimized for user accessibility but is grown with the needs of the underlying study. For any questions regarding [1] and the corresponding data, please contact the corresponding authors.

