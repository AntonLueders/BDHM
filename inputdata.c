#include "inputdata.h"

// ----------------------------------------------------------------------------------------

// Prints inputfile if "Setup" is used.
void PrintInputfile() {

    printf("Write intput file using the default values. Please wait...\n");

    printf("\n");

    FILE *file = fopen("inputfile", "w");
    fprintf(file, "Geometry\t%f\t%d\t%d\t%f\n", distance_poles, num_poles[0], num_poles[1], Box[2]);
    fprintf(file, "InitBox\t%f\t%f\t%f\n", init_box[0], init_box[1], init_box[2]);
    fprintf(file, "N\t%d\n", N);
    fprintf(file, "dt\t%1.12f\n", dt);
    fprintf(file, "StepNumber\t%d\t%d\n", steps, stepsrate);
    fprintf(file, "Magnetics\t%f\t%f\t%f\t%f\t%f\n", B_field_strength, H[0], H[1], H[2], H_freq);
    fprintf(file, "HiMode\t%d\n", hi_mode);
    fprintf(file, "VerletList\t%f\n", rcut_ver);
    fprintf(file, "Seed\t%ld\n", seed);
    fprintf(file, "Flow\t%f\n", v_flow_max);
    fprintf(file, "ParticleManagement\t%d\t%d\n", N_start, N_rate);
    fprintf(file, "StationaryMode\t%d\n", stationary_mode);
    fprintf(file, "CalcVelocity\t%d\t%d\n", calc_velocity, goal_pole);
    fprintf(file, "CalcParticlesPerPole\t%d\t%d\n", calc_particles_per_pole, pole_array_step);

    fclose(file);
}

// ----------------------------------------------------------------------------------------

