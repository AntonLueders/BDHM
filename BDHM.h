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
#define INIT_BOXX 7.
#define INIT_BOXY 7.
#define INIT_BOXZ 3.5
#define NP 50
#define DT 0.00000001
#define STEPS 10000
#define STEPSRATE 10
#define SEED 100
#define EPS_WCA 10000.
#define EPS_WALL 10000.
#define B_FIELD_STRENGTH 90.
#define F_DIPOL_0 63850.0389
#define HX 0.
#define HY 1.
#define HZ 0.
#define H_FREQ 0.04188
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
#define V_FLOW_MAX 10.
#define N_START 20
#define N_RATE 10
#define STATIONARY_MODE 0
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

gsl_rng *generator;
double dim;
double Box[DIM];
double init_box[DIM];
int N;
double dt;
int steps;
int stepsrate;
long seed;
double eps_wca;
double eps_wall;
double B_field_strength;
double F_dipol_0;
double H[DIM];
double H_freq;
double n[DIM];
double q;
double width_poles;
double pos_poles_z;
double distance_poles;
double chi_poles;
double moment_const_poles;
int num_poles[2];
int hi_mode;
double dr_ver_max1;
double dr_ver_max2;
double rcut_ver;
double rcut_dipol_part;
double rcut_dipol_poles;
double v_flow_max;
int N_start;
int N_rate;
bool stationary_mode;
bool calc_velocity;
int start_pole;
int goal_pole;
int front_number;
int calc_particles_per_pole;
int pole_array_step;
Node *partInSim;
Node *waitingParticles;

#endif