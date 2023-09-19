#ifndef FORCE_H
#define FORCE_H

#include "BDHM.h"
#include "distance.h"
#include "magnetics.h"

double CalcWCA(double sigmasq, double dijsq, double eps);
void CalcDipole(double mi[3], double mj[3], double dissq, double dij[3], double F_0, double f_mag[3]);
double CalcPairSte(Particle *p_i, Particle *p_j);
double CalcPairMag(Particle *p_i, Particle *p_j);
double CalcWall(Particle *p_i, Particle *Wall);
double CalcPole(Particle *p_i, Particle *pole_j);
void CalcForce(Particle *P, Particle *Poles, Particle *Wall);
void CalcForceWithVerlet(Particle *P, Particle *Poles, Particle *Wall);
//void AddDrive(Particle *P);

#endif