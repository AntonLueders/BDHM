#ifndef PARTMANAGEMENT_H
#define PARTMANAGEMENT_H

#include "BDHM.h"
#include "distance.h"
#include "lists.h"

void InitPartLists();
void FreePartLists();
int AddParticle(Particle *P);
int DeleteParticle(Particle *p_i, int index_i);
int CheckForDeltetion(Particle *P);
void CheckStability(Particle *P);

#endif
