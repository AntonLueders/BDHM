#ifndef MAGNETICS_H
#define MAGNETICS_H

#include "BDHM.h"

void Rotation_Matrix(double phi, double vec[2], double new_vec[2]);
void CalcAnisotropiePoles();
double CalcFDipol();
void CalcMomentPart(Particle *p_i);
void CalcMomentPole(Particle *pole_i);
void RotateH(double freq);

#endif
