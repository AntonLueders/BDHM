#include "verletlist.h"

// ----------------------------------------------------------------------------------------

// Check if Verlet-lists must be updated
bool CheckVerlet(Particle *P) {

    Node *current = partInSim->next;
    while (current != NULL) {
        
        int i = current->value;
        
        double dr = 0.;
        for (int d = 0; d < dim; d++) {
            if(stationary_mode && d < 2) {
                dr += pow(PeriodicDis(P[i].r[d] - P[i].r_old[d], Box[d]), 2.);     // See distance.c
            } else {
                dr += pow(P[i].r[d] - P[i].r_old[d], 2.);
            }
        }
        dr = sqrt(dr);

        if (dr > dr_ver_max1) {
            dr_ver_max2 = dr_ver_max1;
            dr_ver_max1 = dr;
        } else if (dr > dr_ver_max2) {
            dr_ver_max2 = dr;
        }

        if (dr_ver_max1 + dr_ver_max2 > rcut_ver) {
            dr_ver_max1 = 0.;
            dr_ver_max2 = 0.;
            
            return true;
        }
        
        current = current->next;
    }

    return false;
}

// ----------------------------------------------------------------------------------------

// Clears Verlet lists and frees allocated memory.
void FreeVerLists(Particle *P, Particle *Poles, Particle *Wall) {
    
    for(int i = 0; i < N; i++) {
        clearList(P[i].verList_ste);     // See lists.c
        free(P[i].verList_ste);
        clearList(P[i].verList_mag);     // See lists.c
        free(P[i].verList_mag);
    }
    
    for(int j = 0; j < num_poles[0] * num_poles[1]; j++) {
        clearList(Poles[j].verList_mag);     // See lists.c
        free(Poles[j].verList_mag);
    }
    
    for(int k = 0; k < 3; k++) {
        clearList(Wall[k].verList_ste);     // See lists.c
        free(Wall[k].verList_ste);
    }
}

// ----------------------------------------------------------------------------------------
