#include "partmanagement.h"

// ----------------------------------------------------------------------------------------

// Generates the list of particles that a currently in the simulation and particles
// that are currently waiting to spawn in the simulation. This split is relevant for
// the simulations with the Poiseuille flow, where particles enter and leave the system.
void InitPartLists() {
        
    partInSim = malloc(sizeof(Node));
    partInSim->value = -1;
    partInSim->next = NULL;

    // The number of particles is different for StationaryMode 0 and 1 
    int N_limit = 0;
    if (stationary_mode) {        
        N_limit = N;
    } else {
        N_limit = N_start;
    }
    
    for(int i = 0; i < N_limit; i++) {
        
        push(partInSim, i);     // See lists.c
    }
    
    waitingParticles = malloc(sizeof(Node));
    waitingParticles->value = -1;
    waitingParticles->next = NULL;
    
    for(int j = N_limit; j < N; j++) {
        
        push(waitingParticles, j);     // See lists.c
    }
}

// ----------------------------------------------------------------------------------------

// Clears lists and frees allocated memory.
void FreePartLists() {
        
    clearList(partInSim);     // See lists.c
    free(partInSim);
    
    clearList(waitingParticles);     // See lists.c
    free(waitingParticles);
}

// ----------------------------------------------------------------------------------------

// Adds new particles to the simulation by taking one from the list of waiting particles.
int AddParticle(Particle *P) {
    
    if (waitingParticles->next != NULL) {
        
        int i = waitingParticles->next->value;
        
        bool check_overlap = true;
        
        while(check_overlap) {
            
            // Sets particle position
            for (int d = 0; d < dim; d++) {
                
                P[i].r[d] = init_box[d] * gsl_rng_uniform(generator);
                // Pushes the new particle outside the init_box 
                // and away from the micromagnet array
                if (d == 0) {
                    P[i].r[d] -= init_box[d];
                // Increases distance to the bottom wall for numerical stability
                } else if (d == 2) {
                    if (P[i].r[d] < 0.75) {
                        P[i].r[d] += 0.75;
                    // Increases distance to the top wall for numerical stability
                    // Note the finite size of the particles
                    } else if (!stationary_mode && P[i].r[d] > Box[2] - 0.5) {
                        P[i].r[d] -= 0.5;
                    }
                }
            }
            check_overlap = false;
            
            // Checks for overlap with particles currently in the simulation
            Node *current_j = partInSim->next;
            while (current_j != NULL) {
        
                int j = current_j->value;
                
                double dij[3];
                double dissq = 0.;

                // Particles overlap if they would interact via the WCA potential
                dissq = Distance(&(P[i]), &(P[j]), dij);     // See distance.c
                if(sqrt(dissq) < pow(2., 1. / 6.) * (P[i].sigma + P[j].sigma) / 2.) {
                    check_overlap = true;
                    break;
                }
                
                current_j = current_j->next;
            }
        }
            
        push(partInSim, i);     // See lists.c
        removeByValue(waitingParticles, i);     // See lists.c

        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------------------

// Removes particles from the simulation after leaving the simulation box.
// This is only relevant when periodic boundary conditions are not applied 
// (i.e., if stationary_mode = 0).
int DeleteParticle(Particle *p_i, int index_i) {
    
    bool delete_it = false;
        
    if (p_i->r[0] > Box[0]) {
            
        delete_it = true;
    }
    
    if(delete_it) {
        for(int d = 0; d < dim; d++) {
            
            // Particles currently not in the simulation are parked at this position
            p_i->r[d] = 0.;
            if(d == 0) {
                p_i->r[d] -= 2.;
            }
        }

        push(waitingParticles, index_i);     // See lists.c
        removeByValue(partInSim, index_i);     // See lists.c
    }
    
    return delete_it;
}

// ----------------------------------------------------------------------------------------

// Checks for each particle, if it moved outside the simulation box and if so, deletes it. 
// Returns true if a at least one particle was deleted.
int CheckForDeletion(Particle *P) {
    
    bool part_was_deleted = false;
    
    Node *current = partInSim->next;
    while (current != NULL) {
        
        Node *tmp = current->next;        
        int i = current->value;
        
        bool part_was_deleted_temp = DeleteParticle(&(P[i]), i);     // See partmanagement.c
        
        if(part_was_deleted_temp && !part_was_deleted) {
            part_was_deleted = true;
        }
        
        current = tmp;
    }
    
    return part_was_deleted;
}

// ----------------------------------------------------------------------------------------

// Checks for unphysical movements. If an error is found, the simulation stops.
void CheckStability(Particle *P) {
    
    double dr;
    
    Node *current = partInSim->next;
    while (current != NULL) {
        
        int i = current->value;
    
        dr = 0.;
        for (int d = 0; d < dim; d++) {
            if(stationary_mode && d < 2) {
                dr += pow(PeriodicDis(P[i].r[d] - P[i].r_old[d], Box[d]), 2.);     // See distance.c
            } else {
                dr += pow(P[i].r[d] - P[i].r_old[d], 2.);
            }
        }
        
        dr = sqrt(dr);
        
        if (dr > 4. * rcut_ver) {
            printf("ERROR: Stability problem. (System exploded)\n");
            printf("Particle %i moved from %f\t%f\t%f to %f\t%f\t%f\n", i, 
                    P[i].r_old[0], P[i].r_old[1], P[i].r_old[2], P[i].r[0], P[i].r[1], P[i].r[2]);
            exit(1);
        }
        
        current = current->next;    
    }    
}

// ----------------------------------------------------------------------------------------
