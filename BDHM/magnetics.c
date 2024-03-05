#include "magnetics.h"

// ----------------------------------------------------------------------------------------

// Rotates a vector. Using a two-dimensional rotation matrix,
// vec is rotated in respect to the angle phi and saved in new_vec
void Rotation_Matrix(double phi, double vec[2], double new_vec[2]) {

    double Rot_Matrix[3][3];
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(i == j) {
                Rot_Matrix[i][j] = 1.;
            } else {
                Rot_Matrix[i][j] = 0.;
            }
        }
    }

    Rot_Matrix[1][1] = cos(phi);
    Rot_Matrix[1][2] = -sin(phi);
    Rot_Matrix[2][1] = sin(phi);
    Rot_Matrix[2][2] = cos(phi);

    for (int d = 0; d < dim; d++) {
        new_vec[d] = 0.;
    }

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            new_vec[i] += Rot_Matrix[i][j] * vec[j];
        }
    }
}

// ----------------------------------------------------------------------------------------

// Calculates the magentic anisotropy (demagnetization factors) of the ellipsoidal micromagnets. 
// These quantities enter the calculation of the magnetic susceptibility tensor of the micromagnets.
// See, for instance, IEEE Transactions on Robotics, vol. 23, no. 6, pp. 1247-1252.
void CalcAnisotropiePoles() {
    
    double na = 1. / (q * q - 1.) * (q / (2. * sqrt(q * q - 1.)) * log((q + sqrt(q * q - 1.)) 
                / (q - sqrt(q * q - 1.))) - 1.); 
    double nr = (1. - na) / 2.;
    
    for (int d = 0; d < 2; d++) {
        n[d] = nr;
    }
    n[2] = na;
    
    printf("Anisotropie of poles:\t%f\t%f\t%f\n", n[0], n[1], n[2]);
}

// ----------------------------------------------------------------------------------------

// Calculates the strength of the magnetic dipole interactions
double CalcFDipol() {

    // First factor summerizes all fixed factors contained in F0 of the magentic dipol 
    // interactions. Second factor is the magentic flux density in mT. Third factor deals
    // with the m in mT. Fourth factor transforms N in k_BT/sigma.
    double F_0 = 3.19042969 * pow(10., -9.) * pow(B_field_strength, 2.) * pow(10., -6.) 
                 * (2.47073956 * pow(10., 15.));
    
    printf("F0 of Dipol force:\t%f\n", F_0);
    
    return F_0;
}

// ----------------------------------------------------------------------------------------

// Calculates the magnetic moments of the superparamagnetic particles.
void CalcMomentPart(Particle * p_i) {
    
    for (int d = 0; d < dim; d++) {
        p_i->m[d] = H[d];
    }
}

// ----------------------------------------------------------------------------------------

// Calculates the magnetic moments of the micromagnets. 
void CalcMomentPole(Particle * pole_i) {
    
    for (int d = 0; d < dim; d++) {
        pole_i->m[d] = moment_const_poles * chi_poles / (1. + chi_poles * n[d]) * H[d];
    }
}

// ----------------------------------------------------------------------------------------

// Rotates the magnetic field during each simulation step.
void RotateH(double freq) {
    
    double new_H[3];
    Rotation_Matrix(-2. * M_PI * freq * dt, H, new_H);     // See magentics.c
    
    double H_norm = 0.0;
    for(int d = 0; d < dim; d++) {
        H[d] = new_H[d];
        H_norm += pow(H[d], 2.0);
    }

    // Just to make sure that there is no drift in the magnitude of the magnetic field
    for(int d = 0; d < dim; d++) {
        H[d] = H[d] / sqrt(H_norm);
    }
}

// ----------------------------------------------------------------------------------------
