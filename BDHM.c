#include "BDHM.h"
#include "inputdata.h"
#include "distance.h"
#include "init.h"
#include "printdata.h"
#include "hi.h"
#include "bda.h"
#include "force.h"
#include "magnetics.h"
#include "verletlist.h"
#include "printdisplay.h"
#include "flow.h"
#include "partmanagement.h"
#include "calc.h"

// ----------------------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------------------	

double dim = DIM;                                           // Dimension of the system, i.e., 3
double Box[DIM] = {BOXX, BOXY, BOXZ};                       // Box lengths of the simulation box
double init_box[DIM] = {INIT_BOXX, INIT_BOXY, INIT_BOXZ};   // Random inital position are in this box
int N = NP;                                                 // (Maximum) number of particle
double dt = DT;                                             // Length of simulation step
int steps = STEPS;                                          // Number of steps
int stepsrate = STEPSRATE;                                  // Position are printed every "steprate" steps
long seed = SEED;                                           // Random seed
double eps_wca = EPS_WCA;                                   // Interaction strength for WCA particle interactions
double eps_wall = EPS_WALL;                                 // Interaction strength for wall-particle interactions
double B_field_strength = B_FIELD_STRENGTH;                 // Magnitude of B field
double F_dipol_0 = F_DIPOL_0;                               // Strength of dipole-dipole interaction
double H[DIM] = {HX, HY, HZ};                               // Direction of the magnetic field
double H_freq = H_FREQ;                                     // Rotation frequence of the magnetic field
double n[DIM] = {NR, NR, NA};                               // Magnetic anisotropy of the micromagnets
double q = Q;                                               // Aspect ratio micromagnets
double width_poles = WIDTH_POLES;                           // Minor axis of ellipsoidal micromagnets
double pos_poles_z = POS_POLES_Z;                           // Center position of micromagnets
double distance_poles = DISTANCE_POLES;                     // Lattice constant of the micromagnets grid
double chi_poles = CHI_POLES;                               // Susceptibility of the micromagnets
double moment_const_poles = MOMENT_CONST_POLES;             // "Magnitude" of the micomagnet moment relative to particle moment
int num_poles[2] = {NUM_POLESX, NUM_POLESY};                // Number of micromagnets
int hi_mode = HI_MODE;                                      // Accuracy of hydrodynamic calculations
double dr_ver_max1 = 0.;                                    // For Verlet lists: one of the current max. distances
double dr_ver_max2 = 0.;                                    // For Verlet lists: one of the current max. distances
double rcut_ver = RCUT_VER;                                 // Cut-off radius for Verlet lists (WCA particle interactions)
double rcut_dipol_part = RCUT_DIPOL_PART;                   // Cut-off radius for Verlet lists (Magnetic partice interactions)
double rcut_dipol_poles = RCUT_DIPOL_POLES;                 // Cut-off radius for Verlet lists (Micromagnet-particle interactions)
double v_flow_max = V_FLOW_MAX;                             // Maximum velocity of the parabolic Poiseuille flow
int N_start = N_START;                                      // Number of particles at t = 0
int N_rate = N_RATE;                                        // A new particle is added after "N_rate" steps
bool stationary_mode = STATIONARY_MODE;                     // If true: no flow, only one wall, periodic boundaries
bool calc_velocity = CALC_VELOCITY;                         // Add Velocity calculation
int start_pole = START_POLE;                                // Start line for velocity calculation
int goal_pole = GOAL_POLE;                                  // Finish line for velocity calculation
int front_number = FRONT_NUMBER;                            // Number of particles that have to pass the lines (velocity calculation)
int calc_particles_per_pole = CALC_PARTICLES_PER_POLE;      // Calculations of the mean number of particle per occupied micomagnet
int pole_array_step = POLE_ARRAY_STEP;                      // After "pole_array_step" steps, a list of micomagnet occupation is printed
Node *partInSim = NULL;                                     // All particles currently in the simulation box
Node *waitingParticles = NULL;                              // All particles currently not in the simulation box
gsl_rng *generator = NULL;                                  // Random number generator
// ----------------------------------------------------------------------------------------
// Start of program
// ----------------------------------------------------------------------------------------	