// Reads inputfile. First, the "temp" variables are generated. If reading the input
// was successful, the global variables are set to the temp values.
void ReadInput() {

    // If there is no inputfile, an error occures.
    FILE *input = fopen("inputfile", "r");
    if (input == NULL) {
        printf("ERROR: No input found. Use Setup option.\n");

        printf("\n");

        exit(1);
    }

    // Temp variables
    char dummy[40] = " ";
    char temp[40] = " ";
    double distance_poles_temp = distance_poles;
    int num_poles_temp[2] = {num_poles[0], num_poles[1]};
    double BoxZ_temp = Box[2];
    double init_box_temp[3] = {init_box[0], init_box[1], init_box[2]};
    int N_temp = N;
    double dt_temp = dt;
    int steps_temp = steps;
    int stepsrate_temp = stepsrate;
    double B_field_strength_temp = B_field_strength;
    double H_temp[3] = {H[0], H[1], H[2]};
    double H_freq_temp = H_freq;
    int hi_mode_temp = hi_mode;
    double rcut_ver_temp = rcut_ver;
    long seed_temp = seed;
    double v_flow_max_temp = v_flow_max;
    int N_start_temp = N_start;
    int N_rate_temp = N_rate;
    int stationary_mode_temp = stationary_mode;
    int calc_velocity_temp = calc_velocity;
    int goal_pole_temp = goal_pole;
    int calc_particles_per_pole_temp = calc_particles_per_pole;
    int pole_array_step_temp = pole_array_step;

    // Reading the inputfile
    if (fscanf(input, "%s\t%lf\t%d\t%d\t%lf%[^\n]\n", temp, &distance_poles_temp, &num_poles_temp[0], &num_poles_temp[1], &BoxZ_temp,
               dummy) == 5) {
        distance_poles = distance_poles_temp;
        for (int d = 0; d < 2; d++) {
            num_poles[d] = num_poles_temp[d];
        }
        for(int d = 0; d < 2; d++) {
            Box[d] = (double)num_poles[d] * distance_poles;
        }
        Box[2] = BoxZ_temp;
    } else {
        printf("ERROR: Check input or use Setup option. (Geometry)\n");
        exit(1);
    }
    if (fscanf(input, "%s\t%lf\t%lf\t%lf%[^\n]\n", temp, &init_box_temp[0], &init_box_temp[1], &init_box_temp[2],
               dummy) == 4) {
        for (int d = 0; d < dim; d++) {
            init_box[d] = init_box_temp[d];
        }
    } else {
        printf("ERROR: Check input or use Setup option. (InitBox)\n");
        exit(1);
    }
    if (fscanf(input, "%s\t%d%[^\n]\n", temp, &N_temp, dummy) == 2) {
        N = N_temp;
    } else {
        printf("ERROR: Check input or use Setup option. (N)\n");
        exit(1);
    }
    if (fscanf(input, "%s\t%lf%[^\n]\n", temp, &dt_temp, dummy) == 2) {
        dt = dt_temp;
    } else {
        printf("ERROR: Check input or use Setup option. (dt)\n");
        exit(1);
    }
    if (fscanf(input, "%s\t%d\t%d%[^\n]\n", temp, &steps_temp, &stepsrate_temp,
               dummy) == 3) {
        steps = steps_temp;
        stepsrate = stepsrate_temp;
    } else {
        printf("ERROR: Check input or use Setup option. (StepNumber)\n");
        exit(1);
    }
    if (fscanf(input, "%s\t%lf\t%lf\t%lf\t%lf\t%lf%[^\n]\n", temp, &B_field_strength_temp, &H_temp[0], &H_temp[1], &H_temp[2], &H_freq_temp,
               dummy) == 6) {
        B_field_strength = B_field_strength_temp;
        for (int d = 0; d < 3; d++) {
            H[d] = H_temp[d];
        }
        H_freq = H_freq_temp;
    } else {
        printf("ERROR: Check input or use Setup option. (Magnetics)\n");
        exit(1);
    }
    if (fscanf(input, "%s\t%d%[^\n]\n", temp, &hi_mode_temp, dummy) == 2) {
        hi_mode = hi_mode_temp;
    } else {
        printf("ERROR: Check input or use Setup option. (HiMode)\n");
        exit(1);
    }
    if (fscanf(input, "%s\t%lf%[^\n]\n", temp, &rcut_ver_temp, dummy) == 2) {
        rcut_ver = rcut_ver_temp;
    } else {
        printf("ERROR: Check input or use Setup option. (VerletList)\n");
        exit(1);
    }
    if (fscanf(input, "%s\t%ld%[^\n]\n", temp, &seed_temp, dummy) == 2) {
        seed = seed_temp;
    } else {
        printf("ERROR: Check input or use Setup option. (Seed)\n");
        exit(1);
    }
    if (fscanf(input, "%s\t%lf%[^\n]\n", temp, &v_flow_max_temp, dummy) == 2) {
        v_flow_max = v_flow_max_temp;
    } else {
        printf("ERROR: Check input or use Setup option. (Flow)\n");
        exit(1);
    }
    if (fscanf(input, "%s\t%d\t%d%[^\n]\n", temp, &N_start_temp, &N_rate_temp,
               dummy) == 3) {
        N_start = N_start_temp;
        N_rate = N_rate_temp;
    } else {
        printf("ERROR: Check input or use Setup option. (ParticleManagement)\n");
        exit(1);
    }
    if (fscanf(input, "%s\t%d%[^\n]\n", temp, &stationary_mode_temp, dummy) == 2) {
        stationary_mode = stationary_mode_temp;
    } else {
        printf("ERROR: Check input or use Setup option. (StationaryMode)\n");
        exit(1);
    }
    if (fscanf(input, "%s\t%d\t%d%[^\n]\n", temp, &calc_velocity_temp, &goal_pole_temp,
               dummy) == 3) {
        calc_velocity = calc_velocity_temp;
        goal_pole = goal_pole_temp;
    } else {
        printf("ERROR: Check input or use Setup option. (CalcVelocity)\n");
        exit(1);
    }
    if (fscanf(input, "%s\t%d\t%d%[^\n]\n", temp, &calc_particles_per_pole_temp, &pole_array_step_temp, dummy) == 3) {
        calc_particles_per_pole = calc_particles_per_pole_temp;
        pole_array_step = pole_array_step_temp;
    } else {
        printf("ERROR: Check input or use Setup option. (CalcParticlesPerPole)\n");
        exit(1);
    }

    fclose(input);
}

// ----------------------------------------------------------------------------------------

// Checks for conflicting input
void CheckForConflict() {
    
    if (init_box[0] - Box[0] >= pow(10., -10.) || init_box[1] - Box[1] >= pow(10., -10.) || init_box[2] - Box[2] >= pow(10., -10.)) {
        printf("ERROR: InitBox larger than simulation box.\n");
        exit(1);
    }
    
    if (hi_mode < 0 || hi_mode > 2) {
        printf("ERROR: Unknown HiMode.\n");
        exit(1);        
    }
    
    if (fabs(sqrt(H[0] * H[0] + H[1] * H[1] + H[2] * H[2]) - 1.) >= pow(10., -10.)) {
        printf("ERROR: H direction not normalized.\n");
        exit(1);    
    }
    
    if (N_start > N) {
        printf("ERROR: N_start larger than N.\n");
        exit(1);
    }
    
    if (!stationary_mode && calc_velocity) {
        printf("ERROR: Velocity calculation only defined for StationaryMode.\n");
        exit(1);
    }
    
    if (calc_velocity && (goal_pole + 3 > num_poles[1] || goal_pole <= start_pole)) {
        printf("ERROR: Goal does not exist.\n");
        exit(1);
    }
    
    if (!stationary_mode && calc_particles_per_pole) {
        printf("ERROR: ParticlePerPole calculation only defined for StationaryMode.\n");
        exit(1);
    }
    
    if (calc_particles_per_pole && pole_array_step > steps) {
        printf("ERROR: Pole array step larger than total number of steps.\n");
        exit(1);
    }
}

// ----------------------------------------------------------------------------------------