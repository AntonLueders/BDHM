#ifndef HI_H
#define HI_H

#include "BDHM.h"
#include "distance.h"

void CalcOseenTensor(double dissq, double dij[3], double Oij[3][3]);
void CalcDeltaOseen(Particle *p_i, Particle *p_j, double delta_Oij[3][3]);
void CalcDij(int index_i, int index_j, Particle *p_i, Particle *p_j, double Dij[3][3]);
void CalcDTensor(Particle *P, double D_Tensor[N][N][3][3]);

#endif
