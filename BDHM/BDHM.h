#ifndef BDHM_H
#define BDHM_H

#include "lists.h"
#include <gsl/gsl_math.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_sf_log.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define DIM 3
#define BOXX 10.
#define BOXY 10.
#define BOXZ 4.
#define INIT_BOXX 18.
#define INIT_BOXY 7.
#define INIT_BOXZ 3.5
#define NP 100
#define DT 0.00000001
#define STEPS 100000
#define STEPSRATE 100
#define SEED 100
#define EPS_WCA 10000.
#define EPS_WALL 10000.
#define B_FIELD_STRENGTH 90.
#define F_DIPOL_0 63850.0389
#define HX 0.
#define HY 1.
#define HZ 0.
#define H_FREQ 9314.47
#define NA 1.
#define NR 1.
#define Q 3.
#define WIDTH_POLES 1.86
#define POS_POLES_Z 2.1
#define DISTANCE_POLES 6.
#define CHI_POLES 20.
#define MOMENT_CONST_POLES 781.3061
#define NUM_POLESX 3.
#define NUM_POLESY 10.
#define HI_MODE 1
#define RCUT_VER 1.5
#define RCUT_DIPOL_PART 2.5
#define RCUT_DIPOL_POLES 3.5
#define V_FLOW_MAX 1000000.
#define N_START 20
#define N_RATE 10
#define STATIONARY_MODE 1
#define CALC_VELOCITY 0
#define START_POLE 4
#define GOAL_POLE 6
#define FRONT_NUMBER 20
#define CALC_PARTICLES_PER_POLE 1
#define POLE_ARRAY_STEP 1000

// Definition of bool-variables
typedef enum { false, true } bool;

// Structure which manages the particle information
struct particle {
    double r[DIM]; // Position 
    double r_old[DIM];
    double f_mag[DIM];
    double f_ste[DIM];
    double f[DIM];
    double m[DIM];
    double D;
    double sigma;
    Node *verList_ste; // Verlet-list
    Node *verList_mag; // Verlet-list
};
typedef struct particle Particle;

extern gsl_rng *generator;
extern double dim;
extern double Box[DIM];
extern double init_box[DIM];
extern int N;
extern double dt;
extern int steps;
extern int stepsrate;
extern long seed;
extern double eps_wca;
extern double eps_wall;
extern double B_field_strength;
extern double F_dipol_0;
extern double H[DIM];
extern double H_freq;
extern double n[DIM];
extern double q;
extern double width_poles;
extern double pos_poles_z;
extern double distance_poles;
extern double chi_poles;
extern double moment_const_poles;
extern int num_poles[2];
extern int hi_mode;
extern double dr_ver_max1;
extern double dr_ver_max2;
extern double rcut_ver;
extern double rcut_dipol_part;
extern double rcut_dipol_poles;
extern double v_flow_max;
extern int N_start;
extern int N_rate;
extern bool stationary_mode;
extern bool calc_velocity;
extern int start_pole;
extern int goal_pole;
extern int front_number;
extern int calc_particles_per_pole;
extern int pole_array_step;
extern Node *partInSim;
extern Node *waitingParticles;

#endif
