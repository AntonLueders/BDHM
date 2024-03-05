#ifndef DISTANCE_H
#define DISTANCE_H

#include "BDHM.h"

double PeriodicPos(double dr, double L);
double PeriodicDis(double dr, double L);
double Distance(Particle *p_i, Particle *p_j, double *dij);

#endif
