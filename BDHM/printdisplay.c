#include "printdisplay.h"

// ----------------------------------------------------------------------------------------

// Welcome screen with the information of the program. (Is printed to the console).
void PrintWelcome() {

    printf("\n");

    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
           "++++"
           "+++++++\n");
    printf("+                                                                  "
           "    "
           "      +\n");
    printf("+             Magnetic colloids, micromagnets and hydrodynamics    "
           "    "
           "      +\n");
    printf("+             Version3 (BDHM3)                                     "
           "    "
           "      +\n");
    printf("+                                                                  "
           "    "
           "      +\n");
    printf("+             Author:   Anton Lueders                              "
           "    "
           "      +\n");
    printf("+             Date :   14.02.2023                                  "
           "    "
           "      +\n");
    printf("+                                                                  "
           "    "
           "      +\n");
    printf("+             AG Nielaba                                           "
           "    "
           "      +\n");
    printf("+             Statistical and computational physics                "
           "    "
           "      +\n");
    printf("+             Univesity of Konstanz                                "
           "    "
           "      +\n");
    printf("+                                                                  "
           "    "
           "      +\n");
    printf("+             Last edited: 16.04.2024                              "
           "    "
           "      +\n");
    printf("+                                                                  "
           "    "
           "      +\n");
    printf("+             V3.10.6                                              "
           "          +\n");
    printf("+                                                                  "
           "    "
           "      +\n");
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
           "++++"
           "+++++++\n");

    printf("\n");
}

// ----------------------------------------------------------------------------------------

// Prints the input data (global variables) to the console.
void PrintInput() {

    printf("The simulation was started using the following input:\n");

    printf("\n");
    
    printf("Box dimensions: \t%f\t%f\t%f\n", Box[0], Box[1], Box[2]);
    printf("Initial conditions: \t%f\t%f\t%f\n", init_box[0], init_box[1], init_box[2]);
    printf("Number of particles:\t%d\n", N);
    printf("Number of poles:\t%d\n", num_poles[0] * num_poles[1]);
    printf("Length of simulation step:\t%e\n", dt);
    printf("Radius of Verlet lists:\t%f\n", rcut_ver);
    printf("Random seed:\t%ld\n", seed);
    printf("Number of steps and xyz output rate:\t%d\t%d\n", steps, stepsrate);
    printf("Magnetic field strength (in mT):\t%f\n", B_field_strength);
    printf("Initial H orientation:\t%f\t%f\t%f\n", H[0], H[1], H[2]);
    printf("Rotation frequency (omega) of H:\t%f\n", H_freq);
    printf("HIs:\t");
    if(hi_mode == 0) {
        printf("   HIs are neglected.\n");
    } else if (hi_mode == 1) {
        printf("   HIs are only applied to the magnetic interactions.\n");
    } else if (hi_mode == 2) {
        printf("   Calculations with full HIs.\n");
    }
    if (!stationary_mode) {
        printf("Maximum velocity of Poiseuille flow:\t%f\n", v_flow_max);
    } else {
        printf("Stationary Mode:\n");
        printf("   Flow is surpressed.\n");
        printf("   Periodic boundaries are used.\n");
        printf("   Only one wall is present.\n");
    }
    printf("Initial number of particles:\t%d\n", N_start);
    if (!stationary_mode) {
        printf("Particle generation rate:\t%d\n", N_rate);
    } else {
        printf("Particle generation rate:\t%d (StationaryMode)\n", 0);
    }
    if (stationary_mode && calc_velocity) {
        printf("Calculation mode:\n");
        printf("   Particle velocity at pole %d\n", goal_pole);
    }
    if (calc_particles_per_pole) {
        printf("Calculation mode:\n");
        printf("   Mean number of particles per occupied pole\n");
        printf("   Array of pole occupation is printed after %d steps\n", pole_array_step);
    }
}

// ----------------------------------------------------------------------------------------
