#include "bda.h"

// ----------------------------------------------------------------------------------------

// "Brownian dynamics algorithm" with HIs and no noise. 
void Move(Particle *P, double D_Tensor[N][N][3][3]) {
	
// ----------------------------------------------------------------------------------------
// No HIs
// ----------------------------------------------------------------------------------------	
	
	if (hi_mode == 0) {

		Node *current = partInSim->next;
		while (current != NULL) {
		
			int i = current->value;
			
			double v_flow[3] = {0., 0., 0.};
			if (!stationary_mode) {
				PoiseulleFlow(&(P[i]), v_flow);     // See flow.c
			}

			for(int d = 0; d < dim; d++) {
				P[i].r[d] += P[i].D * P[i].f[d] * dt + v_flow[d] * dt;
				// The d < 2 conditions results from the wall
				if (stationary_mode && d < 2) {
					P[i].r[d] = PeriodicPos(P[i].r[d], Box[d]);     // See distance.c
				}
			}
			
			current = current->next;
		}

// ----------------------------------------------------------------------------------------
// HIs only for magnetic interactions (used in paper)
// ----------------------------------------------------------------------------------------	
	
	} else if (hi_mode == 1) {
		
		Node *current = partInSim->next;
		while (current != NULL) {
		
			int i = current->value;
			
			double v_flow[3] = {0., 0., 0.};
			if (!stationary_mode) {
				PoiseulleFlow(&(P[i]), v_flow);     // See flow.c
			}
			
			double Delta_r[3] = {0., 0., 0.};
			
			Node *current_j = partInSim->next;
			while (current_j != NULL) {
		
				int j = current_j->value;
				
				for(int d = 0; d < 3; d++) {
					for(int k = 0; k < 3; k++) {
						Delta_r[d] += D_Tensor[i][j][d][k] * P[j].f_mag[k] * dt;
					}
				}
				
				current_j = current_j->next;
			}

			for(int d = 0; d < dim; d++) {
				P[i].r[d] += Delta_r[d] + P[i].D * P[i].f_ste[d] * dt + v_flow[d] * dt;
				// The d < 2 conditions results from the wall
				if (stationary_mode && d < 2) {
					P[i].r[d] = PeriodicPos(P[i].r[d], Box[d]);     // See distance.c
				}
			}
			
			current = current->next;
		}
		
// ----------------------------------------------------------------------------------------
//  Full HIs
// ----------------------------------------------------------------------------------------			
	
	} else if (hi_mode == 2)  {
		
		Node *current = partInSim->next;
		while (current != NULL) {
		
			int i = current->value;
			
			double v_flow[3] = {0., 0., 0.};
			if(!stationary_mode) {
				PoiseulleFlow(&(P[i]), v_flow);     // See flow.c
			}
			
			double Delta_r[3] = {0., 0., 0.};
			
			Node *current_j = partInSim->next;
			while (current_j != NULL) {
		
				int j = current_j->value;
		
				for(int d = 0; d < 3; d++) {
					for(int k = 0; k < 3; k++) {
						Delta_r[d] += D_Tensor[i][j][d][k] * P[j].f[k] * dt;
					}
				}
				
				current_j = current_j->next;
			}

			for(int d = 0; d < dim; d++) {
				P[i].r[d] += Delta_r[d] + v_flow[d] * dt;
				// The d < 2 conditions results from the wall
				if (stationary_mode && d < 2) {
					P[i].r[d] = PeriodicPos(P[i].r[d], Box[d]);     // See distance.c
				}
			}
			
			current = current->next;
		}
		
	}
}

// ----------------------------------------------------------------------------------------