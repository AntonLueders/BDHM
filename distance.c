#include "distance.h"

// ----------------------------------------------------------------------------------------

// Periodic boundary conditions (for position)
double PeriodicPos(double dr, double L) {

    double dr_new = dr - L * floor(dr / L);

    return dr_new;
}

// ----------------------------------------------------------------------------------------

// Periodic boundary conditions (for distance)
double PeriodicDis(double dr, double L) {

    double dr_new = dr - L * nearbyint(dr / L);

    return dr_new;
}

// ----------------------------------------------------------------------------------------

// Distance between two particle centers
double Distance(Particle *p_i, Particle *p_j, double *dij) {

    double dijsq = 0.;
    for (int d = 0; d < dim - 1; d++) {
        
        dij[d] = p_i->r[d] - p_j->r[d];
        
        if(stationary_mode) {
            dij[d] = PeriodicDis(dij[d], Box[d]);     // See distance.c
        }
        dijsq += dij[d] * dij[d];
    }
    
    dij[2] = p_i->r[2] - p_j->r[2];
    dijsq += dij[2] * dij[2];

    return dijsq;
}

// ----------------------------------------------------------------------------------------