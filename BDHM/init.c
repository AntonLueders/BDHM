#include "init.h"

// ----------------------------------------------------------------------------------------

// Initializes the simulation
void InitRandom(Particle *P, Particle *Poles, Particle *Wall) {

// ----------------------------------------------------------------------------------------
//  Initializes the particles
// ----------------------------------------------------------------------------------------    

    long counter_overlap = 0;
    
    for (int i = 0; i < N; i++) {    
    
        if(counter_overlap > (long)N_start * (long)N_start) {
            printf("ERROR: System too dense.\n");
            exit(1);
        }
        
        CalcMomentPart(&(P[i]));     // See magnetics.c
        
        for(int d = 0; d < dim; d++) {
            P[i].r_old[d] = 0.;
            P[i].f_mag[d] = 0.;
            P[i].f_ste[d] = 0.;
            P[i].f[d] = 0.;
        }
        
        P[i].sigma = 1.;
        P[i].D = 1.;
        
        P[i].verList_ste = malloc(sizeof(Node));
        P[i].verList_ste->value = i;
        P[i].verList_ste->next = NULL;
        
        P[i].verList_mag = malloc(sizeof(Node));
        P[i].verList_mag->value = i;
        P[i].verList_mag->next = NULL;
        
        // In stationary_mode, no new particle spawn and all particle are in the 
        // simulation box from the beginning
        int N_limit = 0;
        if(stationary_mode) {
            N_limit = N;
        } else {
            N_limit = N_start;
        }
        if (i < N_limit) {
            for (int d = 0; d < dim; d++) {
                
                P[i].r[d] = init_box[d] * gsl_rng_uniform(generator);
                
                // All particles get a safety distance from the walls at t = 0
                if (d == 2) {
                    if (P[i].r[d] < 0.75) {
                        P[i].r[d] += 0.75;
                    } else if (!stationary_mode && P[i].r[d] > Box[2] - 0.5) {
                        P[i].r[d] -= 0.5;
                    }
                }
            }
            
            // Avoids particle overlaps at t = 0
            for (int j = 0; j < i; j++) {
                double dij[3];
                double dissq = 0.;
                
                dissq = Distance(&(P[i]), &(P[j]), dij);     // See distance.c
                if(sqrt(dissq) < (P[i].sigma + P[j].sigma) / 2.) {
                    free(P[i].verList_ste);
                    free(P[i].verList_mag);
                    i--;
                    counter_overlap++;
                    break;
                }
            }
            
        // Particles that are not in the simulation box at t = 0 waiting parked at this position
        // Only relevant for stationary_mode = 0        
        } else {
            
            for(int d = 0; d < dim; d++) {
                P[i].r[d] = 0.;
                if (d == 0) {
                    P[i].r[d] -= 2.;
                }
            }
        }
    }
    
// ----------------------------------------------------------------------------------------
//  Initializes the micromagnets
// ----------------------------------------------------------------------------------------    
    
    int counter = 0;
    for(int i = 0; i < num_poles[0]; i++) {
        for(int j = 0; j < num_poles[1]; j++) {
            
            Poles[counter].r[0] = distance_poles / 2. + (double)i * distance_poles;
            Poles[counter].r[1] = distance_poles / 2. + (double)j * distance_poles;
            Poles[counter].r[2] = - pos_poles_z;
            
            CalcMomentPole(&(Poles[counter]));     // See magnetics.c
            
            Poles[counter].verList_mag = malloc(sizeof(Node));
            Poles[counter].verList_mag->value = counter;
            Poles[counter].verList_mag->next = NULL;
            
            counter++;
        }
    }
    
// ----------------------------------------------------------------------------------------
//  Initializes the walls
// ----------------------------------------------------------------------------------------    
    
    for(int k = 0; k < 3; k++) {
        

        for(int d = 0; d < dim; d++) {
            Wall[k].r[d] = 0.;
            Wall[k].m[d] = 0.;
        }
        
        Wall[k].sigma = 1.;
    
        Wall[k].verList_ste = malloc(sizeof(Node));
        Wall[k].verList_ste->value = 0;
        Wall[k].verList_ste->next = NULL;
    }
    
    // Positioning the walls for maximum stability
    Wall[0].r[2] = -0.35;
    Wall[1].r[2] = Box[2] + 0.65;
    Wall[2].r[1] = Box[1] + distance_poles + 0.5;
    
    // Marks orientation of the walls, is needed for the calculation of
    // the force. See force.c
    Wall[0].m[2] = 1.;
    Wall[1].m[2] = 1.;
    Wall[2].m[1] = 1.;

    printf("\n");
    printf("Init succesfull.\n");
    printf("\n");
}

// ----------------------------------------------------------------------------------------
