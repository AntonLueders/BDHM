#ifndef CALC_H
#define CALC_H

#include "BDHM.h"

int CalcVelocity(Particle *P);
void PrintVelocity(int step);
int CalcVelocityStart(Particle *P);
void CalcPoleArray(Particle *P, int pole_array[num_poles[0]][num_poles[1]]);
double CalcParticlesPerPole(Particle *P);
void PrintParticlesPerPole(double time, double result);
void PrintPoleArray(Particle *P);

#endif