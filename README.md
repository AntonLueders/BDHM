# Introduction

BDHM (Brownian particle Dynamics with Hydrodynamics and Magnetic forces) combines magnetic dipole interactions and hydrodynamic interactions on the point particle level with a heuristic repulsion model for the spatial expansion of the colloids to imitate the dynamics of the experiments described in 

[1] Scalable high-throughput microfluidic separation of magnetic microparticles by Hongti Gu et al.

Reproducing the qualitative behavior of the experiments in Ref.1 with the heavily simplified toy model implemented in BDHM, we can show the generality of the underlying dynamic phenomena. All information and methods employed by BDHM are described in detail in the supporting information of Ref.1. While BDHM can be used to repeat the corresponding simulations, we strongly encourage full reproduction using the information in said supporting information.

# Table of content:
 - [What BDHM is and what it is not](#What)
 - [Assumptions and simplifications](#Assumptions)
 - [Disclaimer](#Disclaimer)

 <a id="What"></a>
# What BDHM is and what it is not

BDHM is a heavily simplified toy model that trades physical rigorosity for straightforward concepts to act as a proof of concept for the experiments and the suggested system of Ref.1. It successfully reproduces the dynamic behavior of the experiments qualitatively and, thus, shows that the found results are independent of the concrete realization of the system. However, BDHM does not generate precise quantitative results, and the corresponding simulation results should be interpreted qualitatively. Nevertheless, compared to the resolution of the experiments, BDHM can be used to get a rough idea of the behavior of the system on the particle level.

 <a id="Assumptions"></a>
# Assumptions and simplifications

In the following, we list some assumptions and simplifications made by BDHM. In future studies, a more rigorous model can be implemented by eliminating items from the following list.

- Only hydrodynamic interactions on the point particle level are used. This means that the Oseen tensor and the Blake tensor are applied.
- The colloids and the micromagnets of the grid are modeled as point dipoles.
- The hydrodynamic interactions resulting from the steric particle repulsions are neglected in the studies of Ref.1.
- When multiple walls are present (which is the case for the systems with the additional flow field), only the influence of the wall corresponding to the magnetic grid on the hydrodynamic interactions is taken into account.
- Gravity acting on the colloids is neglected.
- Brownian fluctuations are neglected.
- Contact friction between touching colloids is not taken into account.
- The magnetic interactions are cut after a certain distance (see the supporting information of Ref.1).
- Minimum image convention is used for the hydrodynamic and magnetic interactions when periodic boundary conditions are applied. Even for these long-ranged interactions, only interactions with the nearest periodic image are taken into account for simplicity.

Note that the toy model BDHM successfully reproduces the main qualitative behavior found in the experiments, even with all these simplifications.

 <a id="Disclaimer"></a>
# Disclaimer

Note that BDHM is not an ongoing software project with active support. Instead, it is part of the supporting information of Ref.1 and reflects the state of the software used to obtain the corresponding numerical results. For any questions regarding Ref.1 and the corresponding data, please contact the corresponding authors.

