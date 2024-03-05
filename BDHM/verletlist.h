#ifndef VERLETLIST_H
#define VERLETLIST_H

#include "BDHM.h"
#include "distance.h"
#include "lists.h"

bool CheckVerlet(Particle *P);
void FreeVerLists(Particle *P, Particle *Poles, Particle *Wall);

#endif