int main(int argc, char *argv[]) {
	
	// For the calculation of the total simulation time. 
    time_t start;
    time(&start);
	
	PrintWelcome();
	
	// The programm only runs with "inputfile" in the same folder. 
	// A new inputfile can be generated with "./BDHM3.exe Setup"
	if (argc == 2 && !strcmp(argv[1], "Setup")) {

        // Generates an example input file which can be customized. 
        PrintInputfile();     // See inputdata.c

        time_t now;
        time(&now);

        // Total simulation time.
        long simTime = now - start;
        printf("Duration of the simulation: %ld s\n", simTime);

        return 0;
    }
	
    ReadInput();     // See inputdata.c
	
	CheckForConflict();     // See inputdata.c
	
	// Prints input parameter in console.
	PrintInput();     // See printdisplay.c
	
	CalcAnisotropiePoles();     // See magnetics.c
	F_dipol_0 = CalcFDipol();     // See magnetics.c

	Particle P[N];
	
	// Diffusion tensor for hydrodynamic calculations.
	double (*D_Tensor)[N][3][3];
	D_Tensor = malloc(N * sizeof(*D_Tensor));
	
	Particle Poles[num_poles[0] * num_poles[1]];
	
	Particle Wall[3];
	
	InitPartLists();     // partmanagement.c
	
	bool part_was_deleted = false;
	bool part_was_added = false;
	
	bool start_calc_velocity = false;
	int start_time_calc_velocity = 0;
	
	// See GSL documentation for more infos regarding the generator.
	// GLS has to be linked for random numbers!
    generator = gsl_rng_alloc(gsl_rng_ranlxs0);
    printf("Random number generator algorithm: gsl_rng_ranlxs0\n");
	gsl_rng_set(generator, seed);
	
	// Initializes the simulations
	InitRandom(P, Poles, Wall);     // See init.c
	
	FILE *position_file = NULL;
	position_file = fopen("Position_For_VMD.xyz", "w");
	
	// Initializes diffusion tensor if no HIs are applied to avoid undefined behavior
	if (hi_mode == 0) {
		CalcDTensor(P, D_Tensor);     // See hi.c
	}
	
	long counter_ver = 0;
	
// ----------------------------------------------------------------------------------------
// Main simulation loop
// ----------------------------------------------------------------------------------------	
	
	for(int step = 0; step < steps; step++) {
		
		if ((step + 1) % stepsrate == 0) {
			// Prints position (.xyz file)
            Print(position_file, P, Poles, step);     // See printdata.c 
			if (calc_particles_per_pole) {
				PrintParticlesPerPole(step * dt, CalcParticlesPerPole(P));     // See calc.c
			}
        }
		
		if(calc_particles_per_pole && step + 1 == pole_array_step) {
			PrintPoleArray(P);     // See calc.c
		}
		
		if ((step + 1) % N_rate == 0 && !stationary_mode) {
			part_was_added = AddParticle(P);      // See partmanagement.c
		}
		
		// Checks if Verlet lists have to be updated. 
		if (CheckVerlet(P) || step == 0 || part_was_added || part_was_deleted) {
			CalcForce(P, Poles, Wall);     // See force.c
			// Counts how often Verlet lists were updated during the simulation
			counter_ver++;
			if(part_was_deleted) {
				part_was_deleted = false;
			}
			if(part_was_added) {
			  part_was_added = false;
			}
		} else {
			CalcForceWithVerlet(P, Poles, Wall);     // See force.c
		}
		
		if (hi_mode > 0) {
			CalcDTensor(P, D_Tensor);     // See hi.c
		}
		
		if (stationary_mode && calc_velocity) {
			if(!start_calc_velocity) {
				// Checks if particle reached the start line
				start_calc_velocity = CalcVelocityStart(P);     // See calc.c
				start_time_calc_velocity = step;
			}
			if(start_calc_velocity) {
				// Checks if particle reached the finish line
				if (CalcVelocity(P)) {     // See calc.c
					PrintVelocity(step - start_time_calc_velocity);     // See calc.c
					break;
				}
			}
		}
		
		Move(P,D_Tensor);     // See bda.c
		
		if(!stationary_mode) {
			part_was_deleted = CheckForDeltetion(P);     // See partmanagement.c
		}
		
		// Checks for unphysical movements
		CheckStability(P);     // See partmanagement.c

		RotateH(H_freq);     // See magnetics.c
	}
	
	printf("Verlet-lists were updated %ld times.\n", counter_ver);

	gsl_rng_free(generator);
	fclose(position_file);
	
	FreeVerLists(P, Poles, Wall);     // See verletlist.c
	FreePartLists();     // See verletlist.c
	
	free(D_Tensor);
	
	// Obtains the calender time at the end of the simulation.
    time_t now;
    time(&now);

    // Calculating the duration of the simulation
    long simTime = now - start;
    printf("Duration of the simulation: %ld s\n", simTime);
	
    // End of simulation
    return 0;
}

// ----------------------------------------------------------------------------------------
