# Introduction

BDHM (Brownian Dynamics with Hydrodynamics and Magnetic forces) combines magnetic dipole interactions and hydrodynamic interactions on the point particle level with a heuristic repulsion model for the spatial expansion of the colloids to imitate the dynamics of the experiments described in 

[1] Scalable high-throughput microfluidic separation of magnetic microparticles by Hongti Gu et al.

By reproducing the qualitative behavior of the experiments in [1] with the heavily simplified toy model implemented in BDHM, we can show the generality of the underlying dynamic phenomena. All information and methods employed by BDHM are described in detail in the supporting information of [1]. While BDHM can be used to repeat the corresponding simulations, we strongly encourage full reproduction using the information in said supporting information.

# Table of content:
 - [What BDHM is and what it is not](#What)
 - [Requirements](#Requirements)
 - [How to build BDHM](#Build)
 - [Assumptions and simplifications](#Assumptions)
 - [Contents of the particular files](#Contents)
 - [Disclaimer](#Disclaimer)

 <a id="What"></a>
# What BDHM is and what it is not

BDHM is a heavily simplified toy model that trades physical rigorosity for straightforward concepts to act as a proof of concept for the experiments and the suggested system of [1]. It successfully reproduces the dynamic behavior of the experiments qualitatively and, thus, shows that the found results are independent of the concrete realization of the system. However, BDHM does not generate precise quantitative results, and the corresponding numerical data should be interpreted qualitatively. Nevertheless, compared to the resolution of the experiments, BDHM can be used to get a rough idea of the behavior of the system on the particle level.

 <a id="Requirements"></a>
# Requirements

The program can be built for gcc 13.2 (through GitHub). It was heavily tested for gcc 6.3.0 and gcc 7.5.0. The data presented in [1] was mostly obtained through a binary obtained through icc.

To compile BDHM, the library GSL must be linked. However, in the studies presented in [1], GSL is only used for random numbers to generate the random initial conditions. Replacing these initial conditions with fixed ones, the program can easily be adjusted so that GSL is not needed (when repeating the numerical calculations of [1]).

 <a id="Build"></a>
# How to build BDHM

An example makefile is given in the repository. As usually, it can be used to compile the progam by typing **make** in the console while being in the folder that contains the source files. If BDHM must be build "by hand", the particular C files that contain the different functions (i.e., all files except BDHM.c) can be compiled with

``
gcc -Wall -std=c99 -O3 -g -flto -c *filename* 
``

first.

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
- **flow.c:** Corresponds to the optional poiseuille flow.
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

