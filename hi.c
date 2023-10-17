#include "hi.h"

// ----------------------------------------------------------------------------------------

// Writes Oseen tensor corresponding to "dissq" and "dij" in "Oij".
void CalcOseenTensor(double dissq, double dij[3], double Oij[3][3]) {
    
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            Oij[i][j] = 0.;
        }
    }
    
    double rijrij[3][3];
    double unity[3][3];
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            rijrij[i][j] = dij[i] * dij[j] / dissq;
            if(i == j) {
                unity[i][j] = 1.;
            } else {
                unity[i][j] = 0.;
            }
        }
    }
    
    double sigma = 1.;
    
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            Oij[i][j] = 3. * sigma / (8. * sqrt(dissq)) * (unity[i][j] + rijrij[i][j]); 
        }
    }
}

// ----------------------------------------------------------------------------------------

// Writes the wall correction part of the Blake tensor in "delta_Oij".
// See, for example, Phys. Chem. Chem. Phys., 1999,1, 2131-2139.
void CalcDeltaOseen(Particle *p_i, Particle *p_j, double delta_Oij[3][3]) {
    
    double ri[3] = {0., 0., 0.};
    double rj[3] = {0., 0., 0.};
    
    double dij[3] = {0., 0., 0.};
    
    // Mirror particle for Blake tensor
    Particle Mirror = *p_j;
    Mirror.r[2] = - Mirror.r[2];
    
    double dissq = Distance(p_i, &Mirror, dij);     // See distance.c
    
    double s = sqrt(dissq);
    
    for(int d = 0; d < 3; d++) {
        ri[d] = p_i->r[d];
        rj[d] = p_j->r[d];
    }
    
    // In the stationary_mode (i.e., without Poiseuille flow), periodic boundary conditions are applied. 
    // if: with Poiseuille flow
    // else: without Poiseuille flow (periodic boundaries)
    if (!stationary_mode) {
        
        delta_Oij[0][0] = -2. * ri[2] * rj[2] * (1. / pow(s, 3.) - 3. * pow(ri[0] - rj[0], 2.) / pow(s, 5.));
        delta_Oij[1][1] = -2. * ri[2] * rj[2] * (1. / pow(s, 3.) - 3. * pow(ri[1] - rj[1], 2.) / pow(s, 5.));
        delta_Oij[2][2] = 2. * ri[2] * rj[2] * (1. / pow(s, 3.) - 3. * pow(ri[2] + rj[2], 2.) / pow(s, 5.));
        delta_Oij[0][1] = 6. * ri[2] * rj[2] * (ri[0] - rj[0]) * (ri[1] - rj[1]) / pow(s, 5.);
        delta_Oij[1][0] = delta_Oij[0][1];
        delta_Oij[0][2] = 2. * (ri[0] - rj[0]) * (rj[2] / pow(s, 3.) - 3. * ri[2] * rj[2] * (ri[2] + rj[2]) / pow(s, 5.));
        delta_Oij[2][0] = 2. * (ri[0] - rj[0]) * (rj[2] / pow(s, 3.) + 3. * ri[2] * rj[2] * (ri[2] + rj[2]) / pow(s, 5.));
        delta_Oij[1][2] = 2. * (ri[1] - rj[1]) * (rj[2] / pow(s, 3.) - 3. * ri[2] * rj[2] * (ri[2] + rj[2]) / pow(s, 5.));
        delta_Oij[2][1] = 2. * (ri[1] - rj[1]) * (rj[2] / pow(s, 3.) + 3. * ri[2] * rj[2] * (ri[2] + rj[2]) / pow(s, 5.));
        
    } else {
        
        // See distance.c for "PeriodicDis(...)"
        delta_Oij[0][0] = -2. * ri[2] * rj[2] * (1. / pow(s, 3.) - 3. * pow(PeriodicDis(ri[0] - rj[0], Box[0]), 2.) / pow(s, 5.));
        delta_Oij[1][1] = -2. * ri[2] * rj[2] * (1. / pow(s, 3.) - 3. * pow(PeriodicDis(ri[1] - rj[1], Box[1]), 2.) / pow(s, 5.));
        delta_Oij[2][2] = 2. * ri[2] * rj[2] * (1. / pow(s, 3.) - 3. * pow(ri[2] + rj[2], 2.) / pow(s, 5.));
        delta_Oij[0][1] = 6. * ri[2] * rj[2] * PeriodicDis(ri[0] - rj[0], Box[0]) * PeriodicDis(ri[1] - rj[1], Box[1]) / pow(s, 5.);
        delta_Oij[1][0] = delta_Oij[0][1];
        delta_Oij[0][2] = 2. * PeriodicDis(ri[0] - rj[0], Box[0]) * (rj[2] / pow(s, 3.) - 3. * ri[2] * rj[2] * (ri[2] + rj[2]) / pow(s, 5.));
        delta_Oij[2][0] = 2. * PeriodicDis(ri[0] - rj[0], Box[0]) * (rj[2] / pow(s, 3.) + 3. * ri[2] * rj[2] * (ri[2] + rj[2]) / pow(s, 5.));
        delta_Oij[1][2] = 2. * PeriodicDis(ri[1] - rj[1], Box[1]) * (rj[2] / pow(s, 3.) - 3. * ri[2] * rj[2] * (ri[2] + rj[2]) / pow(s, 5.));
        delta_Oij[2][1] = 2. * PeriodicDis(ri[1] - rj[1], Box[1]) * (rj[2] / pow(s, 3.) + 3. * ri[2] * rj[2] * (ri[2] + rj[2]) / pow(s, 5.));
    }
    
    double sigma = 1.;
    
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            delta_Oij[i][j] = 3. * sigma / 8. * delta_Oij[i][j];
        }
    }
}

