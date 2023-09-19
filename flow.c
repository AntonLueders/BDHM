#include "flow.h"

// ----------------------------------------------------------------------------------------

// Parabolic Poiseulle flow between two walls. 
// Not relevant in the stationary_mode. 
void PoiseulleFlow(Particle *p_i, double v_flow[3]) {
	
	for(int d = 1; d < 3; d++) {
		v_flow[d] = 0.;
	}
	v_flow[0] = v_flow_max * (1. - pow(p_i->r[2] - Box[2] / 2., 2.) / pow(Box[2] / 2., 2.));
	
	if (v_flow[0] < 0.) {
		printf("ERROR: Stability problem. (Particle behind wall)\n");
        exit(1);
	}
}

// ----------------------------------------------------------------------------------------