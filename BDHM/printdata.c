#include "printdata.h"

// ----------------------------------------------------------------------------------------

// Writes the xyz position file
// This file can be used for particle visualization (for instance with VMD - Visual Molecular Dynamics)
void Print(FILE *f, Particle *P, Particle *Poles, int step) {
    
    fprintf(f, "%d\n", N + 2 * num_poles[0] * num_poles[1]);
    fprintf(f, "step = %d\n", step + 1);
    
    for (int i = 0; i < num_poles[0] * num_poles[1]; i++) {
        fprintf(f, "P1\t%f\t%f\t%f\n", Poles[i].r[0], Poles[i].r[1], Poles[i].r[2] + q * width_poles / 2.- 0.5);
        fprintf(f, "P1\t%f\t%f\t%f\n", Poles[i].r[0], Poles[i].r[1], Poles[i].r[2] - q * width_poles / 2. + 0.5);
    }

    for (int i = 0; i < N; i++) {
        fprintf(f, "S1\t%f\t%f\t%f\n", P[i].r[0], P[i].r[1], P[i].r[2]);
    }
}

// ----------------------------------------------------------------------------------------
