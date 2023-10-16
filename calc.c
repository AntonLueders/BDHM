#include "calc.h"

// ----------------------------------------------------------------------------------------

// Checks if particles reached the finish line
int CalcVelocity(Particle *P) {
    
    int count_particle_in_goal = 0;
    
    Node *current = partInSim->next;
    while (current != NULL) {
        
        int i = current->value;
        
        // Second condition accounts for errors resulting from periodic boundaries
        if (P[i].r[1] > distance_poles / 2. + (double)goal_pole * distance_poles 
                  && P[i].r[1] < distance_poles / 2. + (double)(goal_pole + 3) * distance_poles) {
            
            count_particle_in_goal++;
        }
        
        // "front_number" is the the number of particle needed to pass the line
        // for the function to register that the finish line is reached
        if (count_particle_in_goal >= front_number) {
            return 1;
        }
            
        current = current->next;
    }

    return 0;
}

// ----------------------------------------------------------------------------------------

// Checks if particles have reached the start line
int CalcVelocityStart(Particle *P) {
    
    int count_particle_at_start = 0;
    
    Node *current = partInSim->next;
    while (current != NULL) {
        
        int i = current->value;

        // Second condition accounts for errors resulting from periodic boundaries        
        if (P[i].r[1] > distance_poles / 2. + (double)start_pole * distance_poles 
                  && P[i].r[1] < distance_poles / 2. + (double)(start_pole + 3) * distance_poles) {
            
            count_particle_at_start++;
        }
        
        // "front_number" is the the number of particle needed to pass the line
        // for the function to register that the start line is reached
        if (count_particle_at_start >= front_number) {
            return 1;
        }
            
        current = current->next;
    }

    return 0;
}

// ----------------------------------------------------------------------------------------

// Calculates and prints velocity of particles
void PrintVelocity(int step) {
    
    printf("Particle velocity:\t%f\n", ((double)goal_pole - (double)start_pole) * distance_poles / ((double)step * dt));
}

// ----------------------------------------------------------------------------------------

// Calculates for each micromagnet the number of particles in its vicinity 
void CalcPoleArray(Particle *P, int pole_array[num_poles[0]][num_poles[1]]) {
    
    for(int i = 0; i < num_poles[0]; i++) {
        for(int j = 0; j < num_poles[1]; j++) {
            pole_array[i][j] = 0;
        }
    }
    
    Node *current = partInSim->next;
    while (current != NULL) {
        
        int i = current->value;
        
        int pol_x = (int)floor(P[i].r[0] / distance_poles);
        int pol_y = (int)floor(P[i].r[1] / distance_poles);
        if (pol_x < num_poles[0] && pol_y < num_poles[1]) {
            pole_array[pol_x][pol_y]++;
        }
        
        current = current->next;
    }
}

// ----------------------------------------------------------------------------------------

// Calculates the average number of particles per occupied micromagnet
double CalcParticlesPerPole(Particle *P) {
    
    int pole_array[num_poles[0]][num_poles[1]];
    
    CalcPoleArray(P, pole_array);     // See calc.c
    
    int num_poles_with_particles = 0;
    int mean_particle = 0;
    
    for(int i = 0; i < num_poles[0]; i++) {
        for(int j = 0; j < num_poles[1]; j++) {
            
            if(pole_array[i][j] > 0) {
                num_poles_with_particles++;
                mean_particle += pole_array[i][j];
            }
        }
    }
    
    if (num_poles_with_particles == 0) {
        
        return 0.;
    } else {
        
        return (double)mean_particle / (double)num_poles_with_particles;
    }
}

// ----------------------------------------------------------------------------------------

void PrintParticlesPerPole(double time, double result) {
    
    FILE *data_file = NULL;
    data_file = fopen("ParticlesPerPole.dat", "a");
    
    fprintf(data_file, "%f\t%f\n", time, result);
    
    fclose(data_file);
}

// ----------------------------------------------------------------------------------------

void PrintPoleArray(Particle *P) {
    
    FILE *data_file = NULL;
    data_file = fopen("PoleArray.dat", "w");
    
    int pole_array[num_poles[0]][num_poles[1]];
    
    CalcPoleArray(P, pole_array);     // See calc.c
    
    for(int i = 0; i < num_poles[1]; i++) {
        for(int j = 0; j < num_poles[0]; j++) {
            
            fprintf(data_file, "%d\t", pole_array[j][i]);
        }
        fprintf(data_file, "\n");
    }
    
    fclose(data_file);
    
    printf("Array of pole occupation printed successfully.\n");
}

// ----------------------------------------------------------------------------------------