// ----------------------------------------------------------------------------------------

// Writes for two particles the corresponding Blake tensor in "Dij". 
// See, for example, Phys. Chem. Chem. Phys., 1999,1, 2131-2139.
void CalcDij(int index_i, int index_j, Particle *p_i, Particle *p_j, double Dij[3][3]) {

    // unity matrix
    double mat_unity[3][3];
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(i == j) {
                mat_unity[i][j] = 1.;
            } else {
                mat_unity[i][j] = 0.;
            }
        }
    }
    
// ----------------------------------------------------------------------------------------
//  Single particle interaction with sourounding dispersion agent and wall
// ----------------------------------------------------------------------------------------    
    
    if (index_i == index_j) {
        
        double Oij_mirror[3][3];
        double delta_Oij[3][3];
        double dij[3];
        double dissq = 0.;
        
        Particle Mirror = *p_j;
        Mirror.r[2] = - Mirror.r[2];
    
        dissq = Distance(p_i, &Mirror, dij);     // See distance.c        
        CalcOseenTensor(dissq, dij, Oij_mirror);     // See hi.c
        
        CalcDeltaOseen(p_i, p_i, delta_Oij);     // See hi.c
        
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                Dij[i][j] = p_i->D * mat_unity[i][j] - Oij_mirror[i][j] + delta_Oij[i][j];
            }
        }
        
// ----------------------------------------------------------------------------------------
//  HIs between two (different) particles
// ----------------------------------------------------------------------------------------        
        
    } else {
        
        double Oij[3][3];
        double Oij_mirror[3][3];
        double delta_Oij[3][3];
        double dij[3];
        double dissq = 0.;
        
        // See distance.c
        dissq = Distance(p_i, p_j, dij);     // See distance.c
        CalcOseenTensor(dissq, dij, Oij);     // See hi.c
        
        Particle Mirror = *p_j;
        Mirror.r[2] = - Mirror.r[2];
        
        dissq = Distance(p_i, &(Mirror), dij);     // See distance.c
        CalcOseenTensor(dissq, dij, Oij_mirror);     // See hi.c
        
        CalcDeltaOseen(p_i, p_j, delta_Oij);     // See hi.c
        
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                Dij[i][j] = Oij[i][j] - Oij_mirror[i][j] + delta_Oij[i][j];
            }
        }
    }
}

// ----------------------------------------------------------------------------------------

// Calculates the complete N particle diffusion tensor containing the HIs.
void CalcDTensor(Particle *P, double D_Tensor[N][N][3][3]) {
    
    double Dij[3][3];
    Node *current = partInSim->next;
    while (current != NULL) {
        
        int i = current->value;
        
        Node *current_j = partInSim->next;
        while (current_j != NULL) {
        
            int j = current_j->value;
            
            CalcDij(i, j, &(P[i]), &(P[j]), Dij);     // See hi.c
            for(int d1 = 0; d1 < 3; d1++) {
                for(int d2 = 0; d2 < 3; d2++) {
                    D_Tensor[i][j][d1][d2] = Dij[d1][d2];
                }
            }
            
            current_j = current_j->next;
        }        
        current = current->next;
    }
}

// ----------------------------------------------------------------------------------------